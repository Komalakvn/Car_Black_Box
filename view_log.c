/*
 * File:   view_log.c
 * Author: komala
 *
 * Created on 24 August, 2023, 12:11 PM
 */


#include <xc.h>
#include "clcd.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "ext_eeprom.h"
#include "main.h"


extern int wr_flag;
extern char over_flag;
int s_lap,ind=0;
char pre_key,key;
extern char event[][3];

void check_event(void)                //checking the event 
{
    if(over_flag == 0)
   {
       s_lap = wr_flag;
   }
   else 
       s_lap = 10;
}

void print_log(void)              //function to display all the events on clcd
{
  char data = 0,j = 1; 
  
  for(int i=0;i<10;i++)
  {
      
      if(i == 2 || i == 4)
      {
          clcd_putch(':',LINE2(j));
          j++;
          
      }
      else if(i == 6 || i == 8)
      {
          clcd_putch(' ',LINE2(j));
          j++;
      }
      
         data = read_external_eeprom(((s_lap+ind) % 10) *10+i);
         clcd_putch(data,LINE2(j));
         j++;
  }
  
}


void view_log(void)            //function to store all the events 
{
    clear_scr();
    clcd_print("   My Logs       ",LINE1(0));
    check_event();
    int delay = 0;
    if(wr_flag != -1)
    {
       while(1)
       {
          print_log();
          pre_key = key;
          key = read_switches(LEVEL_CHANGE);
          if(key != 0xFF)
          {
            if(delay++ == 1000)
            {
              delay = 0;
              if(key == MK_SW12)
              {
                clear_scr();
                break;
              }
            }
         }
         else if(delay < 1000 && delay > 0)
         {
           delay = 0;
           if(pre_key == MK_SW12)
           {
             if(ind < s_lap)
               ind++;
           }
           else if(pre_key == MK_SW11)
           {
             if(ind > 0)
               ind--;
           }
         }
       
       }
    }
    else
    {
       clear_scr();
       clcd_print(" No Log Saved   ",LINE2(0));         //if no event is saved prints no log saved
       for(int i=0;i<5000;i++);    
    }
        
}
