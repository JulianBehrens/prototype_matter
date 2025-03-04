/*
*	File:		ConsoleMCBuss.h
*	
*	Version:	1.0
* 
*	Created:	9/30/23
*	
*	Copyright:  Copyright � 2023 Airwindows, Airwindows uses the MIT license
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
#include "ConsoleMCBussVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __ConsoleMCBuss_h__
#define __ConsoleMCBuss_h__


#pragma mark ____ConsoleMCBuss Parameters

// parameters
static const float kDefaultValue_ParamOne = 1.0;
static CFStringRef kParameterOneName = CFSTR("Master");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	//Add your parameters here...
	kNumberOfParameters=1
};

#pragma mark ____ConsoleMCBuss
class ConsoleMCBuss : public AUEffectBase
{
public:
	ConsoleMCBuss(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~ConsoleMCBuss () { delete mDebugDispatcher; }
#endif
	
	virtual ComponentResult Reset(AudioUnitScope inScope, AudioUnitElement inElement);

	virtual OSStatus ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags, 
						const AudioBufferList & inBuffer, AudioBufferList & outBuffer, 
						UInt32 inFramesToProcess);
	virtual UInt32 SupportedNumChannels(const AUChannelInfo ** outInfo);

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
    virtual Float64				GetTailTime() {return (1.0/GetSampleRate())*0.0;} //in SECONDS! gsr * a number = in samples
    virtual Float64				GetLatency() {return (1.0/GetSampleRate())*0.0;}	// in SECONDS! gsr * a number = in samples
	
	/*! @method Version */
	virtual ComponentResult		Version() { return kConsoleMCBussVersion; }
	
	private: 
	enum {
		prevSampL1,
		prevSampR1,
		invSampL1,
		invSampR1,
		threshold1,
		prevSampL2,
		prevSampR2,
		invSampL2,
		invSampR2,
		threshold2,
		prevSampL3,
		prevSampR3,
		invSampL3,
		invSampR3,
		threshold3,
		prevSampL4,
		prevSampR4,
		invSampL4,
		invSampR4,
		threshold4,
		prevSampL5,
		prevSampR5,
		invSampL5,
		invSampR5,
		threshold5,
		prevSampL6,
		prevSampR6,
		invSampL6,
		invSampR6,
		threshold6,
		prevSampL7,
		prevSampR7,
		invSampL7,
		invSampR7,
		threshold7,
		prevSampL8,
		prevSampR8,
		invSampL8,
		invSampR8,
		threshold8,
		prevSampL9,
		prevSampR9,
		invSampL9,
		invSampR9,
		threshold9,
		prevSampL10,
		prevSampR10,
		invSampL10,
		invSampR10,
		threshold10,
		gslew_total
	}; //fixed frequency pear filter for ultrasonics, stereo
	double gslew[gslew_total]; //probably worth just using a number here
	
	double subAL;
	double subAR;
	double subBL;
	double subBR;
	double subCL;
	double subCR;
	double subDL;
	double subDR;
	
	double gainA;
	double gainB; //smoothed master fader for channel, from Z2 series filter code
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif