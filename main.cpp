#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>

#include "motor.h"
#include "sonic.h"
#include "lane_detect.h"
#include "control.h"
#include "motor_control.h"
#include "camera_control.h"

using namespace cv;
using namespace std;

// 센서 처리 스레드 함수
void sensorProcessingThread(double& dis1, double& dis2, bool& stopSensor);
void laneProcessingThread(Mat& frame, VideoCapture& cap, bool& stopLane);

// 검출된 라인을 통한 모터 제어 스레드 함수
void detectProcessingMoveThread(bool& stopDetect);

// 센서 처리 스레드 객체
thread sensorProcessingThreadObj;
thread laneProcessingThreadObj;
thread detectProcessingThreadObj;

int i=0;
std::atomic<int> direction_state(0);

// 큐와 뮤텍스
queue<Mat> frameQueue;
mutex frameMutex;

int main() {
    
    // 스레드 동작을 제어할 변수들을 생성, sensor
    bool stopSensor = false;
    bool firstSensor = false;
    
    // 스레드 동작을 제어할 변수들을 생성, lane
    bool stopLane = false;
    bool firstLane = false;
 
    // 스레드 동작을 제어할 변수들을 생성, detect
    bool stopDetect = false;
    bool firstDetect = false;
    
    // 센서 데이터를 저장할 변수들
    double dis1 = 0; // 자동차 앞 초음파 거리 변수
    double dis2 = 0; // 자동차 뒤 초음파 거리 변수
    
	Mat frame;
    VideoCapture cap;
    cap.open("libcamerasrc ! video/x-raw, width=1280, height=720, framerate=30/1 ! videoconvert ! videoscale ! video/x-raw, width=680, height=480 ! appsink");
	
    if (!cap.isOpened()) {
        cerr << "ERROR! 카메라를 열 수 없습니다.\n";
        return -1;
    }
    // 카메라 영상에 상하 반전 적용
    flip(frame, frame, 0); // 0은 상하 반전을 의미합니다.
    flip(frame, frame, 1); // 0: 수직 방향으로 좌우 반전
	
    if (wiringPiSetup() == -1) {// 라이브러리 초기화 함수
        printf("wiringPiSetup fail");
        return 1;
    }

    // 초음파(자동차 앞) 설정
    pinMode(pinTrigger_1, OUTPUT);
    pinMode(pinEcho_1, INPUT);

    // 초음파(자동차 뒤) 설정
    pinMode(pinTrigger_2, OUTPUT);
    pinMode(pinEcho_2, INPUT);

    //모터 설정
    pinMode(MOTOR1, OUTPUT);
    pinMode(MOTOR2, OUTPUT);
    pinMode(MOTOR3, OUTPUT);
    pinMode(MOTOR4, OUTPUT);
    pinMode(MOTOR5, OUTPUT);
    pinMode(MOTOR6, OUTPUT);
    pinMode(MOTOR7, OUTPUT);
    pinMode(MOTOR8, OUTPUT);

    //모터 pwm 설정
    softPwmCreate(PWM_PIN_1, 0, 200);
    softPwmCreate(PWM_PIN_2, 0, 200);
    softPwmCreate(PWM_PIN_3, 0, 200);
    softPwmCreate(PWM_PIN_4, 0, 200);
	
    //http 초기 상태 설정
    pinMode(pinPassive, OUTPUT); // PASSIVE/AUTO MODE, 1=PASSIVE
	digitalWrite(pinTOUCH,0);
    digitalWrite(pinPassive,0); // default: auto
    
    delay(2000);
    
    while (true) {
        //수동 모드
        if(digitalRead(pinPassive)){ //passive mode 
            
            //sensor thread stop
            stopSensor = true; // 스레드 종료를 위해 stopSensor 값을 true로 변경
            firstSensor = false;
            if (sensorProcessingThreadObj.joinable()) {
                sensorProcessingThreadObj.join(); // 스레드가 종료될 때까지 대기
                cout << "sensor 스레드가 종료됩니다." << endl;
            }
            
            //lane thread stop
            stopLane = true; // 스레드 종료를 위해 stopSensor 값을 true로 변경
            firstLane = false;
            if (laneProcessingThreadObj.joinable()) {
                laneProcessingThreadObj.join(); // 스레드가 종료될 때까지 대기
                cout << "lane 스레드가 종료됩니다." << endl;
            }

            //lane thread stop
            stopDetect = true; // 스레드 종료를 위해 stopSensor 값을 true로 변경
            firstDetect = false;
            if (detectProcessingThreadObj.joinable()) {
                detectProcessingThreadObj.join(); // 스레드가 종료될 때까지 대기
                cout << "lane 스레드가 종료됩니다." << endl;
            }
            
            control();
            
            
            
            camera(frame, cap);
            
            if (waitKey(1) != -1)
                break;
            
        }
        
        
        //자동 모드
        else{
            
            //sonic thread start
            if(!firstSensor){
                sensorProcessingThreadObj = thread(sensorProcessingThread, ref(dis1), ref(dis2), ref(stopSensor));
                firstSensor = true;
            }
            
            //lane thread start
            if(!firstLane){
                laneProcessingThreadObj = thread(laneProcessingThread, std::ref(frame), std::ref(cap), std::ref(stopLane));
                firstLane = true;
            }
            
            // 메인 스레드에서 큐에 있는 프레임을 가져와서 처리
            if (!frameQueue.empty()) {
                Mat frame;
                {
                    lock_guard<mutex> lock(frameMutex);
                    frame = frameQueue.front();
                    frameQueue.pop();
                }

                // 원본 프레임을 imshow로 보여줌
                imshow("Lane Detect", frame);

                if (waitKey(1) != -1)
                    break;
            }
            
            if(!firstDetect){
                detectProcessingThreadObj = thread(detectProcessingMoveThread, std::ref(stopDetect));
                firstDetect = true;
            }

        }
    }

    return 0;
}

