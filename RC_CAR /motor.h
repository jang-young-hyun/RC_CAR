#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
 
//front
#define MOTOR1 25 // BCM_GPIO 26
#define MOTOR2 24 // BCM_GPIO 19 
#define MOTOR3 23 // BCM_GPIO 13 
#define MOTOR4 22 // BCM_GPIO 6
//back
#define MOTOR5 29 // BCM_GPIO 21
#define MOTOR6 28 // BCM_GPIO 20
#define MOTOR7 27 // BCM_GPIO 16
#define MOTOR8 26 // BCM_GPIO 12
//pwm
#define PWM_PIN_1 21 // BCM_GPIO 5
#define PWM_PIN_2 3 // BCM_GPIO 22
#define PWM_PIN_3 6 // BCM_GPIO 15
#define PWM_PIN_4 5 // BCM_GPIO 24

void motorForward();
void motorBackward();
void motorStop();
void motor_turn_right();
void motor_turn_left();
void motorForward_turn();

void motorForward() {
   digitalWrite(MOTOR1, HIGH);
   digitalWrite(MOTOR2, LOW);

   digitalWrite(MOTOR3, HIGH);
   digitalWrite(MOTOR4, LOW);

   digitalWrite(MOTOR5, HIGH);
   digitalWrite(MOTOR6, LOW);

   digitalWrite(MOTOR7, HIGH);
   digitalWrite(MOTOR8, LOW);

   softPwmWrite(PWM_PIN_1, 22);
   softPwmWrite(PWM_PIN_2, 22);
   softPwmWrite(PWM_PIN_3, 22);
   softPwmWrite(PWM_PIN_4, 22);

}

void motorForward2() {
   digitalWrite(MOTOR1, HIGH);
   digitalWrite(MOTOR2, LOW);

   digitalWrite(MOTOR3, HIGH);
   digitalWrite(MOTOR4, LOW);

   digitalWrite(MOTOR5, HIGH);
   digitalWrite(MOTOR6, LOW);

   digitalWrite(MOTOR7, HIGH);
   digitalWrite(MOTOR8, LOW);

   softPwmWrite(PWM_PIN_1, 90);
   softPwmWrite(PWM_PIN_2, 90);
   softPwmWrite(PWM_PIN_3, 90);
   softPwmWrite(PWM_PIN_4, 90);

}

void motorForward3() {
   digitalWrite(MOTOR1, HIGH);
   digitalWrite(MOTOR2, LOW);

   digitalWrite(MOTOR3, HIGH);
   digitalWrite(MOTOR4, LOW);

   digitalWrite(MOTOR5, HIGH);
   digitalWrite(MOTOR6, LOW);

   digitalWrite(MOTOR7, HIGH);
   digitalWrite(MOTOR8, LOW);


}

void motorBackward() {
   digitalWrite(MOTOR1, LOW);
   digitalWrite(MOTOR2, HIGH);

   digitalWrite(MOTOR3, LOW);
   digitalWrite(MOTOR4, HIGH);

   digitalWrite(MOTOR5, LOW);
   digitalWrite(MOTOR6, HIGH);

   digitalWrite(MOTOR7, LOW);
   digitalWrite(MOTOR8, HIGH);

   softPwmWrite(PWM_PIN_1, 35);
   softPwmWrite(PWM_PIN_2, 35);
   softPwmWrite(PWM_PIN_3, 35);
   softPwmWrite(PWM_PIN_4, 35);
}

void motorStop() {
   digitalWrite(MOTOR1, LOW);
   digitalWrite(MOTOR2, LOW);

   digitalWrite(MOTOR3, LOW);
   digitalWrite(MOTOR4, LOW);

   digitalWrite(MOTOR5, LOW);
   digitalWrite(MOTOR6, LOW);

   digitalWrite(MOTOR7, LOW);
   digitalWrite(MOTOR8, LOW);
}

void motor_turn_right() {//2,4
   //전진앞으로 도는 바퀴
   digitalWrite(MOTOR1, HIGH);
   digitalWrite(MOTOR2, LOW);

   digitalWrite(MOTOR7, HIGH);
   digitalWrite(MOTOR8, LOW);


   //뒤로 도는 바퀴   
   
   digitalWrite(MOTOR5, LOW);
   digitalWrite(MOTOR6, HIGH);


   digitalWrite(MOTOR3, LOW);
   digitalWrite(MOTOR4, HIGH);
   
   softPwmWrite(PWM_PIN_1, 150);
   softPwmWrite(PWM_PIN_3, 150);
   softPwmWrite(PWM_PIN_4, 40);
   softPwmWrite(PWM_PIN_2, 40);
}

void motor_turn_right2() {//완만한 각도
   //전진앞으로 도는 바퀴
   digitalWrite(MOTOR1, HIGH);
   digitalWrite(MOTOR2, LOW);

   digitalWrite(MOTOR7, HIGH);
   digitalWrite(MOTOR8, LOW);


   //뒤로 도는 바퀴   
   
   digitalWrite(MOTOR5, LOW);
   digitalWrite(MOTOR6, HIGH);


   digitalWrite(MOTOR3, LOW);
   digitalWrite(MOTOR4, HIGH);
   
   softPwmWrite(PWM_PIN_1, 110);
   softPwmWrite(PWM_PIN_3, 110);
   softPwmWrite(PWM_PIN_4, 30);
   softPwmWrite(PWM_PIN_2, 30);
}


void motor_turn_left() {	//완만한 각도
	//뒤로 도는 바퀴

   digitalWrite(MOTOR1, LOW);
   digitalWrite(MOTOR2, HIGH);

   digitalWrite(MOTOR7, LOW);
   digitalWrite(MOTOR8, HIGH);


   //앞으로 도는 바퀴 
   digitalWrite(MOTOR5, HIGH);
   digitalWrite(MOTOR6, LOW);


   digitalWrite(MOTOR3, HIGH);
   digitalWrite(MOTOR4, LOW);
	
   softPwmWrite(PWM_PIN_1, 30);
   softPwmWrite(PWM_PIN_3, 30);
   softPwmWrite(PWM_PIN_4, 110);
   softPwmWrite(PWM_PIN_2, 110);
   
}

#endif // MOTOR_H
