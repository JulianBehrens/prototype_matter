/*******************************************************************************************************************
Copyright (c) 2023 Cycling '74

The code that Max generates automatically and that end users are capable of
exporting and using, and any associated documentation files (the “Software”)
is a work of authorship for which Cycling '74 is the author and owner for
copyright purposes.

This Software is dual-licensed either under the terms of the Cycling '74
License for Max-Generated Code for Export, or alternatively under the terms
of the General Public License (GPL) Version 3. You may use the Software
according to either of these licenses as it is most appropriate for your
project on a case-by-case basis (proprietary or not).

A) Cycling '74 License for Max-Generated Code for Export

A license is hereby granted, free of charge, to any person obtaining a copy
of the Software (“Licensee”) to use, copy, modify, merge, publish, and
distribute copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The Software is licensed to Licensee for all uses that do not include the sale,
sublicensing, or commercial distribution of software that incorporates this
source code. This means that the Licensee is free to use this software for
educational, research, and prototyping purposes, to create musical or other
creative works with software that incorporates this source code, or any other
use that does not constitute selling software that makes use of this source
code. Commercial distribution also includes the packaging of free software with
other paid software, hardware, or software-provided commercial services.

For entities with UNDER $200k in annual revenue or funding, a license is hereby
granted, free of charge, for the sale, sublicensing, or commercial distribution
of software that incorporates this source code, for as long as the entity's
annual revenue remains below $200k annual revenue or funding.

For entities with OVER $200k in annual revenue or funding interested in the
sale, sublicensing, or commercial distribution of software that incorporates
this source code, please send inquiries to licensing@cycling74.com.

The above copyright notice and this license shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Please see
https://support.cycling74.com/hc/en-us/articles/10730637742483-RNBO-Export-Licensing-FAQ
for additional information

B) General Public License Version 3 (GPLv3)
Details of the GPLv3 license can be found at: https://www.gnu.org/licenses/gpl-3.0.html
*******************************************************************************************************************/

#include "RNBO_Common.h"
#include "RNBO_AudioSignal.h"

namespace RNBO {


#define trunc(x) ((Int)(x))

#if defined(__GNUC__) || defined(__clang__)
    #define RNBO_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define RNBO_RESTRICT __restrict
#endif

#define FIXEDSIZEARRAYINIT(...) { }

class granular : public PatcherInterfaceImpl {
public:

granular()
{
}

~granular()
{
}

granular* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
}

template <typename T> void listquicksort(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    if (l < h) {
        Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
        this->listquicksort(arr, sortindices, l, p - 1, ascending);
        this->listquicksort(arr, sortindices, p + 1, h, ascending);
    }
}

template <typename T> Int listpartition(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    number x = arr[(Index)h];
    Int i = (Int)(l - 1);

    for (Int j = (Int)(l); j <= h - 1; j++) {
        bool asc = (bool)((bool)(ascending) && arr[(Index)j] <= x);
        bool desc = (bool)((bool)(!(bool)(ascending)) && arr[(Index)j] >= x);

        if ((bool)(asc) || (bool)(desc)) {
            i++;
            this->listswapelements(arr, i, j);
            this->listswapelements(sortindices, i, j);
        }
    }

    i++;
    this->listswapelements(arr, i, h);
    this->listswapelements(sortindices, i, h);
    return i;
}

template <typename T> void listswapelements(T& arr, Int a, Int b) {
    auto tmp = arr[(Index)a];
    arr[(Index)a] = arr[(Index)b];
    arr[(Index)b] = tmp;
}

inline number linearinterp(number frac, number x, number y) {
    return x + (y - x) * frac;
}

inline number cubicinterp(number a, number w, number x, number y, number z) {
    number a2 = a * a;
    number f0 = z - y - w + x;
    number f1 = w - x - f0;
    number f2 = y - w;
    number f3 = x;
    return f0 * a * a2 + f1 * a2 + f2 * a + f3;
}

inline number splineinterp(number a, number w, number x, number y, number z) {
    number a2 = a * a;
    number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
    number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
    number f2 = -0.5 * w + 0.5 * y;
    return f0 * a * a2 + f1 * a2 + f2 * a + x;
}

inline number cosT8(number r) {
    number t84 = 56.0;
    number t83 = 1680.0;
    number t82 = 20160.0;
    number t81 = 2.4801587302e-05;
    number t73 = 42.0;
    number t72 = 840.0;
    number t71 = 1.9841269841e-04;

    if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
        number rr = r * r;
        return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
    } else if (r > 0.0) {
        r -= 1.57079632679489661923132169163975144;
        number rr = r * r;
        return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
    } else {
        r += 1.57079632679489661923132169163975144;
        number rr = r * r;
        return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
    }
}

inline number cosineinterp(number frac, number x, number y) {
    number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
    return x * (1.0 - a2) + y * a2;
}

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
}

number samplerate() {
    return this->sr;
}

Index vectorsize() {
    return this->vs;
}

number maximum(number x, number y) {
    return (x < y ? y : x);
}

Index voice() {
    return this->_voiceIndex;
}

number random(number low, number high) {
    number range = high - low;
    return rand01() * range + low;
}

inline number safemod(number f, number m) {
    if (m != 0) {
        Int f_trunc = (Int)(trunc(f));
        Int m_trunc = (Int)(trunc(m));

        if (f == f_trunc && m == m_trunc) {
            f = f_trunc % m_trunc;
        } else {
            if (m < 0) {
                m = -m;
            }

            if (f >= m) {
                if (f >= m * 2.0) {
                    number d = f / m;
                    Int i = (Int)(trunc(d));
                    d = d - i;
                    f = d * m;
                } else {
                    f -= m;
                }
            } else if (f <= -m) {
                if (f <= -m * 2.0) {
                    number d = f / m;
                    Int i = (Int)(trunc(d));
                    d = d - i;
                    f = d * m;
                } else {
                    f += m;
                }
            }
        }
    } else {
        f = 0.0;
    }

    return f;
}

number wrap(number x, number low, number high) {
    number lo;
    number hi;

    if (low == high)
        return low;

    if (low > high) {
        hi = low;
        lo = high;
    } else {
        lo = low;
        hi = high;
    }

    number range = hi - lo;

    if (x >= lo && x < hi)
        return x;

    if (range <= 0.000000001)
        return lo;

    long numWraps = (long)(trunc((x - lo) / range));
    numWraps = numWraps - ((x < lo ? 1 : 0));
    number result = x - range * numWraps;

    if (result >= hi)
        return result - range;
    else
        return result;
}

template <typename T> inline array<SampleValue, 1 + 1> peek_default(T& buffer, SampleValue sampleIndex, Index channelOffset) {
    number bufferSize = buffer->getSize();
    const Index bufferChannels = (const Index)(buffer->getChannels());
    constexpr int ___N2 = 1 + 1;
    array<SampleValue, ___N2> out = FIXEDSIZEARRAYINIT(1 + 1);

    if (bufferSize == 0 || (5 == 5 && (sampleIndex < 0 || sampleIndex >= bufferSize))) {
        return out;
    } else {
        for (Index channel = 0; channel < 1; channel++) {
            Index channelIndex = (Index)(channel + channelOffset);

            {
                if (channelIndex >= bufferChannels || channelIndex < 0) {
                    out[(Index)channel] = 0;
                    continue;
                }
            }

            SampleValue bufferreadsample_result;

            {
                auto& __buffer = buffer;

                if (sampleIndex < 0.0) {
                    bufferreadsample_result = 0.0;
                }

                SampleIndex index1 = (SampleIndex)(trunc(sampleIndex));

                {
                    {
                        {
                            {
                                bufferreadsample_result = __buffer->getSample(channelIndex, index1);
                            }
                        }
                    }
                }
            }

            out[(Index)channel] = bufferreadsample_result;
        }

        out[1] = sampleIndex;
        return out;
    }
}

template <typename T> inline void poke_default(
    T& buffer,
    SampleValue value,
    SampleValue sampleIndex,
    int channel,
    number overdub
) {
    number bufferSize = buffer->getSize();
    const Index bufferChannels = (const Index)(buffer->getChannels());

    if (bufferSize > 0 && (5 != 5 || (sampleIndex >= 0 && sampleIndex < bufferSize)) && (5 != 5 || (channel >= 0 && channel < bufferChannels))) {
        if (overdub != 0) {
            number currentValue = buffer->getSample(channel, sampleIndex);

            {
                value = value * (1. - overdub) + currentValue * overdub;
            }
        }

        buffer->setSample(channel, sampleIndex, value);
        buffer->setTouched(true);
    }
}

MillisecondTime currenttime() {
    return this->_currentTime;
}

number tempo() {
    return this->getTopLevelPatcher()->globaltransport_getTempo(this->currenttime());
}

number mstobeats(number ms) {
    return ms * this->tempo() * 0.008 / (number)480;
}

MillisecondTime sampstoms(number samps) {
    return samps * 1000 / this->sr;
}

Index getNumMidiInputPorts() const {
    return 0;
}

void processMidiEvent(MillisecondTime , int , ConstByteArray , Index ) {}

Index getNumMidiOutputPorts() const {
    return 0;
}

void process(
    const SampleValue * const* inputs,
    Index numInputs,
    SampleValue * const* outputs,
    Index numOutputs,
    Index n
) {
    this->vs = n;
    this->updateTime(this->getEngine()->getCurrentTime());
    SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
    SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);

    this->gen_01_perform(
        in1,
        in2,
        this->gen_01_gain,
        this->gen_01_mix,
        this->gen_01_pitch,
        this->gen_01_width,
        this->gen_01_grainPos,
        this->gen_01_grainSize,
        this->gen_01_interval,
        out1,
        out2,
        n
    );

    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        this->globaltransport_tempo = resizeSignal(this->globaltransport_tempo, this->maxvs, maxBlockSize);
        this->globaltransport_state = resizeSignal(this->globaltransport_state, this->maxvs, maxBlockSize);
        this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
        this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
        this->didAllocateSignals = true;
    }

    const bool sampleRateChanged = sampleRate != this->sr;
    const bool maxvsChanged = maxBlockSize != this->maxvs;
    const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;

    if (sampleRateChanged || maxvsChanged) {
        this->vs = maxBlockSize;
        this->maxvs = maxBlockSize;
        this->sr = sampleRate;
        this->invsr = 1 / sampleRate;
    }

    this->gen_01_dspsetup(forceDSPSetup);
    this->globaltransport_dspsetup(forceDSPSetup);

    if (sampleRateChanged)
        this->onSampleRateChanged(sampleRate);
}

void setProbingTarget(MessageTag id) {
    switch (id) {
    default:
        {
        this->setProbingIndex(-1);
        break;
        }
    }
}

void setProbingIndex(ProbingIndex ) {}

Index getProbingChannels(MessageTag outletId) const {
    RNBO_UNUSED(outletId);
    return 0;
}

DataRef* getDataRef(DataRefIndex index)  {
    switch (index) {
    case 0:
        {
        return addressOf(this->data_param);
        break;
        }
    case 1:
        {
        return addressOf(this->gen_01_del_inR_bufferobj);
        break;
        }
    case 2:
        {
        return addressOf(this->gen_01_del_inL_bufferobj);
        break;
        }
    default:
        {
        return nullptr;
        }
    }
}

DataRefIndex getNumDataRefs() const {
    return 3;
}

void fillDataRef(DataRefIndex , DataRef& ) {}

void zeroDataRef(DataRef& ref) {
    ref->setZero();
}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->updateTime(time);

    if (index == 0) {
        this->gen_01_data_param = new Float64Buffer(this->data_param);
    }

    if (index == 1) {
        this->gen_01_del_inR_buffer = new Float64Buffer(this->gen_01_del_inR_bufferobj);
    }

    if (index == 2) {
        this->gen_01_del_inL_buffer = new Float64Buffer(this->gen_01_del_inL_bufferobj);
    }
}

void initialize() {
    this->data_param = initDataRef("data_param", true, nullptr, "buffer~");
    this->gen_01_del_inR_bufferobj = initDataRef("gen_01_del_inR_bufferobj", true, nullptr, "buffer~");
    this->gen_01_del_inL_bufferobj = initDataRef("gen_01_del_inL_bufferobj", true, nullptr, "buffer~");
    this->assign_defaults();
    this->setState();
    this->data_param->setIndex(0);
    this->gen_01_data_param = new Float64Buffer(this->data_param);
    this->gen_01_del_inR_bufferobj->setIndex(1);
    this->gen_01_del_inR_buffer = new Float64Buffer(this->gen_01_del_inR_bufferobj);
    this->gen_01_del_inL_bufferobj->setIndex(2);
    this->gen_01_del_inL_buffer = new Float64Buffer(this->gen_01_del_inL_bufferobj);
    this->initializeObjects();
    this->allocateDataRefs();
    this->startup();
}

Index getIsMuted()  {
    return this->isMuted;
}

void setIsMuted(Index v)  {
    this->isMuted = v;
}

Index getPatcherSerial() const {
    return 0;
}

void getState(PatcherStateInterface& ) {}

void setState() {}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_01_getPresetValue(getSubState(preset, "width"));
    this->param_02_getPresetValue(getSubState(preset, "gain"));
    this->param_03_getPresetValue(getSubState(preset, "mix"));
    this->param_04_getPresetValue(getSubState(preset, "pitch"));
    this->param_05_getPresetValue(getSubState(preset, "grainPos"));
    this->param_06_getPresetValue(getSubState(preset, "grainSize"));
    this->param_07_getPresetValue(getSubState(preset, "interval"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_01_setPresetValue(getSubState(preset, "width"));
    this->param_02_setPresetValue(getSubState(preset, "gain"));
    this->param_03_setPresetValue(getSubState(preset, "mix"));
    this->param_04_setPresetValue(getSubState(preset, "pitch"));
    this->param_05_setPresetValue(getSubState(preset, "grainPos"));
    this->param_06_setPresetValue(getSubState(preset, "grainSize"));
    this->param_07_setPresetValue(getSubState(preset, "interval"));
}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(this->_currentTime, tempo, false))
        {}
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(this->_currentTime, state, false))
        {}
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(this->_currentTime, beattime, false))
        {}
}

void onSampleRateChanged(double ) {}