void sensorProcessingThread(double& dis1, double& dis2, bool& stopSensor) {
    while (!stopSensor) {
        // 여기에 센서 처리 로직을 추가합니다.
        dis1 = distance(pinTrigger_1, pinEcho_1);
        dis2 = distance(pinTrigger_2, pinEcho_2);

        printf("1. Distance:%.2fcm\n",dis1);      
        printf("2. Distance:%.2fcm\n",dis2); 

        if(dis1 <= 100)
            motorStop();
            
        if(dis2 <= 100)
            motorStop();
            
        // 0.5초 대기
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

void laneProcessingThread(Mat& frame, VideoCapture& cap, bool& stopLane){
    while (!stopLane) {
        
        cap.read(frame);
            
        if (frame.empty()) {
            cerr << "ERROR! 빈 프레임을 캡처했습니다.\n";
            break;
        }
        
        
        // 운전 방향 
        int local_direction_state = detectLane(frame, cap);
        direction_state.store(local_direction_state, std::memory_order_relaxed); // direction_state 업데이트
        
        {
            lock_guard<mutex> lock(frameMutex);
            //frameQueue.push(frame);
        }
             
        // 0.01초 대기
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void detectProcessingMoveThread(bool& stopDetect) {
    int prev_direction_state = -1; // 이전 동작 초기화
    int consecutive_count = 0; // 연속 횟수 초기화

    while (!stopDetect) {
        int current_direction_state = direction_state.load(std::memory_order_relaxed); // 원자적으로 변수 읽기
        cout << "current_direction_state : " << current_direction_state << endl;
         
        if (current_direction_state == prev_direction_state) {
            consecutive_count++;
            if (consecutive_count >= 25) { // 연속으로 5번 나타나면
                if (current_direction_state == 1) { // 직진
                    motorForward2();
                    cout << "차량 진행 방향: 직진 (지속 0.5초)" << endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    // 대기 시간
                    //std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 1초 동안 유지
                    

                } 
                

                consecutive_count = 0; // 연속 횟수 초기화
                prev_direction_state = -1; // 이전 동작 초기화
            }
        } 
        
        else {
            consecutive_count = 0; // 연속 횟수 초기화
            prev_direction_state = current_direction_state; // 이전 동작 업데이트
            
            if(current_direction_state == 1){
                printf("측정된 값 : %d\n",current_direction_state);
                motorForward();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                //motorStop() ;
                //std::this_thread::sleep_for(std::chrono::milliseconds(10));
                cout << "차량 진행 방향  : 직진" << endl;
            }

            
        }
    
    
        //우회전 
        if(current_direction_state == 3){
            printf("측정된 값 : %d\n",current_direction_state);
            for(int i = 0; i<2;i++){
                motor_turn_right2();
                //delay(50);
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 100밀리초 대기
            }
            
            for(int i = 0; i<3;i++){
                motorForward2();
                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 100밀리초 대기
                
                //delay(10);
            }
            
            cout << "차량 진행 방향  : 우회전" << endl;
        }

        //좌회전
        else if(current_direction_state == 2){
            printf("측정된 값 : %d\n",current_direction_state);
            
            for(int i = 0; i<=2;i++){
                motor_turn_left();
                //delay(50);
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 100밀리초 대기
            }
            
            for(int i = 0; i<=3;i++){
                motorForward2();
                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 100밀리초 대기
                
                //delay(10);
            }
            motorStop();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            cout << "차량 진행 방향  : 좌회전" << endl;
        }
        
        // 대기 시간
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 적절한 대기 시간을 선택합니다.
    }
}
