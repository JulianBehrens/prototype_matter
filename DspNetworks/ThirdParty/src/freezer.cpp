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

class freezer : public PatcherInterfaceImpl {
public:

class RNBOSubpatcher_47 : public PatcherInterfaceImpl {
    
    friend class freezer;
    
    public:
    
    RNBOSubpatcher_47()
    {
    }
    
    ~RNBOSubpatcher_47()
    {
    }
    
    virtual freezer* getPatcher() const {
        return static_cast<freezer *>(_parentPatcher);
    }
    
    freezer* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -611950441, false);
        getEngine()->flushClockEvents(this, -1584063977, false);
        getEngine()->flushClockEvents(this, -1357044121, false);
        getEngine()->flushClockEvents(this, 1812006465, false);
        getEngine()->flushClockEvents(this, 252884992, false);
    }
    
    MillisecondTime currenttime() {
        return this->_currentTime;
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
        RNBO_UNUSED(numOutputs);
        RNBO_UNUSED(outputs);
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
        const SampleValue * in2 = (numInputs >= 2 && inputs[1] ? inputs[1] : this->zeroBuffer);
    
        this->rampsmooth_tilde_01_perform(
            this->rampsmooth_tilde_01_x,
            this->rampsmooth_tilde_01_up,
            this->rampsmooth_tilde_01_down,
            this->signals[0],
            n
        );
    
        this->dspexpr_03_perform(this->signals[0], this->dspexpr_03_in2, this->signals[1], n);
    
        this->average_absolute_tilde_01_perform(
            this->signals[1],
            this->average_absolute_tilde_01_windowSize,
            this->average_absolute_tilde_01_reset,
            this->signals[0],
            n
        );
    
        this->peakamp_02_perform(this->signals[0], n);
        this->dspexpr_01_perform(in1, in2, this->signals[0], n);
        this->dspexpr_02_perform(this->signals[0], this->dspexpr_02_in2, this->signals[2], n);
        this->peakamp_01_perform(this->signals[2], n);
        this->dspexpr_05_perform(this->dspexpr_05_in1, this->dspexpr_05_in2, this->signals[2], n);
        this->dspexpr_04_perform(this->signals[2], this->dspexpr_04_in2, this->signals[0], n);
        this->thresh_tilde_01_perform(this->signals[1], this->signals[0], this->signals[2], this->signals[3], n);
        this->edge_01_perform(this->signals[3], n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
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
    
        this->rampsmooth_tilde_01_dspsetup(forceDSPSetup);
        this->average_absolute_tilde_01_dspsetup(forceDSPSetup);
        this->peakamp_02_dspsetup(forceDSPSetup);
        this->peakamp_01_dspsetup(forceDSPSetup);
        this->edge_01_dspsetup(forceDSPSetup);
    
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
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -611950441:
            {
            this->edge_01_onout_bang();
            break;
            }
        case -1584063977:
            {
            this->edge_01_offout_bang();
            break;
            }
        case -1357044121:
            {
            this->delay_01_out_bang();
            break;
            }
        case 1812006465:
            {
            this->peakamp_02_output_set(value);
            break;
            }
        case 252884992:
            {
            this->peakamp_01_output_set(value);
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
            if (TAG("Attack~/number_obj-74") == objectId)
                this->numberobj_01_valin_set(payload);
    
            if (TAG("Attack~/toggle_obj-64") == objectId)
                this->toggle_01_valin_set(payload);
    
            if (TAG("Attack~/number_obj-29") == objectId)
                this->numberobj_02_valin_set(payload);
    
            if (TAG("Attack~/toggle_obj-16") == objectId)
                this->toggle_02_valin_set(payload);
    
            if (TAG("Attack~/number_obj-61") == objectId)
                this->numberobj_03_valin_set(payload);
    
            if (TAG("Attack~/number_obj-58") == objectId)
                this->numberobj_04_valin_set(payload);
    
            if (TAG("Attack~/number_obj-38") == objectId)
                this->numberobj_05_valin_set(payload);
    
            if (TAG("Attack~/number_obj-124") == objectId)
                this->numberobj_06_valin_set(payload);
    
            break;
            }
        case TAG("format"):
            {
            if (TAG("Attack~/number_obj-74") == objectId)
                this->numberobj_01_format_set(payload);
    
            if (TAG("Attack~/number_obj-29") == objectId)
                this->numberobj_02_format_set(payload);
    
            if (TAG("Attack~/number_obj-61") == objectId)
                this->numberobj_03_format_set(payload);
    
            if (TAG("Attack~/number_obj-58") == objectId)
                this->numberobj_04_format_set(payload);
    
            if (TAG("Attack~/number_obj-38") == objectId)
                this->numberobj_05_format_set(payload);
    
            if (TAG("Attack~/number_obj-124") == objectId)
                this->numberobj_06_format_set(payload);
    
            break;
            }
        case TAG("listin"):
            {
            if (TAG("Attack~/message_obj-44") == objectId)
                this->message_01_listin_number_set(payload);
    
            break;
            }
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("listin"):
            {
            if (TAG("Attack~/message_obj-44") == objectId)
                this->message_01_listin_list_set(payload);
    
            break;
            }
        }
    }
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("bangin"):
            {
            if (TAG("Attack~/button_obj-78") == objectId)
                this->button_01_bangin_bang();
    
            if (TAG("Attack~/button_obj-80") == objectId)
                this->button_02_bangin_bang();
    
            if (TAG("Attack~/button_obj-53") == objectId)
                this->button_03_bangin_bang();
    
            if (TAG("Attack~/button_obj-43") == objectId)
                this->button_04_bangin_bang();
    
            break;
            }
        case TAG("listin"):
            {
            if (TAG("Attack~/message_obj-44") == objectId)
                this->message_01_listin_bang_bang();
    
            break;
            }
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("bangout"):
            {
            return "bangout";
            }
        case TAG("Attack~/button_obj-78"):
            {
            return "Attack~/button_obj-78";
            }
        case TAG("valout"):
            {
            return "valout";
            }
        case TAG("Attack~/number_obj-74"):
            {
            return "Attack~/number_obj-74";
            }
        case TAG("setup"):
            {
            return "setup";
            }
        case TAG("Attack~/button_obj-80"):
            {
            return "Attack~/button_obj-80";
            }
        case TAG("Attack~/toggle_obj-64"):
            {
            return "Attack~/toggle_obj-64";
            }
        case TAG("Attack~/number_obj-29"):
            {
            return "Attack~/number_obj-29";
            }
        case TAG("Attack~/toggle_obj-16"):
            {
            return "Attack~/toggle_obj-16";
            }
        case TAG("Attack~/button_obj-53"):
            {
            return "Attack~/button_obj-53";
            }
        case TAG("listout"):
            {
            return "listout";
            }
        case TAG("Attack~/message_obj-44"):
            {
            return "Attack~/message_obj-44";
            }
        case TAG("attack"):
            {
            return "attack";
            }
        case TAG(""):
            {
            return "";
            }
        case TAG("Attack~/number_obj-61"):
            {
            return "Attack~/number_obj-61";
            }
        case TAG("Attack~/number_obj-58"):
            {
            return "Attack~/number_obj-58";
            }
        case TAG("Attack~/button_obj-43"):
            {
            return "Attack~/button_obj-43";
            }
        case TAG("Attack~/number_obj-38"):
            {
            return "Attack~/number_obj-38";
            }
        case TAG("Attack~/number_obj-124"):
            {
            return "Attack~/number_obj-124";
            }
        case TAG("bangin"):
            {
            return "bangin";
            }
        case TAG("valin"):
            {
            return "valin";
            }
        case TAG("format"):
            {
            return "format";
            }
        case TAG("listin"):
            {
            return "listin";
            }
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 3:
            {
            return addressOf(this->average_absolute_tilde_01_av_bufferobj);
            break;
            }
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 1;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void zeroDataRef(DataRef& ref) {
        ref->setZero();
    }
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 3) {
            this->average_absolute_tilde_01_av_buffer = new Float64Buffer(this->average_absolute_tilde_01_av_bufferobj);
        }
    }
    
    void initialize() {
        this->average_absolute_tilde_01_av_bufferobj = initDataRef("average_absolute_tilde_01_av_bufferobj", true, nullptr, "buffer~");
        this->assign_defaults();
        this->setState();
        this->average_absolute_tilde_01_av_bufferobj->setIndex(3);
        this->average_absolute_tilde_01_av_buffer = new Float64Buffer(this->average_absolute_tilde_01_av_bufferobj);
    }
    
    protected:
    
    void button_01_bangin_bang() {
        this->button_01_bangval_bang();
    }
    
    void numberobj_01_valin_set(number v) {
        this->numberobj_01_value_set(v);
    }
    
    void numberobj_01_format_set(number v) {
        this->numberobj_01_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void edge_01_onout_bang() {
        this->button_01_bangval_bang();
    }
    
    void edge_01_offout_bang() {
        this->button_02_bangval_bang();
    }
    
    void button_02_bangin_bang() {
        this->button_02_bangval_bang();
    }
    
    void toggle_01_valin_set(number v) {
        this->toggle_01_value_number_set(v);
    }
    
    void numberobj_02_valin_set(number v) {
        this->numberobj_02_value_set(v);
    }
    
    void numberobj_02_format_set(number v) {
        this->numberobj_02_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void toggle_02_valin_set(number v) {
        this->toggle_02_value_number_set(v);
    }
    
    void button_03_bangin_bang() {
        this->button_03_bangval_bang();
    }
    
    void message_01_listin_list_set(const list& v) {
        this->message_01_set_set(v);
    }
    
    void message_01_listin_number_set(number v) {
        this->message_01_set_set(v);
    }
    
    void message_01_listin_bang_bang() {
        this->message_01_trigger_bang();
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
    
    void eventinlet_01_out1_bang_bang() {
        this->numberobj_03_value_bang();
    }
    
    void eventinlet_01_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_03_value_set(converted);
        }
    }
    
    void button_04_bangin_bang() {
        this->button_04_bangval_bang();
    }
    
    void delay_01_out_bang() {
        this->button_04_bangval_bang();
    }
    
    void numberobj_05_valin_set(number v) {
        this->numberobj_05_value_set(v);
    }
    
    void numberobj_05_format_set(number v) {
        this->numberobj_05_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void eventinlet_02_out1_bang_bang() {
        this->numberobj_05_value_bang();
    }
    
    void eventinlet_02_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_05_value_set(converted);
        }
    }
    
    void numberobj_06_valin_set(number v) {
        this->numberobj_06_value_set(v);
    }
    
    void numberobj_06_format_set(number v) {
        this->numberobj_06_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void peakamp_02_output_set(number v) {
        this->peakamp_02_output = v;
        this->dspexpr_05_in1_set(v);
    }
    
    void peakamp_01_output_set(number v) {
        this->peakamp_01_output = v;
        this->rampsmooth_tilde_01_x_set(v);
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
        return 0;
    }
    
    void initializeObjects() {
        this->numberobj_01_init();
        this->thresh_tilde_01_init();
        this->average_absolute_tilde_01_av_init();
        this->numberobj_02_init();
        this->message_01_init();
        this->numberobj_03_init();
        this->numberobj_04_init();
        this->numberobj_05_init();
        this->numberobj_06_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->average_absolute_tilde_01_av_buffer = this->average_absolute_tilde_01_av_buffer->allocateIfNeeded();
    
        if (this->average_absolute_tilde_01_av_bufferobj->hasRequestedSize()) {
            if (this->average_absolute_tilde_01_av_bufferobj->wantsFill())
                this->zeroDataRef(this->average_absolute_tilde_01_av_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(3);
        }
    }
    
    void expr_02_in2_set(number v) {
        this->expr_02_in2 = v;
    }
    
    void numberobj_04_output_set(number v) {
        this->expr_02_in2_set(v);
    }
    
    void numberobj_04_value_set(number v) {
        this->numberobj_04_value_setter(v);
        v = this->numberobj_04_value;
        number localvalue = v;
    
        if (this->numberobj_04_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Attack~/number_obj-58"),
            localvalue,
            this->_currentTime
        );
    
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
    
    void dspexpr_05_in2_set(number v) {
        this->dspexpr_05_in2 = v;
    }
    
    void numberobj_02_output_set(number v) {
        this->dspexpr_05_in2_set(v);
    }
    
    void numberobj_02_value_set(number v) {
        this->numberobj_02_value_setter(v);
        v = this->numberobj_02_value;
        number localvalue = v;
    
        if (this->numberobj_02_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Attack~/number_obj-29"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_02_output_set(localvalue);
    }
    
    void scale_01_out_set(const list& v) {
        this->scale_01_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_02_value_set(converted);
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
    
    void dspexpr_02_in2_set(number v) {
        this->dspexpr_02_in2 = v;
    }
    
    void toggle_01_output_set(number v) {
        this->dspexpr_02_in2_set(v);
    }
    
    void toggle_01_value_number_set(number v) {
        this->toggle_01_value_number_setter(v);
        v = this->toggle_01_value_number;
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Attack~/toggle_obj-64"), v, this->_currentTime);
        this->toggle_01_output_set(v);
    }
    
    void expr_01_out1_set(number v) {
        this->expr_01_out1 = v;
        this->toggle_01_value_number_set(this->expr_01_out1);
    }
    
    void expr_01_in1_set(number in1) {
        this->expr_01_in1 = in1;
        this->expr_01_out1_set(this->expr_01_in1 > this->expr_01_in2);//#map:Attack~/>_obj-59:1
    }
    
    void numberobj_03_output_set(number v) {
        {
            list converted = {v};
            this->scale_02_input_set(converted);
        }
    
        {
            list converted = {v};
            this->scale_01_input_set(converted);
        }
    
        this->expr_01_in1_set(v);
    }
    
    void numberobj_03_value_set(number v) {
        this->numberobj_03_value_setter(v);
        v = this->numberobj_03_value;
        number localvalue = v;
    
        if (this->numberobj_03_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Attack~/number_obj-61"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_03_output_set(localvalue);
    }
    
    void eventinlet_01_out1_number_set(number v) {
        this->numberobj_03_value_set(v);
    }
    
    static number delay_01_time_constrain(number v) {
        if (v < 0)
            v = 0;
    
        return v;
    }
    
    void delay_01_time_set(number v) {
        v = this->delay_01_time_constrain(v);
        this->delay_01_time = v;
    }
    
    void numberobj_06_output_set(number v) {
        this->delay_01_time_set(v);
    }
    
    void numberobj_06_value_set(number v) {
        this->numberobj_06_value_setter(v);
        v = this->numberobj_06_value;
        number localvalue = v;
    
        if (this->numberobj_06_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Attack~/number_obj-124"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_06_output_set(localvalue);
    }
    
    void scale_03_out_set(const list& v) {
        this->scale_03_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_06_value_set(converted);
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
    
    void numberobj_05_output_set(number v) {
        {
            list converted = {v};
            this->scale_03_input_set(converted);
        }
    }
    
    void numberobj_05_value_set(number v) {
        this->numberobj_05_value_setter(v);
        v = this->numberobj_05_value;
        number localvalue = v;
    
        if (this->numberobj_05_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Attack~/number_obj-38"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_05_output_set(localvalue);
    }
    
    void eventinlet_02_out1_number_set(number v) {
        this->numberobj_05_value_set(v);
    }
    
    void delay_01_stop_bang() {
        this->getEngine()->flushClockEvents(this, -1357044121, false);;
    }
    
    void delay_01_input_bang() {
        if ((bool)(!(bool)(this->delay_01_delayall)))
            this->delay_01_stop_bang();
    
        this->getEngine()->scheduleClockEvent(this, -1357044121, this->delay_01_time + this->_currentTime);;
    }
    
    void outport_01_input_list_set(const list& v) {
        this->getEngine()->sendListMessage(TAG("attack"), TAG(""), v, this->_currentTime);
    }
    
    void message_01_out_set(const list& v) {
        this->outport_01_input_list_set(v);
    }
    
    void message_01_trigger_bang() {
        this->message_01_out_set(this->message_01_set);
    }
    
    void button_03_output_bang() {
        this->delay_01_input_bang();
        this->message_01_trigger_bang();
    }
    
    void button_03_bangval_bang() {
        this->getEngine()->sendBangMessage(TAG("bangout"), TAG("Attack~/button_obj-53"), this->_currentTime);;
        this->button_03_output_bang();
    }
    
    void select_01_match1_bang() {
        this->button_03_bangval_bang();
    }
    
    void select_01_nomatch_number_set(number ) {}
    
    void select_01_input_number_set(number v) {
        if (v == this->select_01_test1)
            this->select_01_match1_bang();
        else
            this->select_01_nomatch_number_set(v);
    }
    
    void toggle_02_output_set(number v) {
        this->select_01_input_number_set(v);
    }
    
    void toggle_02_value_number_set(number v) {
        this->toggle_02_value_number_setter(v);
        v = this->toggle_02_value_number;
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Attack~/toggle_obj-16"), v, this->_currentTime);
        this->toggle_02_output_set(v);
    }
    
    void expr_02_out1_set(number v) {
        this->expr_02_out1 = v;
        this->toggle_02_value_number_set(this->expr_02_out1);
    }
    
    void expr_02_in1_set(number in1) {
        this->expr_02_in1 = in1;
        this->expr_02_out1_set(this->expr_02_in1 >= this->expr_02_in2);//#map:Attack~/>=_obj-26:1
    }
    
    void numberobj_01_output_set(number v) {
        this->expr_02_in1_set(v);
    }
    
    void numberobj_01_value_set(number v) {
        this->numberobj_01_value_setter(v);
        v = this->numberobj_01_value;
        number localvalue = v;
    
        if (this->numberobj_01_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Attack~/number_obj-74"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_01_output_set(localvalue);
    }
    
    void timer_01_out_set(number v) {
        this->numberobj_01_value_set(v);
    }
    
    void timer_01_end_bang() {
        this->timer_01_elapsed = this->currenttime();
        this->timer_01_out_set(this->timer_01_elapsed - this->timer_01_starttime);
        this->timer_01_elapsed = 0;
    }
    
    void button_01_output_bang() {
        this->timer_01_end_bang();
    }
    
    void button_01_bangval_bang() {
        this->getEngine()->sendBangMessage(TAG("bangout"), TAG("Attack~/button_obj-78"), this->_currentTime);;
        this->button_01_output_bang();
    }
    
    void timer_01_start_bang() {
        this->timer_01_starttime = this->currenttime();
    }
    
    void button_02_output_bang() {
        this->timer_01_start_bang();
    }
    
    void button_02_bangval_bang() {
        this->getEngine()->sendBangMessage(TAG("bangout"), TAG("Attack~/button_obj-80"), this->_currentTime);;
        this->button_02_output_bang();
    }
    
    void message_01_set_set(const list& v) {
        this->message_01_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Attack~/message_obj-44"), v, this->_currentTime);
    }
    
    void numberobj_03_value_bang() {
        number v = this->numberobj_03_value;
        number localvalue = v;
    
        if (this->numberobj_03_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Attack~/number_obj-61"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_03_output_set(localvalue);
    }
    
    void eventoutlet_01_in1_bang_bang() {
        ;
        this->getPatcher()->updateTime(this->_currentTime);
        this->getPatcher()->p_01_out1_bang_bang();
    }
    
    void button_04_output_bang() {
        this->eventoutlet_01_in1_bang_bang();
    }
    
    void button_04_bangval_bang() {
        this->getEngine()->sendBangMessage(TAG("bangout"), TAG("Attack~/button_obj-43"), this->_currentTime);;
        this->button_04_output_bang();
    }
    
    void numberobj_05_value_bang() {
        number v = this->numberobj_05_value;
        number localvalue = v;
    
        if (this->numberobj_05_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Attack~/number_obj-38"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_05_output_set(localvalue);
    }
    
    void rampsmooth_tilde_01_perform(number x, number up, number down, SampleValue * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __rampsmooth_tilde_01_increment = this->rampsmooth_tilde_01_increment;
        auto __rampsmooth_tilde_01_index = this->rampsmooth_tilde_01_index;
        auto __rampsmooth_tilde_01_prev = this->rampsmooth_tilde_01_prev;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (this->rampsmooth_tilde_01_d_next(x) != 0.) {
                if (x > __rampsmooth_tilde_01_prev) {
                    number _up = 480;
    
                    if (_up < 1)
                        _up = 1;
    
                    __rampsmooth_tilde_01_index = _up;
                    __rampsmooth_tilde_01_increment = (x - __rampsmooth_tilde_01_prev) / _up;
                } else if (x < __rampsmooth_tilde_01_prev) {
                    number _down = 480;
    
                    if (_down < 1)
                        _down = 1;
    
                    __rampsmooth_tilde_01_index = _down;
                    __rampsmooth_tilde_01_increment = (x - __rampsmooth_tilde_01_prev) / _down;
                }
            }
    
            if (__rampsmooth_tilde_01_index > 0) {
                __rampsmooth_tilde_01_prev += __rampsmooth_tilde_01_increment;
                __rampsmooth_tilde_01_index -= 1;
            } else {
                __rampsmooth_tilde_01_prev = x;
            }
    
            out1[(Index)i] = __rampsmooth_tilde_01_prev;
        }
    
        this->rampsmooth_tilde_01_prev = __rampsmooth_tilde_01_prev;
        this->rampsmooth_tilde_01_index = __rampsmooth_tilde_01_index;
        this->rampsmooth_tilde_01_increment = __rampsmooth_tilde_01_increment;
    }
    
    void dspexpr_03_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 100;//#map:_###_obj_###_:1
        }
    }
    
    void average_absolute_tilde_01_perform(
        const Sample * x,
        number windowSize,
        number reset,
        SampleValue * out1,
        Index n
    ) {
        RNBO_UNUSED(reset);
        RNBO_UNUSED(windowSize);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = this->average_absolute_tilde_01_av_next(rnbo_abs(x[(Index)i]), 22000, 0);
        }
    }
    
    void dspexpr_05_in1_set(number v) {
        this->dspexpr_05_in1 = v;
    }
    
    void peakamp_02_perform(const SampleValue * input_signal, Index n) {
        auto __peakamp_02_index = this->peakamp_02_index;
        auto __peakamp_02_lastMaximum = this->peakamp_02_lastMaximum;
        auto __peakamp_02_maxIndex = this->peakamp_02_maxIndex;
        auto __peakamp_02_interval = this->peakamp_02_interval;
    
        for (Index i = 0; i < n; i++) {
            if ((bool)(this->peakamp_02_d_next(__peakamp_02_interval))) {
                __peakamp_02_maxIndex = this->mstosamps(__peakamp_02_interval);
            }
    
            number temp = rnbo_abs(input_signal[(Index)i]);
    
            if (temp > __peakamp_02_lastMaximum) {
                __peakamp_02_lastMaximum = temp;
            }
    
            __peakamp_02_index++;
    
            if (__peakamp_02_maxIndex > 0 && __peakamp_02_index >= __peakamp_02_maxIndex) {
                this->getEngine()->scheduleClockEventWithValue(
                    this,
                    1812006465,
                    this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                    __peakamp_02_lastMaximum
                );;
    
                __peakamp_02_lastMaximum = 0;
            }
    
            if (__peakamp_02_maxIndex == 0 || __peakamp_02_index >= __peakamp_02_maxIndex) {
                __peakamp_02_index = 0;
            }
        }
    
        this->peakamp_02_maxIndex = __peakamp_02_maxIndex;
        this->peakamp_02_lastMaximum = __peakamp_02_lastMaximum;
        this->peakamp_02_index = __peakamp_02_index;
    }
    
    void dspexpr_01_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_02_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void rampsmooth_tilde_01_x_set(number v) {
        this->rampsmooth_tilde_01_x = v;
    }
    
    void peakamp_01_perform(const SampleValue * input_signal, Index n) {
        auto __peakamp_01_index = this->peakamp_01_index;
        auto __peakamp_01_lastMaximum = this->peakamp_01_lastMaximum;
        auto __peakamp_01_maxIndex = this->peakamp_01_maxIndex;
        auto __peakamp_01_interval = this->peakamp_01_interval;
    
        for (Index i = 0; i < n; i++) {
            if ((bool)(this->peakamp_01_d_next(__peakamp_01_interval))) {
                __peakamp_01_maxIndex = this->mstosamps(__peakamp_01_interval);
            }
    
            number temp = rnbo_abs(input_signal[(Index)i]);
    
            if (temp > __peakamp_01_lastMaximum) {
                __peakamp_01_lastMaximum = temp;
            }
    
            __peakamp_01_index++;
    
            if (__peakamp_01_maxIndex > 0 && __peakamp_01_index >= __peakamp_01_maxIndex) {
                this->getEngine()->scheduleClockEventWithValue(
                    this,
                    252884992,
                    this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                    __peakamp_01_lastMaximum
                );;
    
                __peakamp_01_lastMaximum = 0;
            }
    
            if (__peakamp_01_maxIndex == 0 || __peakamp_01_index >= __peakamp_01_maxIndex) {
                __peakamp_01_index = 0;
            }
        }
    
        this->peakamp_01_maxIndex = __peakamp_01_maxIndex;
        this->peakamp_01_lastMaximum = __peakamp_01_lastMaximum;
        this->peakamp_01_index = __peakamp_01_index;
    }
    
    void dspexpr_05_perform(number in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1 + in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_04_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + -1;//#map:_###_obj_###_:1
        }
    }
    
    void thresh_tilde_01_perform(
        const Sample * x,
        const Sample * lowThresh,
        const Sample * highThresh,
        SampleValue * out1,
        Index n
    ) {
        auto __thresh_tilde_01_isAbove = this->thresh_tilde_01_isAbove;
        Index i;
    
        for (i = 0; i < n; i++) {
            if ((bool)(__thresh_tilde_01_isAbove)) {
                if (lowThresh[(Index)i] == highThresh[(Index)i]) {
                    if (x[(Index)i] < lowThresh[(Index)i])
                        __thresh_tilde_01_isAbove = false;
                } else if (x[(Index)i] <= lowThresh[(Index)i])
                    __thresh_tilde_01_isAbove = false;
            } else if (x[(Index)i] >= highThresh[(Index)i]) {
                __thresh_tilde_01_isAbove = true;
            }
    
            out1[(Index)i] = ((bool)(__thresh_tilde_01_isAbove) ? 1. : 0.);
        }
    
        this->thresh_tilde_01_isAbove = __thresh_tilde_01_isAbove;
    }
    
    void edge_01_perform(const SampleValue * input, Index n) {
        auto __edge_01_currentState = this->edge_01_currentState;
    
        for (Index i = 0; i < n; i++) {
            if (__edge_01_currentState == 1) {
                if (input[(Index)i] == 0.) {
                    this->getEngine()->scheduleClockEvent(this, -1584063977, this->sampsToMs(i) + this->_currentTime);;
                    __edge_01_currentState = 0;
                }
            } else {
                if (input[(Index)i] != 0.) {
                    this->getEngine()->scheduleClockEvent(this, -611950441, this->sampsToMs(i) + this->_currentTime);;
                    __edge_01_currentState = 1;
                }
            }
        }
    
        this->edge_01_currentState = __edge_01_currentState;
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
    
    void toggle_01_value_number_setter(number v) {
        this->toggle_01_value_number = (v != 0 ? 1 : 0);
    }
    
    void numberobj_02_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_02_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_02_value = localvalue;
    }
    
    void toggle_02_value_number_setter(number v) {
        this->toggle_02_value_number = (v != 0 ? 1 : 0);
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
    
        if (this->numberobj_05_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_05_value = localvalue;
    }
    
    void numberobj_06_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_06_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_06_value = localvalue;
    }
    
    void numberobj_01_init() {
        this->numberobj_01_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Attack~/number_obj-74"), 1, this->_currentTime);
    }
    
    void numberobj_01_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_01_value;
    }
    
    void numberobj_01_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_01_value_set(preset["value"]);
    }
    
    number rampsmooth_tilde_01_d_next(number x) {
        number temp = (number)(x - this->rampsmooth_tilde_01_d_prev);
        this->rampsmooth_tilde_01_d_prev = x;
        return temp;
    }
    
    void rampsmooth_tilde_01_d_dspsetup() {
        this->rampsmooth_tilde_01_d_reset();
    }
    
    void rampsmooth_tilde_01_d_reset() {
        this->rampsmooth_tilde_01_d_prev = 0;
    }
    
    void rampsmooth_tilde_01_dspsetup(bool force) {
        if ((bool)(this->rampsmooth_tilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rampsmooth_tilde_01_setupDone = true;
        this->rampsmooth_tilde_01_d_dspsetup();
    }
    
    void edge_01_dspsetup(bool force) {
        if ((bool)(this->edge_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->edge_01_setupDone = true;
    }
    
    void thresh_tilde_01_init() {
        this->thresh_tilde_01_isAbove = false;
    }
    
    number peakamp_01_d_next(number x) {
        number temp = (number)(x - this->peakamp_01_d_prev);
        this->peakamp_01_d_prev = x;
        return temp;
    }
    
    void peakamp_01_d_dspsetup() {
        this->peakamp_01_d_reset();
    }
    
    void peakamp_01_d_reset() {
        this->peakamp_01_d_prev = 0;
    }
    
    void peakamp_01_dspsetup(bool force) {
        if ((bool)(this->peakamp_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->peakamp_01_index = 0;
        this->peakamp_01_lastMaximum = 0;
        this->peakamp_01_lastOutput = 0;
        this->peakamp_01_setupDone = true;
        this->peakamp_01_d_dspsetup();
    }
    
    number peakamp_02_d_next(number x) {
        number temp = (number)(x - this->peakamp_02_d_prev);
        this->peakamp_02_d_prev = x;
        return temp;
    }
    
    void peakamp_02_d_dspsetup() {
        this->peakamp_02_d_reset();
    }
    
    void peakamp_02_d_reset() {
        this->peakamp_02_d_prev = 0;
    }
    
    void peakamp_02_dspsetup(bool force) {
        if ((bool)(this->peakamp_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->peakamp_02_index = 0;
        this->peakamp_02_lastMaximum = 0;
        this->peakamp_02_lastOutput = 0;
        this->peakamp_02_setupDone = true;
        this->peakamp_02_d_dspsetup();
    }
    
    number average_absolute_tilde_01_av_next(number x, int windowSize, bool reset) {
        if (windowSize > 0)
            this->average_absolute_tilde_01_av_setwindowsize(windowSize);
    
        if (reset != 0) {
            if (this->average_absolute_tilde_01_av_resetFlag != 1) {
                this->average_absolute_tilde_01_av_wantsReset = 1;
                this->average_absolute_tilde_01_av_resetFlag = 1;
            }
        } else {
            this->average_absolute_tilde_01_av_resetFlag = 0;
        }
    
        if (this->average_absolute_tilde_01_av_wantsReset == 1) {
            this->average_absolute_tilde_01_av_doReset();
        }
    
        this->average_absolute_tilde_01_av_accum += x;
        this->average_absolute_tilde_01_av_buffer[(Index)this->average_absolute_tilde_01_av_bufferPos] = x;
        number bufferSize = this->average_absolute_tilde_01_av_buffer->getSize();
    
        if (this->average_absolute_tilde_01_av_effectiveWindowSize < this->average_absolute_tilde_01_av_currentWindowSize) {
            this->average_absolute_tilde_01_av_effectiveWindowSize++;
        } else {
            number bufferReadPos = this->average_absolute_tilde_01_av_bufferPos - this->average_absolute_tilde_01_av_effectiveWindowSize;
    
            while (bufferReadPos < 0)
                bufferReadPos += bufferSize;
    
            this->average_absolute_tilde_01_av_accum -= this->average_absolute_tilde_01_av_buffer[(Index)bufferReadPos];
        }
    
        this->average_absolute_tilde_01_av_bufferPos++;
    
        if (this->average_absolute_tilde_01_av_bufferPos >= bufferSize) {
            this->average_absolute_tilde_01_av_bufferPos -= bufferSize;
        }
    
        return this->average_absolute_tilde_01_av_accum / this->average_absolute_tilde_01_av_effectiveWindowSize;
    }
    
    void average_absolute_tilde_01_av_setwindowsize(int wsize) {
        wsize = trunc(wsize);
    
        if (wsize != this->average_absolute_tilde_01_av_currentWindowSize && wsize > 0 && wsize <= this->sr) {
            this->average_absolute_tilde_01_av_currentWindowSize = wsize;
            this->average_absolute_tilde_01_av_wantsReset = 1;
        }
    }
    
    void average_absolute_tilde_01_av_reset() {
        this->average_absolute_tilde_01_av_wantsReset = 1;
    }
    
    void average_absolute_tilde_01_av_dspsetup() {
        this->average_absolute_tilde_01_av_wantsReset = 1;
    
        if (this->sr > this->average_absolute_tilde_01_av_buffer->getSize()) {
            this->average_absolute_tilde_01_av_buffer->setSize(this->sr + 1);
            updateDataRef(this, this->average_absolute_tilde_01_av_buffer);
        }
    }
    
    void average_absolute_tilde_01_av_doReset() {
        this->average_absolute_tilde_01_av_accum = 0;
        this->average_absolute_tilde_01_av_effectiveWindowSize = 0;
        this->average_absolute_tilde_01_av_bufferPos = 0;
        this->average_absolute_tilde_01_av_wantsReset = 0;
    }
    
    void average_absolute_tilde_01_av_init() {
        this->average_absolute_tilde_01_av_currentWindowSize = this->sr;
        this->average_absolute_tilde_01_av_buffer->requestSize(this->sr + 1, 1);
        this->average_absolute_tilde_01_av_doReset();
    }
    
    void average_absolute_tilde_01_dspsetup(bool force) {
        if ((bool)(this->average_absolute_tilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->average_absolute_tilde_01_setupDone = true;
        this->average_absolute_tilde_01_av_dspsetup();
    }
    
    void toggle_01_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->toggle_01_value_number;
    }
    
    void toggle_01_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->toggle_01_value_number_set(preset["value"]);
    }
    
    void numberobj_02_init() {
        this->numberobj_02_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Attack~/number_obj-29"), 1, this->_currentTime);
    }
    
    void numberobj_02_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_02_value;
    }
    
    void numberobj_02_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_02_value_set(preset["value"]);
    }
    
    void toggle_02_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->toggle_02_value_number;
    }
    
    void toggle_02_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->toggle_02_value_number_set(preset["value"]);
    }
    
    void message_01_init() {
        this->message_01_set_set({1});
    }
    
    void numberobj_03_init() {
        this->numberobj_03_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Attack~/number_obj-61"), 1, this->_currentTime);
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
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Attack~/number_obj-58"), 1, this->_currentTime);
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
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Attack~/number_obj-38"), 1, this->_currentTime);
    }
    
    void numberobj_05_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_05_value;
    }
    
    void numberobj_05_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_05_value_set(preset["value"]);
    }
    
    void numberobj_06_init() {
        this->numberobj_06_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Attack~/number_obj-124"), 1, this->_currentTime);
    }
    
    void numberobj_06_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_06_value;
    }
    
    void numberobj_06_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_06_value_set(preset["value"]);
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
        dspexpr_01_in1 = 0;
        dspexpr_01_in2 = 0;
        numberobj_01_value = 0;
        numberobj_01_value_setter(numberobj_01_value);
        dspexpr_02_in1 = 0;
        dspexpr_02_in2 = 0;
        rampsmooth_tilde_01_x = 0;
        rampsmooth_tilde_01_up = 480;
        rampsmooth_tilde_01_down = 480;
        thresh_tilde_01_x = 0;
        thresh_tilde_01_lowThresh = 0;
        thresh_tilde_01_highThresh = 1;
        dspexpr_03_in1 = 0;
        dspexpr_03_in2 = 100;
        peakamp_01_interval = 10;
        peakamp_01_output = 0;
        dspexpr_04_in1 = 0;
        dspexpr_04_in2 = -1;
        dspexpr_05_in1 = 0;
        dspexpr_05_in2 = 50;
        peakamp_02_interval = 5;
        peakamp_02_output = 0;
        average_absolute_tilde_01_x = 0;
        average_absolute_tilde_01_windowSize = 22000;
        average_absolute_tilde_01_reset = 0;
        toggle_01_value_number = 0;
        toggle_01_value_number_setter(toggle_01_value_number);
        expr_01_in1 = 0;
        expr_01_in2 = 0;
        expr_01_out1 = 0;
        numberobj_02_value = 0;
        numberobj_02_value_setter(numberobj_02_value);
        scale_01_inlow = 0;
        scale_01_inhigh = 100;
        scale_01_outlow = 100;
        scale_01_outhigh = 0;
        scale_01_power = 0.5;
        toggle_02_value_number = 0;
        toggle_02_value_number_setter(toggle_02_value_number);
        select_01_test1 = 1;
        expr_02_in1 = 0;
        expr_02_in2 = 1000;
        expr_02_out1 = 0;
        numberobj_03_value = 0;
        numberobj_03_value_setter(numberobj_03_value);
        numberobj_04_value = 0;
        numberobj_04_value_setter(numberobj_04_value);
        scale_02_inlow = 0;
        scale_02_inhigh = 100;
        scale_02_outlow = 1000;
        scale_02_outhigh = 0;
        scale_02_power = 0.5;
        delay_01_time = 500;
        delay_01_delayall = 1;
        numberobj_05_value = 0;
        numberobj_05_value_setter(numberobj_05_value);
        scale_03_inlow = 0;
        scale_03_inhigh = 100;
        scale_03_outlow = 0;
        scale_03_outhigh = 200;
        scale_03_power = 1;
        numberobj_06_value = 0;
        numberobj_06_value_setter(numberobj_06_value);
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        numberobj_01_currentFormat = 6;
        numberobj_01_lastValue = 0;
        rampsmooth_tilde_01_prev = 0;
        rampsmooth_tilde_01_index = 0;
        rampsmooth_tilde_01_increment = 0;
        rampsmooth_tilde_01_d_prev = 0;
        rampsmooth_tilde_01_setupDone = false;
        timer_01_starttime = 0;
        timer_01_elapsed = 0;
        edge_01_setupDone = false;
        thresh_tilde_01_isAbove = false;
        peakamp_01_index = 0;
        peakamp_01_maxIndex = 0;
        peakamp_01_lastMaximum = 0;
        peakamp_01_lastOutput = 0;
        peakamp_01_d_prev = 0;
        peakamp_01_setupDone = false;
        peakamp_02_index = 0;
        peakamp_02_maxIndex = 0;
        peakamp_02_lastMaximum = 0;
        peakamp_02_lastOutput = 0;
        peakamp_02_d_prev = 0;
        peakamp_02_setupDone = false;
        average_absolute_tilde_01_av_currentWindowSize = 44100;
        average_absolute_tilde_01_av_accum = 0;
        average_absolute_tilde_01_av_effectiveWindowSize = 0;
        average_absolute_tilde_01_av_bufferPos = 0;
        average_absolute_tilde_01_av_wantsReset = false;
        average_absolute_tilde_01_av_resetFlag = false;
        average_absolute_tilde_01_setupDone = false;
        toggle_01_lastValue = 0;
        numberobj_02_currentFormat = 6;
        numberobj_02_lastValue = 0;
        toggle_02_lastValue = 0;
        numberobj_03_currentFormat = 6;
        numberobj_03_lastValue = 0;
        numberobj_04_currentFormat = 6;
        numberobj_04_lastValue = 0;
        numberobj_05_currentFormat = 6;
        numberobj_05_lastValue = 0;
        numberobj_06_currentFormat = 6;
        numberobj_06_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_01_in1;
        number dspexpr_01_in2;
        number numberobj_01_value;
        number dspexpr_02_in1;
        number dspexpr_02_in2;
        number rampsmooth_tilde_01_x;
        number rampsmooth_tilde_01_up;
        number rampsmooth_tilde_01_down;
        number thresh_tilde_01_x;
        number thresh_tilde_01_lowThresh;
        number thresh_tilde_01_highThresh;
        number dspexpr_03_in1;
        number dspexpr_03_in2;
        number peakamp_01_interval;
        number peakamp_01_output;
        number dspexpr_04_in1;
        number dspexpr_04_in2;
        number dspexpr_05_in1;
        number dspexpr_05_in2;
        number peakamp_02_interval;
        number peakamp_02_output;
        number average_absolute_tilde_01_x;
        number average_absolute_tilde_01_windowSize;
        number average_absolute_tilde_01_reset;
        number toggle_01_value_number;
        number expr_01_in1;
        number expr_01_in2;
        number expr_01_out1;
        number numberobj_02_value;
        list scale_01_input;
        number scale_01_inlow;
        number scale_01_inhigh;
        number scale_01_outlow;
        number scale_01_outhigh;
        number scale_01_power;
        list scale_01_out;
        number toggle_02_value_number;
        number select_01_test1;
        number expr_02_in1;
        number expr_02_in2;
        number expr_02_out1;
        list message_01_set;
        number numberobj_03_value;
        number numberobj_04_value;
        list scale_02_input;
        number scale_02_inlow;
        number scale_02_inhigh;
        number scale_02_outlow;
        number scale_02_outhigh;
        number scale_02_power;
        list scale_02_out;
        number delay_01_time;
        number delay_01_delayall;
        number numberobj_05_value;
        list scale_03_input;
        number scale_03_inlow;
        number scale_03_inhigh;
        number scale_03_outlow;
        number scale_03_outhigh;
        number scale_03_power;
        list scale_03_out;
        number numberobj_06_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[4];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Int numberobj_01_currentFormat;
        number numberobj_01_lastValue;
        number rampsmooth_tilde_01_prev;
        number rampsmooth_tilde_01_index;
        number rampsmooth_tilde_01_increment;
        number rampsmooth_tilde_01_d_prev;
        bool rampsmooth_tilde_01_setupDone;
        number timer_01_starttime;
        number timer_01_elapsed;
        number edge_01_currentState;
        bool edge_01_setupDone;
        bool thresh_tilde_01_isAbove;
        int peakamp_01_index;
        int peakamp_01_maxIndex;
        number peakamp_01_lastMaximum;
        number peakamp_01_lastOutput;
        number peakamp_01_d_prev;
        bool peakamp_01_setupDone;
        int peakamp_02_index;
        int peakamp_02_maxIndex;
        number peakamp_02_lastMaximum;
        number peakamp_02_lastOutput;
        number peakamp_02_d_prev;
        bool peakamp_02_setupDone;
        int average_absolute_tilde_01_av_currentWindowSize;
        number average_absolute_tilde_01_av_accum;
        int average_absolute_tilde_01_av_effectiveWindowSize;
        int average_absolute_tilde_01_av_bufferPos;
        bool average_absolute_tilde_01_av_wantsReset;
        bool average_absolute_tilde_01_av_resetFlag;
        Float64BufferRef average_absolute_tilde_01_av_buffer;
        bool average_absolute_tilde_01_setupDone;
        number toggle_01_lastValue;
        Int numberobj_02_currentFormat;
        number numberobj_02_lastValue;
        number toggle_02_lastValue;
        Int numberobj_03_currentFormat;
        number numberobj_03_lastValue;
        Int numberobj_04_currentFormat;
        number numberobj_04_lastValue;
        Int numberobj_05_currentFormat;
        number numberobj_05_lastValue;
        Int numberobj_06_currentFormat;
        number numberobj_06_lastValue;
        number stackprotect_count;
        DataRef average_absolute_tilde_01_av_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_48 : public PatcherInterfaceImpl {
    
    friend class freezer;
    
    public:
    
    RNBOSubpatcher_48()
    {
    }
    
    ~RNBOSubpatcher_48()
    {
    }
    
    virtual freezer* getPatcher() const {
        return static_cast<freezer *>(_parentPatcher);
    }
    
    freezer* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -871642103, false);
        getEngine()->flushClockEvents(this, -281953904, false);
    }
    
    SampleIndex currentsampletime() {
        return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
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
        RNBO_UNUSED(numInputs);
        RNBO_UNUSED(inputs);
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        this->linetilde_01_perform(this->signals[0], n);
        this->dspexpr_06_perform(this->signals[0], this->dspexpr_06_in2, out1, n);
        this->stackprotect_perform(n);
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
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -871642103:
            {
            this->loadbang_01_startupbang_bang();
            break;
            }
        case -281953904:
            {
            this->linetilde_01_target_bang();
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
            if (TAG("Transp~/number_obj-52") == objectId)
                this->numberobj_07_valin_set(payload);
    
            if (TAG("Transp~/number_obj-4") == objectId)
                this->numberobj_08_valin_set(payload);
    
            if (TAG("Transp~/number_obj-29") == objectId)
                this->numberobj_09_valin_set(payload);
    
            break;
            }
        case TAG("format"):
            {
            if (TAG("Transp~/number_obj-52") == objectId)
                this->numberobj_07_format_set(payload);
    
            if (TAG("Transp~/number_obj-4") == objectId)
                this->numberobj_08_format_set(payload);
    
            if (TAG("Transp~/number_obj-29") == objectId)
                this->numberobj_09_format_set(payload);
    
            break;
            }
        case TAG("listin"):
            {
            if (TAG("Transp~/message_obj-26") == objectId)
                this->message_02_listin_number_set(payload);
    
            break;
            }
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("listin"):
            {
            if (TAG("Transp~/message_obj-26") == objectId)
                this->message_02_listin_list_set(payload);
    
            break;
            }
        }
    }
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("startupbang"):
            {
            if (TAG("Transp~/loadbang_obj-33") == objectId)
                this->loadbang_01_startupbang_bang();
    
            break;
            }
        case TAG("listin"):
            {
            if (TAG("Transp~/message_obj-26") == objectId)
                this->message_02_listin_bang_bang();
    
            break;
            }
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("valout"):
            {
            return "valout";
            }
        case TAG("Transp~/number_obj-52"):
            {
            return "Transp~/number_obj-52";
            }
        case TAG("setup"):
            {
            return "setup";
            }
        case TAG("Transp~/number_obj-4"):
            {
            return "Transp~/number_obj-4";
            }
        case TAG("Transp~/number_obj-29"):
            {
            return "Transp~/number_obj-29";
            }
        case TAG("listout"):
            {
            return "listout";
            }
        case TAG("Transp~/message_obj-26"):
            {
            return "Transp~/message_obj-26";
            }
        case TAG("valin"):
            {
            return "valin";
            }
        case TAG("format"):
            {
            return "format";
            }
        case TAG("startupbang"):
            {
            return "startupbang";
            }
        case TAG("Transp~/loadbang_obj-33"):
            {
            return "Transp~/loadbang_obj-33";
            }
        case TAG("listin"):
            {
            return "listin";
            }
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
    
    void eventinlet_03_out1_bang_bang() {
        this->numberobj_07_value_bang();
    }
    
    void eventinlet_03_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_07_value_set(converted);
        }
    }
    
    void loadbang_01_startupbang_bang() {
        this->loadbang_01_output_bang();
    }
    
    void numberobj_09_valin_set(number v) {
        this->numberobj_09_value_set(v);
    }
    
    void numberobj_09_format_set(number v) {
        this->numberobj_09_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void message_02_listin_list_set(const list& v) {
        this->message_02_set_set(v);
    }
    
    void message_02_listin_number_set(number v) {
        this->message_02_set_set(v);
    }
    
    void message_02_listin_bang_bang() {
        this->message_02_trigger_bang();
    }
    
    void linetilde_01_target_bang() {}
    
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
        return 0;
    }
    
    Index getNumOutputChannels() const {
        return 1;
    }
    
    void initializeObjects() {
        this->numberobj_07_init();
        this->numberobj_08_init();
        this->numberobj_09_init();
        this->message_02_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
        this->getEngine()->scheduleClockEvent(this, -871642103, 0 + this->_currentTime);;
    }
    
    void allocateDataRefs() {}
    
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
    
    void numberobj_08_output_set(number v) {
        {
            list converted = {v};
            this->linetilde_01_segments_set(converted);
        }
    }
    
    void numberobj_08_value_set(number v) {
        this->numberobj_08_value_setter(v);
        v = this->numberobj_08_value;
        number localvalue = v;
    
        if (this->numberobj_08_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Transp~/number_obj-4"), localvalue, this->_currentTime);
        this->numberobj_08_output_set(localvalue);
    }
    
    void expr_03_out1_set(number v) {
        this->expr_03_out1 = v;
        this->numberobj_08_value_set(this->expr_03_out1);
    }
    
    void expr_03_in1_set(number in1) {
        this->expr_03_in1 = in1;
        this->expr_03_out1_set(fixnan(rnbo_pow(2, this->expr_03_in1 / (number)12)));//#map:Transp~/expr_obj-49:1
    }
    
    void numberobj_07_output_set(number v) {
        this->expr_03_in1_set(v);
    }
    
    void numberobj_07_value_set(number v) {
        this->numberobj_07_value_setter(v);
        v = this->numberobj_07_value;
        number localvalue = v;
    
        if (this->numberobj_07_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Transp~/number_obj-52"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_07_output_set(localvalue);
    }
    
    void eventinlet_03_out1_number_set(number v) {
        this->numberobj_07_value_set(v);
    }
    
    void numberobj_07_value_bang() {
        number v = this->numberobj_07_value;
        number localvalue = v;
    
        if (this->numberobj_07_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Transp~/number_obj-52"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_07_output_set(localvalue);
    }
    
    void dspexpr_06_in2_set(number v) {
        this->dspexpr_06_in2 = v;
    }
    
    void numberobj_09_output_set(number v) {
        this->dspexpr_06_in2_set(v);
    }
    
    void numberobj_09_value_set(number v) {
        this->numberobj_09_value_setter(v);
        v = this->numberobj_09_value;
        number localvalue = v;
    
        if (this->numberobj_09_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("Transp~/number_obj-29"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_09_output_set(localvalue);
    }
    
    void expr_04_out1_set(number v) {
        this->expr_04_out1 = v;
        this->numberobj_09_value_set(this->expr_04_out1);
    }
    
    void expr_04_in1_set(number in1) {
        this->expr_04_in1 = in1;
        this->expr_04_out1_set((this->expr_04_in1 == 0. ? 0. : this->expr_04_in2 / this->expr_04_in1));//#map:Transp~/!/_obj-27:1
    }
    
    void expr_04_in2_set(number v) {
        this->expr_04_in2 = v;
    }
    
    void message_02_out_set(const list& v) {
        {
            if (v->length > 1)
                this->expr_04_in2_set(v[1]);
    
            number converted = (v->length > 0 ? v[0] : 0);
            this->expr_04_in1_set(converted);
        }
    }
    
    void message_02_trigger_bang() {
        this->message_02_out_set(this->message_02_set);
    }
    
    void loadbang_01_output_bang() {
        this->message_02_trigger_bang();
    }
    
    void message_02_set_set(const list& v) {
        this->message_02_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Transp~/message_obj-26"), v, this->_currentTime);
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
                            -281953904,
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
    
    void dspexpr_06_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
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
    
    void numberobj_07_init() {
        this->numberobj_07_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Transp~/number_obj-52"), 1, this->_currentTime);
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
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Transp~/number_obj-4"), 1, this->_currentTime);
    }
    
    void numberobj_08_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_08_value;
    }
    
    void numberobj_08_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_08_value_set(preset["value"]);
    }
    
    void numberobj_09_init() {
        this->numberobj_09_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Transp~/number_obj-29"), 1, this->_currentTime);
    }
    
    void numberobj_09_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_09_value;
    }
    
    void numberobj_09_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_09_value_set(preset["value"]);
    }
    
    void message_02_init() {
        this->message_02_set_set({200});
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
        numberobj_07_value = 0;
        numberobj_07_value_setter(numberobj_07_value);
        expr_03_in1 = 0;
        expr_03_out1 = 0;
        dspexpr_06_in1 = 0;
        dspexpr_06_in2 = 5;
        linetilde_01_time = 10;
        linetilde_01_keepramp = 0;
        numberobj_08_value = 0;
        numberobj_08_value_setter(numberobj_08_value);
        numberobj_09_value = 0;
        numberobj_09_value_setter(numberobj_09_value);
        expr_04_in1 = 0;
        expr_04_in2 = 1000;
        expr_04_out1 = 0;
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
        numberobj_07_currentFormat = 6;
        numberobj_07_lastValue = 0;
        linetilde_01_currentValue = 1;
        numberobj_08_currentFormat = 6;
        numberobj_08_lastValue = 0;
        numberobj_09_currentFormat = 6;
        numberobj_09_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number numberobj_07_value;
        number expr_03_in1;
        number expr_03_out1;
        number dspexpr_06_in1;
        number dspexpr_06_in2;
        list linetilde_01_segments;
        number linetilde_01_time;
        number linetilde_01_keepramp;
        number numberobj_08_value;
        number numberobj_09_value;
        number expr_04_in1;
        number expr_04_in2;
        number expr_04_out1;
        list message_02_set;
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
        Int numberobj_07_currentFormat;
        number numberobj_07_lastValue;
        list linetilde_01_activeRamps;
        number linetilde_01_currentValue;
        Int numberobj_08_currentFormat;
        number numberobj_08_lastValue;
        Int numberobj_09_currentFormat;
        number numberobj_09_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_49 : public PatcherInterfaceImpl {
    
    friend class freezer;
    
    public:
    
    RNBOSubpatcher_49()
    {
    }
    
    ~RNBOSubpatcher_49()
    {
    }
    
    virtual freezer* getPatcher() const {
        return static_cast<freezer *>(_parentPatcher);
    }
    
    freezer* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -871642103, false);
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
    
    template <typename T> inline array<SampleValue, 2 + 1> sample_channels_2(T& buffer, SampleValue sampleIndex, Index channelOffset) {
        number bufferSize = buffer->getSize();
        const Index bufferChannels = (const Index)(buffer->getChannels());
        constexpr int ___N3 = 2 + 1;
        array<SampleValue, ___N3> out = FIXEDSIZEARRAYINIT(2 + 1);
    
        {
            SampleValue bufferphasetoindex_result;
    
            {
                auto __end = bufferSize;
                auto __start = 0;
                auto __phase = sampleIndex;
    
                {
                    number size = __end - 1 - __start;
                    bufferphasetoindex_result = __phase * size;
                }
            }
    
            sampleIndex = bufferphasetoindex_result;
        }
    
        if (bufferSize == 0 || (5 == 5 && (sampleIndex < 0 || sampleIndex >= bufferSize))) {
            for (Index i = 0; i < 2 + 1; i++) {
                out[(Index)i] = 0;
            }
        } else {
            for (Index channel = 0; channel < 2; channel++) {
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
                        number frac = sampleIndex - index1;
                        number wrap = bufferSize - 1;
                        SampleIndex index2 = (SampleIndex)(index1 + 1);
    
                        if (index2 > wrap)
                            index2 = 0;
    
                        bufferreadsample_result = this->linearinterp(
                            frac,
                            __buffer->getSample(channelIndex, index1),
                            __buffer->getSample(channelIndex, index2)
                        );
                    }
                }
    
                out[(Index)channel] = bufferreadsample_result;
            }
    
            out[2] = sampleIndex;
        }
    
        return out;
    }
    
    int notenumber() {
        return this->_noteNumber;
    }
    
    Index voice() {
        return this->_voiceIndex;
    }
    
    number samplerate() {
        return this->sr;
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
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    
        if (this->getIsMuted())
            return;
    
        this->dspexpr_10_perform(in1, this->dspexpr_10_in2, this->signals[0], n);
        this->rand_tilde_01_perform(this->rand_tilde_01_frequency, this->signals[1], n);
    
        this->cycle_tilde_01_perform(
            this->signals[1],
            this->cycle_tilde_01_phase_offset,
            this->signals[2],
            this->dummyBuffer,
            n
        );
    
        this->dspexpr_13_perform(this->signals[2], this->dspexpr_13_in2, this->signals[1], n);
    
        this->rampsmooth_tilde_04_perform(
            this->signals[1],
            this->rampsmooth_tilde_04_up,
            this->rampsmooth_tilde_04_down,
            this->signals[2],
            n
        );
    
        this->dspexpr_09_perform(this->signals[0], this->signals[2], this->signals[1], n);
    
        this->wrap_tilde_01_perform(
            this->signals[1],
            this->wrap_tilde_01_low,
            this->wrap_tilde_01_high,
            this->signals[2],
            n
        );
    
        this->sample_01_perform(
            this->signals[2],
            this->sample_01_offset,
            this->signals[1],
            this->signals[0],
            this->dummyBuffer,
            n
        );
    
        this->hann_tilde_01_perform(this->signals[2], this->signals[3], n);
        this->dspexpr_08_perform(this->signals[1], this->signals[3], this->signals[2], n);
        this->dspexpr_12_perform(this->signals[0], this->signals[3], this->signals[1], n);
        this->rand_tilde_02_perform(this->rand_tilde_02_frequency, this->signals[3], n);
    
        this->cycle_tilde_02_perform(
            this->signals[3],
            this->cycle_tilde_02_phase_offset,
            this->signals[0],
            this->dummyBuffer,
            n
        );
    
        this->dspexpr_16_perform(this->signals[0], this->dspexpr_16_in2, this->signals[3], n);
        this->dspexpr_15_perform(this->signals[3], this->dspexpr_15_in2, this->signals[0], n);
        this->dspexpr_14_perform(this->signals[0], this->dspexpr_14_in2, this->signals[3], n);
    
        this->rampsmooth_tilde_02_perform(
            this->signals[3],
            this->rampsmooth_tilde_02_up,
            this->rampsmooth_tilde_02_down,
            this->signals[0],
            n
        );
    
        this->dspexpr_07_perform(this->signals[2], this->signals[0], this->signals[3], n);
        this->dspexpr_11_perform(this->signals[1], this->signals[0], this->signals[2], n);
        this->rand_tilde_03_perform(this->rand_tilde_03_frequency, this->signals[0], n);
    
        this->cycle_tilde_03_perform(
            this->signals[0],
            this->cycle_tilde_03_phase_offset,
            this->signals[1],
            this->dummyBuffer,
            n
        );
    
        this->dspexpr_19_perform(this->signals[1], this->dspexpr_19_in2, this->signals[0], n);
        this->dspexpr_18_perform(this->signals[0], this->dspexpr_18_in2, this->signals[1], n);
        this->dspexpr_17_perform(this->signals[1], this->dspexpr_17_in2, this->signals[0], n);
    
        this->rampsmooth_tilde_03_perform(
            this->signals[0],
            this->rampsmooth_tilde_03_up,
            this->rampsmooth_tilde_03_down,
            this->signals[1],
            n
        );
    
        this->pan_tilde_01_perform(
            this->signals[1],
            this->signals[3],
            this->signals[2],
            this->signals[0],
            this->signals[4],
            n
        );
    
        this->signaladder_03_perform(this->signals[0], out1, out1, n);
        this->signaladder_04_perform(this->signals[4], out2, out2, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 5; i++) {
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
    
        this->rand_tilde_01_dspsetup(forceDSPSetup);
        this->cycle_tilde_01_dspsetup(forceDSPSetup);
        this->rampsmooth_tilde_04_dspsetup(forceDSPSetup);
        this->rand_tilde_02_dspsetup(forceDSPSetup);
        this->cycle_tilde_02_dspsetup(forceDSPSetup);
        this->rampsmooth_tilde_02_dspsetup(forceDSPSetup);
        this->rand_tilde_03_dspsetup(forceDSPSetup);
        this->cycle_tilde_03_dspsetup(forceDSPSetup);
        this->rampsmooth_tilde_03_dspsetup(forceDSPSetup);
    
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
    
    void setVoiceIndex(Index index)  {
        this->_voiceIndex = index;
    }
    
    void setNoteNumber(Int noteNumber)  {
        this->_noteNumber = noteNumber;
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
    
    ParameterValue getPolyParameterValue(PatcherInterface** voices, ParameterIndex index)  {
        switch (index) {
        default:
            {
            return voices[0]->getParameterValue(index);
            }
        }
    }
    
    void setPolyParameterValue(
        PatcherInterface** voices,
        ParameterIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        switch (index) {
        default:
            {
            for (Index i = 0; i < 8; i++)
                voices[i]->setParameterValue(index, value, time);
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
        if (this->_voiceIndex == 1)
            this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void sendPolyParameter(ParameterIndex index, Index voiceIndex, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset + voiceIndex - 1, ignoreValue);
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
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -871642103:
            {
            this->loadbang_02_startupbang_bang();
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("startupbang"):
            {
            if (TAG("Freezer-Voice~/loadbang_obj-8") == objectId)
                this->loadbang_02_startupbang_bang();
    
            break;
            }
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("startupbang"):
            {
            return "startupbang";
            }
        case TAG("Freezer-Voice~/loadbang_obj-8"):
            {
            return "Freezer-Voice~/loadbang_obj-8";
            }
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
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 0) {
            this->sample_01_buffer = new Float64Buffer(this->getPatcher()->FreezeA);
        }
    
        if (index == 1) {
            this->cycle_tilde_01_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
            this->cycle_tilde_01_bufferUpdated();
            this->cycle_tilde_02_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
            this->cycle_tilde_02_bufferUpdated();
            this->cycle_tilde_03_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
            this->cycle_tilde_03_bufferUpdated();
        }
    }
    
    void initialize() {
        this->assign_defaults();
        this->setState();
        this->sample_01_buffer = new Float64Buffer(this->getPatcher()->FreezeA);
        this->cycle_tilde_01_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
        this->cycle_tilde_02_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
        this->cycle_tilde_03_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
    }
    
    protected:
    
    void loadbang_02_startupbang_bang() {
        this->loadbang_02_output_bang();
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
        return 1;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->rand_tilde_01_n_init();
        this->rand_tilde_01_init();
        this->rand_tilde_02_n_init();
        this->rand_tilde_02_init();
        this->rand_tilde_03_n_init();
        this->rand_tilde_03_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
        this->getEngine()->scheduleClockEvent(this, -871642103, 0 + this->_currentTime);;
    }
    
    void allocateDataRefs() {
        this->cycle_tilde_01_buffer->requestSize(16384, 1);
        this->cycle_tilde_01_buffer->setSampleRate(this->sr);
        this->cycle_tilde_02_buffer->requestSize(16384, 1);
        this->cycle_tilde_02_buffer->setSampleRate(this->sr);
        this->cycle_tilde_03_buffer->requestSize(16384, 1);
        this->cycle_tilde_03_buffer->setSampleRate(this->sr);
        this->sample_01_buffer = this->sample_01_buffer->allocateIfNeeded();
        this->cycle_tilde_01_buffer = this->cycle_tilde_01_buffer->allocateIfNeeded();
        this->cycle_tilde_02_buffer = this->cycle_tilde_02_buffer->allocateIfNeeded();
        this->cycle_tilde_03_buffer = this->cycle_tilde_03_buffer->allocateIfNeeded();
    }
    
    void dspexpr_13_in2_set(number v) {
        this->dspexpr_13_in2 = v;
    }
    
    void receive_04_output_number_set(number v) {
        this->receive_04_output_number = v;
        this->dspexpr_13_in2_set(v);
    }
    
    void dspexpr_19_in2_set(number v) {
        this->dspexpr_19_in2 = v;
    }
    
    void receive_06_output_number_set(number v) {
        this->receive_06_output_number = v;
        this->dspexpr_19_in2_set(v);
    }
    
    void dspexpr_16_in2_set(number v) {
        this->dspexpr_16_in2 = v;
    }
    
    void receive_05_output_number_set(number v) {
        this->receive_05_output_number = v;
        this->dspexpr_16_in2_set(v);
    }
    
    void voice_01_noteNumber_set(number ) {}
    
    void dspexpr_10_in2_set(number v) {
        this->dspexpr_10_in2 = v;
    }
    
    void expr_05_out1_set(number v) {
        this->expr_05_out1 = v;
        this->dspexpr_10_in2_set(this->expr_05_out1);
    }
    
    void expr_05_in1_set(number in1) {
        this->expr_05_in1 = in1;
    
        this->expr_05_out1_set(
            (this->expr_05_in2 == 0 ? 0 : (this->expr_05_in2 == 0. ? 0. : this->expr_05_in1 / this->expr_05_in2))
        );//#map:Freezer-Voice~//_obj-7:1
    }
    
    void expr_06_out1_set(number v) {
        this->expr_06_out1 = v;
        this->expr_05_in1_set(this->expr_06_out1);
    }
    
    void expr_06_in1_set(number in1) {
        this->expr_06_in1 = in1;
        this->expr_06_out1_set(this->expr_06_in1 - this->expr_06_in2);//#map:Freezer-Voice~/-_obj-6:1
    }
    
    void voice_01_voicenumber_set(number v) {
        this->expr_06_in1_set(v);
    }
    
    void voice_01_voicebang_bang() {
        this->voice_01_noteNumber_set(this->notenumber());
        this->voice_01_voicenumber_set(this->voice());
    }
    
    void loadbang_02_output_bang() {
        this->voice_01_voicebang_bang();
    }
    
    void midiouthelper_midiout_set(number ) {}
    
    void dspexpr_10_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void rand_tilde_01_perform(number frequency, SampleValue * out1, Index n) {
        auto __rand_tilde_01_lastValue = this->rand_tilde_01_lastValue;
        auto __rand_tilde_01_oldTargetValue = this->rand_tilde_01_oldTargetValue;
        auto __rand_tilde_01_targetValue = this->rand_tilde_01_targetValue;
        number f = (frequency > 0.0001 ? frequency : 0.0001);
        Index i;
    
        for (i = 0; i < n; i++) {
            number phasorValue = this->rand_tilde_01_p_next(f, -1);
    
            if (this->rand_tilde_01_d_next(phasorValue) < 0) {
                __rand_tilde_01_targetValue = this->rand_tilde_01_n_next();
                __rand_tilde_01_oldTargetValue = __rand_tilde_01_lastValue;
            }
    
            __rand_tilde_01_lastValue = __rand_tilde_01_oldTargetValue + phasorValue * (__rand_tilde_01_targetValue - __rand_tilde_01_oldTargetValue);
            out1[(Index)i] = __rand_tilde_01_lastValue;
        }
    
        this->rand_tilde_01_targetValue = __rand_tilde_01_targetValue;
        this->rand_tilde_01_oldTargetValue = __rand_tilde_01_oldTargetValue;
        this->rand_tilde_01_lastValue = __rand_tilde_01_lastValue;
    }
    
    void cycle_tilde_01_perform(
        const Sample * frequency,
        number phase_offset,
        SampleValue * out1,
        SampleValue * out2,
        Index n
    ) {
        RNBO_UNUSED(phase_offset);
        auto __cycle_tilde_01_buffer = this->cycle_tilde_01_buffer;
        Index i;
    
        for (i = 0; i < n; i++) {
            {
                uint32_t uint_phase;
    
                {
                    {
                        uint_phase = uint32_trunc(frequency[(Index)i] * 4294967296.0);
                    }
                }
    
                uint32_t idx = (uint32_t)(uint32_rshift(uint_phase, 18));
                number frac = ((BinOpInt)((BinOpInt)uint_phase & (BinOpInt)262143)) * 3.81471181759574e-6;
                number y0 = __cycle_tilde_01_buffer[(Index)idx];
                number y1 = __cycle_tilde_01_buffer[(Index)((BinOpInt)(idx + 1) & (BinOpInt)16383)];
                number y = y0 + frac * (y1 - y0);
                out1[(Index)i] = y;
                out2[(Index)i] = uint_phase * 0.232830643653869629e-9;
                continue;
            }
        }
    }
    
    void dspexpr_13_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void rampsmooth_tilde_04_perform(const Sample * x, number up, number down, SampleValue * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __rampsmooth_tilde_04_increment = this->rampsmooth_tilde_04_increment;
        auto __rampsmooth_tilde_04_index = this->rampsmooth_tilde_04_index;
        auto __rampsmooth_tilde_04_prev = this->rampsmooth_tilde_04_prev;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (this->rampsmooth_tilde_04_d_next(x[(Index)i]) != 0.) {
                if (x[(Index)i] > __rampsmooth_tilde_04_prev) {
                    number _up = 480;
    
                    if (_up < 1)
                        _up = 1;
    
                    __rampsmooth_tilde_04_index = _up;
                    __rampsmooth_tilde_04_increment = (x[(Index)i] - __rampsmooth_tilde_04_prev) / _up;
                } else if (x[(Index)i] < __rampsmooth_tilde_04_prev) {
                    number _down = 480;
    
                    if (_down < 1)
                        _down = 1;
    
                    __rampsmooth_tilde_04_index = _down;
                    __rampsmooth_tilde_04_increment = (x[(Index)i] - __rampsmooth_tilde_04_prev) / _down;
                }
            }
    
            if (__rampsmooth_tilde_04_index > 0) {
                __rampsmooth_tilde_04_prev += __rampsmooth_tilde_04_increment;
                __rampsmooth_tilde_04_index -= 1;
            } else {
                __rampsmooth_tilde_04_prev = x[(Index)i];
            }
    
            out1[(Index)i] = __rampsmooth_tilde_04_prev;
        }
    
        this->rampsmooth_tilde_04_prev = __rampsmooth_tilde_04_prev;
        this->rampsmooth_tilde_04_index = __rampsmooth_tilde_04_index;
        this->rampsmooth_tilde_04_increment = __rampsmooth_tilde_04_increment;
    }
    
    void dspexpr_09_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void wrap_tilde_01_perform(const Sample * x, number low, number high, SampleValue * out1, Index n) {
        RNBO_UNUSED(high);
        RNBO_UNUSED(low);
        Index i;
    
        for (i = 0; i < n; i++) {
            number lo;
            number hi;
            lo = 0;
            hi = 1;
            number range = hi - lo;
    
            if (x[(Index)i] >= lo && x[(Index)i] < hi) {
                out1[(Index)i] = x[(Index)i];
                continue;
            }
    
            if (range <= 0.000000001) {
                out1[(Index)i] = lo;
                continue;
            }
    
            long numWraps = (long)(trunc((x[(Index)i] - lo) / range));
            numWraps = numWraps - ((x[(Index)i] < lo ? 1 : 0));
            number result = x[(Index)i] - range * numWraps;
    
            if (result >= hi) {
                out1[(Index)i] = result - range;
                continue;
            } else {
                out1[(Index)i] = result;
                continue;
            }
        }
    }
    
    void sample_01_perform(
        const Sample * value,
        number offset,
        SampleValue * out1,
        SampleValue * out2,
        SampleValue * outindex,
        Index n
    ) {
        RNBO_UNUSED(out2);
        RNBO_UNUSED(out1);
        RNBO_UNUSED(offset);
        auto __sample_01_buffer = this->sample_01_buffer;
        SampleArray<2> out = {out1, out2};
    
        for (Index i = 0; i < n; i++) {
            auto result = this->sample_channels_2(__sample_01_buffer, value[(Index)i], 0);
    
            for (Index j = 0; j < 2; j++) {
                out[(Index)j][(Index)i] = result[(Index)j];
            }
    
            outindex[(Index)i] = result[2];
        }
    }
    
    void hann_tilde_01_perform(const Sample * x, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            number __x = x[(Index)i];
            __x = (__x > 1 ? 1 : (__x < 0 ? 0 : __x));
            out1[(Index)i] = 0.5 * (1 - rnbo_cos(6.28318530717958647692 * __x));
        }
    }
    
    void dspexpr_08_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_12_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void rand_tilde_02_perform(number frequency, SampleValue * out1, Index n) {
        auto __rand_tilde_02_lastValue = this->rand_tilde_02_lastValue;
        auto __rand_tilde_02_oldTargetValue = this->rand_tilde_02_oldTargetValue;
        auto __rand_tilde_02_targetValue = this->rand_tilde_02_targetValue;
        number f = (frequency > 0.0001 ? frequency : 0.0001);
        Index i;
    
        for (i = 0; i < n; i++) {
            number phasorValue = this->rand_tilde_02_p_next(f, -1);
    
            if (this->rand_tilde_02_d_next(phasorValue) < 0) {
                __rand_tilde_02_targetValue = this->rand_tilde_02_n_next();
                __rand_tilde_02_oldTargetValue = __rand_tilde_02_lastValue;
            }
    
            __rand_tilde_02_lastValue = __rand_tilde_02_oldTargetValue + phasorValue * (__rand_tilde_02_targetValue - __rand_tilde_02_oldTargetValue);
            out1[(Index)i] = __rand_tilde_02_lastValue;
        }
    
        this->rand_tilde_02_targetValue = __rand_tilde_02_targetValue;
        this->rand_tilde_02_oldTargetValue = __rand_tilde_02_oldTargetValue;
        this->rand_tilde_02_lastValue = __rand_tilde_02_lastValue;
    }
    
    void cycle_tilde_02_perform(
        const Sample * frequency,
        number phase_offset,
        SampleValue * out1,
        SampleValue * out2,
        Index n
    ) {
        RNBO_UNUSED(phase_offset);
        auto __cycle_tilde_02_buffer = this->cycle_tilde_02_buffer;
        Index i;
    
        for (i = 0; i < n; i++) {
            {
                uint32_t uint_phase;
    
                {
                    {
                        uint_phase = uint32_trunc(frequency[(Index)i] * 4294967296.0);
                    }
                }
    
                uint32_t idx = (uint32_t)(uint32_rshift(uint_phase, 18));
                number frac = ((BinOpInt)((BinOpInt)uint_phase & (BinOpInt)262143)) * 3.81471181759574e-6;
                number y0 = __cycle_tilde_02_buffer[(Index)idx];
                number y1 = __cycle_tilde_02_buffer[(Index)((BinOpInt)(idx + 1) & (BinOpInt)16383)];
                number y = y0 + frac * (y1 - y0);
                out1[(Index)i] = y;
                out2[(Index)i] = uint_phase * 0.232830643653869629e-9;
                continue;
            }
        }
    }
    
    void dspexpr_16_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_15_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + 1;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_14_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void rampsmooth_tilde_02_perform(const Sample * x, number up, number down, SampleValue * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __rampsmooth_tilde_02_increment = this->rampsmooth_tilde_02_increment;
        auto __rampsmooth_tilde_02_index = this->rampsmooth_tilde_02_index;
        auto __rampsmooth_tilde_02_prev = this->rampsmooth_tilde_02_prev;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (this->rampsmooth_tilde_02_d_next(x[(Index)i]) != 0.) {
                if (x[(Index)i] > __rampsmooth_tilde_02_prev) {
                    number _up = 480;
    
                    if (_up < 1)
                        _up = 1;
    
                    __rampsmooth_tilde_02_index = _up;
                    __rampsmooth_tilde_02_increment = (x[(Index)i] - __rampsmooth_tilde_02_prev) / _up;
                } else if (x[(Index)i] < __rampsmooth_tilde_02_prev) {
                    number _down = 480;
    
                    if (_down < 1)
                        _down = 1;
    
                    __rampsmooth_tilde_02_index = _down;
                    __rampsmooth_tilde_02_increment = (x[(Index)i] - __rampsmooth_tilde_02_prev) / _down;
                }
            }
    
            if (__rampsmooth_tilde_02_index > 0) {
                __rampsmooth_tilde_02_prev += __rampsmooth_tilde_02_increment;
                __rampsmooth_tilde_02_index -= 1;
            } else {
                __rampsmooth_tilde_02_prev = x[(Index)i];
            }
    
            out1[(Index)i] = __rampsmooth_tilde_02_prev;
        }
    
        this->rampsmooth_tilde_02_prev = __rampsmooth_tilde_02_prev;
        this->rampsmooth_tilde_02_index = __rampsmooth_tilde_02_index;
        this->rampsmooth_tilde_02_increment = __rampsmooth_tilde_02_increment;
    }
    
    void dspexpr_07_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_11_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void rand_tilde_03_perform(number frequency, SampleValue * out1, Index n) {
        auto __rand_tilde_03_lastValue = this->rand_tilde_03_lastValue;
        auto __rand_tilde_03_oldTargetValue = this->rand_tilde_03_oldTargetValue;
        auto __rand_tilde_03_targetValue = this->rand_tilde_03_targetValue;
        number f = (frequency > 0.0001 ? frequency : 0.0001);
        Index i;
    
        for (i = 0; i < n; i++) {
            number phasorValue = this->rand_tilde_03_p_next(f, -1);
    
            if (this->rand_tilde_03_d_next(phasorValue) < 0) {
                __rand_tilde_03_targetValue = this->rand_tilde_03_n_next();
                __rand_tilde_03_oldTargetValue = __rand_tilde_03_lastValue;
            }
    
            __rand_tilde_03_lastValue = __rand_tilde_03_oldTargetValue + phasorValue * (__rand_tilde_03_targetValue - __rand_tilde_03_oldTargetValue);
            out1[(Index)i] = __rand_tilde_03_lastValue;
        }
    
        this->rand_tilde_03_targetValue = __rand_tilde_03_targetValue;
        this->rand_tilde_03_oldTargetValue = __rand_tilde_03_oldTargetValue;
        this->rand_tilde_03_lastValue = __rand_tilde_03_lastValue;
    }
    
    void cycle_tilde_03_perform(
        const Sample * frequency,
        number phase_offset,
        SampleValue * out1,
        SampleValue * out2,
        Index n
    ) {
        RNBO_UNUSED(phase_offset);
        auto __cycle_tilde_03_buffer = this->cycle_tilde_03_buffer;
        Index i;
    
        for (i = 0; i < n; i++) {
            {
                uint32_t uint_phase;
    
                {
                    {
                        uint_phase = uint32_trunc(frequency[(Index)i] * 4294967296.0);
                    }
                }
    
                uint32_t idx = (uint32_t)(uint32_rshift(uint_phase, 18));
                number frac = ((BinOpInt)((BinOpInt)uint_phase & (BinOpInt)262143)) * 3.81471181759574e-6;
                number y0 = __cycle_tilde_03_buffer[(Index)idx];
                number y1 = __cycle_tilde_03_buffer[(Index)((BinOpInt)(idx + 1) & (BinOpInt)16383)];
                number y = y0 + frac * (y1 - y0);
                out1[(Index)i] = y;
                out2[(Index)i] = uint_phase * 0.232830643653869629e-9;
                continue;
            }
        }
    }
    
    void dspexpr_19_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_18_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + 1;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_17_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void rampsmooth_tilde_03_perform(const Sample * x, number up, number down, SampleValue * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __rampsmooth_tilde_03_increment = this->rampsmooth_tilde_03_increment;
        auto __rampsmooth_tilde_03_index = this->rampsmooth_tilde_03_index;
        auto __rampsmooth_tilde_03_prev = this->rampsmooth_tilde_03_prev;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (this->rampsmooth_tilde_03_d_next(x[(Index)i]) != 0.) {
                if (x[(Index)i] > __rampsmooth_tilde_03_prev) {
                    number _up = 480;
    
                    if (_up < 1)
                        _up = 1;
    
                    __rampsmooth_tilde_03_index = _up;
                    __rampsmooth_tilde_03_increment = (x[(Index)i] - __rampsmooth_tilde_03_prev) / _up;
                } else if (x[(Index)i] < __rampsmooth_tilde_03_prev) {
                    number _down = 480;
    
                    if (_down < 1)
                        _down = 1;
    
                    __rampsmooth_tilde_03_index = _down;
                    __rampsmooth_tilde_03_increment = (x[(Index)i] - __rampsmooth_tilde_03_prev) / _down;
                }
            }
    
            if (__rampsmooth_tilde_03_index > 0) {
                __rampsmooth_tilde_03_prev += __rampsmooth_tilde_03_increment;
                __rampsmooth_tilde_03_index -= 1;
            } else {
                __rampsmooth_tilde_03_prev = x[(Index)i];
            }
    
            out1[(Index)i] = __rampsmooth_tilde_03_prev;
        }
    
        this->rampsmooth_tilde_03_prev = __rampsmooth_tilde_03_prev;
        this->rampsmooth_tilde_03_index = __rampsmooth_tilde_03_index;
        this->rampsmooth_tilde_03_increment = __rampsmooth_tilde_03_increment;
    }
    
    void pan_tilde_01_perform(
        const Sample * pos,
        const SampleValue * in1,
        const SampleValue * in2,
        SampleValue * out1,
        SampleValue * out2,
        Index n
    ) {
        auto __pan_tilde_01_spread = this->pan_tilde_01_spread;
        Index i;
    
        for (i = 0; i < n; i++) {
            out2[(Index)i] = in1[(Index)i] * this->pan_tilde_01_func_next(pos[(Index)i] + -0.5 * __pan_tilde_01_spread, 1) + in2[(Index)i] * this->pan_tilde_01_func_next(pos[(Index)i] + 0.5 * __pan_tilde_01_spread, 1);
            out1[(Index)i] = in1[(Index)i] * this->pan_tilde_01_func_next(pos[(Index)i] + -0.5 * __pan_tilde_01_spread, 0) + in2[(Index)i] * this->pan_tilde_01_func_next(pos[(Index)i] + 0.5 * __pan_tilde_01_spread, 0);
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
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number pan_tilde_01_func_next(number pos, int channel) {
        {
            {
                number nchan_1 = 2 - 1;
    
                {
                    pos = pos * nchan_1;
                }
    
                {
                    {
                        if (pos >= 0) {
                            pos = this->safemod(pos, 2);
                        } else {
                            pos = this->safemod(2 + this->safemod(pos, 2), 2);
                        }
    
                        if (channel == 0 && pos >= nchan_1)
                            pos = pos - 2;
                    }
                }
    
                pos = pos - channel;
    
                if (pos > -1 && pos < 1) {
                    {
                        {
                            return this->safesqrt(1.0 - rnbo_abs(pos));
                        }
                    }
                } else {
                    return 0;
                }
            }
        }
    }
    
    void pan_tilde_01_func_reset() {}
    
    number rampsmooth_tilde_02_d_next(number x) {
        number temp = (number)(x - this->rampsmooth_tilde_02_d_prev);
        this->rampsmooth_tilde_02_d_prev = x;
        return temp;
    }
    
    void rampsmooth_tilde_02_d_dspsetup() {
        this->rampsmooth_tilde_02_d_reset();
    }
    
    void rampsmooth_tilde_02_d_reset() {
        this->rampsmooth_tilde_02_d_prev = 0;
    }
    
    void rampsmooth_tilde_02_dspsetup(bool force) {
        if ((bool)(this->rampsmooth_tilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rampsmooth_tilde_02_setupDone = true;
        this->rampsmooth_tilde_02_d_dspsetup();
    }
    
    number rampsmooth_tilde_03_d_next(number x) {
        number temp = (number)(x - this->rampsmooth_tilde_03_d_prev);
        this->rampsmooth_tilde_03_d_prev = x;
        return temp;
    }
    
    void rampsmooth_tilde_03_d_dspsetup() {
        this->rampsmooth_tilde_03_d_reset();
    }
    
    void rampsmooth_tilde_03_d_reset() {
        this->rampsmooth_tilde_03_d_prev = 0;
    }
    
    void rampsmooth_tilde_03_dspsetup(bool force) {
        if ((bool)(this->rampsmooth_tilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rampsmooth_tilde_03_setupDone = true;
        this->rampsmooth_tilde_03_d_dspsetup();
    }
    
    number rampsmooth_tilde_04_d_next(number x) {
        number temp = (number)(x - this->rampsmooth_tilde_04_d_prev);
        this->rampsmooth_tilde_04_d_prev = x;
        return temp;
    }
    
    void rampsmooth_tilde_04_d_dspsetup() {
        this->rampsmooth_tilde_04_d_reset();
    }
    
    void rampsmooth_tilde_04_d_reset() {
        this->rampsmooth_tilde_04_d_prev = 0;
    }
    
    void rampsmooth_tilde_04_dspsetup(bool force) {
        if ((bool)(this->rampsmooth_tilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rampsmooth_tilde_04_setupDone = true;
        this->rampsmooth_tilde_04_d_dspsetup();
    }
    
    number cycle_tilde_01_ph_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->cycle_tilde_01_ph_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->cycle_tilde_01_ph_conv;
    
        if (this->cycle_tilde_01_ph_currentPhase < 0.)
            this->cycle_tilde_01_ph_currentPhase = 1. + this->cycle_tilde_01_ph_currentPhase;
    
        if (this->cycle_tilde_01_ph_currentPhase > 1.)
            this->cycle_tilde_01_ph_currentPhase = this->cycle_tilde_01_ph_currentPhase - 1.;
    
        number tmp = this->cycle_tilde_01_ph_currentPhase;
        this->cycle_tilde_01_ph_currentPhase += pincr;
        return tmp;
    }
    
    void cycle_tilde_01_ph_reset() {
        this->cycle_tilde_01_ph_currentPhase = 0;
    }
    
    void cycle_tilde_01_ph_dspsetup() {
        this->cycle_tilde_01_ph_conv = (number)1 / this->sr;
    }
    
    void cycle_tilde_01_dspsetup(bool force) {
        if ((bool)(this->cycle_tilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->cycle_tilde_01_phasei = 0;
        this->cycle_tilde_01_f2i = (number)4294967296 / this->samplerate();
        this->cycle_tilde_01_wrap = (long)(this->cycle_tilde_01_buffer->getSize()) - 1;
        this->cycle_tilde_01_setupDone = true;
        this->cycle_tilde_01_ph_dspsetup();
    }
    
    void cycle_tilde_01_bufferUpdated() {
        this->cycle_tilde_01_wrap = (long)(this->cycle_tilde_01_buffer->getSize()) - 1;
    }
    
    number rand_tilde_01_p_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->rand_tilde_01_p_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->rand_tilde_01_p_conv;
    
        if (this->rand_tilde_01_p_currentPhase < 0.)
            this->rand_tilde_01_p_currentPhase = 1. + this->rand_tilde_01_p_currentPhase;
    
        if (this->rand_tilde_01_p_currentPhase > 1.)
            this->rand_tilde_01_p_currentPhase = this->rand_tilde_01_p_currentPhase - 1.;
    
        number tmp = this->rand_tilde_01_p_currentPhase;
        this->rand_tilde_01_p_currentPhase += pincr;
        return tmp;
    }
    
    void rand_tilde_01_p_reset() {
        this->rand_tilde_01_p_currentPhase = 0;
    }
    
    void rand_tilde_01_p_dspsetup() {
        this->rand_tilde_01_p_conv = (number)1 / this->sr;
    }
    
    void rand_tilde_01_n_reset() {
        xoshiro_reset(
            systemticks() + this->voice() + this->random(0, 10000),
            this->rand_tilde_01_n_state
        );
    }
    
    void rand_tilde_01_n_init() {
        this->rand_tilde_01_n_reset();
    }
    
    void rand_tilde_01_n_seed(number v) {
        xoshiro_reset(v, this->rand_tilde_01_n_state);
    }
    
    number rand_tilde_01_n_next() {
        return xoshiro_next(this->rand_tilde_01_n_state);
    }
    
    number rand_tilde_01_d_next(number x) {
        number temp = (number)(x - this->rand_tilde_01_d_prev);
        this->rand_tilde_01_d_prev = x;
        return temp;
    }
    
    void rand_tilde_01_d_dspsetup() {
        this->rand_tilde_01_d_reset();
    }
    
    void rand_tilde_01_d_reset() {
        this->rand_tilde_01_d_prev = 0;
    }
    
    void rand_tilde_01_init() {
        this->rand_tilde_01_targetValue = this->rand_tilde_01_n_next();
    }
    
    void rand_tilde_01_dspsetup(bool force) {
        if ((bool)(this->rand_tilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rand_tilde_01_setupDone = true;
        this->rand_tilde_01_p_dspsetup();
        this->rand_tilde_01_d_dspsetup();
    }
    
    number cycle_tilde_02_ph_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->cycle_tilde_02_ph_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->cycle_tilde_02_ph_conv;
    
        if (this->cycle_tilde_02_ph_currentPhase < 0.)
            this->cycle_tilde_02_ph_currentPhase = 1. + this->cycle_tilde_02_ph_currentPhase;
    
        if (this->cycle_tilde_02_ph_currentPhase > 1.)
            this->cycle_tilde_02_ph_currentPhase = this->cycle_tilde_02_ph_currentPhase - 1.;
    
        number tmp = this->cycle_tilde_02_ph_currentPhase;
        this->cycle_tilde_02_ph_currentPhase += pincr;
        return tmp;
    }
    
    void cycle_tilde_02_ph_reset() {
        this->cycle_tilde_02_ph_currentPhase = 0;
    }
    
    void cycle_tilde_02_ph_dspsetup() {
        this->cycle_tilde_02_ph_conv = (number)1 / this->sr;
    }
    
    void cycle_tilde_02_dspsetup(bool force) {
        if ((bool)(this->cycle_tilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->cycle_tilde_02_phasei = 0;
        this->cycle_tilde_02_f2i = (number)4294967296 / this->samplerate();
        this->cycle_tilde_02_wrap = (long)(this->cycle_tilde_02_buffer->getSize()) - 1;
        this->cycle_tilde_02_setupDone = true;
        this->cycle_tilde_02_ph_dspsetup();
    }
    
    void cycle_tilde_02_bufferUpdated() {
        this->cycle_tilde_02_wrap = (long)(this->cycle_tilde_02_buffer->getSize()) - 1;
    }
    
    number rand_tilde_02_p_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->rand_tilde_02_p_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->rand_tilde_02_p_conv;
    
        if (this->rand_tilde_02_p_currentPhase < 0.)
            this->rand_tilde_02_p_currentPhase = 1. + this->rand_tilde_02_p_currentPhase;
    
        if (this->rand_tilde_02_p_currentPhase > 1.)
            this->rand_tilde_02_p_currentPhase = this->rand_tilde_02_p_currentPhase - 1.;
    
        number tmp = this->rand_tilde_02_p_currentPhase;
        this->rand_tilde_02_p_currentPhase += pincr;
        return tmp;
    }
    
    void rand_tilde_02_p_reset() {
        this->rand_tilde_02_p_currentPhase = 0;
    }
    
    void rand_tilde_02_p_dspsetup() {
        this->rand_tilde_02_p_conv = (number)1 / this->sr;
    }
    
    void rand_tilde_02_n_reset() {
        xoshiro_reset(
            systemticks() + this->voice() + this->random(0, 10000),
            this->rand_tilde_02_n_state
        );
    }
    
    void rand_tilde_02_n_init() {
        this->rand_tilde_02_n_reset();
    }
    
    void rand_tilde_02_n_seed(number v) {
        xoshiro_reset(v, this->rand_tilde_02_n_state);
    }
    
    number rand_tilde_02_n_next() {
        return xoshiro_next(this->rand_tilde_02_n_state);
    }
    
    number rand_tilde_02_d_next(number x) {
        number temp = (number)(x - this->rand_tilde_02_d_prev);
        this->rand_tilde_02_d_prev = x;
        return temp;
    }
    
    void rand_tilde_02_d_dspsetup() {
        this->rand_tilde_02_d_reset();
    }
    
    void rand_tilde_02_d_reset() {
        this->rand_tilde_02_d_prev = 0;
    }
    
    void rand_tilde_02_init() {
        this->rand_tilde_02_targetValue = this->rand_tilde_02_n_next();
    }
    
    void rand_tilde_02_dspsetup(bool force) {
        if ((bool)(this->rand_tilde_02_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rand_tilde_02_setupDone = true;
        this->rand_tilde_02_p_dspsetup();
        this->rand_tilde_02_d_dspsetup();
    }
    
    number cycle_tilde_03_ph_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->cycle_tilde_03_ph_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->cycle_tilde_03_ph_conv;
    
        if (this->cycle_tilde_03_ph_currentPhase < 0.)
            this->cycle_tilde_03_ph_currentPhase = 1. + this->cycle_tilde_03_ph_currentPhase;
    
        if (this->cycle_tilde_03_ph_currentPhase > 1.)
            this->cycle_tilde_03_ph_currentPhase = this->cycle_tilde_03_ph_currentPhase - 1.;
    
        number tmp = this->cycle_tilde_03_ph_currentPhase;
        this->cycle_tilde_03_ph_currentPhase += pincr;
        return tmp;
    }
    
    void cycle_tilde_03_ph_reset() {
        this->cycle_tilde_03_ph_currentPhase = 0;
    }
    
    void cycle_tilde_03_ph_dspsetup() {
        this->cycle_tilde_03_ph_conv = (number)1 / this->sr;
    }
    
    void cycle_tilde_03_dspsetup(bool force) {
        if ((bool)(this->cycle_tilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->cycle_tilde_03_phasei = 0;
        this->cycle_tilde_03_f2i = (number)4294967296 / this->samplerate();
        this->cycle_tilde_03_wrap = (long)(this->cycle_tilde_03_buffer->getSize()) - 1;
        this->cycle_tilde_03_setupDone = true;
        this->cycle_tilde_03_ph_dspsetup();
    }
    
    void cycle_tilde_03_bufferUpdated() {
        this->cycle_tilde_03_wrap = (long)(this->cycle_tilde_03_buffer->getSize()) - 1;
    }
    
    number rand_tilde_03_p_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->rand_tilde_03_p_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->rand_tilde_03_p_conv;
    
        if (this->rand_tilde_03_p_currentPhase < 0.)
            this->rand_tilde_03_p_currentPhase = 1. + this->rand_tilde_03_p_currentPhase;
    
        if (this->rand_tilde_03_p_currentPhase > 1.)
            this->rand_tilde_03_p_currentPhase = this->rand_tilde_03_p_currentPhase - 1.;
    
        number tmp = this->rand_tilde_03_p_currentPhase;
        this->rand_tilde_03_p_currentPhase += pincr;
        return tmp;
    }
    
    void rand_tilde_03_p_reset() {
        this->rand_tilde_03_p_currentPhase = 0;
    }
    
    void rand_tilde_03_p_dspsetup() {
        this->rand_tilde_03_p_conv = (number)1 / this->sr;
    }
    
    void rand_tilde_03_n_reset() {
        xoshiro_reset(
            systemticks() + this->voice() + this->random(0, 10000),
            this->rand_tilde_03_n_state
        );
    }
    
    void rand_tilde_03_n_init() {
        this->rand_tilde_03_n_reset();
    }
    
    void rand_tilde_03_n_seed(number v) {
        xoshiro_reset(v, this->rand_tilde_03_n_state);
    }
    
    number rand_tilde_03_n_next() {
        return xoshiro_next(this->rand_tilde_03_n_state);
    }
    
    number rand_tilde_03_d_next(number x) {
        number temp = (number)(x - this->rand_tilde_03_d_prev);
        this->rand_tilde_03_d_prev = x;
        return temp;
    }
    
    void rand_tilde_03_d_dspsetup() {
        this->rand_tilde_03_d_reset();
    }
    
    void rand_tilde_03_d_reset() {
        this->rand_tilde_03_d_prev = 0;
    }
    
    void rand_tilde_03_init() {
        this->rand_tilde_03_targetValue = this->rand_tilde_03_n_next();
    }
    
    void rand_tilde_03_dspsetup(bool force) {
        if ((bool)(this->rand_tilde_03_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rand_tilde_03_setupDone = true;
        this->rand_tilde_03_p_dspsetup();
        this->rand_tilde_03_d_dspsetup();
    }
    
    void midiouthelper_sendMidi(number v) {
        this->midiouthelper_midiout_set(v);
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
        dspexpr_07_in1 = 0;
        dspexpr_07_in2 = 0;
        pan_tilde_01_spread = 0.5;
        pan_tilde_01_pos = 0;
        dspexpr_08_in1 = 0;
        dspexpr_08_in2 = 0;
        sample_01_value = 0;
        sample_01_offset = 0;
        wrap_tilde_01_x = 0;
        wrap_tilde_01_low = 0;
        wrap_tilde_01_high = 1;
        dspexpr_09_in1 = 0;
        dspexpr_09_in2 = 0;
        dspexpr_10_in1 = 0;
        dspexpr_10_in2 = 0;
        dspexpr_11_in1 = 0;
        dspexpr_11_in2 = 0;
        dspexpr_12_in1 = 0;
        dspexpr_12_in2 = 0;
        expr_05_in1 = 0;
        expr_05_in2 = 8;
        expr_05_out1 = 0;
        expr_06_in1 = 0;
        expr_06_in2 = 1;
        expr_06_out1 = 0;
        voice_01_mute_number = 0;
        hann_tilde_01_x = 0;
        rampsmooth_tilde_02_x = 0;
        rampsmooth_tilde_02_up = 480;
        rampsmooth_tilde_02_down = 480;
        rampsmooth_tilde_03_x = 0;
        rampsmooth_tilde_03_up = 480;
        rampsmooth_tilde_03_down = 480;
        rampsmooth_tilde_04_x = 0;
        rampsmooth_tilde_04_up = 480;
        rampsmooth_tilde_04_down = 480;
        cycle_tilde_01_frequency = 0;
        cycle_tilde_01_phase_offset = 0;
        dspexpr_13_in1 = 0;
        dspexpr_13_in2 = 0.005;
        rand_tilde_01_frequency = 1;
        receive_01_output_number = 0;
        dspexpr_14_in1 = 0;
        dspexpr_14_in2 = 0.5;
        dspexpr_15_in1 = 0;
        dspexpr_15_in2 = 1;
        cycle_tilde_02_frequency = 0;
        cycle_tilde_02_phase_offset = 0;
        dspexpr_16_in1 = 0;
        dspexpr_16_in2 = 0.1;
        rand_tilde_02_frequency = 1;
        receive_02_output_number = 0;
        dspexpr_17_in1 = 0;
        dspexpr_17_in2 = 0.5;
        dspexpr_18_in1 = 0;
        dspexpr_18_in2 = 1;
        cycle_tilde_03_frequency = 0;
        cycle_tilde_03_phase_offset = 0;
        dspexpr_19_in1 = 0;
        dspexpr_19_in2 = 0.1;
        rand_tilde_03_frequency = 1;
        receive_03_output_number = 0;
        receive_04_output_number = 0;
        receive_05_output_number = 0;
        receive_06_output_number = 0;
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
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        rampsmooth_tilde_02_prev = 0;
        rampsmooth_tilde_02_index = 0;
        rampsmooth_tilde_02_increment = 0;
        rampsmooth_tilde_02_d_prev = 0;
        rampsmooth_tilde_02_setupDone = false;
        rampsmooth_tilde_03_prev = 0;
        rampsmooth_tilde_03_index = 0;
        rampsmooth_tilde_03_increment = 0;
        rampsmooth_tilde_03_d_prev = 0;
        rampsmooth_tilde_03_setupDone = false;
        rampsmooth_tilde_04_prev = 0;
        rampsmooth_tilde_04_index = 0;
        rampsmooth_tilde_04_increment = 0;
        rampsmooth_tilde_04_d_prev = 0;
        rampsmooth_tilde_04_setupDone = false;
        cycle_tilde_01_wrap = 0;
        cycle_tilde_01_ph_currentPhase = 0;
        cycle_tilde_01_ph_conv = 0;
        cycle_tilde_01_setupDone = false;
        rand_tilde_01_lastValue = 0;
        rand_tilde_01_targetValue = 0;
        rand_tilde_01_oldTargetValue = 0;
        rand_tilde_01_p_currentPhase = 0;
        rand_tilde_01_p_conv = 0;
        rand_tilde_01_d_prev = 0;
        rand_tilde_01_setupDone = false;
        cycle_tilde_02_wrap = 0;
        cycle_tilde_02_ph_currentPhase = 0;
        cycle_tilde_02_ph_conv = 0;
        cycle_tilde_02_setupDone = false;
        rand_tilde_02_lastValue = 0;
        rand_tilde_02_targetValue = 0;
        rand_tilde_02_oldTargetValue = 0;
        rand_tilde_02_p_currentPhase = 0;
        rand_tilde_02_p_conv = 0;
        rand_tilde_02_d_prev = 0;
        rand_tilde_02_setupDone = false;
        cycle_tilde_03_wrap = 0;
        cycle_tilde_03_ph_currentPhase = 0;
        cycle_tilde_03_ph_conv = 0;
        cycle_tilde_03_setupDone = false;
        rand_tilde_03_lastValue = 0;
        rand_tilde_03_targetValue = 0;
        rand_tilde_03_oldTargetValue = 0;
        rand_tilde_03_p_currentPhase = 0;
        rand_tilde_03_p_conv = 0;
        rand_tilde_03_d_prev = 0;
        rand_tilde_03_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 0;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_07_in1;
        number dspexpr_07_in2;
        number pan_tilde_01_spread;
        number pan_tilde_01_pos;
        number dspexpr_08_in1;
        number dspexpr_08_in2;
        number sample_01_value;
        number sample_01_offset;
        number wrap_tilde_01_x;
        number wrap_tilde_01_low;
        number wrap_tilde_01_high;
        number dspexpr_09_in1;
        number dspexpr_09_in2;
        number dspexpr_10_in1;
        number dspexpr_10_in2;
        number dspexpr_11_in1;
        number dspexpr_11_in2;
        number dspexpr_12_in1;
        number dspexpr_12_in2;
        number expr_05_in1;
        number expr_05_in2;
        number expr_05_out1;
        number expr_06_in1;
        number expr_06_in2;
        number expr_06_out1;
        number voice_01_mute_number;
        number hann_tilde_01_x;
        number rampsmooth_tilde_02_x;
        number rampsmooth_tilde_02_up;
        number rampsmooth_tilde_02_down;
        number rampsmooth_tilde_03_x;
        number rampsmooth_tilde_03_up;
        number rampsmooth_tilde_03_down;
        number rampsmooth_tilde_04_x;
        number rampsmooth_tilde_04_up;
        number rampsmooth_tilde_04_down;
        number cycle_tilde_01_frequency;
        number cycle_tilde_01_phase_offset;
        number dspexpr_13_in1;
        number dspexpr_13_in2;
        number rand_tilde_01_frequency;
        number receive_01_output_number;
        list receive_01_output_list;
        number dspexpr_14_in1;
        number dspexpr_14_in2;
        number dspexpr_15_in1;
        number dspexpr_15_in2;
        number cycle_tilde_02_frequency;
        number cycle_tilde_02_phase_offset;
        number dspexpr_16_in1;
        number dspexpr_16_in2;
        number rand_tilde_02_frequency;
        number receive_02_output_number;
        list receive_02_output_list;
        number dspexpr_17_in1;
        number dspexpr_17_in2;
        number dspexpr_18_in1;
        number dspexpr_18_in2;
        number cycle_tilde_03_frequency;
        number cycle_tilde_03_phase_offset;
        number dspexpr_19_in1;
        number dspexpr_19_in2;
        number rand_tilde_03_frequency;
        number receive_03_output_number;
        list receive_03_output_list;
        number receive_04_output_number;
        list receive_04_output_list;
        number receive_05_output_number;
        list receive_05_output_list;
        number receive_06_output_number;
        list receive_06_output_list;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[5];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float64BufferRef sample_01_buffer;
        number rampsmooth_tilde_02_prev;
        number rampsmooth_tilde_02_index;
        number rampsmooth_tilde_02_increment;
        number rampsmooth_tilde_02_d_prev;
        bool rampsmooth_tilde_02_setupDone;
        number rampsmooth_tilde_03_prev;
        number rampsmooth_tilde_03_index;
        number rampsmooth_tilde_03_increment;
        number rampsmooth_tilde_03_d_prev;
        bool rampsmooth_tilde_03_setupDone;
        number rampsmooth_tilde_04_prev;
        number rampsmooth_tilde_04_index;
        number rampsmooth_tilde_04_increment;
        number rampsmooth_tilde_04_d_prev;
        bool rampsmooth_tilde_04_setupDone;
        Float64BufferRef cycle_tilde_01_buffer;
        long cycle_tilde_01_wrap;
        uint32_t cycle_tilde_01_phasei;
        SampleValue cycle_tilde_01_f2i;
        number cycle_tilde_01_ph_currentPhase;
        number cycle_tilde_01_ph_conv;
        bool cycle_tilde_01_setupDone;
        number rand_tilde_01_lastValue;
        number rand_tilde_01_targetValue;
        number rand_tilde_01_oldTargetValue;
        number rand_tilde_01_p_currentPhase;
        number rand_tilde_01_p_conv;
        UInt rand_tilde_01_n_state[4] = { };
        number rand_tilde_01_d_prev;
        bool rand_tilde_01_setupDone;
        Float64BufferRef cycle_tilde_02_buffer;
        long cycle_tilde_02_wrap;
        uint32_t cycle_tilde_02_phasei;
        SampleValue cycle_tilde_02_f2i;
        number cycle_tilde_02_ph_currentPhase;
        number cycle_tilde_02_ph_conv;
        bool cycle_tilde_02_setupDone;
        number rand_tilde_02_lastValue;
        number rand_tilde_02_targetValue;
        number rand_tilde_02_oldTargetValue;
        number rand_tilde_02_p_currentPhase;
        number rand_tilde_02_p_conv;
        UInt rand_tilde_02_n_state[4] = { };
        number rand_tilde_02_d_prev;
        bool rand_tilde_02_setupDone;
        Float64BufferRef cycle_tilde_03_buffer;
        long cycle_tilde_03_wrap;
        uint32_t cycle_tilde_03_phasei;
        SampleValue cycle_tilde_03_f2i;
        number cycle_tilde_03_ph_currentPhase;
        number cycle_tilde_03_ph_conv;
        bool cycle_tilde_03_setupDone;
        number rand_tilde_03_lastValue;
        number rand_tilde_03_targetValue;
        number rand_tilde_03_oldTargetValue;
        number rand_tilde_03_p_currentPhase;
        number rand_tilde_03_p_conv;
        UInt rand_tilde_03_n_state[4] = { };
        number rand_tilde_03_d_prev;
        bool rand_tilde_03_setupDone;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_50 : public PatcherInterfaceImpl {
    
    friend class freezer;
    
    public:
    
    RNBOSubpatcher_50()
    {
    }
    
    ~RNBOSubpatcher_50()
    {
    }
    
    virtual freezer* getPatcher() const {
        return static_cast<freezer *>(_parentPatcher);
    }
    
    freezer* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -1841075377, false);
        getEngine()->flushClockEvents(this, -281953904, false);
        getEngine()->flushClockEvents(this, 894770446, false);
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
    
    SampleIndex currentsampletime() {
        return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
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
        this->linetilde_02_perform(this->signals[0], n);
        this->onepole_tilde_02_perform(in1, this->signals[0], this->signals[1], n);
        this->dspexpr_22_perform(in1, this->signals[1], this->signals[2], n);
        this->onepole_tilde_04_perform(in2, this->signals[0], this->signals[1], n);
        this->dspexpr_25_perform(in2, this->signals[1], this->signals[0], n);
        this->linetilde_03_perform(this->signals[1], n);
        this->linetilde_04_perform(this->signals[3], n);
        this->onepole_tilde_03_perform(this->signals[0], this->signals[3], this->signals[4], n);
        this->dspexpr_24_perform(this->signals[4], this->signals[1], this->signals[0], n);
        this->dspexpr_23_perform(this->signals[0], out2, n);
        this->onepole_tilde_01_perform(this->signals[2], this->signals[3], this->signals[0], n);
        this->dspexpr_21_perform(this->signals[0], this->signals[1], this->signals[3], n);
        this->dspexpr_20_perform(this->signals[3], out1, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 5; i++) {
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
    
        this->onepole_tilde_02_dspsetup(forceDSPSetup);
        this->onepole_tilde_04_dspsetup(forceDSPSetup);
        this->onepole_tilde_03_dspsetup(forceDSPSetup);
        this->onepole_tilde_01_dspsetup(forceDSPSetup);
    
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
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -1841075377:
            {
            this->linetilde_02_target_bang();
            break;
            }
        case -281953904:
            {
            this->linetilde_03_target_bang();
            break;
            }
        case 894770446:
            {
            this->linetilde_04_target_bang();
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
            if (TAG("EQ&Volume~/number_obj-21") == objectId)
                this->numberobj_10_valin_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-12") == objectId)
                this->numberobj_11_valin_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-45") == objectId)
                this->numberobj_12_valin_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-37") == objectId)
                this->numberobj_13_valin_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-40") == objectId)
                this->numberobj_14_valin_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-27") == objectId)
                this->numberobj_15_valin_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-32") == objectId)
                this->numberobj_16_valin_set(payload);
    
            break;
            }
        case TAG("format"):
            {
            if (TAG("EQ&Volume~/number_obj-21") == objectId)
                this->numberobj_10_format_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-12") == objectId)
                this->numberobj_11_format_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-45") == objectId)
                this->numberobj_12_format_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-37") == objectId)
                this->numberobj_13_format_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-40") == objectId)
                this->numberobj_14_format_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-27") == objectId)
                this->numberobj_15_format_set(payload);
    
            if (TAG("EQ&Volume~/number_obj-32") == objectId)
                this->numberobj_16_format_set(payload);
    
            break;
            }
        }
    }
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("valout"):
            {
            return "valout";
            }
        case TAG("EQ&Volume~/number_obj-21"):
            {
            return "EQ&Volume~/number_obj-21";
            }
        case TAG("setup"):
            {
            return "setup";
            }
        case TAG("EQ&Volume~/number_obj-12"):
            {
            return "EQ&Volume~/number_obj-12";
            }
        case TAG("EQ&Volume~/number_obj-45"):
            {
            return "EQ&Volume~/number_obj-45";
            }
        case TAG("EQ&Volume~/number_obj-37"):
            {
            return "EQ&Volume~/number_obj-37";
            }
        case TAG("EQ&Volume~/number_obj-40"):
            {
            return "EQ&Volume~/number_obj-40";
            }
        case TAG("EQ&Volume~/number_obj-27"):
            {
            return "EQ&Volume~/number_obj-27";
            }
        case TAG("EQ&Volume~/number_obj-32"):
            {
            return "EQ&Volume~/number_obj-32";
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
    
    void numberobj_10_valin_set(number v) {
        this->numberobj_10_value_set(v);
    }
    
    void numberobj_10_format_set(number v) {
        this->numberobj_10_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void eventinlet_04_out1_bang_bang() {
        this->numberobj_11_value_bang();
    }
    
    void eventinlet_04_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_11_value_set(converted);
        }
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
    
    void numberobj_13_valin_set(number v) {
        this->numberobj_13_value_set(v);
    }
    
    void numberobj_13_format_set(number v) {
        this->numberobj_13_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void eventinlet_05_out1_bang_bang() {
        this->numberobj_14_value_bang();
    }
    
    void eventinlet_05_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_14_value_set(converted);
        }
    }
    
    void numberobj_14_valin_set(number v) {
        this->numberobj_14_value_set(v);
    }
    
    void numberobj_14_format_set(number v) {
        this->numberobj_14_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void numberobj_15_valin_set(number v) {
        this->numberobj_15_value_set(v);
    }
    
    void numberobj_15_format_set(number v) {
        this->numberobj_15_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void eventinlet_06_out1_bang_bang() {
        this->numberobj_16_value_bang();
    }
    
    void eventinlet_06_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_16_value_set(converted);
        }
    }
    
    void numberobj_16_valin_set(number v) {
        this->numberobj_16_value_set(v);
    }
    
    void numberobj_16_format_set(number v) {
        this->numberobj_16_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void linetilde_02_target_bang() {}
    
    void linetilde_03_target_bang() {}
    
    void linetilde_04_target_bang() {}
    
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
        this->numberobj_10_init();
        this->numberobj_11_init();
        this->numberobj_12_init();
        this->numberobj_13_init();
        this->numberobj_14_init();
        this->numberobj_15_init();
        this->numberobj_16_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
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
    
    void numberobj_10_output_set(number v) {
        {
            list converted = {v};
            this->linetilde_02_segments_set(converted);
        }
    }
    
    void numberobj_10_value_set(number v) {
        this->numberobj_10_value_setter(v);
        v = this->numberobj_10_value;
        number localvalue = v;
    
        if (this->numberobj_10_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("EQ&Volume~/number_obj-21"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_10_output_set(localvalue);
    }
    
    void scale_04_out_set(const list& v) {
        this->scale_04_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_10_value_set(converted);
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
    
    void numberobj_11_output_set(number v) {
        {
            list converted = {v};
            this->scale_04_input_set(converted);
        }
    }
    
    void numberobj_11_value_set(number v) {
        this->numberobj_11_value_setter(v);
        v = this->numberobj_11_value;
        number localvalue = v;
    
        if (this->numberobj_11_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("EQ&Volume~/number_obj-12"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_11_output_set(localvalue);
    }
    
    void eventinlet_04_out1_number_set(number v) {
        this->numberobj_11_value_set(v);
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
    
    void numberobj_15_output_set(number v) {
        {
            list converted = {v};
            this->linetilde_04_segments_set(converted);
        }
    }
    
    void numberobj_15_value_set(number v) {
        this->numberobj_15_value_setter(v);
        v = this->numberobj_15_value;
        number localvalue = v;
    
        if (this->numberobj_15_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("EQ&Volume~/number_obj-27"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_15_output_set(localvalue);
    }
    
    void scale_06_out_set(const list& v) {
        this->scale_06_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_15_value_set(converted);
        }
    }
    
    void scale_06_input_set(const list& v) {
        this->scale_06_input = jsCreateListCopy(v);
        list tmp = {};
    
        for (Index i = 0; i < v->length; i++) {
            tmp->push(this->scale(
                v[(Index)i],
                this->scale_06_inlow,
                this->scale_06_inhigh,
                this->scale_06_outlow,
                this->scale_06_outhigh,
                this->scale_06_power
            ));
        }
    
        this->scale_06_out_set(tmp);
    }
    
    void numberobj_16_output_set(number v) {
        {
            list converted = {v};
            this->scale_06_input_set(converted);
        }
    }
    
    void numberobj_16_value_set(number v) {
        this->numberobj_16_value_setter(v);
        v = this->numberobj_16_value;
        number localvalue = v;
    
        if (this->numberobj_16_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("EQ&Volume~/number_obj-32"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_16_output_set(localvalue);
    }
    
    void eventinlet_06_out1_number_set(number v) {
        this->numberobj_16_value_set(v);
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
    
    void numberobj_12_output_set(number v) {
        {
            list converted = {v};
            this->linetilde_03_segments_set(converted);
        }
    }
    
    void numberobj_12_value_set(number v) {
        this->numberobj_12_value_setter(v);
        v = this->numberobj_12_value;
        number localvalue = v;
    
        if (this->numberobj_12_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("EQ&Volume~/number_obj-45"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_12_output_set(localvalue);
    }
    
    void expr_07_out1_set(number v) {
        this->expr_07_out1 = v;
        this->numberobj_12_value_set(this->expr_07_out1);
    }
    
    void expr_07_in1_set(number in1) {
        this->expr_07_in1 = in1;
        this->expr_07_out1_set(rnbo_pow(10, this->expr_07_in1 * 0.05));//#map:EQ&Volume~/dbtoa_obj-44:1
    }
    
    void numberobj_13_output_set(number v) {
        this->expr_07_in1_set(v);
    }
    
    void numberobj_13_value_set(number v) {
        this->numberobj_13_value_setter(v);
        v = this->numberobj_13_value;
        number localvalue = v;
    
        if (this->numberobj_13_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("EQ&Volume~/number_obj-37"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_13_output_set(localvalue);
    }
    
    void scale_05_out_set(const list& v) {
        this->scale_05_out = jsCreateListCopy(v);
    
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_13_value_set(converted);
        }
    }
    
    void scale_05_input_set(const list& v) {
        this->scale_05_input = jsCreateListCopy(v);
        list tmp = {};
    
        for (Index i = 0; i < v->length; i++) {
            tmp->push(this->scale(
                v[(Index)i],
                this->scale_05_inlow,
                this->scale_05_inhigh,
                this->scale_05_outlow,
                this->scale_05_outhigh,
                this->scale_05_power
            ));
        }
    
        this->scale_05_out_set(tmp);
    }
    
    void numberobj_14_output_set(number v) {
        {
            list converted = {v};
            this->scale_05_input_set(converted);
        }
    }
    
    void numberobj_14_value_set(number v) {
        this->numberobj_14_value_setter(v);
        v = this->numberobj_14_value;
        number localvalue = v;
    
        if (this->numberobj_14_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("EQ&Volume~/number_obj-40"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_14_output_set(localvalue);
    }
    
    void eventinlet_05_out1_number_set(number v) {
        this->numberobj_14_value_set(v);
    }
    
    void numberobj_11_value_bang() {
        number v = this->numberobj_11_value;
        number localvalue = v;
    
        if (this->numberobj_11_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("EQ&Volume~/number_obj-12"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_11_output_set(localvalue);
    }
    
    void numberobj_14_value_bang() {
        number v = this->numberobj_14_value;
        number localvalue = v;
    
        if (this->numberobj_14_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("EQ&Volume~/number_obj-40"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_14_output_set(localvalue);
    }
    
    void numberobj_16_value_bang() {
        number v = this->numberobj_16_value;
        number localvalue = v;
    
        if (this->numberobj_16_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(
            TAG("valout"),
            TAG("EQ&Volume~/number_obj-32"),
            localvalue,
            this->_currentTime
        );
    
        this->numberobj_16_output_set(localvalue);
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
                            -1841075377,
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
    
    void dspexpr_22_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
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
    
    void dspexpr_25_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] - in2[(Index)i];//#map:_###_obj_###_:1
        }
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
                            -281953904,
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
                            894770446,
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
    
    void dspexpr_24_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_23_perform(const Sample * in1, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = rnbo_atan(in1[(Index)i]);//#map:_###_obj_###_:1
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
    
    void dspexpr_21_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_20_perform(const Sample * in1, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = rnbo_atan(in1[(Index)i]);//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_10_value_setter(number v) {
        number localvalue = v;
    
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
    
        if (this->numberobj_12_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_12_value = localvalue;
    }
    
    void numberobj_13_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_13_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_13_value = localvalue;
    }
    
    void numberobj_14_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_14_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_14_value = localvalue;
    }
    
    void numberobj_15_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_15_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_15_value = localvalue;
    }
    
    void numberobj_16_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_16_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_16_value = localvalue;
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
    
    void numberobj_10_init() {
        this->numberobj_10_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("EQ&Volume~/number_obj-21"), 1, this->_currentTime);
    }
    
    void numberobj_10_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_10_value;
    }
    
    void numberobj_10_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_10_value_set(preset["value"]);
    }
    
    void numberobj_11_init() {
        this->numberobj_11_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("EQ&Volume~/number_obj-12"), 1, this->_currentTime);
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
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("EQ&Volume~/number_obj-45"), 1, this->_currentTime);
    }
    
    void numberobj_12_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_12_value;
    }
    
    void numberobj_12_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_12_value_set(preset["value"]);
    }
    
    void numberobj_13_init() {
        this->numberobj_13_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("EQ&Volume~/number_obj-37"), 1, this->_currentTime);
    }
    
    void numberobj_13_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_13_value;
    }
    
    void numberobj_13_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_13_value_set(preset["value"]);
    }
    
    void numberobj_14_init() {
        this->numberobj_14_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("EQ&Volume~/number_obj-40"), 1, this->_currentTime);
    }
    
    void numberobj_14_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_14_value;
    }
    
    void numberobj_14_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_14_value_set(preset["value"]);
    }
    
    void numberobj_15_init() {
        this->numberobj_15_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("EQ&Volume~/number_obj-27"), 1, this->_currentTime);
    }
    
    void numberobj_15_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_15_value;
    }
    
    void numberobj_15_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_15_value_set(preset["value"]);
    }
    
    void numberobj_16_init() {
        this->numberobj_16_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("EQ&Volume~/number_obj-32"), 1, this->_currentTime);
    }
    
    void numberobj_16_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_16_value;
    }
    
    void numberobj_16_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_16_value_set(preset["value"]);
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
        dspexpr_20_in1 = 0;
        dspexpr_21_in1 = 0;
        dspexpr_21_in2 = 0;
        onepole_tilde_01_x = 0;
        onepole_tilde_01_freqInHz = 1;
        dspexpr_22_in1 = 0;
        dspexpr_22_in2 = 0;
        onepole_tilde_02_x = 0;
        onepole_tilde_02_freqInHz = 1;
        dspexpr_23_in1 = 0;
        dspexpr_24_in1 = 0;
        dspexpr_24_in2 = 0;
        onepole_tilde_03_x = 0;
        onepole_tilde_03_freqInHz = 1;
        dspexpr_25_in1 = 0;
        dspexpr_25_in2 = 0;
        onepole_tilde_04_x = 0;
        onepole_tilde_04_freqInHz = 1;
        numberobj_10_value = 0;
        numberobj_10_value_setter(numberobj_10_value);
        scale_04_inlow = 0;
        scale_04_inhigh = 100;
        scale_04_outlow = 20;
        scale_04_outhigh = 1000;
        scale_04_power = 1;
        numberobj_11_value = 0;
        numberobj_11_value_setter(numberobj_11_value);
        linetilde_02_time = 10;
        linetilde_02_keepramp = 0;
        numberobj_12_value = 0;
        numberobj_12_value_setter(numberobj_12_value);
        expr_07_in1 = 0;
        expr_07_out1 = 0;
        numberobj_13_value = 0;
        numberobj_13_value_setter(numberobj_13_value);
        scale_05_inlow = -100;
        scale_05_inhigh = 100;
        scale_05_outlow = -18;
        scale_05_outhigh = 18;
        scale_05_power = 1;
        numberobj_14_value = 0;
        numberobj_14_value_setter(numberobj_14_value);
        numberobj_15_value = 0;
        numberobj_15_value_setter(numberobj_15_value);
        scale_06_inlow = 0;
        scale_06_inhigh = 100;
        scale_06_outlow = 10000;
        scale_06_outhigh = 200;
        scale_06_power = 0.1;
        numberobj_16_value = 0;
        numberobj_16_value_setter(numberobj_16_value);
        linetilde_03_time = 10;
        linetilde_03_keepramp = 0;
        linetilde_04_time = 10;
        linetilde_04_keepramp = 0;
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
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        onepole_tilde_01_freq = 0;
        onepole_tilde_01_needsUpdate = false;
        onepole_tilde_01_lastY = 0;
        onepole_tilde_01_a0 = 0;
        onepole_tilde_01_b1 = 0;
        onepole_tilde_01_setupDone = false;
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
        onepole_tilde_04_freq = 0;
        onepole_tilde_04_needsUpdate = false;
        onepole_tilde_04_lastY = 0;
        onepole_tilde_04_a0 = 0;
        onepole_tilde_04_b1 = 0;
        onepole_tilde_04_setupDone = false;
        numberobj_10_currentFormat = 6;
        numberobj_10_lastValue = 0;
        numberobj_11_currentFormat = 6;
        numberobj_11_lastValue = 0;
        linetilde_02_currentValue = 20;
        numberobj_12_currentFormat = 6;
        numberobj_12_lastValue = 0;
        numberobj_13_currentFormat = 6;
        numberobj_13_lastValue = 0;
        numberobj_14_currentFormat = 6;
        numberobj_14_lastValue = 0;
        numberobj_15_currentFormat = 6;
        numberobj_15_lastValue = 0;
        numberobj_16_currentFormat = 6;
        numberobj_16_lastValue = 0;
        linetilde_03_currentValue = 1;
        linetilde_04_currentValue = 10000;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_20_in1;
        number dspexpr_21_in1;
        number dspexpr_21_in2;
        number onepole_tilde_01_x;
        number onepole_tilde_01_freqInHz;
        number dspexpr_22_in1;
        number dspexpr_22_in2;
        number onepole_tilde_02_x;
        number onepole_tilde_02_freqInHz;
        number dspexpr_23_in1;
        number dspexpr_24_in1;
        number dspexpr_24_in2;
        number onepole_tilde_03_x;
        number onepole_tilde_03_freqInHz;
        number dspexpr_25_in1;
        number dspexpr_25_in2;
        number onepole_tilde_04_x;
        number onepole_tilde_04_freqInHz;
        number numberobj_10_value;
        list scale_04_input;
        number scale_04_inlow;
        number scale_04_inhigh;
        number scale_04_outlow;
        number scale_04_outhigh;
        number scale_04_power;
        list scale_04_out;
        number numberobj_11_value;
        list linetilde_02_segments;
        number linetilde_02_time;
        number linetilde_02_keepramp;
        number numberobj_12_value;
        number expr_07_in1;
        number expr_07_out1;
        number numberobj_13_value;
        list scale_05_input;
        number scale_05_inlow;
        number scale_05_inhigh;
        number scale_05_outlow;
        number scale_05_outhigh;
        number scale_05_power;
        list scale_05_out;
        number numberobj_14_value;
        number numberobj_15_value;
        list scale_06_input;
        number scale_06_inlow;
        number scale_06_inhigh;
        number scale_06_outlow;
        number scale_06_outhigh;
        number scale_06_power;
        list scale_06_out;
        number numberobj_16_value;
        list linetilde_03_segments;
        number linetilde_03_time;
        number linetilde_03_keepramp;
        list linetilde_04_segments;
        number linetilde_04_time;
        number linetilde_04_keepramp;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[5];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        number onepole_tilde_01_freq;
        bool onepole_tilde_01_needsUpdate;
        number onepole_tilde_01_lastY;
        number onepole_tilde_01_a0;
        number onepole_tilde_01_b1;
        bool onepole_tilde_01_setupDone;
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
        number onepole_tilde_04_freq;
        bool onepole_tilde_04_needsUpdate;
        number onepole_tilde_04_lastY;
        number onepole_tilde_04_a0;
        number onepole_tilde_04_b1;
        bool onepole_tilde_04_setupDone;
        Int numberobj_10_currentFormat;
        number numberobj_10_lastValue;
        Int numberobj_11_currentFormat;
        number numberobj_11_lastValue;
        list linetilde_02_activeRamps;
        number linetilde_02_currentValue;
        Int numberobj_12_currentFormat;
        number numberobj_12_lastValue;
        Int numberobj_13_currentFormat;
        number numberobj_13_lastValue;
        Int numberobj_14_currentFormat;
        number numberobj_14_lastValue;
        Int numberobj_15_currentFormat;
        number numberobj_15_lastValue;
        Int numberobj_16_currentFormat;
        number numberobj_16_lastValue;
        list linetilde_03_activeRamps;
        number linetilde_03_currentValue;
        list linetilde_04_activeRamps;
        number linetilde_04_currentValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_51 : public PatcherInterfaceImpl {
    
    friend class freezer;
    
    public:
    
    RNBOSubpatcher_51()
    {
    }
    
    ~RNBOSubpatcher_51()
    {
    }
    
    virtual freezer* getPatcher() const {
        return static_cast<freezer *>(_parentPatcher);
    }
    
    freezer* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -1357044121, false);
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
        RNBO_UNUSED(numOutputs);
        RNBO_UNUSED(outputs);
        RNBO_UNUSED(numInputs);
        RNBO_UNUSED(inputs);
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
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
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -1357044121:
            {
            this->delay_02_out_bang();
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
            if (TAG("Freeze/number_obj-2") == objectId)
                this->numberobj_17_valin_set(payload);
    
            if (TAG("Freeze/number_obj-81") == objectId)
                this->numberobj_18_valin_set(payload);
    
            if (TAG("Freeze/toggle_obj-4") == objectId)
                this->toggle_03_valin_set(payload);
    
            if (TAG("Freeze/toggle_obj-5") == objectId)
                this->toggle_04_valin_set(payload);
    
            if (TAG("Freeze/number_obj-8") == objectId)
                this->numberobj_19_valin_set(payload);
    
            if (TAG("Freeze/toggle_obj-11") == objectId)
                this->toggle_05_valin_set(payload);
    
            break;
            }
        case TAG("format"):
            {
            if (TAG("Freeze/number_obj-2") == objectId)
                this->numberobj_17_format_set(payload);
    
            if (TAG("Freeze/number_obj-81") == objectId)
                this->numberobj_18_format_set(payload);
    
            if (TAG("Freeze/number_obj-8") == objectId)
                this->numberobj_19_format_set(payload);
    
            break;
            }
        case TAG("listin"):
            {
            if (TAG("Freeze/message_obj-56") == objectId)
                this->message_03_listin_number_set(payload);
    
            if (TAG("Freeze/message_obj-64") == objectId)
                this->message_04_listin_number_set(payload);
    
            if (TAG("Freeze/message_obj-91") == objectId)
                this->message_05_listin_number_set(payload);
    
            if (TAG("Freeze/message_obj-93") == objectId)
                this->message_06_listin_number_set(payload);
    
            if (TAG("Freeze/message_obj-62") == objectId)
                this->message_07_listin_number_set(payload);
    
            if (TAG("Freeze/message_obj-69") == objectId)
                this->message_08_listin_number_set(payload);
    
            if (TAG("Freeze/message_obj-92") == objectId)
                this->message_09_listin_number_set(payload);
    
            if (TAG("Freeze/message_obj-94") == objectId)
                this->message_10_listin_number_set(payload);
    
            break;
            }
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("listin"):
            {
            if (TAG("Freeze/message_obj-56") == objectId)
                this->message_03_listin_list_set(payload);
    
            if (TAG("Freeze/message_obj-64") == objectId)
                this->message_04_listin_list_set(payload);
    
            if (TAG("Freeze/message_obj-91") == objectId)
                this->message_05_listin_list_set(payload);
    
            if (TAG("Freeze/message_obj-93") == objectId)
                this->message_06_listin_list_set(payload);
    
            if (TAG("Freeze/message_obj-62") == objectId)
                this->message_07_listin_list_set(payload);
    
            if (TAG("Freeze/message_obj-69") == objectId)
                this->message_08_listin_list_set(payload);
    
            if (TAG("Freeze/message_obj-92") == objectId)
                this->message_09_listin_list_set(payload);
    
            if (TAG("Freeze/message_obj-94") == objectId)
                this->message_10_listin_list_set(payload);
    
            break;
            }
        }
    }
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("bangin"):
            {
            if (TAG("Freeze/button_obj-6") == objectId)
                this->button_05_bangin_bang();
    
            if (TAG("Freeze/button_obj-88") == objectId)
                this->button_06_bangin_bang();
    
            if (TAG("Freeze/button_obj-220") == objectId)
                this->button_07_bangin_bang();
    
            if (TAG("Freeze/button_obj-89") == objectId)
                this->button_08_bangin_bang();
    
            break;
            }
        case TAG("listin"):
            {
            if (TAG("Freeze/message_obj-56") == objectId)
                this->message_03_listin_bang_bang();
    
            if (TAG("Freeze/message_obj-64") == objectId)
                this->message_04_listin_bang_bang();
    
            if (TAG("Freeze/message_obj-91") == objectId)
                this->message_05_listin_bang_bang();
    
            if (TAG("Freeze/message_obj-93") == objectId)
                this->message_06_listin_bang_bang();
    
            if (TAG("Freeze/message_obj-62") == objectId)
                this->message_07_listin_bang_bang();
    
            if (TAG("Freeze/message_obj-69") == objectId)
                this->message_08_listin_bang_bang();
    
            if (TAG("Freeze/message_obj-92") == objectId)
                this->message_09_listin_bang_bang();
    
            if (TAG("Freeze/message_obj-94") == objectId)
                this->message_10_listin_bang_bang();
    
            break;
            }
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("valout"):
            {
            return "valout";
            }
        case TAG("Freeze/number_obj-2"):
            {
            return "Freeze/number_obj-2";
            }
        case TAG("setup"):
            {
            return "setup";
            }
        case TAG("bangout"):
            {
            return "bangout";
            }
        case TAG("Freeze/button_obj-6"):
            {
            return "Freeze/button_obj-6";
            }
        case TAG("Freeze/number_obj-81"):
            {
            return "Freeze/number_obj-81";
            }
        case TAG("Freeze/button_obj-88"):
            {
            return "Freeze/button_obj-88";
            }
        case TAG("listout"):
            {
            return "listout";
            }
        case TAG("Freeze/message_obj-56"):
            {
            return "Freeze/message_obj-56";
            }
        case TAG("Freeze/message_obj-64"):
            {
            return "Freeze/message_obj-64";
            }
        case TAG("Freeze/toggle_obj-4"):
            {
            return "Freeze/toggle_obj-4";
            }
        case TAG("Freeze/message_obj-91"):
            {
            return "Freeze/message_obj-91";
            }
        case TAG("Freeze/toggle_obj-5"):
            {
            return "Freeze/toggle_obj-5";
            }
        case TAG("Freeze/button_obj-220"):
            {
            return "Freeze/button_obj-220";
            }
        case TAG("Freeze/message_obj-93"):
            {
            return "Freeze/message_obj-93";
            }
        case TAG("Freeze/number_obj-8"):
            {
            return "Freeze/number_obj-8";
            }
        case TAG("Freeze/message_obj-62"):
            {
            return "Freeze/message_obj-62";
            }
        case TAG("Freeze/message_obj-69"):
            {
            return "Freeze/message_obj-69";
            }
        case TAG("Freeze/toggle_obj-11"):
            {
            return "Freeze/toggle_obj-11";
            }
        case TAG("Freeze/message_obj-92"):
            {
            return "Freeze/message_obj-92";
            }
        case TAG("Freeze/button_obj-89"):
            {
            return "Freeze/button_obj-89";
            }
        case TAG("Freeze/message_obj-94"):
            {
            return "Freeze/message_obj-94";
            }
        case TAG("valin"):
            {
            return "valin";
            }
        case TAG("format"):
            {
            return "format";
            }
        case TAG("bangin"):
            {
            return "bangin";
            }
        case TAG("listin"):
            {
            return "listin";
            }
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
    
    void numberobj_17_valin_set(number v) {
        this->numberobj_17_value_set(v);
    }
    
    void numberobj_17_format_set(number v) {
        this->numberobj_17_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void button_05_bangin_bang() {
        this->button_05_bangval_bang();
    }
    
    void numberobj_18_valin_set(number v) {
        this->numberobj_18_value_set(v);
    }
    
    void numberobj_18_format_set(number v) {
        this->numberobj_18_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void button_06_bangin_bang() {
        this->button_06_bangval_bang();
    }
    
    void message_03_listin_list_set(const list& v) {
        this->message_03_set_set(v);
    }
    
    void message_03_listin_number_set(number v) {
        this->message_03_set_set(v);
    }
    
    void message_03_listin_bang_bang() {
        this->message_03_trigger_bang();
    }
    
    void message_04_listin_list_set(const list& v) {
        this->message_04_set_set(v);
    }
    
    void message_04_listin_number_set(number v) {
        this->message_04_set_set(v);
    }
    
    void message_04_listin_bang_bang() {
        this->message_04_trigger_bang();
    }
    
    void delay_02_out_bang() {
        this->onebang_01_reset_bang();
    }
    
    void toggle_03_valin_set(number v) {
        this->toggle_03_value_number_set(v);
    }
    
    void message_05_listin_list_set(const list& v) {
        this->message_05_set_set(v);
    }
    
    void message_05_listin_number_set(number v) {
        this->message_05_set_set(v);
    }
    
    void message_05_listin_bang_bang() {
        this->message_05_trigger_bang();
    }
    
    void toggle_04_valin_set(number v) {
        this->toggle_04_value_number_set(v);
    }
    
    void eventinlet_07_out1_bang_bang() {
        this->toggle_04_value_bang_bang();
    }
    
    void eventinlet_07_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->toggle_04_value_number_set(converted);
        }
    }
    
    void button_07_bangin_bang() {
        this->button_07_bangval_bang();
    }
    
    void message_06_listin_list_set(const list& v) {
        this->message_06_set_set(v);
    }
    
    void message_06_listin_number_set(number v) {
        this->message_06_set_set(v);
    }
    
    void message_06_listin_bang_bang() {
        this->message_06_trigger_bang();
    }
    
    void numberobj_19_valin_set(number v) {
        this->numberobj_19_value_set(v);
    }
    
    void numberobj_19_format_set(number v) {
        this->numberobj_19_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
    }
    
    void message_07_listin_list_set(const list& v) {
        this->message_07_set_set(v);
    }
    
    void message_07_listin_number_set(number v) {
        this->message_07_set_set(v);
    }
    
    void message_07_listin_bang_bang() {
        this->message_07_trigger_bang();
    }
    
    void message_08_listin_list_set(const list& v) {
        this->message_08_set_set(v);
    }
    
    void message_08_listin_number_set(number v) {
        this->message_08_set_set(v);
    }
    
    void message_08_listin_bang_bang() {
        this->message_08_trigger_bang();
    }
    
    void toggle_05_valin_set(number v) {
        this->toggle_05_value_number_set(v);
    }
    
    void message_09_listin_list_set(const list& v) {
        this->message_09_set_set(v);
    }
    
    void message_09_listin_number_set(number v) {
        this->message_09_set_set(v);
    }
    
    void message_09_listin_bang_bang() {
        this->message_09_trigger_bang();
    }
    
    void button_08_bangin_bang() {
        this->button_08_bangval_bang();
    }
    
    void message_10_listin_list_set(const list& v) {
        this->message_10_set_set(v);
    }
    
    void message_10_listin_number_set(number v) {
        this->message_10_set_set(v);
    }
    
    void message_10_listin_bang_bang() {
        this->message_10_trigger_bang();
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
        return 0;
    }
    
    Index getNumOutputChannels() const {
        return 0;
    }
    
    void initializeObjects() {
        this->numberobj_17_init();
        this->numberobj_18_init();
        this->message_03_init();
        this->message_04_init();
        this->message_05_init();
        this->message_06_init();
        this->numberobj_19_init();
        this->message_07_init();
        this->message_08_init();
        this->message_09_init();
        this->message_10_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {}
    
    void delay_02_stop_bang() {
        this->getEngine()->flushClockEvents(this, -1357044121, false);;
    }
    
    void delay_02_input_bang() {
        if ((bool)(!(bool)(this->delay_02_delayall)))
            this->delay_02_stop_bang();
    
        this->getEngine()->scheduleClockEvent(this, -1357044121, this->delay_02_time + this->_currentTime);;
    }
    
    void eventoutlet_05_in1_number_set(number v) {
        this->getPatcher()->updateTime(this->_currentTime);
        this->getPatcher()->p_05_out4_number_set(v);
    }
    
    void toggle_05_output_set(number v) {
        this->eventoutlet_05_in1_number_set(v);
    }
    
    void toggle_05_value_number_set(number v) {
        this->toggle_05_value_number_setter(v);
        v = this->toggle_05_value_number;
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Freeze/toggle_obj-11"), v, this->_currentTime);
        this->toggle_05_output_set(v);
    }
    
    void message_10_out_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->toggle_05_value_number_set(converted);
        }
    }
    
    void message_10_trigger_bang() {
        this->message_10_out_set(this->message_10_set);
    }
    
    void eventoutlet_04_in1_number_set(number v) {
        this->getPatcher()->updateTime(this->_currentTime);
        this->getPatcher()->p_05_out3_number_set(v);
    }
    
    void numberobj_19_output_set(number v) {
        this->eventoutlet_04_in1_number_set(v);
    }
    
    void numberobj_19_value_set(number v) {
        this->numberobj_19_value_setter(v);
        v = this->numberobj_19_value;
        number localvalue = v;
    
        if (this->numberobj_19_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Freeze/number_obj-8"), localvalue, this->_currentTime);
        this->numberobj_19_output_set(localvalue);
    }
    
    void message_08_out_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_19_value_set(converted);
        }
    }
    
    void message_08_trigger_bang() {
        this->message_08_out_set(this->message_08_set);
    }
    
    void eventoutlet_03_in1_number_set(number v) {
        this->getPatcher()->updateTime(this->_currentTime);
        this->getPatcher()->p_05_out2_number_set(v);
    }
    
    void toggle_03_output_set(number v) {
        this->eventoutlet_03_in1_number_set(v);
    }
    
    void toggle_03_value_number_set(number v) {
        this->toggle_03_value_number_setter(v);
        v = this->toggle_03_value_number;
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Freeze/toggle_obj-4"), v, this->_currentTime);
        this->toggle_03_output_set(v);
    }
    
    void message_05_out_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->toggle_03_value_number_set(converted);
        }
    }
    
    void message_05_trigger_bang() {
        this->message_05_out_set(this->message_05_set);
    }
    
    void eventoutlet_02_in1_number_set(number v) {
        this->getPatcher()->updateTime(this->_currentTime);
        this->getPatcher()->p_05_out1_number_set(v);
    }
    
    void numberobj_17_output_set(number v) {
        this->eventoutlet_02_in1_number_set(v);
    }
    
    void numberobj_17_value_set(number v) {
        this->numberobj_17_value_setter(v);
        v = this->numberobj_17_value;
        number localvalue = v;
    
        if (this->numberobj_17_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Freeze/number_obj-2"), localvalue, this->_currentTime);
        this->numberobj_17_output_set(localvalue);
    }
    
    void message_03_out_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_17_value_set(converted);
        }
    }
    
    void message_03_trigger_bang() {
        this->message_03_out_set(this->message_03_set);
    }
    
    void button_06_output_bang() {
        this->message_10_trigger_bang();
        this->message_08_trigger_bang();
        this->message_05_trigger_bang();
        this->message_03_trigger_bang();
    }
    
    void button_06_bangval_bang() {
        this->getEngine()->sendBangMessage(TAG("bangout"), TAG("Freeze/button_obj-88"), this->_currentTime);;
        this->button_06_output_bang();
    }
    
    void select_02_match1_bang() {
        this->button_06_bangval_bang();
    }
    
    void message_09_out_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->toggle_05_value_number_set(converted);
        }
    }
    
    void message_09_trigger_bang() {
        this->message_09_out_set(this->message_09_set);
    }
    
    void message_07_out_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_19_value_set(converted);
        }
    }
    
    void message_07_trigger_bang() {
        this->message_07_out_set(this->message_07_set);
    }
    
    void message_06_out_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->toggle_03_value_number_set(converted);
        }
    }
    
    void message_06_trigger_bang() {
        this->message_06_out_set(this->message_06_set);
    }
    
    void message_04_out_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->numberobj_17_value_set(converted);
        }
    }
    
    void message_04_trigger_bang() {
        this->message_04_out_set(this->message_04_set);
    }
    
    void button_08_output_bang() {
        this->message_09_trigger_bang();
        this->message_07_trigger_bang();
        this->message_06_trigger_bang();
        this->message_04_trigger_bang();
    }
    
    void button_08_bangval_bang() {
        this->getEngine()->sendBangMessage(TAG("bangout"), TAG("Freeze/button_obj-89"), this->_currentTime);;
        this->button_08_output_bang();
    }
    
    void select_02_match2_bang() {
        this->button_08_bangval_bang();
    }
    
    void select_02_nomatch_number_set(number ) {}
    
    void select_02_input_number_set(number v) {
        if (v == this->select_02_test1)
            this->select_02_match1_bang();
        else if (v == this->select_02_test2)
            this->select_02_match2_bang();
        else
            this->select_02_nomatch_number_set(v);
    }
    
    void numberobj_18_output_set(number v) {
        this->select_02_input_number_set(v);
    }
    
    void numberobj_18_value_set(number v) {
        this->numberobj_18_value_setter(v);
        v = this->numberobj_18_value;
        number localvalue = v;
    
        if (this->numberobj_18_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Freeze/number_obj-81"), localvalue, this->_currentTime);
        this->numberobj_18_output_set(localvalue);
    }
    
    void counter_01_output_set(number v) {
        this->counter_01_output = v;
        this->numberobj_18_value_set(v);
    }
    
    void counter_01_overflow_bang_bang() {}
    
    static number counter_01_overflow_number_constrain(number v) {
        if (v < 0)
            v = 0;
    
        if (v > 1)
            v = 1;
    
        return v;
    }
    
    void counter_01_overflow_number_set(number v) {
        v = this->counter_01_overflow_number_constrain(v);
        this->counter_01_overflow_number = v;
    }
    
    void counter_01_carry_set(number v) {
        this->counter_01_carry = v;
    }
    
    void counter_01_underflow_bang_bang() {}
    
    static number counter_01_underflow_number_constrain(number v) {
        if (v < 0)
            v = 0;
    
        if (v > 1)
            v = 1;
    
        return v;
    }
    
    void counter_01_underflow_number_set(number v) {
        v = this->counter_01_underflow_number_constrain(v);
        this->counter_01_underflow_number = v;
    }
    
    void counter_01_input_bang() {
        this->counter_01_output_set(this->counter_01_count);
        this->counter_01_inc();
    
        if (this->counter_01_count > this->counter_01_maximum) {
            if (this->counter_01_direction == 2) {
                this->counter_01_phase = !(bool)(this->counter_01_phase);
                this->counter_01_count = this->counter_01_maximum;
                this->counter_01_inc();
            } else
                this->counter_01_count = this->counter_01_minimum;
    
            if ((bool)(!(bool)(this->counter_01_overflow_number))) {
                if (this->counter_01_carryflag == 1)
                    this->counter_01_overflow_bang_bang();
                else
                    this->counter_01_overflow_number_set(1);
            }
    
            this->counter_01_carry_set(this->counter_01_carry + 1);
        } else if ((bool)(this->counter_01_overflow_number) && this->counter_01_carryflag == 0)
            this->counter_01_overflow_number_set(0);
    
        if (this->counter_01_count < this->counter_01_minimum) {
            if (this->counter_01_direction == 2) {
                this->counter_01_phase = !(bool)(this->counter_01_phase);
                this->counter_01_count = this->counter_01_minimum;
                this->counter_01_inc();
            } else
                this->counter_01_count = this->counter_01_maximum;
    
            if ((bool)(!(bool)(this->counter_01_underflow_number))) {
                if (this->counter_01_carryflag == 1)
                    this->counter_01_underflow_bang_bang();
                else
                    this->counter_01_underflow_number_set(1);
            }
    
            this->counter_01_carry_set(this->counter_01_carry + 1);
        } else if ((bool)(this->counter_01_underflow_number) && this->counter_01_carryflag == 0)
            this->counter_01_underflow_number_set(0);
    }
    
    void onebang_01_out1_bang() {
        this->delay_02_input_bang();
        this->counter_01_input_bang();
    }
    
    void onebang_01_out2_bang() {}
    
    void onebang_01_input_bang() {
        if ((bool)(this->onebang_01_allow)) {
            this->onebang_01_out1_bang();
            this->onebang_01_allow = false;
        } else
            this->onebang_01_out2_bang();
    }
    
    void button_05_output_bang() {
        this->onebang_01_input_bang();
    }
    
    void button_05_bangval_bang() {
        this->getEngine()->sendBangMessage(TAG("bangout"), TAG("Freeze/button_obj-6"), this->_currentTime);;
        this->button_05_output_bang();
    }
    
    void select_03_match1_bang() {
        this->button_05_bangval_bang();
    }
    
    void button_07_output_bang() {
        this->message_10_trigger_bang();
        this->message_08_trigger_bang();
        this->message_06_trigger_bang();
        this->message_04_trigger_bang();
    }
    
    void button_07_bangval_bang() {
        this->getEngine()->sendBangMessage(TAG("bangout"), TAG("Freeze/button_obj-220"), this->_currentTime);;
        this->button_07_output_bang();
    }
    
    void select_03_match2_bang() {
        this->button_07_bangval_bang();
    }
    
    void select_03_nomatch_number_set(number ) {}
    
    void select_03_input_number_set(number v) {
        if (v == this->select_03_test1)
            this->select_03_match1_bang();
        else if (v == this->select_03_test2)
            this->select_03_match2_bang();
        else
            this->select_03_nomatch_number_set(v);
    }
    
    void toggle_04_output_set(number v) {
        this->select_03_input_number_set(v);
    }
    
    void toggle_04_value_number_set(number v) {
        this->toggle_04_value_number_setter(v);
        v = this->toggle_04_value_number;
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Freeze/toggle_obj-5"), v, this->_currentTime);
        this->toggle_04_output_set(v);
    }
    
    void eventinlet_07_out1_number_set(number v) {
        this->toggle_04_value_number_set(v);
    }
    
    void message_03_set_set(const list& v) {
        this->message_03_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Freeze/message_obj-56"), v, this->_currentTime);
    }
    
    void message_04_set_set(const list& v) {
        this->message_04_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Freeze/message_obj-64"), v, this->_currentTime);
    }
    
    void onebang_01_reset_bang() {
        this->onebang_01_allow = true;
    }
    
    void message_05_set_set(const list& v) {
        this->message_05_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Freeze/message_obj-91"), v, this->_currentTime);
    }
    
    void toggle_04_value_bang_bang() {
        number val = (this->toggle_04_value_number == 1 ? 0 : 1);
        this->toggle_04_value_number_set(val);
    }
    
    void message_06_set_set(const list& v) {
        this->message_06_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Freeze/message_obj-93"), v, this->_currentTime);
    }
    
    void message_07_set_set(const list& v) {
        this->message_07_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Freeze/message_obj-62"), v, this->_currentTime);
    }
    
    void message_08_set_set(const list& v) {
        this->message_08_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Freeze/message_obj-69"), v, this->_currentTime);
    }
    
    void message_09_set_set(const list& v) {
        this->message_09_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Freeze/message_obj-92"), v, this->_currentTime);
    }
    
    void message_10_set_set(const list& v) {
        this->message_10_set = jsCreateListCopy(v);
        this->getEngine()->sendListMessage(TAG("listout"), TAG("Freeze/message_obj-94"), v, this->_currentTime);
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void numberobj_17_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_17_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_17_value = localvalue;
    }
    
    void onebang_01_initial_setter(number v) {
        if (v > 0)
            this->onebang_01_allow = true;
        else
            this->onebang_01_allow = false;
    }
    
    void numberobj_18_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_18_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_18_value = localvalue;
    }
    
    void toggle_03_value_number_setter(number v) {
        this->toggle_03_value_number = (v != 0 ? 1 : 0);
    }
    
    void toggle_04_value_number_setter(number v) {
        this->toggle_04_value_number = (v != 0 ? 1 : 0);
    }
    
    void numberobj_19_value_setter(number v) {
        number localvalue = v;
    
        if (this->numberobj_19_currentFormat != 6) {
            localvalue = trunc(localvalue);
        }
    
        this->numberobj_19_value = localvalue;
    }
    
    void toggle_05_value_number_setter(number v) {
        this->toggle_05_value_number = (v != 0 ? 1 : 0);
    }
    
    void numberobj_17_init() {
        this->numberobj_17_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Freeze/number_obj-2"), 1, this->_currentTime);
    }
    
    void numberobj_17_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_17_value;
    }
    
    void numberobj_17_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_17_value_set(preset["value"]);
    }
    
    void numberobj_18_init() {
        this->numberobj_18_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Freeze/number_obj-81"), 1, this->_currentTime);
    }
    
    void numberobj_18_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_18_value;
    }
    
    void numberobj_18_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_18_value_set(preset["value"]);
    }
    
    void counter_01_inc() {
        this->counter_01_count = ((bool)(this->counter_01_phase) ? this->counter_01_count + 1 : this->counter_01_count - 1);
    }
    
    void message_03_init() {
        this->message_03_set_set({0.5});
    }
    
    void message_04_init() {
        this->message_04_set_set({0});
    }
    
    void toggle_03_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->toggle_03_value_number;
    }
    
    void toggle_03_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->toggle_03_value_number_set(preset["value"]);
    }
    
    void message_05_init() {
        this->message_05_set_set({1});
    }
    
    void toggle_04_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->toggle_04_value_number;
    }
    
    void toggle_04_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->toggle_04_value_number_set(preset["value"]);
    }
    
    void message_06_init() {
        this->message_06_set_set({0});
    }
    
    void numberobj_19_init() {
        this->numberobj_19_currentFormat = 6;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("Freeze/number_obj-8"), 1, this->_currentTime);
    }
    
    void numberobj_19_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->numberobj_19_value;
    }
    
    void numberobj_19_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->numberobj_19_value_set(preset["value"]);
    }
    
    void message_07_init() {
        this->message_07_set_set({0.5});
    }
    
    void message_08_init() {
        this->message_08_set_set({0});
    }
    
    void toggle_05_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->toggle_05_value_number;
    }
    
    void toggle_05_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->toggle_05_value_number_set(preset["value"]);
    }
    
    void message_09_init() {
        this->message_09_set_set({1});
    }
    
    void message_10_init() {
        this->message_10_set_set({0});
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
        numberobj_17_value = 0;
        numberobj_17_value_setter(numberobj_17_value);
        onebang_01_initial = 1;
        onebang_01_initial_setter(onebang_01_initial);
        select_02_test1 = 1;
        select_02_test2 = 2;
        numberobj_18_value = 0;
        numberobj_18_value_setter(numberobj_18_value);
        counter_01_carryflag = 0;
        counter_01_direction = 0;
        counter_01_resetnext_number = 0;
        counter_01_resetnow_number = 0;
        counter_01_maximum = 2;
        counter_01_output = 0;
        counter_01_underflow_number = 0;
        counter_01_overflow_number = 0;
        counter_01_carry = 0;
        counter_01_minimum = 1;
        delay_02_time = 500;
        delay_02_delayall = 1;
        toggle_03_value_number = 0;
        toggle_03_value_number_setter(toggle_03_value_number);
        select_03_test1 = 1;
        select_03_test2 = 0;
        toggle_04_value_number = 0;
        toggle_04_value_number_setter(toggle_04_value_number);
        numberobj_19_value = 0;
        numberobj_19_value_setter(numberobj_19_value);
        toggle_05_value_number = 0;
        toggle_05_value_number_setter(toggle_05_value_number);
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
        numberobj_17_currentFormat = 6;
        numberobj_17_lastValue = 0;
        numberobj_18_currentFormat = 6;
        numberobj_18_lastValue = 0;
        counter_01_count = 1;
        counter_01_phase = true;
        toggle_03_lastValue = 0;
        toggle_04_lastValue = 0;
        numberobj_19_currentFormat = 6;
        numberobj_19_lastValue = 0;
        toggle_05_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number numberobj_17_value;
        number onebang_01_initial;
        number select_02_test1;
        number select_02_test2;
        number numberobj_18_value;
        Int counter_01_carryflag;
        number counter_01_direction;
        number counter_01_resetnext_number;
        number counter_01_resetnow_number;
        number counter_01_maximum;
        number counter_01_output;
        number counter_01_underflow_number;
        number counter_01_overflow_number;
        number counter_01_carry;
        number counter_01_minimum;
        list message_03_set;
        list message_04_set;
        number delay_02_time;
        number delay_02_delayall;
        number toggle_03_value_number;
        list message_05_set;
        number select_03_test1;
        number select_03_test2;
        number toggle_04_value_number;
        list message_06_set;
        number numberobj_19_value;
        list message_07_set;
        list message_08_set;
        number toggle_05_value_number;
        list message_09_set;
        list message_10_set;
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
        Int numberobj_17_currentFormat;
        number numberobj_17_lastValue;
        bool onebang_01_allow;
        Int numberobj_18_currentFormat;
        number numberobj_18_lastValue;
        number counter_01_count;
        bool counter_01_phase;
        number toggle_03_lastValue;
        number toggle_04_lastValue;
        Int numberobj_19_currentFormat;
        number numberobj_19_lastValue;
        number toggle_05_lastValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_52 : public PatcherInterfaceImpl {
    
    friend class freezer;
    
    public:
    
    RNBOSubpatcher_52()
    {
    }
    
    ~RNBOSubpatcher_52()
    {
    }
    
    virtual freezer* getPatcher() const {
        return static_cast<freezer *>(_parentPatcher);
    }
    
    freezer* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -871642103, false);
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
    
    template <typename T> inline array<SampleValue, 2 + 1> sample_channels_2(T& buffer, SampleValue sampleIndex, Index channelOffset) {
        number bufferSize = buffer->getSize();
        const Index bufferChannels = (const Index)(buffer->getChannels());
        constexpr int ___N3 = 2 + 1;
        array<SampleValue, ___N3> out = FIXEDSIZEARRAYINIT(2 + 1);
    
        {
            SampleValue bufferphasetoindex_result;
    
            {
                auto __end = bufferSize;
                auto __start = 0;
                auto __phase = sampleIndex;
    
                {
                    number size = __end - 1 - __start;
                    bufferphasetoindex_result = __phase * size;
                }
            }
    
            sampleIndex = bufferphasetoindex_result;
        }
    
        if (bufferSize == 0 || (5 == 5 && (sampleIndex < 0 || sampleIndex >= bufferSize))) {
            for (Index i = 0; i < 2 + 1; i++) {
                out[(Index)i] = 0;
            }
        } else {
            for (Index channel = 0; channel < 2; channel++) {
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
                        number frac = sampleIndex - index1;
                        number wrap = bufferSize - 1;
                        SampleIndex index2 = (SampleIndex)(index1 + 1);
    
                        if (index2 > wrap)
                            index2 = 0;
    
                        bufferreadsample_result = this->linearinterp(
                            frac,
                            __buffer->getSample(channelIndex, index1),
                            __buffer->getSample(channelIndex, index2)
                        );
                    }
                }
    
                out[(Index)channel] = bufferreadsample_result;
            }
    
            out[2] = sampleIndex;
        }
    
        return out;
    }
    
    int notenumber() {
        return this->_noteNumber;
    }
    
    Index voice() {
        return this->_voiceIndex;
    }
    
    number samplerate() {
        return this->sr;
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
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
        const SampleValue * in1 = (numInputs >= 1 && inputs[0] ? inputs[0] : this->zeroBuffer);
    
        if (this->getIsMuted())
            return;
    
        this->dspexpr_29_perform(in1, this->dspexpr_29_in2, this->signals[0], n);
        this->rand_tilde_04_perform(this->rand_tilde_04_frequency, this->signals[1], n);
    
        this->cycle_tilde_04_perform(
            this->signals[1],
            this->cycle_tilde_04_phase_offset,
            this->signals[2],
            this->dummyBuffer,
            n
        );
    
        this->dspexpr_32_perform(this->signals[2], this->dspexpr_32_in2, this->signals[1], n);
    
        this->rampsmooth_tilde_07_perform(
            this->signals[1],
            this->rampsmooth_tilde_07_up,
            this->rampsmooth_tilde_07_down,
            this->signals[2],
            n
        );
    
        this->dspexpr_28_perform(this->signals[0], this->signals[2], this->signals[1], n);
    
        this->wrap_tilde_02_perform(
            this->signals[1],
            this->wrap_tilde_02_low,
            this->wrap_tilde_02_high,
            this->signals[2],
            n
        );
    
        this->sample_02_perform(
            this->signals[2],
            this->sample_02_offset,
            this->signals[1],
            this->signals[0],
            this->dummyBuffer,
            n
        );
    
        this->hann_tilde_02_perform(this->signals[2], this->signals[3], n);
        this->dspexpr_27_perform(this->signals[1], this->signals[3], this->signals[2], n);
        this->dspexpr_31_perform(this->signals[0], this->signals[3], this->signals[1], n);
        this->rand_tilde_05_perform(this->rand_tilde_05_frequency, this->signals[3], n);
    
        this->cycle_tilde_05_perform(
            this->signals[3],
            this->cycle_tilde_05_phase_offset,
            this->signals[0],
            this->dummyBuffer,
            n
        );
    
        this->dspexpr_35_perform(this->signals[0], this->dspexpr_35_in2, this->signals[3], n);
        this->dspexpr_34_perform(this->signals[3], this->dspexpr_34_in2, this->signals[0], n);
        this->dspexpr_33_perform(this->signals[0], this->dspexpr_33_in2, this->signals[3], n);
    
        this->rampsmooth_tilde_05_perform(
            this->signals[3],
            this->rampsmooth_tilde_05_up,
            this->rampsmooth_tilde_05_down,
            this->signals[0],
            n
        );
    
        this->dspexpr_26_perform(this->signals[2], this->signals[0], this->signals[3], n);
        this->dspexpr_30_perform(this->signals[1], this->signals[0], this->signals[2], n);
        this->rand_tilde_06_perform(this->rand_tilde_06_frequency, this->signals[0], n);
    
        this->cycle_tilde_06_perform(
            this->signals[0],
            this->cycle_tilde_06_phase_offset,
            this->signals[1],
            this->dummyBuffer,
            n
        );
    
        this->dspexpr_38_perform(this->signals[1], this->dspexpr_38_in2, this->signals[0], n);
        this->dspexpr_37_perform(this->signals[0], this->dspexpr_37_in2, this->signals[1], n);
        this->dspexpr_36_perform(this->signals[1], this->dspexpr_36_in2, this->signals[0], n);
    
        this->rampsmooth_tilde_06_perform(
            this->signals[0],
            this->rampsmooth_tilde_06_up,
            this->rampsmooth_tilde_06_down,
            this->signals[1],
            n
        );
    
        this->pan_tilde_02_perform(
            this->signals[1],
            this->signals[3],
            this->signals[2],
            this->signals[0],
            this->signals[4],
            n
        );
    
        this->signaladder_05_perform(this->signals[0], out1, out1, n);
        this->signaladder_06_perform(this->signals[4], out2, out2, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 5; i++) {
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
    
        this->rand_tilde_04_dspsetup(forceDSPSetup);
        this->cycle_tilde_04_dspsetup(forceDSPSetup);
        this->rampsmooth_tilde_07_dspsetup(forceDSPSetup);
        this->rand_tilde_05_dspsetup(forceDSPSetup);
        this->cycle_tilde_05_dspsetup(forceDSPSetup);
        this->rampsmooth_tilde_05_dspsetup(forceDSPSetup);
        this->rand_tilde_06_dspsetup(forceDSPSetup);
        this->cycle_tilde_06_dspsetup(forceDSPSetup);
        this->rampsmooth_tilde_06_dspsetup(forceDSPSetup);
    
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
    
    void setVoiceIndex(Index index)  {
        this->_voiceIndex = index;
    }
    
    void setNoteNumber(Int noteNumber)  {
        this->_noteNumber = noteNumber;
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
    
    ParameterValue getPolyParameterValue(PatcherInterface** voices, ParameterIndex index)  {
        switch (index) {
        default:
            {
            return voices[0]->getParameterValue(index);
            }
        }
    }
    
    void setPolyParameterValue(
        PatcherInterface** voices,
        ParameterIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        switch (index) {
        default:
            {
            for (Index i = 0; i < 8; i++)
                voices[i]->setParameterValue(index, value, time);
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
        if (this->_voiceIndex == 1)
            this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void sendPolyParameter(ParameterIndex index, Index voiceIndex, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset + voiceIndex - 1, ignoreValue);
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
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -871642103:
            {
            this->loadbang_03_startupbang_bang();
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
    
    void processNumMessage(MessageTag , MessageTag , MillisecondTime , number ) {}
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("startupbang"):
            {
            if (TAG("Freezer-Voice~[1]/loadbang_obj-8") == objectId)
                this->loadbang_03_startupbang_bang();
    
            break;
            }
        }
    }
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("startupbang"):
            {
            return "startupbang";
            }
        case TAG("Freezer-Voice~[1]/loadbang_obj-8"):
            {
            return "Freezer-Voice~[1]/loadbang_obj-8";
            }
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
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 2) {
            this->sample_02_buffer = new Float64Buffer(this->getPatcher()->FreezeB);
        }
    
        if (index == 1) {
            this->cycle_tilde_04_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
            this->cycle_tilde_04_bufferUpdated();
            this->cycle_tilde_05_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
            this->cycle_tilde_05_bufferUpdated();
            this->cycle_tilde_06_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
            this->cycle_tilde_06_bufferUpdated();
        }
    }
    
    void initialize() {
        this->assign_defaults();
        this->setState();
        this->sample_02_buffer = new Float64Buffer(this->getPatcher()->FreezeB);
        this->cycle_tilde_04_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
        this->cycle_tilde_05_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
        this->cycle_tilde_06_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultSinus);
    }
    
    protected:
    
    void loadbang_03_startupbang_bang() {
        this->loadbang_03_output_bang();
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
        return 1;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->rand_tilde_04_n_init();
        this->rand_tilde_04_init();
        this->rand_tilde_05_n_init();
        this->rand_tilde_05_init();
        this->rand_tilde_06_n_init();
        this->rand_tilde_06_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {
        this->updateTime(this->getEngine()->getCurrentTime());
        this->getEngine()->scheduleClockEvent(this, -871642103, 0 + this->_currentTime);;
    }
    
    void allocateDataRefs() {
        this->cycle_tilde_04_buffer->requestSize(16384, 1);
        this->cycle_tilde_04_buffer->setSampleRate(this->sr);
        this->cycle_tilde_05_buffer->requestSize(16384, 1);
        this->cycle_tilde_05_buffer->setSampleRate(this->sr);
        this->cycle_tilde_06_buffer->requestSize(16384, 1);
        this->cycle_tilde_06_buffer->setSampleRate(this->sr);
        this->sample_02_buffer = this->sample_02_buffer->allocateIfNeeded();
        this->cycle_tilde_04_buffer = this->cycle_tilde_04_buffer->allocateIfNeeded();
        this->cycle_tilde_05_buffer = this->cycle_tilde_05_buffer->allocateIfNeeded();
        this->cycle_tilde_06_buffer = this->cycle_tilde_06_buffer->allocateIfNeeded();
    }
    
    void dspexpr_32_in2_set(number v) {
        this->dspexpr_32_in2 = v;
    }
    
    void receive_10_output_number_set(number v) {
        this->receive_10_output_number = v;
        this->dspexpr_32_in2_set(v);
    }
    
    void dspexpr_38_in2_set(number v) {
        this->dspexpr_38_in2 = v;
    }
    
    void receive_12_output_number_set(number v) {
        this->receive_12_output_number = v;
        this->dspexpr_38_in2_set(v);
    }
    
    void dspexpr_35_in2_set(number v) {
        this->dspexpr_35_in2 = v;
    }
    
    void receive_11_output_number_set(number v) {
        this->receive_11_output_number = v;
        this->dspexpr_35_in2_set(v);
    }
    
    void voice_02_noteNumber_set(number ) {}
    
    void dspexpr_29_in2_set(number v) {
        this->dspexpr_29_in2 = v;
    }
    
    void expr_08_out1_set(number v) {
        this->expr_08_out1 = v;
        this->dspexpr_29_in2_set(this->expr_08_out1);
    }
    
    void expr_08_in1_set(number in1) {
        this->expr_08_in1 = in1;
    
        this->expr_08_out1_set(
            (this->expr_08_in2 == 0 ? 0 : (this->expr_08_in2 == 0. ? 0. : this->expr_08_in1 / this->expr_08_in2))
        );//#map:Freezer-Voice~[1]//_obj-7:1
    }
    
    void expr_09_out1_set(number v) {
        this->expr_09_out1 = v;
        this->expr_08_in1_set(this->expr_09_out1);
    }
    
    void expr_09_in1_set(number in1) {
        this->expr_09_in1 = in1;
        this->expr_09_out1_set(this->expr_09_in1 - this->expr_09_in2);//#map:Freezer-Voice~[1]/-_obj-6:1
    }
    
    void voice_02_voicenumber_set(number v) {
        this->expr_09_in1_set(v);
    }
    
    void voice_02_voicebang_bang() {
        this->voice_02_noteNumber_set(this->notenumber());
        this->voice_02_voicenumber_set(this->voice());
    }
    
    void loadbang_03_output_bang() {
        this->voice_02_voicebang_bang();
    }
    
    void midiouthelper_midiout_set(number ) {}
    
    void dspexpr_29_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2;//#map:_###_obj_###_:1
        }
    }
    
    void rand_tilde_04_perform(number frequency, SampleValue * out1, Index n) {
        auto __rand_tilde_04_lastValue = this->rand_tilde_04_lastValue;
        auto __rand_tilde_04_oldTargetValue = this->rand_tilde_04_oldTargetValue;
        auto __rand_tilde_04_targetValue = this->rand_tilde_04_targetValue;
        number f = (frequency > 0.0001 ? frequency : 0.0001);
        Index i;
    
        for (i = 0; i < n; i++) {
            number phasorValue = this->rand_tilde_04_p_next(f, -1);
    
            if (this->rand_tilde_04_d_next(phasorValue) < 0) {
                __rand_tilde_04_targetValue = this->rand_tilde_04_n_next();
                __rand_tilde_04_oldTargetValue = __rand_tilde_04_lastValue;
            }
    
            __rand_tilde_04_lastValue = __rand_tilde_04_oldTargetValue + phasorValue * (__rand_tilde_04_targetValue - __rand_tilde_04_oldTargetValue);
            out1[(Index)i] = __rand_tilde_04_lastValue;
        }
    
        this->rand_tilde_04_targetValue = __rand_tilde_04_targetValue;
        this->rand_tilde_04_oldTargetValue = __rand_tilde_04_oldTargetValue;
        this->rand_tilde_04_lastValue = __rand_tilde_04_lastValue;
    }
    
    void cycle_tilde_04_perform(
        const Sample * frequency,
        number phase_offset,
        SampleValue * out1,
        SampleValue * out2,
        Index n
    ) {
        RNBO_UNUSED(phase_offset);
        auto __cycle_tilde_04_buffer = this->cycle_tilde_04_buffer;
        Index i;
    
        for (i = 0; i < n; i++) {
            {
                uint32_t uint_phase;
    
                {
                    {
                        uint_phase = uint32_trunc(frequency[(Index)i] * 4294967296.0);
                    }
                }
    
                uint32_t idx = (uint32_t)(uint32_rshift(uint_phase, 18));
                number frac = ((BinOpInt)((BinOpInt)uint_phase & (BinOpInt)262143)) * 3.81471181759574e-6;
                number y0 = __cycle_tilde_04_buffer[(Index)idx];
                number y1 = __cycle_tilde_04_buffer[(Index)((BinOpInt)(idx + 1) & (BinOpInt)16383)];
                number y = y0 + frac * (y1 - y0);
                out1[(Index)i] = y;
                out2[(Index)i] = uint_phase * 0.232830643653869629e-9;
                continue;
            }
        }
    }
    
    void dspexpr_32_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void rampsmooth_tilde_07_perform(const Sample * x, number up, number down, SampleValue * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __rampsmooth_tilde_07_increment = this->rampsmooth_tilde_07_increment;
        auto __rampsmooth_tilde_07_index = this->rampsmooth_tilde_07_index;
        auto __rampsmooth_tilde_07_prev = this->rampsmooth_tilde_07_prev;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (this->rampsmooth_tilde_07_d_next(x[(Index)i]) != 0.) {
                if (x[(Index)i] > __rampsmooth_tilde_07_prev) {
                    number _up = 480;
    
                    if (_up < 1)
                        _up = 1;
    
                    __rampsmooth_tilde_07_index = _up;
                    __rampsmooth_tilde_07_increment = (x[(Index)i] - __rampsmooth_tilde_07_prev) / _up;
                } else if (x[(Index)i] < __rampsmooth_tilde_07_prev) {
                    number _down = 480;
    
                    if (_down < 1)
                        _down = 1;
    
                    __rampsmooth_tilde_07_index = _down;
                    __rampsmooth_tilde_07_increment = (x[(Index)i] - __rampsmooth_tilde_07_prev) / _down;
                }
            }
    
            if (__rampsmooth_tilde_07_index > 0) {
                __rampsmooth_tilde_07_prev += __rampsmooth_tilde_07_increment;
                __rampsmooth_tilde_07_index -= 1;
            } else {
                __rampsmooth_tilde_07_prev = x[(Index)i];
            }
    
            out1[(Index)i] = __rampsmooth_tilde_07_prev;
        }
    
        this->rampsmooth_tilde_07_prev = __rampsmooth_tilde_07_prev;
        this->rampsmooth_tilde_07_index = __rampsmooth_tilde_07_index;
        this->rampsmooth_tilde_07_increment = __rampsmooth_tilde_07_increment;
    }
    
    void dspexpr_28_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void wrap_tilde_02_perform(const Sample * x, number low, number high, SampleValue * out1, Index n) {
        RNBO_UNUSED(high);
        RNBO_UNUSED(low);
        Index i;
    
        for (i = 0; i < n; i++) {
            number lo;
            number hi;
            lo = 0;
            hi = 1;
            number range = hi - lo;
    
            if (x[(Index)i] >= lo && x[(Index)i] < hi) {
                out1[(Index)i] = x[(Index)i];
                continue;
            }
    
            if (range <= 0.000000001) {
                out1[(Index)i] = lo;
                continue;
            }
    
            long numWraps = (long)(trunc((x[(Index)i] - lo) / range));
            numWraps = numWraps - ((x[(Index)i] < lo ? 1 : 0));
            number result = x[(Index)i] - range * numWraps;
    
            if (result >= hi) {
                out1[(Index)i] = result - range;
                continue;
            } else {
                out1[(Index)i] = result;
                continue;
            }
        }
    }
    
    void sample_02_perform(
        const Sample * value,
        number offset,
        SampleValue * out1,
        SampleValue * out2,
        SampleValue * outindex,
        Index n
    ) {
        RNBO_UNUSED(out2);
        RNBO_UNUSED(out1);
        RNBO_UNUSED(offset);
        auto __sample_02_buffer = this->sample_02_buffer;
        SampleArray<2> out = {out1, out2};
    
        for (Index i = 0; i < n; i++) {
            auto result = this->sample_channels_2(__sample_02_buffer, value[(Index)i], 0);
    
            for (Index j = 0; j < 2; j++) {
                out[(Index)j][(Index)i] = result[(Index)j];
            }
    
            outindex[(Index)i] = result[2];
        }
    }
    
    void hann_tilde_02_perform(const Sample * x, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            number __x = x[(Index)i];
            __x = (__x > 1 ? 1 : (__x < 0 ? 0 : __x));
            out1[(Index)i] = 0.5 * (1 - rnbo_cos(6.28318530717958647692 * __x));
        }
    }
    
    void dspexpr_27_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_31_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void rand_tilde_05_perform(number frequency, SampleValue * out1, Index n) {
        auto __rand_tilde_05_lastValue = this->rand_tilde_05_lastValue;
        auto __rand_tilde_05_oldTargetValue = this->rand_tilde_05_oldTargetValue;
        auto __rand_tilde_05_targetValue = this->rand_tilde_05_targetValue;
        number f = (frequency > 0.0001 ? frequency : 0.0001);
        Index i;
    
        for (i = 0; i < n; i++) {
            number phasorValue = this->rand_tilde_05_p_next(f, -1);
    
            if (this->rand_tilde_05_d_next(phasorValue) < 0) {
                __rand_tilde_05_targetValue = this->rand_tilde_05_n_next();
                __rand_tilde_05_oldTargetValue = __rand_tilde_05_lastValue;
            }
    
            __rand_tilde_05_lastValue = __rand_tilde_05_oldTargetValue + phasorValue * (__rand_tilde_05_targetValue - __rand_tilde_05_oldTargetValue);
            out1[(Index)i] = __rand_tilde_05_lastValue;
        }
    
        this->rand_tilde_05_targetValue = __rand_tilde_05_targetValue;
        this->rand_tilde_05_oldTargetValue = __rand_tilde_05_oldTargetValue;
        this->rand_tilde_05_lastValue = __rand_tilde_05_lastValue;
    }
    
    void cycle_tilde_05_perform(
        const Sample * frequency,
        number phase_offset,
        SampleValue * out1,
        SampleValue * out2,
        Index n
    ) {
        RNBO_UNUSED(phase_offset);
        auto __cycle_tilde_05_buffer = this->cycle_tilde_05_buffer;
        Index i;
    
        for (i = 0; i < n; i++) {
            {
                uint32_t uint_phase;
    
                {
                    {
                        uint_phase = uint32_trunc(frequency[(Index)i] * 4294967296.0);
                    }
                }
    
                uint32_t idx = (uint32_t)(uint32_rshift(uint_phase, 18));
                number frac = ((BinOpInt)((BinOpInt)uint_phase & (BinOpInt)262143)) * 3.81471181759574e-6;
                number y0 = __cycle_tilde_05_buffer[(Index)idx];
                number y1 = __cycle_tilde_05_buffer[(Index)((BinOpInt)(idx + 1) & (BinOpInt)16383)];
                number y = y0 + frac * (y1 - y0);
                out1[(Index)i] = y;
                out2[(Index)i] = uint_phase * 0.232830643653869629e-9;
                continue;
            }
        }
    }
    
    void dspexpr_35_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_34_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + 1;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_33_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void rampsmooth_tilde_05_perform(const Sample * x, number up, number down, SampleValue * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __rampsmooth_tilde_05_increment = this->rampsmooth_tilde_05_increment;
        auto __rampsmooth_tilde_05_index = this->rampsmooth_tilde_05_index;
        auto __rampsmooth_tilde_05_prev = this->rampsmooth_tilde_05_prev;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (this->rampsmooth_tilde_05_d_next(x[(Index)i]) != 0.) {
                if (x[(Index)i] > __rampsmooth_tilde_05_prev) {
                    number _up = 480;
    
                    if (_up < 1)
                        _up = 1;
    
                    __rampsmooth_tilde_05_index = _up;
                    __rampsmooth_tilde_05_increment = (x[(Index)i] - __rampsmooth_tilde_05_prev) / _up;
                } else if (x[(Index)i] < __rampsmooth_tilde_05_prev) {
                    number _down = 480;
    
                    if (_down < 1)
                        _down = 1;
    
                    __rampsmooth_tilde_05_index = _down;
                    __rampsmooth_tilde_05_increment = (x[(Index)i] - __rampsmooth_tilde_05_prev) / _down;
                }
            }
    
            if (__rampsmooth_tilde_05_index > 0) {
                __rampsmooth_tilde_05_prev += __rampsmooth_tilde_05_increment;
                __rampsmooth_tilde_05_index -= 1;
            } else {
                __rampsmooth_tilde_05_prev = x[(Index)i];
            }
    
            out1[(Index)i] = __rampsmooth_tilde_05_prev;
        }
    
        this->rampsmooth_tilde_05_prev = __rampsmooth_tilde_05_prev;
        this->rampsmooth_tilde_05_index = __rampsmooth_tilde_05_index;
        this->rampsmooth_tilde_05_increment = __rampsmooth_tilde_05_increment;
    }
    
    void dspexpr_26_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_30_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void rand_tilde_06_perform(number frequency, SampleValue * out1, Index n) {
        auto __rand_tilde_06_lastValue = this->rand_tilde_06_lastValue;
        auto __rand_tilde_06_oldTargetValue = this->rand_tilde_06_oldTargetValue;
        auto __rand_tilde_06_targetValue = this->rand_tilde_06_targetValue;
        number f = (frequency > 0.0001 ? frequency : 0.0001);
        Index i;
    
        for (i = 0; i < n; i++) {
            number phasorValue = this->rand_tilde_06_p_next(f, -1);
    
            if (this->rand_tilde_06_d_next(phasorValue) < 0) {
                __rand_tilde_06_targetValue = this->rand_tilde_06_n_next();
                __rand_tilde_06_oldTargetValue = __rand_tilde_06_lastValue;
            }
    
            __rand_tilde_06_lastValue = __rand_tilde_06_oldTargetValue + phasorValue * (__rand_tilde_06_targetValue - __rand_tilde_06_oldTargetValue);
            out1[(Index)i] = __rand_tilde_06_lastValue;
        }
    
        this->rand_tilde_06_targetValue = __rand_tilde_06_targetValue;
        this->rand_tilde_06_oldTargetValue = __rand_tilde_06_oldTargetValue;
        this->rand_tilde_06_lastValue = __rand_tilde_06_lastValue;
    }
    
    void cycle_tilde_06_perform(
        const Sample * frequency,
        number phase_offset,
        SampleValue * out1,
        SampleValue * out2,
        Index n
    ) {
        RNBO_UNUSED(phase_offset);
        auto __cycle_tilde_06_buffer = this->cycle_tilde_06_buffer;
        Index i;
    
        for (i = 0; i < n; i++) {
            {
                uint32_t uint_phase;
    
                {
                    {
                        uint_phase = uint32_trunc(frequency[(Index)i] * 4294967296.0);
                    }
                }
    
                uint32_t idx = (uint32_t)(uint32_rshift(uint_phase, 18));
                number frac = ((BinOpInt)((BinOpInt)uint_phase & (BinOpInt)262143)) * 3.81471181759574e-6;
                number y0 = __cycle_tilde_06_buffer[(Index)idx];
                number y1 = __cycle_tilde_06_buffer[(Index)((BinOpInt)(idx + 1) & (BinOpInt)16383)];
                number y = y0 + frac * (y1 - y0);
                out1[(Index)i] = y;
                out2[(Index)i] = uint_phase * 0.232830643653869629e-9;
                continue;
            }
        }
    }
    
    void dspexpr_38_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_37_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] + 1;//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_36_perform(const Sample * in1, number in2, SampleValue * out1, Index n) {
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * 0.5;//#map:_###_obj_###_:1
        }
    }
    
    void rampsmooth_tilde_06_perform(const Sample * x, number up, number down, SampleValue * out1, Index n) {
        RNBO_UNUSED(down);
        RNBO_UNUSED(up);
        auto __rampsmooth_tilde_06_increment = this->rampsmooth_tilde_06_increment;
        auto __rampsmooth_tilde_06_index = this->rampsmooth_tilde_06_index;
        auto __rampsmooth_tilde_06_prev = this->rampsmooth_tilde_06_prev;
        Index i;
    
        for (i = 0; i < n; i++) {
            if (this->rampsmooth_tilde_06_d_next(x[(Index)i]) != 0.) {
                if (x[(Index)i] > __rampsmooth_tilde_06_prev) {
                    number _up = 480;
    
                    if (_up < 1)
                        _up = 1;
    
                    __rampsmooth_tilde_06_index = _up;
                    __rampsmooth_tilde_06_increment = (x[(Index)i] - __rampsmooth_tilde_06_prev) / _up;
                } else if (x[(Index)i] < __rampsmooth_tilde_06_prev) {
                    number _down = 480;
    
                    if (_down < 1)
                        _down = 1;
    
                    __rampsmooth_tilde_06_index = _down;
                    __rampsmooth_tilde_06_increment = (x[(Index)i] - __rampsmooth_tilde_06_prev) / _down;
                }
            }
    
            if (__rampsmooth_tilde_06_index > 0) {
                __rampsmooth_tilde_06_prev += __rampsmooth_tilde_06_increment;
                __rampsmooth_tilde_06_index -= 1;
            } else {
                __rampsmooth_tilde_06_prev = x[(Index)i];
            }
    
            out1[(Index)i] = __rampsmooth_tilde_06_prev;
        }
    
        this->rampsmooth_tilde_06_prev = __rampsmooth_tilde_06_prev;
        this->rampsmooth_tilde_06_index = __rampsmooth_tilde_06_index;
        this->rampsmooth_tilde_06_increment = __rampsmooth_tilde_06_increment;
    }
    
    void pan_tilde_02_perform(
        const Sample * pos,
        const SampleValue * in1,
        const SampleValue * in2,
        SampleValue * out1,
        SampleValue * out2,
        Index n
    ) {
        auto __pan_tilde_02_spread = this->pan_tilde_02_spread;
        Index i;
    
        for (i = 0; i < n; i++) {
            out2[(Index)i] = in1[(Index)i] * this->pan_tilde_02_func_next(pos[(Index)i] + -0.5 * __pan_tilde_02_spread, 1) + in2[(Index)i] * this->pan_tilde_02_func_next(pos[(Index)i] + 0.5 * __pan_tilde_02_spread, 1);
            out1[(Index)i] = in1[(Index)i] * this->pan_tilde_02_func_next(pos[(Index)i] + -0.5 * __pan_tilde_02_spread, 0) + in2[(Index)i] * this->pan_tilde_02_func_next(pos[(Index)i] + 0.5 * __pan_tilde_02_spread, 0);
        }
    }
    
    void signaladder_05_perform(
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
    
    void signaladder_06_perform(
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
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    number pan_tilde_02_func_next(number pos, int channel) {
        {
            {
                number nchan_1 = 2 - 1;
    
                {
                    pos = pos * nchan_1;
                }
    
                {
                    {
                        if (pos >= 0) {
                            pos = this->safemod(pos, 2);
                        } else {
                            pos = this->safemod(2 + this->safemod(pos, 2), 2);
                        }
    
                        if (channel == 0 && pos >= nchan_1)
                            pos = pos - 2;
                    }
                }
    
                pos = pos - channel;
    
                if (pos > -1 && pos < 1) {
                    {
                        {
                            return this->safesqrt(1.0 - rnbo_abs(pos));
                        }
                    }
                } else {
                    return 0;
                }
            }
        }
    }
    
    void pan_tilde_02_func_reset() {}
    
    number rampsmooth_tilde_05_d_next(number x) {
        number temp = (number)(x - this->rampsmooth_tilde_05_d_prev);
        this->rampsmooth_tilde_05_d_prev = x;
        return temp;
    }
    
    void rampsmooth_tilde_05_d_dspsetup() {
        this->rampsmooth_tilde_05_d_reset();
    }
    
    void rampsmooth_tilde_05_d_reset() {
        this->rampsmooth_tilde_05_d_prev = 0;
    }
    
    void rampsmooth_tilde_05_dspsetup(bool force) {
        if ((bool)(this->rampsmooth_tilde_05_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rampsmooth_tilde_05_setupDone = true;
        this->rampsmooth_tilde_05_d_dspsetup();
    }
    
    number rampsmooth_tilde_06_d_next(number x) {
        number temp = (number)(x - this->rampsmooth_tilde_06_d_prev);
        this->rampsmooth_tilde_06_d_prev = x;
        return temp;
    }
    
    void rampsmooth_tilde_06_d_dspsetup() {
        this->rampsmooth_tilde_06_d_reset();
    }
    
    void rampsmooth_tilde_06_d_reset() {
        this->rampsmooth_tilde_06_d_prev = 0;
    }
    
    void rampsmooth_tilde_06_dspsetup(bool force) {
        if ((bool)(this->rampsmooth_tilde_06_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rampsmooth_tilde_06_setupDone = true;
        this->rampsmooth_tilde_06_d_dspsetup();
    }
    
    number rampsmooth_tilde_07_d_next(number x) {
        number temp = (number)(x - this->rampsmooth_tilde_07_d_prev);
        this->rampsmooth_tilde_07_d_prev = x;
        return temp;
    }
    
    void rampsmooth_tilde_07_d_dspsetup() {
        this->rampsmooth_tilde_07_d_reset();
    }
    
    void rampsmooth_tilde_07_d_reset() {
        this->rampsmooth_tilde_07_d_prev = 0;
    }
    
    void rampsmooth_tilde_07_dspsetup(bool force) {
        if ((bool)(this->rampsmooth_tilde_07_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rampsmooth_tilde_07_setupDone = true;
        this->rampsmooth_tilde_07_d_dspsetup();
    }
    
    number cycle_tilde_04_ph_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->cycle_tilde_04_ph_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->cycle_tilde_04_ph_conv;
    
        if (this->cycle_tilde_04_ph_currentPhase < 0.)
            this->cycle_tilde_04_ph_currentPhase = 1. + this->cycle_tilde_04_ph_currentPhase;
    
        if (this->cycle_tilde_04_ph_currentPhase > 1.)
            this->cycle_tilde_04_ph_currentPhase = this->cycle_tilde_04_ph_currentPhase - 1.;
    
        number tmp = this->cycle_tilde_04_ph_currentPhase;
        this->cycle_tilde_04_ph_currentPhase += pincr;
        return tmp;
    }
    
    void cycle_tilde_04_ph_reset() {
        this->cycle_tilde_04_ph_currentPhase = 0;
    }
    
    void cycle_tilde_04_ph_dspsetup() {
        this->cycle_tilde_04_ph_conv = (number)1 / this->sr;
    }
    
    void cycle_tilde_04_dspsetup(bool force) {
        if ((bool)(this->cycle_tilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->cycle_tilde_04_phasei = 0;
        this->cycle_tilde_04_f2i = (number)4294967296 / this->samplerate();
        this->cycle_tilde_04_wrap = (long)(this->cycle_tilde_04_buffer->getSize()) - 1;
        this->cycle_tilde_04_setupDone = true;
        this->cycle_tilde_04_ph_dspsetup();
    }
    
    void cycle_tilde_04_bufferUpdated() {
        this->cycle_tilde_04_wrap = (long)(this->cycle_tilde_04_buffer->getSize()) - 1;
    }
    
    number rand_tilde_04_p_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->rand_tilde_04_p_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->rand_tilde_04_p_conv;
    
        if (this->rand_tilde_04_p_currentPhase < 0.)
            this->rand_tilde_04_p_currentPhase = 1. + this->rand_tilde_04_p_currentPhase;
    
        if (this->rand_tilde_04_p_currentPhase > 1.)
            this->rand_tilde_04_p_currentPhase = this->rand_tilde_04_p_currentPhase - 1.;
    
        number tmp = this->rand_tilde_04_p_currentPhase;
        this->rand_tilde_04_p_currentPhase += pincr;
        return tmp;
    }
    
    void rand_tilde_04_p_reset() {
        this->rand_tilde_04_p_currentPhase = 0;
    }
    
    void rand_tilde_04_p_dspsetup() {
        this->rand_tilde_04_p_conv = (number)1 / this->sr;
    }
    
    void rand_tilde_04_n_reset() {
        xoshiro_reset(
            systemticks() + this->voice() + this->random(0, 10000),
            this->rand_tilde_04_n_state
        );
    }
    
    void rand_tilde_04_n_init() {
        this->rand_tilde_04_n_reset();
    }
    
    void rand_tilde_04_n_seed(number v) {
        xoshiro_reset(v, this->rand_tilde_04_n_state);
    }
    
    number rand_tilde_04_n_next() {
        return xoshiro_next(this->rand_tilde_04_n_state);
    }
    
    number rand_tilde_04_d_next(number x) {
        number temp = (number)(x - this->rand_tilde_04_d_prev);
        this->rand_tilde_04_d_prev = x;
        return temp;
    }
    
    void rand_tilde_04_d_dspsetup() {
        this->rand_tilde_04_d_reset();
    }
    
    void rand_tilde_04_d_reset() {
        this->rand_tilde_04_d_prev = 0;
    }
    
    void rand_tilde_04_init() {
        this->rand_tilde_04_targetValue = this->rand_tilde_04_n_next();
    }
    
    void rand_tilde_04_dspsetup(bool force) {
        if ((bool)(this->rand_tilde_04_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rand_tilde_04_setupDone = true;
        this->rand_tilde_04_p_dspsetup();
        this->rand_tilde_04_d_dspsetup();
    }
    
    number cycle_tilde_05_ph_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->cycle_tilde_05_ph_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->cycle_tilde_05_ph_conv;
    
        if (this->cycle_tilde_05_ph_currentPhase < 0.)
            this->cycle_tilde_05_ph_currentPhase = 1. + this->cycle_tilde_05_ph_currentPhase;
    
        if (this->cycle_tilde_05_ph_currentPhase > 1.)
            this->cycle_tilde_05_ph_currentPhase = this->cycle_tilde_05_ph_currentPhase - 1.;
    
        number tmp = this->cycle_tilde_05_ph_currentPhase;
        this->cycle_tilde_05_ph_currentPhase += pincr;
        return tmp;
    }
    
    void cycle_tilde_05_ph_reset() {
        this->cycle_tilde_05_ph_currentPhase = 0;
    }
    
    void cycle_tilde_05_ph_dspsetup() {
        this->cycle_tilde_05_ph_conv = (number)1 / this->sr;
    }
    
    void cycle_tilde_05_dspsetup(bool force) {
        if ((bool)(this->cycle_tilde_05_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->cycle_tilde_05_phasei = 0;
        this->cycle_tilde_05_f2i = (number)4294967296 / this->samplerate();
        this->cycle_tilde_05_wrap = (long)(this->cycle_tilde_05_buffer->getSize()) - 1;
        this->cycle_tilde_05_setupDone = true;
        this->cycle_tilde_05_ph_dspsetup();
    }
    
    void cycle_tilde_05_bufferUpdated() {
        this->cycle_tilde_05_wrap = (long)(this->cycle_tilde_05_buffer->getSize()) - 1;
    }
    
    number rand_tilde_05_p_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->rand_tilde_05_p_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->rand_tilde_05_p_conv;
    
        if (this->rand_tilde_05_p_currentPhase < 0.)
            this->rand_tilde_05_p_currentPhase = 1. + this->rand_tilde_05_p_currentPhase;
    
        if (this->rand_tilde_05_p_currentPhase > 1.)
            this->rand_tilde_05_p_currentPhase = this->rand_tilde_05_p_currentPhase - 1.;
    
        number tmp = this->rand_tilde_05_p_currentPhase;
        this->rand_tilde_05_p_currentPhase += pincr;
        return tmp;
    }
    
    void rand_tilde_05_p_reset() {
        this->rand_tilde_05_p_currentPhase = 0;
    }
    
    void rand_tilde_05_p_dspsetup() {
        this->rand_tilde_05_p_conv = (number)1 / this->sr;
    }
    
    void rand_tilde_05_n_reset() {
        xoshiro_reset(
            systemticks() + this->voice() + this->random(0, 10000),
            this->rand_tilde_05_n_state
        );
    }
    
    void rand_tilde_05_n_init() {
        this->rand_tilde_05_n_reset();
    }
    
    void rand_tilde_05_n_seed(number v) {
        xoshiro_reset(v, this->rand_tilde_05_n_state);
    }
    
    number rand_tilde_05_n_next() {
        return xoshiro_next(this->rand_tilde_05_n_state);
    }
    
    number rand_tilde_05_d_next(number x) {
        number temp = (number)(x - this->rand_tilde_05_d_prev);
        this->rand_tilde_05_d_prev = x;
        return temp;
    }
    
    void rand_tilde_05_d_dspsetup() {
        this->rand_tilde_05_d_reset();
    }
    
    void rand_tilde_05_d_reset() {
        this->rand_tilde_05_d_prev = 0;
    }
    
    void rand_tilde_05_init() {
        this->rand_tilde_05_targetValue = this->rand_tilde_05_n_next();
    }
    
    void rand_tilde_05_dspsetup(bool force) {
        if ((bool)(this->rand_tilde_05_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rand_tilde_05_setupDone = true;
        this->rand_tilde_05_p_dspsetup();
        this->rand_tilde_05_d_dspsetup();
    }
    
    number cycle_tilde_06_ph_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->cycle_tilde_06_ph_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->cycle_tilde_06_ph_conv;
    
        if (this->cycle_tilde_06_ph_currentPhase < 0.)
            this->cycle_tilde_06_ph_currentPhase = 1. + this->cycle_tilde_06_ph_currentPhase;
    
        if (this->cycle_tilde_06_ph_currentPhase > 1.)
            this->cycle_tilde_06_ph_currentPhase = this->cycle_tilde_06_ph_currentPhase - 1.;
    
        number tmp = this->cycle_tilde_06_ph_currentPhase;
        this->cycle_tilde_06_ph_currentPhase += pincr;
        return tmp;
    }
    
    void cycle_tilde_06_ph_reset() {
        this->cycle_tilde_06_ph_currentPhase = 0;
    }
    
    void cycle_tilde_06_ph_dspsetup() {
        this->cycle_tilde_06_ph_conv = (number)1 / this->sr;
    }
    
    void cycle_tilde_06_dspsetup(bool force) {
        if ((bool)(this->cycle_tilde_06_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->cycle_tilde_06_phasei = 0;
        this->cycle_tilde_06_f2i = (number)4294967296 / this->samplerate();
        this->cycle_tilde_06_wrap = (long)(this->cycle_tilde_06_buffer->getSize()) - 1;
        this->cycle_tilde_06_setupDone = true;
        this->cycle_tilde_06_ph_dspsetup();
    }
    
    void cycle_tilde_06_bufferUpdated() {
        this->cycle_tilde_06_wrap = (long)(this->cycle_tilde_06_buffer->getSize()) - 1;
    }
    
    number rand_tilde_06_p_next(number freq, number reset) {
        {
            {
                if (reset >= 0.)
                    this->rand_tilde_06_p_currentPhase = reset;
            }
        }
    
        number pincr = freq * this->rand_tilde_06_p_conv;
    
        if (this->rand_tilde_06_p_currentPhase < 0.)
            this->rand_tilde_06_p_currentPhase = 1. + this->rand_tilde_06_p_currentPhase;
    
        if (this->rand_tilde_06_p_currentPhase > 1.)
            this->rand_tilde_06_p_currentPhase = this->rand_tilde_06_p_currentPhase - 1.;
    
        number tmp = this->rand_tilde_06_p_currentPhase;
        this->rand_tilde_06_p_currentPhase += pincr;
        return tmp;
    }
    
    void rand_tilde_06_p_reset() {
        this->rand_tilde_06_p_currentPhase = 0;
    }
    
    void rand_tilde_06_p_dspsetup() {
        this->rand_tilde_06_p_conv = (number)1 / this->sr;
    }
    
    void rand_tilde_06_n_reset() {
        xoshiro_reset(
            systemticks() + this->voice() + this->random(0, 10000),
            this->rand_tilde_06_n_state
        );
    }
    
    void rand_tilde_06_n_init() {
        this->rand_tilde_06_n_reset();
    }
    
    void rand_tilde_06_n_seed(number v) {
        xoshiro_reset(v, this->rand_tilde_06_n_state);
    }
    
    number rand_tilde_06_n_next() {
        return xoshiro_next(this->rand_tilde_06_n_state);
    }
    
    number rand_tilde_06_d_next(number x) {
        number temp = (number)(x - this->rand_tilde_06_d_prev);
        this->rand_tilde_06_d_prev = x;
        return temp;
    }
    
    void rand_tilde_06_d_dspsetup() {
        this->rand_tilde_06_d_reset();
    }
    
    void rand_tilde_06_d_reset() {
        this->rand_tilde_06_d_prev = 0;
    }
    
    void rand_tilde_06_init() {
        this->rand_tilde_06_targetValue = this->rand_tilde_06_n_next();
    }
    
    void rand_tilde_06_dspsetup(bool force) {
        if ((bool)(this->rand_tilde_06_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->rand_tilde_06_setupDone = true;
        this->rand_tilde_06_p_dspsetup();
        this->rand_tilde_06_d_dspsetup();
    }
    
    void midiouthelper_sendMidi(number v) {
        this->midiouthelper_midiout_set(v);
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
        dspexpr_26_in1 = 0;
        dspexpr_26_in2 = 0;
        pan_tilde_02_spread = 0.5;
        pan_tilde_02_pos = 0;
        dspexpr_27_in1 = 0;
        dspexpr_27_in2 = 0;
        sample_02_value = 0;
        sample_02_offset = 0;
        wrap_tilde_02_x = 0;
        wrap_tilde_02_low = 0;
        wrap_tilde_02_high = 1;
        dspexpr_28_in1 = 0;
        dspexpr_28_in2 = 0;
        dspexpr_29_in1 = 0;
        dspexpr_29_in2 = 0;
        dspexpr_30_in1 = 0;
        dspexpr_30_in2 = 0;
        dspexpr_31_in1 = 0;
        dspexpr_31_in2 = 0;
        expr_08_in1 = 0;
        expr_08_in2 = 8;
        expr_08_out1 = 0;
        expr_09_in1 = 0;
        expr_09_in2 = 1;
        expr_09_out1 = 0;
        voice_02_mute_number = 0;
        hann_tilde_02_x = 0;
        rampsmooth_tilde_05_x = 0;
        rampsmooth_tilde_05_up = 480;
        rampsmooth_tilde_05_down = 480;
        rampsmooth_tilde_06_x = 0;
        rampsmooth_tilde_06_up = 480;
        rampsmooth_tilde_06_down = 480;
        rampsmooth_tilde_07_x = 0;
        rampsmooth_tilde_07_up = 480;
        rampsmooth_tilde_07_down = 480;
        cycle_tilde_04_frequency = 0;
        cycle_tilde_04_phase_offset = 0;
        dspexpr_32_in1 = 0;
        dspexpr_32_in2 = 0.005;
        rand_tilde_04_frequency = 1;
        receive_07_output_number = 0;
        dspexpr_33_in1 = 0;
        dspexpr_33_in2 = 0.5;
        dspexpr_34_in1 = 0;
        dspexpr_34_in2 = 1;
        cycle_tilde_05_frequency = 0;
        cycle_tilde_05_phase_offset = 0;
        dspexpr_35_in1 = 0;
        dspexpr_35_in2 = 0.1;
        rand_tilde_05_frequency = 1;
        receive_08_output_number = 0;
        dspexpr_36_in1 = 0;
        dspexpr_36_in2 = 0.5;
        dspexpr_37_in1 = 0;
        dspexpr_37_in2 = 1;
        cycle_tilde_06_frequency = 0;
        cycle_tilde_06_phase_offset = 0;
        dspexpr_38_in1 = 0;
        dspexpr_38_in2 = 0.1;
        rand_tilde_06_frequency = 1;
        receive_09_output_number = 0;
        receive_10_output_number = 0;
        receive_11_output_number = 0;
        receive_12_output_number = 0;
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
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        rampsmooth_tilde_05_prev = 0;
        rampsmooth_tilde_05_index = 0;
        rampsmooth_tilde_05_increment = 0;
        rampsmooth_tilde_05_d_prev = 0;
        rampsmooth_tilde_05_setupDone = false;
        rampsmooth_tilde_06_prev = 0;
        rampsmooth_tilde_06_index = 0;
        rampsmooth_tilde_06_increment = 0;
        rampsmooth_tilde_06_d_prev = 0;
        rampsmooth_tilde_06_setupDone = false;
        rampsmooth_tilde_07_prev = 0;
        rampsmooth_tilde_07_index = 0;
        rampsmooth_tilde_07_increment = 0;
        rampsmooth_tilde_07_d_prev = 0;
        rampsmooth_tilde_07_setupDone = false;
        cycle_tilde_04_wrap = 0;
        cycle_tilde_04_ph_currentPhase = 0;
        cycle_tilde_04_ph_conv = 0;
        cycle_tilde_04_setupDone = false;
        rand_tilde_04_lastValue = 0;
        rand_tilde_04_targetValue = 0;
        rand_tilde_04_oldTargetValue = 0;
        rand_tilde_04_p_currentPhase = 0;
        rand_tilde_04_p_conv = 0;
        rand_tilde_04_d_prev = 0;
        rand_tilde_04_setupDone = false;
        cycle_tilde_05_wrap = 0;
        cycle_tilde_05_ph_currentPhase = 0;
        cycle_tilde_05_ph_conv = 0;
        cycle_tilde_05_setupDone = false;
        rand_tilde_05_lastValue = 0;
        rand_tilde_05_targetValue = 0;
        rand_tilde_05_oldTargetValue = 0;
        rand_tilde_05_p_currentPhase = 0;
        rand_tilde_05_p_conv = 0;
        rand_tilde_05_d_prev = 0;
        rand_tilde_05_setupDone = false;
        cycle_tilde_06_wrap = 0;
        cycle_tilde_06_ph_currentPhase = 0;
        cycle_tilde_06_ph_conv = 0;
        cycle_tilde_06_setupDone = false;
        rand_tilde_06_lastValue = 0;
        rand_tilde_06_targetValue = 0;
        rand_tilde_06_oldTargetValue = 0;
        rand_tilde_06_p_currentPhase = 0;
        rand_tilde_06_p_conv = 0;
        rand_tilde_06_d_prev = 0;
        rand_tilde_06_setupDone = false;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 0;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_26_in1;
        number dspexpr_26_in2;
        number pan_tilde_02_spread;
        number pan_tilde_02_pos;
        number dspexpr_27_in1;
        number dspexpr_27_in2;
        number sample_02_value;
        number sample_02_offset;
        number wrap_tilde_02_x;
        number wrap_tilde_02_low;
        number wrap_tilde_02_high;
        number dspexpr_28_in1;
        number dspexpr_28_in2;
        number dspexpr_29_in1;
        number dspexpr_29_in2;
        number dspexpr_30_in1;
        number dspexpr_30_in2;
        number dspexpr_31_in1;
        number dspexpr_31_in2;
        number expr_08_in1;
        number expr_08_in2;
        number expr_08_out1;
        number expr_09_in1;
        number expr_09_in2;
        number expr_09_out1;
        number voice_02_mute_number;
        number hann_tilde_02_x;
        number rampsmooth_tilde_05_x;
        number rampsmooth_tilde_05_up;
        number rampsmooth_tilde_05_down;
        number rampsmooth_tilde_06_x;
        number rampsmooth_tilde_06_up;
        number rampsmooth_tilde_06_down;
        number rampsmooth_tilde_07_x;
        number rampsmooth_tilde_07_up;
        number rampsmooth_tilde_07_down;
        number cycle_tilde_04_frequency;
        number cycle_tilde_04_phase_offset;
        number dspexpr_32_in1;
        number dspexpr_32_in2;
        number rand_tilde_04_frequency;
        number receive_07_output_number;
        list receive_07_output_list;
        number dspexpr_33_in1;
        number dspexpr_33_in2;
        number dspexpr_34_in1;
        number dspexpr_34_in2;
        number cycle_tilde_05_frequency;
        number cycle_tilde_05_phase_offset;
        number dspexpr_35_in1;
        number dspexpr_35_in2;
        number rand_tilde_05_frequency;
        number receive_08_output_number;
        list receive_08_output_list;
        number dspexpr_36_in1;
        number dspexpr_36_in2;
        number dspexpr_37_in1;
        number dspexpr_37_in2;
        number cycle_tilde_06_frequency;
        number cycle_tilde_06_phase_offset;
        number dspexpr_38_in1;
        number dspexpr_38_in2;
        number rand_tilde_06_frequency;
        number receive_09_output_number;
        list receive_09_output_list;
        number receive_10_output_number;
        list receive_10_output_list;
        number receive_11_output_number;
        list receive_11_output_list;
        number receive_12_output_number;
        list receive_12_output_list;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[5];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        Float64BufferRef sample_02_buffer;
        number rampsmooth_tilde_05_prev;
        number rampsmooth_tilde_05_index;
        number rampsmooth_tilde_05_increment;
        number rampsmooth_tilde_05_d_prev;
        bool rampsmooth_tilde_05_setupDone;
        number rampsmooth_tilde_06_prev;
        number rampsmooth_tilde_06_index;
        number rampsmooth_tilde_06_increment;
        number rampsmooth_tilde_06_d_prev;
        bool rampsmooth_tilde_06_setupDone;
        number rampsmooth_tilde_07_prev;
        number rampsmooth_tilde_07_index;
        number rampsmooth_tilde_07_increment;
        number rampsmooth_tilde_07_d_prev;
        bool rampsmooth_tilde_07_setupDone;
        Float64BufferRef cycle_tilde_04_buffer;
        long cycle_tilde_04_wrap;
        uint32_t cycle_tilde_04_phasei;
        SampleValue cycle_tilde_04_f2i;
        number cycle_tilde_04_ph_currentPhase;
        number cycle_tilde_04_ph_conv;
        bool cycle_tilde_04_setupDone;
        number rand_tilde_04_lastValue;
        number rand_tilde_04_targetValue;
        number rand_tilde_04_oldTargetValue;
        number rand_tilde_04_p_currentPhase;
        number rand_tilde_04_p_conv;
        UInt rand_tilde_04_n_state[4] = { };
        number rand_tilde_04_d_prev;
        bool rand_tilde_04_setupDone;
        Float64BufferRef cycle_tilde_05_buffer;
        long cycle_tilde_05_wrap;
        uint32_t cycle_tilde_05_phasei;
        SampleValue cycle_tilde_05_f2i;
        number cycle_tilde_05_ph_currentPhase;
        number cycle_tilde_05_ph_conv;
        bool cycle_tilde_05_setupDone;
        number rand_tilde_05_lastValue;
        number rand_tilde_05_targetValue;
        number rand_tilde_05_oldTargetValue;
        number rand_tilde_05_p_currentPhase;
        number rand_tilde_05_p_conv;
        UInt rand_tilde_05_n_state[4] = { };
        number rand_tilde_05_d_prev;
        bool rand_tilde_05_setupDone;
        Float64BufferRef cycle_tilde_06_buffer;
        long cycle_tilde_06_wrap;
        uint32_t cycle_tilde_06_phasei;
        SampleValue cycle_tilde_06_f2i;
        number cycle_tilde_06_ph_currentPhase;
        number cycle_tilde_06_ph_conv;
        bool cycle_tilde_06_setupDone;
        number rand_tilde_06_lastValue;
        number rand_tilde_06_targetValue;
        number rand_tilde_06_oldTargetValue;
        number rand_tilde_06_p_currentPhase;
        number rand_tilde_06_p_conv;
        UInt rand_tilde_06_n_state[4] = { };
        number rand_tilde_06_d_prev;
        bool rand_tilde_06_setupDone;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

class RNBOSubpatcher_53 : public PatcherInterfaceImpl {
    
    friend class freezer;
    
    public:
    
    RNBOSubpatcher_53()
    {
    }
    
    ~RNBOSubpatcher_53()
    {
    }
    
    virtual freezer* getPatcher() const {
        return static_cast<freezer *>(_parentPatcher);
    }
    
    freezer* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, -281953904, false);
    }
    
    SampleIndex currentsampletime() {
        return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
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
        this->linetilde_05_perform(this->signals[0], n);
        this->dspexpr_40_perform(in2, this->signals[0], out2, n);
        this->dspexpr_39_perform(in1, this->signals[0], out1, n);
        this->stackprotect_perform(n);
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
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(value);
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case -281953904:
            {
            this->linetilde_05_target_bang();
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
            if (TAG("Direct~/toggle_obj-40") == objectId)
                this->toggle_06_valin_set(payload);
    
            break;
            }
        }
    }
    
    void processListMessage(MessageTag , MessageTag , MillisecondTime , const list& ) {}
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("valout"):
            {
            return "valout";
            }
        case TAG("Direct~/toggle_obj-40"):
            {
            return "Direct~/toggle_obj-40";
            }
        case TAG("valin"):
            {
            return "valin";
            }
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
    
    void eventinlet_08_out1_bang_bang() {
        this->toggle_06_value_bang_bang();
    }
    
    void eventinlet_08_out1_list_set(const list& v) {
        {
            number converted = (v->length > 0 ? v[0] : 0);
            this->toggle_06_value_number_set(converted);
        }
    }
    
    void toggle_06_valin_set(number v) {
        this->toggle_06_value_number_set(v);
    }
    
    void linetilde_05_target_bang() {}
    
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
    
    void toggle_06_output_set(number v) {
        {
            list converted = {v};
            this->linetilde_05_segments_set(converted);
        }
    }
    
    void toggle_06_value_number_set(number v) {
        this->toggle_06_value_number_setter(v);
        v = this->toggle_06_value_number;
        this->getEngine()->sendNumMessage(TAG("valout"), TAG("Direct~/toggle_obj-40"), v, this->_currentTime);
        this->toggle_06_output_set(v);
    }
    
    void eventinlet_08_out1_number_set(number v) {
        this->toggle_06_value_number_set(v);
    }
    
    void toggle_06_value_bang_bang() {
        number val = (this->toggle_06_value_number == 1 ? 0 : 1);
        this->toggle_06_value_number_set(val);
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
                            -281953904,
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
    
    void dspexpr_40_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void dspexpr_39_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
        }
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void toggle_06_value_number_setter(number v) {
        this->toggle_06_value_number = (v != 0 ? 1 : 0);
    }
    
    void toggle_06_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->toggle_06_value_number;
    }
    
    void toggle_06_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->toggle_06_value_number_set(preset["value"]);
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
        dspexpr_39_in1 = 0;
        dspexpr_39_in2 = 0;
        dspexpr_40_in1 = 0;
        dspexpr_40_in2 = 0;
        toggle_06_value_number = 0;
        toggle_06_value_number_setter(toggle_06_value_number);
        linetilde_05_time = 10;
        linetilde_05_keepramp = 0;
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
        toggle_06_lastValue = 0;
        linetilde_05_currentValue = 1;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number dspexpr_39_in1;
        number dspexpr_39_in2;
        number dspexpr_40_in1;
        number dspexpr_40_in2;
        number toggle_06_value_number;
        list linetilde_05_segments;
        number linetilde_05_time;
        number linetilde_05_keepramp;
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
        number toggle_06_lastValue;
        list linetilde_05_activeRamps;
        number linetilde_05_currentValue;
        number stackprotect_count;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

freezer()
{
}

~freezer()
{
    delete this->p_01;
    delete this->p_02;
    for (int i = 0; i < 8; i++) {
        delete p_03[i];
    }
    delete this->p_04;
    delete this->p_05;
    for (int i = 0; i < 8; i++) {
        delete p_06[i];
    }
    delete this->p_07;
}

freezer* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
    getEngine()->flushClockEvents(this, 2098551528, false);
    getEngine()->flushClockEvents(this, -1841075377, false);
    getEngine()->flushClockEvents(this, 373954921, false);
    getEngine()->flushClockEvents(this, -327874582, false);
    getEngine()->flushClockEvents(this, -281953904, false);
    getEngine()->flushClockEvents(this, 1075784424, false);
    getEngine()->flushClockEvents(this, 1777613927, false);
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

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
}

number samplerate() {
    return this->sr;
}

ParameterValue fromnormalized(ParameterIndex index, ParameterValue normalizedValue) {
    return this->convertFromNormalizedParameterValue(index, normalizedValue);
}

SampleIndex currentsampletime() {
    return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
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

Index vectorsize() {
    return this->vs;
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
    this->ctlin_07_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_08_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_09_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_10_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_11_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    this->ctlin_12_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
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
    this->p_02_perform(this->signals[0], n);
    this->numbertilde_01_perform(this->signals[0], this->dummyBuffer, n);
    this->phasor_01_perform(this->signals[0], this->signals[1], n);
    this->p_03_perform(this->signals[1], this->signals[0], this->signals[2], n);
    this->p_06_perform(this->signals[1], this->signals[3], this->signals[4], n);
    this->p_01_perform(in1, in2, n);

    this->recordtilde_01_perform(
        this->recordtilde_01_record,
        this->recordtilde_01_begin,
        this->recordtilde_01_end,
        in1,
        in2,
        this->dummyBuffer,
        n
    );

    this->recordtilde_02_perform(
        this->recordtilde_02_record,
        this->recordtilde_02_begin,
        this->recordtilde_02_end,
        in1,
        in2,
        this->dummyBuffer,
        n
    );

    this->p_07_perform(in1, in2, this->signals[1], this->signals[5], n);
    this->p_05_perform(n);
    this->linetilde_06_perform(this->signals[6], n);
    this->hann_tilde_03_perform(this->signals[6], this->signals[7], n);
    this->dspexpr_43_perform(this->signals[2], this->signals[7], this->signals[6], n);
    this->metertilde_02_perform(this->signals[6], n);
    this->dspexpr_41_perform(this->signals[0], this->signals[7], this->signals[2], n);
    this->metertilde_01_perform(this->signals[2], n);
    this->linetilde_07_perform(this->signals[7], n);
    this->hann_tilde_04_perform(this->signals[7], this->signals[0], n);
    this->dspexpr_45_perform(this->signals[3], this->signals[0], this->signals[7], n);
    this->metertilde_03_perform(this->signals[7], n);
    this->dspexpr_42_perform(this->signals[2], this->signals[7], this->signals[3], n);
    this->dspexpr_46_perform(this->signals[4], this->signals[0], this->signals[7], n);
    this->metertilde_04_perform(this->signals[7], n);
    this->dspexpr_44_perform(this->signals[6], this->signals[7], this->signals[0], n);
    this->p_04_perform(this->signals[3], this->signals[0], this->signals[7], this->signals[6], n);
    this->signaladder_01_perform(this->signals[7], this->signals[1], out1, n);
    this->signaladder_02_perform(this->signals[6], this->signals[5], out2, n);
    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        Index i;

        for (i = 0; i < 8; i++) {
            this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
        }

        this->phasor_01_sigbuf = resizeSignal(this->phasor_01_sigbuf, this->maxvs, maxBlockSize);
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

    this->numbertilde_01_dspsetup(forceDSPSetup);
    this->phasor_01_dspsetup(forceDSPSetup);
    this->metertilde_02_dspsetup(forceDSPSetup);
    this->metertilde_01_dspsetup(forceDSPSetup);
    this->metertilde_03_dspsetup(forceDSPSetup);
    this->metertilde_04_dspsetup(forceDSPSetup);
    this->data_01_dspsetup(forceDSPSetup);
    this->data_02_dspsetup(forceDSPSetup);
    this->globaltransport_dspsetup(forceDSPSetup);
    this->p_01->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_02->prepareToProcess(sampleRate, maxBlockSize, force);

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->prepareToProcess(sampleRate, maxBlockSize, force);
    }

    this->p_04->prepareToProcess(sampleRate, maxBlockSize, force);
    this->p_05->prepareToProcess(sampleRate, maxBlockSize, force);

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->prepareToProcess(sampleRate, maxBlockSize, force);
    }

    this->p_07->prepareToProcess(sampleRate, maxBlockSize, force);

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
        return addressOf(this->FreezeA);
        break;
        }
    case 1:
        {
        return addressOf(this->RNBODefaultSinus);
        break;
        }
    case 2:
        {
        return addressOf(this->FreezeB);
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

void fillRNBODefaultSinus(DataRef& ref) {
    Float64BufferRef buffer;
    buffer = new Float64Buffer(ref);
    number bufsize = buffer->getSize();

    for (Index i = 0; i < bufsize; i++) {
        buffer[i] = rnbo_cos(i * 3.14159265358979323846 * 2. / bufsize);
    }
}

void fillDataRef(DataRefIndex index, DataRef& ref) {
    switch (index) {
    case 1:
        {
        this->fillRNBODefaultSinus(ref);
        break;
        }
    }
}

void zeroDataRef(DataRef& ref) {
    ref->setZero();
}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    this->updateTime(time);

    if (index == 0) {
        this->recordtilde_01_buffer = new Float64Buffer(this->FreezeA);
        this->data_02_buffer = new Float64Buffer(this->FreezeA);
        this->data_02_bufferUpdated();
    }

    if (index == 2) {
        this->data_01_buffer = new Float64Buffer(this->FreezeB);
        this->data_01_bufferUpdated();
        this->recordtilde_02_buffer = new Float64Buffer(this->FreezeB);
    }

    this->p_01->processDataViewUpdate(index, time);
    this->p_02->processDataViewUpdate(index, time);

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->processDataViewUpdate(index, time);
    }

    this->p_04->processDataViewUpdate(index, time);
    this->p_05->processDataViewUpdate(index, time);

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->processDataViewUpdate(index, time);
    }

    this->p_07->processDataViewUpdate(index, time);
}

void initialize() {
    this->FreezeA = initDataRef("FreezeA", false, nullptr, "data");
    this->RNBODefaultSinus = initDataRef("RNBODefaultSinus", true, nullptr, "buffer~");
    this->FreezeB = initDataRef("FreezeB", false, nullptr, "data");
    this->assign_defaults();
    this->setState();
    this->FreezeA->setIndex(0);
    this->recordtilde_01_buffer = new Float64Buffer(this->FreezeA);
    this->data_02_buffer = new Float64Buffer(this->FreezeA);
    this->RNBODefaultSinus->setIndex(1);
    this->FreezeB->setIndex(2);
    this->data_01_buffer = new Float64Buffer(this->FreezeB);
    this->recordtilde_02_buffer = new Float64Buffer(this->FreezeB);
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
    this->p_01 = new RNBOSubpatcher_47();
    this->p_01->setEngineAndPatcher(this->getEngine(), this);
    this->p_01->initialize();
    this->p_01->setParameterOffset(this->getParameterOffset(this->p_01));
    this->p_02 = new RNBOSubpatcher_48();
    this->p_02->setEngineAndPatcher(this->getEngine(), this);
    this->p_02->initialize();
    this->p_02->setParameterOffset(this->getParameterOffset(this->p_02));

    for (Index i = 0; i < 8; i++) {
        this->p_03[(Index)i] = new RNBOSubpatcher_49();
        this->p_03[(Index)i]->setEngineAndPatcher(this->getEngine(), this);
        this->p_03[(Index)i]->initialize();
        this->p_03[(Index)i]->setParameterOffset(this->getParameterOffset(this->p_03[0]));
        this->p_03[(Index)i]->setVoiceIndex(i + 1);
    }

    this->p_04 = new RNBOSubpatcher_50();
    this->p_04->setEngineAndPatcher(this->getEngine(), this);
    this->p_04->initialize();
    this->p_04->setParameterOffset(this->getParameterOffset(this->p_04));
    this->p_05 = new RNBOSubpatcher_51();
    this->p_05->setEngineAndPatcher(this->getEngine(), this);
    this->p_05->initialize();
    this->p_05->setParameterOffset(this->getParameterOffset(this->p_05));

    for (Index i = 0; i < 8; i++) {
        this->p_06[(Index)i] = new RNBOSubpatcher_52();
        this->p_06[(Index)i]->setEngineAndPatcher(this->getEngine(), this);
        this->p_06[(Index)i]->initialize();
        this->p_06[(Index)i]->setParameterOffset(this->getParameterOffset(this->p_06[0]));
        this->p_06[(Index)i]->setVoiceIndex(i + 1);
    }

    this->p_07 = new RNBOSubpatcher_53();
    this->p_07->setEngineAndPatcher(this->getEngine(), this);
    this->p_07->initialize();
    this->p_07->setParameterOffset(this->getParameterOffset(this->p_07));
}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_01_getPresetValue(getSubState(preset, "transp"));
    this->param_02_getPresetValue(getSubState(preset, "auto"));
    this->param_03_getPresetValue(getSubState(preset, "autodel"));
    this->param_04_getPresetValue(getSubState(preset, "freeze"));
    this->param_05_getPresetValue(getSubState(preset, "lowcut"));
    this->param_06_getPresetValue(getSubState(preset, "highcut"));
    this->param_07_getPresetValue(getSubState(preset, "volume"));
    this->param_08_getPresetValue(getSubState(preset, "fade"));
    this->param_09_getPresetValue(getSubState(preset, "pitchjit"));
    this->param_10_getPresetValue(getSubState(preset, "panjit"));
    this->param_11_getPresetValue(getSubState(preset, "ampjit"));
    this->param_12_getPresetValue(getSubState(preset, "direct"));
    this->p_01->getPreset(getSubState(getSubState(preset, "__sps"), "Attack~"));
    this->p_02->getPreset(getSubState(getSubState(preset, "__sps"), "Transp~"));

    for (Index i = 0; i < 8; i++)
        this->p_03[i]->getPreset(getSubStateAt(getSubState(preset, "__sps"), "Freezer-Voice~", i));

    this->p_04->getPreset(getSubState(getSubState(preset, "__sps"), "EQ&Volume~"));
    this->p_05->getPreset(getSubState(getSubState(preset, "__sps"), "Freeze"));

    for (Index i = 0; i < 8; i++)
        this->p_06[i]->getPreset(getSubStateAt(getSubState(preset, "__sps"), "Freezer-Voice~[1]", i));

    this->p_07->getPreset(getSubState(getSubState(preset, "__sps"), "Direct~"));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_04_setPresetValue(getSubState(preset, "freeze"));
    this->param_02_setPresetValue(getSubState(preset, "auto"));
    this->param_03_setPresetValue(getSubState(preset, "autodel"));
    this->param_01_setPresetValue(getSubState(preset, "transp"));
    this->param_08_setPresetValue(getSubState(preset, "fade"));
    this->param_09_setPresetValue(getSubState(preset, "pitchjit"));
    this->param_11_setPresetValue(getSubState(preset, "ampjit"));
    this->param_10_setPresetValue(getSubState(preset, "panjit"));
    this->param_05_setPresetValue(getSubState(preset, "lowcut"));
    this->param_06_setPresetValue(getSubState(preset, "highcut"));
    this->param_12_setPresetValue(getSubState(preset, "direct"));
    this->param_07_setPresetValue(getSubState(preset, "volume"));
}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(this->_currentTime, tempo, false)) {
        this->p_01->processTempoEvent(time, tempo);
        this->p_02->processTempoEvent(time, tempo);

        for (Index i = 0; i < 8; i++) {
            this->p_03[i]->processTempoEvent(time, tempo);
        }

        this->p_04->processTempoEvent(time, tempo);
        this->p_05->processTempoEvent(time, tempo);

        for (Index i = 0; i < 8; i++) {
            this->p_06[i]->processTempoEvent(time, tempo);
        }

        this->p_07->processTempoEvent(time, tempo);
    }
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(this->_currentTime, state, false)) {
        this->p_01->processTransportEvent(time, state);
        this->p_02->processTransportEvent(time, state);

        for (Index i = 0; i < 8; i++) {
            this->p_03[i]->processTransportEvent(time, state);
        }

        this->p_04->processTransportEvent(time, state);
        this->p_05->processTransportEvent(time, state);

        for (Index i = 0; i < 8; i++) {
            this->p_06[i]->processTransportEvent(time, state);
        }

        this->p_07->processTransportEvent(time, state);
    }
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(this->_currentTime, beattime, false)) {
        this->p_01->processBeatTimeEvent(time, beattime);
        this->p_02->processBeatTimeEvent(time, beattime);

        for (Index i = 0; i < 8; i++) {
            this->p_03[i]->processBeatTimeEvent(time, beattime);
        }

        this->p_04->processBeatTimeEvent(time, beattime);
        this->p_05->processBeatTimeEvent(time, beattime);

        for (Index i = 0; i < 8; i++) {
            this->p_06[i]->processBeatTimeEvent(time, beattime);
        }

        this->p_07->processBeatTimeEvent(time, beattime);
    }
}

void onSampleRateChanged(double ) {}

void processTimeSignatureEvent(MillisecondTime time, int numerator, int denominator) {
    this->updateTime(time);

    if (this->globaltransport_setTimeSignature(this->_currentTime, numerator, denominator, false)) {
        this->p_01->processTimeSignatureEvent(time, numerator, denominator);
        this->p_02->processTimeSignatureEvent(time, numerator, denominator);

        for (Index i = 0; i < 8; i++) {
            this->p_03[i]->processTimeSignatureEvent(time, numerator, denominator);
        }

        this->p_04->processTimeSignatureEvent(time, numerator, denominator);
        this->p_05->processTimeSignatureEvent(time, numerator, denominator);

        for (Index i = 0; i < 8; i++) {
            this->p_06[i]->processTimeSignatureEvent(time, numerator, denominator);
        }

        this->p_07->processTimeSignatureEvent(time, numerator, denominator);
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
    case 6:
        {
        this->param_07_value_set(v);
        break;
        }
    case 7:
        {
        this->param_08_value_set(v);
        break;
        }
    case 8:
        {
        this->param_09_value_set(v);
        break;
        }
    case 9:
        {
        this->param_10_value_set(v);
        break;
        }
    case 10:
        {
        this->param_11_value_set(v);
        break;
        }
    case 11:
        {
        this->param_12_value_set(v);
        break;
        }
    default:
        {
        index -= 12;

        if (index < this->p_01->getNumParameters())
            this->p_01->setParameterValue(index, v, time);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            this->p_02->setParameterValue(index, v, time);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03[0]->getNumParameters())
            this->p_03[0]->setPolyParameterValue((PatcherInterface**)this->p_03, index, v, time);

        index -= this->p_03[0]->getNumParameters();

        if (index < this->p_04->getNumParameters())
            this->p_04->setParameterValue(index, v, time);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            this->p_05->setParameterValue(index, v, time);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06[0]->getNumParameters())
            this->p_06[0]->setPolyParameterValue((PatcherInterface**)this->p_06, index, v, time);

        index -= this->p_06[0]->getNumParameters();

        if (index < this->p_07->getNumParameters())
            this->p_07->setParameterValue(index, v, time);

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
    case 7:
        {
        return this->param_08_value;
        }
    case 8:
        {
        return this->param_09_value;
        }
    case 9:
        {
        return this->param_10_value;
        }
    case 10:
        {
        return this->param_11_value;
        }
    case 11:
        {
        return this->param_12_value;
        }
    default:
        {
        index -= 12;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterValue(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterValue(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03[0]->getNumParameters())
            return this->p_03[0]->getPolyParameterValue((PatcherInterface**)this->p_03, index);

        index -= this->p_03[0]->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterValue(index);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->getParameterValue(index);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06[0]->getNumParameters())
            return this->p_06[0]->getPolyParameterValue((PatcherInterface**)this->p_06, index);

        index -= this->p_06[0]->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->getParameterValue(index);

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
    return 12 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03[0]->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06[0]->getNumParameters() + this->p_07->getNumParameters();
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "transp";
        }
    case 1:
        {
        return "auto";
        }
    case 2:
        {
        return "autodel";
        }
    case 3:
        {
        return "freeze";
        }
    case 4:
        {
        return "lowcut";
        }
    case 5:
        {
        return "highcut";
        }
    case 6:
        {
        return "volume";
        }
    case 7:
        {
        return "fade";
        }
    case 8:
        {
        return "pitchjit";
        }
    case 9:
        {
        return "panjit";
        }
    case 10:
        {
        return "ampjit";
        }
    case 11:
        {
        return "direct";
        }
    default:
        {
        index -= 12;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterName(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterName(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03[0]->getNumParameters()) {
            {
                return this->p_03[0]->getParameterName(index);
            }
        }

        index -= this->p_03[0]->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterName(index);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->getParameterName(index);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06[0]->getNumParameters()) {
            {
                return this->p_06[0]->getParameterName(index);
            }
        }

        index -= this->p_06[0]->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->getParameterName(index);

        return "bogus";
        }
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "transp";
        }
    case 1:
        {
        return "auto";
        }
    case 2:
        {
        return "autodel";
        }
    case 3:
        {
        return "freeze";
        }
    case 4:
        {
        return "lowcut";
        }
    case 5:
        {
        return "highcut";
        }
    case 6:
        {
        return "volume";
        }
    case 7:
        {
        return "fade";
        }
    case 8:
        {
        return "pitchjit";
        }
    case 9:
        {
        return "panjit";
        }
    case 10:
        {
        return "ampjit";
        }
    case 11:
        {
        return "direct";
        }
    default:
        {
        index -= 12;

        if (index < this->p_01->getNumParameters())
            return this->p_01->getParameterId(index);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->getParameterId(index);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03[0]->getNumParameters()) {
            {
                return this->p_03[0]->getParameterId(index);
            }
        }

        index -= this->p_03[0]->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->getParameterId(index);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->getParameterId(index);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06[0]->getNumParameters()) {
            {
                return this->p_06[0]->getParameterId(index);
            }
        }

        index -= this->p_06[0]->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->getParameterId(index);

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
            info->initialValue = 0;
            info->min = -24;
            info->max = 24;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "Transp";
            info->unit = "st";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 1:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "Auto";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 2:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "AutoDel";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 3:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 2;
            static const char * eVal3[] = {"0", "1"};
            info->enumValues = eVal3;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "Frezze";
            info->unit = "On/Off";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 4:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "LowCut";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 5:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "HighCut";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 6:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = -100;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "Volume";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 7:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 25;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "Fade";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 8:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 25;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "PitchJitter";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 9:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 25;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "PanJitter";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 10:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 25;
            info->min = 0;
            info->max = 100;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "AmpJitter";
            info->unit = "%";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 11:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 1;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 2;
            static const char * eVal11[] = {"0", "1"};
            info->enumValues = eVal11;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "Direct";
            info->unit = "On/Off";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        default:
            {
            index -= 12;

            if (index < this->p_01->getNumParameters())
                this->p_01->getParameterInfo(index, info);

            index -= this->p_01->getNumParameters();

            if (index < this->p_02->getNumParameters())
                this->p_02->getParameterInfo(index, info);

            index -= this->p_02->getNumParameters();

            if (index < this->p_03[0]->getNumParameters()) {
                for (Index i = 0; i < 8; i++) {
                    this->p_03[i]->getParameterInfo(index, info);
                }
            }

            index -= this->p_03[0]->getNumParameters();

            if (index < this->p_04->getNumParameters())
                this->p_04->getParameterInfo(index, info);

            index -= this->p_04->getNumParameters();

            if (index < this->p_05->getNumParameters())
                this->p_05->getParameterInfo(index, info);

            index -= this->p_05->getNumParameters();

            if (index < this->p_06[0]->getNumParameters()) {
                for (Index i = 0; i < 8; i++) {
                    this->p_06[i]->getParameterInfo(index, info);
                }
            }

            index -= this->p_06[0]->getNumParameters();

            if (index < this->p_07->getNumParameters())
                this->p_07->getParameterInfo(index, info);

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
        return 12;

    if (subpatcher == this->p_02)
        return 12 + this->p_01->getNumParameters();

    if (subpatcher == this->p_03[0])
        return 12 + this->p_01->getNumParameters() + this->p_02->getNumParameters();

    if (subpatcher == this->p_04)
        return 12 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03[0]->getNumParameters();

    if (subpatcher == this->p_05)
        return 12 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03[0]->getNumParameters() + this->p_04->getNumParameters();

    if (subpatcher == this->p_06[0])
        return 12 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03[0]->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters();

    if (subpatcher == this->p_07)
        return 12 + this->p_01->getNumParameters() + this->p_02->getNumParameters() + this->p_03[0]->getNumParameters() + this->p_04->getNumParameters() + this->p_05->getNumParameters() + this->p_06[0]->getNumParameters();

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
    case 3:
    case 11:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);

            {
                normalizedValue = this->applyStepsToNormalizedParameterValue(normalizedValue, 2);
            }

            return normalizedValue;
        }
        }
    case 1:
    case 2:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
    case 10:
        {
        {
            value = (value < 0 ? 0 : (value > 100 ? 100 : value));
            ParameterValue normalizedValue = (value - 0) / (100 - 0);
            return normalizedValue;
        }
        }
    case 0:
        {
        {
            value = (value < -24 ? -24 : (value > 24 ? 24 : value));
            ParameterValue normalizedValue = (value - -24) / (24 - -24);
            return normalizedValue;
        }
        }
    case 6:
        {
        {
            value = (value < -100 ? -100 : (value > 100 ? 100 : value));
            ParameterValue normalizedValue = (value - -100) / (100 - -100);
            return normalizedValue;
        }
        }
    default:
        {
        index -= 12;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertToNormalizedParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->convertToNormalizedParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03[0]->getNumParameters()) {
            {
                return this->p_03[0]->convertToNormalizedParameterValue(index, value);
            }
        }

        index -= this->p_03[0]->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->convertToNormalizedParameterValue(index, value);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->convertToNormalizedParameterValue(index, value);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06[0]->getNumParameters()) {
            {
                return this->p_06[0]->convertToNormalizedParameterValue(index, value);
            }
        }

        index -= this->p_06[0]->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->convertToNormalizedParameterValue(index, value);

        return value;
        }
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 3:
    case 11:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                value = this->applyStepsToNormalizedParameterValue(value, 2);
            }

            {
                return 0 + value * (1 - 0);
            }
        }
        }
    case 1:
    case 2:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
    case 10:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (100 - 0);
            }
        }
        }
    case 0:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return -24 + value * (24 - -24);
            }
        }
        }
    case 6:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return -100 + value * (100 - -100);
            }
        }
        }
    default:
        {
        index -= 12;

        if (index < this->p_01->getNumParameters())
            return this->p_01->convertFromNormalizedParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->convertFromNormalizedParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03[0]->getNumParameters()) {
            {
                return this->p_03[0]->convertFromNormalizedParameterValue(index, value);
            }
        }

        index -= this->p_03[0]->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->convertFromNormalizedParameterValue(index, value);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->convertFromNormalizedParameterValue(index, value);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06[0]->getNumParameters()) {
            {
                return this->p_06[0]->convertFromNormalizedParameterValue(index, value);
            }
        }

        index -= this->p_06[0]->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->convertFromNormalizedParameterValue(index, value);

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
    case 7:
        {
        return this->param_08_value_constrain(value);
        }
    case 8:
        {
        return this->param_09_value_constrain(value);
        }
    case 9:
        {
        return this->param_10_value_constrain(value);
        }
    case 10:
        {
        return this->param_11_value_constrain(value);
        }
    case 11:
        {
        return this->param_12_value_constrain(value);
        }
    default:
        {
        index -= 12;

        if (index < this->p_01->getNumParameters())
            return this->p_01->constrainParameterValue(index, value);

        index -= this->p_01->getNumParameters();

        if (index < this->p_02->getNumParameters())
            return this->p_02->constrainParameterValue(index, value);

        index -= this->p_02->getNumParameters();

        if (index < this->p_03[0]->getNumParameters()) {
            {
                return this->p_03[0]->constrainParameterValue(index, value);
            }
        }

        index -= this->p_03[0]->getNumParameters();

        if (index < this->p_04->getNumParameters())
            return this->p_04->constrainParameterValue(index, value);

        index -= this->p_04->getNumParameters();

        if (index < this->p_05->getNumParameters())
            return this->p_05->constrainParameterValue(index, value);

        index -= this->p_05->getNumParameters();

        if (index < this->p_06[0]->getNumParameters()) {
            {
                return this->p_06[0]->constrainParameterValue(index, value);
            }
        }

        index -= this->p_06[0]->getNumParameters();

        if (index < this->p_07->getNumParameters())
            return this->p_07->constrainParameterValue(index, value);

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
    RNBO_UNUSED(hasValue);
    this->updateTime(time);

    switch (index) {
    case 2098551528:
        {
        this->numbertilde_01_value_set(value);
        break;
        }
    case -1841075377:
        {
        this->linetilde_06_target_bang();
        break;
        }
    case 373954921:
        {
        this->metertilde_02_value_set(value);
        break;
        }
    case -327874582:
        {
        this->metertilde_01_value_set(value);
        break;
        }
    case -281953904:
        {
        this->linetilde_07_target_bang();
        break;
        }
    case 1075784424:
        {
        this->metertilde_03_value_set(value);
        break;
        }
    case 1777613927:
        {
        this->metertilde_04_value_set(value);
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
    case TAG("listin"):
        {
        if (TAG("message_obj-107") == objectId)
            this->message_11_listin_number_set(payload);

        break;
        }
    case TAG("valin"):
        {
        if (TAG("number_obj-135") == objectId)
            this->numberobj_20_valin_set(payload);

        if (TAG("number_obj-110") == objectId)
            this->numberobj_21_valin_set(payload);

        if (TAG("number_obj-122") == objectId)
            this->numberobj_22_valin_set(payload);

        if (TAG("toggle_obj-11") == objectId)
            this->toggle_07_valin_set(payload);

        if (TAG("number_obj-103") == objectId)
            this->numberobj_23_valin_set(payload);

        if (TAG("toggle_obj-101") == objectId)
            this->toggle_08_valin_set(payload);

        if (TAG("number_obj-50") == objectId)
            this->numberobj_24_valin_set(payload);

        if (TAG("number_obj-39") == objectId)
            this->numberobj_25_valin_set(payload);

        if (TAG("number_obj-105") == objectId)
            this->numberobj_26_valin_set(payload);

        if (TAG("number_obj-57") == objectId)
            this->numberobj_27_valin_set(payload);

        if (TAG("number_obj-153") == objectId)
            this->numberobj_28_valin_set(payload);

        if (TAG("number_obj-123") == objectId)
            this->numberobj_29_valin_set(payload);

        if (TAG("toggle_obj-104") == objectId)
            this->toggle_09_valin_set(payload);

        if (TAG("number_obj-182") == objectId)
            this->numberobj_30_valin_set(payload);

        if (TAG("number_obj-184") == objectId)
            this->numberobj_31_valin_set(payload);

        if (TAG("number_obj-174") == objectId)
            this->numberobj_32_valin_set(payload);

        if (TAG("number_obj-176") == objectId)
            this->numberobj_33_valin_set(payload);

        if (TAG("number_obj-161") == objectId)
            this->numberobj_34_valin_set(payload);

        if (TAG("number_obj-163") == objectId)
            this->numberobj_35_valin_set(payload);

        if (TAG("toggle_obj-79") == objectId)
            this->toggle_10_valin_set(payload);

        break;
        }
    case TAG("format"):
        {
        if (TAG("number_obj-135") == objectId)
            this->numberobj_20_format_set(payload);

        if (TAG("number_obj-110") == objectId)
            this->numberobj_21_format_set(payload);

        if (TAG("number_obj-122") == objectId)
            this->numberobj_22_format_set(payload);

        if (TAG("number_obj-103") == objectId)
            this->numberobj_23_format_set(payload);

        if (TAG("number_obj-50") == objectId)
            this->numberobj_24_format_set(payload);

        if (TAG("number_obj-39") == objectId)
            this->numberobj_25_format_set(payload);

        if (TAG("number_obj-105") == objectId)
            this->numberobj_26_format_set(payload);

        if (TAG("number_obj-57") == objectId)
            this->numberobj_27_format_set(payload);

        if (TAG("number_obj-153") == objectId)
            this->numberobj_28_format_set(payload);

        if (TAG("number_obj-123") == objectId)
            this->numberobj_29_format_set(payload);

        if (TAG("number_obj-182") == objectId)
            this->numberobj_30_format_set(payload);

        if (TAG("number_obj-184") == objectId)
            this->numberobj_31_format_set(payload);

        if (TAG("number_obj-174") == objectId)
            this->numberobj_32_format_set(payload);

        if (TAG("number_obj-176") == objectId)
            this->numberobj_33_format_set(payload);

        if (TAG("number_obj-161") == objectId)
            this->numberobj_34_format_set(payload);

        if (TAG("number_obj-163") == objectId)
            this->numberobj_35_format_set(payload);

        break;
        }
    case TAG("sig"):
        {
        if (TAG("number~_obj-74") == objectId)
            this->numbertilde_01_sig_number_set(payload);

        break;
        }
    case TAG("mode"):
        {
        if (TAG("number~_obj-74") == objectId)
            this->numbertilde_01_mode_set(payload);

        break;
        }
    }

    this->p_01->processNumMessage(tag, objectId, time, payload);
    this->p_02->processNumMessage(tag, objectId, time, payload);

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->processNumMessage(tag, objectId, time, payload);
    }

    this->p_04->processNumMessage(tag, objectId, time, payload);
    this->p_05->processNumMessage(tag, objectId, time, payload);

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->processNumMessage(tag, objectId, time, payload);
    }

    this->p_07->processNumMessage(tag, objectId, time, payload);
}

void processListMessage(
    MessageTag tag,
    MessageTag objectId,
    MillisecondTime time,
    const list& payload
) {
    this->updateTime(time);

    switch (tag) {
    case TAG("listin"):
        {
        if (TAG("message_obj-107") == objectId)
            this->message_11_listin_list_set(payload);

        break;
        }
    case TAG("sig"):
        {
        if (TAG("number~_obj-74") == objectId)
            this->numbertilde_01_sig_list_set(payload);

        break;
        }
    }

    this->p_01->processListMessage(tag, objectId, time, payload);
    this->p_02->processListMessage(tag, objectId, time, payload);

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->processListMessage(tag, objectId, time, payload);
    }

    this->p_04->processListMessage(tag, objectId, time, payload);
    this->p_05->processListMessage(tag, objectId, time, payload);

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->processListMessage(tag, objectId, time, payload);
    }

    this->p_07->processListMessage(tag, objectId, time, payload);
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    this->updateTime(time);

    switch (tag) {
    case TAG("listin"):
        {
        if (TAG("message_obj-107") == objectId)
            this->message_11_listin_bang_bang();

        break;
        }
    case TAG("bangin"):
        {
        if (TAG("button_obj-102") == objectId)
            this->button_09_bangin_bang();

        break;
        }
    }

    this->p_01->processBangMessage(tag, objectId, time);
    this->p_02->processBangMessage(tag, objectId, time);

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->processBangMessage(tag, objectId, time);
    }

    this->p_04->processBangMessage(tag, objectId, time);
    this->p_05->processBangMessage(tag, objectId, time);

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->processBangMessage(tag, objectId, time);
    }

    this->p_07->processBangMessage(tag, objectId, time);
}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {
    case TAG("listout"):
        {
        return "listout";
        }
    case TAG("message_obj-107"):
        {
        return "message_obj-107";
        }
    case TAG("bangout"):
        {
        return "bangout";
        }
    case TAG("button_obj-102"):
        {
        return "button_obj-102";
        }
    case TAG("valout"):
        {
        return "valout";
        }
    case TAG("number_obj-135"):
        {
        return "number_obj-135";
        }
    case TAG("setup"):
        {
        return "setup";
        }
    case TAG("monitor"):
        {
        return "monitor";
        }
    case TAG("number~_obj-74"):
        {
        return "number~_obj-74";
        }
    case TAG("assign"):
        {
        return "assign";
        }
    case TAG("number_obj-110"):
        {
        return "number_obj-110";
        }
    case TAG("meter~_obj-96"):
        {
        return "meter~_obj-96";
        }
    case TAG("peak"):
        {
        return "peak";
        }
    case TAG("meter~_obj-97"):
        {
        return "meter~_obj-97";
        }
    case TAG("number_obj-122"):
        {
        return "number_obj-122";
        }
    case TAG("toggle_obj-11"):
        {
        return "toggle_obj-11";
        }
    case TAG("number_obj-103"):
        {
        return "number_obj-103";
        }
    case TAG("toggle_obj-101"):
        {
        return "toggle_obj-101";
        }
    case TAG("number_obj-50"):
        {
        return "number_obj-50";
        }
    case TAG("meter~_obj-99"):
        {
        return "meter~_obj-99";
        }
    case TAG("number_obj-39"):
        {
        return "number_obj-39";
        }
    case TAG("meter~_obj-98"):
        {
        return "meter~_obj-98";
        }
    case TAG("number_obj-105"):
        {
        return "number_obj-105";
        }
    case TAG("number_obj-57"):
        {
        return "number_obj-57";
        }
    case TAG("number_obj-153"):
        {
        return "number_obj-153";
        }
    case TAG("number_obj-123"):
        {
        return "number_obj-123";
        }
    case TAG("toggle_obj-104"):
        {
        return "toggle_obj-104";
        }
    case TAG("number_obj-182"):
        {
        return "number_obj-182";
        }
    case TAG("number_obj-184"):
        {
        return "number_obj-184";
        }
    case TAG("number_obj-174"):
        {
        return "number_obj-174";
        }
    case TAG("number_obj-176"):
        {
        return "number_obj-176";
        }
    case TAG("number_obj-161"):
        {
        return "number_obj-161";
        }
    case TAG("number_obj-163"):
        {
        return "number_obj-163";
        }
    case TAG("toggle_obj-79"):
        {
        return "toggle_obj-79";
        }
    case TAG("listin"):
        {
        return "listin";
        }
    case TAG("bangin"):
        {
        return "bangin";
        }
    case TAG("valin"):
        {
        return "valin";
        }
    case TAG("format"):
        {
        return "format";
        }
    case TAG("sig"):
        {
        return "sig";
        }
    case TAG("mode"):
        {
        return "mode";
        }
    }

    auto subpatchResult_0 = this->p_01->resolveTag(tag);

    if (subpatchResult_0)
        return subpatchResult_0;

    auto subpatchResult_1 = this->p_02->resolveTag(tag);

    if (subpatchResult_1)
        return subpatchResult_1;

    auto subpatchResult_2 = this->p_03[0]->resolveTag(tag);

    if (subpatchResult_2)
        return subpatchResult_2;

    auto subpatchResult_3 = this->p_04->resolveTag(tag);

    if (subpatchResult_3)
        return subpatchResult_3;

    auto subpatchResult_4 = this->p_05->resolveTag(tag);

    if (subpatchResult_4)
        return subpatchResult_4;

    auto subpatchResult_5 = this->p_06[0]->resolveTag(tag);

    if (subpatchResult_5)
        return subpatchResult_5;

    auto subpatchResult_6 = this->p_07->resolveTag(tag);

    if (subpatchResult_6)
        return subpatchResult_6;

    return "";
}

MessageIndex getNumMessages() const {
    return 1;
}

const MessageInfo& getMessageInfo(MessageIndex index) const {
    switch (index) {
    case 0:
        {
        static const MessageInfo r0 = {
            "attack",
            Outport
        };

        return r0;
        }
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

    this->numberobj_20_value_set(v);
}

void param_02_value_set(number v) {
    v = this->param_02_value_constrain(v);
    this->param_02_value = v;
    this->sendParameter(1, false);

    if (this->param_02_value != this->param_02_lastValue) {
        this->getEngine()->presetTouched();
        this->param_02_lastValue = this->param_02_value;
    }

    this->numberobj_21_value_set(v);
}

void param_03_value_set(number v) {
    v = this->param_03_value_constrain(v);
    this->param_03_value = v;
    this->sendParameter(2, false);

    if (this->param_03_value != this->param_03_lastValue) {
        this->getEngine()->presetTouched();
        this->param_03_lastValue = this->param_03_value;
    }

    this->numberobj_22_value_set(v);
}

void param_04_value_set(number v) {
    v = this->param_04_value_constrain(v);
    this->param_04_value = v;
    this->sendParameter(3, false);

    if (this->param_04_value != this->param_04_lastValue) {
        this->getEngine()->presetTouched();
        this->param_04_lastValue = this->param_04_value;
    }

    this->toggle_07_value_number_set(v);
}

void param_05_value_set(number v) {
    v = this->param_05_value_constrain(v);
    this->param_05_value = v;
    this->sendParameter(4, false);

    if (this->param_05_value != this->param_05_lastValue) {
        this->getEngine()->presetTouched();
        this->param_05_lastValue = this->param_05_value;
    }

    this->numberobj_24_value_set(v);
}

void param_06_value_set(number v) {
    v = this->param_06_value_constrain(v);
    this->param_06_value = v;
    this->sendParameter(5, false);

    if (this->param_06_value != this->param_06_lastValue) {
        this->getEngine()->presetTouched();
        this->param_06_lastValue = this->param_06_value;
    }

    this->numberobj_25_value_set(v);
}

void param_07_value_set(number v) {
    v = this->param_07_value_constrain(v);
    this->param_07_value = v;
    this->sendParameter(6, false);

    if (this->param_07_value != this->param_07_lastValue) {
        this->getEngine()->presetTouched();
        this->param_07_lastValue = this->param_07_value;
    }

    this->numberobj_27_value_set(v);
}

void param_08_value_set(number v) {
    v = this->param_08_value_constrain(v);
    this->param_08_value = v;
    this->sendParameter(7, false);

    if (this->param_08_value != this->param_08_lastValue) {
        this->getEngine()->presetTouched();
        this->param_08_lastValue = this->param_08_value;
    }

    this->numberobj_28_value_set(v);
}

void param_09_value_set(number v) {
    v = this->param_09_value_constrain(v);
    this->param_09_value = v;
    this->sendParameter(8, false);

    if (this->param_09_value != this->param_09_lastValue) {
        this->getEngine()->presetTouched();
        this->param_09_lastValue = this->param_09_value;
    }

    this->numberobj_30_value_set(v);
}

void param_10_value_set(number v) {
    v = this->param_10_value_constrain(v);
    this->param_10_value = v;
    this->sendParameter(9, false);

    if (this->param_10_value != this->param_10_lastValue) {
        this->getEngine()->presetTouched();
        this->param_10_lastValue = this->param_10_value;
    }

    this->numberobj_32_value_set(v);
}

void param_11_value_set(number v) {
    v = this->param_11_value_constrain(v);
    this->param_11_value = v;
    this->sendParameter(10, false);

    if (this->param_11_value != this->param_11_lastValue) {
        this->getEngine()->presetTouched();
        this->param_11_lastValue = this->param_11_value;
    }

    this->numberobj_34_value_set(v);
}

void param_12_value_set(number v) {
    v = this->param_12_value_constrain(v);
    this->param_12_value = v;
    this->sendParameter(11, false);

    if (this->param_12_value != this->param_12_lastValue) {
        this->getEngine()->presetTouched();
        this->param_12_lastValue = this->param_12_value;
    }

    this->toggle_10_value_number_set(v);
}

void message_11_listin_list_set(const list& v) {
    this->message_11_set_set(v);
}

void message_11_listin_number_set(number v) {
    this->message_11_set_set(v);
}

void message_11_listin_bang_bang() {
    this->message_11_trigger_bang();
}

void button_09_bangin_bang() {
    this->button_09_bangval_bang();
}

void numberobj_20_valin_set(number v) {
    this->numberobj_20_value_set(v);
}

void numberobj_20_format_set(number v) {
    this->numberobj_20_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numbertilde_01_sig_number_set(number v) {
    this->numbertilde_01_outValue = v;
}

void numbertilde_01_sig_list_set(const list& v) {
    this->numbertilde_01_outValue = v[0];
}

void numbertilde_01_mode_set(number v) {
    if (v == 1) {
        this->numbertilde_01_currentMode = 0;
    } else if (v == 2) {
        this->numbertilde_01_currentMode = 1;
    }
}

void numberobj_21_valin_set(number v) {
    this->numberobj_21_value_set(v);
}

void numberobj_21_format_set(number v) {
    this->numberobj_21_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_22_valin_set(number v) {
    this->numberobj_22_value_set(v);
}

void numberobj_22_format_set(number v) {
    this->numberobj_22_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void toggle_07_valin_set(number v) {
    this->toggle_07_value_number_set(v);
}

void numberobj_23_valin_set(number v) {
    this->numberobj_23_value_set(v);
}

void numberobj_23_format_set(number v) {
    this->numberobj_23_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void toggle_08_valin_set(number v) {
    this->toggle_08_value_number_set(v);
}

void numberobj_24_valin_set(number v) {
    this->numberobj_24_value_set(v);
}

void numberobj_24_format_set(number v) {
    this->numberobj_24_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_25_valin_set(number v) {
    this->numberobj_25_value_set(v);
}

void numberobj_25_format_set(number v) {
    this->numberobj_25_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_26_valin_set(number v) {
    this->numberobj_26_value_set(v);
}

void numberobj_26_format_set(number v) {
    this->numberobj_26_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_27_valin_set(number v) {
    this->numberobj_27_value_set(v);
}

void numberobj_27_format_set(number v) {
    this->numberobj_27_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_28_valin_set(number v) {
    this->numberobj_28_value_set(v);
}

void numberobj_28_format_set(number v) {
    this->numberobj_28_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_29_valin_set(number v) {
    this->numberobj_29_value_set(v);
}

void numberobj_29_format_set(number v) {
    this->numberobj_29_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void toggle_09_valin_set(number v) {
    this->toggle_09_value_number_set(v);
}

void numberobj_30_valin_set(number v) {
    this->numberobj_30_value_set(v);
}

void numberobj_30_format_set(number v) {
    this->numberobj_30_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_31_valin_set(number v) {
    this->numberobj_31_value_set(v);
}

void numberobj_31_format_set(number v) {
    this->numberobj_31_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_32_valin_set(number v) {
    this->numberobj_32_value_set(v);
}

void numberobj_32_format_set(number v) {
    this->numberobj_32_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_33_valin_set(number v) {
    this->numberobj_33_value_set(v);
}

void numberobj_33_format_set(number v) {
    this->numberobj_33_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_34_valin_set(number v) {
    this->numberobj_34_value_set(v);
}

void numberobj_34_format_set(number v) {
    this->numberobj_34_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void numberobj_35_valin_set(number v) {
    this->numberobj_35_value_set(v);
}

void numberobj_35_format_set(number v) {
    this->numberobj_35_currentFormat = trunc((v > 6 ? 6 : (v < 0 ? 0 : v)));
}

void toggle_10_valin_set(number v) {
    this->toggle_10_value_number_set(v);
}

void numbertilde_01_value_set(number ) {}

void linetilde_06_target_bang() {}

void metertilde_02_value_set(number ) {}

void metertilde_01_value_set(number ) {}

void linetilde_07_target_bang() {}

void metertilde_03_value_set(number ) {}

void metertilde_04_value_set(number ) {}

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

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->allocateDataRefs();
    }

    this->p_04->allocateDataRefs();
    this->p_05->allocateDataRefs();

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->allocateDataRefs();
    }

    this->p_07->allocateDataRefs();
    this->data_02_buffer->requestSize(this->mstosamps(200), 2);
    this->data_02_buffer->setSampleRate(this->sr);
    this->data_01_buffer->requestSize(this->mstosamps(200), 2);
    this->data_01_buffer->setSampleRate(this->sr);
    this->recordtilde_01_buffer = this->recordtilde_01_buffer->allocateIfNeeded();
    this->data_02_buffer = this->data_02_buffer->allocateIfNeeded();

    if (this->FreezeA->hasRequestedSize()) {
        if (this->FreezeA->wantsFill())
            this->zeroDataRef(this->FreezeA);

        this->getEngine()->sendDataRefUpdated(0);
    }

    if (this->RNBODefaultSinus->hasRequestedSize()) {
        if (this->RNBODefaultSinus->wantsFill())
            this->fillRNBODefaultSinus(this->RNBODefaultSinus);

        this->getEngine()->sendDataRefUpdated(1);
    }

    this->data_01_buffer = this->data_01_buffer->allocateIfNeeded();
    this->recordtilde_02_buffer = this->recordtilde_02_buffer->allocateIfNeeded();

    if (this->FreezeB->hasRequestedSize()) {
        if (this->FreezeB->wantsFill())
            this->zeroDataRef(this->FreezeB);

        this->getEngine()->sendDataRefUpdated(2);
    }
}

void initializeObjects() {
    this->message_11_init();
    this->numberobj_20_init();
    this->numbertilde_01_init();
    this->numberobj_21_init();
    this->metertilde_01_init();
    this->metertilde_02_init();
    this->numberobj_22_init();
    this->numberobj_23_init();
    this->numberobj_24_init();
    this->metertilde_03_init();
    this->numberobj_25_init();
    this->metertilde_04_init();
    this->numberobj_26_init();
    this->numberobj_27_init();
    this->numberobj_28_init();
    this->numberobj_29_init();
    this->data_01_init();
    this->data_02_init();
    this->numberobj_30_init();
    this->numberobj_31_init();
    this->numberobj_32_init();
    this->numberobj_33_init();
    this->numberobj_34_init();
    this->numberobj_35_init();
    this->p_01->initializeObjects();
    this->p_02->initializeObjects();

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->initializeObjects();
    }

    this->p_04->initializeObjects();
    this->p_05->initializeObjects();

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->initializeObjects();
    }

    this->p_07->initializeObjects();
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());
    this->p_01->startup();
    this->p_02->startup();

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->startup();
    }

    this->p_04->startup();
    this->p_05->startup();

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->startup();
    }

    this->p_07->startup();

    {
        this->scheduleParamInit(0, 4);
    }

    {
        this->scheduleParamInit(1, 2);
    }

    {
        this->scheduleParamInit(2, 3);
    }

    {
        this->scheduleParamInit(3, 1);
    }

    {
        this->scheduleParamInit(4, 9);
    }

    {
        this->scheduleParamInit(5, 10);
    }

    {
        this->scheduleParamInit(6, 12);
    }

    {
        this->scheduleParamInit(7, 5);
    }

    {
        this->scheduleParamInit(8, 6);
    }

    {
        this->scheduleParamInit(9, 8);
    }

    {
        this->scheduleParamInit(10, 7);
    }

    {
        this->scheduleParamInit(11, 11);
    }

    this->processParamInitEvents();
}

static number param_01_value_constrain(number v) {
    v = (v > 24 ? 24 : (v < -24 ? -24 : v));
    return v;
}

void p_02_in1_number_set(number v) {
    this->p_02->updateTime(this->_currentTime);
    this->p_02->eventinlet_03_out1_number_set(v);
}

void numberobj_20_output_set(number v) {
    this->p_02_in1_number_set(v);
}

void numberobj_20_value_set(number v) {
    this->numberobj_20_value_setter(v);
    v = this->numberobj_20_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < -12) {
        localvalue = -12;
    }

    if ((bool)(true) && localvalue > 12) {
        localvalue = 12;
    }

    if (this->numberobj_20_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-135"), localvalue, this->_currentTime);
    this->numberobj_20_output_set(localvalue);
}

static number param_02_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_01_in3_number_set(number v) {
    this->p_01->updateTime(this->_currentTime);
    this->p_01->eventinlet_01_out1_number_set(v);
}

void numberobj_21_output_set(number v) {
    this->p_01_in3_number_set(v);
}

void numberobj_21_value_set(number v) {
    this->numberobj_21_value_setter(v);
    v = this->numberobj_21_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_21_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-110"), localvalue, this->_currentTime);
    this->numberobj_21_output_set(localvalue);
}

static number param_03_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_01_in4_number_set(number v) {
    this->p_01->updateTime(this->_currentTime);
    this->p_01->eventinlet_02_out1_number_set(v);
}

void numberobj_22_output_set(number v) {
    this->p_01_in4_number_set(v);
}

void numberobj_22_value_set(number v) {
    this->numberobj_22_value_setter(v);
    v = this->numberobj_22_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_22_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-122"), localvalue, this->_currentTime);
    this->numberobj_22_output_set(localvalue);
}

static number param_04_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)1 / (number)1;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void recordtilde_02_record_set(number v) {
    this->recordtilde_02_record = v;
}

void toggle_09_output_set(number v) {
    this->recordtilde_02_record_set(v);
}

void toggle_09_value_number_set(number v) {
    this->toggle_09_value_number_setter(v);
    v = this->toggle_09_value_number;
    this->getEngine()->sendNumMessage(TAG("valout"), TAG("toggle_obj-104"), v, this->_currentTime);
    this->toggle_09_output_set(v);
}

void p_05_out4_number_set(number v) {
    this->toggle_09_value_number_set(v);
}

void linetilde_07_time_set(number v) {
    this->linetilde_07_time = v;
}

void linetilde_07_segments_set(const list& v) {
    this->linetilde_07_segments = jsCreateListCopy(v);

    if ((bool)(v->length)) {
        if (v->length == 1 && this->linetilde_07_time == 0) {
            this->linetilde_07_activeRamps->length = 0;
            this->linetilde_07_currentValue = v[0];
        } else {
            auto currentTime = this->currentsampletime();
            number lastRampValue = this->linetilde_07_currentValue;
            number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;

            for (Index i = 0; i < this->linetilde_07_activeRamps->length; i += 3) {
                rampEnd = this->linetilde_07_activeRamps[(Index)(i + 2)];

                if (rampEnd > currentTime) {
                    this->linetilde_07_activeRamps[(Index)(i + 2)] = currentTime;
                    number diff = rampEnd - currentTime;
                    number valueDiff = diff * this->linetilde_07_activeRamps[(Index)(i + 1)];
                    lastRampValue = this->linetilde_07_activeRamps[(Index)i] - valueDiff;
                    this->linetilde_07_activeRamps[(Index)i] = lastRampValue;
                    this->linetilde_07_activeRamps->length = i + 3;
                    rampEnd = currentTime;
                } else {
                    lastRampValue = this->linetilde_07_activeRamps[(Index)i];
                }
            }

            if (rampEnd < currentTime) {
                this->linetilde_07_activeRamps->push(lastRampValue);
                this->linetilde_07_activeRamps->push(0);
                this->linetilde_07_activeRamps->push(currentTime);
            }

            number lastRampEnd = currentTime;

            for (Index i = 0; i < v->length; i += 2) {
                number destinationValue = v[(Index)i];
                number inc = 0;
                number rampTimeInSamples;

                if (v->length > i + 1) {
                    rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);

                    if ((bool)(this->linetilde_07_keepramp)) {
                        this->linetilde_07_time_set(v[(Index)(i + 1)]);
                    }
                } else {
                    rampTimeInSamples = this->mstosamps(this->linetilde_07_time);
                }

                if (rampTimeInSamples <= 0) {
                    rampTimeInSamples = 1;
                }

                inc = (destinationValue - lastRampValue) / rampTimeInSamples;
                lastRampEnd += rampTimeInSamples;
                this->linetilde_07_activeRamps->push(destinationValue);
                this->linetilde_07_activeRamps->push(inc);
                this->linetilde_07_activeRamps->push(lastRampEnd);
                lastRampValue = destinationValue;
            }
        }
    }
}

void numberobj_26_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_07_segments_set(converted);
    }
}

void numberobj_26_value_set(number v) {
    this->numberobj_26_value_setter(v);
    v = this->numberobj_26_value;
    number localvalue = v;

    if (this->numberobj_26_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-105"), localvalue, this->_currentTime);
    this->numberobj_26_output_set(localvalue);
}

void p_05_out3_number_set(number v) {
    this->numberobj_26_value_set(v);
}

void recordtilde_01_record_set(number v) {
    this->recordtilde_01_record = v;
}

void toggle_08_output_set(number v) {
    this->recordtilde_01_record_set(v);
}

void toggle_08_value_number_set(number v) {
    this->toggle_08_value_number_setter(v);
    v = this->toggle_08_value_number;
    this->getEngine()->sendNumMessage(TAG("valout"), TAG("toggle_obj-101"), v, this->_currentTime);
    this->toggle_08_output_set(v);
}

void p_05_out2_number_set(number v) {
    this->toggle_08_value_number_set(v);
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

void numberobj_23_output_set(number v) {
    {
        list converted = {v};
        this->linetilde_06_segments_set(converted);
    }
}

void numberobj_23_value_set(number v) {
    this->numberobj_23_value_setter(v);
    v = this->numberobj_23_value;
    number localvalue = v;

    if (this->numberobj_23_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-103"), localvalue, this->_currentTime);
    this->numberobj_23_output_set(localvalue);
}

void p_05_out1_number_set(number v) {
    this->numberobj_23_value_set(v);
}

void p_05_in1_number_set(number v) {
    this->p_05->updateTime(this->_currentTime);
    this->p_05->eventinlet_07_out1_number_set(v);
}

void toggle_07_output_set(number v) {
    this->p_05_in1_number_set(v);
}

void toggle_07_value_number_set(number v) {
    this->toggle_07_value_number_setter(v);
    v = this->toggle_07_value_number;
    this->getEngine()->sendNumMessage(TAG("valout"), TAG("toggle_obj-11"), v, this->_currentTime);
    this->toggle_07_output_set(v);
}

static number param_05_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_04_in3_number_set(number v) {
    this->p_04->updateTime(this->_currentTime);
    this->p_04->eventinlet_04_out1_number_set(v);
}

void numberobj_24_output_set(number v) {
    this->p_04_in3_number_set(v);
}

void numberobj_24_value_set(number v) {
    this->numberobj_24_value_setter(v);
    v = this->numberobj_24_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_24_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-50"), localvalue, this->_currentTime);
    this->numberobj_24_output_set(localvalue);
}

static number param_06_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_04_in4_number_set(number v) {
    this->p_04->updateTime(this->_currentTime);
    this->p_04->eventinlet_06_out1_number_set(v);
}

void numberobj_25_output_set(number v) {
    this->p_04_in4_number_set(v);
}

void numberobj_25_value_set(number v) {
    this->numberobj_25_value_setter(v);
    v = this->numberobj_25_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_25_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-39"), localvalue, this->_currentTime);
    this->numberobj_25_output_set(localvalue);
}

static number param_07_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < -100 ? -100 : v));
    return v;
}

void p_04_in5_number_set(number v) {
    this->p_04->updateTime(this->_currentTime);
    this->p_04->eventinlet_05_out1_number_set(v);
}

void numberobj_27_output_set(number v) {
    this->p_04_in5_number_set(v);
}

void numberobj_27_value_set(number v) {
    this->numberobj_27_value_setter(v);
    v = this->numberobj_27_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < -100) {
        localvalue = -100;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_27_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-57"), localvalue, this->_currentTime);
    this->numberobj_27_output_set(localvalue);
}

static number param_08_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void numberobj_29_output_set(number v) {
    this->linetilde_07_time_set(v);
    this->linetilde_06_time_set(v);
}

void numberobj_29_value_set(number v) {
    this->numberobj_29_value_setter(v);
    v = this->numberobj_29_value;
    number localvalue = v;

    if (this->numberobj_29_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-123"), localvalue, this->_currentTime);
    this->numberobj_29_output_set(localvalue);
}

void scale_07_out_set(const list& v) {
    this->scale_07_out = jsCreateListCopy(v);

    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_29_value_set(converted);
    }
}

void scale_07_input_set(const list& v) {
    this->scale_07_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_07_inlow,
            this->scale_07_inhigh,
            this->scale_07_outlow,
            this->scale_07_outhigh,
            this->scale_07_power
        ));
    }

    this->scale_07_out_set(tmp);
}

void numberobj_28_output_set(number v) {
    {
        list converted = {v};
        this->scale_07_input_set(converted);
    }
}

void numberobj_28_value_set(number v) {
    this->numberobj_28_value_setter(v);
    v = this->numberobj_28_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_28_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-153"), localvalue, this->_currentTime);
    this->numberobj_28_output_set(localvalue);
}

static number param_09_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_03_target_PosJitterDepth_number_set(number v) {
    for (Index i = 0; i < 8; i++) {
        if (i + 1 == this->p_03_target || 0 == this->p_03_target) {
            this->p_03[i]->updateTime(this->_currentTime);
        }
    }

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->receive_04_output_number_set(v);
    }
}

void p_06_target_PosJitterDepth_number_set(number v) {
    for (Index i = 0; i < 8; i++) {
        if (i + 1 == this->p_06_target || 0 == this->p_06_target) {
            this->p_06[i]->updateTime(this->_currentTime);
        }
    }

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->receive_10_output_number_set(v);
    }
}

void send_01_input_number_set(number v) {
    this->send_01_input_number = v;
    this->p_03_target_PosJitterDepth_number_set(v);
    this->p_06_target_PosJitterDepth_number_set(v);
}

void numberobj_31_output_set(number v) {
    this->send_01_input_number_set(v);
}

void numberobj_31_value_set(number v) {
    this->numberobj_31_value_setter(v);
    v = this->numberobj_31_value;
    number localvalue = v;

    if (this->numberobj_31_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-184"), localvalue, this->_currentTime);
    this->numberobj_31_output_set(localvalue);
}

void scale_08_out_set(const list& v) {
    this->scale_08_out = jsCreateListCopy(v);

    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_31_value_set(converted);
    }
}

void scale_08_input_set(const list& v) {
    this->scale_08_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_08_inlow,
            this->scale_08_inhigh,
            this->scale_08_outlow,
            this->scale_08_outhigh,
            this->scale_08_power
        ));
    }

    this->scale_08_out_set(tmp);
}

void numberobj_30_output_set(number v) {
    {
        list converted = {v};
        this->scale_08_input_set(converted);
    }
}

void numberobj_30_value_set(number v) {
    this->numberobj_30_value_setter(v);
    v = this->numberobj_30_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_30_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-182"), localvalue, this->_currentTime);
    this->numberobj_30_output_set(localvalue);
}

static number param_10_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_03_target_PanJitterDepth_number_set(number v) {
    for (Index i = 0; i < 8; i++) {
        if (i + 1 == this->p_03_target || 0 == this->p_03_target) {
            this->p_03[i]->updateTime(this->_currentTime);
        }
    }

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->receive_06_output_number_set(v);
    }
}

void p_06_target_PanJitterDepth_number_set(number v) {
    for (Index i = 0; i < 8; i++) {
        if (i + 1 == this->p_06_target || 0 == this->p_06_target) {
            this->p_06[i]->updateTime(this->_currentTime);
        }
    }

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->receive_12_output_number_set(v);
    }
}

void send_02_input_number_set(number v) {
    this->send_02_input_number = v;
    this->p_03_target_PanJitterDepth_number_set(v);
    this->p_06_target_PanJitterDepth_number_set(v);
}

void numberobj_33_output_set(number v) {
    this->send_02_input_number_set(v);
}

void numberobj_33_value_set(number v) {
    this->numberobj_33_value_setter(v);
    v = this->numberobj_33_value;
    number localvalue = v;

    if (this->numberobj_33_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-176"), localvalue, this->_currentTime);
    this->numberobj_33_output_set(localvalue);
}

void scale_09_out_set(const list& v) {
    this->scale_09_out = jsCreateListCopy(v);

    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_33_value_set(converted);
    }
}

void scale_09_input_set(const list& v) {
    this->scale_09_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_09_inlow,
            this->scale_09_inhigh,
            this->scale_09_outlow,
            this->scale_09_outhigh,
            this->scale_09_power
        ));
    }

    this->scale_09_out_set(tmp);
}

void numberobj_32_output_set(number v) {
    {
        list converted = {v};
        this->scale_09_input_set(converted);
    }
}

void numberobj_32_value_set(number v) {
    this->numberobj_32_value_setter(v);
    v = this->numberobj_32_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_32_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-174"), localvalue, this->_currentTime);
    this->numberobj_32_output_set(localvalue);
}

static number param_11_value_constrain(number v) {
    v = (v > 100 ? 100 : (v < 0 ? 0 : v));
    return v;
}

void p_03_target_AmpJitterDepth_number_set(number v) {
    for (Index i = 0; i < 8; i++) {
        if (i + 1 == this->p_03_target || 0 == this->p_03_target) {
            this->p_03[i]->updateTime(this->_currentTime);
        }
    }

    for (Index i = 0; i < 8; i++) {
        this->p_03[i]->receive_05_output_number_set(v);
    }
}

void p_06_target_AmpJitterDepth_number_set(number v) {
    for (Index i = 0; i < 8; i++) {
        if (i + 1 == this->p_06_target || 0 == this->p_06_target) {
            this->p_06[i]->updateTime(this->_currentTime);
        }
    }

    for (Index i = 0; i < 8; i++) {
        this->p_06[i]->receive_11_output_number_set(v);
    }
}

void send_03_input_number_set(number v) {
    this->send_03_input_number = v;
    this->p_03_target_AmpJitterDepth_number_set(v);
    this->p_06_target_AmpJitterDepth_number_set(v);
}

void numberobj_35_output_set(number v) {
    this->send_03_input_number_set(v);
}

void numberobj_35_value_set(number v) {
    this->numberobj_35_value_setter(v);
    v = this->numberobj_35_value;
    number localvalue = v;

    if (this->numberobj_35_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-163"), localvalue, this->_currentTime);
    this->numberobj_35_output_set(localvalue);
}

void scale_10_out_set(const list& v) {
    this->scale_10_out = jsCreateListCopy(v);

    {
        number converted = (v->length > 0 ? v[0] : 0);
        this->numberobj_35_value_set(converted);
    }
}

void scale_10_input_set(const list& v) {
    this->scale_10_input = jsCreateListCopy(v);
    list tmp = {};

    for (Index i = 0; i < v->length; i++) {
        tmp->push(this->scale(
            v[(Index)i],
            this->scale_10_inlow,
            this->scale_10_inhigh,
            this->scale_10_outlow,
            this->scale_10_outhigh,
            this->scale_10_power
        ));
    }

    this->scale_10_out_set(tmp);
}

void numberobj_34_output_set(number v) {
    {
        list converted = {v};
        this->scale_10_input_set(converted);
    }
}

void numberobj_34_value_set(number v) {
    this->numberobj_34_value_setter(v);
    v = this->numberobj_34_value;
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_34_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->getEngine()->sendNumMessage(TAG("valout"), TAG("number_obj-161"), localvalue, this->_currentTime);
    this->numberobj_34_output_set(localvalue);
}

static number param_12_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));

    {
        number oneStep = (number)1 / (number)1;
        number oneStepInv = (oneStep != 0 ? (number)1 / oneStep : 0);
        number numberOfSteps = rnbo_fround(v * oneStepInv * 1 / (number)1) * 1;
        v = numberOfSteps * oneStep;
    }

    return v;
}

void p_07_in5_number_set(number v) {
    this->p_07->updateTime(this->_currentTime);
    this->p_07->eventinlet_08_out1_number_set(v);
}

void toggle_10_output_set(number v) {
    this->p_07_in5_number_set(v);
}

void toggle_10_value_number_set(number v) {
    this->toggle_10_value_number_setter(v);
    v = this->toggle_10_value_number;
    this->getEngine()->sendNumMessage(TAG("valout"), TAG("toggle_obj-79"), v, this->_currentTime);
    this->toggle_10_output_set(v);
}

void message_11_set_set(const list& v) {
    this->message_11_set = jsCreateListCopy(v);
    this->getEngine()->sendListMessage(TAG("listout"), TAG("message_obj-107"), v, this->_currentTime);
}

void param_04_normalizedvalue_set(number v) {
    this->param_04_value_set(this->fromnormalized(3, v));
}

void message_11_out_set(const list& v) {
    {
        if (v->length > 1)
            this->param_04_normalizedvalue_set(v[1]);

        number converted = (v->length > 0 ? v[0] : 0);
        this->param_04_value_set(converted);
    }
}

void message_11_trigger_bang() {
    this->message_11_out_set(this->message_11_set);
}

void button_09_output_bang() {
    this->message_11_trigger_bang();
}

void button_09_bangval_bang() {
    this->getEngine()->sendBangMessage(TAG("bangout"), TAG("button_obj-102"), this->_currentTime);;
    this->button_09_output_bang();
}

void ctlin_01_outchannel_set(number ) {}

void ctlin_01_outcontroller_set(number ) {}

void fromnormalized_01_output_set(number v) {
    this->param_01_value_set(v);
}

void fromnormalized_01_input_set(number v) {
    this->fromnormalized_01_output_set(this->fromnormalized(0, v));
}

void expr_10_out1_set(number v) {
    this->expr_10_out1 = v;
    this->fromnormalized_01_input_set(this->expr_10_out1);
}

void expr_10_in1_set(number in1) {
    this->expr_10_in1 = in1;
    this->expr_10_out1_set(this->expr_10_in1 * this->expr_10_in2);//#map:expr_10:1
}

void ctlin_01_value_set(number v) {
    this->expr_10_in1_set(v);
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

void expr_11_out1_set(number v) {
    this->expr_11_out1 = v;
    this->fromnormalized_02_input_set(this->expr_11_out1);
}

void expr_11_in1_set(number in1) {
    this->expr_11_in1 = in1;
    this->expr_11_out1_set(this->expr_11_in1 * this->expr_11_in2);//#map:expr_11:1
}

void ctlin_02_value_set(number v) {
    this->expr_11_in1_set(v);
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

void expr_12_out1_set(number v) {
    this->expr_12_out1 = v;
    this->fromnormalized_03_input_set(this->expr_12_out1);
}

void expr_12_in1_set(number in1) {
    this->expr_12_in1 = in1;
    this->expr_12_out1_set(this->expr_12_in1 * this->expr_12_in2);//#map:expr_12:1
}

void ctlin_03_value_set(number v) {
    this->expr_12_in1_set(v);
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

void expr_13_out1_set(number v) {
    this->expr_13_out1 = v;
    this->fromnormalized_04_input_set(this->expr_13_out1);
}

void expr_13_in1_set(number in1) {
    this->expr_13_in1 = in1;
    this->expr_13_out1_set(this->expr_13_in1 * this->expr_13_in2);//#map:expr_13:1
}

void ctlin_04_value_set(number v) {
    this->expr_13_in1_set(v);
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

void expr_14_out1_set(number v) {
    this->expr_14_out1 = v;
    this->fromnormalized_05_input_set(this->expr_14_out1);
}

void expr_14_in1_set(number in1) {
    this->expr_14_in1 = in1;
    this->expr_14_out1_set(this->expr_14_in1 * this->expr_14_in2);//#map:expr_14:1
}

void ctlin_05_value_set(number v) {
    this->expr_14_in1_set(v);
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

void expr_15_out1_set(number v) {
    this->expr_15_out1 = v;
    this->fromnormalized_06_input_set(this->expr_15_out1);
}

void expr_15_in1_set(number in1) {
    this->expr_15_in1 = in1;
    this->expr_15_out1_set(this->expr_15_in1 * this->expr_15_in2);//#map:expr_15:1
}

void ctlin_06_value_set(number v) {
    this->expr_15_in1_set(v);
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

void ctlin_07_outchannel_set(number ) {}

void ctlin_07_outcontroller_set(number ) {}

void fromnormalized_07_output_set(number v) {
    this->param_07_value_set(v);
}

void fromnormalized_07_input_set(number v) {
    this->fromnormalized_07_output_set(this->fromnormalized(6, v));
}

void expr_16_out1_set(number v) {
    this->expr_16_out1 = v;
    this->fromnormalized_07_input_set(this->expr_16_out1);
}

void expr_16_in1_set(number in1) {
    this->expr_16_in1 = in1;
    this->expr_16_out1_set(this->expr_16_in1 * this->expr_16_in2);//#map:expr_16:1
}

void ctlin_07_value_set(number v) {
    this->expr_16_in1_set(v);
}

void ctlin_07_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_07_channel || this->ctlin_07_channel == -1) && (data[1] == this->ctlin_07_controller || this->ctlin_07_controller == -1)) {
        this->ctlin_07_outchannel_set(channel);
        this->ctlin_07_outcontroller_set(data[1]);
        this->ctlin_07_value_set(data[2]);
        this->ctlin_07_status = 0;
    }
}

void ctlin_08_outchannel_set(number ) {}

void ctlin_08_outcontroller_set(number ) {}

void fromnormalized_08_output_set(number v) {
    this->param_08_value_set(v);
}

void fromnormalized_08_input_set(number v) {
    this->fromnormalized_08_output_set(this->fromnormalized(7, v));
}

void expr_17_out1_set(number v) {
    this->expr_17_out1 = v;
    this->fromnormalized_08_input_set(this->expr_17_out1);
}

void expr_17_in1_set(number in1) {
    this->expr_17_in1 = in1;
    this->expr_17_out1_set(this->expr_17_in1 * this->expr_17_in2);//#map:expr_17:1
}

void ctlin_08_value_set(number v) {
    this->expr_17_in1_set(v);
}

void ctlin_08_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_08_channel || this->ctlin_08_channel == -1) && (data[1] == this->ctlin_08_controller || this->ctlin_08_controller == -1)) {
        this->ctlin_08_outchannel_set(channel);
        this->ctlin_08_outcontroller_set(data[1]);
        this->ctlin_08_value_set(data[2]);
        this->ctlin_08_status = 0;
    }
}

void ctlin_09_outchannel_set(number ) {}

void ctlin_09_outcontroller_set(number ) {}

void fromnormalized_09_output_set(number v) {
    this->param_09_value_set(v);
}

void fromnormalized_09_input_set(number v) {
    this->fromnormalized_09_output_set(this->fromnormalized(8, v));
}

void expr_18_out1_set(number v) {
    this->expr_18_out1 = v;
    this->fromnormalized_09_input_set(this->expr_18_out1);
}

void expr_18_in1_set(number in1) {
    this->expr_18_in1 = in1;
    this->expr_18_out1_set(this->expr_18_in1 * this->expr_18_in2);//#map:expr_18:1
}

void ctlin_09_value_set(number v) {
    this->expr_18_in1_set(v);
}

void ctlin_09_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_09_channel || this->ctlin_09_channel == -1) && (data[1] == this->ctlin_09_controller || this->ctlin_09_controller == -1)) {
        this->ctlin_09_outchannel_set(channel);
        this->ctlin_09_outcontroller_set(data[1]);
        this->ctlin_09_value_set(data[2]);
        this->ctlin_09_status = 0;
    }
}

void ctlin_10_outchannel_set(number ) {}

void ctlin_10_outcontroller_set(number ) {}

void fromnormalized_10_output_set(number v) {
    this->param_10_value_set(v);
}

void fromnormalized_10_input_set(number v) {
    this->fromnormalized_10_output_set(this->fromnormalized(9, v));
}

void expr_19_out1_set(number v) {
    this->expr_19_out1 = v;
    this->fromnormalized_10_input_set(this->expr_19_out1);
}

void expr_19_in1_set(number in1) {
    this->expr_19_in1 = in1;
    this->expr_19_out1_set(this->expr_19_in1 * this->expr_19_in2);//#map:expr_19:1
}

void ctlin_10_value_set(number v) {
    this->expr_19_in1_set(v);
}

void ctlin_10_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_10_channel || this->ctlin_10_channel == -1) && (data[1] == this->ctlin_10_controller || this->ctlin_10_controller == -1)) {
        this->ctlin_10_outchannel_set(channel);
        this->ctlin_10_outcontroller_set(data[1]);
        this->ctlin_10_value_set(data[2]);
        this->ctlin_10_status = 0;
    }
}

void ctlin_11_outchannel_set(number ) {}

void ctlin_11_outcontroller_set(number ) {}

void fromnormalized_11_output_set(number v) {
    this->param_11_value_set(v);
}

void fromnormalized_11_input_set(number v) {
    this->fromnormalized_11_output_set(this->fromnormalized(10, v));
}

void expr_20_out1_set(number v) {
    this->expr_20_out1 = v;
    this->fromnormalized_11_input_set(this->expr_20_out1);
}

void expr_20_in1_set(number in1) {
    this->expr_20_in1 = in1;
    this->expr_20_out1_set(this->expr_20_in1 * this->expr_20_in2);//#map:expr_20:1
}

void ctlin_11_value_set(number v) {
    this->expr_20_in1_set(v);
}

void ctlin_11_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_11_channel || this->ctlin_11_channel == -1) && (data[1] == this->ctlin_11_controller || this->ctlin_11_controller == -1)) {
        this->ctlin_11_outchannel_set(channel);
        this->ctlin_11_outcontroller_set(data[1]);
        this->ctlin_11_value_set(data[2]);
        this->ctlin_11_status = 0;
    }
}

void ctlin_12_outchannel_set(number ) {}

void ctlin_12_outcontroller_set(number ) {}

void fromnormalized_12_output_set(number v) {
    this->param_12_value_set(v);
}

void fromnormalized_12_input_set(number v) {
    this->fromnormalized_12_output_set(this->fromnormalized(11, v));
}

void expr_21_out1_set(number v) {
    this->expr_21_out1 = v;
    this->fromnormalized_12_input_set(this->expr_21_out1);
}

void expr_21_in1_set(number in1) {
    this->expr_21_in1 = in1;
    this->expr_21_out1_set(this->expr_21_in1 * this->expr_21_in2);//#map:expr_21:1
}

void ctlin_12_value_set(number v) {
    this->expr_21_in1_set(v);
}

void ctlin_12_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(length);
    RNBO_UNUSED(port);

    if (status == 0xB0 && (channel == this->ctlin_12_channel || this->ctlin_12_channel == -1) && (data[1] == this->ctlin_12_controller || this->ctlin_12_controller == -1)) {
        this->ctlin_12_outchannel_set(channel);
        this->ctlin_12_outcontroller_set(data[1]);
        this->ctlin_12_value_set(data[2]);
        this->ctlin_12_status = 0;
    }
}

void p_01_out1_bang_bang() {
    this->button_09_bangval_bang();
}

void p_02_perform(SampleValue * out1, Index n) {
    // subpatcher: Transp~
    SampleArray<1> outs = {out1};

    this->p_02->process(nullptr, 0, outs, 1, n);
}

void numbertilde_01_perform(const SampleValue * input_signal, SampleValue * output, Index n) {
    auto __numbertilde_01_currentIntervalInSamples = this->numbertilde_01_currentIntervalInSamples;
    auto __numbertilde_01_lastValue = this->numbertilde_01_lastValue;
    auto __numbertilde_01_currentInterval = this->numbertilde_01_currentInterval;
    auto __numbertilde_01_rampInSamples = this->numbertilde_01_rampInSamples;
    auto __numbertilde_01_outValue = this->numbertilde_01_outValue;
    auto __numbertilde_01_currentMode = this->numbertilde_01_currentMode;
    number monitorvalue = input_signal[0];

    for (Index i = 0; i < n; i++) {
        if (__numbertilde_01_currentMode == 0) {
            output[(Index)i] = this->numbertilde_01_smooth_next(
                __numbertilde_01_outValue,
                __numbertilde_01_rampInSamples,
                __numbertilde_01_rampInSamples
            );
        } else {
            output[(Index)i] = input_signal[(Index)i];
        }
    }

    __numbertilde_01_currentInterval -= n;

    if (monitorvalue != __numbertilde_01_lastValue && __numbertilde_01_currentInterval <= 0) {
        __numbertilde_01_currentInterval = __numbertilde_01_currentIntervalInSamples;

        this->getEngine()->scheduleClockEventWithValue(
            this,
            2098551528,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            monitorvalue
        );;

        __numbertilde_01_lastValue = monitorvalue;
        this->getEngine()->sendListMessage(TAG("monitor"), TAG("number~_obj-74"), {monitorvalue}, this->_currentTime);;
    }

    this->numbertilde_01_currentInterval = __numbertilde_01_currentInterval;
    this->numbertilde_01_lastValue = __numbertilde_01_lastValue;
}

void phasor_01_perform(const Sample * freq, SampleValue * out, Index n) {
    for (Index i = 0; i < n; i++) {
        out[(Index)i] = this->phasor_01_ph_next(freq[(Index)i], -1);
        this->phasor_01_sigbuf[(Index)i] = -1;
    }
}

void p_03_perform(const SampleValue * in1, SampleValue * out1, SampleValue * out2, Index n) {
    ConstSampleArray<1> ins = {in1};
    SampleArray<2> outs = {out1, out2};

    for (number chan = 0; chan < 2; chan++)
        zeroSignal(outs[(Index)chan], n);

    for (Index i = 0; i < 8; i++)
        this->p_03[(Index)i]->process(ins, 1, outs, 2, n);
}

void p_06_perform(const SampleValue * in1, SampleValue * out1, SampleValue * out2, Index n) {
    ConstSampleArray<1> ins = {in1};
    SampleArray<2> outs = {out1, out2};

    for (number chan = 0; chan < 2; chan++)
        zeroSignal(outs[(Index)chan], n);

    for (Index i = 0; i < 8; i++)
        this->p_06[(Index)i]->process(ins, 1, outs, 2, n);
}

void p_01_perform(const SampleValue * in1, const SampleValue * in2, Index n) {
    // subpatcher: Attack~
    ConstSampleArray<2> ins = {in1, in2};

    this->p_01->process(ins, 2, nullptr, 0, n);
}

void recordtilde_01_perform(
    number record,
    number begin,
    number end,
    const SampleValue * input1,
    const SampleValue * input2,
    SampleValue * sync,
    Index n
) {
    RNBO_UNUSED(input2);
    RNBO_UNUSED(input1);
    RNBO_UNUSED(end);
    RNBO_UNUSED(begin);
    auto __recordtilde_01_loop = this->recordtilde_01_loop;
    auto __recordtilde_01_wIndex = this->recordtilde_01_wIndex;
    auto __recordtilde_01_lastRecord = this->recordtilde_01_lastRecord;
    auto __recordtilde_01_buffer = this->recordtilde_01_buffer;
    ConstSampleArray<2> input = {input1, input2};
    number bufferSize = __recordtilde_01_buffer->getSize();
    number srInv = (number)1 / this->samplerate();

    if (bufferSize > 0) {
        number maxChannel = __recordtilde_01_buffer->getChannels();
        number touched = false;

        for (Index i = 0; i < n; i++) {
            number loopBegin = 0;
            number loopEnd = bufferSize;

            if (loopEnd > loopBegin) {
                {
                    if ((bool)(record) && __recordtilde_01_lastRecord != record) {
                        __recordtilde_01_wIndex = loopBegin;
                    }
                }

                if (record != 0 && __recordtilde_01_wIndex < loopEnd) {
                    for (number channel = 0; channel < 2; channel++) {
                        number effectiveChannel = channel + 0;

                        if (effectiveChannel < maxChannel) {
                            __recordtilde_01_buffer->setSample(channel, __recordtilde_01_wIndex, input[(Index)channel][(Index)i]);
                            touched = true;
                        } else
                            break;
                    }

                    __recordtilde_01_wIndex++;

                    if ((bool)(__recordtilde_01_loop) && __recordtilde_01_wIndex >= loopEnd) {
                        __recordtilde_01_wIndex = loopBegin;
                    }
                } else {
                    sync[(Index)i] = 0;
                }

                {
                    __recordtilde_01_lastRecord = record;
                }
            }
        }

        if ((bool)(touched)) {
            __recordtilde_01_buffer->setTouched(true);
            __recordtilde_01_buffer->setSampleRate(this->samplerate());
        }
    }

    this->recordtilde_01_lastRecord = __recordtilde_01_lastRecord;
    this->recordtilde_01_wIndex = __recordtilde_01_wIndex;
}

void recordtilde_02_perform(
    number record,
    number begin,
    number end,
    const SampleValue * input1,
    const SampleValue * input2,
    SampleValue * sync,
    Index n
) {
    RNBO_UNUSED(input2);
    RNBO_UNUSED(input1);
    RNBO_UNUSED(end);
    RNBO_UNUSED(begin);
    auto __recordtilde_02_loop = this->recordtilde_02_loop;
    auto __recordtilde_02_wIndex = this->recordtilde_02_wIndex;
    auto __recordtilde_02_lastRecord = this->recordtilde_02_lastRecord;
    auto __recordtilde_02_buffer = this->recordtilde_02_buffer;
    ConstSampleArray<2> input = {input1, input2};
    number bufferSize = __recordtilde_02_buffer->getSize();
    number srInv = (number)1 / this->samplerate();

    if (bufferSize > 0) {
        number maxChannel = __recordtilde_02_buffer->getChannels();
        number touched = false;

        for (Index i = 0; i < n; i++) {
            number loopBegin = 0;
            number loopEnd = bufferSize;

            if (loopEnd > loopBegin) {
                {
                    if ((bool)(record) && __recordtilde_02_lastRecord != record) {
                        __recordtilde_02_wIndex = loopBegin;
                    }
                }

                if (record != 0 && __recordtilde_02_wIndex < loopEnd) {
                    for (number channel = 0; channel < 2; channel++) {
                        number effectiveChannel = channel + 0;

                        if (effectiveChannel < maxChannel) {
                            __recordtilde_02_buffer->setSample(channel, __recordtilde_02_wIndex, input[(Index)channel][(Index)i]);
                            touched = true;
                        } else
                            break;
                    }

                    __recordtilde_02_wIndex++;

                    if ((bool)(__recordtilde_02_loop) && __recordtilde_02_wIndex >= loopEnd) {
                        __recordtilde_02_wIndex = loopBegin;
                    }
                } else {
                    sync[(Index)i] = 0;
                }

                {
                    __recordtilde_02_lastRecord = record;
                }
            }
        }

        if ((bool)(touched)) {
            __recordtilde_02_buffer->setTouched(true);
            __recordtilde_02_buffer->setSampleRate(this->samplerate());
        }
    }

    this->recordtilde_02_lastRecord = __recordtilde_02_lastRecord;
    this->recordtilde_02_wIndex = __recordtilde_02_wIndex;
}

void p_07_perform(
    const SampleValue * in1,
    const SampleValue * in2,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    // subpatcher: Direct~
    ConstSampleArray<2> ins = {in1, in2};

    SampleArray<2> outs = {out1, out2};
    this->p_07->process(ins, 2, outs, 2, n);
}

void p_05_perform(Index n) {
    // subpatcher: Freeze
    this->p_05->process(nullptr, 0, nullptr, 0, n);
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
                        -1841075377,
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

void hann_tilde_03_perform(const Sample * x, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        number __x = x[(Index)i];
        __x = (__x > 1 ? 1 : (__x < 0 ? 0 : __x));
        out1[(Index)i] = 0.5 * (1 - rnbo_cos(6.28318530717958647692 * __x));
    }
}

void dspexpr_43_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void metertilde_02_perform(const SampleValue * input, Index n) {
    auto __metertilde_02_currentIntervalInSamples = this->metertilde_02_currentIntervalInSamples;
    auto __metertilde_02_currentInterval = this->metertilde_02_currentInterval;
    auto __metertilde_02_currentPeak = this->metertilde_02_currentPeak;

    for (Index i = 0; i < n; i++) {
        number value = (input[(Index)i] < 0 ? -input[(Index)i] : input[(Index)i]);

        if (value > __metertilde_02_currentPeak)
            __metertilde_02_currentPeak = value;
    }

    __metertilde_02_currentInterval -= n;

    if (__metertilde_02_currentInterval <= 0) {
        __metertilde_02_currentInterval = __metertilde_02_currentIntervalInSamples;

        this->getEngine()->sendNumMessage(
            TAG("peak"),
            TAG("meter~_obj-97"),
            __metertilde_02_currentPeak,
            this->_currentTime
        );;

        this->getEngine()->scheduleClockEventWithValue(
            this,
            373954921,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            __metertilde_02_currentPeak
        );;

        __metertilde_02_currentPeak = 0;
    }

    this->metertilde_02_currentPeak = __metertilde_02_currentPeak;
    this->metertilde_02_currentInterval = __metertilde_02_currentInterval;
}

void dspexpr_41_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void metertilde_01_perform(const SampleValue * input, Index n) {
    auto __metertilde_01_currentIntervalInSamples = this->metertilde_01_currentIntervalInSamples;
    auto __metertilde_01_currentInterval = this->metertilde_01_currentInterval;
    auto __metertilde_01_currentPeak = this->metertilde_01_currentPeak;

    for (Index i = 0; i < n; i++) {
        number value = (input[(Index)i] < 0 ? -input[(Index)i] : input[(Index)i]);

        if (value > __metertilde_01_currentPeak)
            __metertilde_01_currentPeak = value;
    }

    __metertilde_01_currentInterval -= n;

    if (__metertilde_01_currentInterval <= 0) {
        __metertilde_01_currentInterval = __metertilde_01_currentIntervalInSamples;

        this->getEngine()->sendNumMessage(
            TAG("peak"),
            TAG("meter~_obj-96"),
            __metertilde_01_currentPeak,
            this->_currentTime
        );;

        this->getEngine()->scheduleClockEventWithValue(
            this,
            -327874582,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            __metertilde_01_currentPeak
        );;

        __metertilde_01_currentPeak = 0;
    }

    this->metertilde_01_currentPeak = __metertilde_01_currentPeak;
    this->metertilde_01_currentInterval = __metertilde_01_currentInterval;
}

void linetilde_07_perform(SampleValue * out, Index n) {
    auto __linetilde_07_time = this->linetilde_07_time;
    auto __linetilde_07_keepramp = this->linetilde_07_keepramp;
    auto __linetilde_07_currentValue = this->linetilde_07_currentValue;
    Index i = 0;

    if ((bool)(this->linetilde_07_activeRamps->length)) {
        while ((bool)(this->linetilde_07_activeRamps->length) && i < n) {
            number destinationValue = this->linetilde_07_activeRamps[0];
            number inc = this->linetilde_07_activeRamps[1];
            number rampTimeInSamples = this->linetilde_07_activeRamps[2] - this->audioProcessSampleCount - i;
            number val = __linetilde_07_currentValue;

            while (rampTimeInSamples > 0 && i < n) {
                out[(Index)i] = val;
                val += inc;
                i++;
                rampTimeInSamples--;
            }

            if (rampTimeInSamples <= 0) {
                val = destinationValue;
                this->linetilde_07_activeRamps->splice(0, 3);

                if ((bool)(!(bool)(this->linetilde_07_activeRamps->length))) {
                    this->getEngine()->scheduleClockEventWithValue(
                        this,
                        -281953904,
                        this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                        0
                    );;

                    if ((bool)(!(bool)(__linetilde_07_keepramp))) {
                        __linetilde_07_time = 0;
                    }
                }
            }

            __linetilde_07_currentValue = val;
        }
    }

    while (i < n) {
        out[(Index)i] = __linetilde_07_currentValue;
        i++;
    }

    this->linetilde_07_currentValue = __linetilde_07_currentValue;
    this->linetilde_07_time = __linetilde_07_time;
}

void hann_tilde_04_perform(const Sample * x, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        number __x = x[(Index)i];
        __x = (__x > 1 ? 1 : (__x < 0 ? 0 : __x));
        out1[(Index)i] = 0.5 * (1 - rnbo_cos(6.28318530717958647692 * __x));
    }
}

void dspexpr_45_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void metertilde_03_perform(const SampleValue * input, Index n) {
    auto __metertilde_03_currentIntervalInSamples = this->metertilde_03_currentIntervalInSamples;
    auto __metertilde_03_currentInterval = this->metertilde_03_currentInterval;
    auto __metertilde_03_currentPeak = this->metertilde_03_currentPeak;

    for (Index i = 0; i < n; i++) {
        number value = (input[(Index)i] < 0 ? -input[(Index)i] : input[(Index)i]);

        if (value > __metertilde_03_currentPeak)
            __metertilde_03_currentPeak = value;
    }

    __metertilde_03_currentInterval -= n;

    if (__metertilde_03_currentInterval <= 0) {
        __metertilde_03_currentInterval = __metertilde_03_currentIntervalInSamples;

        this->getEngine()->sendNumMessage(
            TAG("peak"),
            TAG("meter~_obj-99"),
            __metertilde_03_currentPeak,
            this->_currentTime
        );;

        this->getEngine()->scheduleClockEventWithValue(
            this,
            1075784424,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            __metertilde_03_currentPeak
        );;

        __metertilde_03_currentPeak = 0;
    }

    this->metertilde_03_currentPeak = __metertilde_03_currentPeak;
    this->metertilde_03_currentInterval = __metertilde_03_currentInterval;
}

void dspexpr_42_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void dspexpr_46_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] * in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void metertilde_04_perform(const SampleValue * input, Index n) {
    auto __metertilde_04_currentIntervalInSamples = this->metertilde_04_currentIntervalInSamples;
    auto __metertilde_04_currentInterval = this->metertilde_04_currentInterval;
    auto __metertilde_04_currentPeak = this->metertilde_04_currentPeak;

    for (Index i = 0; i < n; i++) {
        number value = (input[(Index)i] < 0 ? -input[(Index)i] : input[(Index)i]);

        if (value > __metertilde_04_currentPeak)
            __metertilde_04_currentPeak = value;
    }

    __metertilde_04_currentInterval -= n;

    if (__metertilde_04_currentInterval <= 0) {
        __metertilde_04_currentInterval = __metertilde_04_currentIntervalInSamples;

        this->getEngine()->sendNumMessage(
            TAG("peak"),
            TAG("meter~_obj-98"),
            __metertilde_04_currentPeak,
            this->_currentTime
        );;

        this->getEngine()->scheduleClockEventWithValue(
            this,
            1777613927,
            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
            __metertilde_04_currentPeak
        );;

        __metertilde_04_currentPeak = 0;
    }

    this->metertilde_04_currentPeak = __metertilde_04_currentPeak;
    this->metertilde_04_currentInterval = __metertilde_04_currentInterval;
}

void dspexpr_44_perform(const Sample * in1, const Sample * in2, SampleValue * out1, Index n) {
    Index i;

    for (i = 0; i < n; i++) {
        out1[(Index)i] = in1[(Index)i] + in2[(Index)i];//#map:_###_obj_###_:1
    }
}

void p_04_perform(
    const SampleValue * in1,
    const SampleValue * in2,
    SampleValue * out1,
    SampleValue * out2,
    Index n
) {
    // subpatcher: EQ&Volume~
    ConstSampleArray<2> ins = {in1, in2};

    SampleArray<2> outs = {out1, out2};
    this->p_04->process(ins, 2, outs, 2, n);
}

void signaladder_01_perform(
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

void signaladder_02_perform(
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

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

void data_01_srout_set(number ) {}

void data_01_chanout_set(number ) {}

void data_01_sizeout_set(number v) {
    this->data_01_sizeout = v;
}

void data_02_srout_set(number ) {}

void data_02_chanout_set(number ) {}

void data_02_sizeout_set(number v) {
    this->data_02_sizeout = v;
}

void numberobj_20_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < -12) {
        localvalue = -12;
    }

    if ((bool)(true) && localvalue > 12) {
        localvalue = 12;
    }

    if (this->numberobj_20_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_20_value = localvalue;
}

void numberobj_21_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_21_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_21_value = localvalue;
}

void numberobj_22_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_22_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_22_value = localvalue;
}

void toggle_07_value_number_setter(number v) {
    this->toggle_07_value_number = (v != 0 ? 1 : 0);
}

void numberobj_23_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_23_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_23_value = localvalue;
}

void toggle_08_value_number_setter(number v) {
    this->toggle_08_value_number = (v != 0 ? 1 : 0);
}

void numberobj_24_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_24_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_24_value = localvalue;
}

void numberobj_25_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_25_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_25_value = localvalue;
}

void numberobj_26_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_26_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_26_value = localvalue;
}

void numberobj_27_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < -100) {
        localvalue = -100;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_27_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_27_value = localvalue;
}

void numberobj_28_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_28_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_28_value = localvalue;
}

void numberobj_29_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_29_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_29_value = localvalue;
}

void toggle_09_value_number_setter(number v) {
    this->toggle_09_value_number = (v != 0 ? 1 : 0);
}

void numberobj_30_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_30_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_30_value = localvalue;
}

void numberobj_31_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_31_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_31_value = localvalue;
}

void numberobj_32_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_32_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_32_value = localvalue;
}

void numberobj_33_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_33_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_33_value = localvalue;
}

void numberobj_34_value_setter(number v) {
    number localvalue = v;

    if ((bool)(true) && localvalue < 0) {
        localvalue = 0;
    }

    if ((bool)(true) && localvalue > 100) {
        localvalue = 100;
    }

    if (this->numberobj_34_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_34_value = localvalue;
}

void numberobj_35_value_setter(number v) {
    number localvalue = v;

    if (this->numberobj_35_currentFormat != 6) {
        localvalue = trunc(localvalue);
    }

    this->numberobj_35_value = localvalue;
}

void toggle_10_value_number_setter(number v) {
    this->toggle_10_value_number = (v != 0 ? 1 : 0);
}

void message_11_init() {
    this->message_11_set_set({1});
}

void numberobj_20_init() {
    this->numberobj_20_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-135"), 1, this->_currentTime);
}

void numberobj_20_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_20_value;
}

void numberobj_20_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_20_value_set(preset["value"]);
}

void param_01_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_01_value;
}

void param_01_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_01_value_set(preset["value"]);
}

number numbertilde_01_smooth_d_next(number x) {
    number temp = (number)(x - this->numbertilde_01_smooth_d_prev);
    this->numbertilde_01_smooth_d_prev = x;
    return temp;
}

void numbertilde_01_smooth_d_dspsetup() {
    this->numbertilde_01_smooth_d_reset();
}

void numbertilde_01_smooth_d_reset() {
    this->numbertilde_01_smooth_d_prev = 0;
}

number numbertilde_01_smooth_next(number x, number up, number down) {
    if (this->numbertilde_01_smooth_d_next(x) != 0.) {
        if (x > this->numbertilde_01_smooth_prev) {
            number _up = up;

            if (_up < 1)
                _up = 1;

            this->numbertilde_01_smooth_index = _up;
            this->numbertilde_01_smooth_increment = (x - this->numbertilde_01_smooth_prev) / _up;
        } else if (x < this->numbertilde_01_smooth_prev) {
            number _down = down;

            if (_down < 1)
                _down = 1;

            this->numbertilde_01_smooth_index = _down;
            this->numbertilde_01_smooth_increment = (x - this->numbertilde_01_smooth_prev) / _down;
        }
    }

    if (this->numbertilde_01_smooth_index > 0) {
        this->numbertilde_01_smooth_prev += this->numbertilde_01_smooth_increment;
        this->numbertilde_01_smooth_index -= 1;
    } else {
        this->numbertilde_01_smooth_prev = x;
    }

    return this->numbertilde_01_smooth_prev;
}

void numbertilde_01_smooth_reset() {
    this->numbertilde_01_smooth_prev = 0;
    this->numbertilde_01_smooth_index = 0;
    this->numbertilde_01_smooth_increment = 0;
    this->numbertilde_01_smooth_d_reset();
}

void numbertilde_01_init() {
    this->numbertilde_01_currentMode = 1;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number~_obj-74"), 1, this->_currentTime);
}

void numbertilde_01_dspsetup(bool force) {
    if ((bool)(this->numbertilde_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->numbertilde_01_currentIntervalInSamples = this->mstosamps(100);
    this->numbertilde_01_currentInterval = this->numbertilde_01_currentIntervalInSamples;
    this->numbertilde_01_rampInSamples = this->mstosamps(this->numbertilde_01_ramp);
    this->numbertilde_01_setupDone = true;
    this->numbertilde_01_smooth_d_dspsetup();
}

void numberobj_21_init() {
    this->numberobj_21_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-110"), 1, this->_currentTime);
}

void numberobj_21_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_21_value;
}

void numberobj_21_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_21_value_set(preset["value"]);
}

void param_02_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_02_value;
}

void param_02_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_02_value_set(preset["value"]);
}

number phasor_01_ph_next(number freq, number reset) {
    RNBO_UNUSED(reset);
    number pincr = freq * this->phasor_01_ph_conv;

    if (this->phasor_01_ph_currentPhase < 0.)
        this->phasor_01_ph_currentPhase = 1. + this->phasor_01_ph_currentPhase;

    if (this->phasor_01_ph_currentPhase > 1.)
        this->phasor_01_ph_currentPhase = this->phasor_01_ph_currentPhase - 1.;

    number tmp = this->phasor_01_ph_currentPhase;
    this->phasor_01_ph_currentPhase += pincr;
    return tmp;
}

void phasor_01_ph_reset() {
    this->phasor_01_ph_currentPhase = 0;
}

void phasor_01_ph_dspsetup() {
    this->phasor_01_ph_conv = (number)1 / this->sr;
}

void phasor_01_dspsetup(bool force) {
    if ((bool)(this->phasor_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->phasor_01_conv = (number)1 / this->samplerate();
    this->phasor_01_setupDone = true;
    this->phasor_01_ph_dspsetup();
}

void metertilde_01_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("meter~_obj-96"), 1, this->_currentTime);
}

void metertilde_01_dspsetup(bool force) {
    if ((bool)(this->metertilde_01_setupDone) && (bool)(!(bool)(force)))
        return;

    this->metertilde_01_currentIntervalInSamples = this->mstosamps(50);
    this->metertilde_01_currentInterval = this->metertilde_01_currentIntervalInSamples;
    this->metertilde_01_setupDone = true;
}

void metertilde_02_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("meter~_obj-97"), 1, this->_currentTime);
}

void metertilde_02_dspsetup(bool force) {
    if ((bool)(this->metertilde_02_setupDone) && (bool)(!(bool)(force)))
        return;

    this->metertilde_02_currentIntervalInSamples = this->mstosamps(50);
    this->metertilde_02_currentInterval = this->metertilde_02_currentIntervalInSamples;
    this->metertilde_02_setupDone = true;
}

void numberobj_22_init() {
    this->numberobj_22_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-122"), 1, this->_currentTime);
}

void numberobj_22_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_22_value;
}

void numberobj_22_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_22_value_set(preset["value"]);
}

void param_03_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_03_value;
}

void param_03_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_03_value_set(preset["value"]);
}

void toggle_07_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->toggle_07_value_number;
}

void toggle_07_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->toggle_07_value_number_set(preset["value"]);
}

void param_04_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_04_value;
}

void param_04_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_04_value_set(preset["value"]);
}

void numberobj_23_init() {
    this->numberobj_23_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-103"), 1, this->_currentTime);
}

void numberobj_23_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_23_value;
}

void numberobj_23_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_23_value_set(preset["value"]);
}

void toggle_08_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->toggle_08_value_number;
}

void toggle_08_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->toggle_08_value_number_set(preset["value"]);
}

void numberobj_24_init() {
    this->numberobj_24_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-50"), 1, this->_currentTime);
}

void numberobj_24_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_24_value;
}

void numberobj_24_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_24_value_set(preset["value"]);
}

void param_05_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_05_value;
}

void param_05_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_05_value_set(preset["value"]);
}

void metertilde_03_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("meter~_obj-99"), 1, this->_currentTime);
}

void metertilde_03_dspsetup(bool force) {
    if ((bool)(this->metertilde_03_setupDone) && (bool)(!(bool)(force)))
        return;

    this->metertilde_03_currentIntervalInSamples = this->mstosamps(50);
    this->metertilde_03_currentInterval = this->metertilde_03_currentIntervalInSamples;
    this->metertilde_03_setupDone = true;
}

void numberobj_25_init() {
    this->numberobj_25_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-39"), 1, this->_currentTime);
}

void numberobj_25_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_25_value;
}

void numberobj_25_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_25_value_set(preset["value"]);
}

void param_06_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_06_value;
}

void param_06_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_06_value_set(preset["value"]);
}

void metertilde_04_init() {
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("meter~_obj-98"), 1, this->_currentTime);
}

void metertilde_04_dspsetup(bool force) {
    if ((bool)(this->metertilde_04_setupDone) && (bool)(!(bool)(force)))
        return;

    this->metertilde_04_currentIntervalInSamples = this->mstosamps(50);
    this->metertilde_04_currentInterval = this->metertilde_04_currentIntervalInSamples;
    this->metertilde_04_setupDone = true;
}

void numberobj_26_init() {
    this->numberobj_26_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-105"), 1, this->_currentTime);
}

void numberobj_26_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_26_value;
}

void numberobj_26_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_26_value_set(preset["value"]);
}

void numberobj_27_init() {
    this->numberobj_27_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-57"), 1, this->_currentTime);
}

void numberobj_27_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_27_value;
}

void numberobj_27_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_27_value_set(preset["value"]);
}

void param_07_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_07_value;
}

void param_07_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_07_value_set(preset["value"]);
}

void numberobj_28_init() {
    this->numberobj_28_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-153"), 1, this->_currentTime);
}

void numberobj_28_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_28_value;
}

void numberobj_28_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_28_value_set(preset["value"]);
}

void param_08_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_08_value;
}

void param_08_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_08_value_set(preset["value"]);
}

void numberobj_29_init() {
    this->numberobj_29_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-123"), 1, this->_currentTime);
}

void numberobj_29_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_29_value;
}

void numberobj_29_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_29_value_set(preset["value"]);
}

void data_01_init() {
    this->data_01_buffer->setWantsFill(true);
}

Index data_01_evaluateSizeExpr(number samplerate, number vectorsize) {
    RNBO_UNUSED(vectorsize);
    RNBO_UNUSED(samplerate);
    number size = 0;
    return (Index)(size);
}

void data_01_dspsetup(bool force) {
    if ((bool)(this->data_01_setupDone) && (bool)(!(bool)(force)))
        return;

    if (this->data_01_sizemode == 2) {
        this->data_01_buffer = this->data_01_buffer->setSize((Index)(this->mstosamps(this->data_01_sizems)));
        updateDataRef(this, this->data_01_buffer);
    } else if (this->data_01_sizemode == 3) {
        this->data_01_buffer = this->data_01_buffer->setSize(this->data_01_evaluateSizeExpr(this->samplerate(), this->vectorsize()));
        updateDataRef(this, this->data_01_buffer);
    }

    this->data_01_setupDone = true;
}

void data_01_bufferUpdated() {
    this->data_01_report();
}

void data_01_report() {
    this->data_01_srout_set(this->data_01_buffer->getSampleRate());
    this->data_01_chanout_set(this->data_01_buffer->getChannels());
    this->data_01_sizeout_set(this->data_01_buffer->getSize());
}

void data_02_init() {
    this->data_02_buffer->setWantsFill(true);
}

Index data_02_evaluateSizeExpr(number samplerate, number vectorsize) {
    RNBO_UNUSED(vectorsize);
    RNBO_UNUSED(samplerate);
    number size = 0;
    return (Index)(size);
}

void data_02_dspsetup(bool force) {
    if ((bool)(this->data_02_setupDone) && (bool)(!(bool)(force)))
        return;

    if (this->data_02_sizemode == 2) {
        this->data_02_buffer = this->data_02_buffer->setSize((Index)(this->mstosamps(this->data_02_sizems)));
        updateDataRef(this, this->data_02_buffer);
    } else if (this->data_02_sizemode == 3) {
        this->data_02_buffer = this->data_02_buffer->setSize(this->data_02_evaluateSizeExpr(this->samplerate(), this->vectorsize()));
        updateDataRef(this, this->data_02_buffer);
    }

    this->data_02_setupDone = true;
}

void data_02_bufferUpdated() {
    this->data_02_report();
}

void data_02_report() {
    this->data_02_srout_set(this->data_02_buffer->getSampleRate());
    this->data_02_chanout_set(this->data_02_buffer->getChannels());
    this->data_02_sizeout_set(this->data_02_buffer->getSize());
}

void toggle_09_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->toggle_09_value_number;
}

void toggle_09_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->toggle_09_value_number_set(preset["value"]);
}

void numberobj_30_init() {
    this->numberobj_30_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-182"), 1, this->_currentTime);
}

void numberobj_30_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_30_value;
}

void numberobj_30_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_30_value_set(preset["value"]);
}

void param_09_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_09_value;
}

void param_09_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_09_value_set(preset["value"]);
}

void numberobj_31_init() {
    this->numberobj_31_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-184"), 1, this->_currentTime);
}

void numberobj_31_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_31_value;
}

void numberobj_31_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_31_value_set(preset["value"]);
}

void numberobj_32_init() {
    this->numberobj_32_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-174"), 1, this->_currentTime);
}

void numberobj_32_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_32_value;
}

void numberobj_32_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_32_value_set(preset["value"]);
}

void param_10_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_10_value;
}

void param_10_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_10_value_set(preset["value"]);
}

void numberobj_33_init() {
    this->numberobj_33_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-176"), 1, this->_currentTime);
}

void numberobj_33_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_33_value;
}

void numberobj_33_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_33_value_set(preset["value"]);
}

void numberobj_34_init() {
    this->numberobj_34_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-161"), 1, this->_currentTime);
}

void numberobj_34_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_34_value;
}

void numberobj_34_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_34_value_set(preset["value"]);
}

void param_11_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_11_value;
}

void param_11_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_11_value_set(preset["value"]);
}

void numberobj_35_init() {
    this->numberobj_35_currentFormat = 6;
    this->getEngine()->sendNumMessage(TAG("setup"), TAG("number_obj-163"), 1, this->_currentTime);
}

void numberobj_35_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->numberobj_35_value;
}

void numberobj_35_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->numberobj_35_value_set(preset["value"]);
}

void toggle_10_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->toggle_10_value_number;
}

void toggle_10_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->toggle_10_value_number_set(preset["value"]);
}

void param_12_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_12_value;
}

void param_12_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_12_value_set(preset["value"]);
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
    p_02_target = 0;
    numberobj_20_value = 0;
    numberobj_20_value_setter(numberobj_20_value);
    param_01_value = 0;
    numbertilde_01_input_number = 0;
    numbertilde_01_ramp = 0;
    numberobj_21_value = 0;
    numberobj_21_value_setter(numberobj_21_value);
    param_02_value = 0;
    p_03_target = 0;
    dspexpr_41_in1 = 0;
    dspexpr_41_in2 = 0;
    phasor_01_freq = 5;
    numberobj_22_value = 0;
    numberobj_22_value_setter(numberobj_22_value);
    param_03_value = 0;
    p_04_target = 0;
    dspexpr_42_in1 = 0;
    dspexpr_42_in2 = 0;
    dspexpr_43_in1 = 0;
    dspexpr_43_in2 = 0;
    p_05_target = 0;
    toggle_07_value_number = 0;
    toggle_07_value_number_setter(toggle_07_value_number);
    param_04_value = 0;
    numberobj_23_value = 0;
    numberobj_23_value_setter(numberobj_23_value);
    linetilde_06_time = 1000;
    linetilde_06_keepramp = 0;
    hann_tilde_03_x = 0;
    dspexpr_44_in1 = 0;
    dspexpr_44_in2 = 0;
    p_06_target = 0;
    toggle_08_value_number = 0;
    toggle_08_value_number_setter(toggle_08_value_number);
    dspexpr_45_in1 = 0;
    dspexpr_45_in2 = 0;
    recordtilde_01_record = 0;
    recordtilde_01_begin = 0;
    recordtilde_01_end = -1;
    recordtilde_01_loop = 0;
    numberobj_24_value = 0;
    numberobj_24_value_setter(numberobj_24_value);
    param_05_value = 0;
    numberobj_25_value = 0;
    numberobj_25_value_setter(numberobj_25_value);
    param_06_value = 0;
    dspexpr_46_in1 = 0;
    dspexpr_46_in2 = 0;
    numberobj_26_value = 0;
    numberobj_26_value_setter(numberobj_26_value);
    linetilde_07_time = 1000;
    linetilde_07_keepramp = 0;
    hann_tilde_04_x = 0;
    numberobj_27_value = 0;
    numberobj_27_value_setter(numberobj_27_value);
    param_07_value = 0;
    scale_07_inlow = 0;
    scale_07_inhigh = 100;
    scale_07_outlow = 250;
    scale_07_outhigh = 5000;
    scale_07_power = 1.3313;
    numberobj_28_value = 0;
    numberobj_28_value_setter(numberobj_28_value);
    param_08_value = 25;
    numberobj_29_value = 0;
    numberobj_29_value_setter(numberobj_29_value);
    data_01_sizeout = 0;
    data_01_size = 0;
    data_01_sizems = 200;
    data_01_normalize = 0.995;
    data_01_channels = 2;
    data_02_sizeout = 0;
    data_02_size = 0;
    data_02_sizems = 200;
    data_02_normalize = 0.995;
    data_02_channels = 2;
    toggle_09_value_number = 0;
    toggle_09_value_number_setter(toggle_09_value_number);
    recordtilde_02_record = 0;
    recordtilde_02_begin = 0;
    recordtilde_02_end = -1;
    recordtilde_02_loop = 0;
    p_07_target = 0;
    send_01_input_number = 0;
    scale_08_inlow = 0;
    scale_08_inhigh = 100;
    scale_08_outlow = 0;
    scale_08_outhigh = 0.025;
    scale_08_power = 1.75;
    numberobj_30_value = 0;
    numberobj_30_value_setter(numberobj_30_value);
    param_09_value = 25;
    numberobj_31_value = 0;
    numberobj_31_value_setter(numberobj_31_value);
    send_02_input_number = 0;
    scale_09_inlow = 0;
    scale_09_inhigh = 100;
    scale_09_outlow = 0;
    scale_09_outhigh = 1;
    scale_09_power = 1.66;
    numberobj_32_value = 0;
    numberobj_32_value_setter(numberobj_32_value);
    param_10_value = 25;
    numberobj_33_value = 0;
    numberobj_33_value_setter(numberobj_33_value);
    send_03_input_number = 0;
    scale_10_inlow = 0;
    scale_10_inhigh = 100;
    scale_10_outlow = 0;
    scale_10_outhigh = 1;
    scale_10_power = 1.66;
    numberobj_34_value = 0;
    numberobj_34_value_setter(numberobj_34_value);
    param_11_value = 25;
    numberobj_35_value = 0;
    numberobj_35_value_setter(numberobj_35_value);
    toggle_10_value_number = 0;
    toggle_10_value_number_setter(toggle_10_value_number);
    param_12_value = 1;
    ctlin_01_input = 0;
    ctlin_01_controller = 4;
    ctlin_01_channel = -1;
    expr_10_in1 = 0;
    expr_10_in2 = 0.007874015748;
    expr_10_out1 = 0;
    ctlin_02_input = 0;
    ctlin_02_controller = 2;
    ctlin_02_channel = -1;
    expr_11_in1 = 0;
    expr_11_in2 = 0.007874015748;
    expr_11_out1 = 0;
    ctlin_03_input = 0;
    ctlin_03_controller = 3;
    ctlin_03_channel = -1;
    expr_12_in1 = 0;
    expr_12_in2 = 0.007874015748;
    expr_12_out1 = 0;
    ctlin_04_input = 0;
    ctlin_04_controller = 1;
    ctlin_04_channel = -1;
    expr_13_in1 = 0;
    expr_13_in2 = 0.007874015748;
    expr_13_out1 = 0;
    ctlin_05_input = 0;
    ctlin_05_controller = 9;
    ctlin_05_channel = -1;
    expr_14_in1 = 0;
    expr_14_in2 = 0.007874015748;
    expr_14_out1 = 0;
    ctlin_06_input = 0;
    ctlin_06_controller = 10;
    ctlin_06_channel = -1;
    expr_15_in1 = 0;
    expr_15_in2 = 0.007874015748;
    expr_15_out1 = 0;
    ctlin_07_input = 0;
    ctlin_07_controller = 12;
    ctlin_07_channel = -1;
    expr_16_in1 = 0;
    expr_16_in2 = 0.007874015748;
    expr_16_out1 = 0;
    ctlin_08_input = 0;
    ctlin_08_controller = 5;
    ctlin_08_channel = -1;
    expr_17_in1 = 0;
    expr_17_in2 = 0.007874015748;
    expr_17_out1 = 0;
    ctlin_09_input = 0;
    ctlin_09_controller = 6;
    ctlin_09_channel = -1;
    expr_18_in1 = 0;
    expr_18_in2 = 0.007874015748;
    expr_18_out1 = 0;
    ctlin_10_input = 0;
    ctlin_10_controller = 8;
    ctlin_10_channel = -1;
    expr_19_in1 = 0;
    expr_19_in2 = 0.007874015748;
    expr_19_out1 = 0;
    ctlin_11_input = 0;
    ctlin_11_controller = 7;
    ctlin_11_channel = -1;
    expr_20_in1 = 0;
    expr_20_in2 = 0.007874015748;
    expr_20_out1 = 0;
    ctlin_12_input = 0;
    ctlin_12_controller = 11;
    ctlin_12_channel = -1;
    expr_21_in1 = 0;
    expr_21_in2 = 0.007874015748;
    expr_21_out1 = 0;
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
    numberobj_20_currentFormat = 6;
    numberobj_20_lastValue = 0;
    param_01_lastValue = 0;
    numbertilde_01_currentInterval = 0;
    numbertilde_01_currentIntervalInSamples = 0;
    numbertilde_01_lastValue = 0;
    numbertilde_01_outValue = 0;
    numbertilde_01_rampInSamples = 0;
    numbertilde_01_currentMode = 0;
    numbertilde_01_smooth_d_prev = 0;
    numbertilde_01_smooth_prev = 0;
    numbertilde_01_smooth_index = 0;
    numbertilde_01_smooth_increment = 0;
    numbertilde_01_setupDone = false;
    numberobj_21_currentFormat = 6;
    numberobj_21_lastValue = 0;
    param_02_lastValue = 0;
    phasor_01_sigbuf = nullptr;
    phasor_01_lastLockedPhase = 0;
    phasor_01_conv = 0;
    phasor_01_ph_currentPhase = 0;
    phasor_01_ph_conv = 0;
    phasor_01_setupDone = false;
    metertilde_01_currentPeak = 0;
    metertilde_01_currentInterval = 0;
    metertilde_01_currentIntervalInSamples = 0;
    metertilde_01_setupDone = false;
    metertilde_02_currentPeak = 0;
    metertilde_02_currentInterval = 0;
    metertilde_02_currentIntervalInSamples = 0;
    metertilde_02_setupDone = false;
    numberobj_22_currentFormat = 6;
    numberobj_22_lastValue = 0;
    param_03_lastValue = 0;
    toggle_07_lastValue = 0;
    param_04_lastValue = 0;
    numberobj_23_currentFormat = 6;
    numberobj_23_lastValue = 0;
    linetilde_06_currentValue = 0;
    toggle_08_lastValue = 0;
    recordtilde_01_wIndex = 0;
    recordtilde_01_lastRecord = 0;
    numberobj_24_currentFormat = 6;
    numberobj_24_lastValue = 0;
    param_05_lastValue = 0;
    metertilde_03_currentPeak = 0;
    metertilde_03_currentInterval = 0;
    metertilde_03_currentIntervalInSamples = 0;
    metertilde_03_setupDone = false;
    numberobj_25_currentFormat = 6;
    numberobj_25_lastValue = 0;
    param_06_lastValue = 0;
    metertilde_04_currentPeak = 0;
    metertilde_04_currentInterval = 0;
    metertilde_04_currentIntervalInSamples = 0;
    metertilde_04_setupDone = false;
    numberobj_26_currentFormat = 6;
    numberobj_26_lastValue = 0;
    linetilde_07_currentValue = 0;
    numberobj_27_currentFormat = 6;
    numberobj_27_lastValue = 0;
    param_07_lastValue = 0;
    numberobj_28_currentFormat = 6;
    numberobj_28_lastValue = 0;
    param_08_lastValue = 0;
    numberobj_29_currentFormat = 6;
    numberobj_29_lastValue = 0;
    data_01_sizemode = 2;
    data_01_setupDone = false;
    data_02_sizemode = 2;
    data_02_setupDone = false;
    toggle_09_lastValue = 0;
    recordtilde_02_wIndex = 0;
    recordtilde_02_lastRecord = 0;
    numberobj_30_currentFormat = 6;
    numberobj_30_lastValue = 0;
    param_09_lastValue = 0;
    numberobj_31_currentFormat = 6;
    numberobj_31_lastValue = 0;
    numberobj_32_currentFormat = 6;
    numberobj_32_lastValue = 0;
    param_10_lastValue = 0;
    numberobj_33_currentFormat = 6;
    numberobj_33_lastValue = 0;
    numberobj_34_currentFormat = 6;
    numberobj_34_lastValue = 0;
    param_11_lastValue = 0;
    numberobj_35_currentFormat = 6;
    numberobj_35_lastValue = 0;
    toggle_10_lastValue = 0;
    param_12_lastValue = 0;
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
    ctlin_07_status = 0;
    ctlin_07_byte1 = -1;
    ctlin_07_inchan = 0;
    ctlin_08_status = 0;
    ctlin_08_byte1 = -1;
    ctlin_08_inchan = 0;
    ctlin_09_status = 0;
    ctlin_09_byte1 = -1;
    ctlin_09_inchan = 0;
    ctlin_10_status = 0;
    ctlin_10_byte1 = -1;
    ctlin_10_inchan = 0;
    ctlin_11_status = 0;
    ctlin_11_byte1 = -1;
    ctlin_11_inchan = 0;
    ctlin_12_status = 0;
    ctlin_12_byte1 = -1;
    ctlin_12_inchan = 0;
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

    list message_11_set;
    number p_01_target;
    number p_02_target;
    number numberobj_20_value;
    number param_01_value;
    number numbertilde_01_input_number;
    number numbertilde_01_ramp;
    number numberobj_21_value;
    number param_02_value;
    number p_03_target;
    number dspexpr_41_in1;
    number dspexpr_41_in2;
    number phasor_01_freq;
    number numberobj_22_value;
    number param_03_value;
    number p_04_target;
    number dspexpr_42_in1;
    number dspexpr_42_in2;
    number dspexpr_43_in1;
    number dspexpr_43_in2;
    number p_05_target;
    number toggle_07_value_number;
    number param_04_value;
    number numberobj_23_value;
    list linetilde_06_segments;
    number linetilde_06_time;
    number linetilde_06_keepramp;
    number hann_tilde_03_x;
    number dspexpr_44_in1;
    number dspexpr_44_in2;
    number p_06_target;
    number toggle_08_value_number;
    number dspexpr_45_in1;
    number dspexpr_45_in2;
    number recordtilde_01_record;
    number recordtilde_01_begin;
    number recordtilde_01_end;
    number recordtilde_01_loop;
    number numberobj_24_value;
    number param_05_value;
    number numberobj_25_value;
    number param_06_value;
    number dspexpr_46_in1;
    number dspexpr_46_in2;
    number numberobj_26_value;
    list linetilde_07_segments;
    number linetilde_07_time;
    number linetilde_07_keepramp;
    number hann_tilde_04_x;
    number numberobj_27_value;
    number param_07_value;
    list scale_07_input;
    number scale_07_inlow;
    number scale_07_inhigh;
    number scale_07_outlow;
    number scale_07_outhigh;
    number scale_07_power;
    list scale_07_out;
    number numberobj_28_value;
    number param_08_value;
    number numberobj_29_value;
    number data_01_sizeout;
    number data_01_size;
    number data_01_sizems;
    number data_01_normalize;
    number data_01_channels;
    number data_02_sizeout;
    number data_02_size;
    number data_02_sizems;
    number data_02_normalize;
    number data_02_channels;
    number toggle_09_value_number;
    number recordtilde_02_record;
    number recordtilde_02_begin;
    number recordtilde_02_end;
    number recordtilde_02_loop;
    number p_07_target;
    number send_01_input_number;
    list send_01_input_list;
    list scale_08_input;
    number scale_08_inlow;
    number scale_08_inhigh;
    number scale_08_outlow;
    number scale_08_outhigh;
    number scale_08_power;
    list scale_08_out;
    number numberobj_30_value;
    number param_09_value;
    number numberobj_31_value;
    number send_02_input_number;
    list send_02_input_list;
    list scale_09_input;
    number scale_09_inlow;
    number scale_09_inhigh;
    number scale_09_outlow;
    number scale_09_outhigh;
    number scale_09_power;
    list scale_09_out;
    number numberobj_32_value;
    number param_10_value;
    number numberobj_33_value;
    number send_03_input_number;
    list send_03_input_list;
    list scale_10_input;
    number scale_10_inlow;
    number scale_10_inhigh;
    number scale_10_outlow;
    number scale_10_outhigh;
    number scale_10_power;
    list scale_10_out;
    number numberobj_34_value;
    number param_11_value;
    number numberobj_35_value;
    number toggle_10_value_number;
    number param_12_value;
    number ctlin_01_input;
    number ctlin_01_controller;
    number ctlin_01_channel;
    number expr_10_in1;
    number expr_10_in2;
    number expr_10_out1;
    number ctlin_02_input;
    number ctlin_02_controller;
    number ctlin_02_channel;
    number expr_11_in1;
    number expr_11_in2;
    number expr_11_out1;
    number ctlin_03_input;
    number ctlin_03_controller;
    number ctlin_03_channel;
    number expr_12_in1;
    number expr_12_in2;
    number expr_12_out1;
    number ctlin_04_input;
    number ctlin_04_controller;
    number ctlin_04_channel;
    number expr_13_in1;
    number expr_13_in2;
    number expr_13_out1;
    number ctlin_05_input;
    number ctlin_05_controller;
    number ctlin_05_channel;
    number expr_14_in1;
    number expr_14_in2;
    number expr_14_out1;
    number ctlin_06_input;
    number ctlin_06_controller;
    number ctlin_06_channel;
    number expr_15_in1;
    number expr_15_in2;
    number expr_15_out1;
    number ctlin_07_input;
    number ctlin_07_controller;
    number ctlin_07_channel;
    number expr_16_in1;
    number expr_16_in2;
    number expr_16_out1;
    number ctlin_08_input;
    number ctlin_08_controller;
    number ctlin_08_channel;
    number expr_17_in1;
    number expr_17_in2;
    number expr_17_out1;
    number ctlin_09_input;
    number ctlin_09_controller;
    number ctlin_09_channel;
    number expr_18_in1;
    number expr_18_in2;
    number expr_18_out1;
    number ctlin_10_input;
    number ctlin_10_controller;
    number ctlin_10_channel;
    number expr_19_in1;
    number expr_19_in2;
    number expr_19_out1;
    number ctlin_11_input;
    number ctlin_11_controller;
    number ctlin_11_channel;
    number expr_20_in1;
    number expr_20_in2;
    number expr_20_out1;
    number ctlin_12_input;
    number ctlin_12_controller;
    number ctlin_12_channel;
    number expr_21_in1;
    number expr_21_in2;
    number expr_21_out1;
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
    Int numberobj_20_currentFormat;
    number numberobj_20_lastValue;
    number param_01_lastValue;
    SampleIndex numbertilde_01_currentInterval;
    SampleIndex numbertilde_01_currentIntervalInSamples;
    number numbertilde_01_lastValue;
    number numbertilde_01_outValue;
    number numbertilde_01_rampInSamples;
    Int numbertilde_01_currentMode;
    number numbertilde_01_smooth_d_prev;
    number numbertilde_01_smooth_prev;
    number numbertilde_01_smooth_index;
    number numbertilde_01_smooth_increment;
    bool numbertilde_01_setupDone;
    Int numberobj_21_currentFormat;
    number numberobj_21_lastValue;
    number param_02_lastValue;
    signal phasor_01_sigbuf;
    number phasor_01_lastLockedPhase;
    number phasor_01_conv;
    number phasor_01_ph_currentPhase;
    number phasor_01_ph_conv;
    bool phasor_01_setupDone;
    number metertilde_01_currentPeak;
    SampleIndex metertilde_01_currentInterval;
    SampleIndex metertilde_01_currentIntervalInSamples;
    bool metertilde_01_setupDone;
    number metertilde_02_currentPeak;
    SampleIndex metertilde_02_currentInterval;
    SampleIndex metertilde_02_currentIntervalInSamples;
    bool metertilde_02_setupDone;
    Int numberobj_22_currentFormat;
    number numberobj_22_lastValue;
    number param_03_lastValue;
    number toggle_07_lastValue;
    number param_04_lastValue;
    Int numberobj_23_currentFormat;
    number numberobj_23_lastValue;
    list linetilde_06_activeRamps;
    number linetilde_06_currentValue;
    number toggle_08_lastValue;
    Float64BufferRef recordtilde_01_buffer;
    SampleIndex recordtilde_01_wIndex;
    number recordtilde_01_lastRecord;
    Int numberobj_24_currentFormat;
    number numberobj_24_lastValue;
    number param_05_lastValue;
    number metertilde_03_currentPeak;
    SampleIndex metertilde_03_currentInterval;
    SampleIndex metertilde_03_currentIntervalInSamples;
    bool metertilde_03_setupDone;
    Int numberobj_25_currentFormat;
    number numberobj_25_lastValue;
    number param_06_lastValue;
    number metertilde_04_currentPeak;
    SampleIndex metertilde_04_currentInterval;
    SampleIndex metertilde_04_currentIntervalInSamples;
    bool metertilde_04_setupDone;
    Int numberobj_26_currentFormat;
    number numberobj_26_lastValue;
    list linetilde_07_activeRamps;
    number linetilde_07_currentValue;
    Int numberobj_27_currentFormat;
    number numberobj_27_lastValue;
    number param_07_lastValue;
    Int numberobj_28_currentFormat;
    number numberobj_28_lastValue;
    number param_08_lastValue;
    Int numberobj_29_currentFormat;
    number numberobj_29_lastValue;
    Float64BufferRef data_01_buffer;
    Int data_01_sizemode;
    bool data_01_setupDone;
    Float64BufferRef data_02_buffer;
    Int data_02_sizemode;
    bool data_02_setupDone;
    number toggle_09_lastValue;
    Float64BufferRef recordtilde_02_buffer;
    SampleIndex recordtilde_02_wIndex;
    number recordtilde_02_lastRecord;
    Int numberobj_30_currentFormat;
    number numberobj_30_lastValue;
    number param_09_lastValue;
    Int numberobj_31_currentFormat;
    number numberobj_31_lastValue;
    Int numberobj_32_currentFormat;
    number numberobj_32_lastValue;
    number param_10_lastValue;
    Int numberobj_33_currentFormat;
    number numberobj_33_lastValue;
    Int numberobj_34_currentFormat;
    number numberobj_34_lastValue;
    number param_11_lastValue;
    Int numberobj_35_currentFormat;
    number numberobj_35_lastValue;
    number toggle_10_lastValue;
    number param_12_lastValue;
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
    int ctlin_07_status;
    int ctlin_07_byte1;
    int ctlin_07_inchan;
    int ctlin_08_status;
    int ctlin_08_byte1;
    int ctlin_08_inchan;
    int ctlin_09_status;
    int ctlin_09_byte1;
    int ctlin_09_inchan;
    int ctlin_10_status;
    int ctlin_10_byte1;
    int ctlin_10_inchan;
    int ctlin_11_status;
    int ctlin_11_byte1;
    int ctlin_11_inchan;
    int ctlin_12_status;
    int ctlin_12_byte1;
    int ctlin_12_inchan;
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
    DataRef FreezeA;
    DataRef RNBODefaultSinus;
    DataRef FreezeB;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;
    RNBOSubpatcher_47* p_01;
    RNBOSubpatcher_48* p_02;
    RNBOSubpatcher_49* p_03[8];
    RNBOSubpatcher_50* p_04;
    RNBOSubpatcher_51* p_05;
    RNBOSubpatcher_52* p_06[8];
    RNBOSubpatcher_53* p_07;

};

PatcherInterface* createfreezer()
{
    return new freezer();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr freezerFactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return createfreezer;
}

} // end RNBO namespace

