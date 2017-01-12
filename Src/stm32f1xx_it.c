/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "swtimer.h"
#include "main.h"
#include "GPS.h"
#include <string.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_adc1;

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern GSM_RxBuff_struct RxGSM_DATA;

extern struct_GPS GPS_DATA;

extern modem_comand_type ModemCommand;

extern ServerData_struct DataFromVehicom;

extern GSM_connect_type GSM_status;

extern GPRS_connect_type GPRS_status;

extern IP_conect_type IP_status;

extern TCP_conect_type TCP_status;

extern Vehicom_conect_type Vehicom_status;

extern ADC_HandleTypeDef hadc1;

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}



/*
//przerwanie od GPS
void UART5_IRQHandler(void)
{
   static char cChar_counter = 0;
   static char cStanRamki = 0;
   static char sTempBuf[6];
   static uint8_t *cPointer = 0;
   unsigned char znak,i,counterSemicon;
   if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET){

      znak=USART_ReceiveData(UART5);

      if(znak=='$') // rozpoznanie poczatka ramiki z GPS
         {
            cStanRamki = 1;
            cPointer = 0;
            cChar_counter = 0;             //kk
         }

       if ( cStanRamki == 1 && cChar_counter < 6)
         {
           sTempBuf[cChar_counter] = znak;
         }

      if(cStanRamki > 0) cChar_counter++;
        //kk
      if (cStanRamki == 1 && cChar_counter  == 6) //odebrano oznaczenie typu ramiki
      {
        if (sTempBuf[3]=='G' && sTempBuf[4]=='G' && sTempBuf[5]=='A') 
	   {
		   cPointer = GPS_DATA.GGA;
		   GPS_DATA.GGA_Start = true;
	   }
        if (sTempBuf[3]=='G' && sTempBuf[4]=='S' && sTempBuf[5]=='A')
	   {
		   cPointer = GPS_DATA.GSA;
		   GPS_DATA.GSA_Start = true;
	   }
        if (sTempBuf[3]=='G' && sTempBuf[4]=='S' && sTempBuf[5]=='V') 
	   {
		   cPointer = GPS_DATA.GSV;
		   GPS_DATA.GSV_Start = true;
	   }
        if (sTempBuf[3]=='R' && sTempBuf[4]=='M' && sTempBuf[5]=='C') 
	   {
		   cPointer = GPS_DATA.RMC;
		   GPS_DATA.RMC_Start = true;
	   }
        if (sTempBuf[3]=='V' && sTempBuf[4]=='T' && sTempBuf[5]=='G') 
	   {
		   cPointer = GPS_DATA.VTG;
		   GPS_DATA.VTG_Start = true;
	   }
	   
         if (cPointer != 0)
         {
            GPS_DATA.Data_Ready = 0;
   //         for (i=0; i < (unsigned char) sizeof(GPS.GGA_buffor); i++) *(cPointer+i) = 0x00;          // kk 24.09.2013 wszystki bufory musza miec taki sam rozmiar!!!
            strncpy((char *)cPointer,sTempBuf,5); // kopiowanie naglowka
            cPointer +=5;
            cStanRamki = 2; // odbrano wlasciwy poczatek ramiki
         }
         else cStanRamki = 0; // nie odebrano wlasciwej ramki
      }
      if (cStanRamki == 2)
      {
             *cPointer++ = znak;
      }

      if (cChar_counter >= GPS_BUFFOR_SIZE)
      {
         cChar_counter = 0 ; // z GPS linia nie moze byc dluzsza niz 80 znkow
         cStanRamki = 0 ;
      }

      if (znak=='\r' && cStanRamki == 2) // odebranie znaku konca linii
      {
	    *cPointer = 0x00;  // kk 02.12.2016  - dodanie na koncu zera
         GPS_DATA.Data_Ready = 1;
         cStanRamki = 0;
           
          if (GPS_DATA.GGA_Start == true)
          {
               counterSemicon =0;
               for (i=0; i<sizeof(GPS_DATA.GGA); i++)
               {
                    if (*(GPS_DATA.GGA+i)==',') counterSemicon++;
                    if (counterSemicon == 6)
				{
                         if (*(GPS_DATA.GGA+1)=='1') {GPS_DATA.Data_Fix = 1;}
				     else{ GPS_DATA.Data_Fix = 0;}
				}
               }
          }
     }
   }

}
*/
//// G P  S  ////////////////////////
void USART1_IRQHandler(void)
{
	static char cChar_counter = 0;
	static char cStanRamki = 0;
	static char sTempBuf[6];
	static uint8_t *cPointer = 0;
	unsigned char znak,i,counterSemicon;
	
  /* USER CODE BEGIN USART1_IRQn 0 */
	if(__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE) != RESET) 
	{ 	  
		  znak=(uint8_t)(huart1.Instance->DR & (uint8_t)0x00FF);

      if(znak=='$') // rozpoznanie poczatka ramiki z GPS
         {
            cStanRamki = 1;
            cPointer = 0;
            cChar_counter = 0;             //kk
         }

       if ( cStanRamki == 1 && cChar_counter < 6)
         {
           sTempBuf[cChar_counter] = znak;
         }

      if(cStanRamki > 0) cChar_counter++;
        //kk
      if (cStanRamki == 1 && cChar_counter  == 6) //odebrano oznaczenie typu ramiki
      {
        if (sTempBuf[3]=='G' && sTempBuf[4]=='G' && sTempBuf[5]=='A') 
	   {
		   cPointer = GPS_DATA.GGA;
		   GPS_DATA.GGA_Start = true;
	   }
        if (sTempBuf[3]=='G' && sTempBuf[4]=='S' && sTempBuf[5]=='A')
	   {
		   cPointer = GPS_DATA.GSA;
		   GPS_DATA.GSA_Start = true;
	   }
        if (sTempBuf[3]=='G' && sTempBuf[4]=='S' && sTempBuf[5]=='V') 
	   {
		   cPointer = GPS_DATA.GSV;
		   GPS_DATA.GSV_Start = true;
	   }
        if (sTempBuf[3]=='R' && sTempBuf[4]=='M' && sTempBuf[5]=='C') 
	   {
		   cPointer = GPS_DATA.RMC;
		   GPS_DATA.RMC_Start = true;
	   }
        if (sTempBuf[3]=='V' && sTempBuf[4]=='T' && sTempBuf[5]=='G') 
	   {
		   cPointer = GPS_DATA.VTG;
		   GPS_DATA.VTG_Start = true;
	   }
	   
         if (cPointer != 0)
         {
            GPS_DATA.Data_Ready = 0;
   //         for (i=0; i < (unsigned char) sizeof(GPS.GGA_buffor); i++) *(cPointer+i) = 0x00;          // kk 24.09.2013 wszystki bufory musza miec taki sam rozmiar!!!
            strncpy((char *)cPointer,sTempBuf,5); // kopiowanie naglowka
            cPointer +=5;
            cStanRamki = 2; // odbrano wlasciwy poczatek ramiki
         }
         else cStanRamki = 0; // nie odebrano wlasciwej ramki
      }
      if (cStanRamki == 2)
      {
             *cPointer++ = znak;
      }

      if (cChar_counter >= GPS_BUFFOR_SIZE)
      {
         cChar_counter = 0 ; // z GPS linia nie moze byc dluzsza niz GPS_BUFFOR_SIZE znkow
         cStanRamki = 0 ;
      }

      if ((znak =='\r' || znak == '\n') && cStanRamki == 2) // odebranie znaku konca linii
      {
	    *cPointer = 0x00;  // kk 02.12.2016  - dodanie na koncu zera
         GPS_DATA.Data_Ready = 1;
         cStanRamki = 0;
           
          if (GPS_DATA.GGA_Start == true)
          {
               counterSemicon =0;
               for (i=0; i<sizeof(GPS_DATA.GGA); i++)
               {
                    if (*(GPS_DATA.GGA+i)==',') counterSemicon++;
                    if (counterSemicon == 6)
				{
                         if (*(GPS_DATA.GGA+i+1)=='1')
					{
						GPS_DATA.Data_Fix = 1;
						break;
					}
				     else{ GPS_DATA.Data_Fix = 0;}
				}
               }
          }
     }
   }
		
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}




