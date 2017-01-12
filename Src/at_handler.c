/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "at_handler.h"
#include "swtimer.h"
#include "main.h"

/* Private variables ---------------------------------------------------------*/
uint8_t AT_RxBuff[1024];
uint8_t AT_TxBuff[512];
uint16_t AT_RxBuffRdIdx;

static uint8_t GPS_RxBuff[1024];
static uint8_t GPS_TxBuff[512];
static uint16_t GPS_RxBuffRdIdx;

/* Extern variables ----------------------------------------------------------*/
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;

extern modem_comand_type ModemCommand;

extern GSM_connect_type GSM_status;

extern GPRS_connect_type GPRS_status;

extern IP_conect_type IP_status;

extern TCP_conect_type TCP_status;

extern Vehicom_conect_type Vehicom_status;

extern Connect_type_struct ConectParameter;

extern GSM_RxBuff_struct RxGSM_DATA;

extern ErrorGSM_struct ErrorGSM;;

extern GSM_modem_struct GSM_modem;

extern uint32_t ADC[2];

/* Private function prototypes -----------------------------------------------*/
void ADC_Init(void)
{

//HAL_ADC_Start_DMA(&hadc1, uint32_t* pData, uint32_t Length);
HAL_ADC_Start_DMA (&hadc1,ADC,5);
	
}



void ATH_Init(void)
{
    /* Disable the peripheral */
/*    __HAL_DMA_DISABLE(huart2.hdmarx);
    huart2.hdmarx->Instance->CMAR = (uint32_t)&AT_RxBuff;
    huart2.hdmarx->Instance->CPAR = (uint32_t)&huart2.Instance->DR;
    huart2.hdmarx->Instance->CNDTR = sizeof(AT_RxBuff);
    
    huart2.hdmatx->Instance->CMAR = (uint32_t)&AT_TxBuff;
    huart2.hdmatx->Instance->CPAR = (uint32_t)&huart2.Instance->DR;
    
    /* Enable the peripheral */
  //  __HAL_DMA_ENABLE(huart2.hdmarx);
    
    /* Enable the DMA transfer for the receiver request by setting the DMAR bit 
    in the UART CR3 register */
   // SET_BIT(huart2.Instance->CR3, USART_CR3_DMAR);
    
    /* Enable the DMA transfer for transmit request by setting the DMAT bit
    in the UART CR3 register */
 //   SET_BIT(huart2.Instance->CR3, USART_CR3_DMAT);
    
 //   AT_RxBuffRdIdx = 0;
}

