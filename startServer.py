#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

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

void motorForward_C();
void motorBackward_C();
void motorStop_C();
void motor_turn_right_C();
void motor_turn_left_C();

void motorForward_C() {
   digitalWrite(MOTOR1, HIGH);
   digitalWrite(MOTOR2, LOW);

   digitalWrite(MOTOR3, HIGH);
   digitalWrite(MOTOR4, LOW);

   digitalWrite(MOTOR5, HIGH);
   digitalWrite(MOTOR6, LOW);

   digitalWrite(MOTOR7, HIGH);
   digitalWrite(MOTOR8, LOW);

   softPwmWrite(PWM_PIN_1, 70);
   softPwmWrite(PWM_PIN_2, 70);
   softPwmWrite(PWM_PIN_3, 70);
   softPwmWrite(PWM_PIN_4, 70);

}

void motorBackward_C() {
   digitalWrite(MOTOR1, LOW);
   digitalWrite(MOTOR2, HIGH);

   digitalWrite(MOTOR3, LOW);
   digitalWrite(MOTOR4, HIGH);

   digitalWrite(MOTOR5, LOW);
   digitalWrite(MOTOR6, HIGH);

   digitalWrite(MOTOR7, LOW);
   digitalWrite(MOTOR8, HIGH);

   softPwmWrite(PWM_PIN_1, 70);
   softPwmWrite(PWM_PIN_2, 70);
   softPwmWrite(PWM_PIN_3, 70);
   softPwmWrite(PWM_PIN_4, 70);
}

void motorStop_C() {
   digitalWrite(MOTOR1, LOW);
   digitalWrite(MOTOR2, LOW);

   digitalWrite(MOTOR3, LOW);
   digitalWrite(MOTOR4, LOW);

   digitalWrite(MOTOR5, LOW);
   digitalWrite(MOTOR6, LOW);

   digitalWrite(MOTOR7, LOW);
   digitalWrite(MOTOR8, LOW);
}

void motor_turn_right_C() {//2,4
   //전진앞으로 도는 바퀴
   digitalWrite(MOTOR1, HIGH);softPwmWrite(PWM_PIN_1, 140);
   digitalWrite(MOTOR2, LOW);

   digitalWrite(MOTOR7, HIGH);softPwmWrite(PWM_PIN_3, 140);
   digitalWrite(MOTOR8, LOW);


   //뒤로 도는 바퀴   
   digitalWrite(MOTOR5, LOW);softPwmWrite(PWM_PIN_4, 40);
   digitalWrite(MOTOR6, HIGH);


   digitalWrite(MOTOR3, LOW);softPwmWrite(PWM_PIN_2, 40);
   digitalWrite(MOTOR4, HIGH);

}

void motor_turn_left_C() {	
	//뒤로 도는 바퀴
   digitalWrite(MOTOR1, LOW);softPwmWrite(PWM_PIN_1, 40);
   digitalWrite(MOTOR2, HIGH);

   digitalWrite(MOTOR7, LOW);softPwmWrite(PWM_PIN_3, 40);
   digitalWrite(MOTOR8, HIGH);


   //앞으로 도는 바퀴 
   digitalWrite(MOTOR5, HIGH);softPwmWrite(PWM_PIN_4, 140);
   digitalWrite(MOTOR6, LOW);


   digitalWrite(MOTOR3, HIGH);softPwmWrite(PWM_PIN_2, 140);
   digitalWrite(MOTOR4, LOW);
	
}

#endif // MOTOR_CONTROL_H 
