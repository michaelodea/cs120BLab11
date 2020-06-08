/*              Name & E-mail: Michael O'Dea, mode001@ucr.edu
 *              Lab Section: 025
 *              Assignment: Lab #10  Exercise #1 
 *              Exercise Description: Use the LCD* code, along with a button 
 *              and/or time delay to display the message "CS120B is Legend... 
 *              wait for it DARY!". The string will not fit on the display 
 *              all at once, so you will need to come up with some way to
 *              paginate or scroll the text. Note: If your LCD is 
 *              exceptionally dim, adjust the resistance provided by the 
 *              potentiometer connected to Pin #3. In a previous lab, the LCD
 *              was controlled through PORTC and PORTD, the provided header 
 *              files should be set to use PORTA, PORTB, and/or PORTD (so 
 *              that PORTC can continue to be used for the keypad).
 *
 *              I acknowledge all content contained herein, excluding 
 *              template or example code, is my own original work.
 */

//DEMO: https://drive.google.com/file/d/1Fzqm4zQ6dlztJdr8kyE6ljQN5w1oEJHs/view?usp=sharing


#ifdef _SIMULATE_
#include <avr/io.h>
#include "simAVRHeader.h"
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"
#include "io.c"
#endif

unsigned char message[45] = {' ', ' ', ' ', ' ', 'C', 'S', '1','2','0','B',' ','i','s',' ', 'L','e', 'g','e' ,'n' , 'd', '.','.' ,'.' , ' ', 'w', 'a','i' , 't', ' ', 'f', 'o' ,'r',' ','i','t',' ','D','A','R','Y','!', ' ', ' ', ' ', ' '};

unsigned char move  = 1;
unsigned char size = sizeof(message);
unsigned char i;

enum LCD_States{BEGIN};
int Tick_Print(int lcd_state){
    switch(lcd_state){
        case BEGIN:
        for(i = 0; i <= 16; i++){
        	LCD_Cursor(i);
        	LCD_WriteData(message[move + i - 2]);
        	if(move + i == (size - 1)){
        		move = 1;
         	}
        }
        move++;
        lcd_state = BEGIN;
        break;

        default: 
	lcd_state = BEGIN; 
	break;
    }

    return lcd_state;
}

int main(void) {

    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    const char start = -1;
    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &Tick_Print;

    TimerSet(2);
    TimerOn();
    LCD_init();

    unsigned short i;
    while (1) {
        for(i = 0; i < numTasks; i++){
            if(tasks[i]->elapsedTime == tasks[i]->period){
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elapsedTime += 2;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}


