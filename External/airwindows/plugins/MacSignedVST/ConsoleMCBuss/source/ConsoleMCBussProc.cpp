/* ========================================
 *  ConsoleMCBuss - ConsoleMCBuss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleMCBuss_H
#include "ConsoleMCBuss.h"
#endif

void ConsoleMCBuss::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double source = 0.814/overallscale;	
	gslew[threshold10] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold9] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold8] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold7] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold6] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold5] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold4] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold3] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold2] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold1] = source;
	source *= 1.618033988749894848204586;
	
	gainA = gainB;
	gainB = sqrt(A); //smoothed master fader from Z2 filters
	//this will be applied three times: this is to make the various tone alterations
	//hit differently at different master fader drive levels.
	//in particular, backing off the master fader tightens the super lows
	//but opens up the EverySlew, because more of the attentuation happens before
	//you even get to slew clipping :) and if the fader is not active, it bypasses completely.
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double gain = (gainA*temp)+(gainB*(1.0-temp));
		//setting up smoothed master fader
		
		//begin SubTight section
		double subSampleL = inputSampleL * 0.001;
		double subSampleR = inputSampleR * 0.001;
		
		double scale = 0.5+fabs(subSampleL*0.5);
		subSampleL = (subAL+(sin(subAL-subSampleL)*scale));
		subAL = subSampleL*scale;
		scale = 0.5+fabs(subSampleR*0.5);
		subSampleR = (subAR+(sin(subAR-subSampleR)*scale));
		subAR = subSampleR*scale;
		scale = 0.5+fabs(subSampleL*0.5);
		subSampleL = (subBL+(sin(subBL-subSampleL)*scale));
		subBL = subSampleL*scale;
		scale = 0.5+fabs(subSampleR*0.5);
		subSampleR = (subBR+(sin(subBR-subSampleR)*scale));
		subBR = subSampleR*scale;
		scale = 0.5+fabs(subSampleL*0.5);
		subSampleL = (subCL+(sin(subCL-subSampleL)*scale));
		subCL = subSampleL*scale;
		scale = 0.5+fabs(subSampleR*0.5);
		subSampleR = (subCR+(sin(subCR-subSampleR)*scale));
		subCR = subSampleR*scale;
		scale = 0.5+fabs(subSampleL*0.5);
		subSampleL = (subDL+(sin(subDL-subSampleL)*scale));
		subDL = subSampleL*scale;
		scale = 0.5+fabs(subSampleR*0.5);
		subSampleR = (subDR+(sin(subDR-subSampleR)*scale));
		subDR = subSampleR*scale;
		if (subSampleL > 0.25) subSampleL = 0.25;
		if (subSampleL < -0.25) subSampleL = -0.25;
		if (subSampleR > 0.25) subSampleR = 0.25;
		if (subSampleR < -0.25) subSampleR = -0.25;
		inputSampleL -= (subSampleL*16.0);
		inputSampleR -= (subSampleR*16.0);
		//end SubTight section
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//subtight is always fully engaged: tighten response when restraining full console
		
		//begin Console7Buss which is the one we choose for ConsoleMC
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL = ((asin(inputSampleL*fabs(inputSampleL))/((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL)))*0.618033988749894848204586)+(asin(inputSampleL)*0.381966011250105);
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR = ((asin(inputSampleR*fabs(inputSampleR))/((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR)))*0.618033988749894848204586)+(asin(inputSampleR)*0.381966011250105);
		//this is an asin version of Spiral blended with regular asin ConsoleBuss.
		//It's blending between two different harmonics in the overtones of the algorithm
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//after C7Buss but before EverySlew: allow highs to come out a bit more
		//when pulling back master fader. Less drive equals more open
		
		//begin EverySlew
		for (int x = 20; x < gslew_total; x += 5) { //gslew_total is 50
			
			if (((inputSampleL-gslew[x])-((gslew[x]-gslew[x+2])*0.618033988749894848204586)) > gslew[x+4])
				inputSampleL = (gslew[x]-((gslew[x]-gslew[x+2])*0.141)) + (gslew[x+4]*(1.0-0.141));
			if (-((inputSampleL-gslew[x])-((gslew[x]-gslew[x+2])*0.618033988749894848204586)) > gslew[x+4])
				inputSampleL = (gslew[x]-((gslew[x]-gslew[x+2])*0.141*0.78)) - (gslew[x+4]*(1.0-(0.141*0.78)));
			gslew[x+2] = gslew[x]*(1.0-0.141);
			gslew[x] = inputSampleL;
			
			if (((inputSampleR-gslew[x+1])-((gslew[x+1]-gslew[x+3])*0.618033988749894848204586)) > gslew[x+4])
				inputSampleR = (gslew[x+1]-((gslew[x+1]-gslew[x+3])*0.141)) + (gslew[x+4]*(1.0-0.141));
			if (-((inputSampleR-gslew[x+1])-((gslew[x+1]-gslew[x+3])*0.618033988749894848204586)) > gslew[x+4])
				inputSampleR = (gslew[x+1]-((gslew[x+1]-gslew[x+3])*0.141*0.78)) - (gslew[x+4]*(1.0-(0.141*0.78)));
			gslew[x+3] = gslew[x+1]*(1.0-0.141);
			gslew[x+1] = inputSampleR;
		}
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//after EverySlew fades the total output sound: least change in tone here.
				
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

void ConsoleMCBuss::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double source = 0.814/overallscale;	
	gslew[threshold10] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold9] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold8] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold7] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold6] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold5] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold4] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold3] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold2] = source;
	source *= 1.618033988749894848204586;
	gslew[threshold1] = source;
	source *= 1.618033988749894848204586;
	
	gainA = gainB;
	gainB = sqrt(A); //smoothed master fader from Z2 filters
	//this will be applied three times: this is to make the various tone alterations
	//hit differently at different master fader drive levels.
	//in particular, backing off the master fader tightens the super lows
	//but opens up the EverySlew, because more of the attentuation happens before
	//you even get to slew clipping :) and if the fader is not active, it bypasses completely.
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double gain = (gainA*temp)+(gainB*(1.0-temp));
		//setting up smoothed master fader
		
		//begin SubTight section
		double subSampleL = inputSampleL * 0.001;
		double subSampleR = inputSampleR * 0.001;
		
		double scale = 0.5+fabs(subSampleL*0.5);
		subSampleL = (subAL+(sin(subAL-subSampleL)*scale));
		subAL = subSampleL*scale;
		scale = 0.5+fabs(subSampleR*0.5);
		subSampleR = (subAR+(sin(subAR-subSampleR)*scale));
		subAR = subSampleR*scale;
		scale = 0.5+fabs(subSampleL*0.5);
		subSampleL = (subBL+(sin(subBL-subSampleL)*scale));
		subBL = subSampleL*scale;
		scale = 0.5+fabs(subSampleR*0.5);
		subSampleR = (subBR+(sin(subBR-subSampleR)*scale));
		subBR = subSampleR*scale;
		scale = 0.5+fabs(subSampleL*0.5);
		subSampleL = (subCL+(sin(subCL-subSampleL)*scale));
		subCL = subSampleL*scale;
		scale = 0.5+fabs(subSampleR*0.5);
		subSampleR = (subCR+(sin(subCR-subSampleR)*scale));
		subCR = subSampleR*scale;
		scale = 0.5+fabs(subSampleL*0.5);
		subSampleL = (subDL+(sin(subDL-subSampleL)*scale));
		subDL = subSampleL*scale;
		scale = 0.5+fabs(subSampleR*0.5);
		subSampleR = (subDR+(sin(subDR-subSampleR)*scale));
		subDR = subSampleR*scale;
		if (subSampleL > 0.25) subSampleL = 0.25;
		if (subSampleL < -0.25) subSampleL = -0.25;
		if (subSampleR > 0.25) subSampleR = 0.25;
		if (subSampleR < -0.25) subSampleR = -0.25;
		inputSampleL -= (subSampleL*16.0);
		inputSampleR -= (subSampleR*16.0);
		//end SubTight section
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//subtight is always fully engaged: tighten response when restraining full console
		
		//begin Console7Buss which is the one we choose for ConsoleMC
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL = ((asin(inputSampleL*fabs(inputSampleL))/((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL)))*0.618033988749894848204586)+(asin(inputSampleL)*0.381966011250105);
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR = ((asin(inputSampleR*fabs(inputSampleR))/((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR)))*0.618033988749894848204586)+(asin(inputSampleR)*0.381966011250105);
		//this is an asin version of Spiral blended with regular asin ConsoleBuss.
		//It's blending between two different harmonics in the overtones of the algorithm
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//after C7Buss but before EverySlew: allow highs to come out a bit more
		//when pulling back master fader. Less drive equals more open
		
		//begin EverySlew
		for (int x = 20; x < gslew_total; x += 5) { //gslew_total is 50
			
			if (((inputSampleL-gslew[x])-((gslew[x]-gslew[x+2])*0.618033988749894848204586)) > gslew[x+4])
				inputSampleL = (gslew[x]-((gslew[x]-gslew[x+2])*0.141)) + (gslew[x+4]*(1.0-0.141));
			if (-((inputSampleL-gslew[x])-((gslew[x]-gslew[x+2])*0.618033988749894848204586)) > gslew[x+4])
				inputSampleL = (gslew[x]-((gslew[x]-gslew[x+2])*0.141*0.78)) - (gslew[x+4]*(1.0-(0.141*0.78)));
			gslew[x+2] = gslew[x]*(1.0-0.141);
			gslew[x] = inputSampleL;
			
			if (((inputSampleR-gslew[x+1])-((gslew[x+1]-gslew[x+3])*0.618033988749894848204586)) > gslew[x+4])
				inputSampleR = (gslew[x+1]-((gslew[x+1]-gslew[x+3])*0.141)) + (gslew[x+4]*(1.0-0.141));
			if (-((inputSampleR-gslew[x+1])-((gslew[x+1]-gslew[x+3])*0.618033988749894848204586)) > gslew[x+4])
				inputSampleR = (gslew[x+1]-((gslew[x+1]-gslew[x+3])*0.141*0.78)) - (gslew[x+4]*(1.0-(0.141*0.78)));
			gslew[x+3] = gslew[x+1]*(1.0-0.141);
			gslew[x+1] = inputSampleR;
		}
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		} //if using the master fader, we are going to attenuate three places.
		//after EverySlew fades the total output sound: least change in tone here.
		
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
