/* ========================================
 *  PurestGain - PurestGain.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestGain_H
#include "PurestGain.h"
#endif

void PurestGain::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double inputgain = (A * 80.0)-40.0;
	if (settingchase != inputgain) {
		chasespeed *= 2.0;
		settingchase = inputgain;
		//increment the slowness for each fader movement
		//continuous alteration makes it react smoother
		//sudden jump to setting, not so much
	}
	if (chasespeed > 2500.0) chasespeed = 2500.0;
	//bail out if it's too extreme
	if (gainchase < -60.0) {
		gainchase = pow(10.0,inputgain/20.0);
		//shouldn't even be a negative number
		//this is about starting at whatever's set, when
		//plugin is instantiated.
		//Otherwise it's the target, in dB.
	}
	double targetgain;	
	//done with top controller
	double targetBgain = B;
	if (gainBchase < 0.0) gainBchase = targetBgain;
	//this one is not a dB value, but straight multiplication
	//done with slow fade controller
	double outputgain;
	

	double inputSampleL;
	double inputSampleR;
	
	//A is 0-1 (you can't feed other values to VST hosts, it's always 0-1 internally)
	//B is 0-1 and you need to multiply it by 100 if you want to use the 'percent'
	//C is 0-1 and if you can use a 0-1 value you can use it directly
	//D is 0-1 and you must set global parameters in PurestGain.SetParameter() to use it as a 'popup'
	//assign values here, possibly using const values as they won't change in this context
    
    while (--sampleFrames >= 0)
    {
		targetgain = pow(10.0,settingchase/20.0);
		//now we have the target in our temp variable
		
		chasespeed *= 0.9999;
		chasespeed -= 0.01;
		if (chasespeed < 350.0) chasespeed = 350.0;
		//we have our chase speed compensated for recent fader activity
		
		gainchase = (((gainchase*chasespeed)+targetgain)/(chasespeed+1.0));
		//gainchase is chasing the target, as a simple multiply gain factor
		
		gainBchase = (((gainBchase*4000)+targetBgain)/4001);
		//gainchase is chasing the target, as a simple multiply gain factor
		
		outputgain = gainchase * gainBchase;
		//directly multiply the dB gain by the straight multiply gain

		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		if (1.0 == outputgain)
		{
			*out1 = *in1;
			*out2 = *in2;
		} else {
			inputSampleL *= outputgain;
			inputSampleR *= outputgain;
			//begin 32 bit stereo floating point dither
			int expon; frexpf((float)inputSampleL, &expon);
			fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
			inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
			frexpf((float)inputSampleR, &expon);
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
			inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
			//end 32 bit stereo floating point dither
			*out1 = inputSampleL;
			*out2 = inputSampleR;
		}
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void PurestGain::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double inputgain = (A * 80.0)-40.0;
	if (settingchase != inputgain) {
		chasespeed *= 2.0;
		settingchase = inputgain;
		//increment the slowness for each fader movement
		//continuous alteration makes it react smoother
		//sudden jump to setting, not so much
	}
	if (chasespeed > 2500.0) chasespeed = 2500.0;
	//bail out if it's too extreme
	if (gainchase < -60.0) {
		gainchase = pow(10.0,inputgain/20.0);
		//shouldn't even be a negative number
		//this is about starting at whatever's set, when
		//plugin is instantiated.
		//Otherwise it's the target, in dB.
	}
	double targetgain;	
	//done with top controller
	double targetBgain = B;
	if (gainBchase < 0.0) gainBchase = targetBgain;
	//this one is not a dB value, but straight multiplication
	//done with slow fade controller
	double outputgain;	
	

	double inputSampleL;
	double inputSampleR;

    while (--sampleFrames >= 0)
    {
		targetgain = pow(10.0,settingchase/20.0);
		//now we have the target in our temp variable
		
		chasespeed *= 0.9999;
		chasespeed -= 0.01;
		if (chasespeed < 350.0) chasespeed = 350.0;
		//we have our chase speed compensated for recent fader activity
		
		gainchase = (((gainchase*chasespeed)+targetgain)/(chasespeed+1.0));
		//gainchase is chasing the target, as a simple multiply gain factor
		
		gainBchase = (((gainBchase*4000)+targetBgain)/4001);
		//gainchase is chasing the target, as a simple multiply gain factor
		
		outputgain = gainchase * gainBchase;
		//directly multiply the dB gain by the straight multiply gain
		
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (1.0 == outputgain)
		{
			*out1 = *in1;
			*out2 = *in2;
		} else {
			inputSampleL *= outputgain;
			inputSampleR *= outputgain;
			//begin 64 bit stereo floating point dither
			//int expon; frexp((double)inputSampleL, &expon);
			fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
			//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
			//frexp((double)inputSampleR, &expon);
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
			//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
			//end 64 bit stereo floating point dither
			*out1 = inputSampleL;
			*out2 = inputSampleR;
		}
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}