#ifndef CONTROL_H
#define CONTROL_H

#include <stdio.h>
#include <wiringPi.h>
#include "motor_control.h"

#define pinTOUCH 7 //touch updown (=BCM4)
#define pinF 0    // (=BCM 17) 
#define pinB 2
#define pinL 1
#define pinR 4 
#define pinPassive 8

void control(){
	//http 설정
    pinMode(pinTOUCH,OUTPUT); // 터치 다운 업 조절
	pinMode(pinF, OUTPUT); // FOWARD 조절
    pinMode(pinB, OUTPUT); // BACKWARD
    pinMode(pinL, OUTPUT);  //left
    pinMode(pinR, OUTPUT);  //right
    	
    
	if(digitalRead(pinPassive)==1){ //passive mode 

		if(digitalRead(pinTOUCH)==1){ //아무 버튼 눌러지면 
			printf("touchDown\n");

			if(digitalRead(pinL) == 1){ //좌회전 
				digitalWrite(pinR,0);
				digitalWrite(pinF,0);
				digitalWrite(pinB,0);

				motor_turn_left_C();
				printf("left_receive\n");

				delay(10);
			}  

			if(digitalRead(pinR) == 1){ //우회전 
				digitalWrite(pinL,0);
				digitalWrite(pinF,0);
				digitalWrite(pinB,0);
				
				motor_turn_right_C();
				printf("right_receive\n");
				
				delay(10);
			}     

			if(digitalRead(pinF) == 1){ //전진 
				digitalWrite(pinL,0);
				digitalWrite(pinR,0);
				digitalWrite(pinB,0);

				motorForward_C();
				printf("forward_receive\n");
				
				delay(10);
			}

			if(digitalRead(pinB) == 1){ //후진 
				digitalWrite(pinL,0);
				digitalWrite(pinR,0);
				digitalWrite(pinF,0);

				motorBackward_C();

				printf("backward_receive\n");
				delay(10);
			}     
		}

		if(digitalRead(pinTOUCH)==0){
			digitalWrite(pinL,0);
			digitalWrite(pinR,0);
			digitalWrite(pinF,0);
			digitalWrite(pinB,0);
			
			motorStop_C();
			printf("touchUp\n");
			
			delay(10);
		}

	}

	if(digitalRead(pinPassive)==0){
		motorStop_C();
		printf("Auto mode\n");
		
		delay(1000);
	}
}


#endif //CONTROL_H
