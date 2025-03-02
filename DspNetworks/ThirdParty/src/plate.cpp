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

class plate : public PatcherInterfaceImpl {
public:

class RNBOSubpatcher_17 : public PatcherInterfaceImpl {
    
    friend class plate;
    
    public:
    
    RNBOSubpatcher_17()
    {
    }
    
    ~RNBOSubpatcher_17()
    {
    }
    
    virtual plate* getPatcher() const {
        return static_cast<plate *>(_parentPatcher);
    }
    
    plate* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
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
    
    inline number safesqrt(number num) {
        return (num > 0.0 ? rnbo_sqrt(num) : 0.0);
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
        const SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        const SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        const SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        this->xfade_tilde_01_perform(in5, in1, in3, out1, n);
        this->xfade_tilde_02_perform(in5, in2, in4, out2, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
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
    
        RNBO_UNUSED(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
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
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
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
        return 5;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {}
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void xfade_tilde_01_perform(
        const Sample * pos,
        const SampleValue * in1,
        const SampleValue * in2,
        SampleValue * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out[(Index)i] = in1[(Index)i] * this->xfade_tilde_01_func_next(pos[(Index)i], 0) + in2[(Index)i] * this->xfade_tilde_01_func_next(pos[(Index)i], 1);
        }
    }
    
    void xfade_tilde_02_perform(
        const Sample * pos,
        const SampleValue * in1,
        const SampleValue * in2,
        SampleValue * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out[(Index)i] = in1[(Index)i] * this->xfade_tilde_02_func_next(pos[(Index)i], 0) + in2[(Index)i] * this->xfade_tilde_02_func_next(pos[(Index)i], 1);
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number xfade_tilde_01_func_next(number pos, int channel) {
        {
            {
                number nchan_1 = 2 - 1;
    
                {
                    pos = pos * nchan_1;
                }
    
                {
                    if (pos > nchan_1)
                        pos = nchan_1;
                    else if (pos < 0)
                        pos = 0;
                }
    
                pos = pos - channel;
    
                if (pos > -1 && pos < 1) {
                    {
                        return rnbo_cos(1.57079632679489661923 * pos);
                    }
                } else {
                    return 0;
                }
            }
        }
    }
    
    void xfade_tilde_01_func_reset() {}
    
    number xfade_tilde_02_func_next(number pos, int channel) {
        {
            {
                number nchan_1 = 2 - 1;
    
                {
                    pos = pos * nchan_1;
                }
    
                {
                    if (pos > nchan_1)
                        pos = nchan_1;
                    else if (pos < 0)
                        pos = 0;
                }
    
                pos = pos - channel;
    
                if (pos > -1 && pos < 1) {
                    {
                        return rnbo_cos(1.57079632679489661923 * pos);
                    }
                } else {
                    return 0;
                }
            }
        }
    }
    
    void xfade_tilde_02_func_reset() {}
    
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
        xfade_tilde_01_pos = 0;
        xfade_tilde_02_pos = 0;
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
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number xfade_tilde_01_pos;
        number xfade_tilde_02_pos;
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
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_18 : public PatcherInterfaceImpl {
    
    friend class plate;
    
    public:
    
    RNBOSubpatcher_18()
    {
    }
    
    ~RNBOSubpatcher_18()
    {
    }
    
    virtual plate* getPatcher() const {
        return static_cast<plate *>(_parentPatcher);
    }
    
    plate* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
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
        SampleValue * out3 = (numOutputs >= 3 && outputs[2] ? outputs[2] : this->dummyBuffer);
        SampleValue * out4 = (numOutputs >= 4 && outputs[3] ? outputs[3] : this->dummyBuffer);
        SampleValue * out5 = (numOutputs >= 5 && outputs[4] ? outputs[4] : this->dummyBuffer);
        SampleValue * out6 = (numOutputs >= 6 && outputs[5] ? outputs[5] : this->dummyBuffer);
        SampleValue * out7 = (numOutputs >= 7 && outputs[6] ? outputs[6] : this->dummyBuffer);
        SampleValue * out8 = (numOutputs >= 8 && outputs[7] ? outputs[7] : this->dummyBuffer);
        SampleValue * out9 = (numOutputs >= 9 && outputs[8] ? outputs[8] : this->dummyBuffer);
        SampleValue * out10 = (numOutputs >= 10 && outputs[9] ? outputs[9] : this->dummyBuffer);
        SampleValue * out11 = (numOutputs >= 11 && outputs[10] ? outputs[10] : this->dummyBuffer);
        SampleValue * out12 = (numOutputs >= 12 && outputs[11] ? outputs[11] : this->dummyBuffer);
        SampleValue * out13 = (numOutputs >= 13 && outputs[12] ? outputs[12] : this->dummyBuffer);
        SampleValue * out14 = (numOutputs >= 14 && outputs[13] ? outputs[13] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        const SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        const SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        const SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        const SampleValue * in6 = (numInputs >= 6 && inputs[5] ? inputs[5] : this->zeroBuffer);
        const SampleValue * in7 = (numInputs >= 7 && inputs[6] ? inputs[6] : this->zeroBuffer);
        const SampleValue * in8 = (numInputs >= 8 && inputs[7] ? inputs[7] : this->zeroBuffer);
        this->signalforwarder_01_perform(in1, out2, n);
        this->signalforwarder_02_perform(in1, out1, n);
        this->signalforwarder_03_perform(in2, out12, n);
        this->signalforwarder_04_perform(in3, out13, n);
        this->signalforwarder_05_perform(in3, out3, n);
        this->signalforwarder_06_perform(in4, out14, n);
        this->signalforwarder_07_perform(in4, out4, n);
        this->signalforwarder_08_perform(in5, out8, n);
        this->signalforwarder_09_perform(in5, out9, n);
        this->signalforwarder_10_perform(in6, out5, n);
        this->signalforwarder_11_perform(in7, out10, n);
        this->signalforwarder_12_perform(in7, out6, n);
        this->signalforwarder_13_perform(in8, out11, n);
        this->signalforwarder_14_perform(in8, out7, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
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
    
        RNBO_UNUSED(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
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
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
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
        return 8;
    }
    
    Index getNumOutputChannels() const {
        return 14;
    }
    
    void initializeObjects() {}
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void signalforwarder_01_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_02_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_03_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_04_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_05_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_06_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_07_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_08_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_09_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_10_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_11_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_12_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_13_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_14_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
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
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
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
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_19 : public PatcherInterfaceImpl {
    
    friend class plate;
    
    public:
    
    RNBOSubpatcher_19()
    {
    }
    
    ~RNBOSubpatcher_19()
    {
    }
    
    virtual plate* getPatcher() const {
        return static_cast<plate *>(_parentPatcher);
    }
    
    plate* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
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
    
        this->allpass_tilde_04_perform(
            in1,
            this->allpass_tilde_04_delayTime,
            this->allpass_tilde_04_gain,
            this->signals[0],
            n
        );
    
        this->allpass_tilde_03_perform(
            this->signals[0],
            this->allpass_tilde_03_delayTime,
            this->allpass_tilde_03_gain,
            this->signals[1],
            n
        );
    
        this->allpass_tilde_02_perform(
            this->signals[1],
            this->allpass_tilde_02_delayTime,
            this->allpass_tilde_02_gain,
            this->signals[0],
            n
        );
    
        this->allpass_tilde_01_perform(
            this->signals[0],
            this->allpass_tilde_01_delayTime,
            this->allpass_tilde_01_gain,
            out1,
            n
        );
    
        this->allpass_tilde_08_perform(
            in2,
            this->allpass_tilde_08_delayTime,
            this->allpass_tilde_08_gain,
            this->signals[0],
            n
        );
    
        this->allpass_tilde_07_perform(
            this->signals[0],
            this->allpass_tilde_07_delayTime,
            this->allpass_tilde_07_gain,
            this->signals[1],
            n
        );
    
        this->allpass_tilde_06_perform(
            this->signals[1],
            this->allpass_tilde_06_delayTime,
            this->allpass_tilde_06_gain,
            this->signals[0],
            n
        );
    
        this->allpass_tilde_05_perform(
            this->signals[0],
            this->allpass_tilde_05_delayTime,
            this->allpass_tilde_05_gain,
            out2,
            n
        );
    
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 2; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
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
    
        this->allpass_tilde_04_dspsetup(forceDSPSetup);
        this->allpass_tilde_03_dspsetup(forceDSPSetup);
        this->allpass_tilde_02_dspsetup(forceDSPSetup);
        this->allpass_tilde_01_dspsetup(forceDSPSetup);
        this->allpass_tilde_08_dspsetup(forceDSPSetup);
        this->allpass_tilde_07_dspsetup(forceDSPSetup);
        this->allpass_tilde_06_dspsetup(forceDSPSetup);
        this->allpass_tilde_05_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
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
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
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
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 0:
            {
            return addressOf(this->allpass_tilde_01_feedForward_bufferobj);
            break;
            }
        case 1:
            {
            return addressOf(this->allpass_tilde_01_feedBack_bufferobj);
            break;
            }
        case 2:
            {
            return addressOf(this->allpass_tilde_02_feedForward_bufferobj);
            break;
            }
        case 3:
            {
            return addressOf(this->allpass_tilde_02_feedBack_bufferobj);
            break;
            }
        case 4:
            {
            return addressOf(this->allpass_tilde_03_feedForward_bufferobj);
            break;
            }
        case 5:
            {
            return addressOf(this->allpass_tilde_03_feedBack_bufferobj);
            break;
            }
        case 6:
            {
            return addressOf(this->allpass_tilde_04_feedForward_bufferobj);
            break;
            }
        case 7:
            {
            return addressOf(this->allpass_tilde_04_feedBack_bufferobj);
            break;
            }
        case 8:
            {
            return addressOf(this->allpass_tilde_05_feedForward_bufferobj);
            break;
            }
        case 9:
            {
            return addressOf(this->allpass_tilde_05_feedBack_bufferobj);
            break;
            }
        case 10:
            {
            return addressOf(this->allpass_tilde_06_feedForward_bufferobj);
            break;
            }
        case 11:
            {
            return addressOf(this->allpass_tilde_06_feedBack_bufferobj);
            break;
            }
        case 12:
            {
            return addressOf(this->allpass_tilde_07_feedForward_bufferobj);
            break;
            }
        case 13:
            {
            return addressOf(this->allpass_tilde_07_feedBack_bufferobj);
            break;
            }
        case 14:
            {
            return addressOf(this->allpass_tilde_08_feedForward_bufferobj);
            break;
            }
        case 15:
            {
            return addressOf(this->allpass_tilde_08_feedBack_bufferobj);
            break;
            }
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 16;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void zeroDataRef(DataRef& ref) {
        ref->setZero();
    }
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 0) {
            this->allpass_tilde_01_feedForward_buffer = new Float64Buffer(this->allpass_tilde_01_feedForward_bufferobj);
        }
    
        if (index == 1) {
            this->allpass_tilde_01_feedBack_buffer = new Float64Buffer(this->allpass_tilde_01_feedBack_bufferobj);
        }
    
        if (index == 2) {
            this->allpass_tilde_02_feedForward_buffer = new Float64Buffer(this->allpass_tilde_02_feedForward_bufferobj);
        }
    
        if (index == 3) {
            this->allpass_tilde_02_feedBack_buffer = new Float64Buffer(this->allpass_tilde_02_feedBack_bufferobj);
        }
    
        if (index == 4) {
            this->allpass_tilde_03_feedForward_buffer = new Float64Buffer(this->allpass_tilde_03_feedForward_bufferobj);
        }
    
        if (index == 5) {
            this->allpass_tilde_03_feedBack_buffer = new Float64Buffer(this->allpass_tilde_03_feedBack_bufferobj);
        }
    
        if (index == 6) {
            this->allpass_tilde_04_feedForward_buffer = new Float64Buffer(this->allpass_tilde_04_feedForward_bufferobj);
        }
    
        if (index == 7) {
            this->allpass_tilde_04_feedBack_buffer = new Float64Buffer(this->allpass_tilde_04_feedBack_bufferobj);
        }
    
        if (index == 8) {
            this->allpass_tilde_05_feedForward_buffer = new Float64Buffer(this->allpass_tilde_05_feedForward_bufferobj);
        }
    
        if (index == 9) {
            this->allpass_tilde_05_feedBack_buffer = new Float64Buffer(this->allpass_tilde_05_feedBack_bufferobj);
        }
    
        if (index == 10) {
            this->allpass_tilde_06_feedForward_buffer = new Float64Buffer(this->allpass_tilde_06_feedForward_bufferobj);
        }
    
        if (index == 11) {
            this->allpass_tilde_06_feedBack_buffer = new Float64Buffer(this->allpass_tilde_06_feedBack_bufferobj);
        }
    
        if (index == 12) {
            this->allpass_tilde_07_feedForward_buffer = new Float64Buffer(this->allpass_tilde_07_feedForward_bufferobj);
        }
    
        if (index == 13) {
            this->allpass_tilde_07_feedBack_buffer = new Float64Buffer(this->allpass_tilde_07_feedBack_bufferobj);
        }
    
        if (index == 14) {
            this->allpass_tilde_08_feedForward_buffer = new Float64Buffer(this->allpass_tilde_08_feedForward_bufferobj);
        }
    
        if (index == 15) {
            this->allpass_tilde_08_feedBack_buffer = new Float64Buffer(this->allpass_tilde_08_feedBack_bufferobj);
        }
    }
    
    void initialize() {
        this->allpass_tilde_01_feedForward_bufferobj = initDataRef("allpass_tilde_01_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_01_feedBack_bufferobj = initDataRef("allpass_tilde_01_feedBack_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_02_feedForward_bufferobj = initDataRef("allpass_tilde_02_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_02_feedBack_bufferobj = initDataRef("allpass_tilde_02_feedBack_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_03_feedForward_bufferobj = initDataRef("allpass_tilde_03_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_03_feedBack_bufferobj = initDataRef("allpass_tilde_03_feedBack_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_04_feedForward_bufferobj = initDataRef("allpass_tilde_04_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_04_feedBack_bufferobj = initDataRef("allpass_tilde_04_feedBack_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_05_feedForward_bufferobj = initDataRef("allpass_tilde_05_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_05_feedBack_bufferobj = initDataRef("allpass_tilde_05_feedBack_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_06_feedForward_bufferobj = initDataRef("allpass_tilde_06_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_06_feedBack_bufferobj = initDataRef("allpass_tilde_06_feedBack_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_07_feedForward_bufferobj = initDataRef("allpass_tilde_07_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_07_feedBack_bufferobj = initDataRef("allpass_tilde_07_feedBack_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_08_feedForward_bufferobj = initDataRef("allpass_tilde_08_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_08_feedBack_bufferobj = initDataRef("allpass_tilde_08_feedBack_bufferobj", true, nullptr, "buffer~");
        this->assign_defaults();
        this->setState();
        this->allpass_tilde_01_feedForward_bufferobj->setIndex(0);
        this->allpass_tilde_01_feedForward_buffer = new Float64Buffer(this->allpass_tilde_01_feedForward_bufferobj);
        this->allpass_tilde_01_feedBack_bufferobj->setIndex(1);
        this->allpass_tilde_01_feedBack_buffer = new Float64Buffer(this->allpass_tilde_01_feedBack_bufferobj);
        this->allpass_tilde_02_feedForward_bufferobj->setIndex(2);
        this->allpass_tilde_02_feedForward_buffer = new Float64Buffer(this->allpass_tilde_02_feedForward_bufferobj);
        this->allpass_tilde_02_feedBack_bufferobj->setIndex(3);
        this->allpass_tilde_02_feedBack_buffer = new Float64Buffer(this->allpass_tilde_02_feedBack_bufferobj);
        this->allpass_tilde_03_feedForward_bufferobj->setIndex(4);
        this->allpass_tilde_03_feedForward_buffer = new Float64Buffer(this->allpass_tilde_03_feedForward_bufferobj);
        this->allpass_tilde_03_feedBack_bufferobj->setIndex(5);
        this->allpass_tilde_03_feedBack_buffer = new Float64Buffer(this->allpass_tilde_03_feedBack_bufferobj);
        this->allpass_tilde_04_feedForward_bufferobj->setIndex(6);
        this->allpass_tilde_04_feedForward_buffer = new Float64Buffer(this->allpass_tilde_04_feedForward_bufferobj);
        this->allpass_tilde_04_feedBack_bufferobj->setIndex(7);
        this->allpass_tilde_04_feedBack_buffer = new Float64Buffer(this->allpass_tilde_04_feedBack_bufferobj);
        this->allpass_tilde_05_feedForward_bufferobj->setIndex(8);
        this->allpass_tilde_05_feedForward_buffer = new Float64Buffer(this->allpass_tilde_05_feedForward_bufferobj);
        this->allpass_tilde_05_feedBack_bufferobj->setIndex(9);
        this->allpass_tilde_05_feedBack_buffer = new Float64Buffer(this->allpass_tilde_05_feedBack_bufferobj);
        this->allpass_tilde_06_feedForward_bufferobj->setIndex(10);
        this->allpass_tilde_06_feedForward_buffer = new Float64Buffer(this->allpass_tilde_06_feedForward_bufferobj);
        this->allpass_tilde_06_feedBack_bufferobj->setIndex(11);
        this->allpass_tilde_06_feedBack_buffer = new Float64Buffer(this->allpass_tilde_06_feedBack_bufferobj);
        this->allpass_tilde_07_feedForward_bufferobj->setIndex(12);
        this->allpass_tilde_07_feedForward_buffer = new Float64Buffer(this->allpass_tilde_07_feedForward_bufferobj);
        this->allpass_tilde_07_feedBack_bufferobj->setIndex(13);
        this->allpass_tilde_07_feedBack_buffer = new Float64Buffer(this->allpass_tilde_07_feedBack_bufferobj);
        this->allpass_tilde_08_feedForward_bufferobj->setIndex(14);
        this->allpass_tilde_08_feedForward_buffer = new Float64Buffer(this->allpass_tilde_08_feedForward_bufferobj);
        this->allpass_tilde_08_feedBack_bufferobj->setIndex(15);
        this->allpass_tilde_08_feedBack_buffer = new Float64Buffer(this->allpass_tilde_08_feedBack_bufferobj);
    }
    
    protected:
    
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
    
    void initializeObjects() {
        this->allpass_tilde_01_feedForward_init();
        this->allpass_tilde_01_feedBack_init();
        this->allpass_tilde_01_init();
        this->allpass_tilde_02_feedForward_init();
        this->allpass_tilde_02_feedBack_init();
        this->allpass_tilde_02_init();
        this->allpass_tilde_03_feedForward_init();
        this->allpass_tilde_03_feedBack_init();
        this->allpass_tilde_03_init();
        this->allpass_tilde_04_feedForward_init();
        this->allpass_tilde_04_feedBack_init();
        this->allpass_tilde_04_init();
        this->allpass_tilde_05_feedForward_init();
        this->allpass_tilde_05_feedBack_init();
        this->allpass_tilde_05_init();
        this->allpass_tilde_06_feedForward_init();
        this->allpass_tilde_06_feedBack_init();
        this->allpass_tilde_06_init();
        this->allpass_tilde_07_feedForward_init();
        this->allpass_tilde_07_feedBack_init();
        this->allpass_tilde_07_init();
        this->allpass_tilde_08_feedForward_init();
        this->allpass_tilde_08_feedBack_init();
        this->allpass_tilde_08_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->allpass_tilde_01_feedForward_buffer = this->allpass_tilde_01_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_01_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_01_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_01_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(0);
        }
    
        this->allpass_tilde_01_feedBack_buffer = this->allpass_tilde_01_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_01_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_01_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_01_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    
        this->allpass_tilde_02_feedForward_buffer = this->allpass_tilde_02_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_02_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_02_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_02_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(2);
        }
    
        this->allpass_tilde_02_feedBack_buffer = this->allpass_tilde_02_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_02_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_02_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_02_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(3);
        }
    
        this->allpass_tilde_03_feedForward_buffer = this->allpass_tilde_03_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_03_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_03_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_03_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(4);
        }
    
        this->allpass_tilde_03_feedBack_buffer = this->allpass_tilde_03_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_03_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_03_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_03_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(5);
        }
    
        this->allpass_tilde_04_feedForward_buffer = this->allpass_tilde_04_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_04_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_04_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_04_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(6);
        }
    
        this->allpass_tilde_04_feedBack_buffer = this->allpass_tilde_04_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_04_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_04_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_04_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(7);
        }
    
        this->allpass_tilde_05_feedForward_buffer = this->allpass_tilde_05_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_05_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_05_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_05_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(8);
        }
    
        this->allpass_tilde_05_feedBack_buffer = this->allpass_tilde_05_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_05_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_05_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_05_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(9);
        }
    
        this->allpass_tilde_06_feedForward_buffer = this->allpass_tilde_06_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_06_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_06_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_06_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(10);
        }
    
        this->allpass_tilde_06_feedBack_buffer = this->allpass_tilde_06_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_06_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_06_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_06_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(11);
        }
    
        this->allpass_tilde_07_feedForward_buffer = this->allpass_tilde_07_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_07_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_07_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_07_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(12);
        }
    
        this->allpass_tilde_07_feedBack_buffer = this->allpass_tilde_07_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_07_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_07_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_07_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(13);
        }
    
        this->allpass_tilde_08_feedForward_buffer = this->allpass_tilde_08_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_08_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_08_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_08_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(14);
        }
    
        this->allpass_tilde_08_feedBack_buffer = this->allpass_tilde_08_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_08_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_08_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_08_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(15);
        }
    }
    
    void allpass_tilde_04_perform(
        const Sample * x,
        number delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        RNBO_UNUSED(delayTime);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = 0.75;
            auto delayTimeinSamps = this->mstosamps(4.76);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_04_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_04_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_04_feedForward_write(x[(Index)i]);
            this->allpass_tilde_04_feedBack_write(ov);
            this->allpass_tilde_04_feedForward_step();
            this->allpass_tilde_04_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void allpass_tilde_03_perform(
        const Sample * x,
        number delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        RNBO_UNUSED(delayTime);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = 0.75;
            auto delayTimeinSamps = this->mstosamps(3.58);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_03_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_03_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_03_feedForward_write(x[(Index)i]);
            this->allpass_tilde_03_feedBack_write(ov);
            this->allpass_tilde_03_feedForward_step();
            this->allpass_tilde_03_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void allpass_tilde_02_perform(
        const Sample * x,
        number delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        RNBO_UNUSED(delayTime);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = 0.625;
            auto delayTimeinSamps = this->mstosamps(12.73);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_02_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_02_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_02_feedForward_write(x[(Index)i]);
            this->allpass_tilde_02_feedBack_write(ov);
            this->allpass_tilde_02_feedForward_step();
            this->allpass_tilde_02_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void allpass_tilde_01_perform(
        const Sample * x,
        number delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        RNBO_UNUSED(delayTime);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = 0.625;
            auto delayTimeinSamps = this->mstosamps(9.3);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_01_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_01_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_01_feedForward_write(x[(Index)i]);
            this->allpass_tilde_01_feedBack_write(ov);
            this->allpass_tilde_01_feedForward_step();
            this->allpass_tilde_01_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void allpass_tilde_08_perform(
        const Sample * x,
        number delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        RNBO_UNUSED(delayTime);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = 0.75;
            auto delayTimeinSamps = this->mstosamps(4.76);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_08_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_08_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_08_feedForward_write(x[(Index)i]);
            this->allpass_tilde_08_feedBack_write(ov);
            this->allpass_tilde_08_feedForward_step();
            this->allpass_tilde_08_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void allpass_tilde_07_perform(
        const Sample * x,
        number delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        RNBO_UNUSED(delayTime);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = 0.75;
            auto delayTimeinSamps = this->mstosamps(3.58);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_07_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_07_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_07_feedForward_write(x[(Index)i]);
            this->allpass_tilde_07_feedBack_write(ov);
            this->allpass_tilde_07_feedForward_step();
            this->allpass_tilde_07_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void allpass_tilde_06_perform(
        const Sample * x,
        number delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        RNBO_UNUSED(delayTime);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = 0.625;
            auto delayTimeinSamps = this->mstosamps(12.73);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_06_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_06_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_06_feedForward_write(x[(Index)i]);
            this->allpass_tilde_06_feedBack_write(ov);
            this->allpass_tilde_06_feedForward_step();
            this->allpass_tilde_06_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void allpass_tilde_05_perform(
        const Sample * x,
        number delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        RNBO_UNUSED(delayTime);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = 0.625;
            auto delayTimeinSamps = this->mstosamps(9.3);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_05_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_05_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_05_feedForward_write(x[(Index)i]);
            this->allpass_tilde_05_feedBack_write(ov);
            this->allpass_tilde_05_feedForward_step();
            this->allpass_tilde_05_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void allpass_tilde_01_feedForward_step() {
        this->allpass_tilde_01_feedForward_reader++;
    
        if (this->allpass_tilde_01_feedForward_reader >= (int)(this->allpass_tilde_01_feedForward_buffer->getSize()))
            this->allpass_tilde_01_feedForward_reader = 0;
    }
    
    number allpass_tilde_01_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_01_feedForward_buffer->getSize()) + this->allpass_tilde_01_feedForward_reader - ((size > this->allpass_tilde_01_feedForward__maxdelay ? this->allpass_tilde_01_feedForward__maxdelay : (size < (this->allpass_tilde_01_feedForward_reader != this->allpass_tilde_01_feedForward_writer) ? this->allpass_tilde_01_feedForward_reader != this->allpass_tilde_01_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ), this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_01_feedForward_buffer->getSize()) + this->allpass_tilde_01_feedForward_reader - ((size > this->allpass_tilde_01_feedForward__maxdelay ? this->allpass_tilde_01_feedForward__maxdelay : (size < (1 + this->allpass_tilde_01_feedForward_reader != this->allpass_tilde_01_feedForward_writer) ? 1 + this->allpass_tilde_01_feedForward_reader != this->allpass_tilde_01_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ), this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ), this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ), this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_01_feedForward_buffer->getSize()) + this->allpass_tilde_01_feedForward_reader - ((size > this->allpass_tilde_01_feedForward__maxdelay ? this->allpass_tilde_01_feedForward__maxdelay : (size < (1 + this->allpass_tilde_01_feedForward_reader != this->allpass_tilde_01_feedForward_writer) ? 1 + this->allpass_tilde_01_feedForward_reader != this->allpass_tilde_01_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ), this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ), this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ), this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_01_feedForward_buffer->getSize()) + this->allpass_tilde_01_feedForward_reader - ((size > this->allpass_tilde_01_feedForward__maxdelay ? this->allpass_tilde_01_feedForward__maxdelay : (size < (this->allpass_tilde_01_feedForward_reader != this->allpass_tilde_01_feedForward_writer) ? this->allpass_tilde_01_feedForward_reader != this->allpass_tilde_01_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ), this->allpass_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_01_feedForward_buffer->getSize()) + this->allpass_tilde_01_feedForward_reader - ((size > this->allpass_tilde_01_feedForward__maxdelay ? this->allpass_tilde_01_feedForward__maxdelay : (size < (this->allpass_tilde_01_feedForward_reader != this->allpass_tilde_01_feedForward_writer) ? this->allpass_tilde_01_feedForward_reader != this->allpass_tilde_01_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_01_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_01_feedForward_wrap))
        );
    }
    
    void allpass_tilde_01_feedForward_write(number v) {
        this->allpass_tilde_01_feedForward_writer = this->allpass_tilde_01_feedForward_reader;
        this->allpass_tilde_01_feedForward_buffer[(Index)this->allpass_tilde_01_feedForward_writer] = v;
    }
    
    number allpass_tilde_01_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_01_feedForward__maxdelay : size);
        number val = this->allpass_tilde_01_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_01_feedForward_write(v);
        this->allpass_tilde_01_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_01_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_01_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_01_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_01_feedForward_init() {
        auto result = this->allpass_tilde_01_feedForward_calcSizeInSamples();
        this->allpass_tilde_01_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_01_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_01_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_01_feedForward_clear() {
        this->allpass_tilde_01_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_01_feedForward_reset() {
        auto result = this->allpass_tilde_01_feedForward_calcSizeInSamples();
        this->allpass_tilde_01_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_01_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_01_feedForward_buffer);
        this->allpass_tilde_01_feedForward_wrap = this->allpass_tilde_01_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_01_feedForward_clear();
    
        if (this->allpass_tilde_01_feedForward_reader >= this->allpass_tilde_01_feedForward__maxdelay || this->allpass_tilde_01_feedForward_writer >= this->allpass_tilde_01_feedForward__maxdelay) {
            this->allpass_tilde_01_feedForward_reader = 0;
            this->allpass_tilde_01_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_01_feedForward_dspsetup() {
        this->allpass_tilde_01_feedForward_reset();
    }
    
    number allpass_tilde_01_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_01_feedForward_size() {
        return this->allpass_tilde_01_feedForward__maxdelay;
    }
    
    void allpass_tilde_01_feedBack_step() {
        this->allpass_tilde_01_feedBack_reader++;
    
        if (this->allpass_tilde_01_feedBack_reader >= (int)(this->allpass_tilde_01_feedBack_buffer->getSize()))
            this->allpass_tilde_01_feedBack_reader = 0;
    }
    
    number allpass_tilde_01_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_01_feedBack_buffer->getSize()) + this->allpass_tilde_01_feedBack_reader - ((size > this->allpass_tilde_01_feedBack__maxdelay ? this->allpass_tilde_01_feedBack__maxdelay : (size < (this->allpass_tilde_01_feedBack_reader != this->allpass_tilde_01_feedBack_writer) ? this->allpass_tilde_01_feedBack_reader != this->allpass_tilde_01_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ), this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_01_feedBack_buffer->getSize()) + this->allpass_tilde_01_feedBack_reader - ((size > this->allpass_tilde_01_feedBack__maxdelay ? this->allpass_tilde_01_feedBack__maxdelay : (size < (1 + this->allpass_tilde_01_feedBack_reader != this->allpass_tilde_01_feedBack_writer) ? 1 + this->allpass_tilde_01_feedBack_reader != this->allpass_tilde_01_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ), this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ), this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ), this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_01_feedBack_buffer->getSize()) + this->allpass_tilde_01_feedBack_reader - ((size > this->allpass_tilde_01_feedBack__maxdelay ? this->allpass_tilde_01_feedBack__maxdelay : (size < (1 + this->allpass_tilde_01_feedBack_reader != this->allpass_tilde_01_feedBack_writer) ? 1 + this->allpass_tilde_01_feedBack_reader != this->allpass_tilde_01_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ), this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ), this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ), this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_01_feedBack_buffer->getSize()) + this->allpass_tilde_01_feedBack_reader - ((size > this->allpass_tilde_01_feedBack__maxdelay ? this->allpass_tilde_01_feedBack__maxdelay : (size < (this->allpass_tilde_01_feedBack_reader != this->allpass_tilde_01_feedBack_writer) ? this->allpass_tilde_01_feedBack_reader != this->allpass_tilde_01_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ), this->allpass_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_01_feedBack_buffer->getSize()) + this->allpass_tilde_01_feedBack_reader - ((size > this->allpass_tilde_01_feedBack__maxdelay ? this->allpass_tilde_01_feedBack__maxdelay : (size < (this->allpass_tilde_01_feedBack_reader != this->allpass_tilde_01_feedBack_writer) ? this->allpass_tilde_01_feedBack_reader != this->allpass_tilde_01_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_01_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_01_feedBack_wrap))
        );
    }
    
    void allpass_tilde_01_feedBack_write(number v) {
        this->allpass_tilde_01_feedBack_writer = this->allpass_tilde_01_feedBack_reader;
        this->allpass_tilde_01_feedBack_buffer[(Index)this->allpass_tilde_01_feedBack_writer] = v;
    }
    
    number allpass_tilde_01_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_01_feedBack__maxdelay : size);
        number val = this->allpass_tilde_01_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_01_feedBack_write(v);
        this->allpass_tilde_01_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_01_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_01_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_01_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_01_feedBack_init() {
        auto result = this->allpass_tilde_01_feedBack_calcSizeInSamples();
        this->allpass_tilde_01_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_01_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_01_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_01_feedBack_clear() {
        this->allpass_tilde_01_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_01_feedBack_reset() {
        auto result = this->allpass_tilde_01_feedBack_calcSizeInSamples();
        this->allpass_tilde_01_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_01_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_01_feedBack_buffer);
        this->allpass_tilde_01_feedBack_wrap = this->allpass_tilde_01_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_01_feedBack_clear();
    
        if (this->allpass_tilde_01_feedBack_reader >= this->allpass_tilde_01_feedBack__maxdelay || this->allpass_tilde_01_feedBack_writer >= this->allpass_tilde_01_feedBack__maxdelay) {
            this->allpass_tilde_01_feedBack_reader = 0;
            this->allpass_tilde_01_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_01_feedBack_dspsetup() {
        this->allpass_tilde_01_feedBack_reset();
    }
    
    number allpass_tilde_01_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_01_feedBack_size() {
        return this->allpass_tilde_01_feedBack__maxdelay;
    }
    
    void allpass_tilde_01_init() {
        this->allpass_tilde_01_feedForward_init();
        this->allpass_tilde_01_feedBack_init();
    }
    
    void allpass_tilde_01_clear() {
        this->allpass_tilde_01_feedForward_clear();
        this->allpass_tilde_01_feedBack_clear();
    }
    
    void allpass_tilde_01_reset() {
        this->allpass_tilde_01_init();
        this->allpass_tilde_01_clear();
    }
    
    void allpass_tilde_01_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_01_reset();
        this->allpass_tilde_01_setupDone = true;
        this->allpass_tilde_01_feedForward_dspsetup();
        this->allpass_tilde_01_feedBack_dspsetup();
    }
    
    void allpass_tilde_02_feedForward_step() {
        this->allpass_tilde_02_feedForward_reader++;
    
        if (this->allpass_tilde_02_feedForward_reader >= (int)(this->allpass_tilde_02_feedForward_buffer->getSize()))
            this->allpass_tilde_02_feedForward_reader = 0;
    }
    
    number allpass_tilde_02_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_02_feedForward_buffer->getSize()) + this->allpass_tilde_02_feedForward_reader - ((size > this->allpass_tilde_02_feedForward__maxdelay ? this->allpass_tilde_02_feedForward__maxdelay : (size < (this->allpass_tilde_02_feedForward_reader != this->allpass_tilde_02_feedForward_writer) ? this->allpass_tilde_02_feedForward_reader != this->allpass_tilde_02_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ), this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_02_feedForward_buffer->getSize()) + this->allpass_tilde_02_feedForward_reader - ((size > this->allpass_tilde_02_feedForward__maxdelay ? this->allpass_tilde_02_feedForward__maxdelay : (size < (1 + this->allpass_tilde_02_feedForward_reader != this->allpass_tilde_02_feedForward_writer) ? 1 + this->allpass_tilde_02_feedForward_reader != this->allpass_tilde_02_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ), this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ), this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ), this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_02_feedForward_buffer->getSize()) + this->allpass_tilde_02_feedForward_reader - ((size > this->allpass_tilde_02_feedForward__maxdelay ? this->allpass_tilde_02_feedForward__maxdelay : (size < (1 + this->allpass_tilde_02_feedForward_reader != this->allpass_tilde_02_feedForward_writer) ? 1 + this->allpass_tilde_02_feedForward_reader != this->allpass_tilde_02_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ), this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ), this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ), this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_02_feedForward_buffer->getSize()) + this->allpass_tilde_02_feedForward_reader - ((size > this->allpass_tilde_02_feedForward__maxdelay ? this->allpass_tilde_02_feedForward__maxdelay : (size < (this->allpass_tilde_02_feedForward_reader != this->allpass_tilde_02_feedForward_writer) ? this->allpass_tilde_02_feedForward_reader != this->allpass_tilde_02_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ), this->allpass_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_02_feedForward_buffer->getSize()) + this->allpass_tilde_02_feedForward_reader - ((size > this->allpass_tilde_02_feedForward__maxdelay ? this->allpass_tilde_02_feedForward__maxdelay : (size < (this->allpass_tilde_02_feedForward_reader != this->allpass_tilde_02_feedForward_writer) ? this->allpass_tilde_02_feedForward_reader != this->allpass_tilde_02_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_02_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_02_feedForward_wrap))
        );
    }
    
    void allpass_tilde_02_feedForward_write(number v) {
        this->allpass_tilde_02_feedForward_writer = this->allpass_tilde_02_feedForward_reader;
        this->allpass_tilde_02_feedForward_buffer[(Index)this->allpass_tilde_02_feedForward_writer] = v;
    }
    
    number allpass_tilde_02_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_02_feedForward__maxdelay : size);
        number val = this->allpass_tilde_02_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_02_feedForward_write(v);
        this->allpass_tilde_02_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_02_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_02_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_02_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_02_feedForward_init() {
        auto result = this->allpass_tilde_02_feedForward_calcSizeInSamples();
        this->allpass_tilde_02_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_02_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_02_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_02_feedForward_clear() {
        this->allpass_tilde_02_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_02_feedForward_reset() {
        auto result = this->allpass_tilde_02_feedForward_calcSizeInSamples();
        this->allpass_tilde_02_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_02_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_02_feedForward_buffer);
        this->allpass_tilde_02_feedForward_wrap = this->allpass_tilde_02_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_02_feedForward_clear();
    
        if (this->allpass_tilde_02_feedForward_reader >= this->allpass_tilde_02_feedForward__maxdelay || this->allpass_tilde_02_feedForward_writer >= this->allpass_tilde_02_feedForward__maxdelay) {
            this->allpass_tilde_02_feedForward_reader = 0;
            this->allpass_tilde_02_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_02_feedForward_dspsetup() {
        this->allpass_tilde_02_feedForward_reset();
    }
    
    number allpass_tilde_02_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_02_feedForward_size() {
        return this->allpass_tilde_02_feedForward__maxdelay;
    }
    
    void allpass_tilde_02_feedBack_step() {
        this->allpass_tilde_02_feedBack_reader++;
    
        if (this->allpass_tilde_02_feedBack_reader >= (int)(this->allpass_tilde_02_feedBack_buffer->getSize()))
            this->allpass_tilde_02_feedBack_reader = 0;
    }
    
    number allpass_tilde_02_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_02_feedBack_buffer->getSize()) + this->allpass_tilde_02_feedBack_reader - ((size > this->allpass_tilde_02_feedBack__maxdelay ? this->allpass_tilde_02_feedBack__maxdelay : (size < (this->allpass_tilde_02_feedBack_reader != this->allpass_tilde_02_feedBack_writer) ? this->allpass_tilde_02_feedBack_reader != this->allpass_tilde_02_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ), this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_02_feedBack_buffer->getSize()) + this->allpass_tilde_02_feedBack_reader - ((size > this->allpass_tilde_02_feedBack__maxdelay ? this->allpass_tilde_02_feedBack__maxdelay : (size < (1 + this->allpass_tilde_02_feedBack_reader != this->allpass_tilde_02_feedBack_writer) ? 1 + this->allpass_tilde_02_feedBack_reader != this->allpass_tilde_02_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ), this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ), this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ), this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_02_feedBack_buffer->getSize()) + this->allpass_tilde_02_feedBack_reader - ((size > this->allpass_tilde_02_feedBack__maxdelay ? this->allpass_tilde_02_feedBack__maxdelay : (size < (1 + this->allpass_tilde_02_feedBack_reader != this->allpass_tilde_02_feedBack_writer) ? 1 + this->allpass_tilde_02_feedBack_reader != this->allpass_tilde_02_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ), this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ), this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ), this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_02_feedBack_buffer->getSize()) + this->allpass_tilde_02_feedBack_reader - ((size > this->allpass_tilde_02_feedBack__maxdelay ? this->allpass_tilde_02_feedBack__maxdelay : (size < (this->allpass_tilde_02_feedBack_reader != this->allpass_tilde_02_feedBack_writer) ? this->allpass_tilde_02_feedBack_reader != this->allpass_tilde_02_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ), this->allpass_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_02_feedBack_buffer->getSize()) + this->allpass_tilde_02_feedBack_reader - ((size > this->allpass_tilde_02_feedBack__maxdelay ? this->allpass_tilde_02_feedBack__maxdelay : (size < (this->allpass_tilde_02_feedBack_reader != this->allpass_tilde_02_feedBack_writer) ? this->allpass_tilde_02_feedBack_reader != this->allpass_tilde_02_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_02_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_02_feedBack_wrap))
        );
    }
    
    void allpass_tilde_02_feedBack_write(number v) {
        this->allpass_tilde_02_feedBack_writer = this->allpass_tilde_02_feedBack_reader;
        this->allpass_tilde_02_feedBack_buffer[(Index)this->allpass_tilde_02_feedBack_writer] = v;
    }
    
    number allpass_tilde_02_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_02_feedBack__maxdelay : size);
        number val = this->allpass_tilde_02_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_02_feedBack_write(v);
        this->allpass_tilde_02_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_02_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_02_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_02_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_02_feedBack_init() {
        auto result = this->allpass_tilde_02_feedBack_calcSizeInSamples();
        this->allpass_tilde_02_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_02_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_02_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_02_feedBack_clear() {
        this->allpass_tilde_02_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_02_feedBack_reset() {
        auto result = this->allpass_tilde_02_feedBack_calcSizeInSamples();
        this->allpass_tilde_02_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_02_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_02_feedBack_buffer);
        this->allpass_tilde_02_feedBack_wrap = this->allpass_tilde_02_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_02_feedBack_clear();
    
        if (this->allpass_tilde_02_feedBack_reader >= this->allpass_tilde_02_feedBack__maxdelay || this->allpass_tilde_02_feedBack_writer >= this->allpass_tilde_02_feedBack__maxdelay) {
            this->allpass_tilde_02_feedBack_reader = 0;
            this->allpass_tilde_02_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_02_feedBack_dspsetup() {
        this->allpass_tilde_02_feedBack_reset();
    }
    
    number allpass_tilde_02_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_02_feedBack_size() {
        return this->allpass_tilde_02_feedBack__maxdelay;
    }
    
    void allpass_tilde_02_init() {
        this->allpass_tilde_02_feedForward_init();
        this->allpass_tilde_02_feedBack_init();
    }
    
    void allpass_tilde_02_clear() {
        this->allpass_tilde_02_feedForward_clear();
        this->allpass_tilde_02_feedBack_clear();
    }
    
    void allpass_tilde_02_reset() {
        this->allpass_tilde_02_init();
        this->allpass_tilde_02_clear();
    }
    
    void allpass_tilde_02_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_02_reset();
        this->allpass_tilde_02_setupDone = true;
        this->allpass_tilde_02_feedForward_dspsetup();
        this->allpass_tilde_02_feedBack_dspsetup();
    }
    
    void allpass_tilde_03_feedForward_step() {
        this->allpass_tilde_03_feedForward_reader++;
    
        if (this->allpass_tilde_03_feedForward_reader >= (int)(this->allpass_tilde_03_feedForward_buffer->getSize()))
            this->allpass_tilde_03_feedForward_reader = 0;
    }
    
    number allpass_tilde_03_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_03_feedForward_buffer->getSize()) + this->allpass_tilde_03_feedForward_reader - ((size > this->allpass_tilde_03_feedForward__maxdelay ? this->allpass_tilde_03_feedForward__maxdelay : (size < (this->allpass_tilde_03_feedForward_reader != this->allpass_tilde_03_feedForward_writer) ? this->allpass_tilde_03_feedForward_reader != this->allpass_tilde_03_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ), this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_03_feedForward_buffer->getSize()) + this->allpass_tilde_03_feedForward_reader - ((size > this->allpass_tilde_03_feedForward__maxdelay ? this->allpass_tilde_03_feedForward__maxdelay : (size < (1 + this->allpass_tilde_03_feedForward_reader != this->allpass_tilde_03_feedForward_writer) ? 1 + this->allpass_tilde_03_feedForward_reader != this->allpass_tilde_03_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ), this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ), this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ), this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_03_feedForward_buffer->getSize()) + this->allpass_tilde_03_feedForward_reader - ((size > this->allpass_tilde_03_feedForward__maxdelay ? this->allpass_tilde_03_feedForward__maxdelay : (size < (1 + this->allpass_tilde_03_feedForward_reader != this->allpass_tilde_03_feedForward_writer) ? 1 + this->allpass_tilde_03_feedForward_reader != this->allpass_tilde_03_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ), this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ), this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ), this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_03_feedForward_buffer->getSize()) + this->allpass_tilde_03_feedForward_reader - ((size > this->allpass_tilde_03_feedForward__maxdelay ? this->allpass_tilde_03_feedForward__maxdelay : (size < (this->allpass_tilde_03_feedForward_reader != this->allpass_tilde_03_feedForward_writer) ? this->allpass_tilde_03_feedForward_reader != this->allpass_tilde_03_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ), this->allpass_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_03_feedForward_buffer->getSize()) + this->allpass_tilde_03_feedForward_reader - ((size > this->allpass_tilde_03_feedForward__maxdelay ? this->allpass_tilde_03_feedForward__maxdelay : (size < (this->allpass_tilde_03_feedForward_reader != this->allpass_tilde_03_feedForward_writer) ? this->allpass_tilde_03_feedForward_reader != this->allpass_tilde_03_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_03_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_03_feedForward_wrap))
        );
    }
    
    void allpass_tilde_03_feedForward_write(number v) {
        this->allpass_tilde_03_feedForward_writer = this->allpass_tilde_03_feedForward_reader;
        this->allpass_tilde_03_feedForward_buffer[(Index)this->allpass_tilde_03_feedForward_writer] = v;
    }
    
    number allpass_tilde_03_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_03_feedForward__maxdelay : size);
        number val = this->allpass_tilde_03_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_03_feedForward_write(v);
        this->allpass_tilde_03_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_03_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_03_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_03_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_03_feedForward_init() {
        auto result = this->allpass_tilde_03_feedForward_calcSizeInSamples();
        this->allpass_tilde_03_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_03_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_03_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_03_feedForward_clear() {
        this->allpass_tilde_03_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_03_feedForward_reset() {
        auto result = this->allpass_tilde_03_feedForward_calcSizeInSamples();
        this->allpass_tilde_03_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_03_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_03_feedForward_buffer);
        this->allpass_tilde_03_feedForward_wrap = this->allpass_tilde_03_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_03_feedForward_clear();
    
        if (this->allpass_tilde_03_feedForward_reader >= this->allpass_tilde_03_feedForward__maxdelay || this->allpass_tilde_03_feedForward_writer >= this->allpass_tilde_03_feedForward__maxdelay) {
            this->allpass_tilde_03_feedForward_reader = 0;
            this->allpass_tilde_03_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_03_feedForward_dspsetup() {
        this->allpass_tilde_03_feedForward_reset();
    }
    
    number allpass_tilde_03_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_03_feedForward_size() {
        return this->allpass_tilde_03_feedForward__maxdelay;
    }
    
    void allpass_tilde_03_feedBack_step() {
        this->allpass_tilde_03_feedBack_reader++;
    
        if (this->allpass_tilde_03_feedBack_reader >= (int)(this->allpass_tilde_03_feedBack_buffer->getSize()))
            this->allpass_tilde_03_feedBack_reader = 0;
    }
    
    number allpass_tilde_03_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_03_feedBack_buffer->getSize()) + this->allpass_tilde_03_feedBack_reader - ((size > this->allpass_tilde_03_feedBack__maxdelay ? this->allpass_tilde_03_feedBack__maxdelay : (size < (this->allpass_tilde_03_feedBack_reader != this->allpass_tilde_03_feedBack_writer) ? this->allpass_tilde_03_feedBack_reader != this->allpass_tilde_03_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ), this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_03_feedBack_buffer->getSize()) + this->allpass_tilde_03_feedBack_reader - ((size > this->allpass_tilde_03_feedBack__maxdelay ? this->allpass_tilde_03_feedBack__maxdelay : (size < (1 + this->allpass_tilde_03_feedBack_reader != this->allpass_tilde_03_feedBack_writer) ? 1 + this->allpass_tilde_03_feedBack_reader != this->allpass_tilde_03_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ), this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ), this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ), this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_03_feedBack_buffer->getSize()) + this->allpass_tilde_03_feedBack_reader - ((size > this->allpass_tilde_03_feedBack__maxdelay ? this->allpass_tilde_03_feedBack__maxdelay : (size < (1 + this->allpass_tilde_03_feedBack_reader != this->allpass_tilde_03_feedBack_writer) ? 1 + this->allpass_tilde_03_feedBack_reader != this->allpass_tilde_03_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ), this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ), this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ), this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_03_feedBack_buffer->getSize()) + this->allpass_tilde_03_feedBack_reader - ((size > this->allpass_tilde_03_feedBack__maxdelay ? this->allpass_tilde_03_feedBack__maxdelay : (size < (this->allpass_tilde_03_feedBack_reader != this->allpass_tilde_03_feedBack_writer) ? this->allpass_tilde_03_feedBack_reader != this->allpass_tilde_03_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ), this->allpass_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_03_feedBack_buffer->getSize()) + this->allpass_tilde_03_feedBack_reader - ((size > this->allpass_tilde_03_feedBack__maxdelay ? this->allpass_tilde_03_feedBack__maxdelay : (size < (this->allpass_tilde_03_feedBack_reader != this->allpass_tilde_03_feedBack_writer) ? this->allpass_tilde_03_feedBack_reader != this->allpass_tilde_03_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_03_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_03_feedBack_wrap))
        );
    }
    
    void allpass_tilde_03_feedBack_write(number v) {
        this->allpass_tilde_03_feedBack_writer = this->allpass_tilde_03_feedBack_reader;
        this->allpass_tilde_03_feedBack_buffer[(Index)this->allpass_tilde_03_feedBack_writer] = v;
    }
    
    number allpass_tilde_03_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_03_feedBack__maxdelay : size);
        number val = this->allpass_tilde_03_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_03_feedBack_write(v);
        this->allpass_tilde_03_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_03_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_03_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_03_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_03_feedBack_init() {
        auto result = this->allpass_tilde_03_feedBack_calcSizeInSamples();
        this->allpass_tilde_03_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_03_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_03_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_03_feedBack_clear() {
        this->allpass_tilde_03_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_03_feedBack_reset() {
        auto result = this->allpass_tilde_03_feedBack_calcSizeInSamples();
        this->allpass_tilde_03_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_03_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_03_feedBack_buffer);
        this->allpass_tilde_03_feedBack_wrap = this->allpass_tilde_03_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_03_feedBack_clear();
    
        if (this->allpass_tilde_03_feedBack_reader >= this->allpass_tilde_03_feedBack__maxdelay || this->allpass_tilde_03_feedBack_writer >= this->allpass_tilde_03_feedBack__maxdelay) {
            this->allpass_tilde_03_feedBack_reader = 0;
            this->allpass_tilde_03_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_03_feedBack_dspsetup() {
        this->allpass_tilde_03_feedBack_reset();
    }
    
    number allpass_tilde_03_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_03_feedBack_size() {
        return this->allpass_tilde_03_feedBack__maxdelay;
    }
    
    void allpass_tilde_03_init() {
        this->allpass_tilde_03_feedForward_init();
        this->allpass_tilde_03_feedBack_init();
    }
    
    void allpass_tilde_03_clear() {
        this->allpass_tilde_03_feedForward_clear();
        this->allpass_tilde_03_feedBack_clear();
    }
    
    void allpass_tilde_03_reset() {
        this->allpass_tilde_03_init();
        this->allpass_tilde_03_clear();
    }
    
    void allpass_tilde_03_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_03_reset();
        this->allpass_tilde_03_setupDone = true;
        this->allpass_tilde_03_feedForward_dspsetup();
        this->allpass_tilde_03_feedBack_dspsetup();
    }
    
    void allpass_tilde_04_feedForward_step() {
        this->allpass_tilde_04_feedForward_reader++;
    
        if (this->allpass_tilde_04_feedForward_reader >= (int)(this->allpass_tilde_04_feedForward_buffer->getSize()))
            this->allpass_tilde_04_feedForward_reader = 0;
    }
    
    number allpass_tilde_04_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_04_feedForward_buffer->getSize()) + this->allpass_tilde_04_feedForward_reader - ((size > this->allpass_tilde_04_feedForward__maxdelay ? this->allpass_tilde_04_feedForward__maxdelay : (size < (this->allpass_tilde_04_feedForward_reader != this->allpass_tilde_04_feedForward_writer) ? this->allpass_tilde_04_feedForward_reader != this->allpass_tilde_04_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ), this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_04_feedForward_buffer->getSize()) + this->allpass_tilde_04_feedForward_reader - ((size > this->allpass_tilde_04_feedForward__maxdelay ? this->allpass_tilde_04_feedForward__maxdelay : (size < (1 + this->allpass_tilde_04_feedForward_reader != this->allpass_tilde_04_feedForward_writer) ? 1 + this->allpass_tilde_04_feedForward_reader != this->allpass_tilde_04_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ), this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ), this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ), this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_04_feedForward_buffer->getSize()) + this->allpass_tilde_04_feedForward_reader - ((size > this->allpass_tilde_04_feedForward__maxdelay ? this->allpass_tilde_04_feedForward__maxdelay : (size < (1 + this->allpass_tilde_04_feedForward_reader != this->allpass_tilde_04_feedForward_writer) ? 1 + this->allpass_tilde_04_feedForward_reader != this->allpass_tilde_04_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ), this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ), this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ), this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_04_feedForward_buffer->getSize()) + this->allpass_tilde_04_feedForward_reader - ((size > this->allpass_tilde_04_feedForward__maxdelay ? this->allpass_tilde_04_feedForward__maxdelay : (size < (this->allpass_tilde_04_feedForward_reader != this->allpass_tilde_04_feedForward_writer) ? this->allpass_tilde_04_feedForward_reader != this->allpass_tilde_04_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ), this->allpass_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_04_feedForward_buffer->getSize()) + this->allpass_tilde_04_feedForward_reader - ((size > this->allpass_tilde_04_feedForward__maxdelay ? this->allpass_tilde_04_feedForward__maxdelay : (size < (this->allpass_tilde_04_feedForward_reader != this->allpass_tilde_04_feedForward_writer) ? this->allpass_tilde_04_feedForward_reader != this->allpass_tilde_04_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_04_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_04_feedForward_wrap))
        );
    }
    
    void allpass_tilde_04_feedForward_write(number v) {
        this->allpass_tilde_04_feedForward_writer = this->allpass_tilde_04_feedForward_reader;
        this->allpass_tilde_04_feedForward_buffer[(Index)this->allpass_tilde_04_feedForward_writer] = v;
    }
    
    number allpass_tilde_04_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_04_feedForward__maxdelay : size);
        number val = this->allpass_tilde_04_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_04_feedForward_write(v);
        this->allpass_tilde_04_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_04_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_04_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_04_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_04_feedForward_init() {
        auto result = this->allpass_tilde_04_feedForward_calcSizeInSamples();
        this->allpass_tilde_04_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_04_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_04_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_04_feedForward_clear() {
        this->allpass_tilde_04_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_04_feedForward_reset() {
        auto result = this->allpass_tilde_04_feedForward_calcSizeInSamples();
        this->allpass_tilde_04_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_04_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_04_feedForward_buffer);
        this->allpass_tilde_04_feedForward_wrap = this->allpass_tilde_04_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_04_feedForward_clear();
    
        if (this->allpass_tilde_04_feedForward_reader >= this->allpass_tilde_04_feedForward__maxdelay || this->allpass_tilde_04_feedForward_writer >= this->allpass_tilde_04_feedForward__maxdelay) {
            this->allpass_tilde_04_feedForward_reader = 0;
            this->allpass_tilde_04_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_04_feedForward_dspsetup() {
        this->allpass_tilde_04_feedForward_reset();
    }
    
    number allpass_tilde_04_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_04_feedForward_size() {
        return this->allpass_tilde_04_feedForward__maxdelay;
    }
    
    void allpass_tilde_04_feedBack_step() {
        this->allpass_tilde_04_feedBack_reader++;
    
        if (this->allpass_tilde_04_feedBack_reader >= (int)(this->allpass_tilde_04_feedBack_buffer->getSize()))
            this->allpass_tilde_04_feedBack_reader = 0;
    }
    
    number allpass_tilde_04_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_04_feedBack_buffer->getSize()) + this->allpass_tilde_04_feedBack_reader - ((size > this->allpass_tilde_04_feedBack__maxdelay ? this->allpass_tilde_04_feedBack__maxdelay : (size < (this->allpass_tilde_04_feedBack_reader != this->allpass_tilde_04_feedBack_writer) ? this->allpass_tilde_04_feedBack_reader != this->allpass_tilde_04_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ), this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_04_feedBack_buffer->getSize()) + this->allpass_tilde_04_feedBack_reader - ((size > this->allpass_tilde_04_feedBack__maxdelay ? this->allpass_tilde_04_feedBack__maxdelay : (size < (1 + this->allpass_tilde_04_feedBack_reader != this->allpass_tilde_04_feedBack_writer) ? 1 + this->allpass_tilde_04_feedBack_reader != this->allpass_tilde_04_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ), this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ), this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ), this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_04_feedBack_buffer->getSize()) + this->allpass_tilde_04_feedBack_reader - ((size > this->allpass_tilde_04_feedBack__maxdelay ? this->allpass_tilde_04_feedBack__maxdelay : (size < (1 + this->allpass_tilde_04_feedBack_reader != this->allpass_tilde_04_feedBack_writer) ? 1 + this->allpass_tilde_04_feedBack_reader != this->allpass_tilde_04_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ), this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ), this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ), this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_04_feedBack_buffer->getSize()) + this->allpass_tilde_04_feedBack_reader - ((size > this->allpass_tilde_04_feedBack__maxdelay ? this->allpass_tilde_04_feedBack__maxdelay : (size < (this->allpass_tilde_04_feedBack_reader != this->allpass_tilde_04_feedBack_writer) ? this->allpass_tilde_04_feedBack_reader != this->allpass_tilde_04_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ), this->allpass_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_04_feedBack_buffer->getSize()) + this->allpass_tilde_04_feedBack_reader - ((size > this->allpass_tilde_04_feedBack__maxdelay ? this->allpass_tilde_04_feedBack__maxdelay : (size < (this->allpass_tilde_04_feedBack_reader != this->allpass_tilde_04_feedBack_writer) ? this->allpass_tilde_04_feedBack_reader != this->allpass_tilde_04_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_04_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_04_feedBack_wrap))
        );
    }
    
    void allpass_tilde_04_feedBack_write(number v) {
        this->allpass_tilde_04_feedBack_writer = this->allpass_tilde_04_feedBack_reader;
        this->allpass_tilde_04_feedBack_buffer[(Index)this->allpass_tilde_04_feedBack_writer] = v;
    }
    
    number allpass_tilde_04_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_04_feedBack__maxdelay : size);
        number val = this->allpass_tilde_04_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_04_feedBack_write(v);
        this->allpass_tilde_04_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_04_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_04_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_04_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_04_feedBack_init() {
        auto result = this->allpass_tilde_04_feedBack_calcSizeInSamples();
        this->allpass_tilde_04_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_04_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_04_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_04_feedBack_clear() {
        this->allpass_tilde_04_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_04_feedBack_reset() {
        auto result = this->allpass_tilde_04_feedBack_calcSizeInSamples();
        this->allpass_tilde_04_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_04_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_04_feedBack_buffer);
        this->allpass_tilde_04_feedBack_wrap = this->allpass_tilde_04_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_04_feedBack_clear();
    
        if (this->allpass_tilde_04_feedBack_reader >= this->allpass_tilde_04_feedBack__maxdelay || this->allpass_tilde_04_feedBack_writer >= this->allpass_tilde_04_feedBack__maxdelay) {
            this->allpass_tilde_04_feedBack_reader = 0;
            this->allpass_tilde_04_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_04_feedBack_dspsetup() {
        this->allpass_tilde_04_feedBack_reset();
    }
    
    number allpass_tilde_04_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_04_feedBack_size() {
        return this->allpass_tilde_04_feedBack__maxdelay;
    }
    
    void allpass_tilde_04_init() {
        this->allpass_tilde_04_feedForward_init();
        this->allpass_tilde_04_feedBack_init();
    }
    
    void allpass_tilde_04_clear() {
        this->allpass_tilde_04_feedForward_clear();
        this->allpass_tilde_04_feedBack_clear();
    }
    
    void allpass_tilde_04_reset() {
        this->allpass_tilde_04_init();
        this->allpass_tilde_04_clear();
    }
    
    void allpass_tilde_04_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_04_reset();
        this->allpass_tilde_04_setupDone = true;
        this->allpass_tilde_04_feedForward_dspsetup();
        this->allpass_tilde_04_feedBack_dspsetup();
    }
    
    void allpass_tilde_05_feedForward_step() {
        this->allpass_tilde_05_feedForward_reader++;
    
        if (this->allpass_tilde_05_feedForward_reader >= (int)(this->allpass_tilde_05_feedForward_buffer->getSize()))
            this->allpass_tilde_05_feedForward_reader = 0;
    }
    
    number allpass_tilde_05_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_05_feedForward_buffer->getSize()) + this->allpass_tilde_05_feedForward_reader - ((size > this->allpass_tilde_05_feedForward__maxdelay ? this->allpass_tilde_05_feedForward__maxdelay : (size < (this->allpass_tilde_05_feedForward_reader != this->allpass_tilde_05_feedForward_writer) ? this->allpass_tilde_05_feedForward_reader != this->allpass_tilde_05_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ), this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_05_feedForward_buffer->getSize()) + this->allpass_tilde_05_feedForward_reader - ((size > this->allpass_tilde_05_feedForward__maxdelay ? this->allpass_tilde_05_feedForward__maxdelay : (size < (1 + this->allpass_tilde_05_feedForward_reader != this->allpass_tilde_05_feedForward_writer) ? 1 + this->allpass_tilde_05_feedForward_reader != this->allpass_tilde_05_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ), this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ), this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ), this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_05_feedForward_buffer->getSize()) + this->allpass_tilde_05_feedForward_reader - ((size > this->allpass_tilde_05_feedForward__maxdelay ? this->allpass_tilde_05_feedForward__maxdelay : (size < (1 + this->allpass_tilde_05_feedForward_reader != this->allpass_tilde_05_feedForward_writer) ? 1 + this->allpass_tilde_05_feedForward_reader != this->allpass_tilde_05_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ), this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ), this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ), this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_05_feedForward_buffer->getSize()) + this->allpass_tilde_05_feedForward_reader - ((size > this->allpass_tilde_05_feedForward__maxdelay ? this->allpass_tilde_05_feedForward__maxdelay : (size < (this->allpass_tilde_05_feedForward_reader != this->allpass_tilde_05_feedForward_writer) ? this->allpass_tilde_05_feedForward_reader != this->allpass_tilde_05_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ), this->allpass_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_05_feedForward_buffer->getSize()) + this->allpass_tilde_05_feedForward_reader - ((size > this->allpass_tilde_05_feedForward__maxdelay ? this->allpass_tilde_05_feedForward__maxdelay : (size < (this->allpass_tilde_05_feedForward_reader != this->allpass_tilde_05_feedForward_writer) ? this->allpass_tilde_05_feedForward_reader != this->allpass_tilde_05_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_05_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_05_feedForward_wrap))
        );
    }
    
    void allpass_tilde_05_feedForward_write(number v) {
        this->allpass_tilde_05_feedForward_writer = this->allpass_tilde_05_feedForward_reader;
        this->allpass_tilde_05_feedForward_buffer[(Index)this->allpass_tilde_05_feedForward_writer] = v;
    }
    
    number allpass_tilde_05_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_05_feedForward__maxdelay : size);
        number val = this->allpass_tilde_05_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_05_feedForward_write(v);
        this->allpass_tilde_05_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_05_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_05_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_05_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_05_feedForward_init() {
        auto result = this->allpass_tilde_05_feedForward_calcSizeInSamples();
        this->allpass_tilde_05_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_05_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_05_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_05_feedForward_clear() {
        this->allpass_tilde_05_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_05_feedForward_reset() {
        auto result = this->allpass_tilde_05_feedForward_calcSizeInSamples();
        this->allpass_tilde_05_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_05_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_05_feedForward_buffer);
        this->allpass_tilde_05_feedForward_wrap = this->allpass_tilde_05_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_05_feedForward_clear();
    
        if (this->allpass_tilde_05_feedForward_reader >= this->allpass_tilde_05_feedForward__maxdelay || this->allpass_tilde_05_feedForward_writer >= this->allpass_tilde_05_feedForward__maxdelay) {
            this->allpass_tilde_05_feedForward_reader = 0;
            this->allpass_tilde_05_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_05_feedForward_dspsetup() {
        this->allpass_tilde_05_feedForward_reset();
    }
    
    number allpass_tilde_05_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_05_feedForward_size() {
        return this->allpass_tilde_05_feedForward__maxdelay;
    }
    
    void allpass_tilde_05_feedBack_step() {
        this->allpass_tilde_05_feedBack_reader++;
    
        if (this->allpass_tilde_05_feedBack_reader >= (int)(this->allpass_tilde_05_feedBack_buffer->getSize()))
            this->allpass_tilde_05_feedBack_reader = 0;
    }
    
    number allpass_tilde_05_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_05_feedBack_buffer->getSize()) + this->allpass_tilde_05_feedBack_reader - ((size > this->allpass_tilde_05_feedBack__maxdelay ? this->allpass_tilde_05_feedBack__maxdelay : (size < (this->allpass_tilde_05_feedBack_reader != this->allpass_tilde_05_feedBack_writer) ? this->allpass_tilde_05_feedBack_reader != this->allpass_tilde_05_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ), this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_05_feedBack_buffer->getSize()) + this->allpass_tilde_05_feedBack_reader - ((size > this->allpass_tilde_05_feedBack__maxdelay ? this->allpass_tilde_05_feedBack__maxdelay : (size < (1 + this->allpass_tilde_05_feedBack_reader != this->allpass_tilde_05_feedBack_writer) ? 1 + this->allpass_tilde_05_feedBack_reader != this->allpass_tilde_05_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ), this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ), this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ), this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_05_feedBack_buffer->getSize()) + this->allpass_tilde_05_feedBack_reader - ((size > this->allpass_tilde_05_feedBack__maxdelay ? this->allpass_tilde_05_feedBack__maxdelay : (size < (1 + this->allpass_tilde_05_feedBack_reader != this->allpass_tilde_05_feedBack_writer) ? 1 + this->allpass_tilde_05_feedBack_reader != this->allpass_tilde_05_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ), this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ), this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ), this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_05_feedBack_buffer->getSize()) + this->allpass_tilde_05_feedBack_reader - ((size > this->allpass_tilde_05_feedBack__maxdelay ? this->allpass_tilde_05_feedBack__maxdelay : (size < (this->allpass_tilde_05_feedBack_reader != this->allpass_tilde_05_feedBack_writer) ? this->allpass_tilde_05_feedBack_reader != this->allpass_tilde_05_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ), this->allpass_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_05_feedBack_buffer->getSize()) + this->allpass_tilde_05_feedBack_reader - ((size > this->allpass_tilde_05_feedBack__maxdelay ? this->allpass_tilde_05_feedBack__maxdelay : (size < (this->allpass_tilde_05_feedBack_reader != this->allpass_tilde_05_feedBack_writer) ? this->allpass_tilde_05_feedBack_reader != this->allpass_tilde_05_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_05_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_05_feedBack_wrap))
        );
    }
    
    void allpass_tilde_05_feedBack_write(number v) {
        this->allpass_tilde_05_feedBack_writer = this->allpass_tilde_05_feedBack_reader;
        this->allpass_tilde_05_feedBack_buffer[(Index)this->allpass_tilde_05_feedBack_writer] = v;
    }
    
    number allpass_tilde_05_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_05_feedBack__maxdelay : size);
        number val = this->allpass_tilde_05_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_05_feedBack_write(v);
        this->allpass_tilde_05_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_05_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_05_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_05_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_05_feedBack_init() {
        auto result = this->allpass_tilde_05_feedBack_calcSizeInSamples();
        this->allpass_tilde_05_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_05_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_05_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_05_feedBack_clear() {
        this->allpass_tilde_05_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_05_feedBack_reset() {
        auto result = this->allpass_tilde_05_feedBack_calcSizeInSamples();
        this->allpass_tilde_05_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_05_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_05_feedBack_buffer);
        this->allpass_tilde_05_feedBack_wrap = this->allpass_tilde_05_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_05_feedBack_clear();
    
        if (this->allpass_tilde_05_feedBack_reader >= this->allpass_tilde_05_feedBack__maxdelay || this->allpass_tilde_05_feedBack_writer >= this->allpass_tilde_05_feedBack__maxdelay) {
            this->allpass_tilde_05_feedBack_reader = 0;
            this->allpass_tilde_05_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_05_feedBack_dspsetup() {
        this->allpass_tilde_05_feedBack_reset();
    }
    
    number allpass_tilde_05_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_05_feedBack_size() {
        return this->allpass_tilde_05_feedBack__maxdelay;
    }
    
    void allpass_tilde_05_init() {
        this->allpass_tilde_05_feedForward_init();
        this->allpass_tilde_05_feedBack_init();
    }
    
    void allpass_tilde_05_clear() {
        this->allpass_tilde_05_feedForward_clear();
        this->allpass_tilde_05_feedBack_clear();
    }
    
    void allpass_tilde_05_reset() {
        this->allpass_tilde_05_init();
        this->allpass_tilde_05_clear();
    }
    
    void allpass_tilde_05_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_05_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_05_reset();
        this->allpass_tilde_05_setupDone = true;
        this->allpass_tilde_05_feedForward_dspsetup();
        this->allpass_tilde_05_feedBack_dspsetup();
    }
    
    void allpass_tilde_06_feedForward_step() {
        this->allpass_tilde_06_feedForward_reader++;
    
        if (this->allpass_tilde_06_feedForward_reader >= (int)(this->allpass_tilde_06_feedForward_buffer->getSize()))
            this->allpass_tilde_06_feedForward_reader = 0;
    }
    
    number allpass_tilde_06_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_06_feedForward_buffer->getSize()) + this->allpass_tilde_06_feedForward_reader - ((size > this->allpass_tilde_06_feedForward__maxdelay ? this->allpass_tilde_06_feedForward__maxdelay : (size < (this->allpass_tilde_06_feedForward_reader != this->allpass_tilde_06_feedForward_writer) ? this->allpass_tilde_06_feedForward_reader != this->allpass_tilde_06_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ), this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_06_feedForward_buffer->getSize()) + this->allpass_tilde_06_feedForward_reader - ((size > this->allpass_tilde_06_feedForward__maxdelay ? this->allpass_tilde_06_feedForward__maxdelay : (size < (1 + this->allpass_tilde_06_feedForward_reader != this->allpass_tilde_06_feedForward_writer) ? 1 + this->allpass_tilde_06_feedForward_reader != this->allpass_tilde_06_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ), this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ), this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ), this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_06_feedForward_buffer->getSize()) + this->allpass_tilde_06_feedForward_reader - ((size > this->allpass_tilde_06_feedForward__maxdelay ? this->allpass_tilde_06_feedForward__maxdelay : (size < (1 + this->allpass_tilde_06_feedForward_reader != this->allpass_tilde_06_feedForward_writer) ? 1 + this->allpass_tilde_06_feedForward_reader != this->allpass_tilde_06_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ), this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ), this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ), this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_06_feedForward_buffer->getSize()) + this->allpass_tilde_06_feedForward_reader - ((size > this->allpass_tilde_06_feedForward__maxdelay ? this->allpass_tilde_06_feedForward__maxdelay : (size < (this->allpass_tilde_06_feedForward_reader != this->allpass_tilde_06_feedForward_writer) ? this->allpass_tilde_06_feedForward_reader != this->allpass_tilde_06_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ), this->allpass_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_06_feedForward_buffer->getSize()) + this->allpass_tilde_06_feedForward_reader - ((size > this->allpass_tilde_06_feedForward__maxdelay ? this->allpass_tilde_06_feedForward__maxdelay : (size < (this->allpass_tilde_06_feedForward_reader != this->allpass_tilde_06_feedForward_writer) ? this->allpass_tilde_06_feedForward_reader != this->allpass_tilde_06_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_06_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_06_feedForward_wrap))
        );
    }
    
    void allpass_tilde_06_feedForward_write(number v) {
        this->allpass_tilde_06_feedForward_writer = this->allpass_tilde_06_feedForward_reader;
        this->allpass_tilde_06_feedForward_buffer[(Index)this->allpass_tilde_06_feedForward_writer] = v;
    }
    
    number allpass_tilde_06_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_06_feedForward__maxdelay : size);
        number val = this->allpass_tilde_06_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_06_feedForward_write(v);
        this->allpass_tilde_06_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_06_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_06_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_06_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_06_feedForward_init() {
        auto result = this->allpass_tilde_06_feedForward_calcSizeInSamples();
        this->allpass_tilde_06_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_06_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_06_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_06_feedForward_clear() {
        this->allpass_tilde_06_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_06_feedForward_reset() {
        auto result = this->allpass_tilde_06_feedForward_calcSizeInSamples();
        this->allpass_tilde_06_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_06_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_06_feedForward_buffer);
        this->allpass_tilde_06_feedForward_wrap = this->allpass_tilde_06_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_06_feedForward_clear();
    
        if (this->allpass_tilde_06_feedForward_reader >= this->allpass_tilde_06_feedForward__maxdelay || this->allpass_tilde_06_feedForward_writer >= this->allpass_tilde_06_feedForward__maxdelay) {
            this->allpass_tilde_06_feedForward_reader = 0;
            this->allpass_tilde_06_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_06_feedForward_dspsetup() {
        this->allpass_tilde_06_feedForward_reset();
    }
    
    number allpass_tilde_06_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_06_feedForward_size() {
        return this->allpass_tilde_06_feedForward__maxdelay;
    }
    
    void allpass_tilde_06_feedBack_step() {
        this->allpass_tilde_06_feedBack_reader++;
    
        if (this->allpass_tilde_06_feedBack_reader >= (int)(this->allpass_tilde_06_feedBack_buffer->getSize()))
            this->allpass_tilde_06_feedBack_reader = 0;
    }
    
    number allpass_tilde_06_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_06_feedBack_buffer->getSize()) + this->allpass_tilde_06_feedBack_reader - ((size > this->allpass_tilde_06_feedBack__maxdelay ? this->allpass_tilde_06_feedBack__maxdelay : (size < (this->allpass_tilde_06_feedBack_reader != this->allpass_tilde_06_feedBack_writer) ? this->allpass_tilde_06_feedBack_reader != this->allpass_tilde_06_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ), this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_06_feedBack_buffer->getSize()) + this->allpass_tilde_06_feedBack_reader - ((size > this->allpass_tilde_06_feedBack__maxdelay ? this->allpass_tilde_06_feedBack__maxdelay : (size < (1 + this->allpass_tilde_06_feedBack_reader != this->allpass_tilde_06_feedBack_writer) ? 1 + this->allpass_tilde_06_feedBack_reader != this->allpass_tilde_06_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ), this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ), this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ), this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_06_feedBack_buffer->getSize()) + this->allpass_tilde_06_feedBack_reader - ((size > this->allpass_tilde_06_feedBack__maxdelay ? this->allpass_tilde_06_feedBack__maxdelay : (size < (1 + this->allpass_tilde_06_feedBack_reader != this->allpass_tilde_06_feedBack_writer) ? 1 + this->allpass_tilde_06_feedBack_reader != this->allpass_tilde_06_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ), this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ), this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ), this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_06_feedBack_buffer->getSize()) + this->allpass_tilde_06_feedBack_reader - ((size > this->allpass_tilde_06_feedBack__maxdelay ? this->allpass_tilde_06_feedBack__maxdelay : (size < (this->allpass_tilde_06_feedBack_reader != this->allpass_tilde_06_feedBack_writer) ? this->allpass_tilde_06_feedBack_reader != this->allpass_tilde_06_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ), this->allpass_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_06_feedBack_buffer->getSize()) + this->allpass_tilde_06_feedBack_reader - ((size > this->allpass_tilde_06_feedBack__maxdelay ? this->allpass_tilde_06_feedBack__maxdelay : (size < (this->allpass_tilde_06_feedBack_reader != this->allpass_tilde_06_feedBack_writer) ? this->allpass_tilde_06_feedBack_reader != this->allpass_tilde_06_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_06_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_06_feedBack_wrap))
        );
    }
    
    void allpass_tilde_06_feedBack_write(number v) {
        this->allpass_tilde_06_feedBack_writer = this->allpass_tilde_06_feedBack_reader;
        this->allpass_tilde_06_feedBack_buffer[(Index)this->allpass_tilde_06_feedBack_writer] = v;
    }
    
    number allpass_tilde_06_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_06_feedBack__maxdelay : size);
        number val = this->allpass_tilde_06_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_06_feedBack_write(v);
        this->allpass_tilde_06_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_06_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_06_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_06_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_06_feedBack_init() {
        auto result = this->allpass_tilde_06_feedBack_calcSizeInSamples();
        this->allpass_tilde_06_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_06_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_06_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_06_feedBack_clear() {
        this->allpass_tilde_06_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_06_feedBack_reset() {
        auto result = this->allpass_tilde_06_feedBack_calcSizeInSamples();
        this->allpass_tilde_06_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_06_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_06_feedBack_buffer);
        this->allpass_tilde_06_feedBack_wrap = this->allpass_tilde_06_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_06_feedBack_clear();
    
        if (this->allpass_tilde_06_feedBack_reader >= this->allpass_tilde_06_feedBack__maxdelay || this->allpass_tilde_06_feedBack_writer >= this->allpass_tilde_06_feedBack__maxdelay) {
            this->allpass_tilde_06_feedBack_reader = 0;
            this->allpass_tilde_06_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_06_feedBack_dspsetup() {
        this->allpass_tilde_06_feedBack_reset();
    }
    
    number allpass_tilde_06_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_06_feedBack_size() {
        return this->allpass_tilde_06_feedBack__maxdelay;
    }
    
    void allpass_tilde_06_init() {
        this->allpass_tilde_06_feedForward_init();
        this->allpass_tilde_06_feedBack_init();
    }
    
    void allpass_tilde_06_clear() {
        this->allpass_tilde_06_feedForward_clear();
        this->allpass_tilde_06_feedBack_clear();
    }
    
    void allpass_tilde_06_reset() {
        this->allpass_tilde_06_init();
        this->allpass_tilde_06_clear();
    }
    
    void allpass_tilde_06_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_06_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_06_reset();
        this->allpass_tilde_06_setupDone = true;
        this->allpass_tilde_06_feedForward_dspsetup();
        this->allpass_tilde_06_feedBack_dspsetup();
    }
    
    void allpass_tilde_07_feedForward_step() {
        this->allpass_tilde_07_feedForward_reader++;
    
        if (this->allpass_tilde_07_feedForward_reader >= (int)(this->allpass_tilde_07_feedForward_buffer->getSize()))
            this->allpass_tilde_07_feedForward_reader = 0;
    }
    
    number allpass_tilde_07_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_07_feedForward_buffer->getSize()) + this->allpass_tilde_07_feedForward_reader - ((size > this->allpass_tilde_07_feedForward__maxdelay ? this->allpass_tilde_07_feedForward__maxdelay : (size < (this->allpass_tilde_07_feedForward_reader != this->allpass_tilde_07_feedForward_writer) ? this->allpass_tilde_07_feedForward_reader != this->allpass_tilde_07_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ), this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_07_feedForward_buffer->getSize()) + this->allpass_tilde_07_feedForward_reader - ((size > this->allpass_tilde_07_feedForward__maxdelay ? this->allpass_tilde_07_feedForward__maxdelay : (size < (1 + this->allpass_tilde_07_feedForward_reader != this->allpass_tilde_07_feedForward_writer) ? 1 + this->allpass_tilde_07_feedForward_reader != this->allpass_tilde_07_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ), this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ), this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ), this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_07_feedForward_buffer->getSize()) + this->allpass_tilde_07_feedForward_reader - ((size > this->allpass_tilde_07_feedForward__maxdelay ? this->allpass_tilde_07_feedForward__maxdelay : (size < (1 + this->allpass_tilde_07_feedForward_reader != this->allpass_tilde_07_feedForward_writer) ? 1 + this->allpass_tilde_07_feedForward_reader != this->allpass_tilde_07_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ), this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ), this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ), this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_07_feedForward_buffer->getSize()) + this->allpass_tilde_07_feedForward_reader - ((size > this->allpass_tilde_07_feedForward__maxdelay ? this->allpass_tilde_07_feedForward__maxdelay : (size < (this->allpass_tilde_07_feedForward_reader != this->allpass_tilde_07_feedForward_writer) ? this->allpass_tilde_07_feedForward_reader != this->allpass_tilde_07_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ), this->allpass_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_07_feedForward_buffer->getSize()) + this->allpass_tilde_07_feedForward_reader - ((size > this->allpass_tilde_07_feedForward__maxdelay ? this->allpass_tilde_07_feedForward__maxdelay : (size < (this->allpass_tilde_07_feedForward_reader != this->allpass_tilde_07_feedForward_writer) ? this->allpass_tilde_07_feedForward_reader != this->allpass_tilde_07_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_07_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_07_feedForward_wrap))
        );
    }
    
    void allpass_tilde_07_feedForward_write(number v) {
        this->allpass_tilde_07_feedForward_writer = this->allpass_tilde_07_feedForward_reader;
        this->allpass_tilde_07_feedForward_buffer[(Index)this->allpass_tilde_07_feedForward_writer] = v;
    }
    
    number allpass_tilde_07_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_07_feedForward__maxdelay : size);
        number val = this->allpass_tilde_07_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_07_feedForward_write(v);
        this->allpass_tilde_07_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_07_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_07_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_07_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_07_feedForward_init() {
        auto result = this->allpass_tilde_07_feedForward_calcSizeInSamples();
        this->allpass_tilde_07_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_07_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_07_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_07_feedForward_clear() {
        this->allpass_tilde_07_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_07_feedForward_reset() {
        auto result = this->allpass_tilde_07_feedForward_calcSizeInSamples();
        this->allpass_tilde_07_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_07_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_07_feedForward_buffer);
        this->allpass_tilde_07_feedForward_wrap = this->allpass_tilde_07_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_07_feedForward_clear();
    
        if (this->allpass_tilde_07_feedForward_reader >= this->allpass_tilde_07_feedForward__maxdelay || this->allpass_tilde_07_feedForward_writer >= this->allpass_tilde_07_feedForward__maxdelay) {
            this->allpass_tilde_07_feedForward_reader = 0;
            this->allpass_tilde_07_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_07_feedForward_dspsetup() {
        this->allpass_tilde_07_feedForward_reset();
    }
    
    number allpass_tilde_07_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_07_feedForward_size() {
        return this->allpass_tilde_07_feedForward__maxdelay;
    }
    
    void allpass_tilde_07_feedBack_step() {
        this->allpass_tilde_07_feedBack_reader++;
    
        if (this->allpass_tilde_07_feedBack_reader >= (int)(this->allpass_tilde_07_feedBack_buffer->getSize()))
            this->allpass_tilde_07_feedBack_reader = 0;
    }
    
    number allpass_tilde_07_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_07_feedBack_buffer->getSize()) + this->allpass_tilde_07_feedBack_reader - ((size > this->allpass_tilde_07_feedBack__maxdelay ? this->allpass_tilde_07_feedBack__maxdelay : (size < (this->allpass_tilde_07_feedBack_reader != this->allpass_tilde_07_feedBack_writer) ? this->allpass_tilde_07_feedBack_reader != this->allpass_tilde_07_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ), this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_07_feedBack_buffer->getSize()) + this->allpass_tilde_07_feedBack_reader - ((size > this->allpass_tilde_07_feedBack__maxdelay ? this->allpass_tilde_07_feedBack__maxdelay : (size < (1 + this->allpass_tilde_07_feedBack_reader != this->allpass_tilde_07_feedBack_writer) ? 1 + this->allpass_tilde_07_feedBack_reader != this->allpass_tilde_07_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ), this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ), this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ), this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_07_feedBack_buffer->getSize()) + this->allpass_tilde_07_feedBack_reader - ((size > this->allpass_tilde_07_feedBack__maxdelay ? this->allpass_tilde_07_feedBack__maxdelay : (size < (1 + this->allpass_tilde_07_feedBack_reader != this->allpass_tilde_07_feedBack_writer) ? 1 + this->allpass_tilde_07_feedBack_reader != this->allpass_tilde_07_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ), this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ), this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ), this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_07_feedBack_buffer->getSize()) + this->allpass_tilde_07_feedBack_reader - ((size > this->allpass_tilde_07_feedBack__maxdelay ? this->allpass_tilde_07_feedBack__maxdelay : (size < (this->allpass_tilde_07_feedBack_reader != this->allpass_tilde_07_feedBack_writer) ? this->allpass_tilde_07_feedBack_reader != this->allpass_tilde_07_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ), this->allpass_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_07_feedBack_buffer->getSize()) + this->allpass_tilde_07_feedBack_reader - ((size > this->allpass_tilde_07_feedBack__maxdelay ? this->allpass_tilde_07_feedBack__maxdelay : (size < (this->allpass_tilde_07_feedBack_reader != this->allpass_tilde_07_feedBack_writer) ? this->allpass_tilde_07_feedBack_reader != this->allpass_tilde_07_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_07_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_07_feedBack_wrap))
        );
    }
    
    void allpass_tilde_07_feedBack_write(number v) {
        this->allpass_tilde_07_feedBack_writer = this->allpass_tilde_07_feedBack_reader;
        this->allpass_tilde_07_feedBack_buffer[(Index)this->allpass_tilde_07_feedBack_writer] = v;
    }
    
    number allpass_tilde_07_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_07_feedBack__maxdelay : size);
        number val = this->allpass_tilde_07_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_07_feedBack_write(v);
        this->allpass_tilde_07_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_07_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_07_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_07_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_07_feedBack_init() {
        auto result = this->allpass_tilde_07_feedBack_calcSizeInSamples();
        this->allpass_tilde_07_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_07_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_07_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_07_feedBack_clear() {
        this->allpass_tilde_07_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_07_feedBack_reset() {
        auto result = this->allpass_tilde_07_feedBack_calcSizeInSamples();
        this->allpass_tilde_07_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_07_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_07_feedBack_buffer);
        this->allpass_tilde_07_feedBack_wrap = this->allpass_tilde_07_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_07_feedBack_clear();
    
        if (this->allpass_tilde_07_feedBack_reader >= this->allpass_tilde_07_feedBack__maxdelay || this->allpass_tilde_07_feedBack_writer >= this->allpass_tilde_07_feedBack__maxdelay) {
            this->allpass_tilde_07_feedBack_reader = 0;
            this->allpass_tilde_07_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_07_feedBack_dspsetup() {
        this->allpass_tilde_07_feedBack_reset();
    }
    
    number allpass_tilde_07_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_07_feedBack_size() {
        return this->allpass_tilde_07_feedBack__maxdelay;
    }
    
    void allpass_tilde_07_init() {
        this->allpass_tilde_07_feedForward_init();
        this->allpass_tilde_07_feedBack_init();
    }
    
    void allpass_tilde_07_clear() {
        this->allpass_tilde_07_feedForward_clear();
        this->allpass_tilde_07_feedBack_clear();
    }
    
    void allpass_tilde_07_reset() {
        this->allpass_tilde_07_init();
        this->allpass_tilde_07_clear();
    }
    
    void allpass_tilde_07_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_07_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_07_reset();
        this->allpass_tilde_07_setupDone = true;
        this->allpass_tilde_07_feedForward_dspsetup();
        this->allpass_tilde_07_feedBack_dspsetup();
    }
    
    void allpass_tilde_08_feedForward_step() {
        this->allpass_tilde_08_feedForward_reader++;
    
        if (this->allpass_tilde_08_feedForward_reader >= (int)(this->allpass_tilde_08_feedForward_buffer->getSize()))
            this->allpass_tilde_08_feedForward_reader = 0;
    }
    
    number allpass_tilde_08_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_08_feedForward_buffer->getSize()) + this->allpass_tilde_08_feedForward_reader - ((size > this->allpass_tilde_08_feedForward__maxdelay ? this->allpass_tilde_08_feedForward__maxdelay : (size < (this->allpass_tilde_08_feedForward_reader != this->allpass_tilde_08_feedForward_writer) ? this->allpass_tilde_08_feedForward_reader != this->allpass_tilde_08_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ), this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_08_feedForward_buffer->getSize()) + this->allpass_tilde_08_feedForward_reader - ((size > this->allpass_tilde_08_feedForward__maxdelay ? this->allpass_tilde_08_feedForward__maxdelay : (size < (1 + this->allpass_tilde_08_feedForward_reader != this->allpass_tilde_08_feedForward_writer) ? 1 + this->allpass_tilde_08_feedForward_reader != this->allpass_tilde_08_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ), this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ), this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ), this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_08_feedForward_buffer->getSize()) + this->allpass_tilde_08_feedForward_reader - ((size > this->allpass_tilde_08_feedForward__maxdelay ? this->allpass_tilde_08_feedForward__maxdelay : (size < (1 + this->allpass_tilde_08_feedForward_reader != this->allpass_tilde_08_feedForward_writer) ? 1 + this->allpass_tilde_08_feedForward_reader != this->allpass_tilde_08_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ), this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ), this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ), this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_08_feedForward_buffer->getSize()) + this->allpass_tilde_08_feedForward_reader - ((size > this->allpass_tilde_08_feedForward__maxdelay ? this->allpass_tilde_08_feedForward__maxdelay : (size < (this->allpass_tilde_08_feedForward_reader != this->allpass_tilde_08_feedForward_writer) ? this->allpass_tilde_08_feedForward_reader != this->allpass_tilde_08_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ), this->allpass_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_08_feedForward_buffer->getSize()) + this->allpass_tilde_08_feedForward_reader - ((size > this->allpass_tilde_08_feedForward__maxdelay ? this->allpass_tilde_08_feedForward__maxdelay : (size < (this->allpass_tilde_08_feedForward_reader != this->allpass_tilde_08_feedForward_writer) ? this->allpass_tilde_08_feedForward_reader != this->allpass_tilde_08_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_08_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_08_feedForward_wrap))
        );
    }
    
    void allpass_tilde_08_feedForward_write(number v) {
        this->allpass_tilde_08_feedForward_writer = this->allpass_tilde_08_feedForward_reader;
        this->allpass_tilde_08_feedForward_buffer[(Index)this->allpass_tilde_08_feedForward_writer] = v;
    }
    
    number allpass_tilde_08_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_08_feedForward__maxdelay : size);
        number val = this->allpass_tilde_08_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_08_feedForward_write(v);
        this->allpass_tilde_08_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_08_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_08_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_08_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_08_feedForward_init() {
        auto result = this->allpass_tilde_08_feedForward_calcSizeInSamples();
        this->allpass_tilde_08_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_08_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_08_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_08_feedForward_clear() {
        this->allpass_tilde_08_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_08_feedForward_reset() {
        auto result = this->allpass_tilde_08_feedForward_calcSizeInSamples();
        this->allpass_tilde_08_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_08_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_08_feedForward_buffer);
        this->allpass_tilde_08_feedForward_wrap = this->allpass_tilde_08_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_08_feedForward_clear();
    
        if (this->allpass_tilde_08_feedForward_reader >= this->allpass_tilde_08_feedForward__maxdelay || this->allpass_tilde_08_feedForward_writer >= this->allpass_tilde_08_feedForward__maxdelay) {
            this->allpass_tilde_08_feedForward_reader = 0;
            this->allpass_tilde_08_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_08_feedForward_dspsetup() {
        this->allpass_tilde_08_feedForward_reset();
    }
    
    number allpass_tilde_08_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_08_feedForward_size() {
        return this->allpass_tilde_08_feedForward__maxdelay;
    }
    
    void allpass_tilde_08_feedBack_step() {
        this->allpass_tilde_08_feedBack_reader++;
    
        if (this->allpass_tilde_08_feedBack_reader >= (int)(this->allpass_tilde_08_feedBack_buffer->getSize()))
            this->allpass_tilde_08_feedBack_reader = 0;
    }
    
    number allpass_tilde_08_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_08_feedBack_buffer->getSize()) + this->allpass_tilde_08_feedBack_reader - ((size > this->allpass_tilde_08_feedBack__maxdelay ? this->allpass_tilde_08_feedBack__maxdelay : (size < (this->allpass_tilde_08_feedBack_reader != this->allpass_tilde_08_feedBack_writer) ? this->allpass_tilde_08_feedBack_reader != this->allpass_tilde_08_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ), this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_08_feedBack_buffer->getSize()) + this->allpass_tilde_08_feedBack_reader - ((size > this->allpass_tilde_08_feedBack__maxdelay ? this->allpass_tilde_08_feedBack__maxdelay : (size < (1 + this->allpass_tilde_08_feedBack_reader != this->allpass_tilde_08_feedBack_writer) ? 1 + this->allpass_tilde_08_feedBack_reader != this->allpass_tilde_08_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ), this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ), this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ), this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_08_feedBack_buffer->getSize()) + this->allpass_tilde_08_feedBack_reader - ((size > this->allpass_tilde_08_feedBack__maxdelay ? this->allpass_tilde_08_feedBack__maxdelay : (size < (1 + this->allpass_tilde_08_feedBack_reader != this->allpass_tilde_08_feedBack_writer) ? 1 + this->allpass_tilde_08_feedBack_reader != this->allpass_tilde_08_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ), this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ), this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ), this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_08_feedBack_buffer->getSize()) + this->allpass_tilde_08_feedBack_reader - ((size > this->allpass_tilde_08_feedBack__maxdelay ? this->allpass_tilde_08_feedBack__maxdelay : (size < (this->allpass_tilde_08_feedBack_reader != this->allpass_tilde_08_feedBack_writer) ? this->allpass_tilde_08_feedBack_reader != this->allpass_tilde_08_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ), this->allpass_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_08_feedBack_buffer->getSize()) + this->allpass_tilde_08_feedBack_reader - ((size > this->allpass_tilde_08_feedBack__maxdelay ? this->allpass_tilde_08_feedBack__maxdelay : (size < (this->allpass_tilde_08_feedBack_reader != this->allpass_tilde_08_feedBack_writer) ? this->allpass_tilde_08_feedBack_reader != this->allpass_tilde_08_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_08_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_08_feedBack_wrap))
        );
    }
    
    void allpass_tilde_08_feedBack_write(number v) {
        this->allpass_tilde_08_feedBack_writer = this->allpass_tilde_08_feedBack_reader;
        this->allpass_tilde_08_feedBack_buffer[(Index)this->allpass_tilde_08_feedBack_writer] = v;
    }
    
    number allpass_tilde_08_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_08_feedBack__maxdelay : size);
        number val = this->allpass_tilde_08_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_08_feedBack_write(v);
        this->allpass_tilde_08_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_08_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_08_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_08_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_08_feedBack_init() {
        auto result = this->allpass_tilde_08_feedBack_calcSizeInSamples();
        this->allpass_tilde_08_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_08_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_08_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_08_feedBack_clear() {
        this->allpass_tilde_08_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_08_feedBack_reset() {
        auto result = this->allpass_tilde_08_feedBack_calcSizeInSamples();
        this->allpass_tilde_08_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_08_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_08_feedBack_buffer);
        this->allpass_tilde_08_feedBack_wrap = this->allpass_tilde_08_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_08_feedBack_clear();
    
        if (this->allpass_tilde_08_feedBack_reader >= this->allpass_tilde_08_feedBack__maxdelay || this->allpass_tilde_08_feedBack_writer >= this->allpass_tilde_08_feedBack__maxdelay) {
            this->allpass_tilde_08_feedBack_reader = 0;
            this->allpass_tilde_08_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_08_feedBack_dspsetup() {
        this->allpass_tilde_08_feedBack_reset();
    }
    
    number allpass_tilde_08_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_08_feedBack_size() {
        return this->allpass_tilde_08_feedBack__maxdelay;
    }
    
    void allpass_tilde_08_init() {
        this->allpass_tilde_08_feedForward_init();
        this->allpass_tilde_08_feedBack_init();
    }
    
    void allpass_tilde_08_clear() {
        this->allpass_tilde_08_feedForward_clear();
        this->allpass_tilde_08_feedBack_clear();
    }
    
    void allpass_tilde_08_reset() {
        this->allpass_tilde_08_init();
        this->allpass_tilde_08_clear();
    }
    
    void allpass_tilde_08_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_08_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_08_reset();
        this->allpass_tilde_08_setupDone = true;
        this->allpass_tilde_08_feedForward_dspsetup();
        this->allpass_tilde_08_feedBack_dspsetup();
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
        allpass_tilde_01_x = 0;
        allpass_tilde_01_delayTime = 9.3;
        allpass_tilde_01_gain = 0.625;
        allpass_tilde_02_x = 0;
        allpass_tilde_02_delayTime = 12.73;
        allpass_tilde_02_gain = 0.625;
        allpass_tilde_03_x = 0;
        allpass_tilde_03_delayTime = 3.58;
        allpass_tilde_03_gain = 0.75;
        allpass_tilde_04_x = 0;
        allpass_tilde_04_delayTime = 4.76;
        allpass_tilde_04_gain = 0.75;
        allpass_tilde_05_x = 0;
        allpass_tilde_05_delayTime = 9.3;
        allpass_tilde_05_gain = 0.625;
        allpass_tilde_06_x = 0;
        allpass_tilde_06_delayTime = 12.73;
        allpass_tilde_06_gain = 0.625;
        allpass_tilde_07_x = 0;
        allpass_tilde_07_delayTime = 3.58;
        allpass_tilde_07_gain = 0.75;
        allpass_tilde_08_x = 0;
        allpass_tilde_08_delayTime = 4.76;
        allpass_tilde_08_gain = 0.75;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        allpass_tilde_01_feedForward__maxdelay = 0;
        allpass_tilde_01_feedForward_sizemode = 0;
        allpass_tilde_01_feedForward_wrap = 0;
        allpass_tilde_01_feedForward_reader = 0;
        allpass_tilde_01_feedForward_writer = 0;
        allpass_tilde_01_feedBack__maxdelay = 0;
        allpass_tilde_01_feedBack_sizemode = 0;
        allpass_tilde_01_feedBack_wrap = 0;
        allpass_tilde_01_feedBack_reader = 0;
        allpass_tilde_01_feedBack_writer = 0;
        allpass_tilde_01_setupDone = false;
        allpass_tilde_02_feedForward__maxdelay = 0;
        allpass_tilde_02_feedForward_sizemode = 0;
        allpass_tilde_02_feedForward_wrap = 0;
        allpass_tilde_02_feedForward_reader = 0;
        allpass_tilde_02_feedForward_writer = 0;
        allpass_tilde_02_feedBack__maxdelay = 0;
        allpass_tilde_02_feedBack_sizemode = 0;
        allpass_tilde_02_feedBack_wrap = 0;
        allpass_tilde_02_feedBack_reader = 0;
        allpass_tilde_02_feedBack_writer = 0;
        allpass_tilde_02_setupDone = false;
        allpass_tilde_03_feedForward__maxdelay = 0;
        allpass_tilde_03_feedForward_sizemode = 0;
        allpass_tilde_03_feedForward_wrap = 0;
        allpass_tilde_03_feedForward_reader = 0;
        allpass_tilde_03_feedForward_writer = 0;
        allpass_tilde_03_feedBack__maxdelay = 0;
        allpass_tilde_03_feedBack_sizemode = 0;
        allpass_tilde_03_feedBack_wrap = 0;
        allpass_tilde_03_feedBack_reader = 0;
        allpass_tilde_03_feedBack_writer = 0;
        allpass_tilde_03_setupDone = false;
        allpass_tilde_04_feedForward__maxdelay = 0;
        allpass_tilde_04_feedForward_sizemode = 0;
        allpass_tilde_04_feedForward_wrap = 0;
        allpass_tilde_04_feedForward_reader = 0;
        allpass_tilde_04_feedForward_writer = 0;
        allpass_tilde_04_feedBack__maxdelay = 0;
        allpass_tilde_04_feedBack_sizemode = 0;
        allpass_tilde_04_feedBack_wrap = 0;
        allpass_tilde_04_feedBack_reader = 0;
        allpass_tilde_04_feedBack_writer = 0;
        allpass_tilde_04_setupDone = false;
        allpass_tilde_05_feedForward__maxdelay = 0;
        allpass_tilde_05_feedForward_sizemode = 0;
        allpass_tilde_05_feedForward_wrap = 0;
        allpass_tilde_05_feedForward_reader = 0;
        allpass_tilde_05_feedForward_writer = 0;
        allpass_tilde_05_feedBack__maxdelay = 0;
        allpass_tilde_05_feedBack_sizemode = 0;
        allpass_tilde_05_feedBack_wrap = 0;
        allpass_tilde_05_feedBack_reader = 0;
        allpass_tilde_05_feedBack_writer = 0;
        allpass_tilde_05_setupDone = false;
        allpass_tilde_06_feedForward__maxdelay = 0;
        allpass_tilde_06_feedForward_sizemode = 0;
        allpass_tilde_06_feedForward_wrap = 0;
        allpass_tilde_06_feedForward_reader = 0;
        allpass_tilde_06_feedForward_writer = 0;
        allpass_tilde_06_feedBack__maxdelay = 0;
        allpass_tilde_06_feedBack_sizemode = 0;
        allpass_tilde_06_feedBack_wrap = 0;
        allpass_tilde_06_feedBack_reader = 0;
        allpass_tilde_06_feedBack_writer = 0;
        allpass_tilde_06_setupDone = false;
        allpass_tilde_07_feedForward__maxdelay = 0;
        allpass_tilde_07_feedForward_sizemode = 0;
        allpass_tilde_07_feedForward_wrap = 0;
        allpass_tilde_07_feedForward_reader = 0;
        allpass_tilde_07_feedForward_writer = 0;
        allpass_tilde_07_feedBack__maxdelay = 0;
        allpass_tilde_07_feedBack_sizemode = 0;
        allpass_tilde_07_feedBack_wrap = 0;
        allpass_tilde_07_feedBack_reader = 0;
        allpass_tilde_07_feedBack_writer = 0;
        allpass_tilde_07_setupDone = false;
        allpass_tilde_08_feedForward__maxdelay = 0;
        allpass_tilde_08_feedForward_sizemode = 0;
        allpass_tilde_08_feedForward_wrap = 0;
        allpass_tilde_08_feedForward_reader = 0;
        allpass_tilde_08_feedForward_writer = 0;
        allpass_tilde_08_feedBack__maxdelay = 0;
        allpass_tilde_08_feedBack_sizemode = 0;
        allpass_tilde_08_feedBack_wrap = 0;
        allpass_tilde_08_feedBack_reader = 0;
        allpass_tilde_08_feedBack_writer = 0;
        allpass_tilde_08_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number allpass_tilde_01_x;
        number allpass_tilde_01_delayTime;
        number allpass_tilde_01_gain;
        number allpass_tilde_02_x;
        number allpass_tilde_02_delayTime;
        number allpass_tilde_02_gain;
        number allpass_tilde_03_x;
        number allpass_tilde_03_delayTime;
        number allpass_tilde_03_gain;
        number allpass_tilde_04_x;
        number allpass_tilde_04_delayTime;
        number allpass_tilde_04_gain;
        number allpass_tilde_05_x;
        number allpass_tilde_05_delayTime;
        number allpass_tilde_05_gain;
        number allpass_tilde_06_x;
        number allpass_tilde_06_delayTime;
        number allpass_tilde_06_gain;
        number allpass_tilde_07_x;
        number allpass_tilde_07_delayTime;
        number allpass_tilde_07_gain;
        number allpass_tilde_08_x;
        number allpass_tilde_08_delayTime;
        number allpass_tilde_08_gain;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[2];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float64BufferRef allpass_tilde_01_feedForward_buffer;
        Index allpass_tilde_01_feedForward__maxdelay;
        Int allpass_tilde_01_feedForward_sizemode;
        Index allpass_tilde_01_feedForward_wrap;
        Int allpass_tilde_01_feedForward_reader;
        Int allpass_tilde_01_feedForward_writer;
        Float64BufferRef allpass_tilde_01_feedBack_buffer;
        Index allpass_tilde_01_feedBack__maxdelay;
        Int allpass_tilde_01_feedBack_sizemode;
        Index allpass_tilde_01_feedBack_wrap;
        Int allpass_tilde_01_feedBack_reader;
        Int allpass_tilde_01_feedBack_writer;
        bool allpass_tilde_01_setupDone;
        Float64BufferRef allpass_tilde_02_feedForward_buffer;
        Index allpass_tilde_02_feedForward__maxdelay;
        Int allpass_tilde_02_feedForward_sizemode;
        Index allpass_tilde_02_feedForward_wrap;
        Int allpass_tilde_02_feedForward_reader;
        Int allpass_tilde_02_feedForward_writer;
        Float64BufferRef allpass_tilde_02_feedBack_buffer;
        Index allpass_tilde_02_feedBack__maxdelay;
        Int allpass_tilde_02_feedBack_sizemode;
        Index allpass_tilde_02_feedBack_wrap;
        Int allpass_tilde_02_feedBack_reader;
        Int allpass_tilde_02_feedBack_writer;
        bool allpass_tilde_02_setupDone;
        Float64BufferRef allpass_tilde_03_feedForward_buffer;
        Index allpass_tilde_03_feedForward__maxdelay;
        Int allpass_tilde_03_feedForward_sizemode;
        Index allpass_tilde_03_feedForward_wrap;
        Int allpass_tilde_03_feedForward_reader;
        Int allpass_tilde_03_feedForward_writer;
        Float64BufferRef allpass_tilde_03_feedBack_buffer;
        Index allpass_tilde_03_feedBack__maxdelay;
        Int allpass_tilde_03_feedBack_sizemode;
        Index allpass_tilde_03_feedBack_wrap;
        Int allpass_tilde_03_feedBack_reader;
        Int allpass_tilde_03_feedBack_writer;
        bool allpass_tilde_03_setupDone;
        Float64BufferRef allpass_tilde_04_feedForward_buffer;
        Index allpass_tilde_04_feedForward__maxdelay;
        Int allpass_tilde_04_feedForward_sizemode;
        Index allpass_tilde_04_feedForward_wrap;
        Int allpass_tilde_04_feedForward_reader;
        Int allpass_tilde_04_feedForward_writer;
        Float64BufferRef allpass_tilde_04_feedBack_buffer;
        Index allpass_tilde_04_feedBack__maxdelay;
        Int allpass_tilde_04_feedBack_sizemode;
        Index allpass_tilde_04_feedBack_wrap;
        Int allpass_tilde_04_feedBack_reader;
        Int allpass_tilde_04_feedBack_writer;
        bool allpass_tilde_04_setupDone;
        Float64BufferRef allpass_tilde_05_feedForward_buffer;
        Index allpass_tilde_05_feedForward__maxdelay;
        Int allpass_tilde_05_feedForward_sizemode;
        Index allpass_tilde_05_feedForward_wrap;
        Int allpass_tilde_05_feedForward_reader;
        Int allpass_tilde_05_feedForward_writer;
        Float64BufferRef allpass_tilde_05_feedBack_buffer;
        Index allpass_tilde_05_feedBack__maxdelay;
        Int allpass_tilde_05_feedBack_sizemode;
        Index allpass_tilde_05_feedBack_wrap;
        Int allpass_tilde_05_feedBack_reader;
        Int allpass_tilde_05_feedBack_writer;
        bool allpass_tilde_05_setupDone;
        Float64BufferRef allpass_tilde_06_feedForward_buffer;
        Index allpass_tilde_06_feedForward__maxdelay;
        Int allpass_tilde_06_feedForward_sizemode;
        Index allpass_tilde_06_feedForward_wrap;
        Int allpass_tilde_06_feedForward_reader;
        Int allpass_tilde_06_feedForward_writer;
        Float64BufferRef allpass_tilde_06_feedBack_buffer;
        Index allpass_tilde_06_feedBack__maxdelay;
        Int allpass_tilde_06_feedBack_sizemode;
        Index allpass_tilde_06_feedBack_wrap;
        Int allpass_tilde_06_feedBack_reader;
        Int allpass_tilde_06_feedBack_writer;
        bool allpass_tilde_06_setupDone;
        Float64BufferRef allpass_tilde_07_feedForward_buffer;
        Index allpass_tilde_07_feedForward__maxdelay;
        Int allpass_tilde_07_feedForward_sizemode;
        Index allpass_tilde_07_feedForward_wrap;
        Int allpass_tilde_07_feedForward_reader;
        Int allpass_tilde_07_feedForward_writer;
        Float64BufferRef allpass_tilde_07_feedBack_buffer;
        Index allpass_tilde_07_feedBack__maxdelay;
        Int allpass_tilde_07_feedBack_sizemode;
        Index allpass_tilde_07_feedBack_wrap;
        Int allpass_tilde_07_feedBack_reader;
        Int allpass_tilde_07_feedBack_writer;
        bool allpass_tilde_07_setupDone;
        Float64BufferRef allpass_tilde_08_feedForward_buffer;
        Index allpass_tilde_08_feedForward__maxdelay;
        Int allpass_tilde_08_feedForward_sizemode;
        Index allpass_tilde_08_feedForward_wrap;
        Int allpass_tilde_08_feedForward_reader;
        Int allpass_tilde_08_feedForward_writer;
        Float64BufferRef allpass_tilde_08_feedBack_buffer;
        Index allpass_tilde_08_feedBack__maxdelay;
        Int allpass_tilde_08_feedBack_sizemode;
        Index allpass_tilde_08_feedBack_wrap;
        Int allpass_tilde_08_feedBack_reader;
        Int allpass_tilde_08_feedBack_writer;
        bool allpass_tilde_08_setupDone;
        number stackprotect_count;
        DataRef allpass_tilde_01_feedForward_bufferobj;
        DataRef allpass_tilde_01_feedBack_bufferobj;
        DataRef allpass_tilde_02_feedForward_bufferobj;
        DataRef allpass_tilde_02_feedBack_bufferobj;
        DataRef allpass_tilde_03_feedForward_bufferobj;
        DataRef allpass_tilde_03_feedBack_bufferobj;
        DataRef allpass_tilde_04_feedForward_bufferobj;
        DataRef allpass_tilde_04_feedBack_bufferobj;
        DataRef allpass_tilde_05_feedForward_bufferobj;
        DataRef allpass_tilde_05_feedBack_bufferobj;
        DataRef allpass_tilde_06_feedForward_bufferobj;
        DataRef allpass_tilde_06_feedBack_bufferobj;
        DataRef allpass_tilde_07_feedForward_bufferobj;
        DataRef allpass_tilde_07_feedBack_bufferobj;
        DataRef allpass_tilde_08_feedForward_bufferobj;
        DataRef allpass_tilde_08_feedBack_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_20 : public PatcherInterfaceImpl {
    
    friend class plate;
    
    public:
    
    RNBOSubpatcher_20()
    {
    }
    
    ~RNBOSubpatcher_20()
    {
    }
    
    virtual plate* getPatcher() const {
        return static_cast<plate *>(_parentPatcher);
    }
    
    plate* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
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
        const SampleValue * target_Size = (numInputs >= 8 && inputs[7] ? inputs[7] : this->zeroBuffer);
        const SampleValue * target_Diff = (numInputs >= 9 && inputs[8] ? inputs[8] : this->zeroBuffer);
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        const SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        const SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        const SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        const SampleValue * in6 = (numInputs >= 6 && inputs[5] ? inputs[5] : this->zeroBuffer);
        const SampleValue * in7 = (numInputs >= 7 && inputs[6] ? inputs[6] : this->zeroBuffer);
        this->stackprotect_perform(n);
        this->signalreceive_01_perform(target_Size, this->signals[0], n);
        this->signalreceive_02_perform(target_Diff, this->signals[1], n);
        this->dspexpr_05_perform(this->signals[0], this->signals[1], this->signals[2], n);
        this->dspexpr_10_perform(this->signals[2], this->dspexpr_10_in2, this->signals[1], n);
    
        this->comb_tilde_07_perform(
            in7,
            this->signals[1],
            this->comb_tilde_07_gain,
            this->comb_tilde_07_ffGain,
            this->comb_tilde_07_fbGain,
            this->signals[0],
            n
        );
    
        this->dspexpr_01_perform(this->signals[2], this->dspexpr_01_in2, this->signals[1], n);
    
        this->comb_tilde_01_perform(
            in1,
            this->signals[1],
            this->comb_tilde_01_gain,
            this->comb_tilde_01_ffGain,
            this->comb_tilde_01_fbGain,
            this->signals[3],
            n
        );
    
        this->dspexpr_02_perform(this->signals[2], this->dspexpr_02_in2, this->signals[1], n);
    
        this->comb_tilde_02_perform(
            in2,
            this->signals[1],
            this->comb_tilde_02_gain,
            this->comb_tilde_02_ffGain,
            this->comb_tilde_02_fbGain,
            this->signals[4],
            n
        );
    
        this->dspexpr_03_perform(this->signals[2], this->dspexpr_03_in2, this->signals[1], n);
    
        this->comb_tilde_03_perform(
            in3,
            this->signals[1],
            this->comb_tilde_03_gain,
            this->comb_tilde_03_ffGain,
            this->comb_tilde_03_fbGain,
            this->signals[5],
            n
        );
    
        this->dspexpr_06_perform(this->signals[2], this->dspexpr_06_in2, this->signals[1], n);
    
        this->comb_tilde_04_perform(
            in4,
            this->signals[1],
            this->comb_tilde_04_gain,
            this->comb_tilde_04_ffGain,
            this->comb_tilde_04_fbGain,
            this->signals[6],
            n
        );
    
        this->dspexpr_08_perform(this->signals[2], this->dspexpr_08_in2, this->signals[1], n);
    
        this->comb_tilde_05_perform(
            in5,
            this->signals[1],
            this->comb_tilde_05_gain,
            this->comb_tilde_05_ffGain,
            this->comb_tilde_05_fbGain,
            this->signals[7],
            n
        );
    
        this->dspexpr_09_perform(this->signals[2], this->dspexpr_09_in2, this->signals[1], n);
    
        this->comb_tilde_06_perform(
            in6,
            this->signals[1],
            this->comb_tilde_06_gain,
            this->comb_tilde_06_ffGain,
            this->comb_tilde_06_fbGain,
            this->signals[2],
            n
        );
    
        this->signaladder_01_perform(
            this->signals[0],
            this->signals[2],
            this->signals[7],
            this->signals[5],
            this->signals[5],
            n
        );
    
        this->dspexpr_07_perform(this->signals[5], this->dspexpr_07_in2, this->signals[1], n);
    
        this->signaladder_02_perform(
            this->signals[6],
            this->signals[4],
            this->signals[3],
            this->signals[1],
            this->signals[1],
            n
        );
    
        this->dspexpr_04_perform(this->signals[1], this->dspexpr_04_in2, out1, n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 8; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
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
    
        this->comb_tilde_07_dspsetup(forceDSPSetup);
        this->comb_tilde_01_dspsetup(forceDSPSetup);
        this->comb_tilde_02_dspsetup(forceDSPSetup);
        this->comb_tilde_03_dspsetup(forceDSPSetup);
        this->comb_tilde_04_dspsetup(forceDSPSetup);
        this->comb_tilde_05_dspsetup(forceDSPSetup);
        this->comb_tilde_06_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        default:
            {
            return 0;
            }
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 2;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
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
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 0:
            {
            return addressOf(this->comb_tilde_01_feedForward_bufferobj);
            break;
            }
        case 1:
            {
            return addressOf(this->comb_tilde_01_feedBack_bufferobj);
            break;
            }
        case 2:
            {
            return addressOf(this->comb_tilde_02_feedForward_bufferobj);
            break;
            }
        case 3:
            {
            return addressOf(this->comb_tilde_02_feedBack_bufferobj);
            break;
            }
        case 4:
            {
            return addressOf(this->comb_tilde_03_feedForward_bufferobj);
            break;
            }
        case 5:
            {
            return addressOf(this->comb_tilde_03_feedBack_bufferobj);
            break;
            }
        case 6:
            {
            return addressOf(this->comb_tilde_04_feedForward_bufferobj);
            break;
            }
        case 7:
            {
            return addressOf(this->comb_tilde_04_feedBack_bufferobj);
            break;
            }
        case 8:
            {
            return addressOf(this->comb_tilde_05_feedForward_bufferobj);
            break;
            }
        case 9:
            {
            return addressOf(this->comb_tilde_05_feedBack_bufferobj);
            break;
            }
        case 10:
            {
            return addressOf(this->comb_tilde_06_feedForward_bufferobj);
            break;
            }
        case 11:
            {
            return addressOf(this->comb_tilde_06_feedBack_bufferobj);
            break;
            }
        case 12:
            {
            return addressOf(this->comb_tilde_07_feedForward_bufferobj);
            break;
            }
        case 13:
            {
            return addressOf(this->comb_tilde_07_feedBack_bufferobj);
            break;
            }
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 14;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void zeroDataRef(DataRef& ref) {
        ref->setZero();
    }
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 0) {
            this->comb_tilde_01_feedForward_buffer = new Float64Buffer(this->comb_tilde_01_feedForward_bufferobj);
        }
    
        if (index == 1) {
            this->comb_tilde_01_feedBack_buffer = new Float64Buffer(this->comb_tilde_01_feedBack_bufferobj);
        }
    
        if (index == 2) {
            this->comb_tilde_02_feedForward_buffer = new Float64Buffer(this->comb_tilde_02_feedForward_bufferobj);
        }
    
        if (index == 3) {
            this->comb_tilde_02_feedBack_buffer = new Float64Buffer(this->comb_tilde_02_feedBack_bufferobj);
        }
    
        if (index == 4) {
            this->comb_tilde_03_feedForward_buffer = new Float64Buffer(this->comb_tilde_03_feedForward_bufferobj);
        }
    
        if (index == 5) {
            this->comb_tilde_03_feedBack_buffer = new Float64Buffer(this->comb_tilde_03_feedBack_bufferobj);
        }
    
        if (index == 6) {
            this->comb_tilde_04_feedForward_buffer = new Float64Buffer(this->comb_tilde_04_feedForward_bufferobj);
        }
    
        if (index == 7) {
            this->comb_tilde_04_feedBack_buffer = new Float64Buffer(this->comb_tilde_04_feedBack_bufferobj);
        }
    
        if (index == 8) {
            this->comb_tilde_05_feedForward_buffer = new Float64Buffer(this->comb_tilde_05_feedForward_bufferobj);
        }
    
        if (index == 9) {
            this->comb_tilde_05_feedBack_buffer = new Float64Buffer(this->comb_tilde_05_feedBack_bufferobj);
        }
    
        if (index == 10) {
            this->comb_tilde_06_feedForward_buffer = new Float64Buffer(this->comb_tilde_06_feedForward_bufferobj);
        }
    
        if (index == 11) {
            this->comb_tilde_06_feedBack_buffer = new Float64Buffer(this->comb_tilde_06_feedBack_bufferobj);
        }
    
        if (index == 12) {
            this->comb_tilde_07_feedForward_buffer = new Float64Buffer(this->comb_tilde_07_feedForward_bufferobj);
        }
    
        if (index == 13) {
            this->comb_tilde_07_feedBack_buffer = new Float64Buffer(this->comb_tilde_07_feedBack_bufferobj);
        }
    }
    
    void initialize() {
        this->comb_tilde_01_feedForward_bufferobj = initDataRef("comb_tilde_01_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_01_feedBack_bufferobj = initDataRef("comb_tilde_01_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_02_feedForward_bufferobj = initDataRef("comb_tilde_02_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_02_feedBack_bufferobj = initDataRef("comb_tilde_02_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_03_feedForward_bufferobj = initDataRef("comb_tilde_03_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_03_feedBack_bufferobj = initDataRef("comb_tilde_03_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_04_feedForward_bufferobj = initDataRef("comb_tilde_04_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_04_feedBack_bufferobj = initDataRef("comb_tilde_04_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_05_feedForward_bufferobj = initDataRef("comb_tilde_05_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_05_feedBack_bufferobj = initDataRef("comb_tilde_05_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_06_feedForward_bufferobj = initDataRef("comb_tilde_06_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_06_feedBack_bufferobj = initDataRef("comb_tilde_06_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_07_feedForward_bufferobj = initDataRef("comb_tilde_07_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_07_feedBack_bufferobj = initDataRef("comb_tilde_07_feedBack_bufferobj", true, nullptr, "buffer~");
        this->assign_defaults();
        this->setState();
        this->comb_tilde_01_feedForward_bufferobj->setIndex(0);
        this->comb_tilde_01_feedForward_buffer = new Float64Buffer(this->comb_tilde_01_feedForward_bufferobj);
        this->comb_tilde_01_feedBack_bufferobj->setIndex(1);
        this->comb_tilde_01_feedBack_buffer = new Float64Buffer(this->comb_tilde_01_feedBack_bufferobj);
        this->comb_tilde_02_feedForward_bufferobj->setIndex(2);
        this->comb_tilde_02_feedForward_buffer = new Float64Buffer(this->comb_tilde_02_feedForward_bufferobj);
        this->comb_tilde_02_feedBack_bufferobj->setIndex(3);
        this->comb_tilde_02_feedBack_buffer = new Float64Buffer(this->comb_tilde_02_feedBack_bufferobj);
        this->comb_tilde_03_feedForward_bufferobj->setIndex(4);
        this->comb_tilde_03_feedForward_buffer = new Float64Buffer(this->comb_tilde_03_feedForward_bufferobj);
        this->comb_tilde_03_feedBack_bufferobj->setIndex(5);
        this->comb_tilde_03_feedBack_buffer = new Float64Buffer(this->comb_tilde_03_feedBack_bufferobj);
        this->comb_tilde_04_feedForward_bufferobj->setIndex(6);
        this->comb_tilde_04_feedForward_buffer = new Float64Buffer(this->comb_tilde_04_feedForward_bufferobj);
        this->comb_tilde_04_feedBack_bufferobj->setIndex(7);
        this->comb_tilde_04_feedBack_buffer = new Float64Buffer(this->comb_tilde_04_feedBack_bufferobj);
        this->comb_tilde_05_feedForward_bufferobj->setIndex(8);
        this->comb_tilde_05_feedForward_buffer = new Float64Buffer(this->comb_tilde_05_feedForward_bufferobj);
        this->comb_tilde_05_feedBack_bufferobj->setIndex(9);
        this->comb_tilde_05_feedBack_buffer = new Float64Buffer(this->comb_tilde_05_feedBack_bufferobj);
        this->comb_tilde_06_feedForward_bufferobj->setIndex(10);
        this->comb_tilde_06_feedForward_buffer = new Float64Buffer(this->comb_tilde_06_feedForward_bufferobj);
        this->comb_tilde_06_feedBack_bufferobj->setIndex(11);
        this->comb_tilde_06_feedBack_buffer = new Float64Buffer(this->comb_tilde_06_feedBack_bufferobj);
        this->comb_tilde_07_feedForward_bufferobj->setIndex(12);
        this->comb_tilde_07_feedForward_buffer = new Float64Buffer(this->comb_tilde_07_feedForward_bufferobj);
        this->comb_tilde_07_feedBack_bufferobj->setIndex(13);
        this->comb_tilde_07_feedBack_buffer = new Float64Buffer(this->comb_tilde_07_feedBack_bufferobj);
    }
    
    protected:
    
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
        return 7;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->comb_tilde_01_feedForward_init();
        this->comb_tilde_01_feedBack_init();
        this->comb_tilde_01_init();
        this->comb_tilde_02_feedForward_init();
        this->comb_tilde_02_feedBack_init();
        this->comb_tilde_02_init();
        this->comb_tilde_03_feedForward_init();
        this->comb_tilde_03_feedBack_init();
        this->comb_tilde_03_init();
        this->comb_tilde_04_feedForward_init();
        this->comb_tilde_04_feedBack_init();
        this->comb_tilde_04_init();
        this->comb_tilde_05_feedForward_init();
        this->comb_tilde_05_feedBack_init();
        this->comb_tilde_05_init();
        this->comb_tilde_06_feedForward_init();
        this->comb_tilde_06_feedBack_init();
        this->comb_tilde_06_init();
        this->comb_tilde_07_feedForward_init();
        this->comb_tilde_07_feedBack_init();
        this->comb_tilde_07_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->comb_tilde_01_feedForward_buffer = this->comb_tilde_01_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_01_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_01_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_01_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(0);
        }
    
        this->comb_tilde_01_feedBack_buffer = this->comb_tilde_01_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_01_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_01_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_01_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    
        this->comb_tilde_02_feedForward_buffer = this->comb_tilde_02_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_02_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_02_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_02_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(2);
        }
    
        this->comb_tilde_02_feedBack_buffer = this->comb_tilde_02_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_02_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_02_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_02_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(3);
        }
    
        this->comb_tilde_03_feedForward_buffer = this->comb_tilde_03_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_03_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_03_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_03_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(4);
        }
    
        this->comb_tilde_03_feedBack_buffer = this->comb_tilde_03_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_03_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_03_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_03_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(5);
        }
    
        this->comb_tilde_04_feedForward_buffer = this->comb_tilde_04_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_04_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_04_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_04_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(6);
        }
    
        this->comb_tilde_04_feedBack_buffer = this->comb_tilde_04_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_04_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_04_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_04_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(7);
        }
    
        this->comb_tilde_05_feedForward_buffer = this->comb_tilde_05_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_05_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_05_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_05_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(8);
        }
    
        this->comb_tilde_05_feedBack_buffer = this->comb_tilde_05_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_05_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_05_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_05_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(9);
        }
    
        this->comb_tilde_06_feedForward_buffer = this->comb_tilde_06_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_06_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_06_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_06_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(10);
        }
    
        this->comb_tilde_06_feedBack_buffer = this->comb_tilde_06_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_06_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_06_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_06_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(11);
        }
    
        this->comb_tilde_07_feedForward_buffer = this->comb_tilde_07_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_07_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_07_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_07_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(12);
        }
    
        this->comb_tilde_07_feedBack_buffer = this->comb_tilde_07_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_07_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_07_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_07_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(13);
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void signalreceive_01_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void signalreceive_02_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void dspexpr_05_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_10_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 75;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_07_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_07_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_07_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_07_feedForward_write(x[(Index)i]);
            this->comb_tilde_07_feedBack_write(ov);
            this->comb_tilde_07_feedForward_step();
            this->comb_tilde_07_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_01_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 110;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_01_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_01_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_01_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_01_feedForward_write(x[(Index)i]);
            this->comb_tilde_01_feedBack_write(ov);
            this->comb_tilde_01_feedForward_step();
            this->comb_tilde_01_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_02_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 160;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_02_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_02_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_02_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_02_feedForward_write(x[(Index)i]);
            this->comb_tilde_02_feedBack_write(ov);
            this->comb_tilde_02_feedForward_step();
            this->comb_tilde_02_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_03_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 50;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_03_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_03_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_03_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_03_feedForward_write(x[(Index)i]);
            this->comb_tilde_03_feedBack_write(ov);
            this->comb_tilde_03_feedForward_step();
            this->comb_tilde_03_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_06_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 25;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_04_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_04_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_04_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_04_feedForward_write(x[(Index)i]);
            this->comb_tilde_04_feedBack_write(ov);
            this->comb_tilde_04_feedForward_step();
            this->comb_tilde_04_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_08_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 69;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_05_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_05_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_05_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_05_feedForward_write(x[(Index)i]);
            this->comb_tilde_05_feedBack_write(ov);
            this->comb_tilde_05_feedForward_step();
            this->comb_tilde_05_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_09_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 80;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_06_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_06_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_06_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_06_feedForward_write(x[(Index)i]);
            this->comb_tilde_06_feedBack_write(ov);
            this->comb_tilde_06_feedForward_step();
            this->comb_tilde_06_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void signaladder_01_perform(
        const SampleValue * in1,
        const SampleValue * in2,
        const SampleValue * in3,
        const SampleValue * in4,
        SampleValue * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out[(Index)i] = in1[(Index)i] + in2[(Index)i] + in3[(Index)i] + in4[(Index)i];
        }
    }
    
    void dspexpr_07_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * -1;//#map:_###_obj_###_:1
        }
    }
    
    void signaladder_02_perform(
        const SampleValue * in1,
        const SampleValue * in2,
        const SampleValue * in3,
        const SampleValue * in4,
        SampleValue * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out[(Index)i] = in1[(Index)i] + in2[(Index)i] + in3[(Index)i] + in4[(Index)i];
        }
    }
    
    void dspexpr_04_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 0.355;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_01_feedForward_step() {
        this->comb_tilde_01_feedForward_reader++;
    
        if (this->comb_tilde_01_feedForward_reader >= (int)(this->comb_tilde_01_feedForward_buffer->getSize()))
            this->comb_tilde_01_feedForward_reader = 0;
    }
    
    number comb_tilde_01_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_01_feedForward_buffer->getSize()) + this->comb_tilde_01_feedForward_reader - ((size > this->comb_tilde_01_feedForward__maxdelay ? this->comb_tilde_01_feedForward__maxdelay : (size < (this->comb_tilde_01_feedForward_reader != this->comb_tilde_01_feedForward_writer) ? this->comb_tilde_01_feedForward_reader != this->comb_tilde_01_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ), this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_01_feedForward_buffer->getSize()) + this->comb_tilde_01_feedForward_reader - ((size > this->comb_tilde_01_feedForward__maxdelay ? this->comb_tilde_01_feedForward__maxdelay : (size < (1 + this->comb_tilde_01_feedForward_reader != this->comb_tilde_01_feedForward_writer) ? 1 + this->comb_tilde_01_feedForward_reader != this->comb_tilde_01_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ), this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ), this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ), this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_01_feedForward_buffer->getSize()) + this->comb_tilde_01_feedForward_reader - ((size > this->comb_tilde_01_feedForward__maxdelay ? this->comb_tilde_01_feedForward__maxdelay : (size < (1 + this->comb_tilde_01_feedForward_reader != this->comb_tilde_01_feedForward_writer) ? 1 + this->comb_tilde_01_feedForward_reader != this->comb_tilde_01_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ), this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ), this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ), this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_01_feedForward_buffer->getSize()) + this->comb_tilde_01_feedForward_reader - ((size > this->comb_tilde_01_feedForward__maxdelay ? this->comb_tilde_01_feedForward__maxdelay : (size < (this->comb_tilde_01_feedForward_reader != this->comb_tilde_01_feedForward_writer) ? this->comb_tilde_01_feedForward_reader != this->comb_tilde_01_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ), this->comb_tilde_01_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_01_feedForward_buffer->getSize()) + this->comb_tilde_01_feedForward_reader - ((size > this->comb_tilde_01_feedForward__maxdelay ? this->comb_tilde_01_feedForward__maxdelay : (size < (this->comb_tilde_01_feedForward_reader != this->comb_tilde_01_feedForward_writer) ? this->comb_tilde_01_feedForward_reader != this->comb_tilde_01_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_01_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_01_feedForward_wrap))
        );
    }
    
    void comb_tilde_01_feedForward_write(number v) {
        this->comb_tilde_01_feedForward_writer = this->comb_tilde_01_feedForward_reader;
        this->comb_tilde_01_feedForward_buffer[(Index)this->comb_tilde_01_feedForward_writer] = v;
    }
    
    number comb_tilde_01_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_01_feedForward__maxdelay : size);
        number val = this->comb_tilde_01_feedForward_read(effectiveSize, 0);
        this->comb_tilde_01_feedForward_write(v);
        this->comb_tilde_01_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_01_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_01_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_01_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_01_feedForward_init() {
        auto result = this->comb_tilde_01_feedForward_calcSizeInSamples();
        this->comb_tilde_01_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_01_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_01_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_01_feedForward_clear() {
        this->comb_tilde_01_feedForward_buffer->setZero();
    }
    
    void comb_tilde_01_feedForward_reset() {
        auto result = this->comb_tilde_01_feedForward_calcSizeInSamples();
        this->comb_tilde_01_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_01_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_01_feedForward_buffer);
        this->comb_tilde_01_feedForward_wrap = this->comb_tilde_01_feedForward_buffer->getSize() - 1;
        this->comb_tilde_01_feedForward_clear();
    
        if (this->comb_tilde_01_feedForward_reader >= this->comb_tilde_01_feedForward__maxdelay || this->comb_tilde_01_feedForward_writer >= this->comb_tilde_01_feedForward__maxdelay) {
            this->comb_tilde_01_feedForward_reader = 0;
            this->comb_tilde_01_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_01_feedForward_dspsetup() {
        this->comb_tilde_01_feedForward_reset();
    }
    
    number comb_tilde_01_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_01_feedForward_size() {
        return this->comb_tilde_01_feedForward__maxdelay;
    }
    
    void comb_tilde_01_feedBack_step() {
        this->comb_tilde_01_feedBack_reader++;
    
        if (this->comb_tilde_01_feedBack_reader >= (int)(this->comb_tilde_01_feedBack_buffer->getSize()))
            this->comb_tilde_01_feedBack_reader = 0;
    }
    
    number comb_tilde_01_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_01_feedBack_buffer->getSize()) + this->comb_tilde_01_feedBack_reader - ((size > this->comb_tilde_01_feedBack__maxdelay ? this->comb_tilde_01_feedBack__maxdelay : (size < (this->comb_tilde_01_feedBack_reader != this->comb_tilde_01_feedBack_writer) ? this->comb_tilde_01_feedBack_reader != this->comb_tilde_01_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ), this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_01_feedBack_buffer->getSize()) + this->comb_tilde_01_feedBack_reader - ((size > this->comb_tilde_01_feedBack__maxdelay ? this->comb_tilde_01_feedBack__maxdelay : (size < (1 + this->comb_tilde_01_feedBack_reader != this->comb_tilde_01_feedBack_writer) ? 1 + this->comb_tilde_01_feedBack_reader != this->comb_tilde_01_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ), this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ), this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ), this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_01_feedBack_buffer->getSize()) + this->comb_tilde_01_feedBack_reader - ((size > this->comb_tilde_01_feedBack__maxdelay ? this->comb_tilde_01_feedBack__maxdelay : (size < (1 + this->comb_tilde_01_feedBack_reader != this->comb_tilde_01_feedBack_writer) ? 1 + this->comb_tilde_01_feedBack_reader != this->comb_tilde_01_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ), this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ), this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ), this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_01_feedBack_buffer->getSize()) + this->comb_tilde_01_feedBack_reader - ((size > this->comb_tilde_01_feedBack__maxdelay ? this->comb_tilde_01_feedBack__maxdelay : (size < (this->comb_tilde_01_feedBack_reader != this->comb_tilde_01_feedBack_writer) ? this->comb_tilde_01_feedBack_reader != this->comb_tilde_01_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ), this->comb_tilde_01_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_01_feedBack_buffer->getSize()) + this->comb_tilde_01_feedBack_reader - ((size > this->comb_tilde_01_feedBack__maxdelay ? this->comb_tilde_01_feedBack__maxdelay : (size < (this->comb_tilde_01_feedBack_reader != this->comb_tilde_01_feedBack_writer) ? this->comb_tilde_01_feedBack_reader != this->comb_tilde_01_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_01_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_01_feedBack_wrap))
        );
    }
    
    void comb_tilde_01_feedBack_write(number v) {
        this->comb_tilde_01_feedBack_writer = this->comb_tilde_01_feedBack_reader;
        this->comb_tilde_01_feedBack_buffer[(Index)this->comb_tilde_01_feedBack_writer] = v;
    }
    
    number comb_tilde_01_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_01_feedBack__maxdelay : size);
        number val = this->comb_tilde_01_feedBack_read(effectiveSize, 0);
        this->comb_tilde_01_feedBack_write(v);
        this->comb_tilde_01_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_01_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_01_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_01_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_01_feedBack_init() {
        auto result = this->comb_tilde_01_feedBack_calcSizeInSamples();
        this->comb_tilde_01_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_01_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_01_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_01_feedBack_clear() {
        this->comb_tilde_01_feedBack_buffer->setZero();
    }
    
    void comb_tilde_01_feedBack_reset() {
        auto result = this->comb_tilde_01_feedBack_calcSizeInSamples();
        this->comb_tilde_01_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_01_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_01_feedBack_buffer);
        this->comb_tilde_01_feedBack_wrap = this->comb_tilde_01_feedBack_buffer->getSize() - 1;
        this->comb_tilde_01_feedBack_clear();
    
        if (this->comb_tilde_01_feedBack_reader >= this->comb_tilde_01_feedBack__maxdelay || this->comb_tilde_01_feedBack_writer >= this->comb_tilde_01_feedBack__maxdelay) {
            this->comb_tilde_01_feedBack_reader = 0;
            this->comb_tilde_01_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_01_feedBack_dspsetup() {
        this->comb_tilde_01_feedBack_reset();
    }
    
    number comb_tilde_01_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_01_feedBack_size() {
        return this->comb_tilde_01_feedBack__maxdelay;
    }
    
    void comb_tilde_01_init() {
        this->comb_tilde_01_feedForward_init();
        this->comb_tilde_01_feedBack_init();
    }
    
    void comb_tilde_01_clear() {
        this->comb_tilde_01_feedForward_clear();
        this->comb_tilde_01_feedBack_clear();
    }
    
    void comb_tilde_01_reset() {
        this->comb_tilde_01_init();
        this->comb_tilde_01_clear();
    }
    
    void comb_tilde_01_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_01_reset();
        this->comb_tilde_01_setupDone = true;
        this->comb_tilde_01_feedForward_dspsetup();
        this->comb_tilde_01_feedBack_dspsetup();
    }
    
    void comb_tilde_02_feedForward_step() {
        this->comb_tilde_02_feedForward_reader++;
    
        if (this->comb_tilde_02_feedForward_reader >= (int)(this->comb_tilde_02_feedForward_buffer->getSize()))
            this->comb_tilde_02_feedForward_reader = 0;
    }
    
    number comb_tilde_02_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_02_feedForward_buffer->getSize()) + this->comb_tilde_02_feedForward_reader - ((size > this->comb_tilde_02_feedForward__maxdelay ? this->comb_tilde_02_feedForward__maxdelay : (size < (this->comb_tilde_02_feedForward_reader != this->comb_tilde_02_feedForward_writer) ? this->comb_tilde_02_feedForward_reader != this->comb_tilde_02_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ), this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_02_feedForward_buffer->getSize()) + this->comb_tilde_02_feedForward_reader - ((size > this->comb_tilde_02_feedForward__maxdelay ? this->comb_tilde_02_feedForward__maxdelay : (size < (1 + this->comb_tilde_02_feedForward_reader != this->comb_tilde_02_feedForward_writer) ? 1 + this->comb_tilde_02_feedForward_reader != this->comb_tilde_02_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ), this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ), this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ), this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_02_feedForward_buffer->getSize()) + this->comb_tilde_02_feedForward_reader - ((size > this->comb_tilde_02_feedForward__maxdelay ? this->comb_tilde_02_feedForward__maxdelay : (size < (1 + this->comb_tilde_02_feedForward_reader != this->comb_tilde_02_feedForward_writer) ? 1 + this->comb_tilde_02_feedForward_reader != this->comb_tilde_02_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ), this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ), this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ), this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_02_feedForward_buffer->getSize()) + this->comb_tilde_02_feedForward_reader - ((size > this->comb_tilde_02_feedForward__maxdelay ? this->comb_tilde_02_feedForward__maxdelay : (size < (this->comb_tilde_02_feedForward_reader != this->comb_tilde_02_feedForward_writer) ? this->comb_tilde_02_feedForward_reader != this->comb_tilde_02_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ), this->comb_tilde_02_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_02_feedForward_buffer->getSize()) + this->comb_tilde_02_feedForward_reader - ((size > this->comb_tilde_02_feedForward__maxdelay ? this->comb_tilde_02_feedForward__maxdelay : (size < (this->comb_tilde_02_feedForward_reader != this->comb_tilde_02_feedForward_writer) ? this->comb_tilde_02_feedForward_reader != this->comb_tilde_02_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_02_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_02_feedForward_wrap))
        );
    }
    
    void comb_tilde_02_feedForward_write(number v) {
        this->comb_tilde_02_feedForward_writer = this->comb_tilde_02_feedForward_reader;
        this->comb_tilde_02_feedForward_buffer[(Index)this->comb_tilde_02_feedForward_writer] = v;
    }
    
    number comb_tilde_02_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_02_feedForward__maxdelay : size);
        number val = this->comb_tilde_02_feedForward_read(effectiveSize, 0);
        this->comb_tilde_02_feedForward_write(v);
        this->comb_tilde_02_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_02_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_02_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_02_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_02_feedForward_init() {
        auto result = this->comb_tilde_02_feedForward_calcSizeInSamples();
        this->comb_tilde_02_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_02_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_02_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_02_feedForward_clear() {
        this->comb_tilde_02_feedForward_buffer->setZero();
    }
    
    void comb_tilde_02_feedForward_reset() {
        auto result = this->comb_tilde_02_feedForward_calcSizeInSamples();
        this->comb_tilde_02_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_02_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_02_feedForward_buffer);
        this->comb_tilde_02_feedForward_wrap = this->comb_tilde_02_feedForward_buffer->getSize() - 1;
        this->comb_tilde_02_feedForward_clear();
    
        if (this->comb_tilde_02_feedForward_reader >= this->comb_tilde_02_feedForward__maxdelay || this->comb_tilde_02_feedForward_writer >= this->comb_tilde_02_feedForward__maxdelay) {
            this->comb_tilde_02_feedForward_reader = 0;
            this->comb_tilde_02_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_02_feedForward_dspsetup() {
        this->comb_tilde_02_feedForward_reset();
    }
    
    number comb_tilde_02_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_02_feedForward_size() {
        return this->comb_tilde_02_feedForward__maxdelay;
    }
    
    void comb_tilde_02_feedBack_step() {
        this->comb_tilde_02_feedBack_reader++;
    
        if (this->comb_tilde_02_feedBack_reader >= (int)(this->comb_tilde_02_feedBack_buffer->getSize()))
            this->comb_tilde_02_feedBack_reader = 0;
    }
    
    number comb_tilde_02_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_02_feedBack_buffer->getSize()) + this->comb_tilde_02_feedBack_reader - ((size > this->comb_tilde_02_feedBack__maxdelay ? this->comb_tilde_02_feedBack__maxdelay : (size < (this->comb_tilde_02_feedBack_reader != this->comb_tilde_02_feedBack_writer) ? this->comb_tilde_02_feedBack_reader != this->comb_tilde_02_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ), this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_02_feedBack_buffer->getSize()) + this->comb_tilde_02_feedBack_reader - ((size > this->comb_tilde_02_feedBack__maxdelay ? this->comb_tilde_02_feedBack__maxdelay : (size < (1 + this->comb_tilde_02_feedBack_reader != this->comb_tilde_02_feedBack_writer) ? 1 + this->comb_tilde_02_feedBack_reader != this->comb_tilde_02_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ), this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ), this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ), this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_02_feedBack_buffer->getSize()) + this->comb_tilde_02_feedBack_reader - ((size > this->comb_tilde_02_feedBack__maxdelay ? this->comb_tilde_02_feedBack__maxdelay : (size < (1 + this->comb_tilde_02_feedBack_reader != this->comb_tilde_02_feedBack_writer) ? 1 + this->comb_tilde_02_feedBack_reader != this->comb_tilde_02_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ), this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ), this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ), this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_02_feedBack_buffer->getSize()) + this->comb_tilde_02_feedBack_reader - ((size > this->comb_tilde_02_feedBack__maxdelay ? this->comb_tilde_02_feedBack__maxdelay : (size < (this->comb_tilde_02_feedBack_reader != this->comb_tilde_02_feedBack_writer) ? this->comb_tilde_02_feedBack_reader != this->comb_tilde_02_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ), this->comb_tilde_02_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_02_feedBack_buffer->getSize()) + this->comb_tilde_02_feedBack_reader - ((size > this->comb_tilde_02_feedBack__maxdelay ? this->comb_tilde_02_feedBack__maxdelay : (size < (this->comb_tilde_02_feedBack_reader != this->comb_tilde_02_feedBack_writer) ? this->comb_tilde_02_feedBack_reader != this->comb_tilde_02_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_02_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_02_feedBack_wrap))
        );
    }
    
    void comb_tilde_02_feedBack_write(number v) {
        this->comb_tilde_02_feedBack_writer = this->comb_tilde_02_feedBack_reader;
        this->comb_tilde_02_feedBack_buffer[(Index)this->comb_tilde_02_feedBack_writer] = v;
    }
    
    number comb_tilde_02_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_02_feedBack__maxdelay : size);
        number val = this->comb_tilde_02_feedBack_read(effectiveSize, 0);
        this->comb_tilde_02_feedBack_write(v);
        this->comb_tilde_02_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_02_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_02_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_02_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_02_feedBack_init() {
        auto result = this->comb_tilde_02_feedBack_calcSizeInSamples();
        this->comb_tilde_02_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_02_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_02_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_02_feedBack_clear() {
        this->comb_tilde_02_feedBack_buffer->setZero();
    }
    
    void comb_tilde_02_feedBack_reset() {
        auto result = this->comb_tilde_02_feedBack_calcSizeInSamples();
        this->comb_tilde_02_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_02_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_02_feedBack_buffer);
        this->comb_tilde_02_feedBack_wrap = this->comb_tilde_02_feedBack_buffer->getSize() - 1;
        this->comb_tilde_02_feedBack_clear();
    
        if (this->comb_tilde_02_feedBack_reader >= this->comb_tilde_02_feedBack__maxdelay || this->comb_tilde_02_feedBack_writer >= this->comb_tilde_02_feedBack__maxdelay) {
            this->comb_tilde_02_feedBack_reader = 0;
            this->comb_tilde_02_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_02_feedBack_dspsetup() {
        this->comb_tilde_02_feedBack_reset();
    }
    
    number comb_tilde_02_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_02_feedBack_size() {
        return this->comb_tilde_02_feedBack__maxdelay;
    }
    
    void comb_tilde_02_init() {
        this->comb_tilde_02_feedForward_init();
        this->comb_tilde_02_feedBack_init();
    }
    
    void comb_tilde_02_clear() {
        this->comb_tilde_02_feedForward_clear();
        this->comb_tilde_02_feedBack_clear();
    }
    
    void comb_tilde_02_reset() {
        this->comb_tilde_02_init();
        this->comb_tilde_02_clear();
    }
    
    void comb_tilde_02_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_02_reset();
        this->comb_tilde_02_setupDone = true;
        this->comb_tilde_02_feedForward_dspsetup();
        this->comb_tilde_02_feedBack_dspsetup();
    }
    
    void comb_tilde_03_feedForward_step() {
        this->comb_tilde_03_feedForward_reader++;
    
        if (this->comb_tilde_03_feedForward_reader >= (int)(this->comb_tilde_03_feedForward_buffer->getSize()))
            this->comb_tilde_03_feedForward_reader = 0;
    }
    
    number comb_tilde_03_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_03_feedForward_buffer->getSize()) + this->comb_tilde_03_feedForward_reader - ((size > this->comb_tilde_03_feedForward__maxdelay ? this->comb_tilde_03_feedForward__maxdelay : (size < (this->comb_tilde_03_feedForward_reader != this->comb_tilde_03_feedForward_writer) ? this->comb_tilde_03_feedForward_reader != this->comb_tilde_03_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ), this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_03_feedForward_buffer->getSize()) + this->comb_tilde_03_feedForward_reader - ((size > this->comb_tilde_03_feedForward__maxdelay ? this->comb_tilde_03_feedForward__maxdelay : (size < (1 + this->comb_tilde_03_feedForward_reader != this->comb_tilde_03_feedForward_writer) ? 1 + this->comb_tilde_03_feedForward_reader != this->comb_tilde_03_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ), this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ), this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ), this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_03_feedForward_buffer->getSize()) + this->comb_tilde_03_feedForward_reader - ((size > this->comb_tilde_03_feedForward__maxdelay ? this->comb_tilde_03_feedForward__maxdelay : (size < (1 + this->comb_tilde_03_feedForward_reader != this->comb_tilde_03_feedForward_writer) ? 1 + this->comb_tilde_03_feedForward_reader != this->comb_tilde_03_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ), this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ), this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ), this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_03_feedForward_buffer->getSize()) + this->comb_tilde_03_feedForward_reader - ((size > this->comb_tilde_03_feedForward__maxdelay ? this->comb_tilde_03_feedForward__maxdelay : (size < (this->comb_tilde_03_feedForward_reader != this->comb_tilde_03_feedForward_writer) ? this->comb_tilde_03_feedForward_reader != this->comb_tilde_03_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ), this->comb_tilde_03_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_03_feedForward_buffer->getSize()) + this->comb_tilde_03_feedForward_reader - ((size > this->comb_tilde_03_feedForward__maxdelay ? this->comb_tilde_03_feedForward__maxdelay : (size < (this->comb_tilde_03_feedForward_reader != this->comb_tilde_03_feedForward_writer) ? this->comb_tilde_03_feedForward_reader != this->comb_tilde_03_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_03_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_03_feedForward_wrap))
        );
    }
    
    void comb_tilde_03_feedForward_write(number v) {
        this->comb_tilde_03_feedForward_writer = this->comb_tilde_03_feedForward_reader;
        this->comb_tilde_03_feedForward_buffer[(Index)this->comb_tilde_03_feedForward_writer] = v;
    }
    
    number comb_tilde_03_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_03_feedForward__maxdelay : size);
        number val = this->comb_tilde_03_feedForward_read(effectiveSize, 0);
        this->comb_tilde_03_feedForward_write(v);
        this->comb_tilde_03_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_03_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_03_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_03_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_03_feedForward_init() {
        auto result = this->comb_tilde_03_feedForward_calcSizeInSamples();
        this->comb_tilde_03_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_03_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_03_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_03_feedForward_clear() {
        this->comb_tilde_03_feedForward_buffer->setZero();
    }
    
    void comb_tilde_03_feedForward_reset() {
        auto result = this->comb_tilde_03_feedForward_calcSizeInSamples();
        this->comb_tilde_03_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_03_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_03_feedForward_buffer);
        this->comb_tilde_03_feedForward_wrap = this->comb_tilde_03_feedForward_buffer->getSize() - 1;
        this->comb_tilde_03_feedForward_clear();
    
        if (this->comb_tilde_03_feedForward_reader >= this->comb_tilde_03_feedForward__maxdelay || this->comb_tilde_03_feedForward_writer >= this->comb_tilde_03_feedForward__maxdelay) {
            this->comb_tilde_03_feedForward_reader = 0;
            this->comb_tilde_03_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_03_feedForward_dspsetup() {
        this->comb_tilde_03_feedForward_reset();
    }
    
    number comb_tilde_03_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_03_feedForward_size() {
        return this->comb_tilde_03_feedForward__maxdelay;
    }
    
    void comb_tilde_03_feedBack_step() {
        this->comb_tilde_03_feedBack_reader++;
    
        if (this->comb_tilde_03_feedBack_reader >= (int)(this->comb_tilde_03_feedBack_buffer->getSize()))
            this->comb_tilde_03_feedBack_reader = 0;
    }
    
    number comb_tilde_03_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_03_feedBack_buffer->getSize()) + this->comb_tilde_03_feedBack_reader - ((size > this->comb_tilde_03_feedBack__maxdelay ? this->comb_tilde_03_feedBack__maxdelay : (size < (this->comb_tilde_03_feedBack_reader != this->comb_tilde_03_feedBack_writer) ? this->comb_tilde_03_feedBack_reader != this->comb_tilde_03_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ), this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_03_feedBack_buffer->getSize()) + this->comb_tilde_03_feedBack_reader - ((size > this->comb_tilde_03_feedBack__maxdelay ? this->comb_tilde_03_feedBack__maxdelay : (size < (1 + this->comb_tilde_03_feedBack_reader != this->comb_tilde_03_feedBack_writer) ? 1 + this->comb_tilde_03_feedBack_reader != this->comb_tilde_03_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ), this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ), this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ), this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_03_feedBack_buffer->getSize()) + this->comb_tilde_03_feedBack_reader - ((size > this->comb_tilde_03_feedBack__maxdelay ? this->comb_tilde_03_feedBack__maxdelay : (size < (1 + this->comb_tilde_03_feedBack_reader != this->comb_tilde_03_feedBack_writer) ? 1 + this->comb_tilde_03_feedBack_reader != this->comb_tilde_03_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ), this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ), this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ), this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_03_feedBack_buffer->getSize()) + this->comb_tilde_03_feedBack_reader - ((size > this->comb_tilde_03_feedBack__maxdelay ? this->comb_tilde_03_feedBack__maxdelay : (size < (this->comb_tilde_03_feedBack_reader != this->comb_tilde_03_feedBack_writer) ? this->comb_tilde_03_feedBack_reader != this->comb_tilde_03_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ), this->comb_tilde_03_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_03_feedBack_buffer->getSize()) + this->comb_tilde_03_feedBack_reader - ((size > this->comb_tilde_03_feedBack__maxdelay ? this->comb_tilde_03_feedBack__maxdelay : (size < (this->comb_tilde_03_feedBack_reader != this->comb_tilde_03_feedBack_writer) ? this->comb_tilde_03_feedBack_reader != this->comb_tilde_03_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_03_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_03_feedBack_wrap))
        );
    }
    
    void comb_tilde_03_feedBack_write(number v) {
        this->comb_tilde_03_feedBack_writer = this->comb_tilde_03_feedBack_reader;
        this->comb_tilde_03_feedBack_buffer[(Index)this->comb_tilde_03_feedBack_writer] = v;
    }
    
    number comb_tilde_03_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_03_feedBack__maxdelay : size);
        number val = this->comb_tilde_03_feedBack_read(effectiveSize, 0);
        this->comb_tilde_03_feedBack_write(v);
        this->comb_tilde_03_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_03_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_03_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_03_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_03_feedBack_init() {
        auto result = this->comb_tilde_03_feedBack_calcSizeInSamples();
        this->comb_tilde_03_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_03_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_03_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_03_feedBack_clear() {
        this->comb_tilde_03_feedBack_buffer->setZero();
    }
    
    void comb_tilde_03_feedBack_reset() {
        auto result = this->comb_tilde_03_feedBack_calcSizeInSamples();
        this->comb_tilde_03_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_03_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_03_feedBack_buffer);
        this->comb_tilde_03_feedBack_wrap = this->comb_tilde_03_feedBack_buffer->getSize() - 1;
        this->comb_tilde_03_feedBack_clear();
    
        if (this->comb_tilde_03_feedBack_reader >= this->comb_tilde_03_feedBack__maxdelay || this->comb_tilde_03_feedBack_writer >= this->comb_tilde_03_feedBack__maxdelay) {
            this->comb_tilde_03_feedBack_reader = 0;
            this->comb_tilde_03_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_03_feedBack_dspsetup() {
        this->comb_tilde_03_feedBack_reset();
    }
    
    number comb_tilde_03_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_03_feedBack_size() {
        return this->comb_tilde_03_feedBack__maxdelay;
    }
    
    void comb_tilde_03_init() {
        this->comb_tilde_03_feedForward_init();
        this->comb_tilde_03_feedBack_init();
    }
    
    void comb_tilde_03_clear() {
        this->comb_tilde_03_feedForward_clear();
        this->comb_tilde_03_feedBack_clear();
    }
    
    void comb_tilde_03_reset() {
        this->comb_tilde_03_init();
        this->comb_tilde_03_clear();
    }
    
    void comb_tilde_03_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_03_reset();
        this->comb_tilde_03_setupDone = true;
        this->comb_tilde_03_feedForward_dspsetup();
        this->comb_tilde_03_feedBack_dspsetup();
    }
    
    void comb_tilde_04_feedForward_step() {
        this->comb_tilde_04_feedForward_reader++;
    
        if (this->comb_tilde_04_feedForward_reader >= (int)(this->comb_tilde_04_feedForward_buffer->getSize()))
            this->comb_tilde_04_feedForward_reader = 0;
    }
    
    number comb_tilde_04_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_04_feedForward_buffer->getSize()) + this->comb_tilde_04_feedForward_reader - ((size > this->comb_tilde_04_feedForward__maxdelay ? this->comb_tilde_04_feedForward__maxdelay : (size < (this->comb_tilde_04_feedForward_reader != this->comb_tilde_04_feedForward_writer) ? this->comb_tilde_04_feedForward_reader != this->comb_tilde_04_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ), this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_04_feedForward_buffer->getSize()) + this->comb_tilde_04_feedForward_reader - ((size > this->comb_tilde_04_feedForward__maxdelay ? this->comb_tilde_04_feedForward__maxdelay : (size < (1 + this->comb_tilde_04_feedForward_reader != this->comb_tilde_04_feedForward_writer) ? 1 + this->comb_tilde_04_feedForward_reader != this->comb_tilde_04_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ), this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ), this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ), this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_04_feedForward_buffer->getSize()) + this->comb_tilde_04_feedForward_reader - ((size > this->comb_tilde_04_feedForward__maxdelay ? this->comb_tilde_04_feedForward__maxdelay : (size < (1 + this->comb_tilde_04_feedForward_reader != this->comb_tilde_04_feedForward_writer) ? 1 + this->comb_tilde_04_feedForward_reader != this->comb_tilde_04_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ), this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ), this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ), this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_04_feedForward_buffer->getSize()) + this->comb_tilde_04_feedForward_reader - ((size > this->comb_tilde_04_feedForward__maxdelay ? this->comb_tilde_04_feedForward__maxdelay : (size < (this->comb_tilde_04_feedForward_reader != this->comb_tilde_04_feedForward_writer) ? this->comb_tilde_04_feedForward_reader != this->comb_tilde_04_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ), this->comb_tilde_04_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_04_feedForward_buffer->getSize()) + this->comb_tilde_04_feedForward_reader - ((size > this->comb_tilde_04_feedForward__maxdelay ? this->comb_tilde_04_feedForward__maxdelay : (size < (this->comb_tilde_04_feedForward_reader != this->comb_tilde_04_feedForward_writer) ? this->comb_tilde_04_feedForward_reader != this->comb_tilde_04_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_04_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_04_feedForward_wrap))
        );
    }
    
    void comb_tilde_04_feedForward_write(number v) {
        this->comb_tilde_04_feedForward_writer = this->comb_tilde_04_feedForward_reader;
        this->comb_tilde_04_feedForward_buffer[(Index)this->comb_tilde_04_feedForward_writer] = v;
    }
    
    number comb_tilde_04_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_04_feedForward__maxdelay : size);
        number val = this->comb_tilde_04_feedForward_read(effectiveSize, 0);
        this->comb_tilde_04_feedForward_write(v);
        this->comb_tilde_04_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_04_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_04_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_04_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_04_feedForward_init() {
        auto result = this->comb_tilde_04_feedForward_calcSizeInSamples();
        this->comb_tilde_04_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_04_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_04_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_04_feedForward_clear() {
        this->comb_tilde_04_feedForward_buffer->setZero();
    }
    
    void comb_tilde_04_feedForward_reset() {
        auto result = this->comb_tilde_04_feedForward_calcSizeInSamples();
        this->comb_tilde_04_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_04_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_04_feedForward_buffer);
        this->comb_tilde_04_feedForward_wrap = this->comb_tilde_04_feedForward_buffer->getSize() - 1;
        this->comb_tilde_04_feedForward_clear();
    
        if (this->comb_tilde_04_feedForward_reader >= this->comb_tilde_04_feedForward__maxdelay || this->comb_tilde_04_feedForward_writer >= this->comb_tilde_04_feedForward__maxdelay) {
            this->comb_tilde_04_feedForward_reader = 0;
            this->comb_tilde_04_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_04_feedForward_dspsetup() {
        this->comb_tilde_04_feedForward_reset();
    }
    
    number comb_tilde_04_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_04_feedForward_size() {
        return this->comb_tilde_04_feedForward__maxdelay;
    }
    
    void comb_tilde_04_feedBack_step() {
        this->comb_tilde_04_feedBack_reader++;
    
        if (this->comb_tilde_04_feedBack_reader >= (int)(this->comb_tilde_04_feedBack_buffer->getSize()))
            this->comb_tilde_04_feedBack_reader = 0;
    }
    
    number comb_tilde_04_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_04_feedBack_buffer->getSize()) + this->comb_tilde_04_feedBack_reader - ((size > this->comb_tilde_04_feedBack__maxdelay ? this->comb_tilde_04_feedBack__maxdelay : (size < (this->comb_tilde_04_feedBack_reader != this->comb_tilde_04_feedBack_writer) ? this->comb_tilde_04_feedBack_reader != this->comb_tilde_04_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ), this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_04_feedBack_buffer->getSize()) + this->comb_tilde_04_feedBack_reader - ((size > this->comb_tilde_04_feedBack__maxdelay ? this->comb_tilde_04_feedBack__maxdelay : (size < (1 + this->comb_tilde_04_feedBack_reader != this->comb_tilde_04_feedBack_writer) ? 1 + this->comb_tilde_04_feedBack_reader != this->comb_tilde_04_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ), this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ), this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ), this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_04_feedBack_buffer->getSize()) + this->comb_tilde_04_feedBack_reader - ((size > this->comb_tilde_04_feedBack__maxdelay ? this->comb_tilde_04_feedBack__maxdelay : (size < (1 + this->comb_tilde_04_feedBack_reader != this->comb_tilde_04_feedBack_writer) ? 1 + this->comb_tilde_04_feedBack_reader != this->comb_tilde_04_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ), this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ), this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ), this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_04_feedBack_buffer->getSize()) + this->comb_tilde_04_feedBack_reader - ((size > this->comb_tilde_04_feedBack__maxdelay ? this->comb_tilde_04_feedBack__maxdelay : (size < (this->comb_tilde_04_feedBack_reader != this->comb_tilde_04_feedBack_writer) ? this->comb_tilde_04_feedBack_reader != this->comb_tilde_04_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ), this->comb_tilde_04_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_04_feedBack_buffer->getSize()) + this->comb_tilde_04_feedBack_reader - ((size > this->comb_tilde_04_feedBack__maxdelay ? this->comb_tilde_04_feedBack__maxdelay : (size < (this->comb_tilde_04_feedBack_reader != this->comb_tilde_04_feedBack_writer) ? this->comb_tilde_04_feedBack_reader != this->comb_tilde_04_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_04_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_04_feedBack_wrap))
        );
    }
    
    void comb_tilde_04_feedBack_write(number v) {
        this->comb_tilde_04_feedBack_writer = this->comb_tilde_04_feedBack_reader;
        this->comb_tilde_04_feedBack_buffer[(Index)this->comb_tilde_04_feedBack_writer] = v;
    }
    
    number comb_tilde_04_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_04_feedBack__maxdelay : size);
        number val = this->comb_tilde_04_feedBack_read(effectiveSize, 0);
        this->comb_tilde_04_feedBack_write(v);
        this->comb_tilde_04_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_04_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_04_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_04_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_04_feedBack_init() {
        auto result = this->comb_tilde_04_feedBack_calcSizeInSamples();
        this->comb_tilde_04_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_04_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_04_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_04_feedBack_clear() {
        this->comb_tilde_04_feedBack_buffer->setZero();
    }
    
    void comb_tilde_04_feedBack_reset() {
        auto result = this->comb_tilde_04_feedBack_calcSizeInSamples();
        this->comb_tilde_04_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_04_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_04_feedBack_buffer);
        this->comb_tilde_04_feedBack_wrap = this->comb_tilde_04_feedBack_buffer->getSize() - 1;
        this->comb_tilde_04_feedBack_clear();
    
        if (this->comb_tilde_04_feedBack_reader >= this->comb_tilde_04_feedBack__maxdelay || this->comb_tilde_04_feedBack_writer >= this->comb_tilde_04_feedBack__maxdelay) {
            this->comb_tilde_04_feedBack_reader = 0;
            this->comb_tilde_04_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_04_feedBack_dspsetup() {
        this->comb_tilde_04_feedBack_reset();
    }
    
    number comb_tilde_04_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_04_feedBack_size() {
        return this->comb_tilde_04_feedBack__maxdelay;
    }
    
    void comb_tilde_04_init() {
        this->comb_tilde_04_feedForward_init();
        this->comb_tilde_04_feedBack_init();
    }
    
    void comb_tilde_04_clear() {
        this->comb_tilde_04_feedForward_clear();
        this->comb_tilde_04_feedBack_clear();
    }
    
    void comb_tilde_04_reset() {
        this->comb_tilde_04_init();
        this->comb_tilde_04_clear();
    }
    
    void comb_tilde_04_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_04_reset();
        this->comb_tilde_04_setupDone = true;
        this->comb_tilde_04_feedForward_dspsetup();
        this->comb_tilde_04_feedBack_dspsetup();
    }
    
    void comb_tilde_05_feedForward_step() {
        this->comb_tilde_05_feedForward_reader++;
    
        if (this->comb_tilde_05_feedForward_reader >= (int)(this->comb_tilde_05_feedForward_buffer->getSize()))
            this->comb_tilde_05_feedForward_reader = 0;
    }
    
    number comb_tilde_05_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_05_feedForward_buffer->getSize()) + this->comb_tilde_05_feedForward_reader - ((size > this->comb_tilde_05_feedForward__maxdelay ? this->comb_tilde_05_feedForward__maxdelay : (size < (this->comb_tilde_05_feedForward_reader != this->comb_tilde_05_feedForward_writer) ? this->comb_tilde_05_feedForward_reader != this->comb_tilde_05_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ), this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_05_feedForward_buffer->getSize()) + this->comb_tilde_05_feedForward_reader - ((size > this->comb_tilde_05_feedForward__maxdelay ? this->comb_tilde_05_feedForward__maxdelay : (size < (1 + this->comb_tilde_05_feedForward_reader != this->comb_tilde_05_feedForward_writer) ? 1 + this->comb_tilde_05_feedForward_reader != this->comb_tilde_05_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ), this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ), this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ), this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_05_feedForward_buffer->getSize()) + this->comb_tilde_05_feedForward_reader - ((size > this->comb_tilde_05_feedForward__maxdelay ? this->comb_tilde_05_feedForward__maxdelay : (size < (1 + this->comb_tilde_05_feedForward_reader != this->comb_tilde_05_feedForward_writer) ? 1 + this->comb_tilde_05_feedForward_reader != this->comb_tilde_05_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ), this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ), this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ), this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_05_feedForward_buffer->getSize()) + this->comb_tilde_05_feedForward_reader - ((size > this->comb_tilde_05_feedForward__maxdelay ? this->comb_tilde_05_feedForward__maxdelay : (size < (this->comb_tilde_05_feedForward_reader != this->comb_tilde_05_feedForward_writer) ? this->comb_tilde_05_feedForward_reader != this->comb_tilde_05_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ), this->comb_tilde_05_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_05_feedForward_buffer->getSize()) + this->comb_tilde_05_feedForward_reader - ((size > this->comb_tilde_05_feedForward__maxdelay ? this->comb_tilde_05_feedForward__maxdelay : (size < (this->comb_tilde_05_feedForward_reader != this->comb_tilde_05_feedForward_writer) ? this->comb_tilde_05_feedForward_reader != this->comb_tilde_05_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_05_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_05_feedForward_wrap))
        );
    }
    
    void comb_tilde_05_feedForward_write(number v) {
        this->comb_tilde_05_feedForward_writer = this->comb_tilde_05_feedForward_reader;
        this->comb_tilde_05_feedForward_buffer[(Index)this->comb_tilde_05_feedForward_writer] = v;
    }
    
    number comb_tilde_05_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_05_feedForward__maxdelay : size);
        number val = this->comb_tilde_05_feedForward_read(effectiveSize, 0);
        this->comb_tilde_05_feedForward_write(v);
        this->comb_tilde_05_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_05_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_05_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_05_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_05_feedForward_init() {
        auto result = this->comb_tilde_05_feedForward_calcSizeInSamples();
        this->comb_tilde_05_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_05_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_05_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_05_feedForward_clear() {
        this->comb_tilde_05_feedForward_buffer->setZero();
    }
    
    void comb_tilde_05_feedForward_reset() {
        auto result = this->comb_tilde_05_feedForward_calcSizeInSamples();
        this->comb_tilde_05_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_05_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_05_feedForward_buffer);
        this->comb_tilde_05_feedForward_wrap = this->comb_tilde_05_feedForward_buffer->getSize() - 1;
        this->comb_tilde_05_feedForward_clear();
    
        if (this->comb_tilde_05_feedForward_reader >= this->comb_tilde_05_feedForward__maxdelay || this->comb_tilde_05_feedForward_writer >= this->comb_tilde_05_feedForward__maxdelay) {
            this->comb_tilde_05_feedForward_reader = 0;
            this->comb_tilde_05_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_05_feedForward_dspsetup() {
        this->comb_tilde_05_feedForward_reset();
    }
    
    number comb_tilde_05_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_05_feedForward_size() {
        return this->comb_tilde_05_feedForward__maxdelay;
    }
    
    void comb_tilde_05_feedBack_step() {
        this->comb_tilde_05_feedBack_reader++;
    
        if (this->comb_tilde_05_feedBack_reader >= (int)(this->comb_tilde_05_feedBack_buffer->getSize()))
            this->comb_tilde_05_feedBack_reader = 0;
    }
    
    number comb_tilde_05_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_05_feedBack_buffer->getSize()) + this->comb_tilde_05_feedBack_reader - ((size > this->comb_tilde_05_feedBack__maxdelay ? this->comb_tilde_05_feedBack__maxdelay : (size < (this->comb_tilde_05_feedBack_reader != this->comb_tilde_05_feedBack_writer) ? this->comb_tilde_05_feedBack_reader != this->comb_tilde_05_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ), this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_05_feedBack_buffer->getSize()) + this->comb_tilde_05_feedBack_reader - ((size > this->comb_tilde_05_feedBack__maxdelay ? this->comb_tilde_05_feedBack__maxdelay : (size < (1 + this->comb_tilde_05_feedBack_reader != this->comb_tilde_05_feedBack_writer) ? 1 + this->comb_tilde_05_feedBack_reader != this->comb_tilde_05_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ), this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ), this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ), this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_05_feedBack_buffer->getSize()) + this->comb_tilde_05_feedBack_reader - ((size > this->comb_tilde_05_feedBack__maxdelay ? this->comb_tilde_05_feedBack__maxdelay : (size < (1 + this->comb_tilde_05_feedBack_reader != this->comb_tilde_05_feedBack_writer) ? 1 + this->comb_tilde_05_feedBack_reader != this->comb_tilde_05_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ), this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ), this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ), this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_05_feedBack_buffer->getSize()) + this->comb_tilde_05_feedBack_reader - ((size > this->comb_tilde_05_feedBack__maxdelay ? this->comb_tilde_05_feedBack__maxdelay : (size < (this->comb_tilde_05_feedBack_reader != this->comb_tilde_05_feedBack_writer) ? this->comb_tilde_05_feedBack_reader != this->comb_tilde_05_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ), this->comb_tilde_05_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_05_feedBack_buffer->getSize()) + this->comb_tilde_05_feedBack_reader - ((size > this->comb_tilde_05_feedBack__maxdelay ? this->comb_tilde_05_feedBack__maxdelay : (size < (this->comb_tilde_05_feedBack_reader != this->comb_tilde_05_feedBack_writer) ? this->comb_tilde_05_feedBack_reader != this->comb_tilde_05_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_05_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_05_feedBack_wrap))
        );
    }
    
    void comb_tilde_05_feedBack_write(number v) {
        this->comb_tilde_05_feedBack_writer = this->comb_tilde_05_feedBack_reader;
        this->comb_tilde_05_feedBack_buffer[(Index)this->comb_tilde_05_feedBack_writer] = v;
    }
    
    number comb_tilde_05_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_05_feedBack__maxdelay : size);
        number val = this->comb_tilde_05_feedBack_read(effectiveSize, 0);
        this->comb_tilde_05_feedBack_write(v);
        this->comb_tilde_05_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_05_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_05_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_05_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_05_feedBack_init() {
        auto result = this->comb_tilde_05_feedBack_calcSizeInSamples();
        this->comb_tilde_05_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_05_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_05_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_05_feedBack_clear() {
        this->comb_tilde_05_feedBack_buffer->setZero();
    }
    
    void comb_tilde_05_feedBack_reset() {
        auto result = this->comb_tilde_05_feedBack_calcSizeInSamples();
        this->comb_tilde_05_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_05_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_05_feedBack_buffer);
        this->comb_tilde_05_feedBack_wrap = this->comb_tilde_05_feedBack_buffer->getSize() - 1;
        this->comb_tilde_05_feedBack_clear();
    
        if (this->comb_tilde_05_feedBack_reader >= this->comb_tilde_05_feedBack__maxdelay || this->comb_tilde_05_feedBack_writer >= this->comb_tilde_05_feedBack__maxdelay) {
            this->comb_tilde_05_feedBack_reader = 0;
            this->comb_tilde_05_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_05_feedBack_dspsetup() {
        this->comb_tilde_05_feedBack_reset();
    }
    
    number comb_tilde_05_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_05_feedBack_size() {
        return this->comb_tilde_05_feedBack__maxdelay;
    }
    
    void comb_tilde_05_init() {
        this->comb_tilde_05_feedForward_init();
        this->comb_tilde_05_feedBack_init();
    }
    
    void comb_tilde_05_clear() {
        this->comb_tilde_05_feedForward_clear();
        this->comb_tilde_05_feedBack_clear();
    }
    
    void comb_tilde_05_reset() {
        this->comb_tilde_05_init();
        this->comb_tilde_05_clear();
    }
    
    void comb_tilde_05_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_05_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_05_reset();
        this->comb_tilde_05_setupDone = true;
        this->comb_tilde_05_feedForward_dspsetup();
        this->comb_tilde_05_feedBack_dspsetup();
    }
    
    void comb_tilde_06_feedForward_step() {
        this->comb_tilde_06_feedForward_reader++;
    
        if (this->comb_tilde_06_feedForward_reader >= (int)(this->comb_tilde_06_feedForward_buffer->getSize()))
            this->comb_tilde_06_feedForward_reader = 0;
    }
    
    number comb_tilde_06_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_06_feedForward_buffer->getSize()) + this->comb_tilde_06_feedForward_reader - ((size > this->comb_tilde_06_feedForward__maxdelay ? this->comb_tilde_06_feedForward__maxdelay : (size < (this->comb_tilde_06_feedForward_reader != this->comb_tilde_06_feedForward_writer) ? this->comb_tilde_06_feedForward_reader != this->comb_tilde_06_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ), this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_06_feedForward_buffer->getSize()) + this->comb_tilde_06_feedForward_reader - ((size > this->comb_tilde_06_feedForward__maxdelay ? this->comb_tilde_06_feedForward__maxdelay : (size < (1 + this->comb_tilde_06_feedForward_reader != this->comb_tilde_06_feedForward_writer) ? 1 + this->comb_tilde_06_feedForward_reader != this->comb_tilde_06_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ), this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ), this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ), this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_06_feedForward_buffer->getSize()) + this->comb_tilde_06_feedForward_reader - ((size > this->comb_tilde_06_feedForward__maxdelay ? this->comb_tilde_06_feedForward__maxdelay : (size < (1 + this->comb_tilde_06_feedForward_reader != this->comb_tilde_06_feedForward_writer) ? 1 + this->comb_tilde_06_feedForward_reader != this->comb_tilde_06_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ), this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ), this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ), this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_06_feedForward_buffer->getSize()) + this->comb_tilde_06_feedForward_reader - ((size > this->comb_tilde_06_feedForward__maxdelay ? this->comb_tilde_06_feedForward__maxdelay : (size < (this->comb_tilde_06_feedForward_reader != this->comb_tilde_06_feedForward_writer) ? this->comb_tilde_06_feedForward_reader != this->comb_tilde_06_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ), this->comb_tilde_06_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_06_feedForward_buffer->getSize()) + this->comb_tilde_06_feedForward_reader - ((size > this->comb_tilde_06_feedForward__maxdelay ? this->comb_tilde_06_feedForward__maxdelay : (size < (this->comb_tilde_06_feedForward_reader != this->comb_tilde_06_feedForward_writer) ? this->comb_tilde_06_feedForward_reader != this->comb_tilde_06_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_06_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_06_feedForward_wrap))
        );
    }
    
    void comb_tilde_06_feedForward_write(number v) {
        this->comb_tilde_06_feedForward_writer = this->comb_tilde_06_feedForward_reader;
        this->comb_tilde_06_feedForward_buffer[(Index)this->comb_tilde_06_feedForward_writer] = v;
    }
    
    number comb_tilde_06_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_06_feedForward__maxdelay : size);
        number val = this->comb_tilde_06_feedForward_read(effectiveSize, 0);
        this->comb_tilde_06_feedForward_write(v);
        this->comb_tilde_06_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_06_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_06_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_06_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_06_feedForward_init() {
        auto result = this->comb_tilde_06_feedForward_calcSizeInSamples();
        this->comb_tilde_06_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_06_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_06_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_06_feedForward_clear() {
        this->comb_tilde_06_feedForward_buffer->setZero();
    }
    
    void comb_tilde_06_feedForward_reset() {
        auto result = this->comb_tilde_06_feedForward_calcSizeInSamples();
        this->comb_tilde_06_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_06_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_06_feedForward_buffer);
        this->comb_tilde_06_feedForward_wrap = this->comb_tilde_06_feedForward_buffer->getSize() - 1;
        this->comb_tilde_06_feedForward_clear();
    
        if (this->comb_tilde_06_feedForward_reader >= this->comb_tilde_06_feedForward__maxdelay || this->comb_tilde_06_feedForward_writer >= this->comb_tilde_06_feedForward__maxdelay) {
            this->comb_tilde_06_feedForward_reader = 0;
            this->comb_tilde_06_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_06_feedForward_dspsetup() {
        this->comb_tilde_06_feedForward_reset();
    }
    
    number comb_tilde_06_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_06_feedForward_size() {
        return this->comb_tilde_06_feedForward__maxdelay;
    }
    
    void comb_tilde_06_feedBack_step() {
        this->comb_tilde_06_feedBack_reader++;
    
        if (this->comb_tilde_06_feedBack_reader >= (int)(this->comb_tilde_06_feedBack_buffer->getSize()))
            this->comb_tilde_06_feedBack_reader = 0;
    }
    
    number comb_tilde_06_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_06_feedBack_buffer->getSize()) + this->comb_tilde_06_feedBack_reader - ((size > this->comb_tilde_06_feedBack__maxdelay ? this->comb_tilde_06_feedBack__maxdelay : (size < (this->comb_tilde_06_feedBack_reader != this->comb_tilde_06_feedBack_writer) ? this->comb_tilde_06_feedBack_reader != this->comb_tilde_06_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ), this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_06_feedBack_buffer->getSize()) + this->comb_tilde_06_feedBack_reader - ((size > this->comb_tilde_06_feedBack__maxdelay ? this->comb_tilde_06_feedBack__maxdelay : (size < (1 + this->comb_tilde_06_feedBack_reader != this->comb_tilde_06_feedBack_writer) ? 1 + this->comb_tilde_06_feedBack_reader != this->comb_tilde_06_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ), this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ), this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ), this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_06_feedBack_buffer->getSize()) + this->comb_tilde_06_feedBack_reader - ((size > this->comb_tilde_06_feedBack__maxdelay ? this->comb_tilde_06_feedBack__maxdelay : (size < (1 + this->comb_tilde_06_feedBack_reader != this->comb_tilde_06_feedBack_writer) ? 1 + this->comb_tilde_06_feedBack_reader != this->comb_tilde_06_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ), this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ), this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ), this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_06_feedBack_buffer->getSize()) + this->comb_tilde_06_feedBack_reader - ((size > this->comb_tilde_06_feedBack__maxdelay ? this->comb_tilde_06_feedBack__maxdelay : (size < (this->comb_tilde_06_feedBack_reader != this->comb_tilde_06_feedBack_writer) ? this->comb_tilde_06_feedBack_reader != this->comb_tilde_06_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ), this->comb_tilde_06_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_06_feedBack_buffer->getSize()) + this->comb_tilde_06_feedBack_reader - ((size > this->comb_tilde_06_feedBack__maxdelay ? this->comb_tilde_06_feedBack__maxdelay : (size < (this->comb_tilde_06_feedBack_reader != this->comb_tilde_06_feedBack_writer) ? this->comb_tilde_06_feedBack_reader != this->comb_tilde_06_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_06_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_06_feedBack_wrap))
        );
    }
    
    void comb_tilde_06_feedBack_write(number v) {
        this->comb_tilde_06_feedBack_writer = this->comb_tilde_06_feedBack_reader;
        this->comb_tilde_06_feedBack_buffer[(Index)this->comb_tilde_06_feedBack_writer] = v;
    }
    
    number comb_tilde_06_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_06_feedBack__maxdelay : size);
        number val = this->comb_tilde_06_feedBack_read(effectiveSize, 0);
        this->comb_tilde_06_feedBack_write(v);
        this->comb_tilde_06_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_06_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_06_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_06_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_06_feedBack_init() {
        auto result = this->comb_tilde_06_feedBack_calcSizeInSamples();
        this->comb_tilde_06_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_06_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_06_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_06_feedBack_clear() {
        this->comb_tilde_06_feedBack_buffer->setZero();
    }
    
    void comb_tilde_06_feedBack_reset() {
        auto result = this->comb_tilde_06_feedBack_calcSizeInSamples();
        this->comb_tilde_06_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_06_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_06_feedBack_buffer);
        this->comb_tilde_06_feedBack_wrap = this->comb_tilde_06_feedBack_buffer->getSize() - 1;
        this->comb_tilde_06_feedBack_clear();
    
        if (this->comb_tilde_06_feedBack_reader >= this->comb_tilde_06_feedBack__maxdelay || this->comb_tilde_06_feedBack_writer >= this->comb_tilde_06_feedBack__maxdelay) {
            this->comb_tilde_06_feedBack_reader = 0;
            this->comb_tilde_06_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_06_feedBack_dspsetup() {
        this->comb_tilde_06_feedBack_reset();
    }
    
    number comb_tilde_06_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_06_feedBack_size() {
        return this->comb_tilde_06_feedBack__maxdelay;
    }
    
    void comb_tilde_06_init() {
        this->comb_tilde_06_feedForward_init();
        this->comb_tilde_06_feedBack_init();
    }
    
    void comb_tilde_06_clear() {
        this->comb_tilde_06_feedForward_clear();
        this->comb_tilde_06_feedBack_clear();
    }
    
    void comb_tilde_06_reset() {
        this->comb_tilde_06_init();
        this->comb_tilde_06_clear();
    }
    
    void comb_tilde_06_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_06_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_06_reset();
        this->comb_tilde_06_setupDone = true;
        this->comb_tilde_06_feedForward_dspsetup();
        this->comb_tilde_06_feedBack_dspsetup();
    }
    
    void comb_tilde_07_feedForward_step() {
        this->comb_tilde_07_feedForward_reader++;
    
        if (this->comb_tilde_07_feedForward_reader >= (int)(this->comb_tilde_07_feedForward_buffer->getSize()))
            this->comb_tilde_07_feedForward_reader = 0;
    }
    
    number comb_tilde_07_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_07_feedForward_buffer->getSize()) + this->comb_tilde_07_feedForward_reader - ((size > this->comb_tilde_07_feedForward__maxdelay ? this->comb_tilde_07_feedForward__maxdelay : (size < (this->comb_tilde_07_feedForward_reader != this->comb_tilde_07_feedForward_writer) ? this->comb_tilde_07_feedForward_reader != this->comb_tilde_07_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ), this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_07_feedForward_buffer->getSize()) + this->comb_tilde_07_feedForward_reader - ((size > this->comb_tilde_07_feedForward__maxdelay ? this->comb_tilde_07_feedForward__maxdelay : (size < (1 + this->comb_tilde_07_feedForward_reader != this->comb_tilde_07_feedForward_writer) ? 1 + this->comb_tilde_07_feedForward_reader != this->comb_tilde_07_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ), this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ), this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ), this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_07_feedForward_buffer->getSize()) + this->comb_tilde_07_feedForward_reader - ((size > this->comb_tilde_07_feedForward__maxdelay ? this->comb_tilde_07_feedForward__maxdelay : (size < (1 + this->comb_tilde_07_feedForward_reader != this->comb_tilde_07_feedForward_writer) ? 1 + this->comb_tilde_07_feedForward_reader != this->comb_tilde_07_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ), this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ), this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ), this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_07_feedForward_buffer->getSize()) + this->comb_tilde_07_feedForward_reader - ((size > this->comb_tilde_07_feedForward__maxdelay ? this->comb_tilde_07_feedForward__maxdelay : (size < (this->comb_tilde_07_feedForward_reader != this->comb_tilde_07_feedForward_writer) ? this->comb_tilde_07_feedForward_reader != this->comb_tilde_07_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ), this->comb_tilde_07_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_07_feedForward_buffer->getSize()) + this->comb_tilde_07_feedForward_reader - ((size > this->comb_tilde_07_feedForward__maxdelay ? this->comb_tilde_07_feedForward__maxdelay : (size < (this->comb_tilde_07_feedForward_reader != this->comb_tilde_07_feedForward_writer) ? this->comb_tilde_07_feedForward_reader != this->comb_tilde_07_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_07_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_07_feedForward_wrap))
        );
    }
    
    void comb_tilde_07_feedForward_write(number v) {
        this->comb_tilde_07_feedForward_writer = this->comb_tilde_07_feedForward_reader;
        this->comb_tilde_07_feedForward_buffer[(Index)this->comb_tilde_07_feedForward_writer] = v;
    }
    
    number comb_tilde_07_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_07_feedForward__maxdelay : size);
        number val = this->comb_tilde_07_feedForward_read(effectiveSize, 0);
        this->comb_tilde_07_feedForward_write(v);
        this->comb_tilde_07_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_07_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_07_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_07_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_07_feedForward_init() {
        auto result = this->comb_tilde_07_feedForward_calcSizeInSamples();
        this->comb_tilde_07_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_07_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_07_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_07_feedForward_clear() {
        this->comb_tilde_07_feedForward_buffer->setZero();
    }
    
    void comb_tilde_07_feedForward_reset() {
        auto result = this->comb_tilde_07_feedForward_calcSizeInSamples();
        this->comb_tilde_07_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_07_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_07_feedForward_buffer);
        this->comb_tilde_07_feedForward_wrap = this->comb_tilde_07_feedForward_buffer->getSize() - 1;
        this->comb_tilde_07_feedForward_clear();
    
        if (this->comb_tilde_07_feedForward_reader >= this->comb_tilde_07_feedForward__maxdelay || this->comb_tilde_07_feedForward_writer >= this->comb_tilde_07_feedForward__maxdelay) {
            this->comb_tilde_07_feedForward_reader = 0;
            this->comb_tilde_07_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_07_feedForward_dspsetup() {
        this->comb_tilde_07_feedForward_reset();
    }
    
    number comb_tilde_07_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_07_feedForward_size() {
        return this->comb_tilde_07_feedForward__maxdelay;
    }
    
    void comb_tilde_07_feedBack_step() {
        this->comb_tilde_07_feedBack_reader++;
    
        if (this->comb_tilde_07_feedBack_reader >= (int)(this->comb_tilde_07_feedBack_buffer->getSize()))
            this->comb_tilde_07_feedBack_reader = 0;
    }
    
    number comb_tilde_07_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_07_feedBack_buffer->getSize()) + this->comb_tilde_07_feedBack_reader - ((size > this->comb_tilde_07_feedBack__maxdelay ? this->comb_tilde_07_feedBack__maxdelay : (size < (this->comb_tilde_07_feedBack_reader != this->comb_tilde_07_feedBack_writer) ? this->comb_tilde_07_feedBack_reader != this->comb_tilde_07_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ), this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_07_feedBack_buffer->getSize()) + this->comb_tilde_07_feedBack_reader - ((size > this->comb_tilde_07_feedBack__maxdelay ? this->comb_tilde_07_feedBack__maxdelay : (size < (1 + this->comb_tilde_07_feedBack_reader != this->comb_tilde_07_feedBack_writer) ? 1 + this->comb_tilde_07_feedBack_reader != this->comb_tilde_07_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ), this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ), this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ), this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_07_feedBack_buffer->getSize()) + this->comb_tilde_07_feedBack_reader - ((size > this->comb_tilde_07_feedBack__maxdelay ? this->comb_tilde_07_feedBack__maxdelay : (size < (1 + this->comb_tilde_07_feedBack_reader != this->comb_tilde_07_feedBack_writer) ? 1 + this->comb_tilde_07_feedBack_reader != this->comb_tilde_07_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ), this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ), this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ), this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_07_feedBack_buffer->getSize()) + this->comb_tilde_07_feedBack_reader - ((size > this->comb_tilde_07_feedBack__maxdelay ? this->comb_tilde_07_feedBack__maxdelay : (size < (this->comb_tilde_07_feedBack_reader != this->comb_tilde_07_feedBack_writer) ? this->comb_tilde_07_feedBack_reader != this->comb_tilde_07_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ), this->comb_tilde_07_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_07_feedBack_buffer->getSize()) + this->comb_tilde_07_feedBack_reader - ((size > this->comb_tilde_07_feedBack__maxdelay ? this->comb_tilde_07_feedBack__maxdelay : (size < (this->comb_tilde_07_feedBack_reader != this->comb_tilde_07_feedBack_writer) ? this->comb_tilde_07_feedBack_reader != this->comb_tilde_07_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_07_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_07_feedBack_wrap))
        );
    }
    
    void comb_tilde_07_feedBack_write(number v) {
        this->comb_tilde_07_feedBack_writer = this->comb_tilde_07_feedBack_reader;
        this->comb_tilde_07_feedBack_buffer[(Index)this->comb_tilde_07_feedBack_writer] = v;
    }
    
    number comb_tilde_07_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_07_feedBack__maxdelay : size);
        number val = this->comb_tilde_07_feedBack_read(effectiveSize, 0);
        this->comb_tilde_07_feedBack_write(v);
        this->comb_tilde_07_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_07_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_07_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_07_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_07_feedBack_init() {
        auto result = this->comb_tilde_07_feedBack_calcSizeInSamples();
        this->comb_tilde_07_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_07_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_07_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_07_feedBack_clear() {
        this->comb_tilde_07_feedBack_buffer->setZero();
    }
    
    void comb_tilde_07_feedBack_reset() {
        auto result = this->comb_tilde_07_feedBack_calcSizeInSamples();
        this->comb_tilde_07_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_07_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_07_feedBack_buffer);
        this->comb_tilde_07_feedBack_wrap = this->comb_tilde_07_feedBack_buffer->getSize() - 1;
        this->comb_tilde_07_feedBack_clear();
    
        if (this->comb_tilde_07_feedBack_reader >= this->comb_tilde_07_feedBack__maxdelay || this->comb_tilde_07_feedBack_writer >= this->comb_tilde_07_feedBack__maxdelay) {
            this->comb_tilde_07_feedBack_reader = 0;
            this->comb_tilde_07_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_07_feedBack_dspsetup() {
        this->comb_tilde_07_feedBack_reset();
    }
    
    number comb_tilde_07_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_07_feedBack_size() {
        return this->comb_tilde_07_feedBack__maxdelay;
    }
    
    void comb_tilde_07_init() {
        this->comb_tilde_07_feedForward_init();
        this->comb_tilde_07_feedBack_init();
    }
    
    void comb_tilde_07_clear() {
        this->comb_tilde_07_feedForward_clear();
        this->comb_tilde_07_feedBack_clear();
    }
    
    void comb_tilde_07_reset() {
        this->comb_tilde_07_init();
        this->comb_tilde_07_clear();
    }
    
    void comb_tilde_07_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_07_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_07_reset();
        this->comb_tilde_07_setupDone = true;
        this->comb_tilde_07_feedForward_dspsetup();
        this->comb_tilde_07_feedBack_dspsetup();
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
        comb_tilde_01_x = 0;
        comb_tilde_01_delayTime = 110;
        comb_tilde_01_gain = 0;
        comb_tilde_01_ffGain = 1;
        comb_tilde_01_fbGain = 0;
        dspexpr_01_in1 = 0;
        dspexpr_01_in2 = 110;
        comb_tilde_02_x = 0;
        comb_tilde_02_delayTime = 160;
        comb_tilde_02_gain = 0;
        comb_tilde_02_ffGain = 1;
        comb_tilde_02_fbGain = 0;
        dspexpr_02_in1 = 0;
        dspexpr_02_in2 = 160;
        comb_tilde_03_x = 0;
        comb_tilde_03_delayTime = 50;
        comb_tilde_03_gain = 0;
        comb_tilde_03_ffGain = 1;
        comb_tilde_03_fbGain = 0;
        dspexpr_03_in1 = 0;
        dspexpr_03_in2 = 50;
        dspexpr_04_in1 = 0;
        dspexpr_04_in2 = 0.355;
        comb_tilde_04_x = 0;
        comb_tilde_04_delayTime = 25;
        comb_tilde_04_gain = 0;
        comb_tilde_04_ffGain = 1;
        comb_tilde_04_fbGain = 0;
        dspexpr_05_in1 = 0;
        dspexpr_05_in2 = 0;
        dspexpr_06_in1 = 0;
        dspexpr_06_in2 = 25;
        dspexpr_07_in1 = 0;
        dspexpr_07_in2 = -1;
        comb_tilde_05_x = 0;
        comb_tilde_05_delayTime = 69;
        comb_tilde_05_gain = 0;
        comb_tilde_05_ffGain = 1;
        comb_tilde_05_fbGain = 0;
        dspexpr_08_in1 = 0;
        dspexpr_08_in2 = 69;
        comb_tilde_06_x = 0;
        comb_tilde_06_delayTime = 80;
        comb_tilde_06_gain = 0;
        comb_tilde_06_ffGain = 1;
        comb_tilde_06_fbGain = 0;
        dspexpr_09_in1 = 0;
        dspexpr_09_in2 = 80;
        comb_tilde_07_x = 0;
        comb_tilde_07_delayTime = 75;
        comb_tilde_07_gain = 0;
        comb_tilde_07_ffGain = 1;
        comb_tilde_07_fbGain = 0;
        dspexpr_10_in1 = 0;
        dspexpr_10_in2 = 75;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        signals[4] = nullptr;
        signals[5] = nullptr;
        signals[6] = nullptr;
        signals[7] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        comb_tilde_01_feedForward__maxdelay = 0;
        comb_tilde_01_feedForward_sizemode = 0;
        comb_tilde_01_feedForward_wrap = 0;
        comb_tilde_01_feedForward_reader = 0;
        comb_tilde_01_feedForward_writer = 0;
        comb_tilde_01_feedBack__maxdelay = 0;
        comb_tilde_01_feedBack_sizemode = 0;
        comb_tilde_01_feedBack_wrap = 0;
        comb_tilde_01_feedBack_reader = 0;
        comb_tilde_01_feedBack_writer = 0;
        comb_tilde_01_setupDone = false;
        comb_tilde_02_feedForward__maxdelay = 0;
        comb_tilde_02_feedForward_sizemode = 0;
        comb_tilde_02_feedForward_wrap = 0;
        comb_tilde_02_feedForward_reader = 0;
        comb_tilde_02_feedForward_writer = 0;
        comb_tilde_02_feedBack__maxdelay = 0;
        comb_tilde_02_feedBack_sizemode = 0;
        comb_tilde_02_feedBack_wrap = 0;
        comb_tilde_02_feedBack_reader = 0;
        comb_tilde_02_feedBack_writer = 0;
        comb_tilde_02_setupDone = false;
        comb_tilde_03_feedForward__maxdelay = 0;
        comb_tilde_03_feedForward_sizemode = 0;
        comb_tilde_03_feedForward_wrap = 0;
        comb_tilde_03_feedForward_reader = 0;
        comb_tilde_03_feedForward_writer = 0;
        comb_tilde_03_feedBack__maxdelay = 0;
        comb_tilde_03_feedBack_sizemode = 0;
        comb_tilde_03_feedBack_wrap = 0;
        comb_tilde_03_feedBack_reader = 0;
        comb_tilde_03_feedBack_writer = 0;
        comb_tilde_03_setupDone = false;
        comb_tilde_04_feedForward__maxdelay = 0;
        comb_tilde_04_feedForward_sizemode = 0;
        comb_tilde_04_feedForward_wrap = 0;
        comb_tilde_04_feedForward_reader = 0;
        comb_tilde_04_feedForward_writer = 0;
        comb_tilde_04_feedBack__maxdelay = 0;
        comb_tilde_04_feedBack_sizemode = 0;
        comb_tilde_04_feedBack_wrap = 0;
        comb_tilde_04_feedBack_reader = 0;
        comb_tilde_04_feedBack_writer = 0;
        comb_tilde_04_setupDone = false;
        comb_tilde_05_feedForward__maxdelay = 0;
        comb_tilde_05_feedForward_sizemode = 0;
        comb_tilde_05_feedForward_wrap = 0;
        comb_tilde_05_feedForward_reader = 0;
        comb_tilde_05_feedForward_writer = 0;
        comb_tilde_05_feedBack__maxdelay = 0;
        comb_tilde_05_feedBack_sizemode = 0;
        comb_tilde_05_feedBack_wrap = 0;
        comb_tilde_05_feedBack_reader = 0;
        comb_tilde_05_feedBack_writer = 0;
        comb_tilde_05_setupDone = false;
        comb_tilde_06_feedForward__maxdelay = 0;
        comb_tilde_06_feedForward_sizemode = 0;
        comb_tilde_06_feedForward_wrap = 0;
        comb_tilde_06_feedForward_reader = 0;
        comb_tilde_06_feedForward_writer = 0;
        comb_tilde_06_feedBack__maxdelay = 0;
        comb_tilde_06_feedBack_sizemode = 0;
        comb_tilde_06_feedBack_wrap = 0;
        comb_tilde_06_feedBack_reader = 0;
        comb_tilde_06_feedBack_writer = 0;
        comb_tilde_06_setupDone = false;
        comb_tilde_07_feedForward__maxdelay = 0;
        comb_tilde_07_feedForward_sizemode = 0;
        comb_tilde_07_feedForward_wrap = 0;
        comb_tilde_07_feedForward_reader = 0;
        comb_tilde_07_feedForward_writer = 0;
        comb_tilde_07_feedBack__maxdelay = 0;
        comb_tilde_07_feedBack_sizemode = 0;
        comb_tilde_07_feedBack_wrap = 0;
        comb_tilde_07_feedBack_reader = 0;
        comb_tilde_07_feedBack_writer = 0;
        comb_tilde_07_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number comb_tilde_01_x;
        number comb_tilde_01_delayTime;
        number comb_tilde_01_gain;
        number comb_tilde_01_ffGain;
        number comb_tilde_01_fbGain;
        number dspexpr_01_in1;
        number dspexpr_01_in2;
        number comb_tilde_02_x;
        number comb_tilde_02_delayTime;
        number comb_tilde_02_gain;
        number comb_tilde_02_ffGain;
        number comb_tilde_02_fbGain;
        number dspexpr_02_in1;
        number dspexpr_02_in2;
        number comb_tilde_03_x;
        number comb_tilde_03_delayTime;
        number comb_tilde_03_gain;
        number comb_tilde_03_ffGain;
        number comb_tilde_03_fbGain;
        number dspexpr_03_in1;
        number dspexpr_03_in2;
        number dspexpr_04_in1;
        number dspexpr_04_in2;
        number comb_tilde_04_x;
        number comb_tilde_04_delayTime;
        number comb_tilde_04_gain;
        number comb_tilde_04_ffGain;
        number comb_tilde_04_fbGain;
        number dspexpr_05_in1;
        number dspexpr_05_in2;
        number dspexpr_06_in1;
        number dspexpr_06_in2;
        number dspexpr_07_in1;
        number dspexpr_07_in2;
        number comb_tilde_05_x;
        number comb_tilde_05_delayTime;
        number comb_tilde_05_gain;
        number comb_tilde_05_ffGain;
        number comb_tilde_05_fbGain;
        number dspexpr_08_in1;
        number dspexpr_08_in2;
        number comb_tilde_06_x;
        number comb_tilde_06_delayTime;
        number comb_tilde_06_gain;
        number comb_tilde_06_ffGain;
        number comb_tilde_06_fbGain;
        number dspexpr_09_in1;
        number dspexpr_09_in2;
        number comb_tilde_07_x;
        number comb_tilde_07_delayTime;
        number comb_tilde_07_gain;
        number comb_tilde_07_ffGain;
        number comb_tilde_07_fbGain;
        number dspexpr_10_in1;
        number dspexpr_10_in2;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[8];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float64BufferRef comb_tilde_01_feedForward_buffer;
        Index comb_tilde_01_feedForward__maxdelay;
        Int comb_tilde_01_feedForward_sizemode;
        Index comb_tilde_01_feedForward_wrap;
        Int comb_tilde_01_feedForward_reader;
        Int comb_tilde_01_feedForward_writer;
        Float64BufferRef comb_tilde_01_feedBack_buffer;
        Index comb_tilde_01_feedBack__maxdelay;
        Int comb_tilde_01_feedBack_sizemode;
        Index comb_tilde_01_feedBack_wrap;
        Int comb_tilde_01_feedBack_reader;
        Int comb_tilde_01_feedBack_writer;
        bool comb_tilde_01_setupDone;
        Float64BufferRef comb_tilde_02_feedForward_buffer;
        Index comb_tilde_02_feedForward__maxdelay;
        Int comb_tilde_02_feedForward_sizemode;
        Index comb_tilde_02_feedForward_wrap;
        Int comb_tilde_02_feedForward_reader;
        Int comb_tilde_02_feedForward_writer;
        Float64BufferRef comb_tilde_02_feedBack_buffer;
        Index comb_tilde_02_feedBack__maxdelay;
        Int comb_tilde_02_feedBack_sizemode;
        Index comb_tilde_02_feedBack_wrap;
        Int comb_tilde_02_feedBack_reader;
        Int comb_tilde_02_feedBack_writer;
        bool comb_tilde_02_setupDone;
        Float64BufferRef comb_tilde_03_feedForward_buffer;
        Index comb_tilde_03_feedForward__maxdelay;
        Int comb_tilde_03_feedForward_sizemode;
        Index comb_tilde_03_feedForward_wrap;
        Int comb_tilde_03_feedForward_reader;
        Int comb_tilde_03_feedForward_writer;
        Float64BufferRef comb_tilde_03_feedBack_buffer;
        Index comb_tilde_03_feedBack__maxdelay;
        Int comb_tilde_03_feedBack_sizemode;
        Index comb_tilde_03_feedBack_wrap;
        Int comb_tilde_03_feedBack_reader;
        Int comb_tilde_03_feedBack_writer;
        bool comb_tilde_03_setupDone;
        Float64BufferRef comb_tilde_04_feedForward_buffer;
        Index comb_tilde_04_feedForward__maxdelay;
        Int comb_tilde_04_feedForward_sizemode;
        Index comb_tilde_04_feedForward_wrap;
        Int comb_tilde_04_feedForward_reader;
        Int comb_tilde_04_feedForward_writer;
        Float64BufferRef comb_tilde_04_feedBack_buffer;
        Index comb_tilde_04_feedBack__maxdelay;
        Int comb_tilde_04_feedBack_sizemode;
        Index comb_tilde_04_feedBack_wrap;
        Int comb_tilde_04_feedBack_reader;
        Int comb_tilde_04_feedBack_writer;
        bool comb_tilde_04_setupDone;
        Float64BufferRef comb_tilde_05_feedForward_buffer;
        Index comb_tilde_05_feedForward__maxdelay;
        Int comb_tilde_05_feedForward_sizemode;
        Index comb_tilde_05_feedForward_wrap;
        Int comb_tilde_05_feedForward_reader;
        Int comb_tilde_05_feedForward_writer;
        Float64BufferRef comb_tilde_05_feedBack_buffer;
        Index comb_tilde_05_feedBack__maxdelay;
        Int comb_tilde_05_feedBack_sizemode;
        Index comb_tilde_05_feedBack_wrap;
        Int comb_tilde_05_feedBack_reader;
        Int comb_tilde_05_feedBack_writer;
        bool comb_tilde_05_setupDone;
        Float64BufferRef comb_tilde_06_feedForward_buffer;
        Index comb_tilde_06_feedForward__maxdelay;
        Int comb_tilde_06_feedForward_sizemode;
        Index comb_tilde_06_feedForward_wrap;
        Int comb_tilde_06_feedForward_reader;
        Int comb_tilde_06_feedForward_writer;
        Float64BufferRef comb_tilde_06_feedBack_buffer;
        Index comb_tilde_06_feedBack__maxdelay;
        Int comb_tilde_06_feedBack_sizemode;
        Index comb_tilde_06_feedBack_wrap;
        Int comb_tilde_06_feedBack_reader;
        Int comb_tilde_06_feedBack_writer;
        bool comb_tilde_06_setupDone;
        Float64BufferRef comb_tilde_07_feedForward_buffer;
        Index comb_tilde_07_feedForward__maxdelay;
        Int comb_tilde_07_feedForward_sizemode;
        Index comb_tilde_07_feedForward_wrap;
        Int comb_tilde_07_feedForward_reader;
        Int comb_tilde_07_feedForward_writer;
        Float64BufferRef comb_tilde_07_feedBack_buffer;
        Index comb_tilde_07_feedBack__maxdelay;
        Int comb_tilde_07_feedBack_sizemode;
        Index comb_tilde_07_feedBack_wrap;
        Int comb_tilde_07_feedBack_reader;
        Int comb_tilde_07_feedBack_writer;
        bool comb_tilde_07_setupDone;
        number stackprotect_count;
        DataRef comb_tilde_01_feedForward_bufferobj;
        DataRef comb_tilde_01_feedBack_bufferobj;
        DataRef comb_tilde_02_feedForward_bufferobj;
        DataRef comb_tilde_02_feedBack_bufferobj;
        DataRef comb_tilde_03_feedForward_bufferobj;
        DataRef comb_tilde_03_feedBack_bufferobj;
        DataRef comb_tilde_04_feedForward_bufferobj;
        DataRef comb_tilde_04_feedBack_bufferobj;
        DataRef comb_tilde_05_feedForward_bufferobj;
        DataRef comb_tilde_05_feedBack_bufferobj;
        DataRef comb_tilde_06_feedForward_bufferobj;
        DataRef comb_tilde_06_feedBack_bufferobj;
        DataRef comb_tilde_07_feedForward_bufferobj;
        DataRef comb_tilde_07_feedBack_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_21 : public PatcherInterfaceImpl {
    
    friend class plate;
    
    public:
    
    RNBOSubpatcher_21()
    {
    }
    
    ~RNBOSubpatcher_21()
    {
    }
    
    virtual plate* getPatcher() const {
        return static_cast<plate *>(_parentPatcher);
    }
    
    plate* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
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
        const SampleValue * target_Size = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        const SampleValue * target_Decay = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        const SampleValue * target_Jitter = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        const SampleValue * target_Damp = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
        SampleValue * out3 = (numOutputs >= 3 && outputs[2] ? outputs[2] : this->dummyBuffer);
        SampleValue * out4 = (numOutputs >= 4 && outputs[3] ? outputs[3] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        this->noise_tilde_01_perform(this->signals[0], n);
    
        this->lores_01_perform(
            this->lores_01_cutoff,
            this->lores_01_resonance,
            this->signals[0],
            this->signals[1],
            n
        );
    
        this->feedbackreader_01_perform(this->signals[0], n);
        this->stackprotect_perform(n);
        this->signalreceive_03_perform(target_Size, this->signals[2], n);
        this->dspexpr_13_perform(this->signals[2], this->dspexpr_13_in2, this->signals[3], n);
        this->dspexpr_11_perform(this->signals[2], this->dspexpr_11_in2, this->signals[4], n);
        this->signaladder_03_perform(in1, this->signals[0], this->signals[0], n);
        this->allpass_tilde_09_perform(this->signals[0], this->signals[4], this->allpass_tilde_09_gain, out1, n);
        this->signalreceive_07_perform(target_Size, this->signals[4], n);
        this->dspexpr_17_perform(this->signals[4], this->dspexpr_17_in2, this->signals[2], n);
        this->mstosamps_tilde_01_perform(this->signals[2], this->signals[5], n);
        this->dspexpr_16_perform(this->signals[4], this->dspexpr_16_in2, this->signals[2], n);
        this->signalreceive_04_perform(target_Decay, this->signals[4], n);
        this->signalreceive_05_perform(target_Jitter, this->signals[6], n);
        this->dspexpr_15_perform(this->signals[1], this->signals[6], this->signals[7], n);
        this->dspexpr_12_perform(this->signals[3], this->signals[7], this->signals[6], n);
    
        this->comb_tilde_08_perform(
            out1,
            this->signals[6],
            this->comb_tilde_08_gain,
            this->comb_tilde_08_ffGain,
            this->comb_tilde_08_fbGain,
            this->signals[7],
            n
        );
    
        this->signalforwarder_15_perform(this->signals[7], out2, n);
        this->signalreceive_06_perform(target_Damp, this->signals[6], n);
        this->onepole_tilde_01_perform(this->signals[7], this->signals[6], this->signals[3], n);
        this->dspexpr_14_perform(this->signals[3], this->signals[4], this->signals[6], n);
    
        this->allpass_tilde_10_perform(
            this->signals[6],
            this->signals[2],
            this->allpass_tilde_10_gain,
            this->signals[4],
            n
        );
    
        this->delaytilde_01_perform(this->signals[5], this->signals[4], out4, n);
        this->feedbackwriter_01_perform(out4, n);
        this->signalforwarder_16_perform(this->signals[4], out3, n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 8; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->feedbacktilde_01_feedbackbuffer = resizeSignal(this->feedbacktilde_01_feedbackbuffer, this->maxvs, maxBlockSize);
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
    
        this->lores_01_dspsetup(forceDSPSetup);
        this->allpass_tilde_09_dspsetup(forceDSPSetup);
        this->comb_tilde_08_dspsetup(forceDSPSetup);
        this->onepole_tilde_01_dspsetup(forceDSPSetup);
        this->allpass_tilde_10_dspsetup(forceDSPSetup);
        this->delaytilde_01_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        default:
            {
            return 0;
            }
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 4;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
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
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 0:
            {
            return addressOf(this->allpass_tilde_09_feedForward_bufferobj);
            break;
            }
        case 1:
            {
            return addressOf(this->allpass_tilde_09_feedBack_bufferobj);
            break;
            }
        case 2:
            {
            return addressOf(this->comb_tilde_08_feedForward_bufferobj);
            break;
            }
        case 3:
            {
            return addressOf(this->comb_tilde_08_feedBack_bufferobj);
            break;
            }
        case 4:
            {
            return addressOf(this->allpass_tilde_10_feedForward_bufferobj);
            break;
            }
        case 5:
            {
            return addressOf(this->allpass_tilde_10_feedBack_bufferobj);
            break;
            }
        case 6:
            {
            return addressOf(this->delaytilde_01_del_bufferobj);
            break;
            }
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 7;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void zeroDataRef(DataRef& ref) {
        ref->setZero();
    }
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 0) {
            this->allpass_tilde_09_feedForward_buffer = new Float64Buffer(this->allpass_tilde_09_feedForward_bufferobj);
        }
    
        if (index == 1) {
            this->allpass_tilde_09_feedBack_buffer = new Float64Buffer(this->allpass_tilde_09_feedBack_bufferobj);
        }
    
        if (index == 2) {
            this->comb_tilde_08_feedForward_buffer = new Float64Buffer(this->comb_tilde_08_feedForward_bufferobj);
        }
    
        if (index == 3) {
            this->comb_tilde_08_feedBack_buffer = new Float64Buffer(this->comb_tilde_08_feedBack_bufferobj);
        }
    
        if (index == 4) {
            this->allpass_tilde_10_feedForward_buffer = new Float64Buffer(this->allpass_tilde_10_feedForward_bufferobj);
        }
    
        if (index == 5) {
            this->allpass_tilde_10_feedBack_buffer = new Float64Buffer(this->allpass_tilde_10_feedBack_bufferobj);
        }
    
        if (index == 6) {
            this->delaytilde_01_del_buffer = new Float64Buffer(this->delaytilde_01_del_bufferobj);
        }
    }
    
    void initialize() {
        this->allpass_tilde_09_feedForward_bufferobj = initDataRef("allpass_tilde_09_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_09_feedBack_bufferobj = initDataRef("allpass_tilde_09_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_08_feedForward_bufferobj = initDataRef("comb_tilde_08_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_08_feedBack_bufferobj = initDataRef("comb_tilde_08_feedBack_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_10_feedForward_bufferobj = initDataRef("allpass_tilde_10_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_10_feedBack_bufferobj = initDataRef("allpass_tilde_10_feedBack_bufferobj", true, nullptr, "buffer~");
        this->delaytilde_01_del_bufferobj = initDataRef("delaytilde_01_del_bufferobj", true, nullptr, "buffer~");
        this->assign_defaults();
        this->setState();
        this->allpass_tilde_09_feedForward_bufferobj->setIndex(0);
        this->allpass_tilde_09_feedForward_buffer = new Float64Buffer(this->allpass_tilde_09_feedForward_bufferobj);
        this->allpass_tilde_09_feedBack_bufferobj->setIndex(1);
        this->allpass_tilde_09_feedBack_buffer = new Float64Buffer(this->allpass_tilde_09_feedBack_bufferobj);
        this->comb_tilde_08_feedForward_bufferobj->setIndex(2);
        this->comb_tilde_08_feedForward_buffer = new Float64Buffer(this->comb_tilde_08_feedForward_bufferobj);
        this->comb_tilde_08_feedBack_bufferobj->setIndex(3);
        this->comb_tilde_08_feedBack_buffer = new Float64Buffer(this->comb_tilde_08_feedBack_bufferobj);
        this->allpass_tilde_10_feedForward_bufferobj->setIndex(4);
        this->allpass_tilde_10_feedForward_buffer = new Float64Buffer(this->allpass_tilde_10_feedForward_bufferobj);
        this->allpass_tilde_10_feedBack_bufferobj->setIndex(5);
        this->allpass_tilde_10_feedBack_buffer = new Float64Buffer(this->allpass_tilde_10_feedBack_bufferobj);
        this->delaytilde_01_del_bufferobj->setIndex(6);
        this->delaytilde_01_del_buffer = new Float64Buffer(this->delaytilde_01_del_bufferobj);
    }
    
    protected:
    
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
        return 1;
    }
    
    Index getNumOutputChannels() const {
        return 4;
    }
    
    void initializeObjects() {
        this->allpass_tilde_09_feedForward_init();
        this->allpass_tilde_09_feedBack_init();
        this->allpass_tilde_09_init();
        this->comb_tilde_08_feedForward_init();
        this->comb_tilde_08_feedBack_init();
        this->comb_tilde_08_init();
        this->allpass_tilde_10_feedForward_init();
        this->allpass_tilde_10_feedBack_init();
        this->allpass_tilde_10_init();
        this->noise_tilde_01_init();
        this->delaytilde_01_del_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->allpass_tilde_09_feedForward_buffer = this->allpass_tilde_09_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_09_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_09_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_09_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(0);
        }
    
        this->allpass_tilde_09_feedBack_buffer = this->allpass_tilde_09_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_09_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_09_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_09_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    
        this->comb_tilde_08_feedForward_buffer = this->comb_tilde_08_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_08_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_08_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_08_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(2);
        }
    
        this->comb_tilde_08_feedBack_buffer = this->comb_tilde_08_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_08_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_08_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_08_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(3);
        }
    
        this->allpass_tilde_10_feedForward_buffer = this->allpass_tilde_10_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_10_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_10_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_10_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(4);
        }
    
        this->allpass_tilde_10_feedBack_buffer = this->allpass_tilde_10_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_10_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_10_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_10_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(5);
        }
    
        this->delaytilde_01_del_buffer = this->delaytilde_01_del_buffer->allocateIfNeeded();
    
        if (this->delaytilde_01_del_bufferobj->hasRequestedSize()) {
            if (this->delaytilde_01_del_bufferobj->wantsFill())
                this->zeroDataRef(this->delaytilde_01_del_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(6);
        }
    }
    
    void noise_tilde_01_perform(SampleValue * out1, Index n) {
        auto __noise_tilde_01_state = this->noise_tilde_01_state;
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = xoshiro_next(__noise_tilde_01_state);
        }
    }
    
    void lores_01_perform(
        number cutoff,
        number resonance,
        const SampleValue * input,
        SampleValue * out,
        Index n
    ) {
        RNBO_UNUSED(resonance);
        RNBO_UNUSED(cutoff);
        auto __lores_01_y_2 = this->lores_01_y_2;
        auto __lores_01_y_1 = this->lores_01_y_1;
        auto __lores_01_x_0 = this->lores_01_x_0;
        auto __lores_01_b_0 = this->lores_01_b_0;
        auto __lores_01_a_1 = this->lores_01_a_1;
        auto __lores_01_a_2 = this->lores_01_a_2;
        auto __lores_01_last_freq_calc = this->lores_01_last_freq_calc;
        auto __lores_01_last_res_calc = this->lores_01_last_res_calc;
        auto __lores_01_last_frequency = this->lores_01_last_frequency;
        auto __lores_01_last_resonance = this->lores_01_last_resonance;
    
        for (Index i = 0; i < n; i++) {
            if (0.995 != __lores_01_last_resonance || 0 != __lores_01_last_frequency) {
                __lores_01_last_res_calc = rnbo_exp(0.995 * 0.125) * 0.882497;
                __lores_01_last_freq_calc = rnbo_cos(2.0 * pi01() / this->sr * 0);
                __lores_01_last_resonance = 0.995;
                __lores_01_last_frequency = 0;
            }
    
            number res_temp = __lores_01_last_res_calc;
            number freq_temp = __lores_01_last_freq_calc;
            __lores_01_a_2 = res_temp * res_temp;
            __lores_01_a_1 = -2.0 * res_temp * freq_temp;
            __lores_01_b_0 = 1.0 + __lores_01_a_1 + __lores_01_a_2;
            __lores_01_x_0 = input[(Index)i];
            number y_0 = __lores_01_x_0 * __lores_01_b_0 - __lores_01_y_1 * __lores_01_a_1 - __lores_01_y_2 * __lores_01_a_2;
            __lores_01_y_2 = __lores_01_y_1;
            __lores_01_y_1 = y_0;
            out[(Index)i] = y_0;
        }
    
        this->lores_01_last_resonance = __lores_01_last_resonance;
        this->lores_01_last_frequency = __lores_01_last_frequency;
        this->lores_01_last_res_calc = __lores_01_last_res_calc;
        this->lores_01_last_freq_calc = __lores_01_last_freq_calc;
        this->lores_01_a_2 = __lores_01_a_2;
        this->lores_01_a_1 = __lores_01_a_1;
        this->lores_01_b_0 = __lores_01_b_0;
        this->lores_01_x_0 = __lores_01_x_0;
        this->lores_01_y_1 = __lores_01_y_1;
        this->lores_01_y_2 = __lores_01_y_2;
    }
    
    void feedbackreader_01_perform(SampleValue * output, Index n) {
        auto& buffer = this->feedbacktilde_01_feedbackbuffer;
    
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = buffer[(Index)i];
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void signalreceive_03_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void dspexpr_13_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 149.6;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_11_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 22.58;//#map:_###_obj_###_:1
        }
    }
    
    void signaladder_03_perform(
        const SampleValue * in1,
        const SampleValue * in2,
        SampleValue * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out[(Index)i] = in1[(Index)i] + in2[(Index)i];
        }
    }
    
    void allpass_tilde_09_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = -0.7;
            auto delayTimeinSamps = this->mstosamps(delayTime[(Index)i]);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_09_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_09_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_09_feedForward_write(x[(Index)i]);
            this->allpass_tilde_09_feedBack_write(ov);
            this->allpass_tilde_09_feedForward_step();
            this->allpass_tilde_09_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void signalreceive_07_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void dspexpr_17_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 125;//#map:_###_obj_###_:1
        }
    }
    
    void mstosamps_tilde_01_perform(const Sample * ms, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = ms[(Index)i] * this->sr * 0.001;
        }
    }
    
    void dspexpr_16_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 60.48;//#map:_###_obj_###_:1
        }
    }
    
    void signalreceive_04_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void signalreceive_05_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void dspexpr_15_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_12_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_08_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_08_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_08_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_08_feedForward_write(x[(Index)i]);
            this->comb_tilde_08_feedBack_write(ov);
            this->comb_tilde_08_feedForward_step();
            this->comb_tilde_08_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void signalforwarder_15_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalreceive_06_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void onepole_tilde_01_perform(const Sample * x, const Sample * freqInHz, SampleValue * out1, Index n) {
        auto __onepole_tilde_01_lastY = this->onepole_tilde_01_lastY;
        auto __onepole_tilde_01_b1 = this->onepole_tilde_01_b1;
        auto __onepole_tilde_01_a0 = this->onepole_tilde_01_a0;
        auto __onepole_tilde_01_needsUpdate = this->onepole_tilde_01_needsUpdate;
        auto __onepole_tilde_01_freq = this->onepole_tilde_01_freq;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (__onepole_tilde_01_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_01_needsUpdate)) {
                __onepole_tilde_01_freq = freqInHz[(Index)i];
                __onepole_tilde_01_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_01_a0 = (__onepole_tilde_01_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_01_a0 < 0.00001 ? 0.00001 : __onepole_tilde_01_a0));
                __onepole_tilde_01_b1 = 1 - __onepole_tilde_01_a0;
                __onepole_tilde_01_needsUpdate = false;
            }
    
            __onepole_tilde_01_lastY = __onepole_tilde_01_a0 * x[(Index)i] + __onepole_tilde_01_b1 * __onepole_tilde_01_lastY;
            out1[(Index)i] = __onepole_tilde_01_lastY;
        }
    
        this->onepole_tilde_01_freq = __onepole_tilde_01_freq;
        this->onepole_tilde_01_needsUpdate = __onepole_tilde_01_needsUpdate;
        this->onepole_tilde_01_a0 = __onepole_tilde_01_a0;
        this->onepole_tilde_01_b1 = __onepole_tilde_01_b1;
        this->onepole_tilde_01_lastY = __onepole_tilde_01_lastY;
    }
    
    void dspexpr_14_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void allpass_tilde_10_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = 0.5;
            auto delayTimeinSamps = this->mstosamps(delayTime[(Index)i]);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_10_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_10_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_10_feedForward_write(x[(Index)i]);
            this->allpass_tilde_10_feedBack_write(ov);
            this->allpass_tilde_10_feedForward_step();
            this->allpass_tilde_10_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void delaytilde_01_perform(
        const Sample * delay,
        const SampleValue * input,
        SampleValue * output,
        Index n
    ) {
        auto __delaytilde_01_crossfadeDelay = this->delaytilde_01_crossfadeDelay;
        auto __delaytilde_01_rampInSamples = this->delaytilde_01_rampInSamples;
        auto __delaytilde_01_ramp = this->delaytilde_01_ramp;
        auto __delaytilde_01_lastDelay = this->delaytilde_01_lastDelay;
    
        for (Index i = 0; i < n; i++) {
            if (__delaytilde_01_lastDelay == -1) {
                __delaytilde_01_lastDelay = delay[(Index)i];
            }
    
            if (__delaytilde_01_ramp > 0) {
                number factor = __delaytilde_01_ramp / __delaytilde_01_rampInSamples;
                output[(Index)i] = this->delaytilde_01_del_read(__delaytilde_01_crossfadeDelay, 0) * factor + this->delaytilde_01_del_read(__delaytilde_01_lastDelay, 0) * (1. - factor);
                __delaytilde_01_ramp--;
            } else {
                number effectiveDelay = delay[(Index)i];
    
                if (effectiveDelay != __delaytilde_01_lastDelay) {
                    __delaytilde_01_ramp = __delaytilde_01_rampInSamples;
                    __delaytilde_01_crossfadeDelay = __delaytilde_01_lastDelay;
                    __delaytilde_01_lastDelay = effectiveDelay;
                    output[(Index)i] = this->delaytilde_01_del_read(__delaytilde_01_crossfadeDelay, 0);
                    __delaytilde_01_ramp--;
                } else {
                    output[(Index)i] = this->delaytilde_01_del_read(effectiveDelay, 0);
                }
            }
    
            this->delaytilde_01_del_write(input[(Index)i]);
            this->delaytilde_01_del_step();
        }
    
        this->delaytilde_01_lastDelay = __delaytilde_01_lastDelay;
        this->delaytilde_01_ramp = __delaytilde_01_ramp;
        this->delaytilde_01_crossfadeDelay = __delaytilde_01_crossfadeDelay;
    }
    
    void feedbackwriter_01_perform(const SampleValue * input, Index n) {
        auto& buffer = this->feedbacktilde_01_feedbackbuffer;
    
        for (Index i = 0; i < n; i++) {
            buffer[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_16_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void lores_01_resonance_setter(number v) {
        if (v < 0.0) {
            v = 0.0;
        } else if (v >= 1.0) {
            v = 1.0 - 1E-20;
        }
    
        this->lores_01_resonance = v;
    }
    
    void allpass_tilde_09_feedForward_step() {
        this->allpass_tilde_09_feedForward_reader++;
    
        if (this->allpass_tilde_09_feedForward_reader >= (int)(this->allpass_tilde_09_feedForward_buffer->getSize()))
            this->allpass_tilde_09_feedForward_reader = 0;
    }
    
    number allpass_tilde_09_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_09_feedForward_buffer->getSize()) + this->allpass_tilde_09_feedForward_reader - ((size > this->allpass_tilde_09_feedForward__maxdelay ? this->allpass_tilde_09_feedForward__maxdelay : (size < (this->allpass_tilde_09_feedForward_reader != this->allpass_tilde_09_feedForward_writer) ? this->allpass_tilde_09_feedForward_reader != this->allpass_tilde_09_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ), this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_09_feedForward_buffer->getSize()) + this->allpass_tilde_09_feedForward_reader - ((size > this->allpass_tilde_09_feedForward__maxdelay ? this->allpass_tilde_09_feedForward__maxdelay : (size < (1 + this->allpass_tilde_09_feedForward_reader != this->allpass_tilde_09_feedForward_writer) ? 1 + this->allpass_tilde_09_feedForward_reader != this->allpass_tilde_09_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ), this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ), this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ), this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_09_feedForward_buffer->getSize()) + this->allpass_tilde_09_feedForward_reader - ((size > this->allpass_tilde_09_feedForward__maxdelay ? this->allpass_tilde_09_feedForward__maxdelay : (size < (1 + this->allpass_tilde_09_feedForward_reader != this->allpass_tilde_09_feedForward_writer) ? 1 + this->allpass_tilde_09_feedForward_reader != this->allpass_tilde_09_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ), this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ), this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ), this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_09_feedForward_buffer->getSize()) + this->allpass_tilde_09_feedForward_reader - ((size > this->allpass_tilde_09_feedForward__maxdelay ? this->allpass_tilde_09_feedForward__maxdelay : (size < (this->allpass_tilde_09_feedForward_reader != this->allpass_tilde_09_feedForward_writer) ? this->allpass_tilde_09_feedForward_reader != this->allpass_tilde_09_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ), this->allpass_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_09_feedForward_buffer->getSize()) + this->allpass_tilde_09_feedForward_reader - ((size > this->allpass_tilde_09_feedForward__maxdelay ? this->allpass_tilde_09_feedForward__maxdelay : (size < (this->allpass_tilde_09_feedForward_reader != this->allpass_tilde_09_feedForward_writer) ? this->allpass_tilde_09_feedForward_reader != this->allpass_tilde_09_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_09_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_09_feedForward_wrap))
        );
    }
    
    void allpass_tilde_09_feedForward_write(number v) {
        this->allpass_tilde_09_feedForward_writer = this->allpass_tilde_09_feedForward_reader;
        this->allpass_tilde_09_feedForward_buffer[(Index)this->allpass_tilde_09_feedForward_writer] = v;
    }
    
    number allpass_tilde_09_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_09_feedForward__maxdelay : size);
        number val = this->allpass_tilde_09_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_09_feedForward_write(v);
        this->allpass_tilde_09_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_09_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_09_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_09_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_09_feedForward_init() {
        auto result = this->allpass_tilde_09_feedForward_calcSizeInSamples();
        this->allpass_tilde_09_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_09_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_09_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_09_feedForward_clear() {
        this->allpass_tilde_09_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_09_feedForward_reset() {
        auto result = this->allpass_tilde_09_feedForward_calcSizeInSamples();
        this->allpass_tilde_09_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_09_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_09_feedForward_buffer);
        this->allpass_tilde_09_feedForward_wrap = this->allpass_tilde_09_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_09_feedForward_clear();
    
        if (this->allpass_tilde_09_feedForward_reader >= this->allpass_tilde_09_feedForward__maxdelay || this->allpass_tilde_09_feedForward_writer >= this->allpass_tilde_09_feedForward__maxdelay) {
            this->allpass_tilde_09_feedForward_reader = 0;
            this->allpass_tilde_09_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_09_feedForward_dspsetup() {
        this->allpass_tilde_09_feedForward_reset();
    }
    
    number allpass_tilde_09_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_09_feedForward_size() {
        return this->allpass_tilde_09_feedForward__maxdelay;
    }
    
    void allpass_tilde_09_feedBack_step() {
        this->allpass_tilde_09_feedBack_reader++;
    
        if (this->allpass_tilde_09_feedBack_reader >= (int)(this->allpass_tilde_09_feedBack_buffer->getSize()))
            this->allpass_tilde_09_feedBack_reader = 0;
    }
    
    number allpass_tilde_09_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_09_feedBack_buffer->getSize()) + this->allpass_tilde_09_feedBack_reader - ((size > this->allpass_tilde_09_feedBack__maxdelay ? this->allpass_tilde_09_feedBack__maxdelay : (size < (this->allpass_tilde_09_feedBack_reader != this->allpass_tilde_09_feedBack_writer) ? this->allpass_tilde_09_feedBack_reader != this->allpass_tilde_09_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ), this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_09_feedBack_buffer->getSize()) + this->allpass_tilde_09_feedBack_reader - ((size > this->allpass_tilde_09_feedBack__maxdelay ? this->allpass_tilde_09_feedBack__maxdelay : (size < (1 + this->allpass_tilde_09_feedBack_reader != this->allpass_tilde_09_feedBack_writer) ? 1 + this->allpass_tilde_09_feedBack_reader != this->allpass_tilde_09_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ), this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ), this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ), this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_09_feedBack_buffer->getSize()) + this->allpass_tilde_09_feedBack_reader - ((size > this->allpass_tilde_09_feedBack__maxdelay ? this->allpass_tilde_09_feedBack__maxdelay : (size < (1 + this->allpass_tilde_09_feedBack_reader != this->allpass_tilde_09_feedBack_writer) ? 1 + this->allpass_tilde_09_feedBack_reader != this->allpass_tilde_09_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ), this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ), this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ), this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_09_feedBack_buffer->getSize()) + this->allpass_tilde_09_feedBack_reader - ((size > this->allpass_tilde_09_feedBack__maxdelay ? this->allpass_tilde_09_feedBack__maxdelay : (size < (this->allpass_tilde_09_feedBack_reader != this->allpass_tilde_09_feedBack_writer) ? this->allpass_tilde_09_feedBack_reader != this->allpass_tilde_09_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ), this->allpass_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_09_feedBack_buffer->getSize()) + this->allpass_tilde_09_feedBack_reader - ((size > this->allpass_tilde_09_feedBack__maxdelay ? this->allpass_tilde_09_feedBack__maxdelay : (size < (this->allpass_tilde_09_feedBack_reader != this->allpass_tilde_09_feedBack_writer) ? this->allpass_tilde_09_feedBack_reader != this->allpass_tilde_09_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_09_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_09_feedBack_wrap))
        );
    }
    
    void allpass_tilde_09_feedBack_write(number v) {
        this->allpass_tilde_09_feedBack_writer = this->allpass_tilde_09_feedBack_reader;
        this->allpass_tilde_09_feedBack_buffer[(Index)this->allpass_tilde_09_feedBack_writer] = v;
    }
    
    number allpass_tilde_09_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_09_feedBack__maxdelay : size);
        number val = this->allpass_tilde_09_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_09_feedBack_write(v);
        this->allpass_tilde_09_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_09_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_09_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_09_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_09_feedBack_init() {
        auto result = this->allpass_tilde_09_feedBack_calcSizeInSamples();
        this->allpass_tilde_09_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_09_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_09_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_09_feedBack_clear() {
        this->allpass_tilde_09_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_09_feedBack_reset() {
        auto result = this->allpass_tilde_09_feedBack_calcSizeInSamples();
        this->allpass_tilde_09_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_09_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_09_feedBack_buffer);
        this->allpass_tilde_09_feedBack_wrap = this->allpass_tilde_09_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_09_feedBack_clear();
    
        if (this->allpass_tilde_09_feedBack_reader >= this->allpass_tilde_09_feedBack__maxdelay || this->allpass_tilde_09_feedBack_writer >= this->allpass_tilde_09_feedBack__maxdelay) {
            this->allpass_tilde_09_feedBack_reader = 0;
            this->allpass_tilde_09_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_09_feedBack_dspsetup() {
        this->allpass_tilde_09_feedBack_reset();
    }
    
    number allpass_tilde_09_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_09_feedBack_size() {
        return this->allpass_tilde_09_feedBack__maxdelay;
    }
    
    void allpass_tilde_09_init() {
        this->allpass_tilde_09_feedForward_init();
        this->allpass_tilde_09_feedBack_init();
    }
    
    void allpass_tilde_09_clear() {
        this->allpass_tilde_09_feedForward_clear();
        this->allpass_tilde_09_feedBack_clear();
    }
    
    void allpass_tilde_09_reset() {
        this->allpass_tilde_09_init();
        this->allpass_tilde_09_clear();
    }
    
    void allpass_tilde_09_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_09_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_09_reset();
        this->allpass_tilde_09_setupDone = true;
        this->allpass_tilde_09_feedForward_dspsetup();
        this->allpass_tilde_09_feedBack_dspsetup();
    }
    
    void comb_tilde_08_feedForward_step() {
        this->comb_tilde_08_feedForward_reader++;
    
        if (this->comb_tilde_08_feedForward_reader >= (int)(this->comb_tilde_08_feedForward_buffer->getSize()))
            this->comb_tilde_08_feedForward_reader = 0;
    }
    
    number comb_tilde_08_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_08_feedForward_buffer->getSize()) + this->comb_tilde_08_feedForward_reader - ((size > this->comb_tilde_08_feedForward__maxdelay ? this->comb_tilde_08_feedForward__maxdelay : (size < (this->comb_tilde_08_feedForward_reader != this->comb_tilde_08_feedForward_writer) ? this->comb_tilde_08_feedForward_reader != this->comb_tilde_08_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ), this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_08_feedForward_buffer->getSize()) + this->comb_tilde_08_feedForward_reader - ((size > this->comb_tilde_08_feedForward__maxdelay ? this->comb_tilde_08_feedForward__maxdelay : (size < (1 + this->comb_tilde_08_feedForward_reader != this->comb_tilde_08_feedForward_writer) ? 1 + this->comb_tilde_08_feedForward_reader != this->comb_tilde_08_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ), this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ), this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ), this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_08_feedForward_buffer->getSize()) + this->comb_tilde_08_feedForward_reader - ((size > this->comb_tilde_08_feedForward__maxdelay ? this->comb_tilde_08_feedForward__maxdelay : (size < (1 + this->comb_tilde_08_feedForward_reader != this->comb_tilde_08_feedForward_writer) ? 1 + this->comb_tilde_08_feedForward_reader != this->comb_tilde_08_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ), this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ), this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ), this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_08_feedForward_buffer->getSize()) + this->comb_tilde_08_feedForward_reader - ((size > this->comb_tilde_08_feedForward__maxdelay ? this->comb_tilde_08_feedForward__maxdelay : (size < (this->comb_tilde_08_feedForward_reader != this->comb_tilde_08_feedForward_writer) ? this->comb_tilde_08_feedForward_reader != this->comb_tilde_08_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ), this->comb_tilde_08_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_08_feedForward_buffer->getSize()) + this->comb_tilde_08_feedForward_reader - ((size > this->comb_tilde_08_feedForward__maxdelay ? this->comb_tilde_08_feedForward__maxdelay : (size < (this->comb_tilde_08_feedForward_reader != this->comb_tilde_08_feedForward_writer) ? this->comb_tilde_08_feedForward_reader != this->comb_tilde_08_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_08_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_08_feedForward_wrap))
        );
    }
    
    void comb_tilde_08_feedForward_write(number v) {
        this->comb_tilde_08_feedForward_writer = this->comb_tilde_08_feedForward_reader;
        this->comb_tilde_08_feedForward_buffer[(Index)this->comb_tilde_08_feedForward_writer] = v;
    }
    
    number comb_tilde_08_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_08_feedForward__maxdelay : size);
        number val = this->comb_tilde_08_feedForward_read(effectiveSize, 0);
        this->comb_tilde_08_feedForward_write(v);
        this->comb_tilde_08_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_08_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_08_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_08_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_08_feedForward_init() {
        auto result = this->comb_tilde_08_feedForward_calcSizeInSamples();
        this->comb_tilde_08_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_08_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_08_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_08_feedForward_clear() {
        this->comb_tilde_08_feedForward_buffer->setZero();
    }
    
    void comb_tilde_08_feedForward_reset() {
        auto result = this->comb_tilde_08_feedForward_calcSizeInSamples();
        this->comb_tilde_08_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_08_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_08_feedForward_buffer);
        this->comb_tilde_08_feedForward_wrap = this->comb_tilde_08_feedForward_buffer->getSize() - 1;
        this->comb_tilde_08_feedForward_clear();
    
        if (this->comb_tilde_08_feedForward_reader >= this->comb_tilde_08_feedForward__maxdelay || this->comb_tilde_08_feedForward_writer >= this->comb_tilde_08_feedForward__maxdelay) {
            this->comb_tilde_08_feedForward_reader = 0;
            this->comb_tilde_08_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_08_feedForward_dspsetup() {
        this->comb_tilde_08_feedForward_reset();
    }
    
    number comb_tilde_08_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_08_feedForward_size() {
        return this->comb_tilde_08_feedForward__maxdelay;
    }
    
    void comb_tilde_08_feedBack_step() {
        this->comb_tilde_08_feedBack_reader++;
    
        if (this->comb_tilde_08_feedBack_reader >= (int)(this->comb_tilde_08_feedBack_buffer->getSize()))
            this->comb_tilde_08_feedBack_reader = 0;
    }
    
    number comb_tilde_08_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_08_feedBack_buffer->getSize()) + this->comb_tilde_08_feedBack_reader - ((size > this->comb_tilde_08_feedBack__maxdelay ? this->comb_tilde_08_feedBack__maxdelay : (size < (this->comb_tilde_08_feedBack_reader != this->comb_tilde_08_feedBack_writer) ? this->comb_tilde_08_feedBack_reader != this->comb_tilde_08_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ), this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_08_feedBack_buffer->getSize()) + this->comb_tilde_08_feedBack_reader - ((size > this->comb_tilde_08_feedBack__maxdelay ? this->comb_tilde_08_feedBack__maxdelay : (size < (1 + this->comb_tilde_08_feedBack_reader != this->comb_tilde_08_feedBack_writer) ? 1 + this->comb_tilde_08_feedBack_reader != this->comb_tilde_08_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ), this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ), this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ), this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_08_feedBack_buffer->getSize()) + this->comb_tilde_08_feedBack_reader - ((size > this->comb_tilde_08_feedBack__maxdelay ? this->comb_tilde_08_feedBack__maxdelay : (size < (1 + this->comb_tilde_08_feedBack_reader != this->comb_tilde_08_feedBack_writer) ? 1 + this->comb_tilde_08_feedBack_reader != this->comb_tilde_08_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ), this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ), this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ), this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_08_feedBack_buffer->getSize()) + this->comb_tilde_08_feedBack_reader - ((size > this->comb_tilde_08_feedBack__maxdelay ? this->comb_tilde_08_feedBack__maxdelay : (size < (this->comb_tilde_08_feedBack_reader != this->comb_tilde_08_feedBack_writer) ? this->comb_tilde_08_feedBack_reader != this->comb_tilde_08_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ), this->comb_tilde_08_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_08_feedBack_buffer->getSize()) + this->comb_tilde_08_feedBack_reader - ((size > this->comb_tilde_08_feedBack__maxdelay ? this->comb_tilde_08_feedBack__maxdelay : (size < (this->comb_tilde_08_feedBack_reader != this->comb_tilde_08_feedBack_writer) ? this->comb_tilde_08_feedBack_reader != this->comb_tilde_08_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_08_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_08_feedBack_wrap))
        );
    }
    
    void comb_tilde_08_feedBack_write(number v) {
        this->comb_tilde_08_feedBack_writer = this->comb_tilde_08_feedBack_reader;
        this->comb_tilde_08_feedBack_buffer[(Index)this->comb_tilde_08_feedBack_writer] = v;
    }
    
    number comb_tilde_08_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_08_feedBack__maxdelay : size);
        number val = this->comb_tilde_08_feedBack_read(effectiveSize, 0);
        this->comb_tilde_08_feedBack_write(v);
        this->comb_tilde_08_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_08_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_08_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_08_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_08_feedBack_init() {
        auto result = this->comb_tilde_08_feedBack_calcSizeInSamples();
        this->comb_tilde_08_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_08_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_08_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_08_feedBack_clear() {
        this->comb_tilde_08_feedBack_buffer->setZero();
    }
    
    void comb_tilde_08_feedBack_reset() {
        auto result = this->comb_tilde_08_feedBack_calcSizeInSamples();
        this->comb_tilde_08_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_08_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_08_feedBack_buffer);
        this->comb_tilde_08_feedBack_wrap = this->comb_tilde_08_feedBack_buffer->getSize() - 1;
        this->comb_tilde_08_feedBack_clear();
    
        if (this->comb_tilde_08_feedBack_reader >= this->comb_tilde_08_feedBack__maxdelay || this->comb_tilde_08_feedBack_writer >= this->comb_tilde_08_feedBack__maxdelay) {
            this->comb_tilde_08_feedBack_reader = 0;
            this->comb_tilde_08_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_08_feedBack_dspsetup() {
        this->comb_tilde_08_feedBack_reset();
    }
    
    number comb_tilde_08_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_08_feedBack_size() {
        return this->comb_tilde_08_feedBack__maxdelay;
    }
    
    void comb_tilde_08_init() {
        this->comb_tilde_08_feedForward_init();
        this->comb_tilde_08_feedBack_init();
    }
    
    void comb_tilde_08_clear() {
        this->comb_tilde_08_feedForward_clear();
        this->comb_tilde_08_feedBack_clear();
    }
    
    void comb_tilde_08_reset() {
        this->comb_tilde_08_init();
        this->comb_tilde_08_clear();
    }
    
    void comb_tilde_08_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_08_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_08_reset();
        this->comb_tilde_08_setupDone = true;
        this->comb_tilde_08_feedForward_dspsetup();
        this->comb_tilde_08_feedBack_dspsetup();
    }
    
    void allpass_tilde_10_feedForward_step() {
        this->allpass_tilde_10_feedForward_reader++;
    
        if (this->allpass_tilde_10_feedForward_reader >= (int)(this->allpass_tilde_10_feedForward_buffer->getSize()))
            this->allpass_tilde_10_feedForward_reader = 0;
    }
    
    number allpass_tilde_10_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_10_feedForward_buffer->getSize()) + this->allpass_tilde_10_feedForward_reader - ((size > this->allpass_tilde_10_feedForward__maxdelay ? this->allpass_tilde_10_feedForward__maxdelay : (size < (this->allpass_tilde_10_feedForward_reader != this->allpass_tilde_10_feedForward_writer) ? this->allpass_tilde_10_feedForward_reader != this->allpass_tilde_10_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ), this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_10_feedForward_buffer->getSize()) + this->allpass_tilde_10_feedForward_reader - ((size > this->allpass_tilde_10_feedForward__maxdelay ? this->allpass_tilde_10_feedForward__maxdelay : (size < (1 + this->allpass_tilde_10_feedForward_reader != this->allpass_tilde_10_feedForward_writer) ? 1 + this->allpass_tilde_10_feedForward_reader != this->allpass_tilde_10_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ), this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ), this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ), this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_10_feedForward_buffer->getSize()) + this->allpass_tilde_10_feedForward_reader - ((size > this->allpass_tilde_10_feedForward__maxdelay ? this->allpass_tilde_10_feedForward__maxdelay : (size < (1 + this->allpass_tilde_10_feedForward_reader != this->allpass_tilde_10_feedForward_writer) ? 1 + this->allpass_tilde_10_feedForward_reader != this->allpass_tilde_10_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ), this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ), this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ), this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_10_feedForward_buffer->getSize()) + this->allpass_tilde_10_feedForward_reader - ((size > this->allpass_tilde_10_feedForward__maxdelay ? this->allpass_tilde_10_feedForward__maxdelay : (size < (this->allpass_tilde_10_feedForward_reader != this->allpass_tilde_10_feedForward_writer) ? this->allpass_tilde_10_feedForward_reader != this->allpass_tilde_10_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ), this->allpass_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_10_feedForward_buffer->getSize()) + this->allpass_tilde_10_feedForward_reader - ((size > this->allpass_tilde_10_feedForward__maxdelay ? this->allpass_tilde_10_feedForward__maxdelay : (size < (this->allpass_tilde_10_feedForward_reader != this->allpass_tilde_10_feedForward_writer) ? this->allpass_tilde_10_feedForward_reader != this->allpass_tilde_10_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_10_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_10_feedForward_wrap))
        );
    }
    
    void allpass_tilde_10_feedForward_write(number v) {
        this->allpass_tilde_10_feedForward_writer = this->allpass_tilde_10_feedForward_reader;
        this->allpass_tilde_10_feedForward_buffer[(Index)this->allpass_tilde_10_feedForward_writer] = v;
    }
    
    number allpass_tilde_10_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_10_feedForward__maxdelay : size);
        number val = this->allpass_tilde_10_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_10_feedForward_write(v);
        this->allpass_tilde_10_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_10_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_10_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_10_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_10_feedForward_init() {
        auto result = this->allpass_tilde_10_feedForward_calcSizeInSamples();
        this->allpass_tilde_10_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_10_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_10_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_10_feedForward_clear() {
        this->allpass_tilde_10_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_10_feedForward_reset() {
        auto result = this->allpass_tilde_10_feedForward_calcSizeInSamples();
        this->allpass_tilde_10_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_10_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_10_feedForward_buffer);
        this->allpass_tilde_10_feedForward_wrap = this->allpass_tilde_10_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_10_feedForward_clear();
    
        if (this->allpass_tilde_10_feedForward_reader >= this->allpass_tilde_10_feedForward__maxdelay || this->allpass_tilde_10_feedForward_writer >= this->allpass_tilde_10_feedForward__maxdelay) {
            this->allpass_tilde_10_feedForward_reader = 0;
            this->allpass_tilde_10_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_10_feedForward_dspsetup() {
        this->allpass_tilde_10_feedForward_reset();
    }
    
    number allpass_tilde_10_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_10_feedForward_size() {
        return this->allpass_tilde_10_feedForward__maxdelay;
    }
    
    void allpass_tilde_10_feedBack_step() {
        this->allpass_tilde_10_feedBack_reader++;
    
        if (this->allpass_tilde_10_feedBack_reader >= (int)(this->allpass_tilde_10_feedBack_buffer->getSize()))
            this->allpass_tilde_10_feedBack_reader = 0;
    }
    
    number allpass_tilde_10_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_10_feedBack_buffer->getSize()) + this->allpass_tilde_10_feedBack_reader - ((size > this->allpass_tilde_10_feedBack__maxdelay ? this->allpass_tilde_10_feedBack__maxdelay : (size < (this->allpass_tilde_10_feedBack_reader != this->allpass_tilde_10_feedBack_writer) ? this->allpass_tilde_10_feedBack_reader != this->allpass_tilde_10_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ), this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_10_feedBack_buffer->getSize()) + this->allpass_tilde_10_feedBack_reader - ((size > this->allpass_tilde_10_feedBack__maxdelay ? this->allpass_tilde_10_feedBack__maxdelay : (size < (1 + this->allpass_tilde_10_feedBack_reader != this->allpass_tilde_10_feedBack_writer) ? 1 + this->allpass_tilde_10_feedBack_reader != this->allpass_tilde_10_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ), this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ), this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ), this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_10_feedBack_buffer->getSize()) + this->allpass_tilde_10_feedBack_reader - ((size > this->allpass_tilde_10_feedBack__maxdelay ? this->allpass_tilde_10_feedBack__maxdelay : (size < (1 + this->allpass_tilde_10_feedBack_reader != this->allpass_tilde_10_feedBack_writer) ? 1 + this->allpass_tilde_10_feedBack_reader != this->allpass_tilde_10_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ), this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ), this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ), this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_10_feedBack_buffer->getSize()) + this->allpass_tilde_10_feedBack_reader - ((size > this->allpass_tilde_10_feedBack__maxdelay ? this->allpass_tilde_10_feedBack__maxdelay : (size < (this->allpass_tilde_10_feedBack_reader != this->allpass_tilde_10_feedBack_writer) ? this->allpass_tilde_10_feedBack_reader != this->allpass_tilde_10_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ), this->allpass_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_10_feedBack_buffer->getSize()) + this->allpass_tilde_10_feedBack_reader - ((size > this->allpass_tilde_10_feedBack__maxdelay ? this->allpass_tilde_10_feedBack__maxdelay : (size < (this->allpass_tilde_10_feedBack_reader != this->allpass_tilde_10_feedBack_writer) ? this->allpass_tilde_10_feedBack_reader != this->allpass_tilde_10_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_10_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_10_feedBack_wrap))
        );
    }
    
    void allpass_tilde_10_feedBack_write(number v) {
        this->allpass_tilde_10_feedBack_writer = this->allpass_tilde_10_feedBack_reader;
        this->allpass_tilde_10_feedBack_buffer[(Index)this->allpass_tilde_10_feedBack_writer] = v;
    }
    
    number allpass_tilde_10_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_10_feedBack__maxdelay : size);
        number val = this->allpass_tilde_10_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_10_feedBack_write(v);
        this->allpass_tilde_10_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_10_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_10_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_10_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_10_feedBack_init() {
        auto result = this->allpass_tilde_10_feedBack_calcSizeInSamples();
        this->allpass_tilde_10_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_10_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_10_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_10_feedBack_clear() {
        this->allpass_tilde_10_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_10_feedBack_reset() {
        auto result = this->allpass_tilde_10_feedBack_calcSizeInSamples();
        this->allpass_tilde_10_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_10_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_10_feedBack_buffer);
        this->allpass_tilde_10_feedBack_wrap = this->allpass_tilde_10_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_10_feedBack_clear();
    
        if (this->allpass_tilde_10_feedBack_reader >= this->allpass_tilde_10_feedBack__maxdelay || this->allpass_tilde_10_feedBack_writer >= this->allpass_tilde_10_feedBack__maxdelay) {
            this->allpass_tilde_10_feedBack_reader = 0;
            this->allpass_tilde_10_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_10_feedBack_dspsetup() {
        this->allpass_tilde_10_feedBack_reset();
    }
    
    number allpass_tilde_10_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_10_feedBack_size() {
        return this->allpass_tilde_10_feedBack__maxdelay;
    }
    
    void allpass_tilde_10_init() {
        this->allpass_tilde_10_feedForward_init();
        this->allpass_tilde_10_feedBack_init();
    }
    
    void allpass_tilde_10_clear() {
        this->allpass_tilde_10_feedForward_clear();
        this->allpass_tilde_10_feedBack_clear();
    }
    
    void allpass_tilde_10_reset() {
        this->allpass_tilde_10_init();
        this->allpass_tilde_10_clear();
    }
    
    void allpass_tilde_10_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_10_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_10_reset();
        this->allpass_tilde_10_setupDone = true;
        this->allpass_tilde_10_feedForward_dspsetup();
        this->allpass_tilde_10_feedBack_dspsetup();
    }
    
    void onepole_tilde_01_reset() {
        this->onepole_tilde_01_lastY = 0;
        this->onepole_tilde_01_a0 = 0;
        this->onepole_tilde_01_b1 = 0;
    }
    
    void onepole_tilde_01_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_01_needsUpdate = true;
        this->onepole_tilde_01_reset();
        this->onepole_tilde_01_setupDone = true;
    }
    
    void lores_01_reset() {
        this->lores_01_y_1 = 0;
        this->lores_01_y_2 = 0;
        this->lores_01_last_resonance = 0;
        this->lores_01_last_frequency = 0;
        this->lores_01_last_res_calc = 0;
        this->lores_01_last_freq_calc = 0;
    }
    
    void lores_01_dspsetup(bool force) {
        if ((bool)(this->lores_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->lores_01_reset();
        this->lores_01_setupDone = true;
    }
    
    void noise_tilde_01_init() {
        this->noise_tilde_01_reset();
    }
    
    void noise_tilde_01_reset() {
        xoshiro_reset(
            systemticks() + this->voice() + this->random(0, 10000),
            this->noise_tilde_01_state
        );
    }
    
    void delaytilde_01_del_step() {
        this->delaytilde_01_del_reader++;
    
        if (this->delaytilde_01_del_reader >= (int)(this->delaytilde_01_del_buffer->getSize()))
            this->delaytilde_01_del_reader = 0;
    }
    
    number delaytilde_01_del_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->delaytilde_01_del_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? 1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->delaytilde_01_del_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? 1 + this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->delaytilde_01_del_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->delaytilde_01_del_wrap))
            ), this->delaytilde_01_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->delaytilde_01_del_wrap))
            ));
        }
    
        number r = (int)(this->delaytilde_01_del_buffer->getSize()) + this->delaytilde_01_del_reader - ((size > this->delaytilde_01_del__maxdelay ? this->delaytilde_01_del__maxdelay : (size < (this->delaytilde_01_del_reader != this->delaytilde_01_del_writer) ? this->delaytilde_01_del_reader != this->delaytilde_01_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->delaytilde_01_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->delaytilde_01_del_wrap))
        );
    }
    
    void delaytilde_01_del_write(number v) {
        this->delaytilde_01_del_writer = this->delaytilde_01_del_reader;
        this->delaytilde_01_del_buffer[(Index)this->delaytilde_01_del_writer] = v;
    }
    
    number delaytilde_01_del_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->delaytilde_01_del__maxdelay : size);
        number val = this->delaytilde_01_del_read(effectiveSize, 0);
        this->delaytilde_01_del_write(v);
        this->delaytilde_01_del_step();
        return val;
    }
    
    array<Index, 2> delaytilde_01_del_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->delaytilde_01_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->delaytilde_01_del_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void delaytilde_01_del_init() {
        auto result = this->delaytilde_01_del_calcSizeInSamples();
        this->delaytilde_01_del__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->delaytilde_01_del_buffer->requestSize(requestedSizeInSamples, 1);
        this->delaytilde_01_del_wrap = requestedSizeInSamples - 1;
    }
    
    void delaytilde_01_del_clear() {
        this->delaytilde_01_del_buffer->setZero();
    }
    
    void delaytilde_01_del_reset() {
        auto result = this->delaytilde_01_del_calcSizeInSamples();
        this->delaytilde_01_del__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->delaytilde_01_del_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->delaytilde_01_del_buffer);
        this->delaytilde_01_del_wrap = this->delaytilde_01_del_buffer->getSize() - 1;
        this->delaytilde_01_del_clear();
    
        if (this->delaytilde_01_del_reader >= this->delaytilde_01_del__maxdelay || this->delaytilde_01_del_writer >= this->delaytilde_01_del__maxdelay) {
            this->delaytilde_01_del_reader = 0;
            this->delaytilde_01_del_writer = 0;
        }
    }
    
    void delaytilde_01_del_dspsetup() {
        this->delaytilde_01_del_reset();
    }
    
    number delaytilde_01_del_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 38400;
    }
    
    number delaytilde_01_del_size() {
        return this->delaytilde_01_del__maxdelay;
    }
    
    void delaytilde_01_dspsetup(bool force) {
        if ((bool)(this->delaytilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->delaytilde_01_rampInSamples = (long)(this->mstosamps(0));
        this->delaytilde_01_lastDelay = -1;
        this->delaytilde_01_setupDone = true;
        this->delaytilde_01_del_dspsetup();
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
        allpass_tilde_09_x = 0;
        allpass_tilde_09_delayTime = 22.58;
        allpass_tilde_09_gain = -0.7;
        dspexpr_11_in1 = 0;
        dspexpr_11_in2 = 22.58;
        comb_tilde_08_x = 0;
        comb_tilde_08_delayTime = 149.6;
        comb_tilde_08_gain = 0;
        comb_tilde_08_ffGain = 1;
        comb_tilde_08_fbGain = 0;
        dspexpr_12_in1 = 0;
        dspexpr_12_in2 = 0;
        dspexpr_13_in1 = 0;
        dspexpr_13_in2 = 149.6;
        dspexpr_14_in1 = 0;
        dspexpr_14_in2 = 0.5;
        allpass_tilde_10_x = 0;
        allpass_tilde_10_delayTime = 60.48;
        allpass_tilde_10_gain = 0.5;
        onepole_tilde_01_x = 0;
        onepole_tilde_01_freqInHz = 1800;
        dspexpr_15_in1 = 0;
        dspexpr_15_in2 = 20;
        lores_01_cutoff = 0;
        lores_01_resonance = 0.995;
        lores_01_resonance_setter(lores_01_resonance);
        dspexpr_16_in1 = 0;
        dspexpr_16_in2 = 60.48;
        delaytilde_01_delay = 0;
        mstosamps_tilde_01_ms = 0;
        dspexpr_17_in1 = 0;
        dspexpr_17_in2 = 125;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        signals[4] = nullptr;
        signals[5] = nullptr;
        signals[6] = nullptr;
        signals[7] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        allpass_tilde_09_feedForward__maxdelay = 0;
        allpass_tilde_09_feedForward_sizemode = 0;
        allpass_tilde_09_feedForward_wrap = 0;
        allpass_tilde_09_feedForward_reader = 0;
        allpass_tilde_09_feedForward_writer = 0;
        allpass_tilde_09_feedBack__maxdelay = 0;
        allpass_tilde_09_feedBack_sizemode = 0;
        allpass_tilde_09_feedBack_wrap = 0;
        allpass_tilde_09_feedBack_reader = 0;
        allpass_tilde_09_feedBack_writer = 0;
        allpass_tilde_09_setupDone = false;
        comb_tilde_08_feedForward__maxdelay = 0;
        comb_tilde_08_feedForward_sizemode = 0;
        comb_tilde_08_feedForward_wrap = 0;
        comb_tilde_08_feedForward_reader = 0;
        comb_tilde_08_feedForward_writer = 0;
        comb_tilde_08_feedBack__maxdelay = 0;
        comb_tilde_08_feedBack_sizemode = 0;
        comb_tilde_08_feedBack_wrap = 0;
        comb_tilde_08_feedBack_reader = 0;
        comb_tilde_08_feedBack_writer = 0;
        comb_tilde_08_setupDone = false;
        allpass_tilde_10_feedForward__maxdelay = 0;
        allpass_tilde_10_feedForward_sizemode = 0;
        allpass_tilde_10_feedForward_wrap = 0;
        allpass_tilde_10_feedForward_reader = 0;
        allpass_tilde_10_feedForward_writer = 0;
        allpass_tilde_10_feedBack__maxdelay = 0;
        allpass_tilde_10_feedBack_sizemode = 0;
        allpass_tilde_10_feedBack_wrap = 0;
        allpass_tilde_10_feedBack_reader = 0;
        allpass_tilde_10_feedBack_writer = 0;
        allpass_tilde_10_setupDone = false;
        onepole_tilde_01_freq = 0;
        onepole_tilde_01_needsUpdate = false;
        onepole_tilde_01_lastY = 0;
        onepole_tilde_01_a0 = 0;
        onepole_tilde_01_b1 = 0;
        onepole_tilde_01_setupDone = false;
        lores_01_setupDone = false;
        delaytilde_01_lastDelay = -1;
        delaytilde_01_crossfadeDelay = 0;
        delaytilde_01_ramp = 0;
        delaytilde_01_rampInSamples = 0;
        delaytilde_01_del__maxdelay = 0;
        delaytilde_01_del_sizemode = 0;
        delaytilde_01_del_wrap = 0;
        delaytilde_01_del_reader = 0;
        delaytilde_01_del_writer = 0;
        delaytilde_01_setupDone = false;
        feedbacktilde_01_feedbackbuffer = nullptr;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number allpass_tilde_09_x;
        number allpass_tilde_09_delayTime;
        number allpass_tilde_09_gain;
        number dspexpr_11_in1;
        number dspexpr_11_in2;
        number comb_tilde_08_x;
        number comb_tilde_08_delayTime;
        number comb_tilde_08_gain;
        number comb_tilde_08_ffGain;
        number comb_tilde_08_fbGain;
        number dspexpr_12_in1;
        number dspexpr_12_in2;
        number dspexpr_13_in1;
        number dspexpr_13_in2;
        number dspexpr_14_in1;
        number dspexpr_14_in2;
        number allpass_tilde_10_x;
        number allpass_tilde_10_delayTime;
        number allpass_tilde_10_gain;
        number onepole_tilde_01_x;
        number onepole_tilde_01_freqInHz;
        number dspexpr_15_in1;
        number dspexpr_15_in2;
        number lores_01_cutoff;
        number lores_01_resonance;
        number dspexpr_16_in1;
        number dspexpr_16_in2;
        number delaytilde_01_delay;
        number mstosamps_tilde_01_ms;
        number dspexpr_17_in1;
        number dspexpr_17_in2;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[8];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float64BufferRef allpass_tilde_09_feedForward_buffer;
        Index allpass_tilde_09_feedForward__maxdelay;
        Int allpass_tilde_09_feedForward_sizemode;
        Index allpass_tilde_09_feedForward_wrap;
        Int allpass_tilde_09_feedForward_reader;
        Int allpass_tilde_09_feedForward_writer;
        Float64BufferRef allpass_tilde_09_feedBack_buffer;
        Index allpass_tilde_09_feedBack__maxdelay;
        Int allpass_tilde_09_feedBack_sizemode;
        Index allpass_tilde_09_feedBack_wrap;
        Int allpass_tilde_09_feedBack_reader;
        Int allpass_tilde_09_feedBack_writer;
        bool allpass_tilde_09_setupDone;
        Float64BufferRef comb_tilde_08_feedForward_buffer;
        Index comb_tilde_08_feedForward__maxdelay;
        Int comb_tilde_08_feedForward_sizemode;
        Index comb_tilde_08_feedForward_wrap;
        Int comb_tilde_08_feedForward_reader;
        Int comb_tilde_08_feedForward_writer;
        Float64BufferRef comb_tilde_08_feedBack_buffer;
        Index comb_tilde_08_feedBack__maxdelay;
        Int comb_tilde_08_feedBack_sizemode;
        Index comb_tilde_08_feedBack_wrap;
        Int comb_tilde_08_feedBack_reader;
        Int comb_tilde_08_feedBack_writer;
        bool comb_tilde_08_setupDone;
        Float64BufferRef allpass_tilde_10_feedForward_buffer;
        Index allpass_tilde_10_feedForward__maxdelay;
        Int allpass_tilde_10_feedForward_sizemode;
        Index allpass_tilde_10_feedForward_wrap;
        Int allpass_tilde_10_feedForward_reader;
        Int allpass_tilde_10_feedForward_writer;
        Float64BufferRef allpass_tilde_10_feedBack_buffer;
        Index allpass_tilde_10_feedBack__maxdelay;
        Int allpass_tilde_10_feedBack_sizemode;
        Index allpass_tilde_10_feedBack_wrap;
        Int allpass_tilde_10_feedBack_reader;
        Int allpass_tilde_10_feedBack_writer;
        bool allpass_tilde_10_setupDone;
        number onepole_tilde_01_freq;
        bool onepole_tilde_01_needsUpdate;
        number onepole_tilde_01_lastY;
        number onepole_tilde_01_a0;
        number onepole_tilde_01_b1;
        bool onepole_tilde_01_setupDone;
        number lores_01_b_0;
        number lores_01_a_1;
        number lores_01_a_2;
        number lores_01_x_0;
        number lores_01_y_1;
        number lores_01_y_2;
        number lores_01_last_resonance;
        number lores_01_last_frequency;
        number lores_01_last_res_calc;
        number lores_01_last_freq_calc;
        bool lores_01_setupDone;
        UInt noise_tilde_01_state[4] = { };
        number delaytilde_01_lastDelay;
        number delaytilde_01_crossfadeDelay;
        number delaytilde_01_ramp;
        long delaytilde_01_rampInSamples;
        Float64BufferRef delaytilde_01_del_buffer;
        Index delaytilde_01_del__maxdelay;
        Int delaytilde_01_del_sizemode;
        Index delaytilde_01_del_wrap;
        Int delaytilde_01_del_reader;
        Int delaytilde_01_del_writer;
        bool delaytilde_01_setupDone;
        signal feedbacktilde_01_feedbackbuffer;
        number stackprotect_count;
        DataRef allpass_tilde_09_feedForward_bufferobj;
        DataRef allpass_tilde_09_feedBack_bufferobj;
        DataRef comb_tilde_08_feedForward_bufferobj;
        DataRef comb_tilde_08_feedBack_bufferobj;
        DataRef allpass_tilde_10_feedForward_bufferobj;
        DataRef allpass_tilde_10_feedBack_bufferobj;
        DataRef delaytilde_01_del_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_22 : public PatcherInterfaceImpl {
    
    friend class plate;
    
    public:
    
    RNBOSubpatcher_22()
    {
    }
    
    ~RNBOSubpatcher_22()
    {
    }
    
    virtual plate* getPatcher() const {
        return static_cast<plate *>(_parentPatcher);
    }
    
    plate* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
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
        const SampleValue * target_Damp = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        this->stackprotect_perform(n);
        this->signalreceive_08_perform(target_Damp, this->signals[0], n);
        this->onepole_tilde_03_perform(in2, this->signals[0], out2, n);
        this->onepole_tilde_02_perform(in1, this->signals[0], out1, n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 1; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
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
    
        this->onepole_tilde_03_dspsetup(forceDSPSetup);
        this->onepole_tilde_02_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        default:
            {
            return 0;
            }
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 1;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
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
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 0;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void processDataViewUpdate(DataRefIndex , MillisecondTime ) {}
    
    void initialize() {
        this->assign_defaults();
        this->setState();
    }
    
    protected:
    
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
    
    void initializeObjects() {}
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void signalreceive_08_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void onepole_tilde_03_perform(const Sample * x, const Sample * freqInHz, SampleValue * out1, Index n) {
        auto __onepole_tilde_03_lastY = this->onepole_tilde_03_lastY;
        auto __onepole_tilde_03_b1 = this->onepole_tilde_03_b1;
        auto __onepole_tilde_03_a0 = this->onepole_tilde_03_a0;
        auto __onepole_tilde_03_needsUpdate = this->onepole_tilde_03_needsUpdate;
        auto __onepole_tilde_03_freq = this->onepole_tilde_03_freq;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (__onepole_tilde_03_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_03_needsUpdate)) {
                __onepole_tilde_03_freq = freqInHz[(Index)i];
                __onepole_tilde_03_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_03_a0 = (__onepole_tilde_03_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_03_a0 < 0.00001 ? 0.00001 : __onepole_tilde_03_a0));
                __onepole_tilde_03_b1 = 1 - __onepole_tilde_03_a0;
                __onepole_tilde_03_needsUpdate = false;
            }
    
            __onepole_tilde_03_lastY = __onepole_tilde_03_a0 * x[(Index)i] + __onepole_tilde_03_b1 * __onepole_tilde_03_lastY;
            out1[(Index)i] = __onepole_tilde_03_lastY;
        }
    
        this->onepole_tilde_03_freq = __onepole_tilde_03_freq;
        this->onepole_tilde_03_needsUpdate = __onepole_tilde_03_needsUpdate;
        this->onepole_tilde_03_a0 = __onepole_tilde_03_a0;
        this->onepole_tilde_03_b1 = __onepole_tilde_03_b1;
        this->onepole_tilde_03_lastY = __onepole_tilde_03_lastY;
    }
    
    void onepole_tilde_02_perform(const Sample * x, const Sample * freqInHz, SampleValue * out1, Index n) {
        auto __onepole_tilde_02_lastY = this->onepole_tilde_02_lastY;
        auto __onepole_tilde_02_b1 = this->onepole_tilde_02_b1;
        auto __onepole_tilde_02_a0 = this->onepole_tilde_02_a0;
        auto __onepole_tilde_02_needsUpdate = this->onepole_tilde_02_needsUpdate;
        auto __onepole_tilde_02_freq = this->onepole_tilde_02_freq;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (__onepole_tilde_02_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_02_needsUpdate)) {
                __onepole_tilde_02_freq = freqInHz[(Index)i];
                __onepole_tilde_02_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_02_a0 = (__onepole_tilde_02_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_02_a0 < 0.00001 ? 0.00001 : __onepole_tilde_02_a0));
                __onepole_tilde_02_b1 = 1 - __onepole_tilde_02_a0;
                __onepole_tilde_02_needsUpdate = false;
            }
    
            __onepole_tilde_02_lastY = __onepole_tilde_02_a0 * x[(Index)i] + __onepole_tilde_02_b1 * __onepole_tilde_02_lastY;
            out1[(Index)i] = __onepole_tilde_02_lastY;
        }
    
        this->onepole_tilde_02_freq = __onepole_tilde_02_freq;
        this->onepole_tilde_02_needsUpdate = __onepole_tilde_02_needsUpdate;
        this->onepole_tilde_02_a0 = __onepole_tilde_02_a0;
        this->onepole_tilde_02_b1 = __onepole_tilde_02_b1;
        this->onepole_tilde_02_lastY = __onepole_tilde_02_lastY;
    }
    
    void onepole_tilde_02_reset() {
        this->onepole_tilde_02_lastY = 0;
        this->onepole_tilde_02_a0 = 0;
        this->onepole_tilde_02_b1 = 0;
    }
    
    void onepole_tilde_02_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_02_needsUpdate = true;
        this->onepole_tilde_02_reset();
        this->onepole_tilde_02_setupDone = true;
    }
    
    void onepole_tilde_03_reset() {
        this->onepole_tilde_03_lastY = 0;
        this->onepole_tilde_03_a0 = 0;
        this->onepole_tilde_03_b1 = 0;
    }
    
    void onepole_tilde_03_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_03_needsUpdate = true;
        this->onepole_tilde_03_reset();
        this->onepole_tilde_03_setupDone = true;
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
        onepole_tilde_02_x = 0;
        onepole_tilde_02_freqInHz = 1800;
        onepole_tilde_03_x = 0;
        onepole_tilde_03_freqInHz = 1800;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        onepole_tilde_02_freq = 0;
        onepole_tilde_02_needsUpdate = false;
        onepole_tilde_02_lastY = 0;
        onepole_tilde_02_a0 = 0;
        onepole_tilde_02_b1 = 0;
        onepole_tilde_02_setupDone = false;
        onepole_tilde_03_freq = 0;
        onepole_tilde_03_needsUpdate = false;
        onepole_tilde_03_lastY = 0;
        onepole_tilde_03_a0 = 0;
        onepole_tilde_03_b1 = 0;
        onepole_tilde_03_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number onepole_tilde_02_x;
        number onepole_tilde_02_freqInHz;
        number onepole_tilde_03_x;
        number onepole_tilde_03_freqInHz;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[1];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        number onepole_tilde_02_freq;
        bool onepole_tilde_02_needsUpdate;
        number onepole_tilde_02_lastY;
        number onepole_tilde_02_a0;
        number onepole_tilde_02_b1;
        bool onepole_tilde_02_setupDone;
        number onepole_tilde_03_freq;
        bool onepole_tilde_03_needsUpdate;
        number onepole_tilde_03_lastY;
        number onepole_tilde_03_a0;
        number onepole_tilde_03_b1;
        bool onepole_tilde_03_setupDone;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_23 : public PatcherInterfaceImpl {
    
    friend class plate;
    
    public:
    
    RNBOSubpatcher_23()
    {
    }
    
    ~RNBOSubpatcher_23()
    {
    }
    
    virtual plate* getPatcher() const {
        return static_cast<plate *>(_parentPatcher);
    }
    
    plate* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
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
        const SampleValue * target_Size = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        const SampleValue * target_Decay = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        const SampleValue * target_Jitter = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        const SampleValue * target_Damp = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
        SampleValue * out3 = (numOutputs >= 3 && outputs[2] ? outputs[2] : this->dummyBuffer);
        SampleValue * out4 = (numOutputs >= 4 && outputs[3] ? outputs[3] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        this->noise_tilde_02_perform(this->signals[0], n);
    
        this->lores_02_perform(
            this->lores_02_cutoff,
            this->lores_02_resonance,
            this->signals[0],
            this->signals[1],
            n
        );
    
        this->feedbackreader_02_perform(this->signals[0], n);
        this->stackprotect_perform(n);
        this->signalreceive_09_perform(target_Size, this->signals[2], n);
        this->dspexpr_20_perform(this->signals[2], this->dspexpr_20_in2, this->signals[3], n);
        this->dspexpr_18_perform(this->signals[2], this->dspexpr_18_in2, this->signals[4], n);
        this->signaladder_04_perform(in1, this->signals[0], this->signals[0], n);
        this->allpass_tilde_11_perform(this->signals[0], this->signals[4], this->allpass_tilde_11_gain, out1, n);
        this->signalreceive_13_perform(target_Size, this->signals[4], n);
        this->dspexpr_24_perform(this->signals[4], this->dspexpr_24_in2, this->signals[2], n);
        this->mstosamps_tilde_02_perform(this->signals[2], this->signals[5], n);
        this->dspexpr_23_perform(this->signals[4], this->dspexpr_23_in2, this->signals[2], n);
        this->signalreceive_10_perform(target_Decay, this->signals[4], n);
        this->signalreceive_11_perform(target_Jitter, this->signals[6], n);
        this->dspexpr_22_perform(this->signals[1], this->signals[6], this->signals[7], n);
        this->dspexpr_19_perform(this->signals[3], this->signals[7], this->signals[6], n);
    
        this->comb_tilde_09_perform(
            out1,
            this->signals[6],
            this->comb_tilde_09_gain,
            this->comb_tilde_09_ffGain,
            this->comb_tilde_09_fbGain,
            this->signals[7],
            n
        );
    
        this->signalforwarder_17_perform(this->signals[7], out2, n);
        this->signalreceive_12_perform(target_Damp, this->signals[6], n);
        this->onepole_tilde_04_perform(this->signals[7], this->signals[6], this->signals[3], n);
        this->dspexpr_21_perform(this->signals[3], this->signals[4], this->signals[6], n);
    
        this->allpass_tilde_12_perform(
            this->signals[6],
            this->signals[2],
            this->allpass_tilde_12_gain,
            this->signals[4],
            n
        );
    
        this->delaytilde_02_perform(this->signals[5], this->signals[4], out4, n);
        this->feedbackwriter_02_perform(out4, n);
        this->signalforwarder_18_perform(this->signals[4], out3, n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 8; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->feedbacktilde_02_feedbackbuffer = resizeSignal(this->feedbacktilde_02_feedbackbuffer, this->maxvs, maxBlockSize);
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
    
        this->lores_02_dspsetup(forceDSPSetup);
        this->allpass_tilde_11_dspsetup(forceDSPSetup);
        this->comb_tilde_09_dspsetup(forceDSPSetup);
        this->onepole_tilde_04_dspsetup(forceDSPSetup);
        this->allpass_tilde_12_dspsetup(forceDSPSetup);
        this->delaytilde_02_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        default:
            {
            return 0;
            }
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 4;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
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
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 0:
            {
            return addressOf(this->allpass_tilde_11_feedForward_bufferobj);
            break;
            }
        case 1:
            {
            return addressOf(this->allpass_tilde_11_feedBack_bufferobj);
            break;
            }
        case 2:
            {
            return addressOf(this->comb_tilde_09_feedForward_bufferobj);
            break;
            }
        case 3:
            {
            return addressOf(this->comb_tilde_09_feedBack_bufferobj);
            break;
            }
        case 4:
            {
            return addressOf(this->allpass_tilde_12_feedForward_bufferobj);
            break;
            }
        case 5:
            {
            return addressOf(this->allpass_tilde_12_feedBack_bufferobj);
            break;
            }
        case 6:
            {
            return addressOf(this->delaytilde_02_del_bufferobj);
            break;
            }
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 7;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void zeroDataRef(DataRef& ref) {
        ref->setZero();
    }
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 0) {
            this->allpass_tilde_11_feedForward_buffer = new Float64Buffer(this->allpass_tilde_11_feedForward_bufferobj);
        }
    
        if (index == 1) {
            this->allpass_tilde_11_feedBack_buffer = new Float64Buffer(this->allpass_tilde_11_feedBack_bufferobj);
        }
    
        if (index == 2) {
            this->comb_tilde_09_feedForward_buffer = new Float64Buffer(this->comb_tilde_09_feedForward_bufferobj);
        }
    
        if (index == 3) {
            this->comb_tilde_09_feedBack_buffer = new Float64Buffer(this->comb_tilde_09_feedBack_bufferobj);
        }
    
        if (index == 4) {
            this->allpass_tilde_12_feedForward_buffer = new Float64Buffer(this->allpass_tilde_12_feedForward_bufferobj);
        }
    
        if (index == 5) {
            this->allpass_tilde_12_feedBack_buffer = new Float64Buffer(this->allpass_tilde_12_feedBack_bufferobj);
        }
    
        if (index == 6) {
            this->delaytilde_02_del_buffer = new Float64Buffer(this->delaytilde_02_del_bufferobj);
        }
    }
    
    void initialize() {
        this->allpass_tilde_11_feedForward_bufferobj = initDataRef("allpass_tilde_11_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_11_feedBack_bufferobj = initDataRef("allpass_tilde_11_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_09_feedForward_bufferobj = initDataRef("comb_tilde_09_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_09_feedBack_bufferobj = initDataRef("comb_tilde_09_feedBack_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_12_feedForward_bufferobj = initDataRef("allpass_tilde_12_feedForward_bufferobj", true, nullptr, "buffer~");
        this->allpass_tilde_12_feedBack_bufferobj = initDataRef("allpass_tilde_12_feedBack_bufferobj", true, nullptr, "buffer~");
        this->delaytilde_02_del_bufferobj = initDataRef("delaytilde_02_del_bufferobj", true, nullptr, "buffer~");
        this->assign_defaults();
        this->setState();
        this->allpass_tilde_11_feedForward_bufferobj->setIndex(0);
        this->allpass_tilde_11_feedForward_buffer = new Float64Buffer(this->allpass_tilde_11_feedForward_bufferobj);
        this->allpass_tilde_11_feedBack_bufferobj->setIndex(1);
        this->allpass_tilde_11_feedBack_buffer = new Float64Buffer(this->allpass_tilde_11_feedBack_bufferobj);
        this->comb_tilde_09_feedForward_bufferobj->setIndex(2);
        this->comb_tilde_09_feedForward_buffer = new Float64Buffer(this->comb_tilde_09_feedForward_bufferobj);
        this->comb_tilde_09_feedBack_bufferobj->setIndex(3);
        this->comb_tilde_09_feedBack_buffer = new Float64Buffer(this->comb_tilde_09_feedBack_bufferobj);
        this->allpass_tilde_12_feedForward_bufferobj->setIndex(4);
        this->allpass_tilde_12_feedForward_buffer = new Float64Buffer(this->allpass_tilde_12_feedForward_bufferobj);
        this->allpass_tilde_12_feedBack_bufferobj->setIndex(5);
        this->allpass_tilde_12_feedBack_buffer = new Float64Buffer(this->allpass_tilde_12_feedBack_bufferobj);
        this->delaytilde_02_del_bufferobj->setIndex(6);
        this->delaytilde_02_del_buffer = new Float64Buffer(this->delaytilde_02_del_bufferobj);
    }
    
    protected:
    
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
        return 1;
    }
    
    Index getNumOutputChannels() const {
        return 4;
    }
    
    void initializeObjects() {
        this->allpass_tilde_11_feedForward_init();
        this->allpass_tilde_11_feedBack_init();
        this->allpass_tilde_11_init();
        this->comb_tilde_09_feedForward_init();
        this->comb_tilde_09_feedBack_init();
        this->comb_tilde_09_init();
        this->allpass_tilde_12_feedForward_init();
        this->allpass_tilde_12_feedBack_init();
        this->allpass_tilde_12_init();
        this->noise_tilde_02_init();
        this->delaytilde_02_del_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->allpass_tilde_11_feedForward_buffer = this->allpass_tilde_11_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_11_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_11_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_11_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(0);
        }
    
        this->allpass_tilde_11_feedBack_buffer = this->allpass_tilde_11_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_11_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_11_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_11_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    
        this->comb_tilde_09_feedForward_buffer = this->comb_tilde_09_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_09_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_09_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_09_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(2);
        }
    
        this->comb_tilde_09_feedBack_buffer = this->comb_tilde_09_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_09_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_09_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_09_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(3);
        }
    
        this->allpass_tilde_12_feedForward_buffer = this->allpass_tilde_12_feedForward_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_12_feedForward_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_12_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_12_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(4);
        }
    
        this->allpass_tilde_12_feedBack_buffer = this->allpass_tilde_12_feedBack_buffer->allocateIfNeeded();
    
        if (this->allpass_tilde_12_feedBack_bufferobj->hasRequestedSize()) {
            if (this->allpass_tilde_12_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->allpass_tilde_12_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(5);
        }
    
        this->delaytilde_02_del_buffer = this->delaytilde_02_del_buffer->allocateIfNeeded();
    
        if (this->delaytilde_02_del_bufferobj->hasRequestedSize()) {
            if (this->delaytilde_02_del_bufferobj->wantsFill())
                this->zeroDataRef(this->delaytilde_02_del_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(6);
        }
    }
    
    void noise_tilde_02_perform(SampleValue * out1, Index n) {
        auto __noise_tilde_02_state = this->noise_tilde_02_state;
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = xoshiro_next(__noise_tilde_02_state);
        }
    }
    
    void lores_02_perform(
        number cutoff,
        number resonance,
        const SampleValue * input,
        SampleValue * out,
        Index n
    ) {
        RNBO_UNUSED(resonance);
        RNBO_UNUSED(cutoff);
        auto __lores_02_y_2 = this->lores_02_y_2;
        auto __lores_02_y_1 = this->lores_02_y_1;
        auto __lores_02_x_0 = this->lores_02_x_0;
        auto __lores_02_b_0 = this->lores_02_b_0;
        auto __lores_02_a_1 = this->lores_02_a_1;
        auto __lores_02_a_2 = this->lores_02_a_2;
        auto __lores_02_last_freq_calc = this->lores_02_last_freq_calc;
        auto __lores_02_last_res_calc = this->lores_02_last_res_calc;
        auto __lores_02_last_frequency = this->lores_02_last_frequency;
        auto __lores_02_last_resonance = this->lores_02_last_resonance;
    
        for (Index i = 0; i < n; i++) {
            if (0.995 != __lores_02_last_resonance || 0 != __lores_02_last_frequency) {
                __lores_02_last_res_calc = rnbo_exp(0.995 * 0.125) * 0.882497;
                __lores_02_last_freq_calc = rnbo_cos(2.0 * pi01() / this->sr * 0);
                __lores_02_last_resonance = 0.995;
                __lores_02_last_frequency = 0;
            }
    
            number res_temp = __lores_02_last_res_calc;
            number freq_temp = __lores_02_last_freq_calc;
            __lores_02_a_2 = res_temp * res_temp;
            __lores_02_a_1 = -2.0 * res_temp * freq_temp;
            __lores_02_b_0 = 1.0 + __lores_02_a_1 + __lores_02_a_2;
            __lores_02_x_0 = input[(Index)i];
            number y_0 = __lores_02_x_0 * __lores_02_b_0 - __lores_02_y_1 * __lores_02_a_1 - __lores_02_y_2 * __lores_02_a_2;
            __lores_02_y_2 = __lores_02_y_1;
            __lores_02_y_1 = y_0;
            out[(Index)i] = y_0;
        }
    
        this->lores_02_last_resonance = __lores_02_last_resonance;
        this->lores_02_last_frequency = __lores_02_last_frequency;
        this->lores_02_last_res_calc = __lores_02_last_res_calc;
        this->lores_02_last_freq_calc = __lores_02_last_freq_calc;
        this->lores_02_a_2 = __lores_02_a_2;
        this->lores_02_a_1 = __lores_02_a_1;
        this->lores_02_b_0 = __lores_02_b_0;
        this->lores_02_x_0 = __lores_02_x_0;
        this->lores_02_y_1 = __lores_02_y_1;
        this->lores_02_y_2 = __lores_02_y_2;
    }
    
    void feedbackreader_02_perform(SampleValue * output, Index n) {
        auto& buffer = this->feedbacktilde_02_feedbackbuffer;
    
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = buffer[(Index)i];
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void signalreceive_09_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void dspexpr_20_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 141.7;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_18_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 30.51;//#map:_###_obj_###_:1
        }
    }
    
    void signaladder_04_perform(
        const SampleValue * in1,
        const SampleValue * in2,
        SampleValue * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out[(Index)i] = in1[(Index)i] + in2[(Index)i];
        }
    }
    
    void allpass_tilde_11_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = -0.7;
            auto delayTimeinSamps = this->mstosamps(delayTime[(Index)i]);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_11_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_11_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_11_feedForward_write(x[(Index)i]);
            this->allpass_tilde_11_feedBack_write(ov);
            this->allpass_tilde_11_feedForward_step();
            this->allpass_tilde_11_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void signalreceive_13_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void dspexpr_24_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 106.28;//#map:_###_obj_###_:1
        }
    }
    
    void mstosamps_tilde_02_perform(const Sample * ms, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = ms[(Index)i] * this->sr * 0.001;
        }
    }
    
    void dspexpr_23_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 89.24;//#map:_###_obj_###_:1
        }
    }
    
    void signalreceive_10_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void signalreceive_11_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void dspexpr_22_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_19_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_09_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_09_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_09_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_09_feedForward_write(x[(Index)i]);
            this->comb_tilde_09_feedBack_write(ov);
            this->comb_tilde_09_feedForward_step();
            this->comb_tilde_09_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void signalforwarder_17_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void signalreceive_12_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void onepole_tilde_04_perform(const Sample * x, const Sample * freqInHz, SampleValue * out1, Index n) {
        auto __onepole_tilde_04_lastY = this->onepole_tilde_04_lastY;
        auto __onepole_tilde_04_b1 = this->onepole_tilde_04_b1;
        auto __onepole_tilde_04_a0 = this->onepole_tilde_04_a0;
        auto __onepole_tilde_04_needsUpdate = this->onepole_tilde_04_needsUpdate;
        auto __onepole_tilde_04_freq = this->onepole_tilde_04_freq;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (__onepole_tilde_04_freq != freqInHz[(Index)i] || (bool)(__onepole_tilde_04_needsUpdate)) {
                __onepole_tilde_04_freq = freqInHz[(Index)i];
                __onepole_tilde_04_a0 = 1 - rnbo_exp(-6.28318530717958647692 * freqInHz[(Index)i] / this->sr);
                __onepole_tilde_04_a0 = (__onepole_tilde_04_a0 > 0.99999 ? 0.99999 : (__onepole_tilde_04_a0 < 0.00001 ? 0.00001 : __onepole_tilde_04_a0));
                __onepole_tilde_04_b1 = 1 - __onepole_tilde_04_a0;
                __onepole_tilde_04_needsUpdate = false;
            }
    
            __onepole_tilde_04_lastY = __onepole_tilde_04_a0 * x[(Index)i] + __onepole_tilde_04_b1 * __onepole_tilde_04_lastY;
            out1[(Index)i] = __onepole_tilde_04_lastY;
        }
    
        this->onepole_tilde_04_freq = __onepole_tilde_04_freq;
        this->onepole_tilde_04_needsUpdate = __onepole_tilde_04_needsUpdate;
        this->onepole_tilde_04_a0 = __onepole_tilde_04_a0;
        this->onepole_tilde_04_b1 = __onepole_tilde_04_b1;
        this->onepole_tilde_04_lastY = __onepole_tilde_04_lastY;
    }
    
    void dspexpr_21_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void allpass_tilde_12_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __gain = 0.5;
            auto delayTimeinSamps = this->mstosamps(delayTime[(Index)i]);
            delayTimeinSamps = (delayTimeinSamps > 44100 ? 44100 : (delayTimeinSamps < 0 ? 0 : delayTimeinSamps));
            __gain = (__gain > 0.99 ? 0.99 : (__gain < 0 ? 0 : __gain));
            number ff = this->allpass_tilde_12_feedForward_read(delayTimeinSamps, 0);
            number fb = this->allpass_tilde_12_feedBack_read(delayTimeinSamps, 0);
            number ov = ff + (x[(Index)i] - fb) * (__gain * -1.0);
            fixdenorm(ov);
            this->allpass_tilde_12_feedForward_write(x[(Index)i]);
            this->allpass_tilde_12_feedBack_write(ov);
            this->allpass_tilde_12_feedForward_step();
            this->allpass_tilde_12_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void delaytilde_02_perform(
        const Sample * delay,
        const SampleValue * input,
        SampleValue * output,
        Index n
    ) {
        auto __delaytilde_02_crossfadeDelay = this->delaytilde_02_crossfadeDelay;
        auto __delaytilde_02_rampInSamples = this->delaytilde_02_rampInSamples;
        auto __delaytilde_02_ramp = this->delaytilde_02_ramp;
        auto __delaytilde_02_lastDelay = this->delaytilde_02_lastDelay;
    
        for (Index i = 0; i < n; i++) {
            if (__delaytilde_02_lastDelay == -1) {
                __delaytilde_02_lastDelay = delay[(Index)i];
            }
    
            if (__delaytilde_02_ramp > 0) {
                number factor = __delaytilde_02_ramp / __delaytilde_02_rampInSamples;
                output[(Index)i] = this->delaytilde_02_del_read(__delaytilde_02_crossfadeDelay, 0) * factor + this->delaytilde_02_del_read(__delaytilde_02_lastDelay, 0) * (1. - factor);
                __delaytilde_02_ramp--;
            } else {
                number effectiveDelay = delay[(Index)i];
    
                if (effectiveDelay != __delaytilde_02_lastDelay) {
                    __delaytilde_02_ramp = __delaytilde_02_rampInSamples;
                    __delaytilde_02_crossfadeDelay = __delaytilde_02_lastDelay;
                    __delaytilde_02_lastDelay = effectiveDelay;
                    output[(Index)i] = this->delaytilde_02_del_read(__delaytilde_02_crossfadeDelay, 0);
                    __delaytilde_02_ramp--;
                } else {
                    output[(Index)i] = this->delaytilde_02_del_read(effectiveDelay, 0);
                }
            }
    
            this->delaytilde_02_del_write(input[(Index)i]);
            this->delaytilde_02_del_step();
        }
    
        this->delaytilde_02_lastDelay = __delaytilde_02_lastDelay;
        this->delaytilde_02_ramp = __delaytilde_02_ramp;
        this->delaytilde_02_crossfadeDelay = __delaytilde_02_crossfadeDelay;
    }
    
    void feedbackwriter_02_perform(const SampleValue * input, Index n) {
        auto& buffer = this->feedbacktilde_02_feedbackbuffer;
    
        for (Index i = 0; i < n; i++) {
            buffer[(Index)i] = input[(Index)i];
        }
    }
    
    void signalforwarder_18_perform(const SampleValue * input, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = input[(Index)i];
        }
    }
    
    void lores_02_resonance_setter(number v) {
        if (v < 0.0) {
            v = 0.0;
        } else if (v >= 1.0) {
            v = 1.0 - 1E-20;
        }
    
        this->lores_02_resonance = v;
    }
    
    void allpass_tilde_11_feedForward_step() {
        this->allpass_tilde_11_feedForward_reader++;
    
        if (this->allpass_tilde_11_feedForward_reader >= (int)(this->allpass_tilde_11_feedForward_buffer->getSize()))
            this->allpass_tilde_11_feedForward_reader = 0;
    }
    
    number allpass_tilde_11_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_11_feedForward_buffer->getSize()) + this->allpass_tilde_11_feedForward_reader - ((size > this->allpass_tilde_11_feedForward__maxdelay ? this->allpass_tilde_11_feedForward__maxdelay : (size < (this->allpass_tilde_11_feedForward_reader != this->allpass_tilde_11_feedForward_writer) ? this->allpass_tilde_11_feedForward_reader != this->allpass_tilde_11_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ), this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_11_feedForward_buffer->getSize()) + this->allpass_tilde_11_feedForward_reader - ((size > this->allpass_tilde_11_feedForward__maxdelay ? this->allpass_tilde_11_feedForward__maxdelay : (size < (1 + this->allpass_tilde_11_feedForward_reader != this->allpass_tilde_11_feedForward_writer) ? 1 + this->allpass_tilde_11_feedForward_reader != this->allpass_tilde_11_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ), this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ), this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ), this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_11_feedForward_buffer->getSize()) + this->allpass_tilde_11_feedForward_reader - ((size > this->allpass_tilde_11_feedForward__maxdelay ? this->allpass_tilde_11_feedForward__maxdelay : (size < (1 + this->allpass_tilde_11_feedForward_reader != this->allpass_tilde_11_feedForward_writer) ? 1 + this->allpass_tilde_11_feedForward_reader != this->allpass_tilde_11_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ), this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ), this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ), this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_11_feedForward_buffer->getSize()) + this->allpass_tilde_11_feedForward_reader - ((size > this->allpass_tilde_11_feedForward__maxdelay ? this->allpass_tilde_11_feedForward__maxdelay : (size < (this->allpass_tilde_11_feedForward_reader != this->allpass_tilde_11_feedForward_writer) ? this->allpass_tilde_11_feedForward_reader != this->allpass_tilde_11_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ), this->allpass_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_11_feedForward_buffer->getSize()) + this->allpass_tilde_11_feedForward_reader - ((size > this->allpass_tilde_11_feedForward__maxdelay ? this->allpass_tilde_11_feedForward__maxdelay : (size < (this->allpass_tilde_11_feedForward_reader != this->allpass_tilde_11_feedForward_writer) ? this->allpass_tilde_11_feedForward_reader != this->allpass_tilde_11_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_11_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_11_feedForward_wrap))
        );
    }
    
    void allpass_tilde_11_feedForward_write(number v) {
        this->allpass_tilde_11_feedForward_writer = this->allpass_tilde_11_feedForward_reader;
        this->allpass_tilde_11_feedForward_buffer[(Index)this->allpass_tilde_11_feedForward_writer] = v;
    }
    
    number allpass_tilde_11_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_11_feedForward__maxdelay : size);
        number val = this->allpass_tilde_11_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_11_feedForward_write(v);
        this->allpass_tilde_11_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_11_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_11_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_11_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_11_feedForward_init() {
        auto result = this->allpass_tilde_11_feedForward_calcSizeInSamples();
        this->allpass_tilde_11_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_11_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_11_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_11_feedForward_clear() {
        this->allpass_tilde_11_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_11_feedForward_reset() {
        auto result = this->allpass_tilde_11_feedForward_calcSizeInSamples();
        this->allpass_tilde_11_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_11_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_11_feedForward_buffer);
        this->allpass_tilde_11_feedForward_wrap = this->allpass_tilde_11_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_11_feedForward_clear();
    
        if (this->allpass_tilde_11_feedForward_reader >= this->allpass_tilde_11_feedForward__maxdelay || this->allpass_tilde_11_feedForward_writer >= this->allpass_tilde_11_feedForward__maxdelay) {
            this->allpass_tilde_11_feedForward_reader = 0;
            this->allpass_tilde_11_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_11_feedForward_dspsetup() {
        this->allpass_tilde_11_feedForward_reset();
    }
    
    number allpass_tilde_11_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_11_feedForward_size() {
        return this->allpass_tilde_11_feedForward__maxdelay;
    }
    
    void allpass_tilde_11_feedBack_step() {
        this->allpass_tilde_11_feedBack_reader++;
    
        if (this->allpass_tilde_11_feedBack_reader >= (int)(this->allpass_tilde_11_feedBack_buffer->getSize()))
            this->allpass_tilde_11_feedBack_reader = 0;
    }
    
    number allpass_tilde_11_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_11_feedBack_buffer->getSize()) + this->allpass_tilde_11_feedBack_reader - ((size > this->allpass_tilde_11_feedBack__maxdelay ? this->allpass_tilde_11_feedBack__maxdelay : (size < (this->allpass_tilde_11_feedBack_reader != this->allpass_tilde_11_feedBack_writer) ? this->allpass_tilde_11_feedBack_reader != this->allpass_tilde_11_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ), this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_11_feedBack_buffer->getSize()) + this->allpass_tilde_11_feedBack_reader - ((size > this->allpass_tilde_11_feedBack__maxdelay ? this->allpass_tilde_11_feedBack__maxdelay : (size < (1 + this->allpass_tilde_11_feedBack_reader != this->allpass_tilde_11_feedBack_writer) ? 1 + this->allpass_tilde_11_feedBack_reader != this->allpass_tilde_11_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ), this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ), this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ), this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_11_feedBack_buffer->getSize()) + this->allpass_tilde_11_feedBack_reader - ((size > this->allpass_tilde_11_feedBack__maxdelay ? this->allpass_tilde_11_feedBack__maxdelay : (size < (1 + this->allpass_tilde_11_feedBack_reader != this->allpass_tilde_11_feedBack_writer) ? 1 + this->allpass_tilde_11_feedBack_reader != this->allpass_tilde_11_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ), this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ), this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ), this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_11_feedBack_buffer->getSize()) + this->allpass_tilde_11_feedBack_reader - ((size > this->allpass_tilde_11_feedBack__maxdelay ? this->allpass_tilde_11_feedBack__maxdelay : (size < (this->allpass_tilde_11_feedBack_reader != this->allpass_tilde_11_feedBack_writer) ? this->allpass_tilde_11_feedBack_reader != this->allpass_tilde_11_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ), this->allpass_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_11_feedBack_buffer->getSize()) + this->allpass_tilde_11_feedBack_reader - ((size > this->allpass_tilde_11_feedBack__maxdelay ? this->allpass_tilde_11_feedBack__maxdelay : (size < (this->allpass_tilde_11_feedBack_reader != this->allpass_tilde_11_feedBack_writer) ? this->allpass_tilde_11_feedBack_reader != this->allpass_tilde_11_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_11_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_11_feedBack_wrap))
        );
    }
    
    void allpass_tilde_11_feedBack_write(number v) {
        this->allpass_tilde_11_feedBack_writer = this->allpass_tilde_11_feedBack_reader;
        this->allpass_tilde_11_feedBack_buffer[(Index)this->allpass_tilde_11_feedBack_writer] = v;
    }
    
    number allpass_tilde_11_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_11_feedBack__maxdelay : size);
        number val = this->allpass_tilde_11_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_11_feedBack_write(v);
        this->allpass_tilde_11_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_11_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_11_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_11_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_11_feedBack_init() {
        auto result = this->allpass_tilde_11_feedBack_calcSizeInSamples();
        this->allpass_tilde_11_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_11_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_11_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_11_feedBack_clear() {
        this->allpass_tilde_11_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_11_feedBack_reset() {
        auto result = this->allpass_tilde_11_feedBack_calcSizeInSamples();
        this->allpass_tilde_11_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_11_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_11_feedBack_buffer);
        this->allpass_tilde_11_feedBack_wrap = this->allpass_tilde_11_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_11_feedBack_clear();
    
        if (this->allpass_tilde_11_feedBack_reader >= this->allpass_tilde_11_feedBack__maxdelay || this->allpass_tilde_11_feedBack_writer >= this->allpass_tilde_11_feedBack__maxdelay) {
            this->allpass_tilde_11_feedBack_reader = 0;
            this->allpass_tilde_11_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_11_feedBack_dspsetup() {
        this->allpass_tilde_11_feedBack_reset();
    }
    
    number allpass_tilde_11_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_11_feedBack_size() {
        return this->allpass_tilde_11_feedBack__maxdelay;
    }
    
    void allpass_tilde_11_init() {
        this->allpass_tilde_11_feedForward_init();
        this->allpass_tilde_11_feedBack_init();
    }
    
    void allpass_tilde_11_clear() {
        this->allpass_tilde_11_feedForward_clear();
        this->allpass_tilde_11_feedBack_clear();
    }
    
    void allpass_tilde_11_reset() {
        this->allpass_tilde_11_init();
        this->allpass_tilde_11_clear();
    }
    
    void allpass_tilde_11_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_11_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_11_reset();
        this->allpass_tilde_11_setupDone = true;
        this->allpass_tilde_11_feedForward_dspsetup();
        this->allpass_tilde_11_feedBack_dspsetup();
    }
    
    void comb_tilde_09_feedForward_step() {
        this->comb_tilde_09_feedForward_reader++;
    
        if (this->comb_tilde_09_feedForward_reader >= (int)(this->comb_tilde_09_feedForward_buffer->getSize()))
            this->comb_tilde_09_feedForward_reader = 0;
    }
    
    number comb_tilde_09_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_09_feedForward_buffer->getSize()) + this->comb_tilde_09_feedForward_reader - ((size > this->comb_tilde_09_feedForward__maxdelay ? this->comb_tilde_09_feedForward__maxdelay : (size < (this->comb_tilde_09_feedForward_reader != this->comb_tilde_09_feedForward_writer) ? this->comb_tilde_09_feedForward_reader != this->comb_tilde_09_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ), this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_09_feedForward_buffer->getSize()) + this->comb_tilde_09_feedForward_reader - ((size > this->comb_tilde_09_feedForward__maxdelay ? this->comb_tilde_09_feedForward__maxdelay : (size < (1 + this->comb_tilde_09_feedForward_reader != this->comb_tilde_09_feedForward_writer) ? 1 + this->comb_tilde_09_feedForward_reader != this->comb_tilde_09_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ), this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ), this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ), this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_09_feedForward_buffer->getSize()) + this->comb_tilde_09_feedForward_reader - ((size > this->comb_tilde_09_feedForward__maxdelay ? this->comb_tilde_09_feedForward__maxdelay : (size < (1 + this->comb_tilde_09_feedForward_reader != this->comb_tilde_09_feedForward_writer) ? 1 + this->comb_tilde_09_feedForward_reader != this->comb_tilde_09_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ), this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ), this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ), this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_09_feedForward_buffer->getSize()) + this->comb_tilde_09_feedForward_reader - ((size > this->comb_tilde_09_feedForward__maxdelay ? this->comb_tilde_09_feedForward__maxdelay : (size < (this->comb_tilde_09_feedForward_reader != this->comb_tilde_09_feedForward_writer) ? this->comb_tilde_09_feedForward_reader != this->comb_tilde_09_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ), this->comb_tilde_09_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_09_feedForward_buffer->getSize()) + this->comb_tilde_09_feedForward_reader - ((size > this->comb_tilde_09_feedForward__maxdelay ? this->comb_tilde_09_feedForward__maxdelay : (size < (this->comb_tilde_09_feedForward_reader != this->comb_tilde_09_feedForward_writer) ? this->comb_tilde_09_feedForward_reader != this->comb_tilde_09_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_09_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_09_feedForward_wrap))
        );
    }
    
    void comb_tilde_09_feedForward_write(number v) {
        this->comb_tilde_09_feedForward_writer = this->comb_tilde_09_feedForward_reader;
        this->comb_tilde_09_feedForward_buffer[(Index)this->comb_tilde_09_feedForward_writer] = v;
    }
    
    number comb_tilde_09_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_09_feedForward__maxdelay : size);
        number val = this->comb_tilde_09_feedForward_read(effectiveSize, 0);
        this->comb_tilde_09_feedForward_write(v);
        this->comb_tilde_09_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_09_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_09_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_09_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_09_feedForward_init() {
        auto result = this->comb_tilde_09_feedForward_calcSizeInSamples();
        this->comb_tilde_09_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_09_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_09_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_09_feedForward_clear() {
        this->comb_tilde_09_feedForward_buffer->setZero();
    }
    
    void comb_tilde_09_feedForward_reset() {
        auto result = this->comb_tilde_09_feedForward_calcSizeInSamples();
        this->comb_tilde_09_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_09_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_09_feedForward_buffer);
        this->comb_tilde_09_feedForward_wrap = this->comb_tilde_09_feedForward_buffer->getSize() - 1;
        this->comb_tilde_09_feedForward_clear();
    
        if (this->comb_tilde_09_feedForward_reader >= this->comb_tilde_09_feedForward__maxdelay || this->comb_tilde_09_feedForward_writer >= this->comb_tilde_09_feedForward__maxdelay) {
            this->comb_tilde_09_feedForward_reader = 0;
            this->comb_tilde_09_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_09_feedForward_dspsetup() {
        this->comb_tilde_09_feedForward_reset();
    }
    
    number comb_tilde_09_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_09_feedForward_size() {
        return this->comb_tilde_09_feedForward__maxdelay;
    }
    
    void comb_tilde_09_feedBack_step() {
        this->comb_tilde_09_feedBack_reader++;
    
        if (this->comb_tilde_09_feedBack_reader >= (int)(this->comb_tilde_09_feedBack_buffer->getSize()))
            this->comb_tilde_09_feedBack_reader = 0;
    }
    
    number comb_tilde_09_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_09_feedBack_buffer->getSize()) + this->comb_tilde_09_feedBack_reader - ((size > this->comb_tilde_09_feedBack__maxdelay ? this->comb_tilde_09_feedBack__maxdelay : (size < (this->comb_tilde_09_feedBack_reader != this->comb_tilde_09_feedBack_writer) ? this->comb_tilde_09_feedBack_reader != this->comb_tilde_09_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ), this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_09_feedBack_buffer->getSize()) + this->comb_tilde_09_feedBack_reader - ((size > this->comb_tilde_09_feedBack__maxdelay ? this->comb_tilde_09_feedBack__maxdelay : (size < (1 + this->comb_tilde_09_feedBack_reader != this->comb_tilde_09_feedBack_writer) ? 1 + this->comb_tilde_09_feedBack_reader != this->comb_tilde_09_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ), this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ), this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ), this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_09_feedBack_buffer->getSize()) + this->comb_tilde_09_feedBack_reader - ((size > this->comb_tilde_09_feedBack__maxdelay ? this->comb_tilde_09_feedBack__maxdelay : (size < (1 + this->comb_tilde_09_feedBack_reader != this->comb_tilde_09_feedBack_writer) ? 1 + this->comb_tilde_09_feedBack_reader != this->comb_tilde_09_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ), this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ), this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ), this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_09_feedBack_buffer->getSize()) + this->comb_tilde_09_feedBack_reader - ((size > this->comb_tilde_09_feedBack__maxdelay ? this->comb_tilde_09_feedBack__maxdelay : (size < (this->comb_tilde_09_feedBack_reader != this->comb_tilde_09_feedBack_writer) ? this->comb_tilde_09_feedBack_reader != this->comb_tilde_09_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ), this->comb_tilde_09_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_09_feedBack_buffer->getSize()) + this->comb_tilde_09_feedBack_reader - ((size > this->comb_tilde_09_feedBack__maxdelay ? this->comb_tilde_09_feedBack__maxdelay : (size < (this->comb_tilde_09_feedBack_reader != this->comb_tilde_09_feedBack_writer) ? this->comb_tilde_09_feedBack_reader != this->comb_tilde_09_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_09_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_09_feedBack_wrap))
        );
    }
    
    void comb_tilde_09_feedBack_write(number v) {
        this->comb_tilde_09_feedBack_writer = this->comb_tilde_09_feedBack_reader;
        this->comb_tilde_09_feedBack_buffer[(Index)this->comb_tilde_09_feedBack_writer] = v;
    }
    
    number comb_tilde_09_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_09_feedBack__maxdelay : size);
        number val = this->comb_tilde_09_feedBack_read(effectiveSize, 0);
        this->comb_tilde_09_feedBack_write(v);
        this->comb_tilde_09_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_09_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_09_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_09_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_09_feedBack_init() {
        auto result = this->comb_tilde_09_feedBack_calcSizeInSamples();
        this->comb_tilde_09_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_09_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_09_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_09_feedBack_clear() {
        this->comb_tilde_09_feedBack_buffer->setZero();
    }
    
    void comb_tilde_09_feedBack_reset() {
        auto result = this->comb_tilde_09_feedBack_calcSizeInSamples();
        this->comb_tilde_09_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_09_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_09_feedBack_buffer);
        this->comb_tilde_09_feedBack_wrap = this->comb_tilde_09_feedBack_buffer->getSize() - 1;
        this->comb_tilde_09_feedBack_clear();
    
        if (this->comb_tilde_09_feedBack_reader >= this->comb_tilde_09_feedBack__maxdelay || this->comb_tilde_09_feedBack_writer >= this->comb_tilde_09_feedBack__maxdelay) {
            this->comb_tilde_09_feedBack_reader = 0;
            this->comb_tilde_09_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_09_feedBack_dspsetup() {
        this->comb_tilde_09_feedBack_reset();
    }
    
    number comb_tilde_09_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_09_feedBack_size() {
        return this->comb_tilde_09_feedBack__maxdelay;
    }
    
    void comb_tilde_09_init() {
        this->comb_tilde_09_feedForward_init();
        this->comb_tilde_09_feedBack_init();
    }
    
    void comb_tilde_09_clear() {
        this->comb_tilde_09_feedForward_clear();
        this->comb_tilde_09_feedBack_clear();
    }
    
    void comb_tilde_09_reset() {
        this->comb_tilde_09_init();
        this->comb_tilde_09_clear();
    }
    
    void comb_tilde_09_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_09_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_09_reset();
        this->comb_tilde_09_setupDone = true;
        this->comb_tilde_09_feedForward_dspsetup();
        this->comb_tilde_09_feedBack_dspsetup();
    }
    
    void allpass_tilde_12_feedForward_step() {
        this->allpass_tilde_12_feedForward_reader++;
    
        if (this->allpass_tilde_12_feedForward_reader >= (int)(this->allpass_tilde_12_feedForward_buffer->getSize()))
            this->allpass_tilde_12_feedForward_reader = 0;
    }
    
    number allpass_tilde_12_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_12_feedForward_buffer->getSize()) + this->allpass_tilde_12_feedForward_reader - ((size > this->allpass_tilde_12_feedForward__maxdelay ? this->allpass_tilde_12_feedForward__maxdelay : (size < (this->allpass_tilde_12_feedForward_reader != this->allpass_tilde_12_feedForward_writer) ? this->allpass_tilde_12_feedForward_reader != this->allpass_tilde_12_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ), this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_12_feedForward_buffer->getSize()) + this->allpass_tilde_12_feedForward_reader - ((size > this->allpass_tilde_12_feedForward__maxdelay ? this->allpass_tilde_12_feedForward__maxdelay : (size < (1 + this->allpass_tilde_12_feedForward_reader != this->allpass_tilde_12_feedForward_writer) ? 1 + this->allpass_tilde_12_feedForward_reader != this->allpass_tilde_12_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ), this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ), this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ), this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_12_feedForward_buffer->getSize()) + this->allpass_tilde_12_feedForward_reader - ((size > this->allpass_tilde_12_feedForward__maxdelay ? this->allpass_tilde_12_feedForward__maxdelay : (size < (1 + this->allpass_tilde_12_feedForward_reader != this->allpass_tilde_12_feedForward_writer) ? 1 + this->allpass_tilde_12_feedForward_reader != this->allpass_tilde_12_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ), this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ), this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ), this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_12_feedForward_buffer->getSize()) + this->allpass_tilde_12_feedForward_reader - ((size > this->allpass_tilde_12_feedForward__maxdelay ? this->allpass_tilde_12_feedForward__maxdelay : (size < (this->allpass_tilde_12_feedForward_reader != this->allpass_tilde_12_feedForward_writer) ? this->allpass_tilde_12_feedForward_reader != this->allpass_tilde_12_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ), this->allpass_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_12_feedForward_buffer->getSize()) + this->allpass_tilde_12_feedForward_reader - ((size > this->allpass_tilde_12_feedForward__maxdelay ? this->allpass_tilde_12_feedForward__maxdelay : (size < (this->allpass_tilde_12_feedForward_reader != this->allpass_tilde_12_feedForward_writer) ? this->allpass_tilde_12_feedForward_reader != this->allpass_tilde_12_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_12_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_12_feedForward_wrap))
        );
    }
    
    void allpass_tilde_12_feedForward_write(number v) {
        this->allpass_tilde_12_feedForward_writer = this->allpass_tilde_12_feedForward_reader;
        this->allpass_tilde_12_feedForward_buffer[(Index)this->allpass_tilde_12_feedForward_writer] = v;
    }
    
    number allpass_tilde_12_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_12_feedForward__maxdelay : size);
        number val = this->allpass_tilde_12_feedForward_read(effectiveSize, 0);
        this->allpass_tilde_12_feedForward_write(v);
        this->allpass_tilde_12_feedForward_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_12_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_12_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_12_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_12_feedForward_init() {
        auto result = this->allpass_tilde_12_feedForward_calcSizeInSamples();
        this->allpass_tilde_12_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_12_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_12_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_12_feedForward_clear() {
        this->allpass_tilde_12_feedForward_buffer->setZero();
    }
    
    void allpass_tilde_12_feedForward_reset() {
        auto result = this->allpass_tilde_12_feedForward_calcSizeInSamples();
        this->allpass_tilde_12_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_12_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_12_feedForward_buffer);
        this->allpass_tilde_12_feedForward_wrap = this->allpass_tilde_12_feedForward_buffer->getSize() - 1;
        this->allpass_tilde_12_feedForward_clear();
    
        if (this->allpass_tilde_12_feedForward_reader >= this->allpass_tilde_12_feedForward__maxdelay || this->allpass_tilde_12_feedForward_writer >= this->allpass_tilde_12_feedForward__maxdelay) {
            this->allpass_tilde_12_feedForward_reader = 0;
            this->allpass_tilde_12_feedForward_writer = 0;
        }
    }
    
    void allpass_tilde_12_feedForward_dspsetup() {
        this->allpass_tilde_12_feedForward_reset();
    }
    
    number allpass_tilde_12_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_12_feedForward_size() {
        return this->allpass_tilde_12_feedForward__maxdelay;
    }
    
    void allpass_tilde_12_feedBack_step() {
        this->allpass_tilde_12_feedBack_reader++;
    
        if (this->allpass_tilde_12_feedBack_reader >= (int)(this->allpass_tilde_12_feedBack_buffer->getSize()))
            this->allpass_tilde_12_feedBack_reader = 0;
    }
    
    number allpass_tilde_12_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->allpass_tilde_12_feedBack_buffer->getSize()) + this->allpass_tilde_12_feedBack_reader - ((size > this->allpass_tilde_12_feedBack__maxdelay ? this->allpass_tilde_12_feedBack__maxdelay : (size < (this->allpass_tilde_12_feedBack_reader != this->allpass_tilde_12_feedBack_writer) ? this->allpass_tilde_12_feedBack_reader != this->allpass_tilde_12_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ), this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->allpass_tilde_12_feedBack_buffer->getSize()) + this->allpass_tilde_12_feedBack_reader - ((size > this->allpass_tilde_12_feedBack__maxdelay ? this->allpass_tilde_12_feedBack__maxdelay : (size < (1 + this->allpass_tilde_12_feedBack_reader != this->allpass_tilde_12_feedBack_writer) ? 1 + this->allpass_tilde_12_feedBack_reader != this->allpass_tilde_12_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ), this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ), this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ), this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->allpass_tilde_12_feedBack_buffer->getSize()) + this->allpass_tilde_12_feedBack_reader - ((size > this->allpass_tilde_12_feedBack__maxdelay ? this->allpass_tilde_12_feedBack__maxdelay : (size < (1 + this->allpass_tilde_12_feedBack_reader != this->allpass_tilde_12_feedBack_writer) ? 1 + this->allpass_tilde_12_feedBack_reader != this->allpass_tilde_12_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ), this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ), this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ), this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->allpass_tilde_12_feedBack_buffer->getSize()) + this->allpass_tilde_12_feedBack_reader - ((size > this->allpass_tilde_12_feedBack__maxdelay ? this->allpass_tilde_12_feedBack__maxdelay : (size < (this->allpass_tilde_12_feedBack_reader != this->allpass_tilde_12_feedBack_writer) ? this->allpass_tilde_12_feedBack_reader != this->allpass_tilde_12_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ), this->allpass_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->allpass_tilde_12_feedBack_buffer->getSize()) + this->allpass_tilde_12_feedBack_reader - ((size > this->allpass_tilde_12_feedBack__maxdelay ? this->allpass_tilde_12_feedBack__maxdelay : (size < (this->allpass_tilde_12_feedBack_reader != this->allpass_tilde_12_feedBack_writer) ? this->allpass_tilde_12_feedBack_reader != this->allpass_tilde_12_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->allpass_tilde_12_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->allpass_tilde_12_feedBack_wrap))
        );
    }
    
    void allpass_tilde_12_feedBack_write(number v) {
        this->allpass_tilde_12_feedBack_writer = this->allpass_tilde_12_feedBack_reader;
        this->allpass_tilde_12_feedBack_buffer[(Index)this->allpass_tilde_12_feedBack_writer] = v;
    }
    
    number allpass_tilde_12_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->allpass_tilde_12_feedBack__maxdelay : size);
        number val = this->allpass_tilde_12_feedBack_read(effectiveSize, 0);
        this->allpass_tilde_12_feedBack_write(v);
        this->allpass_tilde_12_feedBack_step();
        return val;
    }
    
    array<Index, 2> allpass_tilde_12_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->allpass_tilde_12_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->allpass_tilde_12_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void allpass_tilde_12_feedBack_init() {
        auto result = this->allpass_tilde_12_feedBack_calcSizeInSamples();
        this->allpass_tilde_12_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_12_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->allpass_tilde_12_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void allpass_tilde_12_feedBack_clear() {
        this->allpass_tilde_12_feedBack_buffer->setZero();
    }
    
    void allpass_tilde_12_feedBack_reset() {
        auto result = this->allpass_tilde_12_feedBack_calcSizeInSamples();
        this->allpass_tilde_12_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->allpass_tilde_12_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->allpass_tilde_12_feedBack_buffer);
        this->allpass_tilde_12_feedBack_wrap = this->allpass_tilde_12_feedBack_buffer->getSize() - 1;
        this->allpass_tilde_12_feedBack_clear();
    
        if (this->allpass_tilde_12_feedBack_reader >= this->allpass_tilde_12_feedBack__maxdelay || this->allpass_tilde_12_feedBack_writer >= this->allpass_tilde_12_feedBack__maxdelay) {
            this->allpass_tilde_12_feedBack_reader = 0;
            this->allpass_tilde_12_feedBack_writer = 0;
        }
    }
    
    void allpass_tilde_12_feedBack_dspsetup() {
        this->allpass_tilde_12_feedBack_reset();
    }
    
    number allpass_tilde_12_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number allpass_tilde_12_feedBack_size() {
        return this->allpass_tilde_12_feedBack__maxdelay;
    }
    
    void allpass_tilde_12_init() {
        this->allpass_tilde_12_feedForward_init();
        this->allpass_tilde_12_feedBack_init();
    }
    
    void allpass_tilde_12_clear() {
        this->allpass_tilde_12_feedForward_clear();
        this->allpass_tilde_12_feedBack_clear();
    }
    
    void allpass_tilde_12_reset() {
        this->allpass_tilde_12_init();
        this->allpass_tilde_12_clear();
    }
    
    void allpass_tilde_12_dspsetup(bool force) {
        if ((bool)(this->allpass_tilde_12_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->allpass_tilde_12_reset();
        this->allpass_tilde_12_setupDone = true;
        this->allpass_tilde_12_feedForward_dspsetup();
        this->allpass_tilde_12_feedBack_dspsetup();
    }
    
    void onepole_tilde_04_reset() {
        this->onepole_tilde_04_lastY = 0;
        this->onepole_tilde_04_a0 = 0;
        this->onepole_tilde_04_b1 = 0;
    }
    
    void onepole_tilde_04_dspsetup(bool force) {
        if ((bool)(this->onepole_tilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->onepole_tilde_04_needsUpdate = true;
        this->onepole_tilde_04_reset();
        this->onepole_tilde_04_setupDone = true;
    }
    
    void lores_02_reset() {
        this->lores_02_y_1 = 0;
        this->lores_02_y_2 = 0;
        this->lores_02_last_resonance = 0;
        this->lores_02_last_frequency = 0;
        this->lores_02_last_res_calc = 0;
        this->lores_02_last_freq_calc = 0;
    }
    
    void lores_02_dspsetup(bool force) {
        if ((bool)(this->lores_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->lores_02_reset();
        this->lores_02_setupDone = true;
    }
    
    void noise_tilde_02_init() {
        this->noise_tilde_02_reset();
    }
    
    void noise_tilde_02_reset() {
        xoshiro_reset(
            systemticks() + this->voice() + this->random(0, 10000),
            this->noise_tilde_02_state
        );
    }
    
    void delaytilde_02_del_step() {
        this->delaytilde_02_del_reader++;
    
        if (this->delaytilde_02_del_reader >= (int)(this->delaytilde_02_del_buffer->getSize()))
            this->delaytilde_02_del_reader = 0;
    }
    
    number delaytilde_02_del_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->delaytilde_02_del_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? 1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->delaytilde_02_del_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? 1 + this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->delaytilde_02_del_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->delaytilde_02_del_wrap))
            ), this->delaytilde_02_del_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->delaytilde_02_del_wrap))
            ));
        }
    
        number r = (int)(this->delaytilde_02_del_buffer->getSize()) + this->delaytilde_02_del_reader - ((size > this->delaytilde_02_del__maxdelay ? this->delaytilde_02_del__maxdelay : (size < (this->delaytilde_02_del_reader != this->delaytilde_02_del_writer) ? this->delaytilde_02_del_reader != this->delaytilde_02_del_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->delaytilde_02_del_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->delaytilde_02_del_wrap))
        );
    }
    
    void delaytilde_02_del_write(number v) {
        this->delaytilde_02_del_writer = this->delaytilde_02_del_reader;
        this->delaytilde_02_del_buffer[(Index)this->delaytilde_02_del_writer] = v;
    }
    
    number delaytilde_02_del_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->delaytilde_02_del__maxdelay : size);
        number val = this->delaytilde_02_del_read(effectiveSize, 0);
        this->delaytilde_02_del_write(v);
        this->delaytilde_02_del_step();
        return val;
    }
    
    array<Index, 2> delaytilde_02_del_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->delaytilde_02_del_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->delaytilde_02_del_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void delaytilde_02_del_init() {
        auto result = this->delaytilde_02_del_calcSizeInSamples();
        this->delaytilde_02_del__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->delaytilde_02_del_buffer->requestSize(requestedSizeInSamples, 1);
        this->delaytilde_02_del_wrap = requestedSizeInSamples - 1;
    }
    
    void delaytilde_02_del_clear() {
        this->delaytilde_02_del_buffer->setZero();
    }
    
    void delaytilde_02_del_reset() {
        auto result = this->delaytilde_02_del_calcSizeInSamples();
        this->delaytilde_02_del__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->delaytilde_02_del_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->delaytilde_02_del_buffer);
        this->delaytilde_02_del_wrap = this->delaytilde_02_del_buffer->getSize() - 1;
        this->delaytilde_02_del_clear();
    
        if (this->delaytilde_02_del_reader >= this->delaytilde_02_del__maxdelay || this->delaytilde_02_del_writer >= this->delaytilde_02_del__maxdelay) {
            this->delaytilde_02_del_reader = 0;
            this->delaytilde_02_del_writer = 0;
        }
    }
    
    void delaytilde_02_del_dspsetup() {
        this->delaytilde_02_del_reset();
    }
    
    number delaytilde_02_del_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 38400;
    }
    
    number delaytilde_02_del_size() {
        return this->delaytilde_02_del__maxdelay;
    }
    
    void delaytilde_02_dspsetup(bool force) {
        if ((bool)(this->delaytilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->delaytilde_02_rampInSamples = (long)(this->mstosamps(0));
        this->delaytilde_02_lastDelay = -1;
        this->delaytilde_02_setupDone = true;
        this->delaytilde_02_del_dspsetup();
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
        allpass_tilde_11_x = 0;
        allpass_tilde_11_delayTime = 30.51;
        allpass_tilde_11_gain = -0.7;
        dspexpr_18_in1 = 0;
        dspexpr_18_in2 = 30.51;
        comb_tilde_09_x = 0;
        comb_tilde_09_delayTime = 141.7;
        comb_tilde_09_gain = 0;
        comb_tilde_09_ffGain = 1;
        comb_tilde_09_fbGain = 0;
        dspexpr_19_in1 = 0;
        dspexpr_19_in2 = 0;
        dspexpr_20_in1 = 0;
        dspexpr_20_in2 = 141.7;
        dspexpr_21_in1 = 0;
        dspexpr_21_in2 = 0.5;
        allpass_tilde_12_x = 0;
        allpass_tilde_12_delayTime = 89.24;
        allpass_tilde_12_gain = 0.5;
        onepole_tilde_04_x = 0;
        onepole_tilde_04_freqInHz = 1800;
        dspexpr_22_in1 = 0;
        dspexpr_22_in2 = 20;
        lores_02_cutoff = 0;
        lores_02_resonance = 0.995;
        lores_02_resonance_setter(lores_02_resonance);
        dspexpr_23_in1 = 0;
        dspexpr_23_in2 = 89.24;
        delaytilde_02_delay = 0;
        mstosamps_tilde_02_ms = 0;
        dspexpr_24_in1 = 0;
        dspexpr_24_in2 = 106.28;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        signals[4] = nullptr;
        signals[5] = nullptr;
        signals[6] = nullptr;
        signals[7] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        allpass_tilde_11_feedForward__maxdelay = 0;
        allpass_tilde_11_feedForward_sizemode = 0;
        allpass_tilde_11_feedForward_wrap = 0;
        allpass_tilde_11_feedForward_reader = 0;
        allpass_tilde_11_feedForward_writer = 0;
        allpass_tilde_11_feedBack__maxdelay = 0;
        allpass_tilde_11_feedBack_sizemode = 0;
        allpass_tilde_11_feedBack_wrap = 0;
        allpass_tilde_11_feedBack_reader = 0;
        allpass_tilde_11_feedBack_writer = 0;
        allpass_tilde_11_setupDone = false;
        comb_tilde_09_feedForward__maxdelay = 0;
        comb_tilde_09_feedForward_sizemode = 0;
        comb_tilde_09_feedForward_wrap = 0;
        comb_tilde_09_feedForward_reader = 0;
        comb_tilde_09_feedForward_writer = 0;
        comb_tilde_09_feedBack__maxdelay = 0;
        comb_tilde_09_feedBack_sizemode = 0;
        comb_tilde_09_feedBack_wrap = 0;
        comb_tilde_09_feedBack_reader = 0;
        comb_tilde_09_feedBack_writer = 0;
        comb_tilde_09_setupDone = false;
        allpass_tilde_12_feedForward__maxdelay = 0;
        allpass_tilde_12_feedForward_sizemode = 0;
        allpass_tilde_12_feedForward_wrap = 0;
        allpass_tilde_12_feedForward_reader = 0;
        allpass_tilde_12_feedForward_writer = 0;
        allpass_tilde_12_feedBack__maxdelay = 0;
        allpass_tilde_12_feedBack_sizemode = 0;
        allpass_tilde_12_feedBack_wrap = 0;
        allpass_tilde_12_feedBack_reader = 0;
        allpass_tilde_12_feedBack_writer = 0;
        allpass_tilde_12_setupDone = false;
        onepole_tilde_04_freq = 0;
        onepole_tilde_04_needsUpdate = false;
        onepole_tilde_04_lastY = 0;
        onepole_tilde_04_a0 = 0;
        onepole_tilde_04_b1 = 0;
        onepole_tilde_04_setupDone = false;
        lores_02_setupDone = false;
        delaytilde_02_lastDelay = -1;
        delaytilde_02_crossfadeDelay = 0;
        delaytilde_02_ramp = 0;
        delaytilde_02_rampInSamples = 0;
        delaytilde_02_del__maxdelay = 0;
        delaytilde_02_del_sizemode = 0;
        delaytilde_02_del_wrap = 0;
        delaytilde_02_del_reader = 0;
        delaytilde_02_del_writer = 0;
        delaytilde_02_setupDone = false;
        feedbacktilde_02_feedbackbuffer = nullptr;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number allpass_tilde_11_x;
        number allpass_tilde_11_delayTime;
        number allpass_tilde_11_gain;
        number dspexpr_18_in1;
        number dspexpr_18_in2;
        number comb_tilde_09_x;
        number comb_tilde_09_delayTime;
        number comb_tilde_09_gain;
        number comb_tilde_09_ffGain;
        number comb_tilde_09_fbGain;
        number dspexpr_19_in1;
        number dspexpr_19_in2;
        number dspexpr_20_in1;
        number dspexpr_20_in2;
        number dspexpr_21_in1;
        number dspexpr_21_in2;
        number allpass_tilde_12_x;
        number allpass_tilde_12_delayTime;
        number allpass_tilde_12_gain;
        number onepole_tilde_04_x;
        number onepole_tilde_04_freqInHz;
        number dspexpr_22_in1;
        number dspexpr_22_in2;
        number lores_02_cutoff;
        number lores_02_resonance;
        number dspexpr_23_in1;
        number dspexpr_23_in2;
        number delaytilde_02_delay;
        number mstosamps_tilde_02_ms;
        number dspexpr_24_in1;
        number dspexpr_24_in2;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[8];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float64BufferRef allpass_tilde_11_feedForward_buffer;
        Index allpass_tilde_11_feedForward__maxdelay;
        Int allpass_tilde_11_feedForward_sizemode;
        Index allpass_tilde_11_feedForward_wrap;
        Int allpass_tilde_11_feedForward_reader;
        Int allpass_tilde_11_feedForward_writer;
        Float64BufferRef allpass_tilde_11_feedBack_buffer;
        Index allpass_tilde_11_feedBack__maxdelay;
        Int allpass_tilde_11_feedBack_sizemode;
        Index allpass_tilde_11_feedBack_wrap;
        Int allpass_tilde_11_feedBack_reader;
        Int allpass_tilde_11_feedBack_writer;
        bool allpass_tilde_11_setupDone;
        Float64BufferRef comb_tilde_09_feedForward_buffer;
        Index comb_tilde_09_feedForward__maxdelay;
        Int comb_tilde_09_feedForward_sizemode;
        Index comb_tilde_09_feedForward_wrap;
        Int comb_tilde_09_feedForward_reader;
        Int comb_tilde_09_feedForward_writer;
        Float64BufferRef comb_tilde_09_feedBack_buffer;
        Index comb_tilde_09_feedBack__maxdelay;
        Int comb_tilde_09_feedBack_sizemode;
        Index comb_tilde_09_feedBack_wrap;
        Int comb_tilde_09_feedBack_reader;
        Int comb_tilde_09_feedBack_writer;
        bool comb_tilde_09_setupDone;
        Float64BufferRef allpass_tilde_12_feedForward_buffer;
        Index allpass_tilde_12_feedForward__maxdelay;
        Int allpass_tilde_12_feedForward_sizemode;
        Index allpass_tilde_12_feedForward_wrap;
        Int allpass_tilde_12_feedForward_reader;
        Int allpass_tilde_12_feedForward_writer;
        Float64BufferRef allpass_tilde_12_feedBack_buffer;
        Index allpass_tilde_12_feedBack__maxdelay;
        Int allpass_tilde_12_feedBack_sizemode;
        Index allpass_tilde_12_feedBack_wrap;
        Int allpass_tilde_12_feedBack_reader;
        Int allpass_tilde_12_feedBack_writer;
        bool allpass_tilde_12_setupDone;
        number onepole_tilde_04_freq;
        bool onepole_tilde_04_needsUpdate;
        number onepole_tilde_04_lastY;
        number onepole_tilde_04_a0;
        number onepole_tilde_04_b1;
        bool onepole_tilde_04_setupDone;
        number lores_02_b_0;
        number lores_02_a_1;
        number lores_02_a_2;
        number lores_02_x_0;
        number lores_02_y_1;
        number lores_02_y_2;
        number lores_02_last_resonance;
        number lores_02_last_frequency;
        number lores_02_last_res_calc;
        number lores_02_last_freq_calc;
        bool lores_02_setupDone;
        UInt noise_tilde_02_state[4] = { };
        number delaytilde_02_lastDelay;
        number delaytilde_02_crossfadeDelay;
        number delaytilde_02_ramp;
        long delaytilde_02_rampInSamples;
        Float64BufferRef delaytilde_02_del_buffer;
        Index delaytilde_02_del__maxdelay;
        Int delaytilde_02_del_sizemode;
        Index delaytilde_02_del_wrap;
        Int delaytilde_02_del_reader;
        Int delaytilde_02_del_writer;
        bool delaytilde_02_setupDone;
        signal feedbacktilde_02_feedbackbuffer;
        number stackprotect_count;
        DataRef allpass_tilde_11_feedForward_bufferobj;
        DataRef allpass_tilde_11_feedBack_bufferobj;
        DataRef comb_tilde_09_feedForward_bufferobj;
        DataRef comb_tilde_09_feedBack_bufferobj;
        DataRef allpass_tilde_12_feedForward_bufferobj;
        DataRef allpass_tilde_12_feedBack_bufferobj;
        DataRef delaytilde_02_del_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_24 : public PatcherInterfaceImpl {
    
    friend class plate;
    
    public:
    
    RNBOSubpatcher_24()
    {
    }
    
    ~RNBOSubpatcher_24()
    {
    }
    
    virtual plate* getPatcher() const {
        return static_cast<plate *>(_parentPatcher);
    }
    
    plate* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
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
        const SampleValue * target_Size = (numInputs >= 8 && inputs[7] ? inputs[7] : this->zeroBuffer);
        const SampleValue * target_Diff = (numInputs >= 9 && inputs[8] ? inputs[8] : this->zeroBuffer);
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
        const SampleValue * in3 = (numInputs >= 3 && inputs[2] ? inputs[2] : this->zeroBuffer);
        const SampleValue * in4 = (numInputs >= 4 && inputs[3] ? inputs[3] : this->zeroBuffer);
        const SampleValue * in5 = (numInputs >= 5 && inputs[4] ? inputs[4] : this->zeroBuffer);
        const SampleValue * in6 = (numInputs >= 6 && inputs[5] ? inputs[5] : this->zeroBuffer);
        const SampleValue * in7 = (numInputs >= 7 && inputs[6] ? inputs[6] : this->zeroBuffer);
        this->stackprotect_perform(n);
        this->signalreceive_14_perform(target_Size, this->signals[0], n);
        this->signalreceive_15_perform(target_Diff, this->signals[1], n);
        this->dspexpr_29_perform(this->signals[0], this->signals[1], this->signals[2], n);
        this->dspexpr_33_perform(this->signals[2], this->dspexpr_33_in2, this->signals[1], n);
    
        this->comb_tilde_15_perform(
            in6,
            this->signals[1],
            this->comb_tilde_15_gain,
            this->comb_tilde_15_ffGain,
            this->comb_tilde_15_fbGain,
            this->signals[0],
            n
        );
    
        this->dspexpr_32_perform(this->signals[2], this->dspexpr_32_in2, this->signals[1], n);
    
        this->comb_tilde_14_perform(
            in5,
            this->signals[1],
            this->comb_tilde_14_gain,
            this->comb_tilde_14_ffGain,
            this->comb_tilde_14_fbGain,
            this->signals[3],
            n
        );
    
        this->dspexpr_30_perform(this->signals[2], this->dspexpr_30_in2, this->signals[1], n);
    
        this->comb_tilde_13_perform(
            in4,
            this->signals[1],
            this->comb_tilde_13_gain,
            this->comb_tilde_13_ffGain,
            this->comb_tilde_13_fbGain,
            this->signals[4],
            n
        );
    
        this->dspexpr_27_perform(this->signals[2], this->dspexpr_27_in2, this->signals[1], n);
    
        this->comb_tilde_12_perform(
            in3,
            this->signals[1],
            this->comb_tilde_12_gain,
            this->comb_tilde_12_ffGain,
            this->comb_tilde_12_fbGain,
            this->signals[5],
            n
        );
    
        this->dspexpr_26_perform(this->signals[2], this->dspexpr_26_in2, this->signals[1], n);
    
        this->comb_tilde_11_perform(
            in2,
            this->signals[1],
            this->comb_tilde_11_gain,
            this->comb_tilde_11_ffGain,
            this->comb_tilde_11_fbGain,
            this->signals[6],
            n
        );
    
        this->dspexpr_25_perform(this->signals[2], this->dspexpr_25_in2, this->signals[1], n);
    
        this->comb_tilde_10_perform(
            in1,
            this->signals[1],
            this->comb_tilde_10_gain,
            this->comb_tilde_10_ffGain,
            this->comb_tilde_10_fbGain,
            this->signals[7],
            n
        );
    
        this->dspexpr_34_perform(this->signals[2], this->dspexpr_34_in2, this->signals[1], n);
    
        this->comb_tilde_16_perform(
            in7,
            this->signals[1],
            this->comb_tilde_16_gain,
            this->comb_tilde_16_ffGain,
            this->comb_tilde_16_fbGain,
            this->signals[2],
            n
        );
    
        this->signaladder_05_perform(
            this->signals[5],
            this->signals[3],
            this->signals[0],
            this->signals[2],
            this->signals[2],
            n
        );
    
        this->dspexpr_31_perform(this->signals[2], this->dspexpr_31_in2, this->signals[1], n);
    
        this->signaladder_06_perform(
            this->signals[7],
            this->signals[6],
            this->signals[4],
            this->signals[1],
            this->signals[1],
            n
        );
    
        this->dspexpr_28_perform(this->signals[1], this->dspexpr_28_in2, out1, n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 8; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
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
    
        this->comb_tilde_15_dspsetup(forceDSPSetup);
        this->comb_tilde_14_dspsetup(forceDSPSetup);
        this->comb_tilde_13_dspsetup(forceDSPSetup);
        this->comb_tilde_12_dspsetup(forceDSPSetup);
        this->comb_tilde_11_dspsetup(forceDSPSetup);
        this->comb_tilde_10_dspsetup(forceDSPSetup);
        this->comb_tilde_16_dspsetup(forceDSPSetup);
    
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
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex , ParameterValue , MillisecondTime ) {}
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        default:
            {
            return 0;
            }
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 2;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 0;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex , ParameterInfo * ) const {}
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
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
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
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
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 0:
            {
            return addressOf(this->comb_tilde_10_feedForward_bufferobj);
            break;
            }
        case 1:
            {
            return addressOf(this->comb_tilde_10_feedBack_bufferobj);
            break;
            }
        case 2:
            {
            return addressOf(this->comb_tilde_11_feedForward_bufferobj);
            break;
            }
        case 3:
            {
            return addressOf(this->comb_tilde_11_feedBack_bufferobj);
            break;
            }
        case 4:
            {
            return addressOf(this->comb_tilde_12_feedForward_bufferobj);
            break;
            }
        case 5:
            {
            return addressOf(this->comb_tilde_12_feedBack_bufferobj);
            break;
            }
        case 6:
            {
            return addressOf(this->comb_tilde_13_feedForward_bufferobj);
            break;
            }
        case 7:
            {
            return addressOf(this->comb_tilde_13_feedBack_bufferobj);
            break;
            }
        case 8:
            {
            return addressOf(this->comb_tilde_14_feedForward_bufferobj);
            break;
            }
        case 9:
            {
            return addressOf(this->comb_tilde_14_feedBack_bufferobj);
            break;
            }
        case 10:
            {
            return addressOf(this->comb_tilde_15_feedForward_bufferobj);
            break;
            }
        case 11:
            {
            return addressOf(this->comb_tilde_15_feedBack_bufferobj);
            break;
            }
        case 12:
            {
            return addressOf(this->comb_tilde_16_feedForward_bufferobj);
            break;
            }
        case 13:
            {
            return addressOf(this->comb_tilde_16_feedBack_bufferobj);
            break;
            }
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 14;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void zeroDataRef(DataRef& ref) {
        ref->setZero();
    }
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 0) {
            this->comb_tilde_10_feedForward_buffer = new Float64Buffer(this->comb_tilde_10_feedForward_bufferobj);
        }
    
        if (index == 1) {
            this->comb_tilde_10_feedBack_buffer = new Float64Buffer(this->comb_tilde_10_feedBack_bufferobj);
        }
    
        if (index == 2) {
            this->comb_tilde_11_feedForward_buffer = new Float64Buffer(this->comb_tilde_11_feedForward_bufferobj);
        }
    
        if (index == 3) {
            this->comb_tilde_11_feedBack_buffer = new Float64Buffer(this->comb_tilde_11_feedBack_bufferobj);
        }
    
        if (index == 4) {
            this->comb_tilde_12_feedForward_buffer = new Float64Buffer(this->comb_tilde_12_feedForward_bufferobj);
        }
    
        if (index == 5) {
            this->comb_tilde_12_feedBack_buffer = new Float64Buffer(this->comb_tilde_12_feedBack_bufferobj);
        }
    
        if (index == 6) {
            this->comb_tilde_13_feedForward_buffer = new Float64Buffer(this->comb_tilde_13_feedForward_bufferobj);
        }
    
        if (index == 7) {
            this->comb_tilde_13_feedBack_buffer = new Float64Buffer(this->comb_tilde_13_feedBack_bufferobj);
        }
    
        if (index == 8) {
            this->comb_tilde_14_feedForward_buffer = new Float64Buffer(this->comb_tilde_14_feedForward_bufferobj);
        }
    
        if (index == 9) {
            this->comb_tilde_14_feedBack_buffer = new Float64Buffer(this->comb_tilde_14_feedBack_bufferobj);
        }
    
        if (index == 10) {
            this->comb_tilde_15_feedForward_buffer = new Float64Buffer(this->comb_tilde_15_feedForward_bufferobj);
        }
    
        if (index == 11) {
            this->comb_tilde_15_feedBack_buffer = new Float64Buffer(this->comb_tilde_15_feedBack_bufferobj);
        }
    
        if (index == 12) {
            this->comb_tilde_16_feedForward_buffer = new Float64Buffer(this->comb_tilde_16_feedForward_bufferobj);
        }
    
        if (index == 13) {
            this->comb_tilde_16_feedBack_buffer = new Float64Buffer(this->comb_tilde_16_feedBack_bufferobj);
        }
    }
    
    void initialize() {
        this->comb_tilde_10_feedForward_bufferobj = initDataRef("comb_tilde_10_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_10_feedBack_bufferobj = initDataRef("comb_tilde_10_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_11_feedForward_bufferobj = initDataRef("comb_tilde_11_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_11_feedBack_bufferobj = initDataRef("comb_tilde_11_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_12_feedForward_bufferobj = initDataRef("comb_tilde_12_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_12_feedBack_bufferobj = initDataRef("comb_tilde_12_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_13_feedForward_bufferobj = initDataRef("comb_tilde_13_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_13_feedBack_bufferobj = initDataRef("comb_tilde_13_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_14_feedForward_bufferobj = initDataRef("comb_tilde_14_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_14_feedBack_bufferobj = initDataRef("comb_tilde_14_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_15_feedForward_bufferobj = initDataRef("comb_tilde_15_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_15_feedBack_bufferobj = initDataRef("comb_tilde_15_feedBack_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_16_feedForward_bufferobj = initDataRef("comb_tilde_16_feedForward_bufferobj", true, nullptr, "buffer~");
        this->comb_tilde_16_feedBack_bufferobj = initDataRef("comb_tilde_16_feedBack_bufferobj", true, nullptr, "buffer~");
        this->assign_defaults();
        this->setState();
        this->comb_tilde_10_feedForward_bufferobj->setIndex(0);
        this->comb_tilde_10_feedForward_buffer = new Float64Buffer(this->comb_tilde_10_feedForward_bufferobj);
        this->comb_tilde_10_feedBack_bufferobj->setIndex(1);
        this->comb_tilde_10_feedBack_buffer = new Float64Buffer(this->comb_tilde_10_feedBack_bufferobj);
        this->comb_tilde_11_feedForward_bufferobj->setIndex(2);
        this->comb_tilde_11_feedForward_buffer = new Float64Buffer(this->comb_tilde_11_feedForward_bufferobj);
        this->comb_tilde_11_feedBack_bufferobj->setIndex(3);
        this->comb_tilde_11_feedBack_buffer = new Float64Buffer(this->comb_tilde_11_feedBack_bufferobj);
        this->comb_tilde_12_feedForward_bufferobj->setIndex(4);
        this->comb_tilde_12_feedForward_buffer = new Float64Buffer(this->comb_tilde_12_feedForward_bufferobj);
        this->comb_tilde_12_feedBack_bufferobj->setIndex(5);
        this->comb_tilde_12_feedBack_buffer = new Float64Buffer(this->comb_tilde_12_feedBack_bufferobj);
        this->comb_tilde_13_feedForward_bufferobj->setIndex(6);
        this->comb_tilde_13_feedForward_buffer = new Float64Buffer(this->comb_tilde_13_feedForward_bufferobj);
        this->comb_tilde_13_feedBack_bufferobj->setIndex(7);
        this->comb_tilde_13_feedBack_buffer = new Float64Buffer(this->comb_tilde_13_feedBack_bufferobj);
        this->comb_tilde_14_feedForward_bufferobj->setIndex(8);
        this->comb_tilde_14_feedForward_buffer = new Float64Buffer(this->comb_tilde_14_feedForward_bufferobj);
        this->comb_tilde_14_feedBack_bufferobj->setIndex(9);
        this->comb_tilde_14_feedBack_buffer = new Float64Buffer(this->comb_tilde_14_feedBack_bufferobj);
        this->comb_tilde_15_feedForward_bufferobj->setIndex(10);
        this->comb_tilde_15_feedForward_buffer = new Float64Buffer(this->comb_tilde_15_feedForward_bufferobj);
        this->comb_tilde_15_feedBack_bufferobj->setIndex(11);
        this->comb_tilde_15_feedBack_buffer = new Float64Buffer(this->comb_tilde_15_feedBack_bufferobj);
        this->comb_tilde_16_feedForward_bufferobj->setIndex(12);
        this->comb_tilde_16_feedForward_buffer = new Float64Buffer(this->comb_tilde_16_feedForward_bufferobj);
        this->comb_tilde_16_feedBack_bufferobj->setIndex(13);
        this->comb_tilde_16_feedBack_buffer = new Float64Buffer(this->comb_tilde_16_feedBack_bufferobj);
    }
    
    protected:
    
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
        return 7;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->comb_tilde_10_feedForward_init();
        this->comb_tilde_10_feedBack_init();
        this->comb_tilde_10_init();
        this->comb_tilde_11_feedForward_init();
        this->comb_tilde_11_feedBack_init();
        this->comb_tilde_11_init();
        this->comb_tilde_12_feedForward_init();
        this->comb_tilde_12_feedBack_init();
        this->comb_tilde_12_init();
        this->comb_tilde_13_feedForward_init();
        this->comb_tilde_13_feedBack_init();
        this->comb_tilde_13_init();
        this->comb_tilde_14_feedForward_init();
        this->comb_tilde_14_feedBack_init();
        this->comb_tilde_14_init();
        this->comb_tilde_15_feedForward_init();
        this->comb_tilde_15_feedBack_init();
        this->comb_tilde_15_init();
        this->comb_tilde_16_feedForward_init();
        this->comb_tilde_16_feedBack_init();
        this->comb_tilde_16_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->comb_tilde_10_feedForward_buffer = this->comb_tilde_10_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_10_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_10_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_10_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(0);
        }
    
        this->comb_tilde_10_feedBack_buffer = this->comb_tilde_10_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_10_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_10_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_10_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    
        this->comb_tilde_11_feedForward_buffer = this->comb_tilde_11_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_11_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_11_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_11_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(2);
        }
    
        this->comb_tilde_11_feedBack_buffer = this->comb_tilde_11_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_11_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_11_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_11_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(3);
        }
    
        this->comb_tilde_12_feedForward_buffer = this->comb_tilde_12_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_12_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_12_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_12_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(4);
        }
    
        this->comb_tilde_12_feedBack_buffer = this->comb_tilde_12_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_12_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_12_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_12_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(5);
        }
    
        this->comb_tilde_13_feedForward_buffer = this->comb_tilde_13_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_13_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_13_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_13_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(6);
        }
    
        this->comb_tilde_13_feedBack_buffer = this->comb_tilde_13_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_13_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_13_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_13_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(7);
        }
    
        this->comb_tilde_14_feedForward_buffer = this->comb_tilde_14_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_14_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_14_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_14_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(8);
        }
    
        this->comb_tilde_14_feedBack_buffer = this->comb_tilde_14_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_14_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_14_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_14_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(9);
        }
    
        this->comb_tilde_15_feedForward_buffer = this->comb_tilde_15_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_15_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_15_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_15_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(10);
        }
    
        this->comb_tilde_15_feedBack_buffer = this->comb_tilde_15_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_15_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_15_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_15_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(11);
        }
    
        this->comb_tilde_16_feedForward_buffer = this->comb_tilde_16_feedForward_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_16_feedForward_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_16_feedForward_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_16_feedForward_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(12);
        }
    
        this->comb_tilde_16_feedBack_buffer = this->comb_tilde_16_feedBack_buffer->allocateIfNeeded();
    
        if (this->comb_tilde_16_feedBack_bufferobj->hasRequestedSize()) {
            if (this->comb_tilde_16_feedBack_bufferobj->wantsFill())
                this->zeroDataRef(this->comb_tilde_16_feedBack_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(13);
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void signalreceive_14_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void signalreceive_15_perform(const SampleValue * receive, SampleValue * output, Index n) {
        for (Index i = 0; i < n; i++) {
            output[(Index)i] = receive[(Index)i];
        }
    }
    
    void dspexpr_29_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_33_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 27;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_15_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_15_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_15_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_15_feedForward_write(x[(Index)i]);
            this->comb_tilde_15_feedBack_write(ov);
            this->comb_tilde_15_feedForward_step();
            this->comb_tilde_15_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_32_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 35;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_14_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_14_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_14_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_14_feedForward_write(x[(Index)i]);
            this->comb_tilde_14_feedBack_write(ov);
            this->comb_tilde_14_feedForward_step();
            this->comb_tilde_14_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_30_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 3;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_13_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_13_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_13_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_13_feedForward_write(x[(Index)i]);
            this->comb_tilde_13_feedBack_write(ov);
            this->comb_tilde_13_feedForward_step();
            this->comb_tilde_13_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_27_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 55;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_12_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_12_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_12_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_12_feedForward_write(x[(Index)i]);
            this->comb_tilde_12_feedBack_write(ov);
            this->comb_tilde_12_feedForward_step();
            this->comb_tilde_12_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_26_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 135;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_11_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_11_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_11_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_11_feedForward_write(x[(Index)i]);
            this->comb_tilde_11_feedBack_write(ov);
            this->comb_tilde_11_feedForward_step();
            this->comb_tilde_11_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_25_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 110;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_10_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_10_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_10_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_10_feedForward_write(x[(Index)i]);
            this->comb_tilde_10_feedBack_write(ov);
            this->comb_tilde_10_feedForward_step();
            this->comb_tilde_10_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void dspexpr_34_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 52;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_16_perform(
        const Sample * x,
        const Sample * delayTime,
        number gain,
        number ffGain,
        number fbGain,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(fbGain);
        RNBO_UNUSED(ffGain);
        RNBO_UNUSED(gain);
        Index i;
    
        for (i = 0; i < n; i++) {
            number __fbGain = 0;
            number __ffGain = 1;
            number __gain = 0;
            number __delayTime = delayTime[(Index)i];
            __delayTime = this->mstosamps(__delayTime);
            __delayTime = (__delayTime > this->sr ? this->sr : (__delayTime < 0 ? 0 : __delayTime));
            __gain = (__gain > 1 ? 1 : (__gain < 0 ? 0 : __gain));
            __ffGain = (__ffGain > 0.99 ? 0.99 : (__ffGain < 0 ? 0 : __ffGain));
            __fbGain = (__fbGain > 0.99 ? 0.99 : (__fbGain < 0 ? 0 : __fbGain));
            number tmp = x[(Index)i] * __gain;
            number ff = this->comb_tilde_16_feedForward_read(__delayTime, 0);
            number fb = this->comb_tilde_16_feedBack_read(__delayTime, 0);
            number ov = tmp + ff * __ffGain + fb * __fbGain;
            fixdenorm(ov);
            this->comb_tilde_16_feedForward_write(x[(Index)i]);
            this->comb_tilde_16_feedBack_write(ov);
            this->comb_tilde_16_feedForward_step();
            this->comb_tilde_16_feedBack_step();
            out1[(Index)i] = ov;
        }
    }
    
    void signaladder_05_perform(
        const SampleValue * in1,
        const SampleValue * in2,
        const SampleValue * in3,
        const SampleValue * in4,
        SampleValue * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out[(Index)i] = in1[(Index)i] + in2[(Index)i] + in3[(Index)i] + in4[(Index)i];
        }
    }
    
    void dspexpr_31_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * -1;//#map:_###_obj_###_:1
        }
    }
    
    void signaladder_06_perform(
        const SampleValue * in1,
        const SampleValue * in2,
        const SampleValue * in3,
        const SampleValue * in4,
        SampleValue * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out[(Index)i] = in1[(Index)i] + in2[(Index)i] + in3[(Index)i] + in4[(Index)i];
        }
    }
    
    void dspexpr_28_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 0.355;//#map:_###_obj_###_:1
        }
    }
    
    void comb_tilde_10_feedForward_step() {
        this->comb_tilde_10_feedForward_reader++;
    
        if (this->comb_tilde_10_feedForward_reader >= (int)(this->comb_tilde_10_feedForward_buffer->getSize()))
            this->comb_tilde_10_feedForward_reader = 0;
    }
    
    number comb_tilde_10_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_10_feedForward_buffer->getSize()) + this->comb_tilde_10_feedForward_reader - ((size > this->comb_tilde_10_feedForward__maxdelay ? this->comb_tilde_10_feedForward__maxdelay : (size < (this->comb_tilde_10_feedForward_reader != this->comb_tilde_10_feedForward_writer) ? this->comb_tilde_10_feedForward_reader != this->comb_tilde_10_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ), this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_10_feedForward_buffer->getSize()) + this->comb_tilde_10_feedForward_reader - ((size > this->comb_tilde_10_feedForward__maxdelay ? this->comb_tilde_10_feedForward__maxdelay : (size < (1 + this->comb_tilde_10_feedForward_reader != this->comb_tilde_10_feedForward_writer) ? 1 + this->comb_tilde_10_feedForward_reader != this->comb_tilde_10_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ), this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ), this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ), this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_10_feedForward_buffer->getSize()) + this->comb_tilde_10_feedForward_reader - ((size > this->comb_tilde_10_feedForward__maxdelay ? this->comb_tilde_10_feedForward__maxdelay : (size < (1 + this->comb_tilde_10_feedForward_reader != this->comb_tilde_10_feedForward_writer) ? 1 + this->comb_tilde_10_feedForward_reader != this->comb_tilde_10_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ), this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ), this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ), this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_10_feedForward_buffer->getSize()) + this->comb_tilde_10_feedForward_reader - ((size > this->comb_tilde_10_feedForward__maxdelay ? this->comb_tilde_10_feedForward__maxdelay : (size < (this->comb_tilde_10_feedForward_reader != this->comb_tilde_10_feedForward_writer) ? this->comb_tilde_10_feedForward_reader != this->comb_tilde_10_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ), this->comb_tilde_10_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_10_feedForward_buffer->getSize()) + this->comb_tilde_10_feedForward_reader - ((size > this->comb_tilde_10_feedForward__maxdelay ? this->comb_tilde_10_feedForward__maxdelay : (size < (this->comb_tilde_10_feedForward_reader != this->comb_tilde_10_feedForward_writer) ? this->comb_tilde_10_feedForward_reader != this->comb_tilde_10_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_10_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_10_feedForward_wrap))
        );
    }
    
    void comb_tilde_10_feedForward_write(number v) {
        this->comb_tilde_10_feedForward_writer = this->comb_tilde_10_feedForward_reader;
        this->comb_tilde_10_feedForward_buffer[(Index)this->comb_tilde_10_feedForward_writer] = v;
    }
    
    number comb_tilde_10_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_10_feedForward__maxdelay : size);
        number val = this->comb_tilde_10_feedForward_read(effectiveSize, 0);
        this->comb_tilde_10_feedForward_write(v);
        this->comb_tilde_10_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_10_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_10_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_10_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_10_feedForward_init() {
        auto result = this->comb_tilde_10_feedForward_calcSizeInSamples();
        this->comb_tilde_10_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_10_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_10_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_10_feedForward_clear() {
        this->comb_tilde_10_feedForward_buffer->setZero();
    }
    
    void comb_tilde_10_feedForward_reset() {
        auto result = this->comb_tilde_10_feedForward_calcSizeInSamples();
        this->comb_tilde_10_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_10_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_10_feedForward_buffer);
        this->comb_tilde_10_feedForward_wrap = this->comb_tilde_10_feedForward_buffer->getSize() - 1;
        this->comb_tilde_10_feedForward_clear();
    
        if (this->comb_tilde_10_feedForward_reader >= this->comb_tilde_10_feedForward__maxdelay || this->comb_tilde_10_feedForward_writer >= this->comb_tilde_10_feedForward__maxdelay) {
            this->comb_tilde_10_feedForward_reader = 0;
            this->comb_tilde_10_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_10_feedForward_dspsetup() {
        this->comb_tilde_10_feedForward_reset();
    }
    
    number comb_tilde_10_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_10_feedForward_size() {
        return this->comb_tilde_10_feedForward__maxdelay;
    }
    
    void comb_tilde_10_feedBack_step() {
        this->comb_tilde_10_feedBack_reader++;
    
        if (this->comb_tilde_10_feedBack_reader >= (int)(this->comb_tilde_10_feedBack_buffer->getSize()))
            this->comb_tilde_10_feedBack_reader = 0;
    }
    
    number comb_tilde_10_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_10_feedBack_buffer->getSize()) + this->comb_tilde_10_feedBack_reader - ((size > this->comb_tilde_10_feedBack__maxdelay ? this->comb_tilde_10_feedBack__maxdelay : (size < (this->comb_tilde_10_feedBack_reader != this->comb_tilde_10_feedBack_writer) ? this->comb_tilde_10_feedBack_reader != this->comb_tilde_10_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ), this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_10_feedBack_buffer->getSize()) + this->comb_tilde_10_feedBack_reader - ((size > this->comb_tilde_10_feedBack__maxdelay ? this->comb_tilde_10_feedBack__maxdelay : (size < (1 + this->comb_tilde_10_feedBack_reader != this->comb_tilde_10_feedBack_writer) ? 1 + this->comb_tilde_10_feedBack_reader != this->comb_tilde_10_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ), this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ), this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ), this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_10_feedBack_buffer->getSize()) + this->comb_tilde_10_feedBack_reader - ((size > this->comb_tilde_10_feedBack__maxdelay ? this->comb_tilde_10_feedBack__maxdelay : (size < (1 + this->comb_tilde_10_feedBack_reader != this->comb_tilde_10_feedBack_writer) ? 1 + this->comb_tilde_10_feedBack_reader != this->comb_tilde_10_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ), this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ), this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ), this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_10_feedBack_buffer->getSize()) + this->comb_tilde_10_feedBack_reader - ((size > this->comb_tilde_10_feedBack__maxdelay ? this->comb_tilde_10_feedBack__maxdelay : (size < (this->comb_tilde_10_feedBack_reader != this->comb_tilde_10_feedBack_writer) ? this->comb_tilde_10_feedBack_reader != this->comb_tilde_10_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ), this->comb_tilde_10_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_10_feedBack_buffer->getSize()) + this->comb_tilde_10_feedBack_reader - ((size > this->comb_tilde_10_feedBack__maxdelay ? this->comb_tilde_10_feedBack__maxdelay : (size < (this->comb_tilde_10_feedBack_reader != this->comb_tilde_10_feedBack_writer) ? this->comb_tilde_10_feedBack_reader != this->comb_tilde_10_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_10_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_10_feedBack_wrap))
        );
    }
    
    void comb_tilde_10_feedBack_write(number v) {
        this->comb_tilde_10_feedBack_writer = this->comb_tilde_10_feedBack_reader;
        this->comb_tilde_10_feedBack_buffer[(Index)this->comb_tilde_10_feedBack_writer] = v;
    }
    
    number comb_tilde_10_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_10_feedBack__maxdelay : size);
        number val = this->comb_tilde_10_feedBack_read(effectiveSize, 0);
        this->comb_tilde_10_feedBack_write(v);
        this->comb_tilde_10_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_10_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_10_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_10_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_10_feedBack_init() {
        auto result = this->comb_tilde_10_feedBack_calcSizeInSamples();
        this->comb_tilde_10_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_10_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_10_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_10_feedBack_clear() {
        this->comb_tilde_10_feedBack_buffer->setZero();
    }
    
    void comb_tilde_10_feedBack_reset() {
        auto result = this->comb_tilde_10_feedBack_calcSizeInSamples();
        this->comb_tilde_10_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_10_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_10_feedBack_buffer);
        this->comb_tilde_10_feedBack_wrap = this->comb_tilde_10_feedBack_buffer->getSize() - 1;
        this->comb_tilde_10_feedBack_clear();
    
        if (this->comb_tilde_10_feedBack_reader >= this->comb_tilde_10_feedBack__maxdelay || this->comb_tilde_10_feedBack_writer >= this->comb_tilde_10_feedBack__maxdelay) {
            this->comb_tilde_10_feedBack_reader = 0;
            this->comb_tilde_10_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_10_feedBack_dspsetup() {
        this->comb_tilde_10_feedBack_reset();
    }
    
    number comb_tilde_10_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_10_feedBack_size() {
        return this->comb_tilde_10_feedBack__maxdelay;
    }
    
    void comb_tilde_10_init() {
        this->comb_tilde_10_feedForward_init();
        this->comb_tilde_10_feedBack_init();
    }
    
    void comb_tilde_10_clear() {
        this->comb_tilde_10_feedForward_clear();
        this->comb_tilde_10_feedBack_clear();
    }
    
    void comb_tilde_10_reset() {
        this->comb_tilde_10_init();
        this->comb_tilde_10_clear();
    }
    
    void comb_tilde_10_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_10_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_10_reset();
        this->comb_tilde_10_setupDone = true;
        this->comb_tilde_10_feedForward_dspsetup();
        this->comb_tilde_10_feedBack_dspsetup();
    }
    
    void comb_tilde_11_feedForward_step() {
        this->comb_tilde_11_feedForward_reader++;
    
        if (this->comb_tilde_11_feedForward_reader >= (int)(this->comb_tilde_11_feedForward_buffer->getSize()))
            this->comb_tilde_11_feedForward_reader = 0;
    }
    
    number comb_tilde_11_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_11_feedForward_buffer->getSize()) + this->comb_tilde_11_feedForward_reader - ((size > this->comb_tilde_11_feedForward__maxdelay ? this->comb_tilde_11_feedForward__maxdelay : (size < (this->comb_tilde_11_feedForward_reader != this->comb_tilde_11_feedForward_writer) ? this->comb_tilde_11_feedForward_reader != this->comb_tilde_11_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ), this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_11_feedForward_buffer->getSize()) + this->comb_tilde_11_feedForward_reader - ((size > this->comb_tilde_11_feedForward__maxdelay ? this->comb_tilde_11_feedForward__maxdelay : (size < (1 + this->comb_tilde_11_feedForward_reader != this->comb_tilde_11_feedForward_writer) ? 1 + this->comb_tilde_11_feedForward_reader != this->comb_tilde_11_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ), this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ), this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ), this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_11_feedForward_buffer->getSize()) + this->comb_tilde_11_feedForward_reader - ((size > this->comb_tilde_11_feedForward__maxdelay ? this->comb_tilde_11_feedForward__maxdelay : (size < (1 + this->comb_tilde_11_feedForward_reader != this->comb_tilde_11_feedForward_writer) ? 1 + this->comb_tilde_11_feedForward_reader != this->comb_tilde_11_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ), this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ), this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ), this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_11_feedForward_buffer->getSize()) + this->comb_tilde_11_feedForward_reader - ((size > this->comb_tilde_11_feedForward__maxdelay ? this->comb_tilde_11_feedForward__maxdelay : (size < (this->comb_tilde_11_feedForward_reader != this->comb_tilde_11_feedForward_writer) ? this->comb_tilde_11_feedForward_reader != this->comb_tilde_11_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ), this->comb_tilde_11_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_11_feedForward_buffer->getSize()) + this->comb_tilde_11_feedForward_reader - ((size > this->comb_tilde_11_feedForward__maxdelay ? this->comb_tilde_11_feedForward__maxdelay : (size < (this->comb_tilde_11_feedForward_reader != this->comb_tilde_11_feedForward_writer) ? this->comb_tilde_11_feedForward_reader != this->comb_tilde_11_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_11_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_11_feedForward_wrap))
        );
    }
    
    void comb_tilde_11_feedForward_write(number v) {
        this->comb_tilde_11_feedForward_writer = this->comb_tilde_11_feedForward_reader;
        this->comb_tilde_11_feedForward_buffer[(Index)this->comb_tilde_11_feedForward_writer] = v;
    }
    
    number comb_tilde_11_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_11_feedForward__maxdelay : size);
        number val = this->comb_tilde_11_feedForward_read(effectiveSize, 0);
        this->comb_tilde_11_feedForward_write(v);
        this->comb_tilde_11_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_11_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_11_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_11_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_11_feedForward_init() {
        auto result = this->comb_tilde_11_feedForward_calcSizeInSamples();
        this->comb_tilde_11_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_11_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_11_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_11_feedForward_clear() {
        this->comb_tilde_11_feedForward_buffer->setZero();
    }
    
    void comb_tilde_11_feedForward_reset() {
        auto result = this->comb_tilde_11_feedForward_calcSizeInSamples();
        this->comb_tilde_11_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_11_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_11_feedForward_buffer);
        this->comb_tilde_11_feedForward_wrap = this->comb_tilde_11_feedForward_buffer->getSize() - 1;
        this->comb_tilde_11_feedForward_clear();
    
        if (this->comb_tilde_11_feedForward_reader >= this->comb_tilde_11_feedForward__maxdelay || this->comb_tilde_11_feedForward_writer >= this->comb_tilde_11_feedForward__maxdelay) {
            this->comb_tilde_11_feedForward_reader = 0;
            this->comb_tilde_11_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_11_feedForward_dspsetup() {
        this->comb_tilde_11_feedForward_reset();
    }
    
    number comb_tilde_11_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_11_feedForward_size() {
        return this->comb_tilde_11_feedForward__maxdelay;
    }
    
    void comb_tilde_11_feedBack_step() {
        this->comb_tilde_11_feedBack_reader++;
    
        if (this->comb_tilde_11_feedBack_reader >= (int)(this->comb_tilde_11_feedBack_buffer->getSize()))
            this->comb_tilde_11_feedBack_reader = 0;
    }
    
    number comb_tilde_11_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_11_feedBack_buffer->getSize()) + this->comb_tilde_11_feedBack_reader - ((size > this->comb_tilde_11_feedBack__maxdelay ? this->comb_tilde_11_feedBack__maxdelay : (size < (this->comb_tilde_11_feedBack_reader != this->comb_tilde_11_feedBack_writer) ? this->comb_tilde_11_feedBack_reader != this->comb_tilde_11_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ), this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_11_feedBack_buffer->getSize()) + this->comb_tilde_11_feedBack_reader - ((size > this->comb_tilde_11_feedBack__maxdelay ? this->comb_tilde_11_feedBack__maxdelay : (size < (1 + this->comb_tilde_11_feedBack_reader != this->comb_tilde_11_feedBack_writer) ? 1 + this->comb_tilde_11_feedBack_reader != this->comb_tilde_11_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ), this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ), this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ), this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_11_feedBack_buffer->getSize()) + this->comb_tilde_11_feedBack_reader - ((size > this->comb_tilde_11_feedBack__maxdelay ? this->comb_tilde_11_feedBack__maxdelay : (size < (1 + this->comb_tilde_11_feedBack_reader != this->comb_tilde_11_feedBack_writer) ? 1 + this->comb_tilde_11_feedBack_reader != this->comb_tilde_11_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ), this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ), this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ), this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_11_feedBack_buffer->getSize()) + this->comb_tilde_11_feedBack_reader - ((size > this->comb_tilde_11_feedBack__maxdelay ? this->comb_tilde_11_feedBack__maxdelay : (size < (this->comb_tilde_11_feedBack_reader != this->comb_tilde_11_feedBack_writer) ? this->comb_tilde_11_feedBack_reader != this->comb_tilde_11_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ), this->comb_tilde_11_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_11_feedBack_buffer->getSize()) + this->comb_tilde_11_feedBack_reader - ((size > this->comb_tilde_11_feedBack__maxdelay ? this->comb_tilde_11_feedBack__maxdelay : (size < (this->comb_tilde_11_feedBack_reader != this->comb_tilde_11_feedBack_writer) ? this->comb_tilde_11_feedBack_reader != this->comb_tilde_11_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_11_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_11_feedBack_wrap))
        );
    }
    
    void comb_tilde_11_feedBack_write(number v) {
        this->comb_tilde_11_feedBack_writer = this->comb_tilde_11_feedBack_reader;
        this->comb_tilde_11_feedBack_buffer[(Index)this->comb_tilde_11_feedBack_writer] = v;
    }
    
    number comb_tilde_11_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_11_feedBack__maxdelay : size);
        number val = this->comb_tilde_11_feedBack_read(effectiveSize, 0);
        this->comb_tilde_11_feedBack_write(v);
        this->comb_tilde_11_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_11_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_11_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_11_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_11_feedBack_init() {
        auto result = this->comb_tilde_11_feedBack_calcSizeInSamples();
        this->comb_tilde_11_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_11_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_11_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_11_feedBack_clear() {
        this->comb_tilde_11_feedBack_buffer->setZero();
    }
    
    void comb_tilde_11_feedBack_reset() {
        auto result = this->comb_tilde_11_feedBack_calcSizeInSamples();
        this->comb_tilde_11_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_11_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_11_feedBack_buffer);
        this->comb_tilde_11_feedBack_wrap = this->comb_tilde_11_feedBack_buffer->getSize() - 1;
        this->comb_tilde_11_feedBack_clear();
    
        if (this->comb_tilde_11_feedBack_reader >= this->comb_tilde_11_feedBack__maxdelay || this->comb_tilde_11_feedBack_writer >= this->comb_tilde_11_feedBack__maxdelay) {
            this->comb_tilde_11_feedBack_reader = 0;
            this->comb_tilde_11_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_11_feedBack_dspsetup() {
        this->comb_tilde_11_feedBack_reset();
    }
    
    number comb_tilde_11_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_11_feedBack_size() {
        return this->comb_tilde_11_feedBack__maxdelay;
    }
    
    void comb_tilde_11_init() {
        this->comb_tilde_11_feedForward_init();
        this->comb_tilde_11_feedBack_init();
    }
    
    void comb_tilde_11_clear() {
        this->comb_tilde_11_feedForward_clear();
        this->comb_tilde_11_feedBack_clear();
    }
    
    void comb_tilde_11_reset() {
        this->comb_tilde_11_init();
        this->comb_tilde_11_clear();
    }
    
    void comb_tilde_11_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_11_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_11_reset();
        this->comb_tilde_11_setupDone = true;
        this->comb_tilde_11_feedForward_dspsetup();
        this->comb_tilde_11_feedBack_dspsetup();
    }
    
    void comb_tilde_12_feedForward_step() {
        this->comb_tilde_12_feedForward_reader++;
    
        if (this->comb_tilde_12_feedForward_reader >= (int)(this->comb_tilde_12_feedForward_buffer->getSize()))
            this->comb_tilde_12_feedForward_reader = 0;
    }
    
    number comb_tilde_12_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_12_feedForward_buffer->getSize()) + this->comb_tilde_12_feedForward_reader - ((size > this->comb_tilde_12_feedForward__maxdelay ? this->comb_tilde_12_feedForward__maxdelay : (size < (this->comb_tilde_12_feedForward_reader != this->comb_tilde_12_feedForward_writer) ? this->comb_tilde_12_feedForward_reader != this->comb_tilde_12_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ), this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_12_feedForward_buffer->getSize()) + this->comb_tilde_12_feedForward_reader - ((size > this->comb_tilde_12_feedForward__maxdelay ? this->comb_tilde_12_feedForward__maxdelay : (size < (1 + this->comb_tilde_12_feedForward_reader != this->comb_tilde_12_feedForward_writer) ? 1 + this->comb_tilde_12_feedForward_reader != this->comb_tilde_12_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ), this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ), this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ), this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_12_feedForward_buffer->getSize()) + this->comb_tilde_12_feedForward_reader - ((size > this->comb_tilde_12_feedForward__maxdelay ? this->comb_tilde_12_feedForward__maxdelay : (size < (1 + this->comb_tilde_12_feedForward_reader != this->comb_tilde_12_feedForward_writer) ? 1 + this->comb_tilde_12_feedForward_reader != this->comb_tilde_12_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ), this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ), this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ), this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_12_feedForward_buffer->getSize()) + this->comb_tilde_12_feedForward_reader - ((size > this->comb_tilde_12_feedForward__maxdelay ? this->comb_tilde_12_feedForward__maxdelay : (size < (this->comb_tilde_12_feedForward_reader != this->comb_tilde_12_feedForward_writer) ? this->comb_tilde_12_feedForward_reader != this->comb_tilde_12_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ), this->comb_tilde_12_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_12_feedForward_buffer->getSize()) + this->comb_tilde_12_feedForward_reader - ((size > this->comb_tilde_12_feedForward__maxdelay ? this->comb_tilde_12_feedForward__maxdelay : (size < (this->comb_tilde_12_feedForward_reader != this->comb_tilde_12_feedForward_writer) ? this->comb_tilde_12_feedForward_reader != this->comb_tilde_12_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_12_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_12_feedForward_wrap))
        );
    }
    
    void comb_tilde_12_feedForward_write(number v) {
        this->comb_tilde_12_feedForward_writer = this->comb_tilde_12_feedForward_reader;
        this->comb_tilde_12_feedForward_buffer[(Index)this->comb_tilde_12_feedForward_writer] = v;
    }
    
    number comb_tilde_12_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_12_feedForward__maxdelay : size);
        number val = this->comb_tilde_12_feedForward_read(effectiveSize, 0);
        this->comb_tilde_12_feedForward_write(v);
        this->comb_tilde_12_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_12_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_12_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_12_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_12_feedForward_init() {
        auto result = this->comb_tilde_12_feedForward_calcSizeInSamples();
        this->comb_tilde_12_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_12_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_12_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_12_feedForward_clear() {
        this->comb_tilde_12_feedForward_buffer->setZero();
    }
    
    void comb_tilde_12_feedForward_reset() {
        auto result = this->comb_tilde_12_feedForward_calcSizeInSamples();
        this->comb_tilde_12_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_12_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_12_feedForward_buffer);
        this->comb_tilde_12_feedForward_wrap = this->comb_tilde_12_feedForward_buffer->getSize() - 1;
        this->comb_tilde_12_feedForward_clear();
    
        if (this->comb_tilde_12_feedForward_reader >= this->comb_tilde_12_feedForward__maxdelay || this->comb_tilde_12_feedForward_writer >= this->comb_tilde_12_feedForward__maxdelay) {
            this->comb_tilde_12_feedForward_reader = 0;
            this->comb_tilde_12_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_12_feedForward_dspsetup() {
        this->comb_tilde_12_feedForward_reset();
    }
    
    number comb_tilde_12_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_12_feedForward_size() {
        return this->comb_tilde_12_feedForward__maxdelay;
    }
    
    void comb_tilde_12_feedBack_step() {
        this->comb_tilde_12_feedBack_reader++;
    
        if (this->comb_tilde_12_feedBack_reader >= (int)(this->comb_tilde_12_feedBack_buffer->getSize()))
            this->comb_tilde_12_feedBack_reader = 0;
    }
    
    number comb_tilde_12_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_12_feedBack_buffer->getSize()) + this->comb_tilde_12_feedBack_reader - ((size > this->comb_tilde_12_feedBack__maxdelay ? this->comb_tilde_12_feedBack__maxdelay : (size < (this->comb_tilde_12_feedBack_reader != this->comb_tilde_12_feedBack_writer) ? this->comb_tilde_12_feedBack_reader != this->comb_tilde_12_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ), this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_12_feedBack_buffer->getSize()) + this->comb_tilde_12_feedBack_reader - ((size > this->comb_tilde_12_feedBack__maxdelay ? this->comb_tilde_12_feedBack__maxdelay : (size < (1 + this->comb_tilde_12_feedBack_reader != this->comb_tilde_12_feedBack_writer) ? 1 + this->comb_tilde_12_feedBack_reader != this->comb_tilde_12_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ), this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ), this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ), this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_12_feedBack_buffer->getSize()) + this->comb_tilde_12_feedBack_reader - ((size > this->comb_tilde_12_feedBack__maxdelay ? this->comb_tilde_12_feedBack__maxdelay : (size < (1 + this->comb_tilde_12_feedBack_reader != this->comb_tilde_12_feedBack_writer) ? 1 + this->comb_tilde_12_feedBack_reader != this->comb_tilde_12_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ), this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ), this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ), this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_12_feedBack_buffer->getSize()) + this->comb_tilde_12_feedBack_reader - ((size > this->comb_tilde_12_feedBack__maxdelay ? this->comb_tilde_12_feedBack__maxdelay : (size < (this->comb_tilde_12_feedBack_reader != this->comb_tilde_12_feedBack_writer) ? this->comb_tilde_12_feedBack_reader != this->comb_tilde_12_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ), this->comb_tilde_12_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_12_feedBack_buffer->getSize()) + this->comb_tilde_12_feedBack_reader - ((size > this->comb_tilde_12_feedBack__maxdelay ? this->comb_tilde_12_feedBack__maxdelay : (size < (this->comb_tilde_12_feedBack_reader != this->comb_tilde_12_feedBack_writer) ? this->comb_tilde_12_feedBack_reader != this->comb_tilde_12_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_12_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_12_feedBack_wrap))
        );
    }
    
    void comb_tilde_12_feedBack_write(number v) {
        this->comb_tilde_12_feedBack_writer = this->comb_tilde_12_feedBack_reader;
        this->comb_tilde_12_feedBack_buffer[(Index)this->comb_tilde_12_feedBack_writer] = v;
    }
    
    number comb_tilde_12_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_12_feedBack__maxdelay : size);
        number val = this->comb_tilde_12_feedBack_read(effectiveSize, 0);
        this->comb_tilde_12_feedBack_write(v);
        this->comb_tilde_12_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_12_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_12_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_12_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_12_feedBack_init() {
        auto result = this->comb_tilde_12_feedBack_calcSizeInSamples();
        this->comb_tilde_12_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_12_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_12_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_12_feedBack_clear() {
        this->comb_tilde_12_feedBack_buffer->setZero();
    }
    
    void comb_tilde_12_feedBack_reset() {
        auto result = this->comb_tilde_12_feedBack_calcSizeInSamples();
        this->comb_tilde_12_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_12_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_12_feedBack_buffer);
        this->comb_tilde_12_feedBack_wrap = this->comb_tilde_12_feedBack_buffer->getSize() - 1;
        this->comb_tilde_12_feedBack_clear();
    
        if (this->comb_tilde_12_feedBack_reader >= this->comb_tilde_12_feedBack__maxdelay || this->comb_tilde_12_feedBack_writer >= this->comb_tilde_12_feedBack__maxdelay) {
            this->comb_tilde_12_feedBack_reader = 0;
            this->comb_tilde_12_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_12_feedBack_dspsetup() {
        this->comb_tilde_12_feedBack_reset();
    }
    
    number comb_tilde_12_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_12_feedBack_size() {
        return this->comb_tilde_12_feedBack__maxdelay;
    }
    
    void comb_tilde_12_init() {
        this->comb_tilde_12_feedForward_init();
        this->comb_tilde_12_feedBack_init();
    }
    
    void comb_tilde_12_clear() {
        this->comb_tilde_12_feedForward_clear();
        this->comb_tilde_12_feedBack_clear();
    }
    
    void comb_tilde_12_reset() {
        this->comb_tilde_12_init();
        this->comb_tilde_12_clear();
    }
    
    void comb_tilde_12_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_12_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_12_reset();
        this->comb_tilde_12_setupDone = true;
        this->comb_tilde_12_feedForward_dspsetup();
        this->comb_tilde_12_feedBack_dspsetup();
    }
    
    void comb_tilde_13_feedForward_step() {
        this->comb_tilde_13_feedForward_reader++;
    
        if (this->comb_tilde_13_feedForward_reader >= (int)(this->comb_tilde_13_feedForward_buffer->getSize()))
            this->comb_tilde_13_feedForward_reader = 0;
    }
    
    number comb_tilde_13_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_13_feedForward_buffer->getSize()) + this->comb_tilde_13_feedForward_reader - ((size > this->comb_tilde_13_feedForward__maxdelay ? this->comb_tilde_13_feedForward__maxdelay : (size < (this->comb_tilde_13_feedForward_reader != this->comb_tilde_13_feedForward_writer) ? this->comb_tilde_13_feedForward_reader != this->comb_tilde_13_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ), this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_13_feedForward_buffer->getSize()) + this->comb_tilde_13_feedForward_reader - ((size > this->comb_tilde_13_feedForward__maxdelay ? this->comb_tilde_13_feedForward__maxdelay : (size < (1 + this->comb_tilde_13_feedForward_reader != this->comb_tilde_13_feedForward_writer) ? 1 + this->comb_tilde_13_feedForward_reader != this->comb_tilde_13_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ), this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ), this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ), this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_13_feedForward_buffer->getSize()) + this->comb_tilde_13_feedForward_reader - ((size > this->comb_tilde_13_feedForward__maxdelay ? this->comb_tilde_13_feedForward__maxdelay : (size < (1 + this->comb_tilde_13_feedForward_reader != this->comb_tilde_13_feedForward_writer) ? 1 + this->comb_tilde_13_feedForward_reader != this->comb_tilde_13_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ), this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ), this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ), this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_13_feedForward_buffer->getSize()) + this->comb_tilde_13_feedForward_reader - ((size > this->comb_tilde_13_feedForward__maxdelay ? this->comb_tilde_13_feedForward__maxdelay : (size < (this->comb_tilde_13_feedForward_reader != this->comb_tilde_13_feedForward_writer) ? this->comb_tilde_13_feedForward_reader != this->comb_tilde_13_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ), this->comb_tilde_13_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_13_feedForward_buffer->getSize()) + this->comb_tilde_13_feedForward_reader - ((size > this->comb_tilde_13_feedForward__maxdelay ? this->comb_tilde_13_feedForward__maxdelay : (size < (this->comb_tilde_13_feedForward_reader != this->comb_tilde_13_feedForward_writer) ? this->comb_tilde_13_feedForward_reader != this->comb_tilde_13_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_13_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_13_feedForward_wrap))
        );
    }
    
    void comb_tilde_13_feedForward_write(number v) {
        this->comb_tilde_13_feedForward_writer = this->comb_tilde_13_feedForward_reader;
        this->comb_tilde_13_feedForward_buffer[(Index)this->comb_tilde_13_feedForward_writer] = v;
    }
    
    number comb_tilde_13_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_13_feedForward__maxdelay : size);
        number val = this->comb_tilde_13_feedForward_read(effectiveSize, 0);
        this->comb_tilde_13_feedForward_write(v);
        this->comb_tilde_13_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_13_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_13_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_13_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_13_feedForward_init() {
        auto result = this->comb_tilde_13_feedForward_calcSizeInSamples();
        this->comb_tilde_13_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_13_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_13_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_13_feedForward_clear() {
        this->comb_tilde_13_feedForward_buffer->setZero();
    }
    
    void comb_tilde_13_feedForward_reset() {
        auto result = this->comb_tilde_13_feedForward_calcSizeInSamples();
        this->comb_tilde_13_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_13_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_13_feedForward_buffer);
        this->comb_tilde_13_feedForward_wrap = this->comb_tilde_13_feedForward_buffer->getSize() - 1;
        this->comb_tilde_13_feedForward_clear();
    
        if (this->comb_tilde_13_feedForward_reader >= this->comb_tilde_13_feedForward__maxdelay || this->comb_tilde_13_feedForward_writer >= this->comb_tilde_13_feedForward__maxdelay) {
            this->comb_tilde_13_feedForward_reader = 0;
            this->comb_tilde_13_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_13_feedForward_dspsetup() {
        this->comb_tilde_13_feedForward_reset();
    }
    
    number comb_tilde_13_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_13_feedForward_size() {
        return this->comb_tilde_13_feedForward__maxdelay;
    }
    
    void comb_tilde_13_feedBack_step() {
        this->comb_tilde_13_feedBack_reader++;
    
        if (this->comb_tilde_13_feedBack_reader >= (int)(this->comb_tilde_13_feedBack_buffer->getSize()))
            this->comb_tilde_13_feedBack_reader = 0;
    }
    
    number comb_tilde_13_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_13_feedBack_buffer->getSize()) + this->comb_tilde_13_feedBack_reader - ((size > this->comb_tilde_13_feedBack__maxdelay ? this->comb_tilde_13_feedBack__maxdelay : (size < (this->comb_tilde_13_feedBack_reader != this->comb_tilde_13_feedBack_writer) ? this->comb_tilde_13_feedBack_reader != this->comb_tilde_13_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ), this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_13_feedBack_buffer->getSize()) + this->comb_tilde_13_feedBack_reader - ((size > this->comb_tilde_13_feedBack__maxdelay ? this->comb_tilde_13_feedBack__maxdelay : (size < (1 + this->comb_tilde_13_feedBack_reader != this->comb_tilde_13_feedBack_writer) ? 1 + this->comb_tilde_13_feedBack_reader != this->comb_tilde_13_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ), this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ), this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ), this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_13_feedBack_buffer->getSize()) + this->comb_tilde_13_feedBack_reader - ((size > this->comb_tilde_13_feedBack__maxdelay ? this->comb_tilde_13_feedBack__maxdelay : (size < (1 + this->comb_tilde_13_feedBack_reader != this->comb_tilde_13_feedBack_writer) ? 1 + this->comb_tilde_13_feedBack_reader != this->comb_tilde_13_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ), this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ), this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ), this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_13_feedBack_buffer->getSize()) + this->comb_tilde_13_feedBack_reader - ((size > this->comb_tilde_13_feedBack__maxdelay ? this->comb_tilde_13_feedBack__maxdelay : (size < (this->comb_tilde_13_feedBack_reader != this->comb_tilde_13_feedBack_writer) ? this->comb_tilde_13_feedBack_reader != this->comb_tilde_13_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ), this->comb_tilde_13_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_13_feedBack_buffer->getSize()) + this->comb_tilde_13_feedBack_reader - ((size > this->comb_tilde_13_feedBack__maxdelay ? this->comb_tilde_13_feedBack__maxdelay : (size < (this->comb_tilde_13_feedBack_reader != this->comb_tilde_13_feedBack_writer) ? this->comb_tilde_13_feedBack_reader != this->comb_tilde_13_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_13_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_13_feedBack_wrap))
        );
    }
    
    void comb_tilde_13_feedBack_write(number v) {
        this->comb_tilde_13_feedBack_writer = this->comb_tilde_13_feedBack_reader;
        this->comb_tilde_13_feedBack_buffer[(Index)this->comb_tilde_13_feedBack_writer] = v;
    }
    
    number comb_tilde_13_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_13_feedBack__maxdelay : size);
        number val = this->comb_tilde_13_feedBack_read(effectiveSize, 0);
        this->comb_tilde_13_feedBack_write(v);
        this->comb_tilde_13_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_13_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_13_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_13_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_13_feedBack_init() {
        auto result = this->comb_tilde_13_feedBack_calcSizeInSamples();
        this->comb_tilde_13_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_13_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_13_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_13_feedBack_clear() {
        this->comb_tilde_13_feedBack_buffer->setZero();
    }
    
    void comb_tilde_13_feedBack_reset() {
        auto result = this->comb_tilde_13_feedBack_calcSizeInSamples();
        this->comb_tilde_13_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_13_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_13_feedBack_buffer);
        this->comb_tilde_13_feedBack_wrap = this->comb_tilde_13_feedBack_buffer->getSize() - 1;
        this->comb_tilde_13_feedBack_clear();
    
        if (this->comb_tilde_13_feedBack_reader >= this->comb_tilde_13_feedBack__maxdelay || this->comb_tilde_13_feedBack_writer >= this->comb_tilde_13_feedBack__maxdelay) {
            this->comb_tilde_13_feedBack_reader = 0;
            this->comb_tilde_13_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_13_feedBack_dspsetup() {
        this->comb_tilde_13_feedBack_reset();
    }
    
    number comb_tilde_13_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_13_feedBack_size() {
        return this->comb_tilde_13_feedBack__maxdelay;
    }
    
    void comb_tilde_13_init() {
        this->comb_tilde_13_feedForward_init();
        this->comb_tilde_13_feedBack_init();
    }
    
    void comb_tilde_13_clear() {
        this->comb_tilde_13_feedForward_clear();
        this->comb_tilde_13_feedBack_clear();
    }
    
    void comb_tilde_13_reset() {
        this->comb_tilde_13_init();
        this->comb_tilde_13_clear();
    }
    
    void comb_tilde_13_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_13_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_13_reset();
        this->comb_tilde_13_setupDone = true;
        this->comb_tilde_13_feedForward_dspsetup();
        this->comb_tilde_13_feedBack_dspsetup();
    }
    
    void comb_tilde_14_feedForward_step() {
        this->comb_tilde_14_feedForward_reader++;
    
        if (this->comb_tilde_14_feedForward_reader >= (int)(this->comb_tilde_14_feedForward_buffer->getSize()))
            this->comb_tilde_14_feedForward_reader = 0;
    }
    
    number comb_tilde_14_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_14_feedForward_buffer->getSize()) + this->comb_tilde_14_feedForward_reader - ((size > this->comb_tilde_14_feedForward__maxdelay ? this->comb_tilde_14_feedForward__maxdelay : (size < (this->comb_tilde_14_feedForward_reader != this->comb_tilde_14_feedForward_writer) ? this->comb_tilde_14_feedForward_reader != this->comb_tilde_14_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ), this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_14_feedForward_buffer->getSize()) + this->comb_tilde_14_feedForward_reader - ((size > this->comb_tilde_14_feedForward__maxdelay ? this->comb_tilde_14_feedForward__maxdelay : (size < (1 + this->comb_tilde_14_feedForward_reader != this->comb_tilde_14_feedForward_writer) ? 1 + this->comb_tilde_14_feedForward_reader != this->comb_tilde_14_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ), this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ), this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ), this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_14_feedForward_buffer->getSize()) + this->comb_tilde_14_feedForward_reader - ((size > this->comb_tilde_14_feedForward__maxdelay ? this->comb_tilde_14_feedForward__maxdelay : (size < (1 + this->comb_tilde_14_feedForward_reader != this->comb_tilde_14_feedForward_writer) ? 1 + this->comb_tilde_14_feedForward_reader != this->comb_tilde_14_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ), this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ), this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ), this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_14_feedForward_buffer->getSize()) + this->comb_tilde_14_feedForward_reader - ((size > this->comb_tilde_14_feedForward__maxdelay ? this->comb_tilde_14_feedForward__maxdelay : (size < (this->comb_tilde_14_feedForward_reader != this->comb_tilde_14_feedForward_writer) ? this->comb_tilde_14_feedForward_reader != this->comb_tilde_14_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ), this->comb_tilde_14_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_14_feedForward_buffer->getSize()) + this->comb_tilde_14_feedForward_reader - ((size > this->comb_tilde_14_feedForward__maxdelay ? this->comb_tilde_14_feedForward__maxdelay : (size < (this->comb_tilde_14_feedForward_reader != this->comb_tilde_14_feedForward_writer) ? this->comb_tilde_14_feedForward_reader != this->comb_tilde_14_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_14_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_14_feedForward_wrap))
        );
    }
    
    void comb_tilde_14_feedForward_write(number v) {
        this->comb_tilde_14_feedForward_writer = this->comb_tilde_14_feedForward_reader;
        this->comb_tilde_14_feedForward_buffer[(Index)this->comb_tilde_14_feedForward_writer] = v;
    }
    
    number comb_tilde_14_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_14_feedForward__maxdelay : size);
        number val = this->comb_tilde_14_feedForward_read(effectiveSize, 0);
        this->comb_tilde_14_feedForward_write(v);
        this->comb_tilde_14_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_14_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_14_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_14_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_14_feedForward_init() {
        auto result = this->comb_tilde_14_feedForward_calcSizeInSamples();
        this->comb_tilde_14_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_14_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_14_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_14_feedForward_clear() {
        this->comb_tilde_14_feedForward_buffer->setZero();
    }
    
    void comb_tilde_14_feedForward_reset() {
        auto result = this->comb_tilde_14_feedForward_calcSizeInSamples();
        this->comb_tilde_14_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_14_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_14_feedForward_buffer);
        this->comb_tilde_14_feedForward_wrap = this->comb_tilde_14_feedForward_buffer->getSize() - 1;
        this->comb_tilde_14_feedForward_clear();
    
        if (this->comb_tilde_14_feedForward_reader >= this->comb_tilde_14_feedForward__maxdelay || this->comb_tilde_14_feedForward_writer >= this->comb_tilde_14_feedForward__maxdelay) {
            this->comb_tilde_14_feedForward_reader = 0;
            this->comb_tilde_14_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_14_feedForward_dspsetup() {
        this->comb_tilde_14_feedForward_reset();
    }
    
    number comb_tilde_14_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_14_feedForward_size() {
        return this->comb_tilde_14_feedForward__maxdelay;
    }
    
    void comb_tilde_14_feedBack_step() {
        this->comb_tilde_14_feedBack_reader++;
    
        if (this->comb_tilde_14_feedBack_reader >= (int)(this->comb_tilde_14_feedBack_buffer->getSize()))
            this->comb_tilde_14_feedBack_reader = 0;
    }
    
    number comb_tilde_14_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_14_feedBack_buffer->getSize()) + this->comb_tilde_14_feedBack_reader - ((size > this->comb_tilde_14_feedBack__maxdelay ? this->comb_tilde_14_feedBack__maxdelay : (size < (this->comb_tilde_14_feedBack_reader != this->comb_tilde_14_feedBack_writer) ? this->comb_tilde_14_feedBack_reader != this->comb_tilde_14_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ), this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_14_feedBack_buffer->getSize()) + this->comb_tilde_14_feedBack_reader - ((size > this->comb_tilde_14_feedBack__maxdelay ? this->comb_tilde_14_feedBack__maxdelay : (size < (1 + this->comb_tilde_14_feedBack_reader != this->comb_tilde_14_feedBack_writer) ? 1 + this->comb_tilde_14_feedBack_reader != this->comb_tilde_14_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ), this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ), this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ), this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_14_feedBack_buffer->getSize()) + this->comb_tilde_14_feedBack_reader - ((size > this->comb_tilde_14_feedBack__maxdelay ? this->comb_tilde_14_feedBack__maxdelay : (size < (1 + this->comb_tilde_14_feedBack_reader != this->comb_tilde_14_feedBack_writer) ? 1 + this->comb_tilde_14_feedBack_reader != this->comb_tilde_14_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ), this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ), this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ), this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_14_feedBack_buffer->getSize()) + this->comb_tilde_14_feedBack_reader - ((size > this->comb_tilde_14_feedBack__maxdelay ? this->comb_tilde_14_feedBack__maxdelay : (size < (this->comb_tilde_14_feedBack_reader != this->comb_tilde_14_feedBack_writer) ? this->comb_tilde_14_feedBack_reader != this->comb_tilde_14_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ), this->comb_tilde_14_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_14_feedBack_buffer->getSize()) + this->comb_tilde_14_feedBack_reader - ((size > this->comb_tilde_14_feedBack__maxdelay ? this->comb_tilde_14_feedBack__maxdelay : (size < (this->comb_tilde_14_feedBack_reader != this->comb_tilde_14_feedBack_writer) ? this->comb_tilde_14_feedBack_reader != this->comb_tilde_14_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_14_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_14_feedBack_wrap))
        );
    }
    
    void comb_tilde_14_feedBack_write(number v) {
        this->comb_tilde_14_feedBack_writer = this->comb_tilde_14_feedBack_reader;
        this->comb_tilde_14_feedBack_buffer[(Index)this->comb_tilde_14_feedBack_writer] = v;
    }
    
    number comb_tilde_14_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_14_feedBack__maxdelay : size);
        number val = this->comb_tilde_14_feedBack_read(effectiveSize, 0);
        this->comb_tilde_14_feedBack_write(v);
        this->comb_tilde_14_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_14_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_14_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_14_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_14_feedBack_init() {
        auto result = this->comb_tilde_14_feedBack_calcSizeInSamples();
        this->comb_tilde_14_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_14_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_14_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_14_feedBack_clear() {
        this->comb_tilde_14_feedBack_buffer->setZero();
    }
    
    void comb_tilde_14_feedBack_reset() {
        auto result = this->comb_tilde_14_feedBack_calcSizeInSamples();
        this->comb_tilde_14_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_14_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_14_feedBack_buffer);
        this->comb_tilde_14_feedBack_wrap = this->comb_tilde_14_feedBack_buffer->getSize() - 1;
        this->comb_tilde_14_feedBack_clear();
    
        if (this->comb_tilde_14_feedBack_reader >= this->comb_tilde_14_feedBack__maxdelay || this->comb_tilde_14_feedBack_writer >= this->comb_tilde_14_feedBack__maxdelay) {
            this->comb_tilde_14_feedBack_reader = 0;
            this->comb_tilde_14_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_14_feedBack_dspsetup() {
        this->comb_tilde_14_feedBack_reset();
    }
    
    number comb_tilde_14_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_14_feedBack_size() {
        return this->comb_tilde_14_feedBack__maxdelay;
    }
    
    void comb_tilde_14_init() {
        this->comb_tilde_14_feedForward_init();
        this->comb_tilde_14_feedBack_init();
    }
    
    void comb_tilde_14_clear() {
        this->comb_tilde_14_feedForward_clear();
        this->comb_tilde_14_feedBack_clear();
    }
    
    void comb_tilde_14_reset() {
        this->comb_tilde_14_init();
        this->comb_tilde_14_clear();
    }
    
    void comb_tilde_14_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_14_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_14_reset();
        this->comb_tilde_14_setupDone = true;
        this->comb_tilde_14_feedForward_dspsetup();
        this->comb_tilde_14_feedBack_dspsetup();
    }
    
    void comb_tilde_15_feedForward_step() {
        this->comb_tilde_15_feedForward_reader++;
    
        if (this->comb_tilde_15_feedForward_reader >= (int)(this->comb_tilde_15_feedForward_buffer->getSize()))
            this->comb_tilde_15_feedForward_reader = 0;
    }
    
    number comb_tilde_15_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_15_feedForward_buffer->getSize()) + this->comb_tilde_15_feedForward_reader - ((size > this->comb_tilde_15_feedForward__maxdelay ? this->comb_tilde_15_feedForward__maxdelay : (size < (this->comb_tilde_15_feedForward_reader != this->comb_tilde_15_feedForward_writer) ? this->comb_tilde_15_feedForward_reader != this->comb_tilde_15_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ), this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_15_feedForward_buffer->getSize()) + this->comb_tilde_15_feedForward_reader - ((size > this->comb_tilde_15_feedForward__maxdelay ? this->comb_tilde_15_feedForward__maxdelay : (size < (1 + this->comb_tilde_15_feedForward_reader != this->comb_tilde_15_feedForward_writer) ? 1 + this->comb_tilde_15_feedForward_reader != this->comb_tilde_15_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ), this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ), this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ), this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_15_feedForward_buffer->getSize()) + this->comb_tilde_15_feedForward_reader - ((size > this->comb_tilde_15_feedForward__maxdelay ? this->comb_tilde_15_feedForward__maxdelay : (size < (1 + this->comb_tilde_15_feedForward_reader != this->comb_tilde_15_feedForward_writer) ? 1 + this->comb_tilde_15_feedForward_reader != this->comb_tilde_15_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ), this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ), this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ), this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_15_feedForward_buffer->getSize()) + this->comb_tilde_15_feedForward_reader - ((size > this->comb_tilde_15_feedForward__maxdelay ? this->comb_tilde_15_feedForward__maxdelay : (size < (this->comb_tilde_15_feedForward_reader != this->comb_tilde_15_feedForward_writer) ? this->comb_tilde_15_feedForward_reader != this->comb_tilde_15_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ), this->comb_tilde_15_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_15_feedForward_buffer->getSize()) + this->comb_tilde_15_feedForward_reader - ((size > this->comb_tilde_15_feedForward__maxdelay ? this->comb_tilde_15_feedForward__maxdelay : (size < (this->comb_tilde_15_feedForward_reader != this->comb_tilde_15_feedForward_writer) ? this->comb_tilde_15_feedForward_reader != this->comb_tilde_15_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_15_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_15_feedForward_wrap))
        );
    }
    
    void comb_tilde_15_feedForward_write(number v) {
        this->comb_tilde_15_feedForward_writer = this->comb_tilde_15_feedForward_reader;
        this->comb_tilde_15_feedForward_buffer[(Index)this->comb_tilde_15_feedForward_writer] = v;
    }
    
    number comb_tilde_15_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_15_feedForward__maxdelay : size);
        number val = this->comb_tilde_15_feedForward_read(effectiveSize, 0);
        this->comb_tilde_15_feedForward_write(v);
        this->comb_tilde_15_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_15_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_15_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_15_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_15_feedForward_init() {
        auto result = this->comb_tilde_15_feedForward_calcSizeInSamples();
        this->comb_tilde_15_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_15_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_15_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_15_feedForward_clear() {
        this->comb_tilde_15_feedForward_buffer->setZero();
    }
    
    void comb_tilde_15_feedForward_reset() {
        auto result = this->comb_tilde_15_feedForward_calcSizeInSamples();
        this->comb_tilde_15_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_15_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_15_feedForward_buffer);
        this->comb_tilde_15_feedForward_wrap = this->comb_tilde_15_feedForward_buffer->getSize() - 1;
        this->comb_tilde_15_feedForward_clear();
    
        if (this->comb_tilde_15_feedForward_reader >= this->comb_tilde_15_feedForward__maxdelay || this->comb_tilde_15_feedForward_writer >= this->comb_tilde_15_feedForward__maxdelay) {
            this->comb_tilde_15_feedForward_reader = 0;
            this->comb_tilde_15_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_15_feedForward_dspsetup() {
        this->comb_tilde_15_feedForward_reset();
    }
    
    number comb_tilde_15_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_15_feedForward_size() {
        return this->comb_tilde_15_feedForward__maxdelay;
    }
    
    void comb_tilde_15_feedBack_step() {
        this->comb_tilde_15_feedBack_reader++;
    
        if (this->comb_tilde_15_feedBack_reader >= (int)(this->comb_tilde_15_feedBack_buffer->getSize()))
            this->comb_tilde_15_feedBack_reader = 0;
    }
    
    number comb_tilde_15_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_15_feedBack_buffer->getSize()) + this->comb_tilde_15_feedBack_reader - ((size > this->comb_tilde_15_feedBack__maxdelay ? this->comb_tilde_15_feedBack__maxdelay : (size < (this->comb_tilde_15_feedBack_reader != this->comb_tilde_15_feedBack_writer) ? this->comb_tilde_15_feedBack_reader != this->comb_tilde_15_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ), this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_15_feedBack_buffer->getSize()) + this->comb_tilde_15_feedBack_reader - ((size > this->comb_tilde_15_feedBack__maxdelay ? this->comb_tilde_15_feedBack__maxdelay : (size < (1 + this->comb_tilde_15_feedBack_reader != this->comb_tilde_15_feedBack_writer) ? 1 + this->comb_tilde_15_feedBack_reader != this->comb_tilde_15_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ), this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ), this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ), this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_15_feedBack_buffer->getSize()) + this->comb_tilde_15_feedBack_reader - ((size > this->comb_tilde_15_feedBack__maxdelay ? this->comb_tilde_15_feedBack__maxdelay : (size < (1 + this->comb_tilde_15_feedBack_reader != this->comb_tilde_15_feedBack_writer) ? 1 + this->comb_tilde_15_feedBack_reader != this->comb_tilde_15_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ), this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ), this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ), this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_15_feedBack_buffer->getSize()) + this->comb_tilde_15_feedBack_reader - ((size > this->comb_tilde_15_feedBack__maxdelay ? this->comb_tilde_15_feedBack__maxdelay : (size < (this->comb_tilde_15_feedBack_reader != this->comb_tilde_15_feedBack_writer) ? this->comb_tilde_15_feedBack_reader != this->comb_tilde_15_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ), this->comb_tilde_15_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_15_feedBack_buffer->getSize()) + this->comb_tilde_15_feedBack_reader - ((size > this->comb_tilde_15_feedBack__maxdelay ? this->comb_tilde_15_feedBack__maxdelay : (size < (this->comb_tilde_15_feedBack_reader != this->comb_tilde_15_feedBack_writer) ? this->comb_tilde_15_feedBack_reader != this->comb_tilde_15_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_15_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_15_feedBack_wrap))
        );
    }
    
    void comb_tilde_15_feedBack_write(number v) {
        this->comb_tilde_15_feedBack_writer = this->comb_tilde_15_feedBack_reader;
        this->comb_tilde_15_feedBack_buffer[(Index)this->comb_tilde_15_feedBack_writer] = v;
    }
    
    number comb_tilde_15_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_15_feedBack__maxdelay : size);
        number val = this->comb_tilde_15_feedBack_read(effectiveSize, 0);
        this->comb_tilde_15_feedBack_write(v);
        this->comb_tilde_15_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_15_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_15_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_15_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_15_feedBack_init() {
        auto result = this->comb_tilde_15_feedBack_calcSizeInSamples();
        this->comb_tilde_15_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_15_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_15_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_15_feedBack_clear() {
        this->comb_tilde_15_feedBack_buffer->setZero();
    }
    
    void comb_tilde_15_feedBack_reset() {
        auto result = this->comb_tilde_15_feedBack_calcSizeInSamples();
        this->comb_tilde_15_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_15_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_15_feedBack_buffer);
        this->comb_tilde_15_feedBack_wrap = this->comb_tilde_15_feedBack_buffer->getSize() - 1;
        this->comb_tilde_15_feedBack_clear();
    
        if (this->comb_tilde_15_feedBack_reader >= this->comb_tilde_15_feedBack__maxdelay || this->comb_tilde_15_feedBack_writer >= this->comb_tilde_15_feedBack__maxdelay) {
            this->comb_tilde_15_feedBack_reader = 0;
            this->comb_tilde_15_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_15_feedBack_dspsetup() {
        this->comb_tilde_15_feedBack_reset();
    }
    
    number comb_tilde_15_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_15_feedBack_size() {
        return this->comb_tilde_15_feedBack__maxdelay;
    }
    
    void comb_tilde_15_init() {
        this->comb_tilde_15_feedForward_init();
        this->comb_tilde_15_feedBack_init();
    }
    
    void comb_tilde_15_clear() {
        this->comb_tilde_15_feedForward_clear();
        this->comb_tilde_15_feedBack_clear();
    }
    
    void comb_tilde_15_reset() {
        this->comb_tilde_15_init();
        this->comb_tilde_15_clear();
    }
    
    void comb_tilde_15_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_15_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_15_reset();
        this->comb_tilde_15_setupDone = true;
        this->comb_tilde_15_feedForward_dspsetup();
        this->comb_tilde_15_feedBack_dspsetup();
    }
    
    void comb_tilde_16_feedForward_step() {
        this->comb_tilde_16_feedForward_reader++;
    
        if (this->comb_tilde_16_feedForward_reader >= (int)(this->comb_tilde_16_feedForward_buffer->getSize()))
            this->comb_tilde_16_feedForward_reader = 0;
    }
    
    number comb_tilde_16_feedForward_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_16_feedForward_buffer->getSize()) + this->comb_tilde_16_feedForward_reader - ((size > this->comb_tilde_16_feedForward__maxdelay ? this->comb_tilde_16_feedForward__maxdelay : (size < (this->comb_tilde_16_feedForward_reader != this->comb_tilde_16_feedForward_writer) ? this->comb_tilde_16_feedForward_reader != this->comb_tilde_16_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ), this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_16_feedForward_buffer->getSize()) + this->comb_tilde_16_feedForward_reader - ((size > this->comb_tilde_16_feedForward__maxdelay ? this->comb_tilde_16_feedForward__maxdelay : (size < (1 + this->comb_tilde_16_feedForward_reader != this->comb_tilde_16_feedForward_writer) ? 1 + this->comb_tilde_16_feedForward_reader != this->comb_tilde_16_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ), this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ), this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ), this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_16_feedForward_buffer->getSize()) + this->comb_tilde_16_feedForward_reader - ((size > this->comb_tilde_16_feedForward__maxdelay ? this->comb_tilde_16_feedForward__maxdelay : (size < (1 + this->comb_tilde_16_feedForward_reader != this->comb_tilde_16_feedForward_writer) ? 1 + this->comb_tilde_16_feedForward_reader != this->comb_tilde_16_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ), this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ), this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ), this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_16_feedForward_buffer->getSize()) + this->comb_tilde_16_feedForward_reader - ((size > this->comb_tilde_16_feedForward__maxdelay ? this->comb_tilde_16_feedForward__maxdelay : (size < (this->comb_tilde_16_feedForward_reader != this->comb_tilde_16_feedForward_writer) ? this->comb_tilde_16_feedForward_reader != this->comb_tilde_16_feedForward_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ), this->comb_tilde_16_feedForward_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_16_feedForward_buffer->getSize()) + this->comb_tilde_16_feedForward_reader - ((size > this->comb_tilde_16_feedForward__maxdelay ? this->comb_tilde_16_feedForward__maxdelay : (size < (this->comb_tilde_16_feedForward_reader != this->comb_tilde_16_feedForward_writer) ? this->comb_tilde_16_feedForward_reader != this->comb_tilde_16_feedForward_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_16_feedForward_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_16_feedForward_wrap))
        );
    }
    
    void comb_tilde_16_feedForward_write(number v) {
        this->comb_tilde_16_feedForward_writer = this->comb_tilde_16_feedForward_reader;
        this->comb_tilde_16_feedForward_buffer[(Index)this->comb_tilde_16_feedForward_writer] = v;
    }
    
    number comb_tilde_16_feedForward_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_16_feedForward__maxdelay : size);
        number val = this->comb_tilde_16_feedForward_read(effectiveSize, 0);
        this->comb_tilde_16_feedForward_write(v);
        this->comb_tilde_16_feedForward_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_16_feedForward_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_16_feedForward_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_16_feedForward_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_16_feedForward_init() {
        auto result = this->comb_tilde_16_feedForward_calcSizeInSamples();
        this->comb_tilde_16_feedForward__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_16_feedForward_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_16_feedForward_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_16_feedForward_clear() {
        this->comb_tilde_16_feedForward_buffer->setZero();
    }
    
    void comb_tilde_16_feedForward_reset() {
        auto result = this->comb_tilde_16_feedForward_calcSizeInSamples();
        this->comb_tilde_16_feedForward__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_16_feedForward_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_16_feedForward_buffer);
        this->comb_tilde_16_feedForward_wrap = this->comb_tilde_16_feedForward_buffer->getSize() - 1;
        this->comb_tilde_16_feedForward_clear();
    
        if (this->comb_tilde_16_feedForward_reader >= this->comb_tilde_16_feedForward__maxdelay || this->comb_tilde_16_feedForward_writer >= this->comb_tilde_16_feedForward__maxdelay) {
            this->comb_tilde_16_feedForward_reader = 0;
            this->comb_tilde_16_feedForward_writer = 0;
        }
    }
    
    void comb_tilde_16_feedForward_dspsetup() {
        this->comb_tilde_16_feedForward_reset();
    }
    
    number comb_tilde_16_feedForward_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_16_feedForward_size() {
        return this->comb_tilde_16_feedForward__maxdelay;
    }
    
    void comb_tilde_16_feedBack_step() {
        this->comb_tilde_16_feedBack_reader++;
    
        if (this->comb_tilde_16_feedBack_reader >= (int)(this->comb_tilde_16_feedBack_buffer->getSize()))
            this->comb_tilde_16_feedBack_reader = 0;
    }
    
    number comb_tilde_16_feedBack_read(number size, Int interp) {
        if (interp == 0) {
            number r = (int)(this->comb_tilde_16_feedBack_buffer->getSize()) + this->comb_tilde_16_feedBack_reader - ((size > this->comb_tilde_16_feedBack__maxdelay ? this->comb_tilde_16_feedBack__maxdelay : (size < (this->comb_tilde_16_feedBack_reader != this->comb_tilde_16_feedBack_writer) ? this->comb_tilde_16_feedBack_reader != this->comb_tilde_16_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ), this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ));
        } else if (interp == 1) {
            number r = (int)(this->comb_tilde_16_feedBack_buffer->getSize()) + this->comb_tilde_16_feedBack_reader - ((size > this->comb_tilde_16_feedBack__maxdelay ? this->comb_tilde_16_feedBack__maxdelay : (size < (1 + this->comb_tilde_16_feedBack_reader != this->comb_tilde_16_feedBack_writer) ? 1 + this->comb_tilde_16_feedBack_reader != this->comb_tilde_16_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->cubicinterp(frac, this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ), this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ), this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ), this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ));
        } else if (interp == 2) {
            number r = (int)(this->comb_tilde_16_feedBack_buffer->getSize()) + this->comb_tilde_16_feedBack_reader - ((size > this->comb_tilde_16_feedBack__maxdelay ? this->comb_tilde_16_feedBack__maxdelay : (size < (1 + this->comb_tilde_16_feedBack_reader != this->comb_tilde_16_feedBack_writer) ? 1 + this->comb_tilde_16_feedBack_reader != this->comb_tilde_16_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
            Index index3 = (Index)(index2 + 1);
            Index index4 = (Index)(index3 + 1);
    
            return this->splineinterp(frac, this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ), this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ), this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index3 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ), this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index4 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ));
        } else if (interp == 3) {
            number r = (int)(this->comb_tilde_16_feedBack_buffer->getSize()) + this->comb_tilde_16_feedBack_reader - ((size > this->comb_tilde_16_feedBack__maxdelay ? this->comb_tilde_16_feedBack__maxdelay : (size < (this->comb_tilde_16_feedBack_reader != this->comb_tilde_16_feedBack_writer) ? this->comb_tilde_16_feedBack_reader != this->comb_tilde_16_feedBack_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            Index index2 = (Index)(index1 + 1);
    
            return this->cosineinterp(frac, this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ), this->comb_tilde_16_feedBack_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
            ));
        }
    
        number r = (int)(this->comb_tilde_16_feedBack_buffer->getSize()) + this->comb_tilde_16_feedBack_reader - ((size > this->comb_tilde_16_feedBack__maxdelay ? this->comb_tilde_16_feedBack__maxdelay : (size < (this->comb_tilde_16_feedBack_reader != this->comb_tilde_16_feedBack_writer) ? this->comb_tilde_16_feedBack_reader != this->comb_tilde_16_feedBack_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->comb_tilde_16_feedBack_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->comb_tilde_16_feedBack_wrap))
        );
    }
    
    void comb_tilde_16_feedBack_write(number v) {
        this->comb_tilde_16_feedBack_writer = this->comb_tilde_16_feedBack_reader;
        this->comb_tilde_16_feedBack_buffer[(Index)this->comb_tilde_16_feedBack_writer] = v;
    }
    
    number comb_tilde_16_feedBack_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->comb_tilde_16_feedBack__maxdelay : size);
        number val = this->comb_tilde_16_feedBack_read(effectiveSize, 0);
        this->comb_tilde_16_feedBack_write(v);
        this->comb_tilde_16_feedBack_step();
        return val;
    }
    
    array<Index, 2> comb_tilde_16_feedBack_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->comb_tilde_16_feedBack_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->comb_tilde_16_feedBack_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void comb_tilde_16_feedBack_init() {
        auto result = this->comb_tilde_16_feedBack_calcSizeInSamples();
        this->comb_tilde_16_feedBack__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_16_feedBack_buffer->requestSize(requestedSizeInSamples, 1);
        this->comb_tilde_16_feedBack_wrap = requestedSizeInSamples - 1;
    }
    
    void comb_tilde_16_feedBack_clear() {
        this->comb_tilde_16_feedBack_buffer->setZero();
    }
    
    void comb_tilde_16_feedBack_reset() {
        auto result = this->comb_tilde_16_feedBack_calcSizeInSamples();
        this->comb_tilde_16_feedBack__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->comb_tilde_16_feedBack_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->comb_tilde_16_feedBack_buffer);
        this->comb_tilde_16_feedBack_wrap = this->comb_tilde_16_feedBack_buffer->getSize() - 1;
        this->comb_tilde_16_feedBack_clear();
    
        if (this->comb_tilde_16_feedBack_reader >= this->comb_tilde_16_feedBack__maxdelay || this->comb_tilde_16_feedBack_writer >= this->comb_tilde_16_feedBack__maxdelay) {
            this->comb_tilde_16_feedBack_reader = 0;
            this->comb_tilde_16_feedBack_writer = 0;
        }
    }
    
    void comb_tilde_16_feedBack_dspsetup() {
        this->comb_tilde_16_feedBack_reset();
    }
    
    number comb_tilde_16_feedBack_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return 44100;
    }
    
    number comb_tilde_16_feedBack_size() {
        return this->comb_tilde_16_feedBack__maxdelay;
    }
    
    void comb_tilde_16_init() {
        this->comb_tilde_16_feedForward_init();
        this->comb_tilde_16_feedBack_init();
    }
    
    void comb_tilde_16_clear() {
        this->comb_tilde_16_feedForward_clear();
        this->comb_tilde_16_feedBack_clear();
    }
    
    void comb_tilde_16_reset() {
        this->comb_tilde_16_init();
        this->comb_tilde_16_clear();
    }
    
    void comb_tilde_16_dspsetup(bool force) {
        if ((bool)(this->comb_tilde_16_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->comb_tilde_16_reset();
        this->comb_tilde_16_setupDone = true;
        this->comb_tilde_16_feedForward_dspsetup();
        this->comb_tilde_16_feedBack_dspsetup();
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
        comb_tilde_10_x = 0;
        comb_tilde_10_delayTime = 110;
        comb_tilde_10_gain = 0;
        comb_tilde_10_ffGain = 1;
        comb_tilde_10_fbGain = 0;
        dspexpr_25_in1 = 0;
        dspexpr_25_in2 = 110;
        comb_tilde_11_x = 0;
        comb_tilde_11_delayTime = 135;
        comb_tilde_11_gain = 0;
        comb_tilde_11_ffGain = 1;
        comb_tilde_11_fbGain = 0;
        dspexpr_26_in1 = 0;
        dspexpr_26_in2 = 135;
        comb_tilde_12_x = 0;
        comb_tilde_12_delayTime = 55;
        comb_tilde_12_gain = 0;
        comb_tilde_12_ffGain = 1;
        comb_tilde_12_fbGain = 0;
        dspexpr_27_in1 = 0;
        dspexpr_27_in2 = 55;
        dspexpr_28_in1 = 0;
        dspexpr_28_in2 = 0.355;
        comb_tilde_13_x = 0;
        comb_tilde_13_delayTime = 3;
        comb_tilde_13_gain = 0;
        comb_tilde_13_ffGain = 1;
        comb_tilde_13_fbGain = 0;
        dspexpr_29_in1 = 0;
        dspexpr_29_in2 = 0;
        dspexpr_30_in1 = 0;
        dspexpr_30_in2 = 3;
        dspexpr_31_in1 = 0;
        dspexpr_31_in2 = -1;
        comb_tilde_14_x = 0;
        comb_tilde_14_delayTime = 35;
        comb_tilde_14_gain = 0;
        comb_tilde_14_ffGain = 1;
        comb_tilde_14_fbGain = 0;
        dspexpr_32_in1 = 0;
        dspexpr_32_in2 = 35;
        comb_tilde_15_x = 0;
        comb_tilde_15_delayTime = 27;
        comb_tilde_15_gain = 0;
        comb_tilde_15_ffGain = 1;
        comb_tilde_15_fbGain = 0;
        dspexpr_33_in1 = 0;
        dspexpr_33_in2 = 27;
        comb_tilde_16_x = 0;
        comb_tilde_16_delayTime = 52;
        comb_tilde_16_gain = 0;
        comb_tilde_16_ffGain = 1;
        comb_tilde_16_fbGain = 0;
        dspexpr_34_in1 = 0;
        dspexpr_34_in2 = 52;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        signals[4] = nullptr;
        signals[5] = nullptr;
        signals[6] = nullptr;
        signals[7] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        comb_tilde_10_feedForward__maxdelay = 0;
        comb_tilde_10_feedForward_sizemode = 0;
        comb_tilde_10_feedForward_wrap = 0;
        comb_tilde_10_feedForward_reader = 0;
        comb_tilde_10_feedForward_writer = 0;
        comb_tilde_10_feedBack__maxdelay = 0;
        comb_tilde_10_feedBack_sizemode = 0;
        comb_tilde_10_feedBack_wrap = 0;
        comb_tilde_10_feedBack_reader = 0;
        comb_tilde_10_feedBack_writer = 0;
        comb_tilde_10_setupDone = false;
        comb_tilde_11_feedForward__maxdelay = 0;
        comb_tilde_11_feedForward_sizemode = 0;
        comb_tilde_11_feedForward_wrap = 0;
        comb_tilde_11_feedForward_reader = 0;
        comb_tilde_11_feedForward_writer = 0;
        comb_tilde_11_feedBack__maxdelay = 0;
        comb_tilde_11_feedBack_sizemode = 0;
        comb_tilde_11_feedBack_wrap = 0;
        comb_tilde_11_feedBack_reader = 0;
        comb_tilde_11_feedBack_writer = 0;
        comb_tilde_11_setupDone = false;
        comb_tilde_12_feedForward__maxdelay = 0;
        comb_tilde_12_feedForward_sizemode = 0;
        comb_tilde_12_feedForward_wrap = 0;
        comb_tilde_12_feedForward_reader = 0;
        comb_tilde_12_feedForward_writer = 0;
        comb_tilde_12_feedBack__maxdelay = 0;
        comb_tilde_12_feedBack_sizemode = 0;
        comb_tilde_12_feedBack_wrap = 0;
        comb_tilde_12_feedBack_reader = 0;
        comb_tilde_12_feedBack_writer = 0;
        comb_tilde_12_setupDone = false;
        comb_tilde_13_feedForward__maxdelay = 0;
        comb_tilde_13_feedForward_sizemode = 0;
        comb_tilde_13_feedForward_wrap = 0;
        comb_tilde_13_feedForward_reader = 0;
        comb_tilde_13_feedForward_writer = 0;
        comb_tilde_13_feedBack__maxdelay = 0;
        comb_tilde_13_feedBack_sizemode = 0;
        comb_tilde_13_feedBack_wrap = 0;
        comb_tilde_13_feedBack_reader = 0;
        comb_tilde_13_feedBack_writer = 0;
        comb_tilde_13_setupDone = false;
        comb_tilde_14_feedForward__maxdelay = 0;
        comb_tilde_14_feedForward_sizemode = 0;
        comb_tilde_14_feedForward_wrap = 0;
        comb_tilde_14_feedForward_reader = 0;
        comb_tilde_14_feedForward_writer = 0;
        comb_tilde_14_feedBack__maxdelay = 0;
        comb_tilde_14_feedBack_sizemode = 0;
        comb_tilde_14_feedBack_wrap = 0;
        comb_tilde_14_feedBack_reader = 0;
        comb_tilde_14_feedBack_writer = 0;
        comb_tilde_14_setupDone = false;
        comb_tilde_15_feedForward__maxdelay = 0;
        comb_tilde_15_feedForward_sizemode = 0;
        comb_tilde_15_feedForward_wrap = 0;
        comb_tilde_15_feedForward_reader = 0;
        comb_tilde_15_feedForward_writer = 0;
        comb_tilde_15_feedBack__maxdelay = 0;
        comb_tilde_15_feedBack_sizemode = 0;
        comb_tilde_15_feedBack_wrap = 0;
        comb_tilde_15_feedBack_reader = 0;
        comb_tilde_15_feedBack_writer = 0;
        comb_tilde_15_setupDone = false;
        comb_tilde_16_feedForward__maxdelay = 0;
        comb_tilde_16_feedForward_sizemode = 0;
        comb_tilde_16_feedForward_wrap = 0;
        comb_tilde_16_feedForward_reader = 0;
        comb_tilde_16_feedForward_writer = 0;
        comb_tilde_16_feedBack__maxdelay = 0;
        comb_tilde_16_feedBack_sizemode = 0;
        comb_tilde_16_feedBack_wrap = 0;
        comb_tilde_16_feedBack_reader = 0;
        comb_tilde_16_feedBack_writer = 0;
        comb_tilde_16_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number comb_tilde_10_x;
        number comb_tilde_10_delayTime;
        number comb_tilde_10_gain;
        number comb_tilde_10_ffGain;
        number comb_tilde_10_fbGain;
        number dspexpr_25_in1;
        number dspexpr_25_in2;
        number comb_tilde_11_x;
        number comb_tilde_11_delayTime;
        number comb_tilde_11_gain;
        number comb_tilde_11_ffGain;
        number comb_tilde_11_fbGain;
        number dspexpr_26_in1;
        number dspexpr_26_in2;
        number comb_tilde_12_x;
        number comb_tilde_12_delayTime;
        number comb_tilde_12_gain;
        number comb_tilde_12_ffGain;
        number comb_tilde_12_fbGain;
        number dspexpr_27_in1;
        number dspexpr_27_in2;
        number dspexpr_28_in1;
        number dspexpr_28_in2;
        number comb_tilde_13_x;
        number comb_tilde_13_delayTime;
        number comb_tilde_13_gain;
        number comb_tilde_13_ffGain;
        number comb_tilde_13_fbGain;
        number dspexpr_29_in1;
        number dspexpr_29_in2;
        number dspexpr_30_in1;
        number dspexpr_30_in2;
        number dspexpr_31_in1;
        number dspexpr_31_in2;
        number comb_tilde_14_x;
        number comb_tilde_14_delayTime;
        number comb_tilde_14_gain;
        number comb_tilde_14_ffGain;
        number comb_tilde_14_fbGain;
        number dspexpr_32_in1;
        number dspexpr_32_in2;
        number comb_tilde_15_x;
        number comb_tilde_15_delayTime;
        number comb_tilde_15_gain;
        number comb_tilde_15_ffGain;
        number comb_tilde_15_fbGain;
        number dspexpr_33_in1;
        number dspexpr_33_in2;
        number comb_tilde_16_x;
        number comb_tilde_16_delayTime;
        number comb_tilde_16_gain;
        number comb_tilde_16_ffGain;
        number comb_tilde_16_fbGain;
        number dspexpr_34_in1;
        number dspexpr_34_in2;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[8];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float64BufferRef comb_tilde_10_feedForward_buffer;
        Index comb_tilde_10_feedForward__maxdelay;
        Int comb_tilde_10_feedForward_sizemode;
        Index comb_tilde_10_feedForward_wrap;
        Int comb_tilde_10_feedForward_reader;
        Int comb_tilde_10_feedForward_writer;
        Float64BufferRef comb_tilde_10_feedBack_buffer;
        Index comb_tilde_10_feedBack__maxdelay;
        Int comb_tilde_10_feedBack_sizemode;
        Index comb_tilde_10_feedBack_wrap;
        Int comb_tilde_10_feedBack_reader;
        Int comb_tilde_10_feedBack_writer;
        bool comb_tilde_10_setupDone;
        Float64BufferRef comb_tilde_11_feedForward_buffer;
        Index comb_tilde_11_feedForward__maxdelay;
        Int comb_tilde_11_feedForward_sizemode;
        Index comb_tilde_11_feedForward_wrap;
        Int comb_tilde_11_feedForward_reader;
        Int comb_tilde_11_feedForward_writer;
        Float64BufferRef comb_tilde_11_feedBack_buffer;
        Index comb_tilde_11_feedBack__maxdelay;
        Int comb_tilde_11_feedBack_sizemode;
        Index comb_tilde_11_feedBack_wrap;
        Int comb_tilde_11_feedBack_reader;
        Int comb_tilde_11_feedBack_writer;
        bool comb_tilde_11_setupDone;
        Float64BufferRef comb_tilde_12_feedForward_buffer;
        Index comb_tilde_12_feedForward__maxdelay;
        Int comb_tilde_12_feedForward_sizemode;
        Index comb_tilde_12_feedForward_wrap;
        Int comb_tilde_12_feedForward_reader;
        Int comb_tilde_12_feedForward_writer;
        Float64BufferRef comb_tilde_12_feedBack_buffer;
        Index comb_tilde_12_feedBack__maxdelay;
        Int comb_tilde_12_feedBack_sizemode;
        Index comb_tilde_12_feedBack_wrap;
        Int comb_tilde_12_feedBack_reader;
        Int comb_tilde_12_feedBack_writer;
        bool comb_tilde_12_setupDone;
        Float64BufferRef comb_tilde_13_feedForward_buffer;
        Index comb_tilde_13_feedForward__maxdelay;
        Int comb_tilde_13_feedForward_sizemode;
        Index comb_tilde_13_feedForward_wrap;
        Int comb_tilde_13_feedForward_reader;
        Int comb_tilde_13_feedForward_writer;
        Float64BufferRef comb_tilde_13_feedBack_buffer;
        Index comb_tilde_13_feedBack__maxdelay;
        Int comb_tilde_13_feedBack_sizemode;
        Index comb_tilde_13_feedBack_wrap;
        Int comb_tilde_13_feedBack_reader;
        Int comb_tilde_13_feedBack_writer;
        bool comb_tilde_13_setupDone;
        Float64BufferRef comb_tilde_14_feedForward_buffer;
        Index comb_tilde_14_feedForward__maxdelay;
        Int comb_tilde_14_feedForward_sizemode;
        Index comb_tilde_14_feedForward_wrap;
        Int comb_tilde_14_feedForward_reader;
        Int comb_tilde_14_feedForward_writer;
        Float64BufferRef comb_tilde_14_feedBack_buffer;
        Index comb_tilde_14_feedBack__maxdelay;
        Int comb_tilde_14_feedBack_sizemode;
        Index comb_tilde_14_feedBack_wrap;
        Int comb_tilde_14_feedBack_reader;
        Int comb_tilde_14_feedBack_writer;
        bool comb_tilde_14_setupDone;
        Float64BufferRef comb_tilde_15_feedForward_buffer;
        Index comb_tilde_15_feedForward__maxdelay;
        Int comb_tilde_15_feedForward_sizemode;
        Index comb_tilde_15_feedForward_wrap;
        Int comb_tilde_15_feedForward_reader;
        Int comb_tilde_15_feedForward_writer;
        Float64BufferRef comb_tilde_15_feedBack_buffer;
        Index comb_tilde_15_feedBack__maxdelay;
        Int comb_tilde_15_feedBack_sizemode;
        Index comb_tilde_15_feedBack_wrap;
        Int comb_tilde_15_feedBack_reader;
        Int comb_tilde_15_feedBack_writer;
        bool comb_tilde_15_setupDone;
        Float64BufferRef comb_tilde_16_feedForward_buffer;
        Index comb_tilde_16_feedForward__maxdelay;
        Int comb_tilde_16_feedForward_sizemode;
        Index comb_tilde_16_feedForward_wrap;
        Int comb_tilde_16_feedForward_reader;
        Int comb_tilde_16_feedForward_writer;
        Float64BufferRef comb_tilde_16_feedBack_buffer;
        Index comb_tilde_16_feedBack__maxdelay;
        Int comb_tilde_16_feedBack_sizemode;
        Index comb_tilde_16_feedBack_wrap;
        Int comb_tilde_16_feedBack_reader;
        Int comb_tilde_16_feedBack_writer;
        bool comb_tilde_16_setupDone;
        number stackprotect_count;
        DataRef comb_tilde_10_feedForward_bufferobj;
        DataRef comb_tilde_10_feedBack_bufferobj;
        DataRef comb_tilde_11_feedForward_bufferobj;
        DataRef comb_tilde_11_feedBack_bufferobj;
        DataRef comb_tilde_12_feedForward_bufferobj;
        DataRef comb_tilde_12_feedBack_bufferobj;
        DataRef comb_tilde_13_feedForward_bufferobj;
        DataRef comb_tilde_13_feedBack_bufferobj;
        DataRef comb_tilde_14_feedForward_bufferobj;
        DataRef comb_tilde_14_feedBack_bufferobj;
        DataRef comb_tilde_15_feedForward_bufferobj;
        DataRef comb_tilde_15_feedBack_bufferobj;
        DataRef comb_tilde_16_feedForward_bufferobj;
        DataRef comb_tilde_16_feedBack_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

plate()
{
}

~plate()
{
    delete this->p_01;
    delete this->p_02;
    delete this->p_03;
    delete this->p_04;
    delete this->p_05;
    delete this->p_06;
    delete this->p_07;
    delete this->p_08;
}

plate* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
    getEngine()->flushClockEvents(this, -664351027, false);
    getEngine()->flushClockEvents(this, -281953904, false);
    getEngine()->flushClockEvents(this, -1841075377, false);
    getEngine()->flushClockEvents(this, 512373323, false);
    getEngine()->flushClockEvents(this, 894770446, false);
    getEngine()->flushClockEvents(this, 2071494796, false);
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

SampleIndex currentsampletime() {
    return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
}

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
}

inline number safediv(number num, number denom) {
    return (denom == 0.0 ? 0.0 : num / denom);
}

number safepow(number base, number exponent) {
    return fixnan(rnbo_pow(base, exponent));
}

number scale(
    number x,
    number lowin,
    number hiin,
    number lowout,
    number highout,
    number pow
) {
    auto inscale = this->safediv(1., hiin - lowin);
    number outdiff = highout - lowout;
    number value = (x - lowin) * inscale;

    if (pow != 1) {
        if (value > 0)
            value = this->safepow(value, pow);
        else
            value = -this->safepow(-value, pow);
    }

    value = value * outdiff + lowout;
    return value;
}

ParameterValue fromnormalized(ParameterIndex index, ParameterValue normalizedValue) {
    return this->convertFromNormalizedParameterValue(index, normalizedValue);
}

number maximum(number x, number y) {
    return (x < y ? y : x);
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
    return 1;
}

void processMidiEvent(MillisecondTime time, int port, ConstByteArray data, Index length) {
    this->updateTime(time);
    this->ctlin_01_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_02_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_03_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_04_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_05_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_06_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
}

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
    this->linetilde_01_perform(this->signals[0], n);
    this->signalsend_01_perform(this->signals[0], this->signals[1], n);
    this->linetilde_02_perform(this->signals[0], n);
    this->linetilde_03_perform(this->signals[2], n);
    this->signalsend_02_perform(this->signals[2], this->signals[3], n);
    this->linetilde_04_perform(this->signals[2], n);
    this->signalsend_03_perform(this->signals[2], this->signals[4], n);
    this->p_06_perform(this->signals[4], in1, in2, this->signals[2], this->signals[5], n);
    this->p_03_perform(this->signals[2], this->signals[5], this->signals[6], this->signals[7], n);
    this->linetilde_05_perform(this->signals[5], n);
    this->signalsend_04_perform(this->signals[5], this->signals[2], n);
    this->linetilde_06_perform(this->signals[5], n);
    this->signalsend_05_perform(this->signals[5], this->signals[8], n);

    this->p_05_perform(
        this->signals[8],
        this->signals[1],
        this->signals[3],
        this->signals[4],
        this->signals[6],
        this->signals[5],
        this->signals[9],
        this->signals[10],
        this->signals[11],
        n
    );

    this->p_07_perform(
        this->signals[8],
        this->signals[1],
        this->signals[3],
        this->signals[4],
        this->signals[7],
        this->signals[6],
        this->signals[12],
        this->signals[13],
        this->signals[14],
        n
    );

    this->p_02_perform(
        this->signals[5],
        this->signals[9],
        this->signals[10],
        this->signals[11],
        this->signals[6],
        this->signals[12],
        this->signals[13],
        this->signals[14],
        this->signals[7],
        this->signals[4],
        this->signals[3],
        this->signals[1],
        this->signals[15],
        this->signals[16],
        this->signals[17],
        this->signals[18],
        this->signals[19],
        this->signals[20],
        this->signals[21],
        this->signals[22],
        this->signals[23],
        this->signals[24],
        n
    );

    this->p_04_perform(
        this->signals[8],
        this->signals[2],
        this->signals[7],
        this->signals[4],
        this->signals[3],
        this->signals[1],
        this->signals[15],
        this->signals[16],
        this->signals[17],
        this->signals[14],
        n
    );

    this->p_08_perform(
        this->signals[8],
        this->signals[2],
        this->signals[18],
        this->signals[19],
        this->signals[20],
        this->signals[21],
        this->signals[22],
        this->signals[23],
        this->signals[24],
        this->signals[17],
        n
    );

    this->p_01_perform(
        in1,
        in2,
        this->signals[14],
        this->signals[17],
        this->signals[0],
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
        Index i;

        for (i = 0; i < 25; i++) {
            this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
        }

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

    this->globaltransport_dspsetup(forceDSPSetup);
    this->p_01->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_02->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_03->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_04->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_05->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_06->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_07->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_08->prepareToProcess(sampleRate, maxBlockSize, force);

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
    default:
        {
        return nullptr;
        }
    }
}

DataRefIndex getNumDataRefs() const {
    return 0;
}

void fillDataRef(DataRefIndex , DataRef& ) {}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->p_01->processDataViewUpdate(index, time);
    this->p_02->processDataViewUpdate(index, time);
    this->p_03->processDataViewUpdate(index, time);
    this->p_04->processDataViewUpdate(index, time);
    this->p_05->processDataViewUpdate(index, time);
    this->p_06->processDataViewUpdate(index, time);
    this->p_07->processDataViewUpdate(index, time);
    this->p_08->processDataViewUpdate(index, time);
}

void initialize() {
    this->assign_defaults();
    this->setState();
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

void setState() {
    this->p_01 = new RNBOSubpatcher_17();
    this->p_01->setEngineAndPatcher(this->getEngine(), this);
    this->p_01->initialize();
    this->p_01->setParameterOffset(this->getParameterOffset(this->p_01));
    this->p_02 = new RNBOSubpatcher_18();
    this->p_02->setEngineAndPatcher(this->getEngine(), this);
    this->p_02->initialize();
    this->p_02->setParameterOffset(this->getParameterOffset(this->p_02));
    this->p_03 = new RNBOSubpatcher_19();
    this->p_03->setEngineAndPatcher(this->getEngine(), this);
    this->p_03->initialize();
    this->p_03->setParameterOffset(this->getParameterOffset(this->p_03));
    this->p_04 = new RNBOSubpatcher_20();
    this->p_04->setEngineAndPatcher(this->getEngine(), this);
    this->p_04->initialize();
    this->p_04->setParameterOffset(this->getParameterOffset(this->p_04));
    this->p_05 = new RNBOSubpatcher_21();
    this->p_05->setEngineAndPatcher(this->getEngine(), this);
    this->p_05->initialize();
    this->p_05->setParameterOffset(this->getParameterOffset(this->p_05));
    this->p_06 = new RNBOSubpatcher_22();
    this->p_06->setEngineAndPatcher(this->getEngine(), this);
    this->p_06->initialize();
    this->p_06->setParameterOffset(this->getParameterOffset(this->p_06));
    this->p_07 = new RNBOSubpatcher_23();
    this->p_07->setEngineAndPatcher(this->getEngine(), this);
    this->p_07->initialize();
    this->p_07->setParameterOffset(this->getParameterOffset(this->p_07));
    this->p_08 = new RNBOSubpatcher_24();
    this->p_08->setEngineAndPatcher(this->getEngine(), this);
    this->p_08->initialize();
    this->p_08->setParameterOffset(this->getParameterOffset(this->p_08));
}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_01_getPresetValue(getSubState(preset, "decay"));
    this->param_02_getPresetValue(getSubState(preset, "mix"));
    this->param_03_getPresetValue(getSubState(preset, "jitter"));
    this->param_04_getPresetValue(getSubState(preset, "damp"));
    this->param_05_getPresetValue(getSubState(preset, "size"));
    this->param_06_getPresetValue(getSubState(preset, "diff"));
    this->p_01->getPreset(getSubState(getSubState(preset, "__sps"), "Mix~"));
    this->p_02->getPreset(getSubState(getSubState(preset, "__sps"), "Shuffle~"));
    this->p_03->getPreset(getSubState(getSubState(preset, "__sps"), "Blur~"));
    this->p_04->getPreset(getSubState(getSubState(preset, "__sps"), "CombL~"));
    this->p_05->getPreset(getSubState(getSubState(preset, "__sps"), "RoomA~"));
    this->p_06->getPreset(getSubState(getSubState(preset, "__sps"), "Damp~"));
    this->p_07->getPreset(getSubState(getSubState(preset, "__sps"), "RoomB~"));
    this->p_08->getPreset(getSubState(getSubState(preset, "__sps"), "CombR~"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_04_setPresetValue(getSubState(preset, "damp"));
    this->param_06_setPresetValue(getSubState(preset, "diff"));
    this->param_05_setPresetValue(getSubState(preset, "size"));
    this->param_01_setPresetValue(getSubState(preset, "decay"));
    this->param_03_setPresetValue(getSubState(preset, "jitter"));
    this->param_02_setPresetValue(getSubState(preset, "mix"));
}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(this->_currentTime, tempo, false)) {
        this->p_01->processTempoEvent(time, tempo);
        this->p_02->processTempoEvent(time, tempo);
        this->p_03->processTempoEvent(time, tempo);
        this->p_04->processTempoEvent(time, tempo);
        this->p_05->processTempoEvent(time, tempo);
        this->p_06->processTempoEvent(time, tempo);
        this->p_07->processTempoEvent(time, tempo);
        this->p_08->processTempoEvent(time, tempo);
    }
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(this->_currentTime, state, false)) {
        this->p_01->processTransportEvent(time, state);
        this->p_02->processTransportEvent(time, state);
        this->p_03->processTransportEvent(time, state);
        this->p_04->processTransportEvent(time, state);
        this->p_05->processTransportEvent(time, state);
        this->p_06->processTransportEvent(time, state);
        this->p_07->processTransportEvent(time, state);
        this->p_08->processTransportEvent(time, state);
    }
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(this->_currentTime, beattime, false)) {
        this->p_01->processBeatTimeEvent(time, beattime);
        this->p_02->processBeatTimeEvent(time, beattime);
        this->p_03->processBeatTimeEvent(time, beattime);
        this->p_04->processBeatTimeEvent(time, beattime);
        this->p_05->processBeatTimeEvent(time, beattime);
        this->p_06->processBeatTimeEvent(time, beattime);
        this->p_07->processBeatTimeEvent(time, beattime);
        this->p_08->processBeatTimeEvent(time, beattime);
    }
}

void onSampleRateChanged(double ) {}

void processTimeSignatureEvent(MillisecondTime time, int numerator, int denominator) {
    this->updateTime(time);

    if (this->globaltransport_setTimeSignature(this->_currentTime, numerator, denominator, false)) {
        this->p_01->processTimeSignatureEvent(time, numerator, denominator);
        this->p_02->processTimeSignatureEvent(time, numerator, denominator);
        this->p_03->processTimeSignatureEvent(time, numerator, denominator);
        this->p_04->processTimeSignatureEvent(time, numerator, denominator);
        this->p_05->processTimeSignatureEvent(time, numerator, denominator);
        this->p_06->processTimeSignatureEvent(time, numerator, denominator);
        this->p_07->processTimeSignatureEvent(time, numerator, denominator);
        this->p_08->processTimeSignatureEvent(time, numerator, denominator);
    }
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
    default:
        {
        index -= 6;

        if (index < this->p_01->getNumParameters())
            this->p_01->setParameterValue(index, v, time);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            this->p_02->setParameterValue(index, v, time);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            this->p_03->setParameterValue(index, v, time);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            this->p_04->setParameterValue(index, v, time);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            this->p_05->setParameterValue(index, v, time);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            this->p_06->setParameterValue(index, v, time);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            this->p_07->setParameterValue(index, v, time);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            this->p_08->setParameterValue(index, v, time);

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
    default:
        {
        index -= 6;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterValue(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterValue(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->getParameterValue(index);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterValue(index);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->getParameterValue(index);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->getParameterValue(index);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->getParameterValue(index);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->getParameterValue(index);

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
    return 6 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters() + this->p_08->getNumParameters();
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "decay";
        }
    case 1:
        {
        return "mix";
        }
    case 2:
        {
        return "jitter";
        }
    case 3:
        {
        return "damp";
        }
    case 4:
        {
        return "size";
        }
    case 5:
        {
        return "diff";
        }
    default:
        {
        index -= 6;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterName(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterName(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->getParameterName(index);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterName(index);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->getParameterName(index);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->getParameterName(index);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->getParameterName(index);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->getParameterName(index);

        return "bogus";
        }
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "decay";
        }
    case 1:
        {
        return "mix";
        }
    case 2:
        {
        return "jitter";
        }
    case 3:
        {
        return "damp";
        }
    case 4:
        {
        return "size";
        }
    case 5:
        {
        return "diff";
        }
    default:
        {
        index -= 6;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterId(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterId(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->getParameterId(index);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterId(index);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->getParameterId(index);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->getParameterId(index);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->getParameterId(index);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->getParameterId(index);

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
            info->displayName = "Decay";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 1:
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
            info->displayName = "Mix";
            info->unit = "%";
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
            info->displayName = "Jitter";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 3:
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
            info->displayName = "Damp";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 4:
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
            info->displayName = "Size";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 5:
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
            info->displayName = "Diff";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        default:
            {
            index -= 6;

            if (index < this->p_01->getNumParameters())
                this->p_01->getParameterInfo(index, info);

            index -= this->p_01->getNumParameters();

            if (index < this->p_02->getNumParameters())
                this->p_02->getParameterInfo(index, info);

            index -= this->p_02->getNumParameters();

            if (index < this->p_03->getNumParameters())
                this->p_03->getParameterInfo(index, info);

            index -= this->p_03->getNumParameters();

            if (index < this->p_04->getNumParameters())
                this->p_04->getParameterInfo(index, info);

            index -= this->p_04->getNumParameters();

            if (index < this->p_05->getNumParameters())
                this->p_05->getParameterInfo(index, info);

            index -= this->p_05->getNumParameters();

            if (index < this->p_06->getNumParameters())
                this->p_06->getParameterInfo(index, info);

            index -= this->p_06->getNumParameters();

            if (index < this->p_07->getNumParameters())
                this->p_07->getParameterInfo(index, info);

            index -= this->p_07->getNumParameters();

            if (index < this->p_08->getNumParameters())
                this->p_08->getParameterInfo(index, info);

            break;
            }
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterIndex getParameterOffset(BaseInterface* subpatcher) const {
    if (subpatcher == this->p_01)
        return 6;

    if (subpatcher == this->p_02)
        return 6 + this->p_01->getNumParameters();

    if (subpatcher == this->p_03)
        return 6 + this->p_01->getNumParameters() + this->p_02->getNumParameters();

    if (subpatcher == this->p_04)
        return 6 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters();

    if (subpatcher == this->p_05)
        return 6 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters();

    if (subpatcher == this->p_06)
        return 6 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters();

    if (subpatcher == this->p_07)
        return 6 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters();

    if (subpatcher == this->p_08)
        return 6 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06->getNumParameters() + this->p_07->getNumParameters();

    return 0;
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
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        {
        {
            value = (value < 0 ? 0 : (value > 100 ? 100 : value));
            ParameterValue normalizedValue = (value - 0) / (100 - 0);
            return normalizedValue;
        }
        }
    default:
        {
        index -= 6;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertToNormalizedParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->convertToNormalizedParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->convertToNormalizedParameterValue(index, value);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->convertToNormalizedParameterValue(index, value);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->convertToNormalizedParameterValue(index, value);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->convertToNormalizedParameterValue(index, value);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->convertToNormalizedParameterValue(index, value);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->convertToNormalizedParameterValue(index, value);

        return value;
        }
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (100 - 0);
            }
        }
        }
    default:
        {
        index -= 6;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertFromNormalizedParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->convertFromNormalizedParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->convertFromNormalizedParameterValue(index, value);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->convertFromNormalizedParameterValue(index, value);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->convertFromNormalizedParameterValue(index, value);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->convertFromNormalizedParameterValue(index, value);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->convertFromNormalizedParameterValue(index, value);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->convertFromNormalizedParameterValue(index, value);

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
    default:
        {
        index -= 6;

        if (index < this->p_01->getNumParameters())
            return this->p_01->constrainParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->constrainParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03->getNumParameters())
            return this->p_03->constrainParameterValue(index, value);

        index -= this->p_03->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->constrainParameterValue(index, value);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->constrainParameterValue(index, value);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06->getNumParameters())
            return this->p_06->constrainParameterValue(index, value);

        index -= this->p_06->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->constrainParameterValue(index, value);

        index -= this->p_07->getNumParameters();

        if (index < this->p_08->getNumParameters())
            return this->p_08->constrainParameterValue(index, value);

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

void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
    RNBO_UNUSED(value);
    RNBO_UNUSED(hasValue);
    this->updateTime(time);

    switch (index) {
    case -664351027:
        {
        this->linetilde_01_target_bang();
        break;
        }
    case -281953904:
        {
        this->linetilde_02_target_bang();
        break;
        }
    case -1841075377:
        {
        this->linetilde_03_target_bang();
        break;
        }
    case 512373323:
        {
        this->linetilde_04_target_bang();
        break;
        }
    case 894770446:
        {
        this->linetilde_05_target_bang();
        break;
        }
    case 2071494796:
        {
        this->linetilde_06_target_bang();
        break;
        }
    }
}

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

void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
    this->updateTime(time);

    switch (tag) {
    case TAG("valin"):
        {
        if (TAG("number_obj-26") == objectId)
            this->numberobj_01_valin_set(payload);

        if (TAG("number_obj-2") == objectId)
            this->numberobj_02_valin_set(payload);

        if (TAG("number_obj-119") == objectId)
            this->numberobj_03_valin_set(payload);

        if (TAG("number_obj-67") == objectId)
            this->numberobj_04_valin_set(payload);

        if (TAG("number_obj-3") == objectId)
            this->numberobj_05_valin_set(payload);

        if (TAG("number_obj-54") == objectId)
            this->numberobj_06_valin_set(payload);

        if (TAG("number_obj-4") == objectId)
            this->numberobj_07_valin_set(payload);

        if (TAG("number_obj-9") == objectId)
            this->numberobj_08_valin_set(payload);

        if (TAG("number_obj-21") == objectId)
            this->numberobj_09_valin_set(payload);

        if (TAG("number_obj-23") == objectId)
            this->numberobj_10_valin_set(payload);

        if (TAG("number_obj-16") == objectId)
            this->numberobj_11_valin_set(payload);

        if (TAG("number_obj-18") == objectId)
            this->numberobj_12_valin_set(payload);

        break;
        }
    case TAG("format"):
        {
        if (TAG("number_obj-26") == objectId)
            this->numberobj_01_format_set(payload);

        if (TAG("number_obj-2") == objectId)
            this->numberobj_02_format_set(payload);

        if (TAG("number_obj-119") == objectId)
            this->numberobj_03_format_set(payload);

        if (TAG("number_obj-67") == objectId)
            this->numberobj_04_format_set(payload);

        if (TAG("number_obj-3") == objectId)
            this->numberobj_05_format_set(payload);

        if (TAG("number_obj-54") == objectId)
            this->numberobj_06_format_set(payload);

        if (TAG("number_obj-4") == objectId)
            this->numberobj_07_format_set(payload);

        if (TAG("number_obj-9") == objectId)
            this->numberobj_08_format_set(payload);

        if (TAG("number_obj-21") == objectId)
            this->numberobj_09_format_set(payload);

        if (TAG("number_obj-23") == objectId)
            this->numberobj_10_format_set(payload);

        if (TAG("number_obj-16") == objectId)
            this->numberobj_11_format_set(payload);

        if (TAG("number_obj-18") == objectId)
            this->numberobj_12_format_set(payload);

        break;
        }
    }

    this->p_01->processNumMessage(tag, objectId, time, payload);
    this->p_02->processNumMessage(tag, objectId, time, payload);
    this->p_03->processNumMessage(tag, objectId, time, payload);
    this->p_04->processNumMessage(tag, objectId, time, payload);
    this->p_05->processNumMessage(tag, objectId, time, payload);
    this->p_06->processNumMessage(tag, objectId, time, payload);
    this->p_07->processNumMessage(tag, objectId, time, payload);
    this->p_08->processNumMessage(tag, objectId, time, payload);
}

void processListMessage(
    MessageTag tag,
    MessageTag objectId,
    MillisecondTime time,
    const list& payload
) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);
    this->p_01->processListMessage(tag, objectId, time, payload);
    this->p_02->processListMessage(tag, objectId, time, payload);
    this->p_03->processListMessage(tag, objectId, time, payload);
    this->p_04->processListMessage(tag, objectId, time, payload);
    this->p_05->processListMessage(tag, objectId, time, payload);
    this->p_06->processListMessage(tag, objectId, time, payload);
    this->p_07->processListMessage(tag, objectId, time, payload);
    this->p_08->processListMessage(tag, objectId, time, payload);
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);
    this->p_01->processBangMessage(tag, objectId, time);
    this->p_02->processBangMessage(tag, objectId, time);
    this->p_03->processBangMessage(tag, objectId, time);
    this->p_04->processBangMessage(tag, objectId, time);
    this->p_05->processBangMessage(tag, objectId, time);
    this->p_06->processBangMessage(tag, objectId, time);
    this->p_07->processBangMessage(tag, objectId, time);
    this->p_08->processBangMessage(tag, objectId, time);
}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {
    case TAG("valout"):
        {
        return "valout";
        }
    case TAG("number_obj-26"):
        {
        return "number_obj-26";
        }
    case TAG("setup"):
        {
        return "setup";
        }
    case TAG("number_obj-2"):
        {
        return "number_obj-2";
        }
    case TAG("number_obj-119"):
        {
        return "number_obj-119";
        }
    case TAG("number_obj-67"):
        {
        return "number_obj-67";
        }
    case TAG("number_obj-3"):
        {
        return "number_obj-3";
        }
    case TAG("number_obj-54"):
        {
        return "number_obj-54";
        }
    case TAG("number_obj-4"):
        {
        return "number_obj-4";
        }
    case TAG("number_obj-9"):
        {
        return "number_obj-9";
        }
    case TAG("number_obj-21"):
        {
        return "number_obj-21";
        }
    case TAG("number_obj-23"):
        {
        return "number_obj-23";
        }
    case TAG("number_obj-16"):
        {
        return "number_obj-16";
        }
    case TAG("number_obj-18"):
        {
        return "number_obj-18";
        }
    case TAG("valin"):
        {
        return "valin";
        }
    case TAG("format"):
        {
        return "format";
        }
    }

    auto subpatchResult_0 = this->p_01->resolveTag(tag);

    if (subpatchResult_0)
        return subpatchResult_0;

    auto subpatchResult_1 = this->p_02->resolveTag(tag);

    if (subpatchResult_1)
        return subpatchResult_1;

    auto subpatchResult_2 = this->p_03->resolveTag(tag);

    if (subpatchResult_2)
        return subpatchResult_2;

    auto subpatchResult_3 = this->p_04->resolveTag(tag);

    if (subpatchResult_3)
        return subpatchResult_3;

    auto subpatchResult_4 = this->p_05->resolveTag(tag);

    if (subpatchResult_4)
        return subpatchResult_4;

    auto subpatchResult_5 = this->p_06->resolveTag(tag);

    if (subpatchResult_5)
        return subpatchResult_5;

    auto subpatchResult_6 = this->p_07->resolveTag(tag);

    if (subpatchResult_6)
        return subpatchResult_6;

    auto subpatchResult_7 = this->p_08->resolveTag(tag);

    if (subpatchResult_7)
        return subpatchResult_7;

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

    this->numberobj_02_value_set(v);
}

void param_02_value_set(number v) {
    v = this->param_02_value_constrain(v);
    this->param_02_value = v;
    this->sendParameter(1, false);

    if (this->param_02_value != this->param_02_lastValue) {
        this->getEngine()->presetTouched();
        this->param_02_lastValue = this->param_02_value;
    }

    this->numberobj_05_value_set(v);
}

void param_03_value_set(number v) {
    v = this->param_03_value_constrain(v);
    this->param_03_value = v;
    this->sendParameter(2, false);

    if (this->param_03_value != this->param_03_lastValue) {
        this->getEngine()->presetTouched();
        this->param_03_lastValue = this->param_03_value;
    }

    this->numberobj_06_value_set(v);
}

void param_04_value_set(number v) {
    v = this->param_04_value_constrain(v);
    this->param_04_value = v;
    this->sendParameter(3, false);

    if (this->param_04_value != this->param_04_lastValue) {
        this->getEngine()->presetTouched();
        this->param_04_lastValue = this->param_04_value;
    }

    this->numberobj_08_value_set(v);
}

void param_05_value_set(number v) {
    v = this->param_05_value_constrain(v);
    this->param_05_value = v;
    this->sendParameter(4, false);

    if (this->param_05_value != this->param_05_lastValue) {
        this->getEngine()->presetTouched();
        this->param_05_lastValue = this->param_05_value;
    }

    this->numberobj_10_value_set(v);
}

void param_06_value_set(number v) {
    v = this->param_06_value_constrain(v);
    this->param_06_value = v;
    this->sendParameter(5, false);

    if (this->param_06_value != this->param_06_lastValue) {
        this->getEngine()->presetTouched();
        this->param_06_lastValue = this->param_06_value;
    }

    this->numberobj_12_value_set(v);
}

void numberobj_01_valin_set(number v) {
    this->numberobj_01_value_set(v);
}

void numberobj_01_format_set(number v) {
    this->numberobj_01_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_02_valin_set(number v) {
    this->numberobj_02_value_set(v);
}

void numberobj_02_format_set(number v) {
    this->numberobj_02_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_03_valin_set(number v) {
    this->numberobj_03_value_set(v);
}

void numberobj_03_format_set(number v) {
    this->numberobj_03_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_04_valin_set(number v) {
    this->numberobj_04_value_set(v);
}

void numberobj_04_format_set(number v) {
    this->numberobj_04_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_05_valin_set(number v) {
    this->numberobj_05_value_set(v);
}

void numberobj_05_format_set(number v) {
    this->numberobj_05_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_06_valin_set(number v) {
    this->numberobj_06_value_set(v);
}

void numberobj_06_format_set(number v) {
    this->numberobj_06_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_07_valin_set(number v) {
    this->numberobj_07_value_set(v);
}

void numberobj_07_format_set(number v) {
    this->numberobj_07_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_08_valin_set(number v) {
    this->numberobj_08_value_set(v);
}

void numberobj_08_format_set(number v) {
    this->numberobj_08_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_09_valin_set(number v) {
    this->numberobj_09_value_set(v);
}

void numberobj_09_format_set(number v) {
    this->numberobj_09_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_10_valin_set(number v) {
    this->numberobj_10_value_set(v);
}

void numberobj_10_format_set(number v) {
    this->numberobj_10_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_11_valin_set(number v) {
    this->numberobj_11_value_set(v);
}

void numberobj_11_format_set(number v) {
    this->numberobj_11_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_12_valin_set(number v) {
    this->numberobj_12_value_set(v);
}

void numberobj_12_format_set(number v) {
    this->numberobj_12_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void linetilde_01_target_bang() {}

void linetilde_02_target_bang() {}

void linetilde_03_target_bang() {}

void linetilde_04_target_bang() {}

void linetilde_05_target_bang() {}

void linetilde_06_target_bang() {}

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
    this->p_01->allocateDataRefs();
    this->p_02->allocateDataRefs();
    this->p_03->allocateDataRefs();
    this->p_04->allocateDataRefs();
    this->p_05->allocateDataRefs();
    this->p_06->allocateDataRefs();
    this->p_07->allocateDataRefs();
    this->p_08->allocateDataRefs();
}

void initializeObjects() {
    this->numberobj_01_init();
    this->numberobj_02_init();
    this->numberobj_03_init();
    this->numberobj_04_init();
    this->numberobj_05_init();
    this->numberobj_06_init();
    this->numberobj_07_init();
    this->numberobj_08_init();
    this->numberobj_09_init();
    this->numberobj_10_init();
    this->numberobj_11_init();
    this->numberobj_12_init();
    this->p_01->initializeObjects();
    this->p_02->initializeObjects();
    this->p_03->initializeObjects();
    this->p_04->initializeObjects();
    this->p_05->initializeObjects();
    this->p_06->initializeObjects();
    this->p_07->initializeObjects();
    this->p_08->initializeObjects();
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());
    this->p_01->startup();
    this->p_02->startup();
    this->p_03->startup();
    this->p_04->startup();
    this->p_05->startup();
    this->p_06->startup();
    this->p_07->startup();
    this->p_08->startup();

    {
        this->scheduleParamInit(0, 4);
    }

    {
        this->scheduleParamInit(1, 6);
    }

    {
        this->scheduleParamInit(2, 5);
    }

    {
        this->scheduleParamInit(3, 1);
    }

    {
        this->scheduleParamInit(4, 3);
    }

    {
        this->scheduleParamInit(5, 2);
    }

    this->processParamInitEvents();
}

static number param_01_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void linetilde_01_time_set(number v) {
    this->linetilde_01_time = v;
}

void linetilde_01_segments_set(const list& v) {
    this->linetilde_01_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_01_time == 0) {
            this->linetilde_01_activeRamps->length = 0;
            this->linetilde_01_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_01_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_01_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_01_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_01_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_01_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_01_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_01_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_01_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_01_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_01_activeRamps->push(lastRampValue);
                this->linetilde_01_activeRamps->push(0);
                this->linetilde_01_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_01_keepramp)) {
                        this->linetilde_01_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_01_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_01_activeRamps->push(destinationValue);
                this->linetilde_01_activeRamps->push(inc);
                this->linetilde_01_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_01_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_01_segments_set(converted);
    }
}

void numberobj_01_value_set(number v) {
    this->numberobj_01_value_setter(v);
    v = this->numberobj_01_value;
    number localvalue = v;

    if (this->numberobj_01_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-26"), localvalue, this->_currentTime);
    this->numberobj_01_output_set(localvalue);
}

void scale_01_out_set(const list& v) {
    this->scale_01_out = jsCreateListCopy(v);

    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_01_value_set(converted);
    }
}

void scale_01_input_set(const list& v) {
    this->scale_01_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_01_inlow,
            this->scale_01_inhigh,
            this->scale_01_outlow,
            this->scale_01_outhigh,
            this->scale_01_power
        ));
    }

    this->scale_01_out_set(tmp);
}

void numberobj_02_output_set(number v) {
    {
        list converted = {v};
        this->scale_01_input_set(converted);
    }
}

void numberobj_02_value_set(number v) {
    this->numberobj_02_value_setter(v);
    v = this->numberobj_02_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_02_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-2"), localvalue, this->_currentTime);
    this->numberobj_02_output_set(localvalue);
}

static number param_02_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void linetilde_02_time_set(number v) {
    this->linetilde_02_time = v;
}

void linetilde_02_segments_set(const list& v) {
    this->linetilde_02_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_02_time == 0) {
            this->linetilde_02_activeRamps->length = 0;
            this->linetilde_02_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_02_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_02_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_02_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_02_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_02_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_02_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_02_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_02_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_02_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_02_activeRamps->push(lastRampValue);
                this->linetilde_02_activeRamps->push(0);
                this->linetilde_02_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_02_keepramp)) {
                        this->linetilde_02_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_02_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_02_activeRamps->push(destinationValue);
                this->linetilde_02_activeRamps->push(inc);
                this->linetilde_02_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_03_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_02_segments_set(converted);
    }
}

void numberobj_03_value_set(number v) {
    this->numberobj_03_value_setter(v);
    v = this->numberobj_03_value;
    number localvalue = v;

    if (this->numberobj_03_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-119"), localvalue, this->_currentTime);
    this->numberobj_03_output_set(localvalue);
}

void expr_02_out1_set(number v) {
    this->expr_02_out1 = v;
    this->numberobj_03_value_set(this->expr_02_out1);
}

void expr_02_in1_set(number in1) {
    this->expr_02_in1 = in1;

    this->expr_02_out1_set(
        (this->expr_02_in2 == 0 ? 0 : (this->expr_02_in2 == 0. ? 0. : this->expr_02_in1 / this->expr_02_in2))
    );//#map:/_obj-120:1
}

void numberobj_05_output_set(number v) {
    this->expr_02_in1_set(v);
}

void numberobj_05_value_set(number v) {
    this->numberobj_05_value_setter(v);
    v = this->numberobj_05_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_05_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-3"), localvalue, this->_currentTime);
    this->numberobj_05_output_set(localvalue);
}

static number param_03_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void linetilde_03_time_set(number v) {
    this->linetilde_03_time = v;
}

void linetilde_03_segments_set(const list& v) {
    this->linetilde_03_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_03_time == 0) {
            this->linetilde_03_activeRamps->length = 0;
            this->linetilde_03_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_03_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_03_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_03_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_03_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_03_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_03_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_03_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_03_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_03_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_03_activeRamps->push(lastRampValue);
                this->linetilde_03_activeRamps->push(0);
                this->linetilde_03_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_03_keepramp)) {
                        this->linetilde_03_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_03_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_03_activeRamps->push(destinationValue);
                this->linetilde_03_activeRamps->push(inc);
                this->linetilde_03_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_04_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_03_segments_set(converted);
    }
}

void numberobj_04_value_set(number v) {
    this->numberobj_04_value_setter(v);
    v = this->numberobj_04_value;
    number localvalue = v;

    if (this->numberobj_04_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-67"), localvalue, this->_currentTime);
    this->numberobj_04_output_set(localvalue);
}

void scale_02_out_set(const list& v) {
    this->scale_02_out = jsCreateListCopy(v);

    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_04_value_set(converted);
    }
}

void scale_02_input_set(const list& v) {
    this->scale_02_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_02_inlow,
            this->scale_02_inhigh,
            this->scale_02_outlow,
            this->scale_02_outhigh,
            this->scale_02_power
        ));
    }

    this->scale_02_out_set(tmp);
}

void numberobj_06_output_set(number v) {
    {
        list converted = {v};
        this->scale_02_input_set(converted);
    }
}

void numberobj_06_value_set(number v) {
    this->numberobj_06_value_setter(v);
    v = this->numberobj_06_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_06_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-54"), localvalue, this->_currentTime);
    this->numberobj_06_output_set(localvalue);
}

static number param_04_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void linetilde_04_time_set(number v) {
    this->linetilde_04_time = v;
}

void linetilde_04_segments_set(const list& v) {
    this->linetilde_04_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_04_time == 0) {
            this->linetilde_04_activeRamps->length = 0;
            this->linetilde_04_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_04_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_04_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_04_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_04_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_04_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_04_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_04_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_04_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_04_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_04_activeRamps->push(lastRampValue);
                this->linetilde_04_activeRamps->push(0);
                this->linetilde_04_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_04_keepramp)) {
                        this->linetilde_04_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_04_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_04_activeRamps->push(destinationValue);
                this->linetilde_04_activeRamps->push(inc);
                this->linetilde_04_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_07_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_04_segments_set(converted);
    }
}

void numberobj_07_value_set(number v) {
    this->numberobj_07_value_setter(v);
    v = this->numberobj_07_value;
    number localvalue = v;

    if (this->numberobj_07_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-4"), localvalue, this->_currentTime);
    this->numberobj_07_output_set(localvalue);
}

void scale_03_out_set(const list& v) {
    this->scale_03_out = jsCreateListCopy(v);

    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_07_value_set(converted);
    }
}

void scale_03_input_set(const list& v) {
    this->scale_03_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_03_inlow,
            this->scale_03_inhigh,
            this->scale_03_outlow,
            this->scale_03_outhigh,
            this->scale_03_power
        ));
    }

    this->scale_03_out_set(tmp);
}

void numberobj_08_output_set(number v) {
    {
        list converted = {v};
        this->scale_03_input_set(converted);
    }
}

void numberobj_08_value_set(number v) {
    this->numberobj_08_value_setter(v);
    v = this->numberobj_08_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_08_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-9"), localvalue, this->_currentTime);
    this->numberobj_08_output_set(localvalue);
}

static number param_05_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void linetilde_06_time_set(number v) {
    this->linetilde_06_time = v;
}

void linetilde_06_segments_set(const list& v) {
    this->linetilde_06_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_06_time == 0) {
            this->linetilde_06_activeRamps->length = 0;
            this->linetilde_06_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_06_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_06_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_06_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_06_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_06_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_06_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_06_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_06_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_06_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_06_activeRamps->push(lastRampValue);
                this->linetilde_06_activeRamps->push(0);
                this->linetilde_06_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_06_keepramp)) {
                        this->linetilde_06_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_06_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_06_activeRamps->push(destinationValue);
                this->linetilde_06_activeRamps->push(inc);
                this->linetilde_06_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_09_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_06_segments_set(converted);
    }
}

void numberobj_09_value_set(number v) {
    this->numberobj_09_value_setter(v);
    v = this->numberobj_09_value;
    number localvalue = v;

    if (this->numberobj_09_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-21"), localvalue, this->_currentTime);
    this->numberobj_09_output_set(localvalue);
}

void scale_04_out_set(const list& v) {
    this->scale_04_out = jsCreateListCopy(v);

    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_09_value_set(converted);
    }
}

void scale_04_input_set(const list& v) {
    this->scale_04_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_04_inlow,
            this->scale_04_inhigh,
            this->scale_04_outlow,
            this->scale_04_outhigh,
            this->scale_04_power
        ));
    }

    this->scale_04_out_set(tmp);
}

void numberobj_10_output_set(number v) {
    {
        list converted = {v};
        this->scale_04_input_set(converted);
    }
}

void numberobj_10_value_set(number v) {
    this->numberobj_10_value_setter(v);
    v = this->numberobj_10_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_10_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-23"), localvalue, this->_currentTime);
    this->numberobj_10_output_set(localvalue);
}

static number param_06_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void linetilde_05_time_set(number v) {
    this->linetilde_05_time = v;
}

void linetilde_05_segments_set(const list& v) {
    this->linetilde_05_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_05_time == 0) {
            this->linetilde_05_activeRamps->length = 0;
            this->linetilde_05_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_05_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_05_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_05_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_05_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_05_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_05_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_05_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_05_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_05_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_05_activeRamps->push(lastRampValue);
                this->linetilde_05_activeRamps->push(0);
                this->linetilde_05_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_05_keepramp)) {
                        this->linetilde_05_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_05_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_05_activeRamps->push(destinationValue);
                this->linetilde_05_activeRamps->push(inc);
                this->linetilde_05_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_11_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_05_segments_set(converted);
    }
}

void numberobj_11_value_set(number v) {
    this->numberobj_11_value_setter(v);
    v = this->numberobj_11_value;
    number localvalue = v;

    if (this->numberobj_11_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-16"), localvalue, this->_currentTime);
    this->numberobj_11_output_set(localvalue);
}

void expr_07_out1_set(number v) {
    this->expr_07_out1 = v;
    this->numberobj_11_value_set(this->expr_07_out1);
}

void expr_07_in1_set(number in1) {
    this->expr_07_in1 = in1;

    this->expr_07_out1_set(
        (this->expr_07_in2 == 0 ? 0 : (this->expr_07_in2 == 0. ? 0. : this->expr_07_in1 / this->expr_07_in2))
    );//#map:/_obj-17:1
}

void numberobj_12_output_set(number v) {
    this->expr_07_in1_set(v);
}

void numberobj_12_value_set(number v) {
    this->numberobj_12_value_setter(v);
    v = this->numberobj_12_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_12_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-18"), localvalue, this->_currentTime);
    this->numberobj_12_output_set(localvalue);
}

void ctlin_01_outchannel_set(number ) {}

void ctlin_01_outcontroller_set(number ) {}

void fromnormalized_01_output_set(number v) {
    this->param_01_value_set(v);
}

void fromnormalized_01_input_set(number v) {
    this->fromnormalized_01_output_set(this->fromnormalized(0, v));
}

void expr_01_out1_set(number v) {
    this->expr_01_out1 = v;
    this->fromnormalized_01_input_set(this->expr_01_out1);
}

void expr_01_in1_set(number in1) {
    this->expr_01_in1 = in1;
    this->expr_01_out1_set(this->expr_01_in1 * this->expr_01_in2);//#map:expr_01:1
}

void ctlin_01_value_set(number v) {
    this->expr_01_in1_set(v);
}

void ctlin_01_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_01_channel || this->ctlin_01_channel == -1) && (data[1] == this->ctlin_01_controller || this->ctlin_01_controller == -1)) {
        this->ctlin_01_outchannel_set(channel);
        this->ctlin_01_outcontroller_set(data[1]);
        this->ctlin_01_value_set(data[2]);
        this->ctlin_01_status = 0;
    }
}

void ctlin_02_outchannel_set(number ) {}

void ctlin_02_outcontroller_set(number ) {}

void fromnormalized_02_output_set(number v) {
    this->param_02_value_set(v);
}

void fromnormalized_02_input_set(number v) {
    this->fromnormalized_02_output_set(this->fromnormalized(1, v));
}

void expr_03_out1_set(number v) {
    this->expr_03_out1 = v;
    this->fromnormalized_02_input_set(this->expr_03_out1);
}

void expr_03_in1_set(number in1) {
    this->expr_03_in1 = in1;
    this->expr_03_out1_set(this->expr_03_in1 * this->expr_03_in2);//#map:expr_03:1
}

void ctlin_02_value_set(number v) {
    this->expr_03_in1_set(v);
}

void ctlin_02_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_02_channel || this->ctlin_02_channel == -1) && (data[1] == this->ctlin_02_controller || this->ctlin_02_controller == -1)) {
        this->ctlin_02_outchannel_set(channel);
        this->ctlin_02_outcontroller_set(data[1]);
        this->ctlin_02_value_set(data[2]);
        this->ctlin_02_status = 0;
    }
}

void ctlin_03_outchannel_set(number ) {}

void ctlin_03_outcontroller_set(number ) {}

void fromnormalized_03_output_set(number v) {
    this->param_03_value_set(v);
}

void fromnormalized_03_input_set(number v) {
    this->fromnormalized_03_output_set(this->fromnormalized(2, v));
}

void expr_04_out1_set(number v) {
    this->expr_04_out1 = v;
    this->fromnormalized_03_input_set(this->expr_04_out1);
}

void expr_04_in1_set(number in1) {
    this->expr_04_in1 = in1;
    this->expr_04_out1_set(this->expr_04_in1 * this->expr_04_in2);//#map:expr_04:1
}

void ctlin_03_value_set(number v) {
    this->expr_04_in1_set(v);
}

void ctlin_03_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_03_channel || this->ctlin_03_channel == -1) && (data[1] == this->ctlin_03_controller || this->ctlin_03_controller == -1)) {
        this->ctlin_03_outchannel_set(channel);
        this->ctlin_03_outcontroller_set(data[1]);
        this->ctlin_03_value_set(data[2]);
        this->ctlin_03_status = 0;
    }
}

void ctlin_04_outchannel_set(number ) {}

void ctlin_04_outcontroller_set(number ) {}

void fromnormalized_04_output_set(number v) {
    this->param_04_value_set(v);
}

void fromnormalized_04_input_set(number v) {
    this->fromnormalized_04_output_set(this->fromnormalized(3, v));
}

void expr_05_out1_set(number v) {
    this->expr_05_out1 = v;
    this->fromnormalized_04_input_set(this->expr_05_out1);
}

void expr_05_in1_set(number in1) {
    this->expr_05_in1 = in1;
    this->expr_05_out1_set(this->expr_05_in1 * this->expr_05_in2);//#map:expr_05:1
}

void ctlin_04_value_set(number v) {
    this->expr_05_in1_set(v);
}

void ctlin_04_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_04_channel || this->ctlin_04_channel == -1) && (data[1] == this->ctlin_04_controller || this->ctlin_04_controller == -1)) {
        this->ctlin_04_outchannel_set(channel);
        this->ctlin_04_outcontroller_set(data[1]);
        this->ctlin_04_value_set(data[2]);
        this->ctlin_04_status = 0;
    }
}

void ctlin_05_outchannel_set(number ) {}

void ctlin_05_outcontroller_set(number ) {}

void fromnormalized_05_output_set(number v) {
    this->param_05_value_set(v);
}

void fromnormalized_05_input_set(number v) {
    this->fromnormalized_05_output_set(this->fromnormalized(4, v));
}

void expr_06_out1_set(number v) {
    this->expr_06_out1 = v;
    this->fromnormalized_05_input_set(this->expr_06_out1);
}

void expr_06_in1_set(number in1) {
    this->expr_06_in1 = in1;
    this->expr_06_out1_set(this->expr_06_in1 * this->expr_06_in2);//#map:expr_06:1
}

void ctlin_05_value_set(number v) {
    this->expr_06_in1_set(v);
}

void ctlin_05_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_05_channel || this->ctlin_05_channel == -1) && (data[1] == this->ctlin_05_controller || this->ctlin_05_controller == -1)) {
        this->ctlin_05_outchannel_set(channel);
        this->ctlin_05_outcontroller_set(data[1]);
        this->ctlin_05_value_set(data[2]);
        this->ctlin_05_status = 0;
    }
}

void ctlin_06_outchannel_set(number ) {}

void ctlin_06_outcontroller_set(number ) {}

void fromnormalized_06_output_set(number v) {
    this->param_06_value_set(v);
}

void fromnormalized_06_input_set(number v) {
    this->fromnormalized_06_output_set(this->fromnormalized(5, v));
}

void expr_08_out1_set(number v) {
    this->expr_08_out1 = v;
    this->fromnormalized_06_input_set(this->expr_08_out1);
}

void expr_08_in1_set(number in1) {
    this->expr_08_in1 = in1;
    this->expr_08_out1_set(this->expr_08_in1 * this->expr_08_in2);//#map:expr_08:1
}

void ctlin_06_value_set(number v) {
    this->expr_08_in1_set(v);
}

void ctlin_06_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_06_channel || this->ctlin_06_channel == -1) && (data[1] == this->ctlin_06_controller || this->ctlin_06_controller == -1)) {
        this->ctlin_06_outchannel_set(channel);
        this->ctlin_06_outcontroller_set(data[1]);
        this->ctlin_06_value_set(data[2]);
        this->ctlin_06_status = 0;
    }
}

void linetilde_01_perform(SampleValue * out, Index n) {
    auto __linetilde_01_time = this->linetilde_01_time;
    auto __linetilde_01_keepramp = this->linetilde_01_keepramp;
    auto __linetilde_01_currentValue = this->linetilde_01_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_01_activeRamps->length)) {
        while ((bool)(this->linetilde_01_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_01_activeRamps[0];
            number inc = this->linetilde_01_activeRamps[1];
            number rampTimeInSamples = this->linetilde_01_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_01_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_01_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_01_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -664351027,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_01_keepramp))) {
                        __linetilde_01_time = 0;
                    }
                }
            }

            __linetilde_01_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_01_currentValue;
        i++;
    }

    this->linetilde_01_currentValue = __linetilde_01_currentValue;
    this->linetilde_01_time = __linetilde_01_time;
}

void signalsend_01_perform(const SampleValue * input, SampleValue * send, Index n) {
    for (Index i = 0; i < n; i++) {
        send[(Index)i] = input[(Index)i];
    }
}

void linetilde_02_perform(SampleValue * out, Index n) {
    auto __linetilde_02_time = this->linetilde_02_time;
    auto __linetilde_02_keepramp = this->linetilde_02_keepramp;
    auto __linetilde_02_currentValue = this->linetilde_02_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_02_activeRamps->length)) {
        while ((bool)(this->linetilde_02_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_02_activeRamps[0];
            number inc = this->linetilde_02_activeRamps[1];
            number rampTimeInSamples = this->linetilde_02_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_02_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_02_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_02_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -281953904,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_02_keepramp))) {
                        __linetilde_02_time = 0;
                    }
                }
            }

            __linetilde_02_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_02_currentValue;
        i++;
    }

    this->linetilde_02_currentValue = __linetilde_02_currentValue;
    this->linetilde_02_time = __linetilde_02_time;
}

void linetilde_03_perform(SampleValue * out, Index n) {
    auto __linetilde_03_time = this->linetilde_03_time;
    auto __linetilde_03_keepramp = this->linetilde_03_keepramp;
    auto __linetilde_03_currentValue = this->linetilde_03_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_03_activeRamps->length)) {
        while ((bool)(this->linetilde_03_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_03_activeRamps[0];
            number inc = this->linetilde_03_activeRamps[1];
            number rampTimeInSamples = this->linetilde_03_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_03_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_03_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_03_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -1841075377,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_03_keepramp))) {
                        __linetilde_03_time = 0;
                    }
                }
            }

            __linetilde_03_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_03_currentValue;
        i++;
    }

    this->linetilde_03_currentValue = __linetilde_03_currentValue;
    this->linetilde_03_time = __linetilde_03_time;
}

void signalsend_02_perform(const SampleValue * input, SampleValue * send, Index n) {
    for (Index i = 0; i < n; i++) {
        send[(Index)i] = input[(Index)i];
    }
}

void linetilde_04_perform(SampleValue * out, Index n) {
    auto __linetilde_04_time = this->linetilde_04_time;
    auto __linetilde_04_keepramp = this->linetilde_04_keepramp;
    auto __linetilde_04_currentValue = this->linetilde_04_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_04_activeRamps->length)) {
        while ((bool)(this->linetilde_04_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_04_activeRamps[0];
            number inc = this->linetilde_04_activeRamps[1];
            number rampTimeInSamples = this->linetilde_04_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_04_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_04_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_04_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        512373323,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_04_keepramp))) {
                        __linetilde_04_time = 0;
                    }
                }
            }

            __linetilde_04_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_04_currentValue;
        i++;
    }

    this->linetilde_04_currentValue = __linetilde_04_currentValue;
    this->linetilde_04_time = __linetilde_04_time;
}

void signalsend_03_perform(const SampleValue * input, SampleValue * send, Index n) {
    for (Index i = 0; i < n; i++) {
        send[(Index)i] = input[(Index)i];
    }
}

void p_06_perform(
    const SampleValue * target_Damp,
    const SampleValue * in1,
    const SampleValue * in2,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    // subpatcher: Damp~
    ConstSampleArray<3> ins = {in1, in2, target_Damp};

    SampleArray<2> outs = {out1, out2};
    this->p_06->process(ins, 3, outs, 2, n);
}

void p_03_perform(
    const SampleValue * in1,
    const SampleValue * in2,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    // subpatcher: Blur~
    ConstSampleArray<2> ins = {in1, in2};

    SampleArray<2> outs = {out1, out2};
    this->p_03->process(ins, 2, outs, 2, n);
}

void linetilde_05_perform(SampleValue * out, Index n) {
    auto __linetilde_05_time = this->linetilde_05_time;
    auto __linetilde_05_keepramp = this->linetilde_05_keepramp;
    auto __linetilde_05_currentValue = this->linetilde_05_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_05_activeRamps->length)) {
        while ((bool)(this->linetilde_05_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_05_activeRamps[0];
            number inc = this->linetilde_05_activeRamps[1];
            number rampTimeInSamples = this->linetilde_05_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_05_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_05_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_05_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        894770446,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_05_keepramp))) {
                        __linetilde_05_time = 0;
                    }
                }
            }

            __linetilde_05_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_05_currentValue;
        i++;
    }

    this->linetilde_05_currentValue = __linetilde_05_currentValue;
    this->linetilde_05_time = __linetilde_05_time;
}

void signalsend_04_perform(const SampleValue * input, SampleValue * send, Index n) {
    for (Index i = 0; i < n; i++) {
        send[(Index)i] = input[(Index)i];
    }
}

void linetilde_06_perform(SampleValue * out, Index n) {
    auto __linetilde_06_time = this->linetilde_06_time;
    auto __linetilde_06_keepramp = this->linetilde_06_keepramp;
    auto __linetilde_06_currentValue = this->linetilde_06_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_06_activeRamps->length)) {
        while ((bool)(this->linetilde_06_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_06_activeRamps[0];
            number inc = this->linetilde_06_activeRamps[1];
            number rampTimeInSamples = this->linetilde_06_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_06_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_06_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_06_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        2071494796,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_06_keepramp))) {
                        __linetilde_06_time = 0;
                    }
                }
            }

            __linetilde_06_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_06_currentValue;
        i++;
    }

    this->linetilde_06_currentValue = __linetilde_06_currentValue;
    this->linetilde_06_time = __linetilde_06_time;
}

void signalsend_05_perform(const SampleValue * input, SampleValue * send, Index n) {
    for (Index i = 0; i < n; i++) {
        send[(Index)i] = input[(Index)i];
    }
}

void p_05_perform(
    const SampleValue * target_Size,
    const SampleValue * target_Decay,
    const SampleValue * target_Jitter,
    const SampleValue * target_Damp,
    const SampleValue * in1,
    SampleValue * out1,
    SampleValue * out2,
    SampleValue * out3,
    SampleValue * out4,
    Index n
) {
    // subpatcher: RoomA~
    ConstSampleArray<5> ins = {in1, target_Size, target_Decay, target_Jitter, target_Damp};

    SampleArray<4> outs = {out1, out2, out3, out4};
    this->p_05->process(ins, 5, outs, 4, n);
}

void p_07_perform(
    const SampleValue * target_Size,
    const SampleValue * target_Decay,
    const SampleValue * target_Jitter,
    const SampleValue * target_Damp,
    const SampleValue * in1,
    SampleValue * out1,
    SampleValue * out2,
    SampleValue * out3,
    SampleValue * out4,
    Index n
) {
    // subpatcher: RoomB~
    ConstSampleArray<5> ins = {in1, target_Size, target_Decay, target_Jitter, target_Damp};

    SampleArray<4> outs = {out1, out2, out3, out4};
    this->p_07->process(ins, 5, outs, 4, n);
}

void p_02_perform(
    const SampleValue * in1,
    const SampleValue * in2,
    const SampleValue * in3,
    const SampleValue * in4,
    const SampleValue * in5,
    const SampleValue * in6,
    const SampleValue * in7,
    const SampleValue * in8,
    SampleValue * out1,
    SampleValue * out2,
    SampleValue * out3,
    SampleValue * out4,
    SampleValue * out5,
    SampleValue * out6,
    SampleValue * out7,
    SampleValue * out8,
    SampleValue * out9,
    SampleValue * out10,
    SampleValue * out11,
    SampleValue * out12,
    SampleValue * out13,
    SampleValue * out14,
    Index n
) {
    // subpatcher: Shuffle~
    ConstSampleArray<8> ins = {in1, in2, in3, in4, in5, in6, in7, in8};

    SampleArray<14> outs = {
        out1,
        out2,
        out3,
        out4,
        out5,
        out6,
        out7,
        out8,
        out9,
        out10,
        out11,
        out12,
        out13,
        out14
    };

    this->p_02->process(ins, 8, outs, 14, n);
}

void p_04_perform(
    const SampleValue * target_Size,
    const SampleValue * target_Diff,
    const SampleValue * in1,
    const SampleValue * in2,
    const SampleValue * in3,
    const SampleValue * in4,
    const SampleValue * in5,
    const SampleValue * in6,
    const SampleValue * in7,
    SampleValue * out1,
    Index n
) {
    // subpatcher: CombL~
    ConstSampleArray<9> ins = {in1, in2, in3, in4, in5, in6, in7, target_Size, target_Diff};

    SampleArray<1> outs = {out1};
    this->p_04->process(ins, 9, outs, 1, n);
}

void p_08_perform(
    const SampleValue * target_Size,
    const SampleValue * target_Diff,
    const SampleValue * in1,
    const SampleValue * in2,
    const SampleValue * in3,
    const SampleValue * in4,
    const SampleValue * in5,
    const SampleValue * in6,
    const SampleValue * in7,
    SampleValue * out1,
    Index n
) {
    // subpatcher: CombR~
    ConstSampleArray<9> ins = {in1, in2, in3, in4, in5, in6, in7, target_Size, target_Diff};

    SampleArray<1> outs = {out1};
    this->p_08->process(ins, 9, outs, 1, n);
}

void p_01_perform(
    const SampleValue * in1,
    const SampleValue * in2,
    const SampleValue * in3,
    const SampleValue * in4,
    const SampleValue * in5,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    // subpatcher: Mix~
    ConstSampleArray<5> ins = {in1, in2, in3, in4, in5};

    SampleArray<2> outs = {out1, out2};
    this->p_01->process(ins, 5, outs, 2, n);
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

void numberobj_01_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_01_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_01_value = localvalue;
}

void numberobj_02_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_02_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_02_value = localvalue;
}

void numberobj_03_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_03_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_03_value = localvalue;
}

void numberobj_04_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_04_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_04_value = localvalue;
}

void numberobj_05_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_05_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_05_value = localvalue;
}

void numberobj_06_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_06_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_06_value = localvalue;
}

void numberobj_07_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_07_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_07_value = localvalue;
}

void numberobj_08_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_08_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_08_value = localvalue;
}

void numberobj_09_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_09_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_09_value = localvalue;
}

void numberobj_10_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_10_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_10_value = localvalue;
}

void numberobj_11_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_11_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_11_value = localvalue;
}

void numberobj_12_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_12_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_12_value = localvalue;
}

void numberobj_01_init() {
    this->numberobj_01_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-26"), 1, this->_currentTime);
}

void numberobj_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_01_value;
}

void numberobj_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_01_value_set(preset["value"]);
}

void numberobj_02_init() {
    this->numberobj_02_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-2"), 1, this->_currentTime);
}

void numberobj_02_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_02_value;
}

void numberobj_02_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_02_value_set(preset["value"]);
}

void param_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_01_value;
}

void param_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_01_value_set(preset["value"]);
}

void numberobj_03_init() {
    this->numberobj_03_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-119"), 1, this->_currentTime);
}

void numberobj_03_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_03_value;
}

void numberobj_03_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_03_value_set(preset["value"]);
}

void numberobj_04_init() {
    this->numberobj_04_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-67"), 1, this->_currentTime);
}

void numberobj_04_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_04_value;
}

void numberobj_04_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_04_value_set(preset["value"]);
}

void numberobj_05_init() {
    this->numberobj_05_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-3"), 1, this->_currentTime);
}

void numberobj_05_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_05_value;
}

void numberobj_05_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_05_value_set(preset["value"]);
}

void param_02_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_02_value;
}

void param_02_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_02_value_set(preset["value"]);
}

void numberobj_06_init() {
    this->numberobj_06_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-54"), 1, this->_currentTime);
}

void numberobj_06_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_06_value;
}

void numberobj_06_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_06_value_set(preset["value"]);
}

void param_03_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_03_value;
}

void param_03_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_03_value_set(preset["value"]);
}

void numberobj_07_init() {
    this->numberobj_07_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-4"), 1, this->_currentTime);
}

void numberobj_07_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_07_value;
}

void numberobj_07_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_07_value_set(preset["value"]);
}

void numberobj_08_init() {
    this->numberobj_08_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-9"), 1, this->_currentTime);
}

void numberobj_08_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_08_value;
}

void numberobj_08_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_08_value_set(preset["value"]);
}

void param_04_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_04_value;
}

void param_04_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_04_value_set(preset["value"]);
}

void numberobj_09_init() {
    this->numberobj_09_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-21"), 1, this->_currentTime);
}

void numberobj_09_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_09_value;
}

void numberobj_09_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_09_value_set(preset["value"]);
}

void numberobj_10_init() {
    this->numberobj_10_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-23"), 1, this->_currentTime);
}

void numberobj_10_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_10_value;
}

void numberobj_10_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_10_value_set(preset["value"]);
}

void param_05_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_05_value;
}

void param_05_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_05_value_set(preset["value"]);
}

void numberobj_11_init() {
    this->numberobj_11_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-16"), 1, this->_currentTime);
}

void numberobj_11_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_11_value;
}

void numberobj_11_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_11_value_set(preset["value"]);
}

void numberobj_12_init() {
    this->numberobj_12_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-18"), 1, this->_currentTime);
}

void numberobj_12_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_12_value;
}

void numberobj_12_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_12_value_set(preset["value"]);
}

void param_06_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_06_value;
}

void param_06_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_06_value_set(preset["value"]);
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
    p_01_target = 0;
    linetilde_01_time = 50;
    linetilde_01_keepramp = 0;
    numberobj_01_value = 0;
    numberobj_01_value_setter(numberobj_01_value);
    scale_01_inlow = 0;
    scale_01_inhigh = 100;
    scale_01_outlow = 0.05;
    scale_01_outhigh = 0.99;
    scale_01_power = 1;
    numberobj_02_value = 0;
    numberobj_02_value_setter(numberobj_02_value);
    param_01_value = 50;
    p_02_target = 0;
    p_03_target = 0;
    p_04_target = 0;
    p_05_target = 0;
    p_06_target = 0;
    p_07_target = 0;
    p_08_target = 0;
    numberobj_03_value = 0;
    numberobj_03_value_setter(numberobj_03_value);
    expr_02_in1 = 0;
    expr_02_in2 = 100;
    expr_02_out1 = 0;
    linetilde_02_time = 10;
    linetilde_02_keepramp = 0;
    numberobj_04_value = 0;
    numberobj_04_value_setter(numberobj_04_value);
    scale_02_inlow = 0;
    scale_02_inhigh = 100;
    scale_02_outlow = 0;
    scale_02_outhigh = 50;
    scale_02_power = 1.322;
    numberobj_05_value = 0;
    numberobj_05_value_setter(numberobj_05_value);
    param_02_value = 50;
    linetilde_03_time = 50;
    linetilde_03_keepramp = 0;
    numberobj_06_value = 0;
    numberobj_06_value_setter(numberobj_06_value);
    param_03_value = 50;
    linetilde_04_time = 50;
    linetilde_04_keepramp = 0;
    numberobj_07_value = 0;
    numberobj_07_value_setter(numberobj_07_value);
    scale_03_inlow = 0;
    scale_03_inhigh = 100;
    scale_03_outlow = 12000;
    scale_03_outhigh = 300;
    scale_03_power = 0.333333;
    numberobj_08_value = 0;
    numberobj_08_value_setter(numberobj_08_value);
    param_04_value = 50;
    linetilde_05_time = 50;
    linetilde_05_keepramp = 0;
    linetilde_06_time = 50;
    linetilde_06_keepramp = 0;
    numberobj_09_value = 0;
    numberobj_09_value_setter(numberobj_09_value);
    scale_04_inlow = 0;
    scale_04_inhigh = 100;
    scale_04_outlow = 0.01;
    scale_04_outhigh = 1.6;
    scale_04_power = 1;
    numberobj_10_value = 0;
    numberobj_10_value_setter(numberobj_10_value);
    param_05_value = 50;
    numberobj_11_value = 0;
    numberobj_11_value_setter(numberobj_11_value);
    expr_07_in1 = 0;
    expr_07_in2 = 100;
    expr_07_out1 = 0;
    numberobj_12_value = 0;
    numberobj_12_value_setter(numberobj_12_value);
    param_06_value = 50;
    ctlin_01_input = 0;
    ctlin_01_controller = 4;
    ctlin_01_channel = -1;
    expr_01_in1 = 0;
    expr_01_in2 = 0.007874015748;
    expr_01_out1 = 0;
    ctlin_02_input = 0;
    ctlin_02_controller = 6;
    ctlin_02_channel = -1;
    expr_03_in1 = 0;
    expr_03_in2 = 0.007874015748;
    expr_03_out1 = 0;
    ctlin_03_input = 0;
    ctlin_03_controller = 5;
    ctlin_03_channel = -1;
    expr_04_in1 = 0;
    expr_04_in2 = 0.007874015748;
    expr_04_out1 = 0;
    ctlin_04_input = 0;
    ctlin_04_controller = 1;
    ctlin_04_channel = -1;
    expr_05_in1 = 0;
    expr_05_in2 = 0.007874015748;
    expr_05_out1 = 0;
    ctlin_05_input = 0;
    ctlin_05_controller = 3;
    ctlin_05_channel = -1;
    expr_06_in1 = 0;
    expr_06_in2 = 0.007874015748;
    expr_06_out1 = 0;
    ctlin_06_input = 0;
    ctlin_06_controller = 2;
    ctlin_06_channel = -1;
    expr_08_in1 = 0;
    expr_08_in2 = 0.007874015748;
    expr_08_out1 = 0;
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    signals[0] = nullptr;
    signals[1] = nullptr;
    signals[2] = nullptr;
    signals[3] = nullptr;
    signals[4] = nullptr;
    signals[5] = nullptr;
    signals[6] = nullptr;
    signals[7] = nullptr;
    signals[8] = nullptr;
    signals[9] = nullptr;
    signals[10] = nullptr;
    signals[11] = nullptr;
    signals[12] = nullptr;
    signals[13] = nullptr;
    signals[14] = nullptr;
    signals[15] = nullptr;
    signals[16] = nullptr;
    signals[17] = nullptr;
    signals[18] = nullptr;
    signals[19] = nullptr;
    signals[20] = nullptr;
    signals[21] = nullptr;
    signals[22] = nullptr;
    signals[23] = nullptr;
    signals[24] = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    linetilde_01_currentValue = 0;
    numberobj_01_currentFormat = 6;
    numberobj_01_lastValue = 0;
    numberobj_02_currentFormat = 6;
    numberobj_02_lastValue = 0;
    param_01_lastValue = 0;
    numberobj_03_currentFormat = 6;
    numberobj_03_lastValue = 0;
    linetilde_02_currentValue = 0;
    numberobj_04_currentFormat = 6;
    numberobj_04_lastValue = 0;
    numberobj_05_currentFormat = 6;
    numberobj_05_lastValue = 0;
    param_02_lastValue = 0;
    linetilde_03_currentValue = 0;
    numberobj_06_currentFormat = 6;
    numberobj_06_lastValue = 0;
    param_03_lastValue = 0;
    linetilde_04_currentValue = 0;
    numberobj_07_currentFormat = 6;
    numberobj_07_lastValue = 0;
    numberobj_08_currentFormat = 6;
    numberobj_08_lastValue = 0;
    param_04_lastValue = 0;
    linetilde_05_currentValue = 0;
    linetilde_06_currentValue = 0;
    numberobj_09_currentFormat = 6;
    numberobj_09_lastValue = 0;
    numberobj_10_currentFormat = 6;
    numberobj_10_lastValue = 0;
    param_05_lastValue = 0;
    numberobj_11_currentFormat = 6;
    numberobj_11_lastValue = 0;
    numberobj_12_currentFormat = 6;
    numberobj_12_lastValue = 0;
    param_06_lastValue = 0;
    ctlin_01_status = 0;
    ctlin_01_byte1 = -1;
    ctlin_01_inchan = 0;
    ctlin_02_status = 0;
    ctlin_02_byte1 = -1;
    ctlin_02_inchan = 0;
    ctlin_03_status = 0;
    ctlin_03_byte1 = -1;
    ctlin_03_inchan = 0;
    ctlin_04_status = 0;
    ctlin_04_byte1 = -1;
    ctlin_04_inchan = 0;
    ctlin_05_status = 0;
    ctlin_05_byte1 = -1;
    ctlin_05_inchan = 0;
    ctlin_06_status = 0;
    ctlin_06_byte1 = -1;
    ctlin_06_inchan = 0;
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

    number p_01_target;
    list linetilde_01_segments;
    number linetilde_01_time;
    number linetilde_01_keepramp;
    number numberobj_01_value;
    list scale_01_input;
    number scale_01_inlow;
    number scale_01_inhigh;
    number scale_01_outlow;
    number scale_01_outhigh;
    number scale_01_power;
    list scale_01_out;
    number numberobj_02_value;
    number param_01_value;
    number p_02_target;
    number p_03_target;
    number p_04_target;
    number p_05_target;
    number p_06_target;
    number p_07_target;
    number p_08_target;
    number numberobj_03_value;
    number expr_02_in1;
    number expr_02_in2;
    number expr_02_out1;
    list linetilde_02_segments;
    number linetilde_02_time;
    number linetilde_02_keepramp;
    number numberobj_04_value;
    list scale_02_input;
    number scale_02_inlow;
    number scale_02_inhigh;
    number scale_02_outlow;
    number scale_02_outhigh;
    number scale_02_power;
    list scale_02_out;
    number numberobj_05_value;
    number param_02_value;
    list linetilde_03_segments;
    number linetilde_03_time;
    number linetilde_03_keepramp;
    number numberobj_06_value;
    number param_03_value;
    list linetilde_04_segments;
    number linetilde_04_time;
    number linetilde_04_keepramp;
    number numberobj_07_value;
    list scale_03_input;
    number scale_03_inlow;
    number scale_03_inhigh;
    number scale_03_outlow;
    number scale_03_outhigh;
    number scale_03_power;
    list scale_03_out;
    number numberobj_08_value;
    number param_04_value;
    list linetilde_05_segments;
    number linetilde_05_time;
    number linetilde_05_keepramp;
    list linetilde_06_segments;
    number linetilde_06_time;
    number linetilde_06_keepramp;
    number numberobj_09_value;
    list scale_04_input;
    number scale_04_inlow;
    number scale_04_inhigh;
    number scale_04_outlow;
    number scale_04_outhigh;
    number scale_04_power;
    list scale_04_out;
    number numberobj_10_value;
    number param_05_value;
    number numberobj_11_value;
    number expr_07_in1;
    number expr_07_in2;
    number expr_07_out1;
    number numberobj_12_value;
    number param_06_value;
    number ctlin_01_input;
    number ctlin_01_controller;
    number ctlin_01_channel;
    number expr_01_in1;
    number expr_01_in2;
    number expr_01_out1;
    number ctlin_02_input;
    number ctlin_02_controller;
    number ctlin_02_channel;
    number expr_03_in1;
    number expr_03_in2;
    number expr_03_out1;
    number ctlin_03_input;
    number ctlin_03_controller;
    number ctlin_03_channel;
    number expr_04_in1;
    number expr_04_in2;
    number expr_04_out1;
    number ctlin_04_input;
    number ctlin_04_controller;
    number ctlin_04_channel;
    number expr_05_in1;
    number expr_05_in2;
    number expr_05_out1;
    number ctlin_05_input;
    number ctlin_05_controller;
    number ctlin_05_channel;
    number expr_06_in1;
    number expr_06_in2;
    number expr_06_out1;
    number ctlin_06_input;
    number ctlin_06_controller;
    number ctlin_06_channel;
    number expr_08_in1;
    number expr_08_in2;
    number expr_08_out1;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    SampleValue * signals[25];
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    list linetilde_01_activeRamps;
    number linetilde_01_currentValue;
    Int numberobj_01_currentFormat;
    number numberobj_01_lastValue;
    Int numberobj_02_currentFormat;
    number numberobj_02_lastValue;
    number param_01_lastValue;
    Int numberobj_03_currentFormat;
    number numberobj_03_lastValue;
    list linetilde_02_activeRamps;
    number linetilde_02_currentValue;
    Int numberobj_04_currentFormat;
    number numberobj_04_lastValue;
    Int numberobj_05_currentFormat;
    number numberobj_05_lastValue;
    number param_02_lastValue;
    list linetilde_03_activeRamps;
    number linetilde_03_currentValue;
    Int numberobj_06_currentFormat;
    number numberobj_06_lastValue;
    number param_03_lastValue;
    list linetilde_04_activeRamps;
    number linetilde_04_currentValue;
    Int numberobj_07_currentFormat;
    number numberobj_07_lastValue;
    Int numberobj_08_currentFormat;
    number numberobj_08_lastValue;
    number param_04_lastValue;
    list linetilde_05_activeRamps;
    number linetilde_05_currentValue;
    list linetilde_06_activeRamps;
    number linetilde_06_currentValue;
    Int numberobj_09_currentFormat;
    number numberobj_09_lastValue;
    Int numberobj_10_currentFormat;
    number numberobj_10_lastValue;
    number param_05_lastValue;
    Int numberobj_11_currentFormat;
    number numberobj_11_lastValue;
    Int numberobj_12_currentFormat;
    number numberobj_12_lastValue;
    number param_06_lastValue;
    int ctlin_01_status;
    int ctlin_01_byte1;
    int ctlin_01_inchan;
    int ctlin_02_status;
    int ctlin_02_byte1;
    int ctlin_02_inchan;
    int ctlin_03_status;
    int ctlin_03_byte1;
    int ctlin_03_inchan;
    int ctlin_04_status;
    int ctlin_04_byte1;
    int ctlin_04_inchan;
    int ctlin_05_status;
    int ctlin_05_byte1;
    int ctlin_05_inchan;
    int ctlin_06_status;
    int ctlin_06_byte1;
    int ctlin_06_inchan;
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
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;
    RNBOSubpatcher_17* p_01;
    RNBOSubpatcher_18* p_02;
    RNBOSubpatcher_19* p_03;
    RNBOSubpatcher_20* p_04;
    RNBOSubpatcher_21* p_05;
    RNBOSubpatcher_22* p_06;
    RNBOSubpatcher_23* p_07;
    RNBOSubpatcher_24* p_08;

};

PatcherInterface* createplate()
{
    return new plate();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr plateFactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return createplate;
}

} // end RNBO namespace

