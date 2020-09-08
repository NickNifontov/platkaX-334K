/*
 * plxPID.c
 *
 *  Created on: Sep 7, 2020
 *      Author: RadaR
 */

#ifndef PLXCORE_SRC_PLXPID_C_
#define PLXCORE_SRC_PLXPID_C_

//*****************************************************//
//Include
#include "plxPID.h"
//*****************************************************//

//*****************************************************//
//Global
volatile float pid_error=0;
volatile float pid_integralFilter=0;
volatile float pid_derivativeFilter=0;
volatile float pid_proportionalComponent=0;
volatile float pid_integralComponent=0;
volatile float pid_derivativeComponent=0;
volatile float pid_tempPID=0;
volatile float pid_outputPID=0;
//*****************************************************//

//*****************************************************//
//PID
void pid_SetReference (float reference)
{
	PID.reference = reference;
}

void pid_SetFeedback (float feedback, float deltaTimeSampling)
{
	PID.feedback = feedback;
	PID.deltaTimeSampling = deltaTimeSampling;
}

void pid_SetCoefficient (float Kp, float Ki, float Kd, float BackSaturation, float filterDerivative) {
	PID.coefficient.proportional = Kp;
	PID.coefficient.integral = Ki;
	PID.coefficient.derivative = Kd;
	PID.coefficient.coefficientBackSaturation = BackSaturation;
	PID.coefficient.filterDerivative = filterDerivative;
}

void pid_SetSaturation (float lowLimit, float highLimit)
{
	PID.saturation.lowThershold = lowLimit;
	PID.saturation.highThershold = highLimit;
}

void pid_Compute()
{
        pid_error = PID.reference - PID.feedback;

        pid_proportionalComponent = PID.coefficient.proportional * pid_error;

        pid_integralComponent += PID.deltaTimeSampling * pid_integralFilter;
        pid_integralFilter = PID.coefficient.integral * pid_error + PID.coefficient.coefficientBackSaturation * (pid_outputPID - pid_tempPID);

        pid_derivativeFilter += PID.deltaTimeSampling * pid_derivativeComponent;
        pid_derivativeComponent = (PID.coefficient.derivative * pid_error - pid_derivativeFilter) * PID.coefficient.filterDerivative;

        pid_outputPID = pid_tempPID = pid_proportionalComponent + pid_integralComponent + pid_derivativeComponent;
        pid_outputPID = fmin(fmax(pid_outputPID, PID.saturation.lowThershold), PID.saturation.highThershold);
}

float pid_Get() {
       return pid_outputPID;
}
//*****************************************************//

#endif /* PLXCORE_SRC_PLXPID_C_ */
