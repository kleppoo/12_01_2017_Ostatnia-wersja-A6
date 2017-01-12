/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPS_H__
#define __GPS_H__

#define GPS_BUFFOR_SIZE 120
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>
/* Exported types ------------------------------------------------------------*/
typedef struct  {
	uint8_t	GGA[GPS_BUFFOR_SIZE];
	bool 	GGA_Start;
	uint8_t	RMC[GPS_BUFFOR_SIZE];
	bool		RMC_Start;
	uint8_t	GSV[GPS_BUFFOR_SIZE];
	bool		GSV_Start;
	uint8_t	VTG[GPS_BUFFOR_SIZE];
	bool		VTG_Start;
	uint8_t	GSA[GPS_BUFFOR_SIZE];
	bool 	GSA_Start;
	char 	Data_Ready;
	char 	Data_Fix;

	uint8_t	TIME[16];		// hhmmss.setne
	uint8_t	Short_TIME[10];	
	uint8_t	DATE[10];			// 			
	uint8_t   Reversed_DATE[10];
	uint8_t	LONGITUDE[16];
	uint8_t	N_or_S;
	uint8_t	LATITUDE[16];
	uint8_t	W_or_E;
	uint8_t	ACTIVE;				// A - aktywne poz, 	
	uint8_t	SPEED[10];				//sdj.xx km/h
	uint8_t	HIGH[10];				// tsdj.xx  wysokosc w metrach
} struct_GPS;
	 
	 
	 
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void GPS_ProcessReceivedData(void);
 
void GPS_Init(void);

void GPS_Job (void);
     
/* Private Constants ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __AT_HANDLER_H */





