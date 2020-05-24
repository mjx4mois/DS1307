/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: Real Time Count
     File Name		: SENSOR_DS1307.c
     Function		: SENSOR_DS1307
     Create Date	: 2017/04/27
---------------------------------------------------------------------- */
#ifndef __DS1307_HEADER__
#define __DS1307_HEADER__

/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/
#define DS1307_SLAVE_ADDRESS     	0xD0
/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/

/* ------------- DEFINE DS1307 REGISTER  ------------*/
#define DS1307_REG_SECONDS		0x00
#define DS1307_REG_MINUTES		0x01
#define DS1307_REG_HOURS		0x02
#define DS1307_REG_DAY			0x03
#define DS1307_REG_DATE			0x04
#define DS1307_REG_MONTH		0x05
#define DS1307_REG_YEAR			0x06
#define DS1307_REG_CONTROL		0x07
/*Register : 0x08~0x3F ---> SRAM REGISTER (USER can use)*/
/* -------------------------------------------------*/


/*---------------------------- Time STRUCT -----------------------------*/
typedef struct DS1307_TIME
{
	CHAR8U SECOND;		/*00~59*/
	CHAR8U MINUTE;			/*00~59*/
	CHAR8U HOUR;			/*00~23*/
	CHAR8U WEEK;			/*01~07*/
	CHAR8U DATE;			/*01~31*/
	CHAR8U MONTH;			/*01~12*/
	CHAR8U YEAR; 			/*00~99  ;  start 2000 to 2100 year ,example if YEAR = 01 -> 2001 , YEAR = 17 -> 2017*/
}DS1307_TIME_STRUCT;
/*---------------------------- Time STRUCT -----------------------------*/


/*--------------------------------- Define Mode -------------------------------------*/
/******* for DS1307_SQW_OUTPUT_SET() Function ********/
#define DS1307_SQW_Dis_Out_Level_Low	 	0x00
#define DS1307_SQW_Dis_Out_Level_High		0x80
/*
		M0 : Rrequency -> 1HZ
		M1 : Rrequency -> 4.096KHZ
		M2 : Rrequency -> 8.192KHZ
		M3 : Rrequency -> 32.768KHZ
*/
#define DS1307_SQW_En_Out_Freq_M0		0x10
#define DS1307_SQW_En_Out_Freq_M1		0x11
#define DS1307_SQW_En_Out_Freq_M2		0x12
#define DS1307_SQW_En_Out_Freq_M3		0x13
/******* for DS1307_SQW_OUTPUT_SET() Function ********/


/******* for DS1307_START_COUNT() & DS1307_STOP_COUNT() Function ********/
#define DS1307_STOP_COUNT_TIME			0x80
#define DS1307_START_COUNT_TIME			0X7F
/******* for DS1307_START_COUNT() & DS1307_STOP_COUNT() Function ********/
/*--------------------------------- Define Mode -------------------------------------*/


/*----------------------------------- Function --------------------------------------*/
CHAR8S DS1307_INIT(void);
CHAR8S DS1307_START_COUNT(void);
CHAR8S DS1307_STOP_COUNT(void);
CHAR8S DS1307_READ_TIME(DS1307_TIME_STRUCT *TIME_INFO);
CHAR8S DS1307_CHANGE_TIME(DS1307_TIME_STRUCT *CHANGE_TIME_INFO);
CHAR8S DS1307_SQW_OUTPUT_SET(CHAR8U MODE);		/*use #define variable*/
/*----------------------------------- Function --------------------------------------*/

#endif		//#ifndef __DS1307_HEADER__