void processTimeSignatureEvent(MillisecondTime time, int numerator, int denominator) {
    this->updateTime(time);

    if (this->globaltransport_setTimeSignature(this->_currentTime, numerator, denominator, false))
        {}
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    this->updateTime(time);

    switch (index) {
    case 0:
        {
        this->param_01_value_set(v);
        break;
        }
    case 1:
        {
        this->param_02_value_set(v);
        break;
        }
    case 2:
        {
        this->param_03_value_set(v);
        break;
        }
    case 3:
        {
        this->param_04_value_set(v);
        break;
        }
    case 4:
        {
        this->param_05_value_set(v);
        break;
        }
    case 5:
        {
        this->param_06_value_set(v);
        break;
        }
    case 6:
        {
        this->param_07_value_set(v);
        break;
        }
    }
}

void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValue(index, value, time);
}

void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValueNormalized(index, value, time);
}

ParameterValue getParameterValue(ParameterIndex index)  {
    switch (index) {
    case 0:
        {
        return this->param_01_value;
        }
    case 1:
        {
        return this->param_02_value;
        }
    case 2:
        {
        return this->param_03_value;
        }
    case 3:
        {
        return this->param_04_value;
        }
    case 4:
        {
        return this->param_05_value;
        }
    case 5:
        {
        return this->param_06_value;
        }
    case 6:
        {
        return this->param_07_value;
        }
    default:
        {
        return 0;
        }
    }
}

ParameterIndex getNumSignalInParameters() const {
    return 0;
}

ParameterIndex getNumSignalOutParameters() const {
    return 0;
}

ParameterIndex getNumParameters() const {
    return 7;
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "width";
        }
    case 1:
        {
        return "gain";
        }
    case 2:
        {
        return "mix";
        }
    case 3:
        {
        return "pitch";
        }
    case 4:
        {
        return "grainPos";
        }
    case 5:
        {
        return "grainSize";
        }
    case 6:
        {
        return "interval";
        }
    default:
        {
        return "bogus";
        }
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "width";
        }
    case 1:
        {
        return "gain";
        }
    case 2:
        {
        return "mix";
        }
    case 3:
        {
        return "pitch";
        }
    case 4:
        {
        return "grainPos";
        }
    case 5:
        {
        return "grainSize";
        }
    case 6:
        {
        return "interval";
        }
    default:
        {
        return "bogus";
        }
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        case 0:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 50;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 1:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = -36;
            info->max = 12;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 2:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 50;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 3:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = -12;
            info->max = 12;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 4:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 100;
            info->min = 10;
            info->max = 500;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 5:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 100;
            info->min = 10;
            info->max = 500;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 6:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 100;
            info->min = 10;
            info->max = 500;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
    if (steps == 1) {
        if (normalizedValue > 0) {
            normalizedValue = 1.;
        }
    } else {
        ParameterValue oneStep = (number)1. / (steps - 1);
        ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
        normalizedValue = numberOfSteps * oneStep;
    }

    return normalizedValue;
}

ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 0:
    case 2:
        {
        {
            value = (value < 0 ? 0 : (value > 100 ? 100 : value));
            ParameterValue normalizedValue = (value - 0) / (100 - 0);
            return normalizedValue;
        }
        }
    case 4:
    case 5:
    case 6:
        {
        {
            value = (value < 10 ? 10 : (value > 500 ? 500 : value));
            ParameterValue normalizedValue = (value - 10) / (500 - 10);
            return normalizedValue;
        }
        }
    case 1:
        {
        {
            value = (value < -36 ? -36 : (value > 12 ? 12 : value));
            ParameterValue normalizedValue = (value - -36) / (12 - -36);
            return normalizedValue;
        }
        }
    case 3:
        {
        {
            value = (value < -12 ? -12 : (value > 12 ? 12 : value));
            ParameterValue normalizedValue = (value - -12) / (12 - -12);
            return normalizedValue;
        }
        }
    default:
        {
        return value;
        }
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 0:
    case 2:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (100 - 0);
            }
        }
        }
    case 4:
    case 5:
    case 6:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 10 + value * (500 - 10);
            }
        }
        }
    case 1:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return -36 + value * (12 - -36);
            }
        }
        }
    case 3:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return -12 + value * (12 - -12);
            }
        }
        }
    default:
        {
        return value;
        }
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 0:
        {
        return this->param_01_value_constrain(value);
        }
    case 1:
        {
        return this->param_02_value_constrain(value);
        }
    case 2:
        {
        return this->param_03_value_constrain(value);
        }
    case 3:
        {
        return this->param_04_value_constrain(value);
        }
    case 4:
        {
        return this->param_05_value_constrain(value);
        }
    case 5:
        {
        return this->param_06_value_constrain(value);
        }
    case 6:
        {
        return this->param_07_value_constrain(value);
        }
    default:
        {
        return value;
        }
    }
}

void scheduleParamInit(ParameterIndex index, Index order) {
    this->paramInitIndices->push(index);
    this->paramInitOrder->push(order);
}

void processParamInitEvents() {
    this->listquicksort(
        this->paramInitOrder,
        this->paramInitIndices,
        0,
        (int)(this->paramInitOrder->length - 1),
        true
    );

    for (Index i = 0; i < this->paramInitOrder->length; i++) {
        this->getEngine()->scheduleParameterChange(
            this->paramInitIndices[i],
            this->getParameterValue(this->paramInitIndices[i]),
            0
        );
    }
}

void processClockEvent(MillisecondTime , ClockId , bool , ParameterValue ) {}

void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}

void processOutletEvent(
    EngineLink* sender,
    OutletIndex index,
    ParameterValue value,
    MillisecondTime time
) {
    this->updateTime(time);
    this->processOutletAtCurrentTime(sender, index, value);
}

void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}

void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}

void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {

    }

    return "";
}

MessageIndex getNumMessages() const {
    return 0;
}

const MessageInfo& getMessageInfo(MessageIndex index) const {
    switch (index) {

    }

    return NullMessageInfo;
}

protected:

void param_01_value_set(number v) {
    v = this->param_01_value_constrain(v);
    this->param_01_value = v;
    this->sendParameter(0, false);

    if (this->param_01_value != this->param_01_lastValue) {
        this->getEngine()->presetTouched();
        this->param_01_lastValue = this->param_01_value;
    }

    this->expr_02_in1_set(v);
}

void param_02_value_set(number v) {
    v = this->param_02_value_constrain(v);
    this->param_02_value = v;
    this->sendParameter(1, false);

    if (this->param_02_value != this->param_02_lastValue) {
        this->getEngine()->presetTouched();
        this->param_02_lastValue = this->param_02_value;
    }

    this->expr_03_in1_set(v);
}

void param_03_value_set(number v) {
    v = this->param_03_value_constrain(v);
    this->param_03_value = v;
    this->sendParameter(2, false);

    if (this->param_03_value != this->param_03_lastValue) {
        this->getEngine()->presetTouched();
        this->param_03_lastValue = this->param_03_value;
    }

    this->expr_01_in1_set(v);
}

void param_04_value_set(number v) {
    v = this->param_04_value_constrain(v);
    this->param_04_value = v;
    this->sendParameter(3, false);

    if (this->param_04_value != this->param_04_lastValue) {
        this->getEngine()->presetTouched();
        this->param_04_lastValue = this->param_04_value;
    }

    this->gen_01_pitch_set(v);
}

void param_05_value_set(number v) {
    v = this->param_05_value_constrain(v);
    this->param_05_value = v;
    this->sendParameter(4, false);

    if (this->param_05_value != this->param_05_lastValue) {
        this->getEngine()->presetTouched();
        this->param_05_lastValue = this->param_05_value;
    }

    this->gen_01_grainPos_set(v);
}

