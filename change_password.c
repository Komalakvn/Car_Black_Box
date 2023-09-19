/*
 * File:   change_password.c
 * Author: komala
 *
 * Created on 24 August, 2023, 12:13 PM
 */


#include <xc.h>
#include "clcd.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "ext_eeprom.h"
#include "main.h"

int count1 = 0,count2 = 0;
extern char password[5];
extern char str[5];
extern char event[][3];
extern char data[11];
char key;
unsigned long int delay=0;
unsigned char user1[4];
unsigned char user1[4];


int ask_password(int count,int stage)           //function to take input password from user
{
    key = read_switches(STATE_CHANGE);
    for(int i=100;i>0;i--);
           if(delay++ <= 500)
              clcd_putch('_',LINE2(count));
           else if(delay <= 1000)
              clcd_putch(' ',LINE2(count));
           else
              delay = 0;
           if(stage == 1)
           {
               if (key == MK_SW11)
               {
                 clcd_putch('*', LINE2(count));
                 password[count] = '0';
                 count++;
               } 
               else if (key == MK_SW12) 
               {
                 clcd_putch('*', LINE2(count));
                 password[count] = '1';
                 count++;
               }
           }
           else if(stage == 2)
           {
              if (key == MK_SW11) 
               {
                 clcd_putch('*', LINE2(count));
                 str[count] = '0';
                 count++;
               } 
               else if (key == MK_SW12) 
               {
                 clcd_putch('*', LINE2(count));
                 str[count] = '1';
                 count++;
               } 
           }
    return count;
}


int match_password(void)           //function to compare the old password and new password
{
    int i = 0;
    while(password[i] != '\0')
    {
        if(str[i] != password[i])
        {
            return FALSE;
        }
        i++;
    }
    return TRUE;
}

void change_psw(void)
{
    store_data(event[11]);      //calling the store data function 
    save_data();               //saving the data
    int once = 1;
    clear_scr();
    while(1)
    {
        if(count1 < 4)
       {
           clcd_print("New Password  ",LINE1(0));            //printing the new password on clcd
           count1 = ask_password(count1,1);                   //taking input for new password
       }
       else if(count1 == 4 && count2 < 4)
       {
          clcd_print("Renter Password  ",LINE1(0));            //re entering the password on clcd
          if(once)
          {
               clcd_print("     ",LINE2(0));                    //clearing the screen
               once = 0;
          }
          count2 = ask_password(count2,2);
       }
       else
       {
         password[4] = '\0';
         str[4] = '\0';
         if(match_password() == FALSE)             //checking the password matched is false
         {
             clear_scr();
             clcd_print("Wrong Match",LINE1(2));             
                 for(int j=1000;j>0;j--)
                     for(int i= 1000; i--;);
                 count1 = 0;
                 count2 = 0;
             break;
             
         }
         else
         {
             clear_scr();
             store_password();
             clcd_print("Password_changed",LINE1(0));       // new password changed 
             for(int j=1000;j>0;j--)
                     for(int i= 1000;i>0;i--);
                     clear_scr();
               count1 = 0;
               count2 = 0;     
             break;
         }
       }
    }
}

    
    
    
   

