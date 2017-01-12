/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>

#include "stm32f1xx_hal.h"
#include "swtimer.h"
#include "lowlevelinit.h"
#include "at_handler.h"
#include "modem.h"
#include "main.h"
#include "GPS.h"
#define CHECK_IP_WWW "AT+CDNSGIP=www.vehicom.pl\r"
#define IP_WWW "www.vehicom.pl"		// na potrzeby poszukiwania adresu IP w odpowiedzi - musi byc takie samo jak linika wyzej

/* Private Constants ---------------------------------------------------------*/

/* Private types -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void MODEM_Reset(void);
 bool  AnalizeData(uint8_t *buff, ServerData_struct *DataFromVehicom);


/* Private variables ---------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
extern modem_comand_type ModemCommand;

extern ServerData_struct DataFromVehicom;

extern GSM_connect_type GSM_status;

extern GPRS_connect_type GPRS_status;

extern IP_conect_type IP_status;

extern TCP_conect_type TCP_status;

extern Vehicom_conect_type Vehicom_status;

extern Connect_type_struct ConectParameter;

extern uint8_t RxServerData[512];

extern Timery_type_struct Timery;

extern ErrorGSM_struct ErrorGSM;

extern struct_GPS	GPS_DATA;

extern GSM_RxBuff_struct RxGSM_DATA;

static uint8_t HeaderDataToSEND[15] = "AT+CIPSEND\r";


uint8_t MODEM_ServerData1[30] = "##,IMEI:359586015829802,A;";
uint8_t MODEM_ServerData2[75] = "IMEI:359586015829802,tracker,000,000,000.13554900601,L,;";

//HeaderDataToSEND = (uint8_t*)"AT+CIPSEND\r";
/*
const modem_cmd_table_t MODEM_CmdTable[] = {
    {
        .cmd = MODEM_CMD_CMEE, // zwraca teksowo info o bledzie + nr bledu
        .p_atStr = (uint8_t*)"AT+CMEE=2\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK, 
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 200,
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_ECHO_OFF
    },
    
    {
        .cmd = MODEM_CMD_ECHO_OFF, // wylaczenie echa
        .p_atStr = (uint8_t*)"ATE0\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK, 
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 200,
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_CCID_REQ
    },
    
    {
        .cmd = MODEM_CMD_CCID_REQ, 
        .p_atStr = (uint8_t*)"AT+CCID\r", 
        .p_respStr = (uint8_t*)"+SCID: SIM", 
        .respFlag = MODEM_RESP_OK | MODEM_RESP_PROMPT, 
        .retryCnt = 3,
        .timeout = 3000,
        .pfn_cmdFrameCb = NULL,
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
//        .pfn_retryCmpltCb = MODEM_CmdCCIDReq_RetryCmplt_Cb,
        .cmdNext = MODEM_CMD_CPIN_REQ
    },
    
    {
        .cmd = MODEM_CMD_CPIN_REQ, 
        .p_atStr = (uint8_t*)"AT+CPIN?\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK | MODEM_RESP_PROMPT, 
        .retryCnt = 1,
        .timeout = 500,
        .pfn_cmdFrameCb = NULL, 
 //       .pfn_respCb = MODEM_CmdCPIN_Req_Resp_Cb, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_DEFAULT
    },
    
    {
        .cmd = MODEM_CMD_CPIN_ENTR, 
        .p_atStr = (uint8_t*)"AT+CPIN=\"2134\"\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK, 
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 500,
        .pfn_cmdFrameCb = NULL,
        .pfn_respCb = NULL, 
 //       .pfn_errorCb = MODEM_CmdCPIN_Entr_Error_Cb,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_CREG
    },
    
    {
        .cmd = MODEM_CMD_CREG, 
        .p_atStr = (uint8_t*)"AT+CREG?\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK | MODEM_RESP_PROMPT,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 2000, 
        .pfn_cmdFrameCb = NULL, 
 //       .pfn_respCb = MODEM_CmdCREG_Resp_Cb, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_CGATT_REQ
    },

    {
        .cmd = MODEM_CMD_CGATT_REQ, 
        .p_atStr = (uint8_t*)"AT+CGATT?\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK | MODEM_RESP_PROMPT,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 2000, 
        .pfn_cmdFrameCb = NULL, 
     //   .pfn_respCb = MODEM_CmdCGATT_Req_Resp_Cb, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_DEFAULT
    },
    
    {
        .cmd = MODEM_CMD_CGATT_DEATTACH, 
        .p_atStr = (uint8_t*)"AT+CGATT=0\r", 
        .p_respStr = (uint8_t*)NULL,
        .respFlag = MODEM_RESP_OK,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 5000, 
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_CGATT_REQ
    },
    
    {
        .cmd = MODEM_CMD_CGATT_ATTACH, 
        .p_atStr = (uint8_t*)"AT+CGATT=1\r", 
        .p_respStr = (uint8_t*)NULL,
        .respFlag = MODEM_RESP_OK,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 5000, 
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_CGDCONT
    },
    
    {
        .cmd = MODEM_CMD_CGDCONT, 
        .p_atStr = (uint8_t*)"AT+CGDCONT=1,\"IP\",\"internet\"\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 200, 
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_CSTT
    },
    
    {
        .cmd = MODEM_CMD_CSTT, 
        .p_atStr = (uint8_t*)"AT+CSTT=\"internet\",\"internet\",\"internet\"\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 200, 
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_CGACT_REQ
    },
    
    {
        .cmd = MODEM_CMD_CGACT_REQ, 
        .p_atStr = (uint8_t*)"AT+CGACT?\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK | MODEM_RESP_PROMPT,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 500, 
        .pfn_cmdFrameCb = NULL, 
 //       .pfn_respCb = MODEM_CmdCGACT_Req_Resp_Cb, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_DEFAULT
    },
    
    {
        .cmd = MODEM_CMD_CGACT_DEACT, 
        .p_atStr = (uint8_t*)"AT+CGACT=0,1\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 5000, 
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_CGACT_REQ
    },
    
    {
        .cmd = MODEM_CMD_CGACT_ACT, 
        .p_atStr = (uint8_t*)"AT+CGACT=1,1\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 5000, 
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_CGACT_REQ
    },
    
    {
        .cmd = MODEM_CMD_CIFSR, 
        .p_atStr = (uint8_t*)"AT+CIFSR\r", 
        .p_respStr = (uint8_t*)".", 
        .respFlag = MODEM_RESP_OK | MODEM_RESP_PROMPT,
        .retryCnt = 1,
        .timeout = 5000, 
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
  //      .pfn_retryCmpltCb = MODEM_CmdCIFSR_RetryCmplt_Cb,
        .cmdNext = MODEM_CMD_CIPSTART
    },
    
    {
        .cmd = MODEM_CMD_CDNSGIP, 
        .p_atStr = (uint8_t*)"AT+CDNSGIP=www.onet.pl\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK | MODEM_RESP_PROMPT,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 5000, 
        .pfn_cmdFrameCb = NULL, 
   //     .pfn_respCb = MODEM_CmdCDNSGIP_Resp_Cb, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_DEFAULT
    },
    
    {
        .cmd = MODEM_CMD_CIPSTART, 
        .p_atStr = (uint8_t*)"AT+CIPSTART=\"TCP\",\"217.149.242.242\",12103\r", 
        .p_respStr = (uint8_t*)"CONNECT OK", 
        .respFlag = MODEM_RESP_OK | MODEM_RESP_PROMPT,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 5000, 
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_CIPSEND_START_PH1
    },
    
    {
        .cmd = MODEM_CMD_CIPSEND_START_PH1, 
        .p_atStr = MODEM_CmdFrameBuff,
        .p_respStr = (uint8_t*)">", 
        .respFlag = MODEM_RESP_PROMPT,
        .retryCnt = 1,
        .timeout = 500, 
   //     .pfn_cmdFrameCb = MODEM_CmdFrame_Cb, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
  //      .pfn_retryCmpltCb = MODEM_CmdCIPSEND_RetryCmplt_Cb,
        .cmdNext = MODEM_CMD_CIPSEND_PH1
    },
    
    {
        .cmd = MODEM_CMD_CIPSEND_PH1, 
        .p_atStr = MODEM_ServerData1,//(uint8_t*)"## IMEI: 359586015829802, A;", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK,
        .retryCnt = 1,
        .timeout = 5000, 
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
     //   .pfn_retryCmpltCb = MODEM_CmdCIPSEND_RetryCmplt_Cb,
        .cmdNext = MODEM_CMD_NIL
    },
    
    {
        .cmd = MODEM_CMD_CIPSEND_START_PH2, 
        .p_atStr = MODEM_CmdFrameBuff,
        .p_respStr = (uint8_t*)">", 
        .respFlag = MODEM_RESP_PROMPT,
        .retryCnt = 1,
        .timeout = 500, 
   //     .pfn_cmdFrameCb = MODEM_CmdFrame_Cb, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
   //     .pfn_retryCmpltCb = MODEM_CmdCIPSEND_RetryCmplt_Cb,
        .cmdNext = MODEM_CMD_CIPSEND_PH2
    },
    
    {
        .cmd = MODEM_CMD_CIPSEND_PH2, 
        .p_atStr = MODEM_ServerData2,//(uint8_t*)"IMEI: 359586015829802, tracker, 000,000,000.13554900601, L,;",
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK,
        .retryCnt = 1,
        .timeout = 5000, 
        .pfn_cmdFrameCb = NULL, 
        .pfn_respCb = NULL, 
        .pfn_errorCb = NULL,
 //       .pfn_retryCmpltCb = MODEM_CmdCIPSEND_RetryCmplt_Cb,
        .cmdNext = MODEM_CMD_NIL
    },
    
    {
        .cmd = MODEM_CMD_CSQ, 
        .p_atStr = (uint8_t*)"AT+CSQ\r", 
        .p_respStr = (uint8_t*)NULL, 
        .respFlag = MODEM_RESP_OK | MODEM_RESP_PROMPT,
        .retryCnt = MODEM_RETRY_FOREVER,
        .timeout = 3000, 
        .pfn_cmdFrameCb = NULL, 
  //      .pfn_respCb = MODEM_CmdCSQ_Resp_Cb, 
        .pfn_errorCb = NULL,
        .pfn_retryCmpltCb = NULL,
        .cmdNext = MODEM_CMD_NIL
    },
};
*/