void param_06_value_set(number v) {
    v = this->param_06_value_constrain(v);
    this->param_06_value = v;
    this->sendParameter(5, false);

    if (this->param_06_value != this->param_06_lastValue) {
        this->getEngine()->presetTouched();
        this->param_06_lastValue = this->param_06_value;
    }

    this->gen_01_grainSize_set(v);
}

void param_07_value_set(number v) {
    v = this->param_07_value_constrain(v);
    this->param_07_value = v;
    this->sendParameter(6, false);

    if (this->param_07_value != this->param_07_lastValue) {
        this->getEngine()->presetTouched();
        this->param_07_lastValue = this->param_07_value;
    }

    this->gen_01_interval_set(v);
}

number msToSamps(MillisecondTime ms, number sampleRate) {
    return ms * sampleRate * 0.001;
}

MillisecondTime sampsToMs(SampleIndex samps) {
    return samps * (this->invsr * 1000);
}

Index getMaxBlockSize() const {
    return this->maxvs;
}

number getSampleRate() const {
    return this->sr;
}

bool hasFixedVectorSize() const {
    return false;
}

Index getNumInputChannels() const {
    return 2;
}

Index getNumOutputChannels() const {
    return 2;
}

void allocateDataRefs() {
    this->gen_01_data_param->requestSize(6, 100);
    this->gen_01_data_param->setSampleRate(this->sr);
    this->gen_01_data_param = this->gen_01_data_param->allocateIfNeeded();

    if (this->data_param->hasRequestedSize()) {
        if (this->data_param->wantsFill())
            this->zeroDataRef(this->data_param);

        this->getEngine()->sendDataRefUpdated(0);
    }

    this->gen_01_del_inR_buffer = this->gen_01_del_inR_buffer->allocateIfNeeded();

    if (this->gen_01_del_inR_bufferobj->hasRequestedSize()) {
        if (this->gen_01_del_inR_bufferobj->wantsFill())
            this->zeroDataRef(this->gen_01_del_inR_bufferobj);

        this->getEngine()->sendDataRefUpdated(1);
    }

    this->gen_01_del_inL_buffer = this->gen_01_del_inL_buffer->allocateIfNeeded();

    if (this->gen_01_del_inL_bufferobj->hasRequestedSize()) {
        if (this->gen_01_del_inL_bufferobj->wantsFill())
            this->zeroDataRef(this->gen_01_del_inL_bufferobj);

        this->getEngine()->sendDataRefUpdated(2);
    }
}

void initializeObjects() {
    this->gen_01_his_gain_init();
    this->gen_01_his_mix_init();
    this->gen_01_his_index_init();
    this->gen_01_del_inR_init();
    this->gen_01_del_inL_init();
    this->gen_01_counter_0_init();
    this->gen_01_noise_4_init();
    this->gen_01_noise_7_init();
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());

    {
        this->scheduleParamInit(0, 0);
    }

    {
        this->scheduleParamInit(1, 0);
    }

    {
        this->scheduleParamInit(2, 0);
    }

    {
        this->scheduleParamInit(3, 0);
    }

    {
        this->scheduleParamInit(4, 0);
    }

    {
        this->scheduleParamInit(5, 0);
    }

    {
        this->scheduleParamInit(6, 0);
    }

    this->processParamInitEvents();
}

static number param_01_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_width_set(number v) {
    this->gen_01_width = v;
}

void expr_02_out1_set(number v) {
    this->expr_02_out1 = v;
    this->gen_01_width_set(this->expr_02_out1);
}

void expr_02_in1_set(number in1) {
    this->expr_02_in1 = in1;
    this->expr_02_out1_set(this->expr_02_in1 * this->expr_02_in2);//#map:*_obj-21:1
}

static number param_02_value_constrain(number v) {
    v = (v > 12 ? 12 : (v < -36 ? -36 : v));
    return v;
}

void gen_01_gain_set(number v) {
    this->gen_01_gain = v;
}

void expr_03_out1_set(number v) {
    this->expr_03_out1 = v;
    this->gen_01_gain_set(this->expr_03_out1);
}

void expr_03_in1_set(number in1) {
    this->expr_03_in1 = in1;
    this->expr_03_out1_set(rnbo_pow(10, this->expr_03_in1 * 0.05));//#map:dbtoa_obj-15:1
}

static number param_03_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void gen_01_mix_set(number v) {
    this->gen_01_mix = v;
}

void expr_01_out1_set(number v) {
    this->expr_01_out1 = v;
    this->gen_01_mix_set(this->expr_01_out1);
}

void expr_01_in1_set(number in1) {
    this->expr_01_in1 = in1;
    this->expr_01_out1_set(this->expr_01_in1 * this->expr_01_in2);//#map:*_obj-22:1
}

static number param_04_value_constrain(number v) {
    v = (v > 12 ? 12 : (v < -12 ? -12 : v));
    return v;
}

void gen_01_pitch_set(number v) {
    this->gen_01_pitch = v;
}

static number param_05_value_constrain(number v) {
    v = (v > 500 ? 500 : (v < 10 ? 10 : v));
    return v;
}

void gen_01_grainPos_set(number v) {
    this->gen_01_grainPos = v;
}

static number param_06_value_constrain(number v) {
    v = (v > 500 ? 500 : (v < 10 ? 10 : v));
    return v;
}

void gen_01_grainSize_set(number v) {
    this->gen_01_grainSize = v;
}

static number param_07_value_constrain(number v) {
    v = (v > 500 ? 500 : (v < 10 ? 10 : v));
    return v;
}

void gen_01_interval_set(number v) {
    this->gen_01_interval = v;
}

