#ifndef SONIC_H
#define SONIC_H

#include <stdio.h>
#include <wiringPi.h>

#define pinTrigger_1 0 // BCM_GPIO 17
#define pinEcho_1 2 // BCM_GPIO 27

#define pinTrigger_2 1 // BCM_GPIO 18
#define pinEcho_2 4 // BCM_GPIO 23

double distance(int pinTrigger, int pinEcho);

double distance(int pinTrigger,int pinEcho){
   digitalWrite(pinTrigger, LOW);   
   delayMicroseconds(2);
   
   digitalWrite(pinTrigger, HIGH);  
   delayMicroseconds(10);
   
   digitalWrite(pinTrigger, LOW);   
   delayMicroseconds(200);

   while(digitalRead(pinEcho)==LOW);
   long startTime = micros();
   
   while(digitalRead(pinEcho)==HIGH);
   long endTime = micros()-startTime;
   
   double distance_result = endTime * 0.034 / 2;
   return distance_result;
}

#endif // SONIC_H
