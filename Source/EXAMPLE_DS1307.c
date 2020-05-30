/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: DS1307 real time count(RTC)
     File Name		: EXAMPLE_DS1307.c
     Function		: EXAMPLE_DS1307
     Create Date	: 2017/05/05
---------------------------------------------------------------------- */

#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <alcd.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_DS1307.h>


void EXAMPLE_DS1307(void);

void EXAMPLE_DS1307(void)
{

	CHAR8S check_flag_DS1307 = 0;
	CHAR8U lcd_char_data[7][2]={0}; /*time display buffer */
	DS1307_TIME_STRUCT DS1307_TIME_BUFFER;
	DS1307_TIME_STRUCT DS1307_TIME_SET;


			printf("-------------------- RealTimeCount DS1307 --------------------\r\n");

			/* slove Read/write fail*/
			i2c_stop_hang();  

			/* set  the SQW/OUT pin output 1HZ square-wave */
			check_flag_DS1307 = DS1307_SQW_OUTPUT_SET(DS1307_SQW_En_Out_Freq_M0);		
			if(check_flag_DS1307 == 0)
			{
				printf("set OUTPUT ok!!\r\n");
			}
               	 else
			{
			 	printf("set OUTPUT fail \r\n");
             	 	}
				 
			/* Set DS1307 start counting time */	 
			check_flag_DS1307 = DS1307_START_COUNT();		
			if(check_flag_DS1307 == 0) 
			{
				printf("set START ok!!\r\n");
			}
			else
			{
				printf("set START fail \r\n");
			}
			
			/* Read DS1307 time parameters */
                	check_flag_DS1307 = DS1307_READ_TIME(&DS1307_TIME_BUFFER);		
                	if(check_flag_DS1307 == 0)
			{
				printf("set read ok!!\r\n");
				printf("Time.SECOND = %d\r\n",DS1307_TIME_BUFFER.SECOND);
				printf("Time.MINUTE = %d\r\n",DS1307_TIME_BUFFER.MINUTE);
				printf("Time.HOUR = %d\r\n",DS1307_TIME_BUFFER.HOUR);
				printf("Time.WEEK = %d\r\n",DS1307_TIME_BUFFER.WEEK);
				printf("Time.DATE = %d\r\n",DS1307_TIME_BUFFER.DATE);
				printf("Time.MONTH = %d\r\n",DS1307_TIME_BUFFER.MONTH);
				printf("Time.YEAR = %d\r\n",DS1307_TIME_BUFFER.YEAR);
			}
		        else
			{
				printf("set read fail \r\n");
		        }

		
			/* Set DS1307 stop counting time */
			check_flag_DS1307 = DS1307_STOP_COUNT();		
			if(check_flag_DS1307 == 0) 
			{
				printf("set STOP ok!!\r\n");
			}
			 else 
			{
				printf("set STOP fail \r\n");
			}


			 /* base 2020.05.24 13:50:30 */	   
			DS1307_TIME_SET.SECOND = 30;
			DS1307_TIME_SET.MINUTE = 50;
			DS1307_TIME_SET.HOUR = 13;
			DS1307_TIME_SET.WEEK = 7;
			DS1307_TIME_SET.DATE = 24;
			DS1307_TIME_SET.MONTH = 5;
			DS1307_TIME_SET.YEAR = 20;

			 /* write time into DS1307 */	 
			check_flag_DS1307 = DS1307_CHANGE_TIME(&DS1307_TIME_SET);		/* Change DS1307 time parameter */
			if(check_flag_DS1307 == 0)
			{
				printf("change time ok!!\r\n");
			}
		       else 
			{
				printf("change time fail\r\n");
			}
            	

			/* Set DS1307 start counting time */	 
			check_flag_DS1307 = DS1307_START_COUNT();		/* Set DS1307 start counting time */
			if(check_flag_DS1307 == 0) 
			{
				printf("set START ok!!\r\n");
			}
			else
			{
				printf("set START fail \r\n");
			}


			while(1) /*LOOP: read DS1307 time  */
			{
				/* Read DS1307 time parameters */
				check_flag_DS1307 = DS1307_READ_TIME(&DS1307_TIME_BUFFER);		
				if(check_flag_DS1307 == 0)
				{
				
					printf("set read ok!!\r\n");
					printf("Time.SECOND = %d\r\n",DS1307_TIME_BUFFER.SECOND);
					printf("Time.MINUTE = %d\r\n",DS1307_TIME_BUFFER.MINUTE);
					printf("Time.HOUR = %d\r\n",DS1307_TIME_BUFFER.HOUR);
					printf("Time.WEEK = %d\r\n",DS1307_TIME_BUFFER.WEEK);
					printf("Time.DATE = %d\r\n",DS1307_TIME_BUFFER.DATE);
					printf("Time.MONTH = %d\r\n",DS1307_TIME_BUFFER.MONTH);
					printf("Time.YEAR = %d\r\n",DS1307_TIME_BUFFER.YEAR);


					/*------ Calculate Time -----*/
					/* SECOND */
					lcd_char_data[0][0] = (CHAR8U)(DS1307_TIME_BUFFER.SECOND/10)%10;	
					lcd_char_data[0][1] = (CHAR8U)(DS1307_TIME_BUFFER.SECOND)%10;		

					/* MINUTE */
					lcd_char_data[1][0] = (CHAR8U)(DS1307_TIME_BUFFER.MINUTE/10)%10;	
					lcd_char_data[1][1] = (CHAR8U)(DS1307_TIME_BUFFER.MINUTE)%10;		

					/* HOUR */
					lcd_char_data[2][0] = (CHAR8U)(DS1307_TIME_BUFFER.HOUR/10)%10;	
					lcd_char_data[2][1] = (CHAR8U)(DS1307_TIME_BUFFER.HOUR)%10;	

					/* WEEK */
					lcd_char_data[3][0] = (CHAR8U)(DS1307_TIME_BUFFER.WEEK/10)%10;	
					lcd_char_data[3][1] = (CHAR8U)(DS1307_TIME_BUFFER.WEEK)%10;		

					/* DATE */
					lcd_char_data[4][0] = (CHAR8U)(DS1307_TIME_BUFFER.DATE/10)%10;	
					lcd_char_data[4][1] = (CHAR8U)(DS1307_TIME_BUFFER.DATE)%10;	

					/* MONTH */
					lcd_char_data[5][0] = (CHAR8U)(DS1307_TIME_BUFFER.MONTH/10)%10;	
					lcd_char_data[5][1] = (CHAR8U)(DS1307_TIME_BUFFER.MONTH)%10;		

					/* YEAR */ /* year range : 2000 ~ 2100 */
					lcd_char_data[6][0] = (CHAR8U)(DS1307_TIME_BUFFER.YEAR/10)%10;	
					lcd_char_data[6][1] = (CHAR8U)(DS1307_TIME_BUFFER.YEAR)%10;	

				

					/* SHOW Time */                    
		            		lcd_gotoxy(0,0);   

					/* show YEAR */
					lcd_putchar(48+2);	/* print "2"  */				
					lcd_putchar(48+0);	/* print "0"  */	
					lcd_putchar(48+lcd_char_data[6][0]);
					lcd_putchar(48+lcd_char_data[6][1]);
					
					lcd_putchar(47);    /* print "/"  */	
					
					/* show MONTH */	
					lcd_putchar(48+lcd_char_data[5][0]);
					lcd_putchar(48+lcd_char_data[5][1]);

					lcd_putchar(47);    /* print "/"  */	

					/* show DATE */	
					lcd_putchar(48+lcd_char_data[4][0]);
					lcd_putchar(48+lcd_char_data[4][1]);

					lcd_putchar(47);    /* print "/"  */					

			
					/* show WEEK */	
					lcd_gotoxy(0,1);   
					lcd_putsf("WEEK:");
					if(DS1307_TIME_BUFFER.WEEK ==1)
					{
						lcd_putsf("Mon");		/* show Mon */	
					}
					else if (DS1307_TIME_BUFFER.WEEK ==2)
					{
						lcd_putsf("Tue");		/* show Tue */	
					}
					else if (DS1307_TIME_BUFFER.WEEK ==3)
					{
						lcd_putsf("Wed");		/* show Wed */	
					}
					else if (DS1307_TIME_BUFFER.WEEK ==4)
					{
						lcd_putsf("Thu");		/* show Thu */	
					}
					else if (DS1307_TIME_BUFFER.WEEK ==5)
					{
						lcd_putsf("Fri");		/* show Fri */	
					}
					else if (DS1307_TIME_BUFFER.WEEK ==6)
					{
						lcd_putsf("Sat");		/* show Sat */	
					}
					else if (DS1307_TIME_BUFFER.WEEK ==7)
					{
						lcd_putsf("Sun");		/* show Sun */	
					}

					/* show Time :  HOUR : MINUTE : SECOND */	
					lcd_gotoxy(0,2);   
					lcd_putsf("Time:");

					/* show HOUR */	
					lcd_putchar(48+lcd_char_data[2][0]);
					lcd_putchar(48+lcd_char_data[2][1]);

					lcd_putchar(58);    /* print ":"  */	

					/* show MINUTE */	
					lcd_putchar(48+lcd_char_data[1][0]);
					lcd_putchar(48+lcd_char_data[1][1]);

					lcd_putchar(58);    /* print ":"  */	
					
					/* show SECOND */	
					lcd_putchar(48+lcd_char_data[0][0]);
					lcd_putchar(48+lcd_char_data[0][1]);


					/* show IC ID */	
					lcd_gotoxy(0,3);   
					lcd_putsf("DS1307");

				
				}			
		       	else 
				{
				printf("set read fail \r\n");
		        	}

			/* delay 150mS */
			delay_ms(150);
		}

               printf("-------------------- RealTimeCount DS1307 --------------------\r\n");

}