void gen_01_perform(
    const Sample * in1,
    const Sample * in2,
    number gain,
    number mix,
    number pitch,
    number width,
    number grainPos,
    number grainSize,
    number interval,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    auto __gen_01_his_index_value = this->gen_01_his_index_value;
    auto __gen_01_his_gain_value = this->gen_01_his_gain_value;
    auto __gen_01_his_mix_value = this->gen_01_his_mix_value;
    Index i0;

    for (i0 = 0; i0 < n; i0++) {
        __gen_01_his_mix_value = (mix - __gen_01_his_mix_value) * 0.001 + __gen_01_his_mix_value;
        __gen_01_his_gain_value = (gain - __gen_01_his_gain_value) * 0.001 + __gen_01_his_gain_value;
        this->gen_01_del_inL_write(in1[(Index)i0]);
        this->gen_01_del_inR_write(in2[(Index)i0]);
        number mCout = 0;
        array<number, 3> result_1 = this->gen_01_counter_0_next(1, 0, this->mstosamps(interval));
        mCout = result_1[0];

        if (mCout == 1) {
            number peek_1 = 0;
            number peek_2 = 0;
            auto result_2 = this->peek_default(this->gen_01_data_param, 0, __gen_01_his_index_value);
            peek_2 = result_2[1];
            peek_1 = result_2[0];

            if (peek_1 == 0) {
                this->poke_default(this->gen_01_data_param, 1, 0, __gen_01_his_index_value, 0);

                this->poke_default(
                    this->gen_01_data_param,
                    this->mstosamps(grainSize),
                    1,
                    __gen_01_his_index_value,
                    0
                );

                auto sizeScaled_3 = this->mstosamps(grainSize);
                number widthRand_5 = this->gen_01_noise_4_next() * 0.5 * width;
                this->poke_default(this->gen_01_data_param, 0.5 + widthRand_5, 3, __gen_01_his_index_value, 0);
                this->poke_default(this->gen_01_data_param, 0.5 - widthRand_5, 4, __gen_01_his_index_value, 0);
                number pitchScaled_6 = fixnan(rnbo_pow(2, pitch / (number)12)) - 1;
                this->poke_default(this->gen_01_data_param, pitchScaled_6, 5, __gen_01_his_index_value, 0);
                __gen_01_his_index_value = this->wrap(__gen_01_his_index_value + 1, 0, 100);
                number posScaled_8 = this->mstosamps(grainPos) * rnbo_abs(this->gen_01_noise_7_next());

                this->poke_default(
                    this->gen_01_data_param,
                    posScaled_8 + this->maximum(sizeScaled_3 * pitchScaled_6, 0),
                    2,
                    __gen_01_his_index_value,
                    0
                );
            }
        }

        number outL_9 = 0;
        number outR_10 = 0;
        number totalWin_11 = 0;

        for (number i = 0; i < 100; i = i + 1) {
            number pCout = 0;
            auto result_12 = this->peek_default(this->gen_01_data_param, 0, i);
            pCout = result_12[0];

            if (pCout != 0) {
                number s = 0;
                auto result_13 = this->peek_default(this->gen_01_data_param, 1, i);
                s = result_13[0];

                if (pCout < s) {
                    number pCoutScaled_14 = (s == 0. ? 0. : pCout / s);
                    number win_15 = 0.5 * (rnbo_sin(4.7123889803847 + pCoutScaled_14 * 6.28318530717958647692) + 1);
                    number peek_3 = 0;
                    number peek_4 = 0;
                    auto result_16 = this->peek_default(this->gen_01_data_param, 2, i);
                    peek_4 = result_16[1];
                    peek_3 = result_16[0];
                    number peek_5 = 0;
                    number peek_6 = 0;
                    auto result_17 = this->peek_default(this->gen_01_data_param, 5, i);
                    peek_6 = result_17[1];
                    peek_5 = result_17[0];
                    number outputL_18 = this->gen_01_del_inL_read(peek_3 - pCout * peek_5, 0) * win_15;
                    number peek_7 = 0;
                    number peek_8 = 0;
                    auto result_19 = this->peek_default(this->gen_01_data_param, 2, i);
                    peek_8 = result_19[1];
                    peek_7 = result_19[0];
                    number peek_9 = 0;
                    number peek_10 = 0;
                    auto result_20 = this->peek_default(this->gen_01_data_param, 5, i);
                    peek_10 = result_20[1];
                    peek_9 = result_20[0];
                    number outputR_21 = this->gen_01_del_inR_read(peek_7 - pCout * peek_9, 0) * win_15;
                    number peek_11 = 0;
                    number peek_12 = 0;
                    auto result_22 = this->peek_default(this->gen_01_data_param, 3, i);
                    peek_12 = result_22[1];
                    peek_11 = result_22[0];
                    outL_9 = outL_9 + outputL_18 * peek_11;
                    number peek_13 = 0;
                    number peek_14 = 0;
                    auto result_23 = this->peek_default(this->gen_01_data_param, 4, i);
                    peek_14 = result_23[1];
                    peek_13 = result_23[0];
                    outR_10 = outR_10 + outputR_21 * peek_13;
                    this->poke_default(this->gen_01_data_param, pCout + 1, 0, i, 0);
                    totalWin_11 = totalWin_11 + win_15;
                } else {
                    this->poke_default(this->gen_01_data_param, 0, 0, i, 0);
                }
            }
        }

        number normalise_24 = fixnan(rnbo_pow(
            (this->maximum(totalWin_11, 1) == 0. ? 0. : (number)1 / this->maximum(totalWin_11, 1)),
            0.3
        )) * __gen_01_his_gain_value;

        number expr_15_25 = in1[(Index)i0] + __gen_01_his_mix_value * (outL_9 * normalise_24 - in1[(Index)i0]);
        number expr_16_26 = in2[(Index)i0] + __gen_01_his_mix_value * (outR_10 * normalise_24 - in2[(Index)i0]);
        out1[(Index)i0] = expr_15_25;
        out2[(Index)i0] = expr_16_26;
        this->gen_01_del_inR_step();
        this->gen_01_del_inL_step();
    }

    this->gen_01_his_mix_value = __gen_01_his_mix_value;
    this->gen_01_his_gain_value = __gen_01_his_gain_value;
    this->gen_01_his_index_value = __gen_01_his_index_value;
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

number gen_01_his_gain_getvalue() {
    return this->gen_01_his_gain_value;
}

void gen_01_his_gain_setvalue(number val) {
    this->gen_01_his_gain_value = val;
}

void gen_01_his_gain_reset() {
    this->gen_01_his_gain_value = 0;
}

void gen_01_his_gain_init() {
    this->gen_01_his_gain_value = 0;
}

number gen_01_his_mix_getvalue() {
    return this->gen_01_his_mix_value;
}

void gen_01_his_mix_setvalue(number val) {
    this->gen_01_his_mix_value = val;
}

void gen_01_his_mix_reset() {
    this->gen_01_his_mix_value = 0;
}

void gen_01_his_mix_init() {
    this->gen_01_his_mix_value = 0;
}

number gen_01_his_index_getvalue() {
    return this->gen_01_his_index_value;
}

void gen_01_his_index_setvalue(number val) {
    this->gen_01_his_index_value = val;
}

void gen_01_his_index_reset() {
    this->gen_01_his_index_value = 0;
}

void gen_01_his_index_init() {
    this->gen_01_his_index_value = 0;
}

void gen_01_del_inR_step() {
    this->gen_01_del_inR_reader++;

    if (this->gen_01_del_inR_reader >= (int)(this->gen_01_del_inR_buffer->getSize()))
        this->gen_01_del_inR_reader = 0;
}

number gen_01_del_inR_read(number size, Int interp) {
    RNBO_UNUSED(interp);

    {
        number r = (int)(this->gen_01_del_inR_buffer->getSize()) + this->gen_01_del_inR_reader - ((size > this->gen_01_del_inR__maxdelay ? this->gen_01_del_inR__maxdelay : (size < (this->gen_01_del_inR_reader != this->gen_01_del_inR_writer) ? this->gen_01_del_inR_reader != this->gen_01_del_inR_writer : size)));
        long index1 = (long)(rnbo_floor(r));
        number frac = r - index1;
        long index2 = (long)(index1 + 1);

        return this->linearinterp(frac, this->gen_01_del_inR_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->gen_01_del_inR_wrap))
        ), this->gen_01_del_inR_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->gen_01_del_inR_wrap))
        ));
    }

    number r = (int)(this->gen_01_del_inR_buffer->getSize()) + this->gen_01_del_inR_reader - ((size > this->gen_01_del_inR__maxdelay ? this->gen_01_del_inR__maxdelay : (size < (this->gen_01_del_inR_reader != this->gen_01_del_inR_writer) ? this->gen_01_del_inR_reader != this->gen_01_del_inR_writer : size)));
    long index1 = (long)(rnbo_floor(r));

    return this->gen_01_del_inR_buffer->getSample(
        0,
        (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->gen_01_del_inR_wrap))
    );
}

void gen_01_del_inR_write(number v) {
    this->gen_01_del_inR_writer = this->gen_01_del_inR_reader;
    this->gen_01_del_inR_buffer[(Index)this->gen_01_del_inR_writer] = v;
}

