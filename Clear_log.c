/*
 * File:   Clear_log.c
 * Author: komala
 *
 * Created on 24 August, 2023, 7:59 PM
 */

#include <xc.h>
#include "main.h"
#include "clcd.h"


extern int wr_flag;
extern char over_flag;
extern char event[][3];
extern char data[11];

void clear_log(void){                 //clearing all the log events 
   check_event(); 
   wr_flag = -1;
   over_flag = 0;
   clear_scr();
   clcd_print("Log cleared  ",LINE1(2));             //after the log clearing printing on clcd
     for(int i=0;i<500;i++)
         for(int j=1000;j>0;j--);
     store_data(event[9]);
     save_data();
}
