/*
 *	File:		StarChild.h
 *	
 *	Version:	1.0 
 * 
 *	Created:	11/9/15
 *	
 *	Copyright:  Copyright � 2015 Airwindows, Airwindows uses the MIT license
 * 
 *	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in 
 *				consideration of your agreement to the following terms, and your use, installation, modification 
 *				or redistribution of this Apple software constitutes acceptance of these terms.  If you do 
 *				not agree with these terms, please do not use, install, modify or redistribute this Apple 
 *				software.
 *
 *				In consideration of your agreement to abide by the following terms, and subject to these terms, 
 *				Apple grants you a personal, non-exclusive license, under Apple's copyrights in this 
 *				original Apple software (the "Apple Software"), to use, reproduce, modify and redistribute the 
 *				Apple Software, with or without modifications, in source and/or binary forms; provided that if you 
 *				redistribute the Apple Software in its entirety and without modifications, you must retain this 
 *				notice and the following text and disclaimers in all such redistributions of the Apple Software. 
 *				Neither the name, trademarks, service marks or logos of Apple Computer, Inc. may be used to 
 *				endorse or promote products derived from the Apple Software without specific prior written 
 *				permission from Apple.  Except as expressly stated in this notice, no other rights or 
 *				licenses, express or implied, are granted by Apple herein, including but not limited to any 
 *				patent rights that may be infringed by your derivative works or by other works in which the 
 *				Apple Software may be incorporated.
 *
 *				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO WARRANTIES, EXPRESS OR 
 *				IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
 *				AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE 
 *				OR IN COMBINATION WITH YOUR PRODUCTS.
 *
 *				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
 *				DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 *				OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
 *				REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER 
 *				UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN 
 *				IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "AUEffectBase.h"
#include "StarChildVersion.h"

#if AU_DEBUG_DISPATCHER
#include "AUDebugDispatcher.h"
#endif


#ifndef __StarChild_h__
#define __StarChild_h__


#pragma mark ____StarChild Parameters

// parameters
static const float kDefaultValue_ParamOne = 1.0;
static const float kDefaultValue_ParamTwo = 0.7;
static const float kDefaultValue_ParamThree = 0.2;

static CFStringRef kParameterOneName = CFSTR("Sustain Duration");
static CFStringRef kParameterTwoName = CFSTR("Grain Coarse/Fine");
static CFStringRef kParameterThreeName = CFSTR("Dry/Wet");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	//Add your parameters here...
	kNumberOfParameters=3
};

#pragma mark ____StarChild
class StarChild : public AUEffectBase
{
public:
	StarChild(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~StarChild () { delete mDebugDispatcher; }
#endif
	
	virtual ComponentResult Reset(AudioUnitScope inScope, AudioUnitElement inElement);
	
	virtual OSStatus ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags, 
										const AudioBufferList & inBuffer, AudioBufferList & outBuffer, 
										UInt32 inFramesToProcess);
	
	virtual	ComponentResult		GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	ComponentResult		GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual ComponentResult		GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual ComponentResult		GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);
	
	virtual UInt32 SupportedNumChannels(const AUChannelInfo ** outInfo);
	
	virtual ComponentResult    Initialize();
	virtual bool				SupportsTail () { return true; }
    virtual Float64				GetTailTime() {return 1.0/GetSampleRate()*22050;} //roughly half seconds tail
    virtual Float64				GetLatency() {return 0.0;}	// edit these because tail time isn't 1000 samples and latency isn't 1
	
	/*! @method Version */
	virtual ComponentResult		Version() { return kStarChildVersion; }
	
private:
	Float32 d[45102];
	UInt32 dCount;
	
	UInt32 p[171];
	UInt32 t[171];
	Float32 outL[171];
	Float32 outR[171];
	UInt32 pitchCounter;
	UInt32 increment;
	UInt32 dutyCycle;
	
	Float64 wearL[11];
	Float64 wearR[11];
	Float64 factor[11];		
	Float64 wearLPrev;
	Float64 wearRPrev;
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif