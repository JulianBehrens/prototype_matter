/* ========================================
 *  FathomFive - FathomFive.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __FathomFive_H
#define __FathomFive_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParamA = 0,
	kParamB = 1,
	kParamC = 2,
	kParamD = 3,
  kNumParameters = 4
}; //

const int kNumPrograms = 1;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'fatm';    //Change this to what the AU identity is!

class FathomFive : 
    public AudioEffectX 
{
public:
    FathomFive(audioMasterCallback audioMaster);
    ~FathomFive();
    virtual bool getEffectName(char* name);                       // The plug-in name
    virtual VstPlugCategory getPlugCategory();                    // The general category for the plug-in
    virtual bool getProductString(char* text);                    // This is a unique plug-in string provided by Steinberg
    virtual bool getVendorString(char* text);                     // Vendor info
    virtual VstInt32 getVendorVersion();                          // Version number
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
    virtual void getProgramName(char *name);                      // read the name from the host
    virtual void setProgramName(char *name);                      // changes the name of the preset displayed in the host
	virtual VstInt32 getChunk (void** data, bool isPreset);
	virtual VstInt32 setChunk (void* data, VstInt32 byteSize, bool isPreset);
	virtual float getParameter(VstInt32 index);                   // get the parameter value at the specified index
    virtual void setParameter(VstInt32 index, float value);       // set the parameter at index to value
    virtual void getParameterLabel(VstInt32 index, char *text);  // label for the parameter (eg dB)
    virtual void getParameterName(VstInt32 index, char *text);    // name of the parameter
    virtual void getParameterDisplay(VstInt32 index, char *text); // text description of the current value    
    virtual VstInt32 canDo(char *text);
private:
    char _programName[kVstMaxProgNameLen + 1];
    std::set< std::string > _canDo;
    	
	bool WasNegativeL;
	bool SubOctaveL;
	double iirSampleLA;
	double iirSampleLB;
	double iirSampleLC;
	double iirSampleLD;

	bool WasNegativeR;
	bool SubOctaveR;
	double iirSampleRA;
	double iirSampleRB;
	double iirSampleRC;
	double iirSampleRD;
	
    float A; //Root Note
    float B; //SubOctave
    float C; //Frequency
    float D; //Dry/Wet
	//parameters. Always 0-1, and we scale/alter them elsewhere.
	uint32_t fpdL;
	uint32_t fpdR;
};

#endif