number gen_01_del_inR_next(number v, int size) {
    number effectiveSize = (size == -1 ? this->gen_01_del_inR__maxdelay : size);
    number val = this->gen_01_del_inR_read(effectiveSize, 0);
    this->gen_01_del_inR_write(v);
    this->gen_01_del_inR_step();
    return val;
}

array<Index, 2> gen_01_del_inR_calcSizeInSamples() {
    number sizeInSamples = 0;
    Index allocatedSizeInSamples = 0;

    {
        sizeInSamples = this->gen_01_del_inR_evaluateSizeExpr(this->samplerate(), this->vectorsize());
        this->gen_01_del_inR_sizemode = 0;
    }

    sizeInSamples = rnbo_floor(sizeInSamples);
    sizeInSamples = this->maximum(sizeInSamples, 2);
    allocatedSizeInSamples = (Index)(sizeInSamples);
    allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
    return {sizeInSamples, allocatedSizeInSamples};
}

void gen_01_del_inR_init() {
    auto result = this->gen_01_del_inR_calcSizeInSamples();
    this->gen_01_del_inR__maxdelay = result[0];
    Index requestedSizeInSamples = (Index)(result[1]);
    this->gen_01_del_inR_buffer->requestSize(requestedSizeInSamples, 1);
    this->gen_01_del_inR_wrap = requestedSizeInSamples - 1;
}

void gen_01_del_inR_clear() {
    this->gen_01_del_inR_buffer->setZero();
}

void gen_01_del_inR_reset() {
    auto result = this->gen_01_del_inR_calcSizeInSamples();
    this->gen_01_del_inR__maxdelay = result[0];
    Index allocatedSizeInSamples = (Index)(result[1]);
    this->gen_01_del_inR_buffer->setSize(allocatedSizeInSamples);
    updateDataRef(this, this->gen_01_del_inR_buffer);
    this->gen_01_del_inR_wrap = this->gen_01_del_inR_buffer->getSize() - 1;
    this->gen_01_del_inR_clear();

    if (this->gen_01_del_inR_reader >= this->gen_01_del_inR__maxdelay || this->gen_01_del_inR_writer >= this->gen_01_del_inR__maxdelay) {
        this->gen_01_del_inR_reader = 0;
        this->gen_01_del_inR_writer = 0;
    }
}

void gen_01_del_inR_dspsetup() {
    this->gen_01_del_inR_reset();
}

number gen_01_del_inR_evaluateSizeExpr(number samplerate, number vectorsize) {
    RNBO_UNUSED(vectorsize);
    RNBO_UNUSED(samplerate);
    return this->samplerate() * 2;
}

number gen_01_del_inR_size() {
    return this->gen_01_del_inR__maxdelay;
}

void gen_01_del_inL_step() {
    this->gen_01_del_inL_reader++;

    if (this->gen_01_del_inL_reader >= (int)(this->gen_01_del_inL_buffer->getSize()))
        this->gen_01_del_inL_reader = 0;
}

number gen_01_del_inL_read(number size, Int interp) {
    RNBO_UNUSED(interp);

    {
        number r = (int)(this->gen_01_del_inL_buffer->getSize()) + this->gen_01_del_inL_reader - ((size > this->gen_01_del_inL__maxdelay ? this->gen_01_del_inL__maxdelay : (size < (this->gen_01_del_inL_reader != this->gen_01_del_inL_writer) ? this->gen_01_del_inL_reader != this->gen_01_del_inL_writer : size)));
        long index1 = (long)(rnbo_floor(r));
        number frac = r - index1;
        long index2 = (long)(index1 + 1);

        return this->linearinterp(frac, this->gen_01_del_inL_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->gen_01_del_inL_wrap))
        ), this->gen_01_del_inL_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->gen_01_del_inL_wrap))
        ));
    }

    number r = (int)(this->gen_01_del_inL_buffer->getSize()) + this->gen_01_del_inL_reader - ((size > this->gen_01_del_inL__maxdelay ? this->gen_01_del_inL__maxdelay : (size < (this->gen_01_del_inL_reader != this->gen_01_del_inL_writer) ? this->gen_01_del_inL_reader != this->gen_01_del_inL_writer : size)));
    long index1 = (long)(rnbo_floor(r));

    return this->gen_01_del_inL_buffer->getSample(
        0,
        (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->gen_01_del_inL_wrap))
    );
}

void gen_01_del_inL_write(number v) {
    this->gen_01_del_inL_writer = this->gen_01_del_inL_reader;
    this->gen_01_del_inL_buffer[(Index)this->gen_01_del_inL_writer] = v;
}

number gen_01_del_inL_next(number v, int size) {
    number effectiveSize = (size == -1 ? this->gen_01_del_inL__maxdelay : size);
    number val = this->gen_01_del_inL_read(effectiveSize, 0);
    this->gen_01_del_inL_write(v);
    this->gen_01_del_inL_step();
    return val;
}

array<Index, 2> gen_01_del_inL_calcSizeInSamples() {
    number sizeInSamples = 0;
    Index allocatedSizeInSamples = 0;

    {
        sizeInSamples = this->gen_01_del_inL_evaluateSizeExpr(this->samplerate(), this->vectorsize());
        this->gen_01_del_inL_sizemode = 0;
    }

    sizeInSamples = rnbo_floor(sizeInSamples);
    sizeInSamples = this->maximum(sizeInSamples, 2);
    allocatedSizeInSamples = (Index)(sizeInSamples);
    allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
    return {sizeInSamples, allocatedSizeInSamples};
}

void gen_01_del_inL_init() {
    auto result = this->gen_01_del_inL_calcSizeInSamples();
    this->gen_01_del_inL__maxdelay = result[0];
    Index requestedSizeInSamples = (Index)(result[1]);
    this->gen_01_del_inL_buffer->requestSize(requestedSizeInSamples, 1);
    this->gen_01_del_inL_wrap = requestedSizeInSamples - 1;
}

void gen_01_del_inL_clear() {
    this->gen_01_del_inL_buffer->setZero();
}

void gen_01_del_inL_reset() {
    auto result = this->gen_01_del_inL_calcSizeInSamples();
    this->gen_01_del_inL__maxdelay = result[0];
    Index allocatedSizeInSamples = (Index)(result[1]);
    this->gen_01_del_inL_buffer->setSize(allocatedSizeInSamples);
    updateDataRef(this, this->gen_01_del_inL_buffer);
    this->gen_01_del_inL_wrap = this->gen_01_del_inL_buffer->getSize() - 1;
    this->gen_01_del_inL_clear();

    if (this->gen_01_del_inL_reader >= this->gen_01_del_inL__maxdelay || this->gen_01_del_inL_writer >= this->gen_01_del_inL__maxdelay) {
        this->gen_01_del_inL_reader = 0;
        this->gen_01_del_inL_writer = 0;
    }
}

void gen_01_del_inL_dspsetup() {
    this->gen_01_del_inL_reset();
}

number gen_01_del_inL_evaluateSizeExpr(number samplerate, number vectorsize) {
    RNBO_UNUSED(vectorsize);
    RNBO_UNUSED(samplerate);
    return this->samplerate() * 2;
}

number gen_01_del_inL_size() {
    return this->gen_01_del_inL__maxdelay;
}

