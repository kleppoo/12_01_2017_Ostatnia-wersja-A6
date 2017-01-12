#ifndef __MAIN_H__
#define __MAIN_H__

#define GSM_Sleep  			HAL_GPIO_WritePin(GPIOA, GSM_sleep_Pin, GPIO_PIN_RESET);
#define GSM_WakeUp 			HAL_GPIO_WritePin(GPIOA, GSM_sleep_Pin, GPIO_PIN_SET);

#define GSM_POWER_ON		HAL_GPIO_WritePin(GSM_PWR_GPIO_Port, GSM_PWR_Pin, GPIO_PIN_RESET);
#define GSM_POWER_OFF		HAL_GPIO_WritePin(GSM_PWR_GPIO_Port, GSM_PWR_Pin, GPIO_PIN_SET);

#define GSM_RESET_ON		HAL_GPIO_WritePin(GSM_RST_GPIO_Port, GSM_RST_Pin, GPIO_PIN_SET);
#define GSM_RESET_OFF		HAL_GPIO_WritePin(GSM_RST_GPIO_Port, GSM_RST_Pin, GPIO_PIN_RESET);





typedef struct
{
	uint8_t DATA[1024];
	uint8_t CommandResponse[512];
	uint8_t DataToSEND[512];
	uint16_t i;
	uint16_t char_to_read;
} GSM_RxBuff_struct;

typedef struct
{
	bool MODEM_ON;
	
}GSM_modem_struct;

typedef struct
{
	uint8_t IMEI[20];
	uint8_t IMSI[20];
	uint8_t MyIP[20];
	uint8_t wwwIP[20];
	uint8_t SignnalQuality;
} Connect_type_struct;

typedef struct
{
	swtimer_t timer;
	swtimer_t timer_Vehicom;
	swtimer_t WaitForResponseTimer;
}Timery_type_struct;

typedef struct
{
	uint8_t No_Response;
	uint8_t Error;
	uint8_t Fail;
	uint8_t ErrorGSM;
	uint8_t ErrorGPRS;
	uint8_t ErrorIP;
	uint8_t ErrorTCP;
	uint8_t ErrorVehicom;
	uint8_t hardReset;

	
}ErrorGSM_struct;

typedef struct
{
	uint8_t DATA[512];
	uint16_t length;
	//uint8_t *pch;
	bool NewData;
//	bool ValidData;
} ServerData_struct;

typedef enum
{
	Power_OFF	= 0,
	Power_ON,
	After_Reset,
	Echo_OFF,
	AGPS_ON,
	GPS_FREQ,
	CheckPINneed,
	ReadIMEI,
	ReadIMSI, 
	Subscriber_number,
	Network_registration_check,
	CheckSignalQuality,
	Network_registrationDo,		
	NetworkRegistrationDisconect,
	CheckGPRS,
	LoginToGPRS,
	DisconectGPRS_Do,
	SetUp_Apn_Id_Pass,
	PIN_is_need,			//nie zrobione
	SetPDPparameter,
	ActivePDPparameter,
	CheckIPstatus,
	Disconect_IP,
	CheckMyIP,
	GetIPDomain,
	ConnectToTCPadress,	
	CommandStartSendDataToServer,
	CommandSendDataToServer,
	WaitForResponse,
	Wait	
} modem_comand_type;

typedef enum
{
	SIM_No_init =0 ,
	Try_SIM_init,	
	No_connectGSM,
	Try_connectGSM,
	ConnectGSM
} GSM_connect_type;
typedef enum
{
	No_connectGPRS=0,
	Try_connect_GPRS,
	ConnectGPRS


}GPRS_connect_type;
typedef enum
{
	No_connect_IP =0,
	SetPDP,
	Try_connect_IP,
	Connect_IP,
	/*ConectToGSM,
	ConectToGPRS,
	RedyToConnectTCP_IP,
	ConnectTCP_IP_Is_OK,
	ReciveDataFromServer,*/
} IP_conect_type;

typedef enum
{
	No_connect_TCP =0,
	Try_connect_TCP,
	Connect_TCP,
	/*ConectToGSM,
	ConectToGPRS,
	RedyToConnectTCP_IP,
	ConnectTCP_IP_Is_OK,
	ReciveDataFromServer,*/
} TCP_conect_type;
typedef enum
{
	NoLogin_vehicom =0,
	TryLogin_vehicom,
	Login_vehicom
	//WaitForResponseServer

} Vehicom_conect_type;

#endif
