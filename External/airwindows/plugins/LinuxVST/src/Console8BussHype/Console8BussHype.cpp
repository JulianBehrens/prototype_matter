/* ========================================
 *  Console8BussHype - Console8BussHype.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console8BussHype_H
#include "Console8BussHype.h"
#endif

AudioEffect* createEffectInstance(audioMasterCallback audioMaster) {return new Console8BussHype(audioMaster);}

Console8BussHype::Console8BussHype(audioMasterCallback audioMaster) :
    AudioEffectX(audioMaster, kNumPrograms, kNumParameters)
{
	iirAL = 0.0; iirBL = 0.0; iirAR = 0.0; iirBR = 0.0;
	for (int x = 0; x < fix_total; x++) fix[x] = 0.0;
	for (int x = 0; x < 10; x++) {softL[x] = 0.0; softR[x] = 0.0;}
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	//this is reset: values being initialized only once. Startup values, whatever they are.
	
    _canDo.insert("plugAsChannelInsert"); // plug-in can be used as a channel insert effect.
    _canDo.insert("plugAsSend"); // plug-in can be used as a send effect.
    _canDo.insert("x2in2out"); 
    setNumInputs(kNumInputs);
    setNumOutputs(kNumOutputs);
    setUniqueID(kUniqueId);
    canProcessReplacing();     // supports output replacing
    canDoubleReplacing();      // supports double precision processing
	programsAreChunks(true);
    vst_strncpy (_programName, "Default", kVstMaxProgNameLen); // default program name
}

Console8BussHype::~Console8BussHype() {}
VstInt32 Console8BussHype::getVendorVersion () {return 1000;}
void Console8BussHype::setProgramName(char *name) {vst_strncpy (_programName, name, kVstMaxProgNameLen);}
void Console8BussHype::getProgramName(char *name) {vst_strncpy (name, _programName, kVstMaxProgNameLen);}
//airwindows likes to ignore this stuff. Make your own programs, and make a different plugin rather than
//trying to do versioning and preventing people from using older versions. Maybe they like the old one!

VstInt32 Console8BussHype::getChunk (void** data, bool isPreset)
{
	float *chunkData = (float *)calloc(kNumParameters, sizeof(float));
	return kNumParameters * sizeof(float);
}

VstInt32 Console8BussHype::setChunk (void* data, VstInt32 byteSize, bool isPreset)
{	
	return 0;
}

void Console8BussHype::setParameter(VstInt32 index, float value) {
    switch (index) {
        default: throw; // unknown parameter, shouldn't happen!
    }
}

float Console8BussHype::getParameter(VstInt32 index) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } return 0.0; //we only need to update the relevant name, this is simple to manage
}

void Console8BussHype::getParameterName(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
    } //this is our labels for displaying in the VST host
}

void Console8BussHype::getParameterDisplay(VstInt32 index, char *text) {
    switch (index) {
        default: break; // unknown parameter, shouldn't happen!
	} //this displays the values and handles 'popups' where it's discrete choices
}

void Console8BussHype::getParameterLabel(VstInt32 index, char *text) {
    switch (index) {
		default: break; // unknown parameter, shouldn't happen!
    }
}

VstInt32 Console8BussHype::canDo(char *text) 
{ return (_canDo.find(text) == _canDo.end()) ? -1: 1; } // 1 = yes, -1 = no, 0 = don't know

bool Console8BussHype::getEffectName(char* name) {
    vst_strncpy(name, "Console8BussHype", kVstMaxProductStrLen); return true;
}

VstPlugCategory Console8BussHype::getPlugCategory() {return kPlugCategEffect;}

bool Console8BussHype::getProductString(char* text) {
  	vst_strncpy (text, "airwindows Console8BussHype", kVstMaxProductStrLen); return true;
}

bool Console8BussHype::getVendorString(char* text) {
  	vst_strncpy (text, "airwindows", kVstMaxVendorStrLen); return true;
}