array<number, 3> gen_01_counter_0_next(number a, number reset, number limit) {
    RNBO_UNUSED(reset);
    RNBO_UNUSED(a);
    number carry_flag = 0;

    {
        this->gen_01_counter_0_count += 1;

        if (limit != 0) {
            if ((1 > 0 && this->gen_01_counter_0_count >= limit) || (1 < 0 && this->gen_01_counter_0_count <= limit)) {
                this->gen_01_counter_0_count = 0;
                this->gen_01_counter_0_carry += 1;
                carry_flag = 1;
            }
        }
    }

    return {this->gen_01_counter_0_count, carry_flag, this->gen_01_counter_0_carry};
}

void gen_01_counter_0_init() {
    this->gen_01_counter_0_count = 0;
}

void gen_01_counter_0_reset() {
    this->gen_01_counter_0_carry = 0;
    this->gen_01_counter_0_count = 0;
}

void gen_01_noise_4_reset() {
    xoshiro_reset(
        systemticks() + this->voice() + this->random(0, 10000),
        this->gen_01_noise_4_state
    );
}

void gen_01_noise_4_init() {
    this->gen_01_noise_4_reset();
}

void gen_01_noise_4_seed(number v) {
    xoshiro_reset(v, this->gen_01_noise_4_state);
}

number gen_01_noise_4_next() {
    return xoshiro_next(this->gen_01_noise_4_state);
}

void gen_01_noise_7_reset() {
    xoshiro_reset(
        systemticks() + this->voice() + this->random(0, 10000),
        this->gen_01_noise_7_state
    );
}

void gen_01_noise_7_init() {
    this->gen_01_noise_7_reset();
}

void gen_01_noise_7_seed(number v) {
    xoshiro_reset(v, this->gen_01_noise_7_state);
}

number gen_01_noise_7_next() {
    return xoshiro_next(this->gen_01_noise_7_state);
}

void gen_01_dspsetup(bool force) {
    if ((bool)(this->gen_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->gen_01_setupDone = true;
    this->gen_01_del_inR_dspsetup();
    this->gen_01_del_inL_dspsetup();
}

void param_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_01_value;
}

void param_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_01_value_set(preset["value"]);
}

void param_02_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_02_value;
}

void param_02_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_02_value_set(preset["value"]);
}

void param_03_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_03_value;
}

void param_03_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_03_value_set(preset["value"]);
}

void param_04_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_04_value;
}

void param_04_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_04_value_set(preset["value"]);
}

void param_05_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_05_value;
}

void param_05_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_05_value_set(preset["value"]);
}

void param_06_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_06_value;
}

void param_06_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_06_value_set(preset["value"]);
}

void param_07_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_07_value;
}

void param_07_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_07_value_set(preset["value"]);
}

Index globaltransport_getSampleOffset(MillisecondTime time) {
    return this->mstosamps(this->maximum(0, time - this->getEngine()->getCurrentTime()));
}

number globaltransport_getTempoAtSample(SampleIndex sampleOffset) {
    return (sampleOffset >= 0 && sampleOffset < this->vs ? this->globaltransport_tempo[(Index)sampleOffset] : this->globaltransport_lastTempo);
}

number globaltransport_getStateAtSample(SampleIndex sampleOffset) {
    return (sampleOffset >= 0 && sampleOffset < this->vs ? this->globaltransport_state[(Index)sampleOffset] : this->globaltransport_lastState);
}

number globaltransport_getState(MillisecondTime time) {
    return this->globaltransport_getStateAtSample(this->globaltransport_getSampleOffset(time));
}

number globaltransport_getBeatTime(MillisecondTime time) {
    number i = 2;

    while (i < this->globaltransport_beatTimeChanges->length && this->globaltransport_beatTimeChanges[(Index)(i + 1)] <= time) {
        i += 2;
    }

    i -= 2;
    number beatTimeBase = this->globaltransport_beatTimeChanges[(Index)i];

    if (this->globaltransport_getState(time) == 0)
        return beatTimeBase;

    number beatTimeBaseMsTime = this->globaltransport_beatTimeChanges[(Index)(i + 1)];
    number diff = time - beatTimeBaseMsTime;
    return beatTimeBase + this->mstobeats(diff);
}

bool globaltransport_setTempo(MillisecondTime time, number tempo, bool notify) {
    if ((bool)(notify)) {
        this->processTempoEvent(time, tempo);
        this->globaltransport_notify = true;
    } else {
        Index offset = (Index)(this->globaltransport_getSampleOffset(time));

        if (this->globaltransport_getTempoAtSample(offset) != tempo) {
            this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime(time));
            this->globaltransport_beatTimeChanges->push(time);
            fillSignal(this->globaltransport_tempo, this->vs, tempo, offset);
            this->globaltransport_lastTempo = tempo;
            this->globaltransport_tempoNeedsReset = true;
            return true;
        }
    }

    return false;
}

number globaltransport_getTempo(MillisecondTime time) {
    return this->globaltransport_getTempoAtSample(this->globaltransport_getSampleOffset(time));
}

bool globaltransport_setState(MillisecondTime time, number state, bool notify) {
    if ((bool)(notify)) {
        this->processTransportEvent(time, TransportState(state));
        this->globaltransport_notify = true;
    } else {
        Index offset = (Index)(this->globaltransport_getSampleOffset(time));

        if (this->globaltransport_getStateAtSample(offset) != state) {
            fillSignal(this->globaltransport_state, this->vs, state, offset);
            this->globaltransport_lastState = TransportState(state);
            this->globaltransport_stateNeedsReset = true;

            if (state == 0) {
                this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime(time));
                this->globaltransport_beatTimeChanges->push(time);
            }

            return true;
        }
    }

    return false;
}

bool globaltransport_setBeatTime(MillisecondTime time, number beattime, bool notify) {
    if ((bool)(notify)) {
        this->processBeatTimeEvent(time, beattime);
        this->globaltransport_notify = true;
        return false;
    } else {
        bool beatTimeHasChanged = false;
        float oldBeatTime = (float)(this->globaltransport_getBeatTime(time));
        float newBeatTime = (float)(beattime);

        if (oldBeatTime != newBeatTime) {
            beatTimeHasChanged = true;
        }

        this->globaltransport_beatTimeChanges->push(beattime);
        this->globaltransport_beatTimeChanges->push(time);
        return beatTimeHasChanged;
    }
}

number globaltransport_getBeatTimeAtSample(SampleIndex sampleOffset) {
    auto msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getBeatTime(this->getEngine()->getCurrentTime() + msOffset);
}

array<number, 2> globaltransport_getTimeSignature(MillisecondTime time) {
    number i = 3;

    while (i < this->globaltransport_timeSignatureChanges->length && this->globaltransport_timeSignatureChanges[(Index)(i + 2)] <= time) {
        i += 3;
    }

    i -= 3;

    return {
        this->globaltransport_timeSignatureChanges[(Index)i],
        this->globaltransport_timeSignatureChanges[(Index)(i + 1)]
    };
}

array<number, 2> globaltransport_getTimeSignatureAtSample(SampleIndex sampleOffset) {
    auto msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getTimeSignature(this->getEngine()->getCurrentTime() + msOffset);
}