/* Extern variables ----------------------------------------------------------*/

void MODEM_Init(void)
{    
	swtimer_t timer;
	
	ErrorGSM.Error =0;
	ErrorGSM.Fail=0;
	ErrorGSM.No_Response=0;
	
	SWTimer_Stop(&timer);
	HAL_GPIO_WritePin(GSM_PWR_GPIO_Port, GSM_PWR_Pin, GPIO_PIN_RESET);
     SWTimer_Start(&timer, 2200);
	while(SWTimer_GetStatus((&timer)) != SWTIMER_ELAPSED);
	
//	while(SWTimer_GetStatus((&timer)) != SWTIMER_ELAPSED);

	HAL_GPIO_WritePin(GSM_PWR_GPIO_Port, GSM_PWR_Pin, GPIO_PIN_SET);
     SWTimer_Start(&timer, 2200);
	while(SWTimer_GetStatus((&timer)) != SWTIMER_ELAPSED);
 //   HAL_GPIO_WritePin(GSM_PWR_GPIO_Port, GSM_PWR_Pin, GPIO_PIN_RESET);
	
}

void MODEM_Reset(void)
{    
	swtimer_t timer;
	
	ErrorGSM.Error =0;
	ErrorGSM.Fail=0;
	ErrorGSM.No_Response=0;
    
	SWTimer_Stop(&timer);
	
	HAL_GPIO_WritePin(GSM_RST_GPIO_Port, GSM_RST_Pin, GPIO_PIN_SET);
	SWTimer_Start(&timer, 1000);

	while(SWTimer_GetStatus((&timer)) != SWTIMER_ELAPSED);

	HAL_GPIO_WritePin(GSM_RST_GPIO_Port, GSM_RST_Pin, GPIO_PIN_RESET);
	SWTimer_Start(&timer, 13000);

	while(SWTimer_GetStatus((&timer)) != SWTIMER_ELAPSED);
	SWTimer_Stop(&timer);

}

uint8_t send_Command(uint8_t *p_atStr, uint16_t delay)
{
	static swtimer_t timer;
	static uint8_t once_send = 0;
	
	if (once_send == 0) // nadawanie
	{
		once_send = 1;
		ATH_Transmit((uint8_t*)p_atStr, strlen((const char*)p_atStr));
		SWTimer_Start(&timer, delay);
		return(0);
	}
	else
	{ // odbior
		if (SWTimer_GetStatus(&timer) == SWTIMER_ELAPSED)
		{
			once_send = 0;
			return(1);
		}
	}
	return(0);
}

