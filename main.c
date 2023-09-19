/*
 * File:   main.c
 * Author: komala
 *
 * Created on 9 August, 2023, 12:41 PM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "ext_eeprom.h"



unsigned char event[][3] = {"ON","GR","GN","G1","G2","G3","G4"," C","DL","CL","ST","CP"};
unsigned char data[11] = {};
unsigned char time[9]="00:00:00",clock_reg[4];
unsigned char flag = 0,index = 0;
unsigned int speed = 0; unsigned char key;
char over_flag = 0;
int wr_flag = -1;
unsigned int i;
unsigned char dash_flag = 1;
unsigned char pass_flag = 0;

void get_time(void)                                    //to get time from rtc         
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}
void init_config(void)                   //configurations for clcd,i2c,ds1307,matrix_keypad and adc
{
	init_clcd();
	init_i2c();
	init_ds1307();
    init_matrix_keypad();
    init_adc();

}
void Line1()                                      //to display in clcd time,Ev and SP 
{
    clcd_print("Time", LINE1(0));
    clcd_print("EV", LINE1(8));
    clcd_print("SP", LINE1(12));   
}

void Line2()                                          //to print the time,event[i] and speed
{
    clcd_print(time,LINE2(0));
    clcd_print(event[index],LINE2(10));
    clcd_putch(speed/10+48,LINE2(14));
    clcd_putch(speed%10+48,LINE2(15));   
}

void store_data(char *event)           //to store all the data of event[index]
{
  data[0] = time[0];
  data[1] = time[1];
  data[2] = time[3];
  data[3] = time[4];
  data[4] = time[6];
  data[5] = time[7];
  data[6] = event[0];
  data[7] = event[1];
  data[8] = (speed/10+48);
  data[9] = (speed%10+48);
  data[10] = '\0';
}

void save_data(void)              //A function to save the data to external eeprom
{
    if(wr_flag < 9)
        {
         wr_flag++;
        }
    else
        {
           wr_flag = 0;
           over_flag = 1;
        }
    for(i=0;i<10;i++)
    {
        write_external_eeprom((wr_flag%10)*10+i,data[i]);       //writing data to external eeprom  
    }
}



void main(void)
{
	init_config();
     store_data(event[index]);
     save_data();
	while (1)
	{
        key = read_switches(STATE_CHANGE);           //reading switches and storing in key
        Line1();                                    //displaying line1 function on clcd
        
        if (key == MK_SW11) {                    
            dash_flag = 0;
            pass_flag = 1;
        }
        if(dash_flag == 1){
            get_time();
            Line2(); 
                if (key == MK_SW1) {                      //if switch1 is pressed store c in index
                    index = 7;
                     store_data(event[index]);
                     save_data();
                } else if (key == MK_SW2) {               //if switch2 is pressed incrementing the index
                    if (index == 7)
                        index = 2;
                    if (index < 6)
                        index++;
                    store_data(event[index]);
                     save_data();
                } else if (key == MK_SW3) {             //if switch3 is pressed decrementing the index
                    if (index == 7)
                        index = 2;
                    if (index > 1)
                        index--;
                    store_data(event[index]);
                     save_data();

                }
                speed = read_adc(CHANNEL4) / 10.33;                //reading the value form adc using channel4
            
        }
    
        else if(pass_flag == 1)              //if passflag is 1 call password function
        {
            
            pass(); 
            pass_flag = 0;
            dash_flag = 1;
            
         }
       
	}
}