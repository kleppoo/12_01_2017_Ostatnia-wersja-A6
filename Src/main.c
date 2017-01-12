/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"
#include "lowlevelinit.h"
#include "swtimer.h"
#include "at_handler.h"
#include "modem.h"
#include "main.h"
#include "GPS.h"

#define ITM_Port8(n) (*((volatile unsigned char *)(0xE0000000 + 4 * n))) 
	
struct_GPS GPS_DATA;

GSM_modem_struct GSM_modem;

ServerData_struct DataFromVehicom;

modem_comand_type ModemCommand;

GSM_connect_type GSM_status;

GPRS_connect_type GPRS_status;

IP_conect_type IP_status;

TCP_conect_type TCP_status;

Vehicom_conect_type Vehicom_status;

Connect_type_struct ConectParameter;

Timery_type_struct Timery;

ErrorGSM_struct ErrorGSM;

GSM_RxBuff_struct RxGSM_DATA;

uint32_t ADC[5];

void SendDataToServerVehicom(void);

/* Private variables ---------------------------------------------------------*/
static volatile uint32_t ExeTimeStamp;

/* Private function prototypes -----------------------------------------------*/

int fputc(int ch, FILE *f) {
    return ITM_SendChar(ch);
}

int main(void)
{   
    volatile unsigned int* SCB_DEMCR = (unsigned int*)0xE000EDFC; //address of the register
    *SCB_DEMCR = *SCB_DEMCR | 0x01000000;

    uint32_t timeStamp;
       
    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Initialize all configured peripherals */
    LOWLEVEL_Init();
	GSM_WakeUp;
	//GSM_Sleep;
	__HAL_DBGMCU_FREEZE_IWDG();

	ADC_Init();
	   
    DWT->CYCCNT = 0; // reset the counter
    DWT->CTRL = DWT->CTRL | 0x00000001 ; // enable the counter
      SWTimer_Start(&Timery.WaitForResponseTimer, 100000);
    while (1)
    {
	    	//    Refresh_IWDG();
        timeStamp = DWT->CYCCNT;
        MODEM_Job();
	   GPS_Job();
        ExeTimeStamp = DWT->CYCCNT - timeStamp;
	   SendDataToServerVehicom();
	    
	/*    
		if (SWTimer_GetStatus(&Timery.WaitForResponseTimer) == SWTIMER_ELAPSED)
		{

			SWTimer_Start(&Timery.WaitForResponseTimer, 5000);
			/*if (GSM_modem.MODEM_ON == true) GSM_modem.MODEM_ON = false;
			else GSM_modem.MODEM_ON = true;*/
		/*	 
 			while (SWTimer_GetStatus(&Timery.WaitForResponseTimer) != SWTIMER_ELAPSED)	
				GSM_RESET_ON ;
				 SWTimer_Start(&Timery.WaitForResponseTimer, 130000);
					GSM_RESET_OFF;

		}*/
    }
}
 
#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

void SendDataToServerVehicom(void)
{
	if (TCP_status == Connect_TCP)
	{	
		if (DataFromVehicom.NewData == true)
		{
				if (strstr ((const char*)DataFromVehicom.DATA, (const char*)"LOAD") != 0)
				{
					SWTimer_Start(&Timery.timer_Vehicom, 15000); 
					Vehicom_status = Login_vehicom;
				}
				else if (strstr ((const char*)DataFromVehicom.DATA, (const char*)"ON") != 0)
				{
					Vehicom_status = Login_vehicom;
				}
				else // brak odpowiedzi
				{					
					Vehicom_status = Login_vehicom;
				//	SWTimer_Start(&Timery.timer_Vehicom, 15000);
				}
				

			/*{
				Vehicom_status = NoLogin_vehicom;
				SWTimer_Start(&Timery.timer_Vehicom, 15000);
			}*/
			DataFromVehicom.NewData = false;
			DataFromVehicom.DATA[0]=0;
			DataFromVehicom.DATA[1]=0;
		}

		switch(Vehicom_status)
		{
			case(NoLogin_vehicom):
				
			if (SWTimer_GetStatus(&Timery.timer_Vehicom) == SWTIMER_ELAPSED || SWTimer_GetStatus(&Timery.timer_Vehicom) == SWTIMER_STOPPED)
			{	ErrorGSM.ErrorVehicom++;
				Vehicom_status = TryLogin_vehicom;
				SWTimer_Start(&Timery.timer_Vehicom, 15000);
				sprintf((char*)RxGSM_DATA.DataToSEND, (const char*)"##,imei:%s,A,;%c",ConectParameter.IMEI,26); //kod 26 to ctrl +z
				ModemCommand= CommandStartSendDataToServer;
			}
			break;
			
			case(TryLogin_vehicom):
				// poczekac 30 sek a pozniej znowu probowac sie polaczyc
			break;
			
			case(Login_vehicom):
				if (SWTimer_GetStatus(&Timery.timer_Vehicom) == SWTIMER_ELAPSED || SWTimer_GetStatus(&Timery.timer_Vehicom) == SWTIMER_STOPPED)
				{
					ErrorGSM.ErrorVehicom=0;
					SWTimer_Start(&Timery.timer_Vehicom, 15000);
					sprintf((char*)RxGSM_DATA.DataToSEND, (const char*)"imei:%s,tracker,%s%s,,F,%s,%c,%s,%c,%s,%c,0.00,0;%c",
					ConectParameter.IMEI,GPS_DATA.Reversed_DATE,GPS_DATA.Short_TIME,GPS_DATA.TIME,GPS_DATA.ACTIVE,GPS_DATA.LATITUDE,
					GPS_DATA.N_or_S,GPS_DATA.LONGITUDE,GPS_DATA.W_or_E,26); //kod 26 to ctrl +z konczacy transmisje
					ModemCommand= CommandStartSendDataToServer;
				}
			break;
			default:
				Vehicom_status = NoLogin_vehicom;
			break;
		}
	}
	else	
	{
		Vehicom_status = NoLogin_vehicom;
		ErrorGSM.ErrorVehicom = 0;
	}
	
}
