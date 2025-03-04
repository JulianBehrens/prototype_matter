/* ========================================
 *  BitGlitter - BitGlitter.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BitGlitter_H
#include "BitGlitter.h"
#endif

void BitGlitter::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double factor = B+1.0;
	factor = pow(factor,7)+2.0;
	int divvy = (int)(factor*overallscale);
	double rateA = 1.0 / divvy;
	double rezA = 0.0016666666666667; //looks to be a fixed bitcrush
	double rateB = 1.61803398875 / divvy;
	double rezB = 0.0026666666666667; //looks to be a fixed bitcrush
	double offset;
	double ingain = pow(10.0,((A * 36.0)-18.0)/14.0); //add adjustment factor
	double outgain = pow(10.0,((C * 36.0)-18.0)/14.0); //add adjustment factor
	double wet = D;

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		

		//first, the distortion section
		inputSampleL *= ingain;
		inputSampleR *= ingain;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		inputSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));

		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		inputSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		
		ataDrySampleL = inputSampleL;
		ataHalfwaySampleL = (inputSampleL + ataLastSampleL ) / 2.0;
		ataLastSampleL = inputSampleL;
		//setting up crude oversampling

		ataDrySampleR = inputSampleR;
		ataHalfwaySampleR = (inputSampleR + ataLastSampleR ) / 2.0;
		ataLastSampleR = inputSampleR;
		//setting up crude oversampling
		
		//begin raw sample L
		positionAL += rateA;
		double outputSampleL = heldSampleAL;
		if (positionAL > 1.0)
		{
			positionAL -= 1.0;
			heldSampleAL = (lastSampleL * positionAL) + (inputSampleL * (1-positionAL));
			outputSampleL = (outputSampleL * 0.5) + (heldSampleAL * 0.5);
			//softens the edge of the derez
		}
		if (outputSampleL > 0)
		{
			offset = outputSampleL;
			while (offset > 0) {offset -= rezA;}
			outputSampleL -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (outputSampleL < 0)
		{
			offset = outputSampleL;
			while (offset < 0) {offset += rezA;}
			outputSampleL -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		outputSampleL *= (1.0 - rezA);
		if (fabs(outputSampleL) < rezA) outputSampleL = 0.0;
		inputSampleL = outputSampleL;
		//end raw sample L
		
		//begin raw sample R
		positionAR += rateA;
		double outputSampleR = heldSampleAR;
		if (positionAR > 1.0)
		{
			positionAR -= 1.0;
			heldSampleAR = (lastSampleR * positionAR) + (inputSampleR * (1-positionAR));
			outputSampleR = (outputSampleR * 0.5) + (heldSampleAR * 0.5);
			//softens the edge of the derez
		}
		if (outputSampleR > 0)
		{
			offset = outputSampleR;
			while (offset > 0) {offset -= rezA;}
			outputSampleR -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (outputSampleR < 0)
		{
			offset = outputSampleR;
			while (offset < 0) {offset += rezA;}
			outputSampleR -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		outputSampleR *= (1.0 - rezA);
		if (fabs(outputSampleR) < rezA) outputSampleR = 0.0;
		inputSampleR = outputSampleR;
		//end raw sample R
		
		//begin interpolated sample L
		positionBL += rateB;
		outputSampleL = heldSampleBL;
		if (positionBL > 1.0)
		{
			positionBL -= 1.0;
			heldSampleBL = (lastSampleL * positionBL) + (ataHalfwaySampleL * (1-positionBL));
			outputSampleL = (outputSampleL * 0.5) + (heldSampleBL * 0.5);
			//softens the edge of the derez
		}
		if (outputSampleL > 0)
		{
			offset = outputSampleL;
			while (offset > 0) {offset -= rezB;}
			outputSampleL -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (outputSampleL < 0)
		{
			offset = outputSampleL;
			while (offset < 0) {offset += rezB;}
			outputSampleL -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		outputSampleL *= (1.0 - rezB);
		if (fabs(outputSampleL) < rezB) outputSampleL = 0.0;
		ataHalfwaySampleL = outputSampleL;
		//end interpolated sample L
		
		//begin interpolated sample R
		positionBR += rateB;
		outputSampleR = heldSampleBR;
		if (positionBR > 1.0)
		{
			positionBR -= 1.0;
			heldSampleBR = (lastSampleR * positionBR) + (ataHalfwaySampleR * (1-positionBR));
			outputSampleR = (outputSampleR * 0.5) + (heldSampleBR * 0.5);
			//softens the edge of the derez
		}
		if (outputSampleR > 0)
		{
			offset = outputSampleR;
			while (offset > 0) {offset -= rezB;}
			outputSampleR -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (outputSampleR < 0)
		{
			offset = outputSampleR;
			while (offset < 0) {offset += rezB;}
			outputSampleR -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		outputSampleR *= (1.0 - rezB);
		if (fabs(outputSampleR) < rezB) outputSampleR = 0.0;
		ataHalfwaySampleR = outputSampleR;
		//end interpolated sample R
				
		inputSampleL += ataHalfwaySampleL;
		inputSampleL /= 2.0;
		//plain old blend the two
		
		inputSampleR += ataHalfwaySampleR;
		inputSampleR /= 2.0;
		//plain old blend the two
		
		outputSampleL = (inputSampleL * (1.0-(wet/2))) + (lastOutputSampleL*(wet/2));
		//darken to extent of wet in wet/dry, maximum 50%
		lastOutputSampleL = inputSampleL;
		outputSampleL *= outgain;
		
		outputSampleR = (inputSampleR * (1.0-(wet/2))) + (lastOutputSampleR*(wet/2));
		//darken to extent of wet in wet/dry, maximum 50%
		lastOutputSampleR = inputSampleR;
		outputSampleR *= outgain;
		
		if (wet < 1.0) {
			outputSampleL = (drySampleL * (1.0-wet)) + (outputSampleL * wet);
			outputSampleR = (drySampleR * (1.0-wet)) + (outputSampleR * wet);
		}
		
		inputSampleL = outputSampleL;
		inputSampleR = outputSampleR;
		
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void BitGlitter::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double factor = B+1.0;
	factor = pow(factor,7)+2.0;
	int divvy = (int)(factor*overallscale);
	double rateA = 1.0 / divvy;
	double rezA = 0.0016666666666667; //looks to be a fixed bitcrush
	double rateB = 1.61803398875 / divvy;
	double rezB = 0.0026666666666667; //looks to be a fixed bitcrush
	double offset;
	double ingain = pow(10.0,((A * 36.0)-18.0)/14.0); //add adjustment factor
	double outgain = pow(10.0,((C * 36.0)-18.0)/14.0); //add adjustment factor
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//first, the distortion section
		inputSampleL *= ingain;
		inputSampleR *= ingain;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		inputSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		inputSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		
		ataDrySampleL = inputSampleL;
		ataHalfwaySampleL = (inputSampleL + ataLastSampleL ) / 2.0;
		ataLastSampleL = inputSampleL;
		//setting up crude oversampling
		
		ataDrySampleR = inputSampleR;
		ataHalfwaySampleR = (inputSampleR + ataLastSampleR ) / 2.0;
		ataLastSampleR = inputSampleR;
		//setting up crude oversampling
		
		//begin raw sample L
		positionAL += rateA;
		double outputSampleL = heldSampleAL;
		if (positionAL > 1.0)
		{
			positionAL -= 1.0;
			heldSampleAL = (lastSampleL * positionAL) + (inputSampleL * (1-positionAL));
			outputSampleL = (outputSampleL * 0.5) + (heldSampleAL * 0.5);
			//softens the edge of the derez
		}
		if (outputSampleL > 0)
		{
			offset = outputSampleL;
			while (offset > 0) {offset -= rezA;}
			outputSampleL -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (outputSampleL < 0)
		{
			offset = outputSampleL;
			while (offset < 0) {offset += rezA;}
			outputSampleL -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		outputSampleL *= (1.0 - rezA);
		if (fabs(outputSampleL) < rezA) outputSampleL = 0.0;
		inputSampleL = outputSampleL;
		//end raw sample L
		
		//begin raw sample R
		positionAR += rateA;
		double outputSampleR = heldSampleAR;
		if (positionAR > 1.0)
		{
			positionAR -= 1.0;
			heldSampleAR = (lastSampleR * positionAR) + (inputSampleR * (1-positionAR));
			outputSampleR = (outputSampleR * 0.5) + (heldSampleAR * 0.5);
			//softens the edge of the derez
		}
		if (outputSampleR > 0)
		{
			offset = outputSampleR;
			while (offset > 0) {offset -= rezA;}
			outputSampleR -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (outputSampleR < 0)
		{
			offset = outputSampleR;
			while (offset < 0) {offset += rezA;}
			outputSampleR -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		outputSampleR *= (1.0 - rezA);
		if (fabs(outputSampleR) < rezA) outputSampleR = 0.0;
		inputSampleR = outputSampleR;
		//end raw sample R
		
		//begin interpolated sample L
		positionBL += rateB;
		outputSampleL = heldSampleBL;
		if (positionBL > 1.0)
		{
			positionBL -= 1.0;
			heldSampleBL = (lastSampleL * positionBL) + (ataHalfwaySampleL * (1-positionBL));
			outputSampleL = (outputSampleL * 0.5) + (heldSampleBL * 0.5);
			//softens the edge of the derez
		}
		if (outputSampleL > 0)
		{
			offset = outputSampleL;
			while (offset > 0) {offset -= rezB;}
			outputSampleL -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (outputSampleL < 0)
		{
			offset = outputSampleL;
			while (offset < 0) {offset += rezB;}
			outputSampleL -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		outputSampleL *= (1.0 - rezB);
		if (fabs(outputSampleL) < rezB) outputSampleL = 0.0;
		ataHalfwaySampleL = outputSampleL;
		//end interpolated sample L
		
		//begin interpolated sample R
		positionBR += rateB;
		outputSampleR = heldSampleBR;
		if (positionBR > 1.0)
		{
			positionBR -= 1.0;
			heldSampleBR = (lastSampleR * positionBR) + (ataHalfwaySampleR * (1-positionBR));
			outputSampleR = (outputSampleR * 0.5) + (heldSampleBR * 0.5);
			//softens the edge of the derez
		}
		if (outputSampleR > 0)
		{
			offset = outputSampleR;
			while (offset > 0) {offset -= rezB;}
			outputSampleR -= offset;
			//it's below 0 so subtracting adds the remainder
		}
		if (outputSampleR < 0)
		{
			offset = outputSampleR;
			while (offset < 0) {offset += rezB;}
			outputSampleR -= offset;
			//it's above 0 so subtracting subtracts the remainder
		}
		outputSampleR *= (1.0 - rezB);
		if (fabs(outputSampleR) < rezB) outputSampleR = 0.0;
		ataHalfwaySampleR = outputSampleR;
		//end interpolated sample R
		
		inputSampleL += ataHalfwaySampleL;
		inputSampleL /= 2.0;
		//plain old blend the two
		
		inputSampleR += ataHalfwaySampleR;
		inputSampleR /= 2.0;
		//plain old blend the two
		
		outputSampleL = (inputSampleL * (1.0-(wet/2))) + (lastOutputSampleL*(wet/2));
		//darken to extent of wet in wet/dry, maximum 50%
		lastOutputSampleL = inputSampleL;
		outputSampleL *= outgain;
		
		outputSampleR = (inputSampleR * (1.0-(wet/2))) + (lastOutputSampleR*(wet/2));
		//darken to extent of wet in wet/dry, maximum 50%
		lastOutputSampleR = inputSampleR;
		outputSampleR *= outgain;
		
		if (wet < 1.0) {
			outputSampleL = (drySampleL * (1.0-wet)) + (outputSampleL * wet);
			outputSampleR = (drySampleR * (1.0-wet)) + (outputSampleR * wet);
		}
		
		inputSampleL = outputSampleL;
		inputSampleR = outputSampleR;
		
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
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