void MODEM_Job(void)
{
	static uint8_t /*HeaderDataToSEND[30],*/ *temp;
//	static uint8_t DataToSEND[500];
	//static uint8_t Repeat;
	uint8_t  *pch, i;
	uint16_t len;
	uint8_t* p_atStr;
	
//	ATH_Transmit((uint8_t*)p_MODEM_CmdTable->p_atStr, strlen((const char*)p_MODEM_CmdTable->p_atStr));
	switch(ModemCommand)
	{
		case (Power_OFF):
			MODEM_Init();
			ModemCommand = Power_ON;
		break;
		
		case (Power_ON):
			MODEM_Reset();
			ATH_Receive(); // odebranie iformacji od modemu na starcie
			ModemCommand = After_Reset;
		break;
		
		case(After_Reset):// w zasadzie modem po resecie sam sie laczy z siecia i tu juz 
			//jest polaczony z siecia GSM wystarczy wyszukac w buforze "+CREG: 1" lub "5"
			p_atStr = (uint8_t*)"AT+CMEE=2\r"; 
			if (send_Command(p_atStr,200) == 1) // nei zmniejszac tego czasu
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					RxGSM_DATA.CommandResponse[0] ='0';
					if  (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0)
					{
						ErrorGSM.Fail = 0;
						ErrorGSM.Error = 0;
						ErrorGSM.No_Response=0;
						ModemCommand = Echo_OFF;
						break;
					}
					ErrorGSM.Error++;
					break;
				}
				else 	ErrorGSM.No_Response++;
			}
			/*
			else if (Repeat >3)
			{
				Repeat = 0;
				// Reset procka dogac!!!!
			}*/
		break;
			
		case(Echo_OFF):
			p_atStr = (uint8_t*)"ATE0\r"; 
			if (send_Command(p_atStr,200) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0)
					{
						ErrorGSM.Fail = 0;
						ErrorGSM.Error = 0;
						ModemCommand = AGPS_ON;
						break;
					}
					ErrorGSM.Error++;
					break;
				}
				else 	ErrorGSM.No_Response++;
			}
		break;
			
		case(AGPS_ON):
			p_atStr = (uint8_t*)"AT+GPS=1\r"; 
			if (send_Command(p_atStr,2000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
			/*		if (strstr ((const char*)buff, (const char*)"OK") != 0)
					{*/
						ErrorGSM.Fail = 0;
						ErrorGSM.Error = 0;
						ModemCommand = CheckPINneed;
						break;
				/*	}
					ErrorGSM.Error++;
					break;*/
				}
				else 	ErrorGSM.No_Response++;
			}
		break;
			
		case(GPS_FREQ):
			p_atStr = (uint8_t*)"AT+GPSRD = 5\r"; 
			if (send_Command(p_atStr,3000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0)
					{
						ErrorGSM.Fail = 0;
						ErrorGSM.Error = 0;
						ModemCommand = CheckPINneed;
						break;
					}
					ErrorGSM.Error++;
					break;
				}
				else 	ErrorGSM.No_Response++;
			}
		break;
			
			
		case(CheckPINneed):
			p_atStr = (uint8_t*)"AT+CPIN?\r";
			if (send_Command(p_atStr,500) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"+CPIN:READY") != 0)
					{
						ErrorGSM.Fail = 0;
						ErrorGSM.Error = 0;
						ModemCommand = ReadIMEI;
						break;
					}
					if (strstr ((const char*)RxGSM_DATA.CommandResponse,(const char*) "+CPIN:SIM PIN") != 0)
					{
						ErrorGSM.Fail = 0;
						ErrorGSM.Error = 0;
						ModemCommand = PIN_is_need;
						break;
					}
					if (strstr((const char*)RxGSM_DATA.CommandResponse,(const char*)"ERROR:SIM not inserted") != 0)
					{
						SWTimer_Start(&Timery.timer, 3000);
						while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
						ErrorGSM.Error++;
						break;
					}
					if (strstr((const char*)RxGSM_DATA.CommandResponse,(const char*)"ERROR") != 0)
					{
						SWTimer_Start(&Timery.timer, 3000);
						while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
						ErrorGSM.Error++;
						break;
					}
				}
				else 	ErrorGSM.No_Response++;
			}		
		break;

		case(ReadIMEI):
			p_atStr = (uint8_t*)"AT+EGMR=2,7\r";
			if (send_Command(p_atStr,200) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"+EGMR:") != 0)
					{
						ErrorGSM.Fail = 0;
						ErrorGSM.Error = 0;
						strchr((const char*)RxGSM_DATA.CommandResponse,':');
						strncpy((char*)ConectParameter.IMEI, strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"+EGMR:")+6, 15); //+ zero
						ModemCommand = ReadIMSI ;
						break;
					}
					if (strstr((const char*)RxGSM_DATA.CommandResponse,(const char*)"ERROR") != 0 )
					{
						ErrorGSM.Error++;
						break;
					}
				}
				else 	ErrorGSM.No_Response++;
			}	
		break;
			
		case(ReadIMSI):
			p_atStr = (uint8_t*)"AT+CIMI\r";
			if (send_Command(p_atStr,200) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0)
					{
						ErrorGSM.Fail = 0;
						ErrorGSM.Error = 0;
				//		strchr((const char*)buff,':');
						strncpy((char*)ConectParameter.IMSI, strchr((const char*)RxGSM_DATA.CommandResponse,'\r')+1, 15); //+ zero
						GSM_status = No_connectGSM; //sim zarejestrowany
						break;
					}
					if (strstr((const char*)RxGSM_DATA.CommandResponse,(const char*)"ERROR") != 0 )
					{
						ErrorGSM.Error++;
						break;
					}
				}
				else 	ErrorGSM.No_Response++;
			}		
		break;	
			
		case(Network_registration_check):
			p_atStr = (uint8_t*)"AT+CREG?\r";
			if (send_Command(p_atStr,1500) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"+CREG:") != 0)
					{
						ErrorGSM.Fail = 0;
						ErrorGSM.Error = 0;
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)",1") != 0 || (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)",5") != 0) )
						{//  w sieci GSM
							GSM_status	= ConnectGSM;
//							ModemCommand = CheckSignalQuality ;
							break;
						}
						else if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)",2") != 0)
						{
							ModemCommand = Network_registrationDo;
							ErrorGSM.Fail++;
							break;
						}
					}
					if (strstr((const char*)RxGSM_DATA.CommandResponse,(const char*)"ERROR:") != 0)
					{
						ErrorGSM.Error++;
						break;
					}
				}
				else 	ErrorGSM.No_Response++;
			}	
		break;
			
		case(NetworkRegistrationDisconect):
			p_atStr = (uint8_t*)"AT+CREG=0\r";
			if (send_Command(p_atStr,2000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"+CREG:") != 0)
					{
						ErrorGSM.Error = 0;
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)",1") != 0 || (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)",5") != 0) )
						{//  w sieci GSM
							ErrorGSM.Fail = 0;
							GSM_status	= No_connectGSM;
							break;
						}
						else if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)",2") != 0)
						{
							ErrorGSM.Fail++;
							break;
						}
					}
					if (strstr((const char*)RxGSM_DATA.CommandResponse,(const char*)"ERROR:") != 0)
					{
						ErrorGSM.Error++;
						break;
					}
				}
				else 	ErrorGSM.No_Response++;
			}	
		break;
			
		case(Network_registrationDo):
			p_atStr = (uint8_t*)"AT+CREG=1\r";
			if (send_Command(p_atStr,2000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0)
					{
						ErrorGSM.Error = 0;
						ErrorGSM.Fail = 0;
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)",1") != 0 || (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)",5") != 0) )
						{//  w sieci GSM
							GSM_status	= ConnectGSM;
							break;
						}
					//	else if (strstr ((const char*)buff, (const char*)",2") != 0)
						{
							GSM_status	= No_connectGSM;
						}
					}
					if (strstr((const char*)RxGSM_DATA.CommandResponse,(const char*)"ERROR:") != 0)
					{
						ErrorGSM.Error++;
						break;
					}
				}
				else 	ErrorGSM.No_Response++;
			}
		break;
			
		case(CheckSignalQuality):
			p_atStr = (uint8_t*)"AT+CSQ\r";
			if (send_Command(p_atStr,500) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					temp=(uint8_t*)strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"+CSQ:");
					if  (temp != 0)
					{
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0 )
						{
							ErrorGSM.Error = 0;
							ErrorGSM.Fail = 0;
							char  *pch;
							pch = (char *)strtok ((char *)temp+5,(char *)","); 
							ConectParameter.SignnalQuality = atoi(pch);
							break;
						}
						else if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0)
						{
							ErrorGSM.Error++;
							break;
						}
					}
					ErrorGSM.Fail++;
					break;
					
				}
				else 	ErrorGSM.No_Response++;
			}	
		break;
			
		case(CheckGPRS):
			p_atStr = (uint8_t*)"AT+CGATT?\r";
			if (send_Command(p_atStr,2000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					temp=(uint8_t*)strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"+CGATT:");
					if  (temp != 0)
					{
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0 )
						{
							ErrorGSM.Error = 0;
							ErrorGSM.Fail = 0;
							if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"1") != 0 )
							{
								GPRS_status	= ConnectGPRS;
								break;
							}
							if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"0") != 0 )
							{
								ModemCommand = LoginToGPRS;
								break;
							}
						}
						else if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0)
						{
							ErrorGSM.Error++;
							break;
						}
					}
					ErrorGSM.Fail++;
					break;				
				}
				else 	ErrorGSM.No_Response++;
			}	
		break;
			
		case(LoginToGPRS):
			p_atStr = (uint8_t*)"AT+CGATT=1\r";
			if (send_Command(p_atStr,5000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					temp=(uint8_t*)strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"+CTZV:");
					if  (temp != 0)
					{
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0 )
						{
							ErrorGSM.Error = 0;
							ErrorGSM.Fail = 0;
							GPRS_status	= ConnectGPRS;
							break;
						}

					}
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0)
					{
						ModemCommand = Network_registrationDo;
						GPRS_status = No_connectGPRS;
						ErrorGSM.Error++;
						break;
					}
					ErrorGSM.Fail++;
					break;
					
				}
				else 	ErrorGSM.No_Response++;
			}
		break;	
			
		case(DisconectGPRS_Do):
			p_atStr = (uint8_t*)"AT+CGATT=0\r";
			if (send_Command(p_atStr,8000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0 )
					{
						ErrorGSM.Error = 0;
						ErrorGSM.Fail = 0;
					/*	if (strstr ((const char*)buff, (const char*)"0") != 0 )
						{
							GPRS_status	= No_connectGPRS;
							break;
						}
						else 
						{*/
							GPRS_status	= No_connectGPRS;
						//	ErrorGSM.Fail++;
							break;
						//}
					}
					else if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0)
					{
						ErrorGSM.Error++;
						break;
					}
				}
				else 	ErrorGSM.No_Response++;
			}
		break;

			
		case(SetUp_Apn_Id_Pass):
			p_atStr = (uint8_t*)"AT+CSTT=\"internet\",\"internet\",\"internet\"\r";
			if (send_Command(p_atStr,500) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					temp=(uint8_t*)strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK");
					if  (temp != 0)
					{
						ErrorGSM.Error = 0;
						ErrorGSM.Fail = 0;
						ModemCommand = SetPDPparameter;
						break;
					}
					ErrorGSM.Error++;
					break;
				}
				else 	ErrorGSM.No_Response++;
			}
		break;
			
		case(SetPDPparameter):
			p_atStr = (uint8_t*)"AT+CGDCONT=1,\"IP\",\"internet\"\r";
			if (send_Command(p_atStr,1000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					temp=(uint8_t*)strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK");
					if  (temp != 0)
					{
						ErrorGSM.Error = 0;
						ErrorGSM.Fail = 0;
						ModemCommand = ActivePDPparameter;
						break;
					}
					ErrorGSM.Error++;
					break;
				}
				else 	ErrorGSM.No_Response++;
			}
		break;
			
		case(ActivePDPparameter):
			p_atStr = (uint8_t*)"AT+CGACT=1,1\r";
			if (send_Command(p_atStr,1000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					temp=(uint8_t*)strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK");
					if  (temp != 0)
					{
						ErrorGSM.Error = 0;
						ErrorGSM.Fail = 0;
						ModemCommand = CheckIPstatus;

					}
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0)
					{
						ModemCommand = DisconectGPRS_Do;
						GPRS_status = Try_connect_GPRS;
						ErrorGSM.Error++;
						break;
					
					}
					ErrorGSM.Error++;
					break;	
				}
				else 	ErrorGSM.No_Response++;
			}
		break;
			
		case(CheckIPstatus):
			p_atStr = (uint8_t*)"AT+CIPSTATUS\r";
			if (send_Command(p_atStr,1000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					temp=(uint8_t*)strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"+CIPSTATUS:");
					if  (temp != 0)
					{
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0)
						{
							ErrorGSM.Error = 0;
							if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"IP START") != 0)
								{
									ModemCommand =ConnectToTCPadress; 
									break;
								}
							if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"IP GPRSACT") != 0)
							{

								if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"IP INITIAL") != 0 )
								{
									ModemCommand = CheckMyIP;
									ErrorGSM.Fail=0;;
									break;
								}
								if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"CONNECT") != 0 )
								{
									IP_status	= Connect_IP;
									ErrorGSM.Fail=0;;
									break;
								}
							} 
							if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"CONNECT") != 0 )
								{
									IP_status	= Connect_IP;
									TCP_status = Connect_TCP;
									ModemCommand = Wait;
								//	ModemCommand = ConnectToTCPadress;
									ErrorGSM.Fail=0;;
									break;
								}
							else
							{
								ModemCommand = Disconect_IP;
								ErrorGSM.Fail++;
								ErrorGSM.Error++;
								break;
							}
						}
						else if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0)
						{
							GPRS_status = No_connectGPRS;
							ErrorGSM.Error++;
							break;
						}
					}
					ErrorGSM.Fail++;
				}
				else 	ErrorGSM.No_Response++;
			}	
		break;
			
		case(Disconect_IP):
			p_atStr = (uint8_t*)"AT+CIPSHUT\r";
			if (send_Command(p_atStr,3000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0)
					{
						ErrorGSM.Error = 0;
						ErrorGSM.Fail = 0;
						ModemCommand = ActivePDPparameter;
						break;
					}
					else if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0)
					{
						GPRS_status = No_connectGPRS;
						ModemCommand = Wait;
				//	GPRS_status	= No_connectGPRS;
						ErrorGSM.Error++;
						break;
					}
					ErrorGSM.Fail++;
				}
				else 	ErrorGSM.No_Response++;
			}
		break;	
			
		case(CheckMyIP):
			p_atStr = (uint8_t*)"AT+CIFSR\r";
			if (send_Command(p_atStr,400) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0 )
					{
						i=0;
						ErrorGSM.Error = 0;
						ErrorGSM.Fail = 0;
						while(RxGSM_DATA.CommandResponse[i+1] != 0x0A && i< 15 && RxGSM_DATA.CommandResponse[i+1] != '\r')
						{
							ConectParameter.MyIP[i]=RxGSM_DATA.CommandResponse[1+i];
							i++;
						}
							
						ModemCommand = GetIPDomain;
						break;
					}
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0)
					{
						ErrorGSM.Error++;
						break;
					}
					ErrorGSM.Fail++;
				}				
				else 	ErrorGSM.No_Response++;
			}
		break;
			
		case(GetIPDomain):
			p_atStr = (uint8_t*)CHECK_IP_WWW;
			if (send_Command(p_atStr,8000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0 )
						{
							pch = (uint8_t*)strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)IP_WWW);
							if (pch != 0 )
							{
								ErrorGSM.Error = 0;
								ErrorGSM.Fail = 0;
								pch+= strlen((const char*)IP_WWW);
								pch+=3; 
								i=0;
								while(pch[i] != '\"' && i<15)
								{
									(ConectParameter.wwwIP[i]=pch[i]);
									i++;
								}
								IP_status = Connect_IP;
								break;
							}
						}
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0)
						{
							GPRS_status = No_connectGPRS;
							ErrorGSM.Error++;
							break;
						}
						ErrorGSM.Fail++;
				}				
				else 	ErrorGSM.No_Response++;
			}
		break;	
			
		case(ConnectToTCPadress):
			p_atStr = (uint8_t*)"AT+CIPSTART=\"TCP\",\"217.149.242.242\",12103\r";
			if (send_Command(p_atStr,19000) == 1)
			{
				len = ATH_Receive();
				if (len != 0) 
				{
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"CONNECT OK") != 0 )
						{
								TCP_status = Connect_TCP;
								ErrorGSM.Error = 0;
								ErrorGSM.Fail = 0;
								ModemCommand = Wait;
								break;
						}
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"CONNECT FAIL") != 0)
						{
							TCP_status = No_connect_TCP;
							ErrorGSM.Fail++;
							break;
						} 
						if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0)
						{
							ModemCommand = Disconect_IP;
					//		IP_status = No_connect_IP;
							TCP_status = No_connect_TCP;
							ErrorGSM.Fail++;
							break;
						
						}
						TCP_status = No_connect_TCP;
						ErrorGSM.Error++;
						break;
						
				}				
				else 	ErrorGSM.No_Response++;
			}
		break;	
			
		case(CommandStartSendDataToServer):
			p_atStr = (uint8_t*)HeaderDataToSEND;
			if (send_Command(p_atStr,800) == 1)
			{
				len = ATH_Receive();
				while (len < (uint16_t)0) len = ATH_Receive();
				if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)">") != 0) 
				{
					ModemCommand = CommandSendDataToServer;
					ErrorGSM.Error = 0;
					ErrorGSM.Fail = 0;
					break;
				}	
				else
				{
					TCP_status = No_connect_TCP;
				}
			}
		break;

		case(CommandSendDataToServer):
			p_atStr = (uint8_t*)RxGSM_DATA.DataToSEND;
			if (send_Command(p_atStr,200) == 1)
			{
				len = ATH_Receive();
				if (len != NULL)
				{	
					if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"OK") != 0) 
					{
						ModemCommand = Wait;///
						ErrorGSM.Error = 0;
						ErrorGSM.Fail = 0;
						break;
					}				
					else if (strstr ((const char*)RxGSM_DATA.CommandResponse, (const char*)"ERROR") != 0) 
					{
						TCP_status = No_connect_TCP;
						ErrorGSM.Error++;
						break;
					}
					ErrorGSM.Fail++;
				}					
				else 	ErrorGSM.No_Response++;
			}
		break;
		case(WaitForResponse):// from Server Vehicom
			if (SWTimer_GetStatus(&Timery.WaitForResponseTimer) == SWTIMER_ELAPSED)
			{

			}
		break;	
			
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (ErrorGSM.ErrorVehicom > 3) 
	{
		SWTimer_Start(&Timery.timer,5000);
		while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
		GSM_status = SIM_No_init;
		ErrorGSM.ErrorVehicom=0;
		ErrorGSM.hardReset++;
	}
	
	if (ErrorGSM.ErrorTCP > 3)
	{
		SWTimer_Start(&Timery.timer,5000);
		while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
		GSM_status = SIM_No_init;
		ErrorGSM.hardReset++;
		ErrorGSM.ErrorTCP=0;
	}
	
	if (ErrorGSM.ErrorIP > 3)
	{
		SWTimer_Start(&Timery.timer,5000);
		while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
		GSM_status = SIM_No_init;
		ErrorGSM.hardReset++;
		ErrorGSM.ErrorIP=0;
	}
	
	if (ErrorGSM.ErrorGPRS > 3)
	{
		SWTimer_Start(&Timery.timer,5000);
		while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
		GSM_status = SIM_No_init;
		ErrorGSM.hardReset++;
		ErrorGSM.ErrorGPRS=0;
	}
	
	if (ErrorGSM.ErrorGSM > 3)
	{
		SWTimer_Start(&Timery.timer,5000);
		while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
		GSM_status = SIM_No_init;
		ErrorGSM.hardReset++;
		ErrorGSM.ErrorGSM=0;
	}
	
	if (ErrorGSM.Fail > 7 ) 
	{
		SWTimer_Start(&Timery.timer,5000);
		while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
		GSM_status = SIM_No_init;
		ErrorGSM.hardReset++;
		ErrorGSM.ErrorGSM=0;
	}
	
	if (ErrorGSM.Error > 3) // reste
	{	
	/*	if (ModemCommand == ConnectToTCPadress)
		{
			TCP_status = No_connect_TCP;
			IP_status = No_connect_IP;

			SWTimer_Start(&Timery.timer,5000);
			while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
			ErrorGSM.Error =0;
		}
		else*/
		{
			SWTimer_Start(&Timery.timer, ErrorGSM.Error*5000);
			while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
			ErrorGSM.Error = 0;
			GSM_status = SIM_No_init;
			ErrorGSM.hardReset++;
		}
	}
	if (ErrorGSM.No_Response >3)
	{
		SWTimer_Start(&Timery.timer,5000);
		while(SWTimer_GetStatus((&Timery.timer)) != SWTIMER_ELAPSED);
		
		GSM_status=SIM_No_init;
	}
	
	if (ErrorGSM.hardReset >3)
	{
	//	restart procesota
	}
	if (ErrorGSM.ErrorVehicom > 1)
	{
		ErrorGSM.ErrorVehicom =0;
		ModemCommand = CheckIPstatus;
		if (ErrorGSM.ErrorVehicom > 2) ModemCommand = Disconect_IP;
	}
	
	switch(GSM_status)
	{
		case(SIM_No_init):
			ModemCommand = Power_OFF;
			GSM_status = Try_SIM_init;
			ErrorGSM.ErrorGSM++;
		break;
		
		case(Try_SIM_init):  //trawa inicjalizacja SIM
		// moze tutaj poczekac 3 minuty i pozniej reste procka
		
		break;
		
		case(No_connectGSM):
			ModemCommand = Network_registration_check ;
			GSM_status = Try_connectGSM;
			
		break;
		case(Try_connectGSM):
		// moze tutaj poczekac 1 min inicjalizajca SIM jeszcze raz;
		break;
		
		case(ConnectGSM):
			ErrorGSM.ErrorGSM=0;
				// moze okresowo mozna badac stan sygnalu GSM???
				// moze tutaj analizowac bufor czy nie nastapilo rozlaczenie GSM
				if (GPRS_status == No_connectGPRS)
				{
				
				}
				else
				{
				
				}

		break;
	}