uint8_t ATH_Transmit(uint8_t* p_inBuff, uint16_t len)
{
	
	return (HAL_UART_Transmit_IT(&huart2,p_inBuff, len));
    //uint8_t status;
    
    // Status by default set to failed
 /*   status = 0;
    
    if(len < sizeof(AT_TxBuff))
    {
        if(huart2.hdmatx->Instance->CNDTR == 0)
        {
            /* Clear the TC flag in the SR register by writing 0 to it */
      /*      CLEAR_BIT(huart2.Instance->SR, USART_SR_TC);

            memcpy((void*)AT_TxBuff, (const void*)p_inBuff, len);
            /* Disable the peripheral */
       /*     __HAL_DMA_DISABLE(huart2.hdmatx);
  
            huart2.hdmatx->Instance->CNDTR = len;
            
            /* Enable the peripheral */
      /*      __HAL_DMA_ENABLE(huart2.hdmatx); 
            
            status = 1;
        }
    }
    */
  //  return status;
}
/*
bool ATH_GetTransmitStatus(void)
{

    
    // Status by default set to failed
   status = false;

    if(huart2.hdmatx->Instance->CNDTR == 0)
    {   
        status = true;
    }
    
    return status;
}
*/
int16_t ATH_Receive()
{
    uint16_t i=0,j=0;
	
	if ( RxGSM_DATA.char_to_read >= sizeof(RxGSM_DATA.CommandResponse) ) return (0);
	
	while (i< RxGSM_DATA.char_to_read)
	{	
		if (RxGSM_DATA.DATA[i] != 0 && RxGSM_DATA.DATA[i] != '\n')
		{
			if (i>= sizeof(RxGSM_DATA.DATA)) i=0;
			if (j>= 300) j=0;
			RxGSM_DATA.CommandResponse[j]= RxGSM_DATA.DATA[i];
			j++;
		}
		i++;
	}
	if (j !=0)  ErrorGSM.No_Response = 0; // wyzrowanie ilosci braku odpowiedzi
	
/*	if (strstr ((const char*)p_outBuff, (const char*)"+TCPCLOSED:") != NULL)
	{
		TCP_status = No_connect_TCP;
	}
*/
	
	RxGSM_DATA.CommandResponse[j] = 0;
	RxGSM_DATA.char_to_read=0;
	RxGSM_DATA.i=0;
	return(j);
	
//	return( HAL_UART_Receive_IT(&huart2, p_outBuff, 10));
 /*  
  	i = 0;
//	while (i< 300)	{		*(p_outBuff+i++)=0x00;		}
//	i = 0; 

    rxBuffRdIdxMax = sizeof(AT_RxBuff) - huart2.hdmarx->Instance->CNDTR;
    tempRxBuffRdIdx = AT_RxBuffRdIdx;
		
    while(AT_RxBuffRdIdx != rxBuffRdIdxMax)
    {

        if(AT_RxBuff[AT_RxBuffRdIdx] == '\r')
        {

        }       
        else if(AT_RxBuff[AT_RxBuffRdIdx] == '\n')
        {
		   p_outBuff[i] = '\n';
		   i++;
        }
        else
        {
            p_outBuff[i] = AT_RxBuff[AT_RxBuffRdIdx];
            i++;
        }
        
        AT_RxBuffRdIdx++;
        
        if(AT_RxBuffRdIdx == sizeof(AT_RxBuff))
        {
            AT_RxBuffRdIdx = 0;
        }

    }
    
	if (strstr ((const char*)p_outBuff, (const char*)"+TCPCLOSED:") != NULL)
	{
		TCP_status = No_connect_TCP;
	}

	if (i !=0)  ErrorGSM.No_Response = 0; // wyzrowanie ilosci braku odpowiedzi
	
	p_outBuff[i] = 0;*/
 //   return i;
}
////////////////////////////////////////////////////////////////////// G P S ////////////////////////////////////////////////////////////////
void GPS_HandlerInit(void)
{
    /* Disable the peripheral */
    __HAL_DMA_DISABLE(huart1.hdmarx);
    huart1.hdmarx->Instance->CMAR = (uint32_t)&GPS_RxBuff;
    huart1.hdmarx->Instance->CPAR = (uint32_t)&huart1.Instance->DR;
    huart1.hdmarx->Instance->CNDTR = sizeof(GPS_RxBuff);
    
    huart1.hdmatx->Instance->CMAR = (uint32_t)&GPS_TxBuff;
    huart1.hdmatx->Instance->CPAR = (uint32_t)&huart1.Instance->DR;
    
    /* Enable the peripheral */
    __HAL_DMA_ENABLE(huart1.hdmarx);
    
    /* Enable the DMA transfer for the receiver request by setting the DMAR bit 
    in the UART CR3 register */
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
    
    /* Enable the DMA transfer for transmit request by setting the DMAT bit
    in the UART CR3 register */
    SET_BIT(huart1.Instance->CR3, USART_CR3_DMAT);
    
    GPS_RxBuffRdIdx = 0;
}

uint16_t GPS_HandlerReceive(uint8_t* p_outBuff)
{
    uint8_t state;
    uint16_t rxBuffRdIdxMax;
    uint16_t i;
    uint16_t tempRxBuffRdIdx;
	//// przerobic to na przerwania :)
	// przenie[ zmiany funkcji init portu + z plku stm32f1zz_hal_msp.c !!!!
    state = 0;
    i = 0;
    rxBuffRdIdxMax = sizeof(GPS_RxBuff) - huart1.hdmarx->Instance->CNDTR;
    tempRxBuffRdIdx = GPS_RxBuffRdIdx;
    
    while(GPS_RxBuffRdIdx != rxBuffRdIdxMax)
    {
        if((GPS_RxBuff[GPS_RxBuffRdIdx] == '\r') && ((state == 0) || (state == 2)))
        {
            state++;
        }        
        else if((GPS_RxBuff[GPS_RxBuffRdIdx] == '\n') && ((state == 1) || (state == 3)))
        {
            state++;
            if(i > 0)
            {
                if(GPS_RxBuffRdIdx == sizeof(GPS_RxBuff))
                {
                    GPS_RxBuffRdIdx = 0;
                }
                break;
            }
        }
        else if(state == 2)
        {
            *p_outBuff = GPS_RxBuff[GPS_RxBuffRdIdx];
            p_outBuff++;
            i++;
        }
        else
        {
            
        }
        
         GPS_RxBuffRdIdx++;
        
        if(GPS_RxBuffRdIdx == sizeof(GPS_RxBuff))
        {
            GPS_RxBuffRdIdx = 0;
        }
        
        if(GPS_RxBuffRdIdx == rxBuffRdIdxMax)
        {
            GPS_RxBuffRdIdx = tempRxBuffRdIdx;
            i = 0;
            break;
        }
    }
    return i;
}