void USART2_IRQHandler(void)
{
	static uint16_t CounterReciveByte=0;
	static uint8_t j=0, temp[16];//temp_length[8];
	static uint16_t length=0;
//	static uint8_t	StanReciveDataCompleted=0;
	static bool redirect= false;
//	uint16_t temp2;
	uint8_t pch;
	

	
  /* USER CODE BEGIN USART2_IRQn 0 */
	
//	  tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
 // tmp_it_source = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);
  /* UART in mode Receiver ---------------------------------------------------*/
  if(__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE) != RESET) 
  { 
	//	temp2 = RxGSM_DATA.char_to_read;
	  
		pch = (uint8_t)(huart2.Instance->DR & (uint8_t)0x00FF);
		if ( pch == '+' || pch== '^' ) j=0;
		temp[j] = pch;
		j++;
		if (j >= sizeof(temp)) j=0;
	 
		RxGSM_DATA.DATA[RxGSM_DATA.i] = pch;
	  	RxGSM_DATA.char_to_read++;
	 	RxGSM_DATA.i++; 
		if (RxGSM_DATA.i >= sizeof(RxGSM_DATA)) RxGSM_DATA.i =0;
	  
	  
	 if (temp[0] == '+' && temp[1] == 'C' && temp[2] == 'I' && temp[3] == 'P' && temp[4] == 'R'
		 && temp[5] == 'C' && temp[6] == 'V' && temp[7] == ':' && redirect == false)
	 {
		if (temp[8] == ',' )
		{//blad 
			temp[0] = 0; temp[3] = 0; temp[8] = 0; temp[9] = 0;temp[10] = 0; temp[11] = 0; temp[12] = 0; temp[13] = 0;
			RxGSM_DATA.i=0;
			RxGSM_DATA.char_to_read=0;
			DataFromVehicom.NewData = false;
			CounterReciveByte=0;
			redirect =false;
		}
		else if (temp[9] == ',')// tylko jeden znak na liczbe danych czyli dane od 1 do 9 znakow
		{
			length= temp[8]-'0';
			CounterReciveByte=0;
			redirect =true;
			temp[0] = 0; temp[3] = 0; temp[8]=0; temp[9] = 0; temp[10] = 0; temp[11] = 0; temp[12] = 0; temp[13] = 0;
		}
		else if (temp[10] == ',')// tylko jeden znak na liczbe danych czyli dane od 10 do 99 znakow
		{
			length= (temp[8]-'0') * 10 + temp[9]-'0';
			CounterReciveByte=0;
			redirect = true;
			temp[0] = 0; temp[3] = 0; temp[8]=0; temp[9] = 0; temp[10] = 0; temp[11] = 0; temp[12] = 0; temp[13] = 0;
		}
		else if (temp[11] == ',')// tylko jeden znak na liczbe danych czyli dane od 100 do 999 znakow
		{
			length= (temp[8]-'0') * 100 + (temp[9]-'0')*10 +temp[10]-'0';
			CounterReciveByte=0;
			redirect=true;
			temp[0] = 0; temp[3] = 0; temp[8]=0; temp[9] = 0; temp[10] = 0; temp[11] = 0; temp[12] = 0; temp[13] = 0;
		}
		else if ( j> 13)
		{//blad 
			j=0;
			temp[0] = 0; temp[3] = 0; temp[8] = 0; temp[9] = 0;temp[10] = 0; temp[11] = 0; temp[12] = 0; temp[13] = 0;
			RxGSM_DATA.i=0;
			RxGSM_DATA.char_to_read=0;
			DataFromVehicom.NewData = true;
			CounterReciveByte=0;
			redirect=false;
		
		}
	}
	if (redirect == true) // przekierowanie 
	{	if (length < sizeof(DataFromVehicom.DATA))
		{		
			if (CounterReciveByte <= length)
				DataFromVehicom.DATA[CounterReciveByte++] = pch;
			else
			{
				DataFromVehicom.DATA[CounterReciveByte] =0;
				RxGSM_DATA.i=0;
				RxGSM_DATA.char_to_read=0;
				DataFromVehicom.NewData = true;
				redirect = false;
				length=0;
			}
		}
		else		// za duzo danych w porownaniu do dlugosci bufora
		{
			length=0;
			redirect = false;
			DataFromVehicom.DATA[0] = 0;
			DataFromVehicom.DATA[1] = 0;
			DataFromVehicom.DATA[0] = 0;
			DataFromVehicom.DATA[1] = 0;
			RxGSM_DATA.i=0;
		}
	 }
	 
	  // wykrycie resetu modemu
	 if (temp[0] == '^' && temp[1] == 'C' && temp[2] == 'I' && temp[3] == 'N' && temp[4] == 'I'
		 && temp[5] == 'T' && temp[6] == ':' && temp[7] == ' ' && temp[8] == '1')
	 {
		 temp[0] =0; temp[3] =0; temp[8]=0; temp[10]=0;
		 if (ModemCommand != Power_OFF && ModemCommand != Power_ON) 
		 	 GSM_status = SIM_No_init;
	 }
	  
	  // wykrcie rozBaczenia TCP IP
	  if (temp[0] == '+' && temp[1] == 'T' && temp[2] == 'C' && temp[3] == 'P' && temp[4] == 'C'
		  && temp[5] == 'L' && temp[6] == 'O' && temp[7] == 'S' && temp[8] == 'E' && temp[9] == 'D' && temp[10] == ':')
	 {
		 temp[0] =0; temp[3] =0; temp[8]=0; temp[10]=0;
		 TCP_status = No_connect_TCP;
		 RxGSM_DATA.char_to_read=0;
		 RxGSM_DATA.i=0;
	 }
 
	
/*	
		
	if (StanReciveDataCompleted == 2)
	{	
		DataFromVehicom.DATA[CounterReciveByte] = RxGSM_DATA.DATA[RxGSM_DATA.i];
		CounterReciveByte++;
		
		if (CounterReciveByte == length)
		{
			DataFromVehicom.NewData = true;
			DataFromVehicom.length = CounterReciveByte;
			StanReciveDataCompleted =0;
			RxGSM_DATA.char_to_read = temp2;
		}
	}
	if (strstr ((const char*)temp, (const char*)"+CIPRCV:") != NULL && StanReciveDataCompleted != 2 )
	{ 
		//if (StanReciveDataCompleted != 2)
		//{
			StanReciveDataCompleted = 1;
			if (temp[j] == ':') 	k =0;
			if ( temp[j] !=',' && k < 3) 	temp_length[k++] = temp[j];
			else if ( k<3 && temp[j] == ',')	
			{	
				StanReciveDataCompleted = 2;
				CounterReciveByte=0;
				length = atoi((char *)&temp_length[1]);
				temp[1] =0; temp[6] =0; temp[12]=0; temp[15]=0;
			}
		//}
	}
*/


  }
  
 // tmp_flag = __HAL_UART_GET_FLAG(huart, UART_FLAG_TXE);
//  tmp_it_source = __HAL_UART_GET_IT_SOURCE(huar it, UART_IT_TXE);
  /* UART in mode Transmitter ------------------------------------------------*/
 // if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) != RESET) 
 // {
	  	//  CLEAR_BIT(huart2.Instance->SR,UART_FLAG_TXE);
 //      UART_Transmit_IT(&huart2);
//  }
	

  /* USER CODE END USART2_IRQn 0 */
//	huart2.Instance->SR= ;
	
	HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
