#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;


void camera(Mat& frame, VideoCapture& cap){
	
	
	// 카메라 영상에 상하 반전 적용
	flip(frame, frame, 0); // 0은 상하 반전을 의미합니다.
	flip(frame, frame, 1); // 0: 수직 방향으로 좌우 반전

	imshow("Camera", frame);

}



#endif //CAMERA_CONTROL_H