bool globaltransport_setTimeSignature(MillisecondTime time, number numerator, number denominator, bool notify) {
    if ((bool)(notify)) {
        this->processTimeSignatureEvent(time, (int)(numerator), (int)(denominator));
        this->globaltransport_notify = true;
    } else {
        array<number, 2> currentSig = this->globaltransport_getTimeSignature(time);

        if (currentSig[0] != numerator || currentSig[1] != denominator) {
            this->globaltransport_timeSignatureChanges->push(numerator);
            this->globaltransport_timeSignatureChanges->push(denominator);
            this->globaltransport_timeSignatureChanges->push(time);
            return true;
        }
    }

    return false;
}

void globaltransport_advance() {
    if ((bool)(this->globaltransport_tempoNeedsReset)) {
        fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
        this->globaltransport_tempoNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTempoEvent(this->globaltransport_lastTempo);
        }
    }

    if ((bool)(this->globaltransport_stateNeedsReset)) {
        fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
        this->globaltransport_stateNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTransportEvent(TransportState(this->globaltransport_lastState));
        }
    }

    if (this->globaltransport_beatTimeChanges->length > 2) {
        this->globaltransport_beatTimeChanges[0] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 2)];
        this->globaltransport_beatTimeChanges[1] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 1)];
        this->globaltransport_beatTimeChanges->length = 2;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendBeatTimeEvent(this->globaltransport_beatTimeChanges[0]);
        }
    }

    if (this->globaltransport_timeSignatureChanges->length > 3) {
        this->globaltransport_timeSignatureChanges[0] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 3)];
        this->globaltransport_timeSignatureChanges[1] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 2)];
        this->globaltransport_timeSignatureChanges[2] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 1)];
        this->globaltransport_timeSignatureChanges->length = 3;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTimeSignatureEvent(
                (int)(this->globaltransport_timeSignatureChanges[0]),
                (int)(this->globaltransport_timeSignatureChanges[1])
            );
        }
    }

    this->globaltransport_notify = false;
}

void globaltransport_dspsetup(bool force) {
    if ((bool)(this->globaltransport_setupDone) && (bool)(!(bool)(force)))
        return;

    fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
    this->globaltransport_tempoNeedsReset = false;
    fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
    this->globaltransport_stateNeedsReset = false;
    this->globaltransport_setupDone = true;
}

bool stackprotect_check() {
    this->stackprotect_count++;

    if (this->stackprotect_count > 128) {
        console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
        return true;
    }

    return false;
}

void updateTime(MillisecondTime time) {
    this->_currentTime = time;
    this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));

    if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;

    if (this->sampleOffsetIntoNextAudioBuffer < 0)
        this->sampleOffsetIntoNextAudioBuffer = 0;
}

void assign_defaults()
{
    gen_01_in1 = 0;
    gen_01_in2 = 0;
    gen_01_gain = 0;
    gen_01_mix = 0.5;
    gen_01_pitch = 0;
    gen_01_width = 0.5;
    gen_01_grainPos = 100;
    gen_01_grainSize = 100;
    gen_01_interval = 100;
    expr_01_in1 = 0;
    expr_01_in2 = 0.01;
    expr_01_out1 = 0;
    expr_02_in1 = 0;
    expr_02_in2 = 0.01;
    expr_02_out1 = 0;
    param_01_value = 50;
    expr_03_in1 = 0;
    expr_03_out1 = 0;
    param_02_value = 0;
    param_03_value = 50;
    param_04_value = 0;
    param_05_value = 100;
    param_06_value = 100;
    param_07_value = 100;
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    gen_01_his_gain_value = 0;
    gen_01_his_mix_value = 0;
    gen_01_his_index_value = 0;
    gen_01_del_inR__maxdelay = 0;
    gen_01_del_inR_sizemode = 0;
    gen_01_del_inR_wrap = 0;
    gen_01_del_inR_reader = 0;
    gen_01_del_inR_writer = 0;
    gen_01_del_inL__maxdelay = 0;
    gen_01_del_inL_sizemode = 0;
    gen_01_del_inL_wrap = 0;
    gen_01_del_inL_reader = 0;
    gen_01_del_inL_writer = 0;
    gen_01_counter_0_carry = 0;
    gen_01_counter_0_count = 0;
    gen_01_setupDone = false;
    param_01_lastValue = 0;
    param_02_lastValue = 0;
    param_03_lastValue = 0;
    param_04_lastValue = 0;
    param_05_lastValue = 0;
    param_06_lastValue = 0;
    param_07_lastValue = 0;
    globaltransport_tempo = nullptr;
    globaltransport_tempoNeedsReset = false;
    globaltransport_lastTempo = 120;
    globaltransport_state = nullptr;
    globaltransport_stateNeedsReset = false;
    globaltransport_lastState = 0;
    globaltransport_beatTimeChanges = { 0, 0 };
    globaltransport_timeSignatureChanges = { 4, 4, 0 };
    globaltransport_notify = false;
    globaltransport_setupDone = false;
    stackprotect_count = 0;
    _voiceIndex = 0;
    _noteNumber = 0;
    isMuted = 1;
}

// member variables

    number gen_01_in1;
    number gen_01_in2;
    number gen_01_gain;
    number gen_01_mix;
    number gen_01_pitch;
    number gen_01_width;
    number gen_01_grainPos;
    number gen_01_grainSize;
    number gen_01_interval;
    number expr_01_in1;
    number expr_01_in2;
    number expr_01_out1;
    number expr_02_in1;
    number expr_02_in2;
    number expr_02_out1;
    number param_01_value;
    number expr_03_in1;
    number expr_03_out1;
    number param_02_value;
    number param_03_value;
    number param_04_value;
    number param_05_value;
    number param_06_value;
    number param_07_value;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    number gen_01_his_gain_value;
    number gen_01_his_mix_value;
    number gen_01_his_index_value;
    Float64BufferRef gen_01_data_param;
    Float64BufferRef gen_01_del_inR_buffer;
    Index gen_01_del_inR__maxdelay;
    Int gen_01_del_inR_sizemode;
    Index gen_01_del_inR_wrap;
    Int gen_01_del_inR_reader;
    Int gen_01_del_inR_writer;
    Float64BufferRef gen_01_del_inL_buffer;
    Index gen_01_del_inL__maxdelay;
    Int gen_01_del_inL_sizemode;
    Index gen_01_del_inL_wrap;
    Int gen_01_del_inL_reader;
    Int gen_01_del_inL_writer;
    int gen_01_counter_0_carry;
    number gen_01_counter_0_count;
    UInt gen_01_noise_4_state[4] = { };
    UInt gen_01_noise_7_state[4] = { };
    bool gen_01_setupDone;
    number param_01_lastValue;
    number param_02_lastValue;
    number param_03_lastValue;
    number param_04_lastValue;
    number param_05_lastValue;
    number param_06_lastValue;
    number param_07_lastValue;
    signal globaltransport_tempo;
    bool globaltransport_tempoNeedsReset;
    number globaltransport_lastTempo;
    signal globaltransport_state;
    bool globaltransport_stateNeedsReset;
    number globaltransport_lastState;
    list globaltransport_beatTimeChanges;
    list globaltransport_timeSignatureChanges;
    bool globaltransport_notify;
    bool globaltransport_setupDone;
    number stackprotect_count;
    DataRef data_param;
    DataRef gen_01_del_inR_bufferobj;
    DataRef gen_01_del_inL_bufferobj;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;

};

PatcherInterface* creategranular()
{
    return new granular();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr granularFactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return creategranular;
}

} // end RNBO namespace

