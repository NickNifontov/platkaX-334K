/*
 * plxADC.c
 *
 *  Created on: Sep 6, 2020
 *      Author: RadaR
 */

//*****************************************************//
//Include
#include "plxADC.h"
#include "plxPID.h"
//*****************************************************//


//*****************************************************//
//Global
volatile uint16_t gV16=0; // global 16V Level
volatile uint16_t gRAW=0; // global RAW Level
volatile uint16_t gTEMP=0; // global TEMP Level
volatile uint16_t gOCV=0; // global Averege OCV Level
volatile uint16_t gOCI=0; // global Averege OCI Level

volatile uint8_t BufferInd=0; // ind of ADC buffer
volatile uint8_t BufferIndOC=0; // ind of ADC buffer

volatile uint8_t CalcFlag=0; // need to calc


 uint16_t plxV16[sizeBuffer];// 16V Level
 uint16_t plxRAW[sizeBuffer];// RAW Level
 uint16_t plxTEMP[sizeBuffer]; // TEMP Level
 uint16_t plxOCV[sizeBuffer]; // Averege OCV Level
 uint16_t plxOCI[sizeBuffer]; // Averege OCI Level

//*****************************************************//

//*****************************************************//
//FilterWindowMedium
 void GetADC1(void) {
	 plxRAW[BufferInd]=ADC1->JDR2;
	 plxOCV[BufferIndOC]=ADC1->JDR1;
 }

 void GetADC2(void) {
	 plxOCI[BufferIndOC]=ADC2->JDR1;
	 plxV16[BufferInd]=ADC2->JDR2;
	 plxTEMP[BufferInd]=ADC2->JDR3;
 }


void ResetADCBuff(uint16_t *array, uint16_t size) {
	for (uint16_t i = 0; i < size; i++) {
		array[i]=0;
	}
}

void Recursive (uint16_t *array, uint16_t size) {
            int i = 0;
            int j = size - 1;

            uint16_t mid = array[size / 2];

            do {
                while(array[i] < mid) {
                    i++;
                }

                while(array[j] > mid) {
                    j--;
                }

                if (i <= j) {
                    uint16_t tmp = array[i];
                    array[i] = array[j];
                    array[j] = tmp;

                    i++;
                    j--;
                }
            } while (i <= j);

            if (j > 0) { Recursive(array, j + 1); }
            if (i < size) { Recursive(&array[i], size - i); }
        }

float FilterWindowMedium (uint16_t *array, uint16_t size, uint16_t window) {
            Recursive(array, size);

            float dataBuffer = 0.0f;

            uint16_t highLimit = (size / 2) + (window / 2);
            uint16_t lowLimit = (size / 2) - (window / 2);

            for (uint16_t i = lowLimit; i < highLimit; i++) {
                dataBuffer += array[i];
            }

            return ((float)(dataBuffer / window));
        }
//*****************************************************//
