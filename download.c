/*
 * File:   download.c
 * Author: komala
 *
 * Created on 24 August, 2023, 1:18 PM
 */


#include <xc.h>
#include "main.h"
#include "ext_eeprom.h"
#include "clcd.h"


extern int wr_flag;
extern char data[11];
extern char event[][3];
extern int s_lap;

void download(void)                           //download all stored events from external eeprom and print on clcd
{
     clcd_print("  Download Done ",LINE1(0));
    for(int i=1000;i--;)
        for(int j = 1000;j--;);
    init_uart();
    check_event();
    store_data(event[8]);
    save_data();
    check_event();
    clear_scr();
    for(int i = 0; i <=s_lap;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            if(j == 2 || j == 4)
                putch(':');
            else if(j == 6 || j == 8)
                putch(' ');
		    putch(read_external_eeprom(i * 10 + j));          //reading all the events from external eeprom
        }
        puts("\n\r");
    }
    clcd_print("  Download Done ",LINE1(0));
    for(int i=1000;i--;)
        for(int j = 1000;j--;);
    puts("Download done\n\r");
    
}
