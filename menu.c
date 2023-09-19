/*
 * File:   menu.c
 * Author: komala
 *
 * Created on 24 August, 2023, 12:04 PM
 */


#include <xc.h>
#include "matrix_keypad.h"
#include "main.h"
#include "clcd.h"

unsigned char log[5][16] = {" View log      "," Download log  "," Clear log     "," Set time      "," Change Pswd   "};
unsigned int menu_flag = 0;
unsigned  int prev_flag = 0, j;

void display_menu()                         
{
    if(prev_flag == menu_flag)
    {
        clcd_putch('*',LINE1(0));
    clcd_print(log[menu_flag],LINE1(2));
    clcd_print(log[menu_flag+1],LINE2(2));
    j = menu_flag;
    }
    else if(menu_flag > prev_flag)
    {
        j = menu_flag ;
        clcd_putch(' ',LINE1(0));
        clcd_putch('*',LINE2(0));
        clcd_print(log[prev_flag],LINE1(2));
        clcd_print(log[menu_flag],LINE2(2));
    }
    else if(prev_flag > menu_flag)
    {
        j = menu_flag;
        clcd_putch(' ',LINE2(0));
        clcd_putch('*',LINE1(0));
        clcd_print(log[menu_flag],LINE1(2));
        clcd_print(log[prev_flag],LINE2(2));
    }
    
}
void menu(void) 
{
    unsigned char key,pre_key;
    unsigned int delay = 0;
    clear_scr();
    while(1)
    {
        
    display_menu();
    pre_key = key;
    key = read_switches(LEVEL_CHANGE);
    //for (int i =0;i<200; i++);
    if(key != 0xFF)
    {
        if(delay++ == 1000)
        {
            delay=0;
            if(key == MK_SW11)                   //if switch 11 is pressed 
            {
                clcd_putch(j+48,LINE1(14));
                for(int i=1000;i--;)
                    for(int j=100;j--;);
                if(j == 0)                  //if j is equall to 0 call viewlog function
                {
                    clear_scr();
                    view_log();
                }
                else if(j == 1)                  //if j is equall to 1 call download function     
                {
                    clear_scr();
                    download();
                }
                else if(j == 2)                 //if j is equall to 2 call clear log function
                {
                   clear_log();
                }
                else if(j == 3)                 //if j is equall to 3 call set time function
                {
                    set_time();
                }
                else if(j == 4)                    //if j is equall to 4 call change password function
                {
                    clear_scr();
                    change_psw();
                }
            }
            else if(key==MK_SW12)            //if switch 12 is pressed clear screen and come out of loop
            {
                clear_scr();
                break;
            }
        }
    }
    else if (delay < 1000 && delay > 0)           //if switch is 
    {
        delay=0;
            if(pre_key == MK_SW11) {
                prev_flag = menu_flag;
                if (menu_flag > 0)
                    menu_flag--;
                    
            } else if (pre_key == MK_SW12) {

                if(menu_flag < 4){
                    prev_flag = menu_flag;
                    menu_flag++;
                }

            }
    }
        
    }    
}
