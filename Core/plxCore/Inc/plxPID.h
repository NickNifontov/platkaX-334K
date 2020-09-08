/*
 * plxPID.h
 *
 *  Created on: Sep 7, 2020
 *      Author: RadaR
 */

#ifndef PLXCORE_INC_PLXPID_H_
#define PLXCORE_INC_PLXPID_H_

//*****************************************************//
//Include
#include "main.h"
#include <math.h>
//*****************************************************//

//*****************************************************//
//Global
extern volatile float pid_error;
extern volatile float pid_integralFilter;
extern volatile float pid_derivativeFilter;
extern volatile float pid_proportionalComponent;
extern volatile float pid_integralComponent;
extern volatile float pid_derivativeComponent;
extern volatile float pid_tempPID;
extern volatile float pid_outputPID;

struct {
    float feedback;
    float reference;
    float deltaTimeSampling;

    struct {
        float proportional;
        float integral;
        float derivative;
        float coefficientBackSaturation;
        float filterDerivative;
    } coefficient;

    struct {
        float lowThershold;
        float highThershold;
    } saturation;
} PID;
//*****************************************************//

//*****************************************************//
//PID
void pid_SetReference (float reference);
void pid_SetFeedback (float feedback, float deltaTimeSampling);
void pid_SetCoefficient (float Kp, float Ki, float Kd, float BackSaturation, float filterDerivative);
void pid_SetSaturation (float lowLimit, float highLimit);
void pid_Compute();
float pid_Get();
//*****************************************************//

#endif /* PLXCORE_INC_PLXPID_H_ */
