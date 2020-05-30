/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: Real Time Count
     File Name		: SENSOR_DS1307.c
     Function		: SENSOR_DS1307
     Create Date	: 2017/05/04
---------------------------------------------------------------------- */

#include <stdio.h>
#include <delay.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_DS1307.h>

/* POWER ON TIME -> 01/01/00 01 00:00:00 (MM/DD/YY DOW HH:MM:SS). */
/*--------------------------------------------------------------------------------------------------*/
CHAR8S DS1307_INIT(void)
{
	CHAR8S init_flag;
	DS1307_TIME_STRUCT INIT_TIME;

		/* Set DS1307 stop counting time */
		init_flag = DS1307_STOP_COUNT();
		if(init_flag !=0) return -1;

		/* Set DS1307 parameter */
		INIT_TIME.SECOND=0;
		INIT_TIME.MINUTE=0;
		INIT_TIME.HOUR=0;
		INIT_TIME.WEEK=1;
		INIT_TIME.DATE=1;
		INIT_TIME.MONTH=1;
		INIT_TIME.YEAR=0;
		
	 	/* write time into DS1307 */
		init_flag = DS1307_CHANGE_TIME(&INIT_TIME);
		if(init_flag ==0)
		{
			/*write success*/
			init_flag = DS1307_START_COUNT();
			if(init_flag == 0)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{	
			/* Set DS1307 start counting time */
			DS1307_START_COUNT();
			return -1;
		}

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S DS1307_START_COUNT(void)
{
	CHAR8S flag = 0;
	CHAR8U temp_data;

		flag =  i2c_read_1_byte_data(DS1307_SLAVE_ADDRESS,0X00,&temp_data);
		if(flag == 1)
		{
			temp_data = temp_data & DS1307_START_COUNT_TIME;
			delay_us(50);	/*tiny delay */
			flag =  i2c_write_1_byte_data(DS1307_SLAVE_ADDRESS,0X00,temp_data);
			if(flag == 1)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S DS1307_STOP_COUNT(void)
{
	CHAR8S flag = 0;
	CHAR8U temp_data;

		flag =  i2c_read_1_byte_data(DS1307_SLAVE_ADDRESS,0X00,&temp_data);
		if(flag == 1)
		{
			temp_data = temp_data | DS1307_STOP_COUNT_TIME;
			delay_us(50); /*tiny delay*/
			flag =  i2c_write_1_byte_data(DS1307_SLAVE_ADDRESS,0X00, temp_data);
			if(flag == 1)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S DS1307_SQW_OUTPUT_SET(CHAR8U MODE)
{

/****** for DS1307_SQW_OUTPUT_SET() Function *******/
/*
	#define DS1307_SQW_Dis_Out_Level_Low	 	0x00
	#define DS1307_SQW_Dis_Out_Level_High		0x80
		M0 : Rrequency -> 1HZ
		M1 : Rrequency -> 4.096KHZ
		M2 : Rrequency -> 8.192KHZ
		M3 : Rrequency -> 32.768KHZ

	#define DS1307_SQW_En_Out_Freq_M0		0x10
	#define DS1307_SQW_En_Out_Freq_M1		0x11
	#define DS1307_SQW_En_Out_Freq_M2		0x12
	#define DS1307_SQW_En_Out_Freq_M3		0x13 
*/
/****** for DS1307_SQW_OUTPUT_SET() Function *******/


	CHAR8S flag=0;
	CHAR8U check_data;
	
		if((MODE == DS1307_SQW_Dis_Out_Level_Low) || (MODE == DS1307_SQW_Dis_Out_Level_High) || (MODE == DS1307_SQW_En_Out_Freq_M0)\
			|| (MODE == DS1307_SQW_En_Out_Freq_M1) || (MODE == DS1307_SQW_En_Out_Freq_M2) || (MODE == DS1307_SQW_En_Out_Freq_M3))
		{

			flag = i2c_write_1_byte_data(DS1307_SLAVE_ADDRESS,DS1307_REG_CONTROL,MODE);

			if(flag == 1)
			{
				delay_us(40);	/*wait stable*/
				flag = i2c_read_1_byte_data(DS1307_SLAVE_ADDRESS,DS1307_REG_CONTROL,&check_data);

				if(flag == 1)
				{
					if(check_data == MODE) 
					{
						return 0; /* set OUTPUT success !!*/
					}
					else
					{
						return -1; 	/*set OUTPUT fail !!*/
					}
				}

			}
			else 
			{
				return -1;	/*write fail*/
			}

	        }
		else
		{
			/* Not the SET MODE code*/
			return -1;	/* fail set mode*/
		}

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S DS1307_READ_TIME(DS1307_TIME_STRUCT *TIME_INFO)
{
/* Note : evey data is BCD format */
/*
	typedef struct DS1307_TIME
	{
		CHAR8U SECOND;		//00~59
		CHAR8U MINUTE;		//00~59
		CHAR8U HOUR;		//00~23
		CHAR8U WEEK;		//01~07
		CHAR8U DATE;		//01~31
		CHAR8U MONTH;		//01~12
		CHAR8U YEAR; 		//00~99  ;  start 2000 to 2100 year ,example if YEAR = 01 -> 2001 , YEAR = 17 -> 2017
	}DS1307_TIME_STRUCT;
*/

	CHAR8S read_flag = 0;
	CHAR8U read_data_buffer[8] = {0},temp_data1,temp_data2;

		/* read time 8 bytes , start reg addr : 0x00 */
		read_flag = i2c_read_n_byte_data(DS1307_SLAVE_ADDRESS,0X00,8,&read_data_buffer[0]);
		if(read_flag == 1)	/*read success*/
		{
			/*--------- SECOND ---------*//*BCD format */
			temp_data1 = ((read_data_buffer[0] & 0x70)>>4)*10 ;
			temp_data2 = (read_data_buffer[0] & 0x0F) + temp_data1;
			TIME_INFO->SECOND = temp_data2;
			/*--------- SECOND ---------*/

			/*--------- MINUTE ---------*//*BCD format */
			temp_data1 = ((read_data_buffer[1] & 0x70)>>4)*10 ;
			temp_data2 = (read_data_buffer[1] & 0x0F) + temp_data1;
			TIME_INFO->MINUTE = temp_data2;
			/*--------- MINUTE ---------*/

			/*--------- HOUR ---------*//*BCD format */
			/* if "1" -> 12hr (AM/PM)  , if "0" -> 24hr*/
			if (read_data_buffer[2] & 0x40 == 1) /* 12hr */
					{
						/* if "1" -> PM , if " 0" -> AM*/
						if (read_data_buffer[2] & 0x20)/*PM*/
						{
							temp_data1 = ((read_data_buffer[2] & 0x70)>>4)*10 + (10) ;		/*+10*/
							temp_data2 =  (read_data_buffer[2] & 0x0F) + temp_data1  + (2) ;		/* +2*/
				                     TIME_INFO->HOUR = temp_data2;
						}
						else/*AM*/
						{
							temp_data1 = ((read_data_buffer[2] & 0x70)>>4)*10; /* No +10*/
							temp_data2 =  (read_data_buffer[2] & 0x0F) + temp_data1 ; /*No +2*/
				                     TIME_INFO->HOUR = temp_data2;
						}
					}
				else	/* 24hr*/
					{
					temp_data1 = ((read_data_buffer[2] & 0x30)>>4)*10 ;
					temp_data2 = (read_data_buffer[2] & 0x0F) + temp_data1;
					TIME_INFO->HOUR = temp_data2;
					}
			/*--------- HOUR ---------*/

			/*--------- WEEK ---------*//*BCD format */
			temp_data1 = (read_data_buffer[3] & 0x07) ;
			TIME_INFO->WEEK = temp_data1;
			/*--------- WEEK ---------*/


			/*--------- DATE ---------*//*BCD format */
			temp_data1 = ((read_data_buffer[4] & 0x30)>>4)*10 ;
			temp_data2 = (read_data_buffer[4] & 0x0F) + temp_data1;
			TIME_INFO->DATE = temp_data2;
			/*--------- DATE ---------*/


			/*--------- MONTH ---------*//*BCD format */
			temp_data1 = ((read_data_buffer[5] & 0x10)>>4)*10 ;
			temp_data2 = (read_data_buffer[5] & 0x0F) + temp_data1;
			TIME_INFO->MONTH = temp_data2;
			/*--------- MONTH ---------*/


			/*--------- YEAR ---------*//*BCD format */
			temp_data1 = ((read_data_buffer[6] & 0xF0)>>4)*10 ;
			temp_data2 = (read_data_buffer[6] & 0x0F) + temp_data1;
			TIME_INFO->YEAR = temp_data2;
			/*--------- YEAR ---------*/

			return 0; 		/*finish trans data*/

			
		  }
	 	else 
	 	{
	 		return -1; 		/*read fail*/
	 	}
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
CHAR8S DS1307_CHANGE_TIME(DS1307_TIME_STRUCT *CHANGE_TIME_INFO)
{
	/*
	typedef struct DS1307_TIME
	{
		CHAR8U SECOND;		//00~59
		CHAR8U MINUTE;		//00~59
		CHAR8U HOUR;		//00~23
		CHAR8U WEEK;		//01~07
		CHAR8U DATE;		//01~31
		CHAR8U MONTH;		//01~12
		CHAR8U YEAR; 		//00~99  ;  start 2000 to 2100 year ,example if YEAR = 01 -> 2001 , YEAR = 17 -> 2017
	}DS1307_TIME_STRUCT;
	*/

	CHAR8S write_flag = 0;
	CHAR8U write_data_buffer[7] = {0},temp_data1,temp_data2,temp_data3;

		/*STOP DS1307 to count  & change a New Time for DS1307 count*/
		write_flag = DS1307_STOP_COUNT();
		if(write_flag != 0)
		{
			return -1;
		}
		
		/*---------------------- set Time ----------------------*/
		/*--------- SECOND ---------*/
		if(CHANGE_TIME_INFO->SECOND > 59) 
		{
			temp_data3 = 59;
		}
		else if(CHANGE_TIME_INFO->SECOND < 0)
		{
			temp_data3 = 0;
		}
		else
		{
			temp_data3 = CHANGE_TIME_INFO->SECOND;
		}
		temp_data1 = ((temp_data3)/10)<<4;
		temp_data2 = (temp_data3)%10;
		write_data_buffer[0] = ( 0x80 | (temp_data1 & 0x70) |(temp_data2 & 0x0F) );  /* 0X80 -> Disable to count */
		/*--------- SECOND ---------*/


		/*--------- MINUTE ---------*/
		if(CHANGE_TIME_INFO->MINUTE > 59)
		{
			temp_data3 = 59;
		}
		else if(CHANGE_TIME_INFO->MINUTE < 0)
		{
			temp_data3 = 0;
		}
		else
		{
			temp_data3 = CHANGE_TIME_INFO->MINUTE;
		}
		temp_data1 = ((temp_data3)/10)<<4;
		temp_data2 = (temp_data3)%10;
	    	write_data_buffer[1]= ((temp_data1 & 0x70) |(temp_data2 & 0x0F));
		/*--------- MINUTE ---------*/


		/*--------- HOUR ---------*/
		if(CHANGE_TIME_INFO->HOUR > 59)
		{
			temp_data3 = 23;
		}
		else if(CHANGE_TIME_INFO->HOUR < 0)
		{
			temp_data3 = 0;
		}
		else
		{
			temp_data3 = CHANGE_TIME_INFO->HOUR;
		}
		temp_data1 = ((temp_data3)/10)<<4;
		temp_data2 = (temp_data3)%10;
        	write_data_buffer[2]= ((temp_data1 & 0x30) |(temp_data2 & 0x0F));
		/*--------- HOUR ---------*/


		/*--------- WEEK ---------*/
		/****** NOT SET THE WEEK ? ******/
		if(CHANGE_TIME_INFO->WEEK> 7)
		{
			temp_data3 = 7;
		}
		else if(CHANGE_TIME_INFO->WEEK < 1)
		{
			temp_data3 = 1;
		}
		else 
		{
			temp_data3 = CHANGE_TIME_INFO->WEEK;
		}
	    	write_data_buffer[3]= (temp_data3 & 0x07);
		/*--------- WEEK ---------*/


		/*--------- DATE ---------*/
		if(CHANGE_TIME_INFO->DATE> 31)
		{
			temp_data3 = 31;
		}
		else if(CHANGE_TIME_INFO->DATE < 1)
		{
			temp_data3 = 1;
		}
		else
		{
			temp_data3 = CHANGE_TIME_INFO->DATE;
		}
		temp_data1 = ((temp_data3)/10)<<4;
		temp_data2 = (temp_data3)%10;
	    	write_data_buffer[4]= ((temp_data1 & 0x30) |(temp_data2 & 0x0F));
		/*--------- DATE ---------*/


		/*--------- MONTH ---------*/
		if(CHANGE_TIME_INFO->MONTH > 12)
		{
			temp_data3 = 12;
		}
		else if(CHANGE_TIME_INFO->MONTH  < 1)
		{
			temp_data3 = 1;
		}
		else
		{
			temp_data3 = CHANGE_TIME_INFO->MONTH;
		}
		temp_data1 = ((temp_data3)/10)<<4;
		temp_data2 = (temp_data3)%10;
	    	write_data_buffer[5]= ((temp_data1 & 0x10) |(temp_data2 & 0x0F));
		/*--------- MONTH ---------*/


		/*--------- YEAR ---------*/
		if(CHANGE_TIME_INFO->YEAR > 100)
		{
			temp_data3 = 100;
		}
		else if(CHANGE_TIME_INFO->YEAR  < 0)
		{
			temp_data3 = 0;
		}
		else
		{
			temp_data3 = CHANGE_TIME_INFO->YEAR;
		}
		temp_data1 = ((temp_data3)/10)<<4;
		temp_data2 = (temp_data3)%10;
	    	write_data_buffer[6]= ((temp_data1 & 0xF0) |(temp_data2 & 0x0F));
		/*--------- YEAR ---------*/
		/*---------------------- set Time ----------------------*/


		/*---------------------------- write Time INFO ---------------------------*/
		write_flag = i2c_write_n_byte_data(DS1307_SLAVE_ADDRESS,0x00,7,&write_data_buffer[0]);
	 	if(write_flag == 1)
		{
			/*write success*/
			write_flag = DS1307_START_COUNT();
				
			if(write_flag == 0)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			/*write fail*/
			DS1307_START_COUNT();
			return -1;
		}
		/*---------------------------- write Time INFO ---------------------------*/

}
/*--------------------------------------------------------------------------------------------------*/

