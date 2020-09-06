/*
 * plxOC_V.h
 *
 *  Created on: Sep 5, 2020
 *      Author: RadaR
 */

#ifndef PLXCORE_INC_PLXOC_V_H_
#define PLXCORE_INC_PLXOC_V_H_

//*****************************************************//
//Include
#include "main.h"
//*****************************************************//

//*****************************************************//
//Defines
#define Sinus_Table_Resolution 									468
#define Sinus_Table_Resolution_DT 								5
#define MAX_SIN (uint16_t)(Sinus_Table_Resolution+Sinus_Table_Resolution_DT)
#define Voltage_Level_Reset 									4095 // ~3.3V

#define PULS_A_BEGIN											4096
#define PULS_A_END												24576
#define PULS_B_BEGIN											28672
#define PULS_B_END												49152
//*****************************************************//

//*****************************************************//
//Global
extern volatile uint16_t OC_V_Table[Sinus_Table_Resolution];
extern volatile uint16_t Sinus_Ind;
extern volatile uint8_t TOP_BOTTOM;
//*****************************************************//

#endif /* PLXCORE_INC_PLXOC_V_H_ */