/////////////////////////////////////////////////	////zestawianie polaczenia GPRS
	
	if (GSM_status == ConnectGSM)
	{
		switch(GPRS_status)
		{
			case(No_connectGPRS):
				ModemCommand = CheckGPRS;
				GPRS_status = Try_connect_GPRS;
				ErrorGSM.ErrorGPRS++;
			break;
			
			case(Try_connect_GPRS):
				// czekamy 1 min i cofamy sie do ....No_connectGSM)
			break;

			case(ConnectGPRS):
				ErrorGSM.ErrorGPRS=0;
			break;

		}
	}
	else		GPRS_status = No_connectGPRS;

///////////////////////////////////////////////////////////	// zestawianie polaczenia IP
	if (GPRS_status == ConnectGPRS)
	{
		switch(IP_status)
		{
			case(No_connect_IP):
				ModemCommand = SetUp_Apn_Id_Pass;
				IP_status = Try_connect_IP;
			ErrorGSM.ErrorIP++;
			break;
			
			case(Try_connect_IP):
				// poczekac 30 sekund a pozniej rest
			break;
			
			case(Connect_IP):
				ErrorGSM.ErrorIP=0;
			
			break;
		}
	}
	else	IP_status = No_connect_IP;

/////////////////////////////////////////////////////////////// zestawinie poczaczenia TCP
	if (IP_status == Connect_IP)
	{
		switch(TCP_status)
		{
		case(No_connect_TCP):
			ModemCommand = ConnectToTCPadress;
			TCP_status = Try_connect_TCP;
			ErrorGSM.ErrorTCP++;
		break;
		case(Try_connect_TCP):
			// poczekac 30 sekun a pozniej znowu probowac polaczyc
		break;
		
		case(Connect_TCP):
			ErrorGSM.ErrorTCP=0;
		break;		
		}

	}
	else		TCP_status = No_connect_TCP;
