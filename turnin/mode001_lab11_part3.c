/*              Name & E-mail: Michael O'Dea, mode001@ucr.edu
 *              Lab Section: 025
 *              Assignment: Lab #11  Exercise #3
 *              Exercise Description: Combine the functionality of the keypad 
 *              and LCD so when keypad is pressed and released, the character 
 *              of the button pressed is displayed on the LCD, and stays 
 *              displayed until a different button press occurs (May be 
 *              accomplished with two tasks: LCD interface & modified test 
 *              harness).
 *
 *              I acknowledge all content contained herein, excluding 
 *              template or example code, is my own original work.
 */

//DEMO: https://drive.google.com/open?id=1G5H2JfqnXWW7Kyf7QE6bUXN8kdWcfSb-

#ifdef _SIMULATE_
#include <avr/io.h>
#include <stdio.h>
#include "io.h"
#include "io.c"
#include "simAVRHeader.h"
#include <avr/interrupt.h>
#include "bit.h"
#include "scheduler.h"
#include "keypad.h"
#include "timer.h"
#endif

unsigned char k;
enum Keypad_States {KEYPAD};
int Tick_Keypad(int state){
	k = GetKeypadKey();
        switch(state){
                case KEYPAD:
                break;
                
		default:
                state = KEYPAD;
                break;
        }
        switch(state){
                case KEYPAD:
                switch(k){
                	case '\0':  
			break;
                        
			case '0':  
			LCD_Cursor(1);
		       	LCD_WriteData(0x00 + '0');  
			break;
                        
			case '1': 
			LCD_Cursor(1); 
			LCD_WriteData(0x01 + '0');  
			break;
                        
			case '2': 
			LCD_Cursor(1); 
			LCD_WriteData(0x02 + '0'); 
		       	break;
                       
		       	case '3':  
			LCD_Cursor(1); 
			LCD_WriteData(0x03 + '0');  
			break;
                        
			case '4':
			LCD_Cursor(1); 
			LCD_WriteData(0x04 + '0'); 
		       	break;
                        
			case '5':  
			LCD_Cursor(1); 
			LCD_WriteData(0x05 + '0');  
			break;
                        
			case '6': 
			LCD_Cursor(1); 
			LCD_WriteData(0x06 + '0');  
			break;
                        
			case '7': 
			LCD_Cursor(1); 
			LCD_WriteData(0x07 + '0');  
			break;
                        
			case '8': 
			LCD_Cursor(1); 
			LCD_WriteData(0x08 + '0');  
			break;
                        
			case '9':  
			LCD_Cursor(1); 
			LCD_WriteData(0x09 + '0'); 
		       	break;
                        
			case '*': 
			LCD_Cursor(1); 
			LCD_WriteData(0x2A); 
			break;
                        
			case '#': 
			LCD_Cursor(1); 
			LCD_WriteData(0x23); 
			break;
                        
			case 'A': 
			LCD_Cursor(1); 
			LCD_WriteData(0x41); 
			break;
                        
			case 'B': 
			LCD_Cursor(1); 
			LCD_WriteData(0x42); 
			break;
                        
			case 'C':  
			LCD_Cursor(1); 
			LCD_WriteData(0x43); 
			break;
                        
			case 'D':
			LCD_Cursor(1); 
			LCD_WriteData(0x44); 
			break;
                        
			default: 
			break;

                }
               	break;
                
		default: 
		break;
        }

        return state;

}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    DDRD = 0xFF; PORTD = 0x00;

    static task task1;
    task *tasks[] = {&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
    const char start = -1;
    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &Tick_Keypad;

    TimerSet(2);
    TimerOn();
    LCD_init();
    LCD_ClearScreen();
    unsigned char j;
    while (1) {
	for(j = 0; j < numTasks; j++){
		if(tasks[j]->elapsedTime == tasks[j]->period){
			tasks[j]->state = tasks[j]->TickFct(tasks[j]->state);
			tasks[j]->elapsedTime = 0;
		}
		tasks[j]->elapsedTime += 2;
	}
	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 0;
}
