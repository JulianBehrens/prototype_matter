/*
*	File:		ResEQ.h
*	
*	Version:	1.0
* 
*	Created:	8/1/07
*	
*	Copyright:  Copyright � 2007 Airwindows, Airwindows uses the MIT license
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
#include "ResEQVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __ResEQ_h__
#define __ResEQ_h__


#pragma mark ____ResEQ Parameters

// parameters
static const float kDefaultValue_ParamOne = 0.0;
static const float kDefaultValue_ParamTwo = 0.0;
static const float kDefaultValue_ParamThree = 0.0;
static const float kDefaultValue_ParamFour = 0.0;
static const float kDefaultValue_ParamFive = 0.0;
static const float kDefaultValue_ParamSix = 0.0;
static const float kDefaultValue_ParamSeven = 0.0;
static const float kDefaultValue_ParamEight = 0.0;
static const float kDefaultValue_ParamNine = 1.0;

static CFStringRef kParameterOneName = CFSTR("Reso 1");
static CFStringRef kParameterTwoName = CFSTR("Reso 2");
static CFStringRef kParameterThreeName = CFSTR("Reso 3");
static CFStringRef kParameterFourName = CFSTR("Reso 4");
static CFStringRef kParameterFiveName = CFSTR("Reso 5");
static CFStringRef kParameterSixName = CFSTR("Reso 6");
static CFStringRef kParameterSevenName = CFSTR("Reso 7");
static CFStringRef kParameterEightName = CFSTR("Reso 8");
static CFStringRef kParameterNineName = CFSTR("Dry/Wet");

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	kParam_Five =4,
	kParam_Six =5,
	kParam_Seven =6,
	kParam_Eight =7,
	kParam_Nine =8,
	//Add your parameters here...
	kNumberOfParameters=9
};

#pragma mark ____ResEQ
class ResEQ : public AUEffectBase
{
public:
	ResEQ(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~ResEQ () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new ResEQKernel(this); }
	
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
	
	virtual ComponentResult    Initialize();
	virtual bool				SupportsTail () { return true; }
    virtual Float64				GetTailTime() {return 1.0/GetSampleRate()*60.0;}
    virtual Float64				GetLatency() {return 1.0/GetSampleRate()*1.0;}	// edit these because tail time isn't 1000 samples and latency isn't 1

	/*! @method Version */
	virtual ComponentResult	Version() { return kResEQVersion; }
	
    
	
protected:
		class ResEQKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		ResEQKernel(AUEffectBase *inAudioUnit )
		: AUKernelBase(inAudioUnit)
	{
	}
		
		// *Required* overides for the process method for this effect
		// processes one channel of interleaved samples
        virtual void 		Process(	const Float32 	*inSourceP,
										Float32		 	*inDestP,
										UInt32 			inFramesToProcess,
										UInt32			inNumChannels,
										bool			&ioSilence);
		
        virtual void		Reset();
		
		private: 
		Float64 b[61];
		Float64 f[61];
		int framenumber;
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif