# RC_CAR
프로젝트 명 : 하만 커넥티드 자율 이동체 개발 프로젝트

주관기관 : [HARMAN] 세미콘 아카데미

프로젝트 인원 : 강은세, 김기영, 장영현(팀장), 홍소연

프로젝트 기간 : 2023.07.03 ~ 2023.08.18

사용 기술 : 
1.	개발 툴 : Putty, Real VNC, Genny
2.	개발 보드 : Raspberry Pi4
3.	사용 기술 : Bluetooth(Flask), OPENCV, PWM제어, ADC(Ultrasonic)

- 세부 내용

1. 개요

(1)	목적 – 보행 간의 교통사고를 방지하기 위한 Safety Bar 제작

(2)	발표영상 https://www.youtube.com/watch?v=TLFrJKQKpiY



2.	시스템, 구성요소

   (1)	하드웨어 블록도
   
   ![image](https://github.com/jang-young-hyun/RC_CAR/assets/124988949/4725c00c-4404-4fc4-97b0-0e68b0fc7f2a)

   (2) SW Stack

   ![image](https://github.com/jang-young-hyun/RC_CAR/assets/124988949/38f19db8-a047-4785-a49a-0ddbc861ffb8)

   (3) 기능 및 외형

   ![image](https://github.com/jang-young-hyun/RC_CAR/assets/124988949/cecb8144-e7e3-457c-8b77-0ec039da34c6)


① Passive Mode

→ Pi Camera

OPENCV의 Videocapture 클래스를 사용하여 연속적으로 카메라 스트리밍을 할 수 있게 구현

→ Bluetooth Controller

(1)	Raspberry Pi를 server로, App inventor(Bluetooth Controller)를 client로 구성

(2)	Controller가 Flask 웹 서버로 HTTP 요청을 보내면, Server로 온 HTTP 응답을 받는 형식으로 App의 Controller 기능을 구현


② Auto Mode

→ OPENCV

(1)	정확한 차선 검출을 위해 Camera에 스트리밍되는 영상을 Grayscale, Gaussian Filter를 이용하여 Noise를 제거

(2)	Hough Transform을 이용하여 검출되는 직선 성분의 선을 표시, 표시되는 선들의 끝점들의 평균값을 구하여 직진, 좌회전, 우회전을 결정


→ Thread

(1)	프로세스의 과부하를 방지하기 위해 차선 검출, 차량 이동, 초음파 검출의 3가지 Tread를 생성

(2)	다중 스레드 환경에서 차선 검출되는 return값을 안전하게 받기 위해 atomic을 사용

(3)	차선 검출에서 적용되는 Hough Transform을 MAIN Tread에서 안전하게 스트리밍하기 위해 mutex를 이용하여 차선 검출된 Mat 클래스 변수를 전달

③ 추가 기능

→ Bluetooth Controller를 통해 Mode 전환 가능




3.	BOM (Bill Of Materials)

![image](https://github.com/jang-young-hyun/RC_CAR/assets/124988949/f927b780-b125-4699-970f-4c25105eedc7)



4.	개선사항
   
① 차선 검출 알고리즘

초기 차선 검출 알고리즘 작성 시 Hough Transform을 통해 검출되는 좌, 우 차선의 교차점을 기준으로 코드를 작성 

 → 그러나 해당 알고리즘으로 동작 시 교차점 값을 계산하기 위한 연산이 많이 필요 
 
 → 0 ~ 30도와 같이 수평선에 근접한 선들은 검출에서 제외하고 검출되는 선들의 끝점의 평균값을 구하는 방법으로 연산 횟수 감소

② Thread

초기 여러 센서를 사용하여 수동 모드에서 자동 모드로 전환 시 프로세스의 과부하로 작동이 중지 

→ 차선 검출, 차량 이동, 초음파 검출 등 3가지 기능에 Thread를 만들어 프로세스의 과부하 해소

③ 카메라 스트리밍

차선 검출 Thread 적용 시 Thread 적용 전과 동일하게 감소된 프레임으로 스트리밍을 진행 

-> 원인 분석 결과 차선 검출 Thread 내에 스트리밍의 GUI를 만들어주는 imshow() 함수가 sub Thread에 있는 것을 확인 

-> imshow() 함수를 MAIN Thread에서 원활하게 작동하기 위해 매개변수인 Mat 클래스 변수를 mutex를 이용하여 안전하게 변수 전달



5.	느낀점
   
① 차량에 조향 장치를 달지 않아 좌, 우회전 시 각 모터의 high, low 상태를 계속 변경해줘야 하여 순간적인 좌, 우회전 상황에서 즉각적으로 반응하지 않았다. 

-> 설계 전 여러 상황을 가정하여 H/W 설계를 해야 한다는 것을 확인


② OPENCV의 GUI를 Thread 내에서 호출하여 낮은 프레임으로 스트리밍되었다. 

-> GUI와 같은 UI 업데이트를 처리할 때는 MAIN Thread에서 동작하도록 설계해야 한다는 것을 확인 
