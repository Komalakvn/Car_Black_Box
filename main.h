#ifndef MAIN_H
#define MAIN_H
void pass(void);
void menu(void);
void clear_scr(void);
void store_data(char *event);
void save_data(void);
void change_psw(void);
void view_log(void);
void get_time(void);
void check_event(void);
void download(void);
void clear_log(void);
void store_password(void);
void set_time(void);

#define FALSE      0
#define TRUE       1


/*Download */
#define RX_PIN					TRISC7
#define TX_PIN					TRISC6

void init_uart(void);
void putch(unsigned char byte);
int puts(const char *s);
unsigned char getch(void);
unsigned char getch_with_timeout(unsigned short max_time);
unsigned char getche(void);


#endif