/////////////////////////////////////////////////////////////////////////////////	 zestawienie polaczenia Vehicom
	if (TCP_status == Connect_TCP)
	{	//AnalizeData(buff, &DataFromVehicom);	
		if (DataFromVehicom.NewData == true)
		{
				if (strstr ((const char*)DataFromVehicom.DATA, (const char*)"LOAD") != NULL)
				{
					SWTimer_Start(&Timery.timer_Vehicom, 15000);
					Vehicom_status = Login_vehicom;
				}
				else if (strstr ((const char*)DataFromVehicom.DATA, (const char*)"ON") != NULL)
				{
					Vehicom_status = Login_vehicom;
				}
				else
				{					
					Vehicom_status = NoLogin_vehicom;
					SWTimer_Start(&Timery.timer_Vehicom, 15000);
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
			/*	
			if (SWTimer_GetStatus(&Timery.timer_Vehicom) == SWTIMER_ELAPSED || SWTimer_GetStatus(&Timery.timer_Vehicom) == SWTIMER_STOPPED)
			{	ErrorGSM.ErrorVehicom++;
				Vehicom_status = TryLogin_vehicom;
				SWTimer_Start(&Timery.timer_Vehicom, 15000);
				sprintf((char*)DataToSEND, (const char*)"##,imei:%s,A,;%c",ConectParameter.IMEI,26); //kod 26 to ctrl +z
				ModemCommand= CommandStartSendDataToServer;
			}*/
			break;
			
			case(TryLogin_vehicom):
				// poczekac 30 sek a pozniej znowu probowac sie polaczyc
			break;
			
			case(Login_vehicom):
				/*if (SWTimer_GetStatus(&Timery.timer_Vehicom) == SWTIMER_ELAPSED || SWTimer_GetStatus(&Timery.timer_Vehicom) == SWTIMER_STOPPED)
				{
					ErrorGSM.ErrorVehicom=0;
					SWTimer_Start(&Timery.timer_Vehicom, 15000);
					sprintf((char*)DataToSEND, (const char*)"imei:%s,tracker,%s%s,,F,%s,%c,%s,%c,%s,%c,0.00,0;%c",
					ConectParameter.IMEI,GPS_DATA.Reversed_DATE,GPS_DATA.Short_TIME,GPS_DATA.TIME,GPS_DATA.ACTIVE,GPS_DATA.LATITUDE,
					GPS_DATA.N_or_S,GPS_DATA.LONGITUDE,GPS_DATA.W_or_E,26); //kod 26 to ctrl +z konczacy transmisje
					ModemCommand= CommandStartSendDataToServer;
				}*/
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
	
	
	/*		
	switch(ConectGSM)
	{
		case(DisconnectToGSM):	
		
		break;
		
		case(ConectToGSM):
			
			if (ModemStatus == Wait)
			{
				ModemStatus = CheckGPRS;
			}
		break;
			
		case(ConnectTCP_IP_Is_OK):
			if (ModemStatus == Wait)
			{	
				if (SWTimer_GetStatus(&Timery.timer_Connect) == SWTIMER_ELAPSED || SWTimer_GetStatus(&Timery.timer_Connect) == SWTIMER_STOPPED)
				{
					if (Server == NoLogin_vehicom)
					{
						SWTimer_Start(&Timery.timer_Connect, 5000);
						sprintf((char*)HeaderDataToSEND, (const char*)"AT+CIPSEND\r");//, strlen((const char*)MODEM_ServerData1));
						sprintf((char*)DataToSEND, (const char*)"%s%c",MODEM_ServerData1,26); //kod 26 to ctrl +z
						ModemStatus= CommandStartSendDataToServer;
						break;
					}
					if (Server == Login_vehicom)
					{
						SWTimer_Start(&Timery.timer_Connect, 5000);
						sprintf((char*)HeaderDataToSEND, (const char*)"AT+CIPSEND\r");//, strlen((const char*)MODEM_ServerData1));
						sprintf((char*)DataToSEND, (const char*)"%s%c",MODEM_ServerData2,26); //kod 26 to ctrl +z
						ModemStatus= CommandStartSendDataToServer;
						break;
					}
					
				}
			}
			if (ModemStatus == ReciveDataFromServer)
			{
				pch = NULL;
				pch = (uint8_t *)strstr((const char*)RxServerData, (const char*)"+CIPRCV:");
				if (pch != NULL)
				{
					pch+=8;
					while(i < 4 && temp[i] != ',') temp[i++] = *(pch+i);
					len = atoi(temp);
					if (Server == NoLogin_vehicom)
					{
						if (strstr ((const char*)RxServerData, (const char*)"LOAD") != 0) 
						{
							Server = Login_vehicom;
							break;
						}
												else
						{
							Server = NoLogin_vehicom;
							break;
						}
					}
					if (Server == Login_vehicom)
					{
						if (strstr ((const char*)RxServerData, (const char*)"OK") != 0)
						{
							Server = Login_vehicom;
							break;
						}
						else
						{
							Server = NoLogin_vehicom;
							break;
						}
					}
				}
			
				ModemStatus = Wait;
			}
		break;
	}*/

/*
 bool  AnalizeData(uint8_t *buff, ServerData_struct *DataFromVehicom)
 {	 
	 uint8_t *pch,*pch1, temp[10];
	 uint16_t len,length;
	 if (SWTimer_GetStatus(&Timery.WaitForResponseTimer) == SWTIMER_ELAPSED)
	 {
		len = ATH_Receive(buff);
		if (len !=0)
		{
			DataFromVehicom->NewData = true;
			pch = (uint8_t *)strstr ((const char*)buff, (const char*)"+CIPRCV:");
			if ( pch != 0) 
			{ // odebrano dane
				pch+= strlen("+CIPRCV:");
				pch1 = (uint8_t *)strstr ((const char*)buff, (const char*)",");
				length= pch1-pch;
				strncpy((char *)temp,(const char*)pch,length);
				DataFromVehicom->length = atoi((char *)temp);
				DataFromVehicom->pch = pch1+1;
				len-=strlen("+CIPRCV:")+length+1;
				strncpy(RxGSM_DATA, DataFromVehicom->pch,len);
				
				DataFromVehicom->ValidData = true;
				return (true);
			}
		}
		DataFromVehicom->ValidData = false;
	}
	return(false);		
}
*/




/*   modem_cmd_t cmd;
    
    switch(MODEM_State)
    {
        case MODEM_STATE_GET_CMD:
            
            cmd = MODEM_CMD_NIL;
        
            if((MODEM_InitFlag == true) && (SWTimer_GetStatus(&MODEM_TimerReadCSQ) == SWTIMER_ELAPSED))
            {
                cmd = MODEM_CMD_CSQ;
                SWTimer_Start(&MODEM_TimerReadCSQ, CSQ_REQ_INTERVAL);
            }
            if((MODEM_ON_RespRcvd == true) && (SWTimer_GetStatus(&MODEM_TimerSendData) == SWTIMER_ELAPSED))
            {
                cmd = MODEM_CMD_CIPSEND_START_PH1;
                MODEM_ON_RespRcvd = false;
            }
            else if(MODEM_LOAD_RespRcvd == true)
            {
			 cmd = MODEM_CMD_CIPSEND_START_PH2;
                SWTimer_Start(&MODEM_TimerSendData, DATA_SEND_INTERVAL); 
                MODEM_LOAD_RespRcvd = false;
            }

            if(cmd == MODEM_CMD_NIL)
            {
                cmd = MODEM_Cmd;
            }
            
            if(cmd != MODEM_CMD_NIL)
            {
                p_MODEM_CmdTable = MODEM_GetCmdTable(cmd);
                MODEM_State = MODEM_STATE_SEND_CMD;
                
                if((MODEM_InitFlag == false) && (cmd == MODEM_CMD_CIPSEND_START_PH1))
                {
                    MODEM_InitFlag = true;
                    SWTimer_Start(&MODEM_TimerReadCSQ, CSQ_REQ_INTERVAL);
                    SWTimer_Start(&MODEM_TimerSendData, DATA_SEND_INTERVAL);
                }
            }
            break;
            
        case MODEM_STATE_SEND_CMD:   
            
            if(p_MODEM_CmdTable != NULL)
            {
                if((SWTimer_GetStatus((&MODEM_TimerCmdTimeout)) == SWTIMER_STOPPED) &&
                    (ATH_GetTransmitStatus() == true))
                {
                    if(p_MODEM_CmdTable->pfn_cmdFrameCb != NULL)
                    {
                        p_MODEM_CmdTable->pfn_cmdFrameCb(p_MODEM_CmdTable->cmd);
                    }
                    ATH_Transmit((uint8_t*)p_MODEM_CmdTable->p_atStr, strlen((const char*)p_MODEM_CmdTable->p_atStr));
                    SWTimer_Start(&MODEM_TimerCmdTimeout, p_MODEM_CmdTable->timeout);
                    MODEM_ResponseAwaitFlag = p_MODEM_CmdTable->respFlag;
                }
                else if(MODEM_ResponseAwaitFlag == 0x00)
                {
                    if(p_MODEM_CmdTable->pfn_respCb == NULL)
                    {
                        MODEM_Cmd = p_MODEM_CmdTable->cmdNext;
                    }
                    
                    MODEM_CmdRetryCnt = 0;
                    SWTimer_Stop(&MODEM_TimerCmdTimeout);
                    p_MODEM_CmdTable = NULL;
                    MODEM_State = MODEM_STATE_GET_CMD;
                }
                else if(SWTimer_GetStatus((&MODEM_TimerCmdTimeout)) == SWTIMER_ELAPSED)
                {
                    SWTimer_Stop(&MODEM_TimerCmdTimeout);
                    if(p_MODEM_CmdTable->retryCnt)
                    {
                        MODEM_CmdRetryCnt++;
                        if(MODEM_CmdRetryCnt >= p_MODEM_CmdTable->retryCnt)
                        {
                            if(p_MODEM_CmdTable->pfn_retryCmpltCb != NULL)
                            {
                                p_MODEM_CmdTable->pfn_retryCmpltCb();
                                p_MODEM_CmdTable = NULL;
                                MODEM_State = MODEM_STATE_GET_CMD;
                            }
                        }
                    }
                }
            }
            else
            {
                p_MODEM_CmdTable = NULL;
                MODEM_State = MODEM_STATE_GET_CMD;
            }
            break;
    }
    
    // Process the modem AT response
    MODEM_ProcessReceivedData();*/
/*
static void MODEM_ProcessReceivedData(void)
{
    uint8_t buff[300];
    uint16_t len;
//    uint16_t i;
    uint16_t error;
    
 //   len = ATH_Receive(buff);
    
    if(len != 0)
    {
//        i = 0;
//        while(i < len)
//        {
//            ITM_SendChar(buff[i++]);
//        }
//        ITM_SendChar('\r');
//        ITM_SendChar('\n');
        
        if(MODEM_ResponseAwaitFlag & MODEM_RESP_PROMPT)
        {
            if(p_MODEM_CmdTable != NULL)
            {
                if(p_MODEM_CmdTable->pfn_respCb != NULL)
                {
                    p_MODEM_CmdTable->pfn_respCb(buff);
                }
                else
                {
                    if(strstr((const char*)buff, (const char*)p_MODEM_CmdTable->p_respStr) != 0)
                    {   
                        // dont wait for error response if actual response to command is available
                        MODEM_ResponseAwaitFlag &= ~MODEM_RESP_PROMPT;
                    }
                }
            }
        }
        else if(MODEM_ResponseAwaitFlag & MODEM_RESP_OK)
        {
            if(strstr((const char*)buff, "OK") != 0)
            {
                MODEM_ResponseAwaitFlag &= ~MODEM_RESP_OK;
            }
        }
        else
        {
            // Do nothing
        }
        
        if(strstr((const char*)buff, (const char*)"+CME ERROR:") != 0)
        {
            if(buff[len - 3] == ':')
            {
                error = ((buff[len - 2] - 0x30) * 10) + (buff[len - 1] - 0x30);
            }
            else
            {
                error = ((buff[len - 3] - 0x30) * 100) + ((buff[len - 2]- 0x30) * 10) + (buff[len - 1] - 0x30);  
            }
         
            if(p_MODEM_CmdTable != NULL)
            {                
                if(p_MODEM_CmdTable->pfn_errorCb != NULL)
                {
                    p_MODEM_CmdTable->pfn_errorCb(error);
                    MODEM_State = MODEM_STATE_GET_CMD;
                }
            }
        }
        else if(strstr((const char*)buff, (const char*)"ERROR") != 0)
        {
            if(p_MODEM_CmdTable != NULL)
            {
                if(p_MODEM_CmdTable->pfn_errorCb != NULL)
                {
                    p_MODEM_CmdTable->pfn_errorCb(error);
                    MODEM_State = MODEM_STATE_GET_CMD;
                }
            }
        }
        else if(strstr((const char*)buff, (const char*)"+CIPRCV") != 0)
        {
            if(strstr((const char*)buff, (const char*)"LOAD") != 0)
            {
                MODEM_LOAD_RespRcvd = true;
            }
            else if(strstr((const char*)buff, (const char*)"ON") != 0)
            {
                MODEM_ON_RespRcvd = true;
            }
        }
    }
}
static void MODEM_CmdCCIDReq_RetryCmplt_Cb(void)
{
    LED_Set(LED_RED, LED_ON);
}
static void MODEM_CmdCPIN_Req_Resp_Cb(uint8_t* p_str)
{
    if(strstr((const char*)p_str, (const char*)"+CPIN:READY") != 0)
    {
        MODEM_Cmd = MODEM_CMD_CREG;
        
        // dont wait for error response if actual response to command is available
        MODEM_ResponseAwaitFlag &= ~MODEM_RESP_PROMPT;
    }
    else if(strstr((const char*)p_str, (const char*)"+CPIN:") != 0)
    {
        MODEM_Cmd = MODEM_CMD_CPIN_ENTR;
        
        // dont wait for error response if actual response to command is available
        MODEM_ResponseAwaitFlag &= ~MODEM_RESP_PROMPT;
    }
}
static void MODEM_CmdCPIN_Entr_Error_Cb(uint16_t error)
{
    LED_Set(LED_RED, LED_ON);
}
static void MODEM_CmdCREG_Resp_Cb(uint8_t* p_str)
{
    if((p_str[9] == '5') || (p_str[9] == '1'))
    {   
        MODEM_Cmd = p_MODEM_CmdTable->cmdNext;
        
        // dont wait for error response if actual response to command is available
        MODEM_ResponseAwaitFlag &= ~MODEM_RESP_PROMPT;
    }
}
static void MODEM_CmdCSQ_Resp_Cb(uint8_t* p_str)
{
    if(strstr((const char*)p_str, (const char*)"+CSQ") != 0)
    {
        if(p_str[7] == ',')
        {
            MODEM_SignalQ = p_str[6] - 0x30;
        }
        else
        {
            MODEM_SignalQ = ((p_str[6] - 0x30) * 10) + (p_str[7] - 0x30);
        }
        
        if(MODEM_SignalQ  == 99)
        {
            MODEM_Cmd = p_MODEM_CmdTable->cmdNext;
        }
        else
        {
            MODEM_Cmd = MODEM_Cmd;
        }
        
        // dont wait for error response if actual response to command is available
        MODEM_ResponseAwaitFlag &= ~MODEM_RESP_PROMPT;
    }
}
static void MODEM_CmdCGATT_Req_Resp_Cb(uint8_t* p_str)
{
    if(strstr((const char*)p_str, (const char*)"+CGATT:1") != 0)
    {
        MODEM_Cmd = MODEM_CMD_CGDCONT;
        
        // dont wait for error response if actual response to command is available
        MODEM_ResponseAwaitFlag &= ~MODEM_RESP_PROMPT;
    }
    else if(strstr((const char*)p_str, (const char*)"+CGATT:0") != 0)
    {
        MODEM_Cmd = MODEM_CMD_CGATT_ATTACH;
        
        // dont wait for error response if actual response to command is available
        MODEM_ResponseAwaitFlag &= ~MODEM_RESP_PROMPT;
    }
}
static void MODEM_CmdCGACT_Req_Resp_Cb(uint8_t* p_str)
{
    if(strstr((const char*)p_str, (const char*)"+CGACT: 1") != 0)
    {
        MODEM_Cmd = MODEM_CMD_CIFSR;
        
        // dont wait for error response if actual response to command is available
        MODEM_ResponseAwaitFlag &= ~MODEM_RESP_PROMPT;
    }
    else if(strstr((const char*)p_str, (const char*)"+CGACT: 0") != 0)
    {
        MODEM_Cmd = MODEM_CMD_CGACT_ACT;
        
        // dont wait for error response if actual response to command is available
        MODEM_ResponseAwaitFlag &= ~MODEM_RESP_PROMPT;
    }
}
static void MODEM_CmdCIFSR_RetryCmplt_Cb(void)
{
    MODEM_Cmd = MODEM_CMD_CGACT_DEACT;
}
static void MODEM_CmdCDNSGIP_Resp_Cb(uint8_t* p_str)
{
    if(strstr((const char*)p_str, (const char*)"+CDNSGIP") != 0)
    {
        strcpy((char*)MODEM_Onetpl_IpAddr, (const char*)p_str);
        MODEM_Cmd = MODEM_CMD_CIPSTART;
        
        // dont wait for error response if actual response to command is available
        MODEM_ResponseAwaitFlag &= ~MODEM_RESP_PROMPT;
    }
}
static void MODEM_CmdCIPSEND_RetryCmplt_Cb(void)
{
    MODEM_LOAD_RespRcvd = false;
    MODEM_ON_RespRcvd = true;
    SWTimer_Start(&MODEM_TimerSendData, DATA_SEND_INTERVAL);
    MODEM_Cmd = MODEM_CMD_NIL;
}
static modem_cmd_table_t* MODEM_GetCmdTable(modem_cmd_t cmd)
{
    uint16_t i;
    modem_cmd_table_t* p_cmdTable;
    modem_cmd_table_t* p_returnCmdTable;
    
    i = MODEM_INITCMDTABLE_SIZE;
    p_cmdTable = (modem_cmd_table_t*)&MODEM_CmdTable[0];
    p_returnCmdTable = NULL;
    
    while(i--)
    {
        if(p_cmdTable->cmd == cmd)
        {
            p_returnCmdTable = p_cmdTable;
            break;
        }
        p_cmdTable++;
    }
    return p_returnCmdTable;
}
static void MODEM_CmdFrame_Cb(modem_cmd_t cmd)
{
    switch((uint16_t)cmd)
    {
        case MODEM_CMD_CIPSEND_START_PH1:
            sprintf((char*)MODEM_CmdFrameBuff, (const char*)"AT+CIPSEND=%d\r", strlen((const char*)MODEM_ServerData1));
            break;
        
        case MODEM_CMD_CIPSEND_PH1:
            sprintf((char*)MODEM_CmdFrameBuff, "%s", (const char*)MODEM_ServerData1);
            break;
        
        case MODEM_CMD_CIPSEND_START_PH2:
            sprintf((char*)MODEM_CmdFrameBuff, (const char*)"AT+CIPSEND=%d\r", strlen((const char*)MODEM_ServerData2));
            break;
        
        case MODEM_CMD_CIPSEND_PH2:
            sprintf((char*)MODEM_CmdFrameBuff, "%s", (const char*)MODEM_ServerData2);
            break;
    }
}
#if 0
static bool MODEM_PushCmdToPriorityQ(modem_cmd_t cmd)
{
    bool status;
    
    if (MODEM_CmdPriorityQTop == (sizeof(MODEM_CmdPriorityQ) - 1))
    {
        status = false;
    }
    else
    {   
        status = true;
        ++MODEM_CmdPriorityQTop;
        MODEM_CmdPriorityQ[MODEM_CmdPriorityQTop] = cmd;
    }
    return status;
}

static bool MODEM_PopCmdFromPriorityQ(modem_cmd_t* p_cmdOut)
{
    bool status;
    
    if (MODEM_CmdPriorityQTop == -1)
    {   
        status = false;
    }
    else
    {   
        status = true;
        *p_cmdOut = MODEM_CmdPriorityQ[MODEM_CmdPriorityQTop];
        --MODEM_CmdPriorityQTop;
    }
    return status;
}
#endif

void MODEM_SetServerData1(char* p_str)
{
    strcpy((char*)MODEM_ServerData1, (const char*)p_str);
}

void MODEM_SetServerData2(char* p_str)
{
    strcpy((char*)MODEM_ServerData2, (const char*)p_str);
}
*/
