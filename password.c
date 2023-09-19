/*
 * File:   password.c
 * Author: komala
 *
 * Created on 24 August, 2023, 10:57 AM
 */


#include <xc.h>
#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"
#include "ext_eeprom.h"

unsigned char str[5];
static int count = 0;
unsigned long int delay = 0;
char password[] = "1111";

void check_matrix_keypad(void)
{
	unsigned char key;
	static unsigned int i;
	  key = read_switches(STATE_CHANGE);               // Reading Matrix keypad and storing in key
      for(i=0;i<200;i++);
      
      if(key == 11 && count <4)                             // If Switch1 pressed
      {
        str[count] = '0';
        clcd_putch('*',LINE2(count));
        count++;
      }
      else if(key == 12 && count <4)                              // If Switch4 is pressed
      {
        str[count] = '1';
        clcd_putch('*',LINE2(count));
        count++;
      }
      else if(count == 4)                                 // if 8 character taken from user
      {
          str[count] = '\0';
      }     
}

void clear_scr(void)                                //function to clear the screen 
{
    clcd_print("                ",LINE1(0));
    clcd_print("                ",LINE2(0));
}
void init_display(void)                             //function to display enter password
{
   clcd_print(" Enter Password ", LINE1(0)); 
}

void wrong_password(int attempt)                   //if wrong password is entered
{
    clear_scr();
    
    while(delay++ <= 2000)
    {   
      clcd_print("x Try Again",LINE1(0));
      clcd_putch('0'+attempt,LINE2(0));
      clcd_print("attempt left",LINE2(2));
    }
    
}
int _strcmp(const char* str1, const char* str2)            //comparing the 2 strings 
{
    int i=0;
    while(str1[i] != '\0')
    {
        if(str1[i] != str2[i])
        {
            return 1;
        }
        i++;
    }
    return 0;
}

void store_password()                       //storing the password
{
    for(int i=0;i<5;i++)
    {
       write_external_eeprom((100+i),password[i]);
    }
}

void pass(void)
{
    clear_scr();
    init_display();
    static int attempt = 3;
    unsigned char i = 0;
	while(1)
	{
        if(count < 4)
        {
		  check_matrix_keypad();
          for(int i=2000;i>0;i--);
        }
        else
        {
            clear_scr();
            if(_strcmp(password,str) == 1)                   // If Wrong Password Entered
            {
                      attempt--;
                      if(attempt > 0)
                      {
                        wrong_password(attempt);
                        delay = 0; 
                        count = 0;
                        clear_scr(); 
                        init_display();  
                      }
                      else                                  
                      {
                          clear_scr();
                          clcd_print("  Login Failed  ",LINE1(0));
                          for(int j = 120; j>0;j--)
                         {
                             clcd_putch((j/100)+48,LINE2(0));
                             clcd_putch(((j/10)%10)+48,LINE2(1));
                             clcd_putch((j%10)+48,LINE2(2));
                         for(volatile int i=10000;i>0;i--);
                         }
                          
                         delay = 0; 
                         count = 0;
                         clear_scr(); 
                         init_display();                          
                         attempt = 3;
                         
                      }
        
            }
            else                                     
            {
                clear_scr();
                clcd_print("Login Successful",LINE1(0));             //if correct password is entered 
                for(int i=0;i<200;i++);
                clear_scr();
                menu();                                          //calling the menu function
                break;
                
            }
                
        }
            
    }
       	
}
