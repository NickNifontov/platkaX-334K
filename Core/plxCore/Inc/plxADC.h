/*
 * plxADC.h
 *
 *  Created on: Sep 6, 2020
 *      Author: RadaR
 */

#ifndef PLXCORE_INC_PLXADC_H_
#define PLXCORE_INC_PLXADC_H_

//*****************************************************//
//Include
#include "main.h"
//*****************************************************//

//*****************************************************//
//Defines
#define sizeBuffer				(uint16_t)(50)
#define sizeWindow				(uint16_t)(20)
//*****************************************************//


//*****************************************************//
//Global
extern uint16_t plxV16[sizeBuffer]; // 16V Level
extern uint16_t plxRAW[sizeBuffer]; // RAW Level
extern uint16_t plxTEMP[sizeBuffer]; // TEMP Level
extern uint16_t plxOCV[sizeBuffer]; // Averege OCV Level
extern uint16_t plxOCI[sizeBuffer]; // Averege OCI Level


extern volatile uint16_t gV16; // global 16V Level
extern volatile uint16_t gRAW; // global RAW Level
extern volatile uint16_t gTEMP; // global TEMP Level
extern volatile uint16_t gOCV; // global Averege OCV Level
extern volatile uint16_t gOCI; // global Averege OCI Level

extern volatile uint8_t BufferInd; // ind of ADC buffer
extern volatile uint8_t BufferIndOC; // ind of ADC buffer

extern volatile uint8_t CalcFlag; // need to calc
//*****************************************************//

//*****************************************************//
//FilterWindowMedium
void GetADC1(void);
void GetADC2(void);
void ResetADCBuff(uint16_t *array, uint16_t size);
void Recursive (uint16_t *array, uint16_t size) ;
float FilterWindowMedium(uint16_t *array, uint16_t size, uint16_t window);
//*****************************************************//

#endif /* PLXCORE_INC_PLXADC_H_ */
