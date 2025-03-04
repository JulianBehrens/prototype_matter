/* ========================================
 *  PurestWarm2 - PurestWarm2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#include "PurestWarm2.h"
#endif

void PurestWarm2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double pos = A;
	double neg = B;
	double cutoff = 25000.0 / getSampleRate();
	if (cutoff > 0.49) cutoff = 0.49; //don't crash if run at 44.1k
	fixA[fix_freq] = cutoff;
	fixA[fix_reso] = 0.70710678; //butterworth Q
	double K = tan(M_PI * fixA[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fixA[fix_reso] + K * K);
	fixA[fix_a0] = K * K * norm;
	fixA[fix_a1] = 2.0 * fixA[fix_a0];
	fixA[fix_a2] = fixA[fix_a0];
	fixA[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixA[fix_b2] = (1.0 - K / fixA[fix_reso] + K * K) * norm;
	fixA[fix_sL1] = 0.0;
	fixA[fix_sL2] = 0.0;
	fixA[fix_sR1] = 0.0;
	fixA[fix_sR2] = 0.0;
	//define filters here: on VST you can't define them in reset 'cos getSampleRate isn't returning good information yet
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double outSample = (inputSampleL * fixA[fix_a0]) + fixA[fix_sL1];
		fixA[fix_sL1] = (inputSampleL * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sL2];
		fixA[fix_sL2] = (inputSampleL * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		
		if (inputSampleL > 0) inputSampleL = (sin(inputSampleL*1.57079634*pos)/1.57079634)+(inputSampleL*(1.0-pos));
		if (inputSampleL < 0) inputSampleL = (sin(inputSampleL*1.57079634*neg)/1.57079634)+(inputSampleL*(1.0-neg));
	
		outSample = (inputSampleR * fixA[fix_a0]) + fixA[fix_sR1];
		fixA[fix_sR1] = (inputSampleR * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sR2];
		fixA[fix_sR2] = (inputSampleR * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		if (inputSampleR > 0) inputSampleR = (sin(inputSampleR*1.57079634*pos)/1.57079634)+(inputSampleR*(1.0-pos));
		if (inputSampleR < 0) inputSampleR = (sin(inputSampleR*1.57079634*neg)/1.57079634)+(inputSampleR*(1.0-neg));
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void PurestWarm2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double pos = A;
	double neg = B;
	double cutoff = 25000.0 / getSampleRate();
	if (cutoff > 0.49) cutoff = 0.49; //don't crash if run at 44.1k
	fixA[fix_freq] = cutoff;
	fixA[fix_reso] = 0.70710678; //butterworth Q
	double K = tan(M_PI * fixA[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fixA[fix_reso] + K * K);
	fixA[fix_a0] = K * K * norm;
	fixA[fix_a1] = 2.0 * fixA[fix_a0];
	fixA[fix_a2] = fixA[fix_a0];
	fixA[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixA[fix_b2] = (1.0 - K / fixA[fix_reso] + K * K) * norm;
	fixA[fix_sL1] = 0.0;
	fixA[fix_sL2] = 0.0;
	fixA[fix_sR1] = 0.0;
	fixA[fix_sR2] = 0.0;
	//define filters here: on VST you can't define them in reset 'cos getSampleRate isn't returning good information yet
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double outSample = (inputSampleL * fixA[fix_a0]) + fixA[fix_sL1];
		fixA[fix_sL1] = (inputSampleL * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sL2];
		fixA[fix_sL2] = (inputSampleL * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		
		if (inputSampleL > 0) inputSampleL = (sin(inputSampleL*1.57079634*pos)/1.57079634)+(inputSampleL*(1.0-pos));
		if (inputSampleL < 0) inputSampleL = (sin(inputSampleL*1.57079634*neg)/1.57079634)+(inputSampleL*(1.0-neg));
		
		outSample = (inputSampleR * fixA[fix_a0]) + fixA[fix_sR1];
		fixA[fix_sR1] = (inputSampleR * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sR2];
		fixA[fix_sR2] = (inputSampleR * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		if (inputSampleR > 0) inputSampleR = (sin(inputSampleR*1.57079634*pos)/1.57079634)+(inputSampleR*(1.0-pos));
		if (inputSampleR < 0) inputSampleR = (sin(inputSampleR*1.57079634*neg)/1.57079634)+(inputSampleR*(1.0-neg));
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
