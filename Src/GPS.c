/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "stm32f1xx_hal.h"
#include "swtimer.h"
#include "lowlevelinit.h"
#include "at_handler.h"
#include "modem.h"
#include "GPS.h"
extern struct_GPS	GPS_DATA;

void GPS_Job (void)
{
	GPS_ProcessReceivedData();
}
/*
volatile struct_GPS	GPS_DATA;
static volatile uint32_t ExeTimeStamp;
typedef struct  {
	uint8_t	GGA[80];
	uint8_t	RMC[80];
	uint8_t	GSV[80];
	uint8_t	VTG[80];
	uint8_t	GSA[80];
	
	uint8_t	TIME[10];		// hh:mm:ss		
	uint8_t	DATE[10];			// 			
	uint8_t	LONGITUDE[16];		
	uint8_t	LATITUDE[16];
	uint8_t	ACTIVE;				// A - aktywne poz, 	
	uint8_t	SPEED[10];				//sdj.xx km/h
	uint8_t	HIGH[10];				// tsdj.xx  wysokosc w metrach
} struct_GPS;
*/


void GPS_ProcessReceivedData(void)
{
	uint8_t i;
	uint8_t * pch;
	uint8_t u8_conuter= 0;
	
	if (GPS_DATA.Data_Fix == 1) 
	{	
		if (GPS_DATA.Data_Ready == 1)
		{
			if (GPS_DATA.RMC_Start == true)
			{
				GPS_DATA.Data_Ready =0;
				GPS_DATA.RMC_Start = false;

				// wyodrebnienia poszcegolnych pol
				pch = (uint8_t *)strtok ((char *)GPS_DATA.RMC,(char *)","); 				// wydzielenie pierwszego przed przecinkiem czyli $GPRMC
				while (pch != NULL)
				{
					// funkcja strtok moze zle dzialac bo nie ma w ciagu buff znacznika konca stringu??!!

					pch = (uint8_t *)strtok (NULL,",");
					u8_conuter++;
					switch(u8_conuter)
					{
//http://www.gpsinformation.org/dale/nmea.htm#RMC
						case (0):
						break;
						case(1):	//hhmmss	godzina UTC
							strcpy((char*)GPS_DATA.TIME,(char *)pch);
							i=0;
							while(GPS_DATA.TIME[i] != 0 && GPS_DATA.TIME[i] != '.' && i< 10)
							{
								GPS_DATA.Short_TIME[i] = GPS_DATA.TIME[i];
								i++;
							}
							break;
						case(2):	// A - active v = void
							GPS_DATA.ACTIVE = *pch;
							break;
						case(3):	// Latitude
							strcpy((char*)GPS_DATA.LATITUDE,(char *)pch);
							break;
						case(4):	// N or S
							GPS_DATA.N_or_S = *pch;
							break;
						case(5):	//Longitude
							strcpy((char*)GPS_DATA.LONGITUDE,(char *)pch);
							break;
						case(6):	// E or W
							GPS_DATA.W_or_E = *pch;
							break;
						case(7):	//speed over ground in knots
							strcpy((char*)GPS_DATA.SPEED,(char *)pch);
							break;
						case(8):	// Track angle in degrees True
							
							break;
						case(9):	//  ddmmrr	date
							strcpy((char*)GPS_DATA.DATE,(char *)pch);
							i=0;
							GPS_DATA.Reversed_DATE[0] = GPS_DATA.DATE[4];
							GPS_DATA.Reversed_DATE[1] = GPS_DATA.DATE[5];
							GPS_DATA.Reversed_DATE[2] = GPS_DATA.DATE[2];
							GPS_DATA.Reversed_DATE[3] = GPS_DATA.DATE[3];
							GPS_DATA.Reversed_DATE[4] = GPS_DATA.DATE[0];
							GPS_DATA.Reversed_DATE[5] = GPS_DATA.DATE[1];
						break;
						case(10):
							// Magnetic Variation
							break;
						case(11):
							//check sume
							break;
						case(12):
//							return(0);	nie powinno wystapic!!!
						break;
					}
				} 
			}
		}	   
	}
	else// zerowanie wszystkich
	{
		GPS_DATA.ACTIVE = 'V';
	}
}

void GPS_Init(void)
{
	GPS_HandlerInit();
}
