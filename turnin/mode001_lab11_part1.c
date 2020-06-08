/*              Name & E-mail: Michael O'Dea, mode001@ucr.edu
 *              Lab Section: 025
 *              Assignment: Lab #11  Exercise #1 
 *              Exercise Description: Modify the keypad code to be in an SM 
 *              task. Then, modify the keypad SM to utilize the simple task 
 *              scheduler format. All code from here on out should use the 
 *              task scheduler. 
 *
 *              I acknowledge all content contained herein, excluding 
 *              template or example code, is my own original work.
 */

#ifndef __SIMULATE__
#include <avr/io.h>
#include "simAVRHeader.h"
#include <avr/interrupt.h>
#include "scheduler.h"
#include "keypad.h"
#include "timer.h"
#endif

enum Keypad_States {keypad};
int Tick_Keypad(int keypad_state){
        unsigned char k = GetKeypadKey();
        switch(keypad_state){
                case keypad:
                keypad_state = keypad;
                break;
                
		default:
                keypad_state = keypad;
                break;
        }
        switch(keypad_state){
                case keypad:
                switch(k){
                        case '\0': 
			PORTB = 0x1F;
		       	break;
                       
		       	case '0': 
			PORTB = 0x00; 
			break;
                        
			case '1': 
			PORTB = 0x01; 
			break;
                        
			case '2': 
			PORTB = 0x02; 
			break;
                        
			case '3': 
			PORTB = 0x03; 
			break;
                        
			case '4': 
			PORTB = 0x04; 
			break;
                        
			case '5': 
			PORTB = 0x05; 
			break;
                        
			case '6': 
			PORTB = 0x06;
		       	break;
                       
		       	case '7': 
			PORTB = 0x07; 
			break;
                        
			case '8': 
			PORTB = 0x08; 
			break;
                        
			case '9': 
			PORTB = 0x09; 
			break;
                        
			case '*': 
			PORTB = 0x0E; 
			break;
                        
			case '#': 
			PORTB = 0x0F; 
			break;
                        
			case 'A': 
			PORTB = 0x0A; 
			break;
                        
			case 'B': 
			PORTB = 0x0B; 
			break;
                        
			case 'C':
		       	PORTB = 0x0C; 
			break;
                        
			case 'D': 
			PORTB = 0x0D; 
			break;
                        
			default: 
			PORTB = 0x1B; 
			break;
                }
                break;
                
		default: 
		break;
        }
        return keypad_state;
}

int main(void) {
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;

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
    unsigned short j;
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
