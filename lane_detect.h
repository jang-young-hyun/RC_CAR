#ifndef LANE_DETECT_H
#define LANE_DETECT_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <cstdlib>

using namespace cv;
using namespace std;

struct Line {
    Point start;
    Point end;
};

int zeroCount; // 연속된 0 값 카운트 변수
int previousNonZeroX; // 0이 아닌 이전의 값
int previousDirectionState;

int sumX;
int validFrames;

int count_right;
int count_left;
int detectLane(Mat& frame, VideoCapture& cap){
    // 이미지 너비와 높이 얻기
    int width = frame.cols;
    int height = frame.rows;
    
    int angle_direction = 0;
    
    
    // 검출된 선들의 평균 x좌표를 저장할 배열과 인덱스 변수
    const int numFramesToAverage = 5; // 평균을 내는 프레임 수
    int averagedXCoordinates[numFramesToAverage] = {0};
    int currentIndex = 0;
    
    int direction_state =  0; // Variable to store the direction state

    int consecutiveRightOrStraight = 0; // 연속된 우회전 또는 직진 횟수
    const int consecutiveThreshold = 3; // 우회전 또는 직진 연속 허용 횟수

    
    // 카메라 영상에 상하 반전 적용
    flip(frame, frame, 0); // 0은 상하 반전을 의미합니다.
    flip(frame, frame, 1); // 0: 수직 방향으로 좌우 반전

    // 그레이스케일 변환
    Mat grayFrame;
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

    // 가우시안 블러 적용
    GaussianBlur(grayFrame, grayFrame, Size(5, 5), 0);

    // 케니 에지 검출
    Mat edges;
    Canny(grayFrame, edges, 50, 150);

    // 관심 영역 설정
    Point points[1][4];
    points[0][0] = Point(0, 720);
    points[0][1] = Point(0, 250);
    points[0][2] = Point(1280, 250);
    points[0][3] = Point(1280, 720);
    const Point* ppt[1] = { points[0] };
    int npt[] = { 4 };

    // 마스크 생성
    Mat mask = Mat::zeros(edges.size(), CV_8UC1);
    fillPoly(mask, ppt, npt, 1, Scalar(255));

    // 관심 영역 적용
    Mat roiEdges;
    bitwise_and(edges, mask, roiEdges);

    // 오른쪽과 왼쪽에 초록색 선 그리기
    Point rightCenter(width * 3.2 / 5, 0); // 오른쪽 상단 점
    Point rightCenter2(width * 3.2 / 5, height); // 오른쪽 하단 점
    Point leftCenter(width * 1.2 / 5, 0); // 왼쪽 상단 점
    Point leftCenter2(width * 1.2 / 5, height); // 왼쪽 하단 점

    Line rightGreenLine, leftGreenLine;

    // 오른쪽 초록색 선의 좌표를 설정합니다.
    rightGreenLine.start = rightCenter;
    rightGreenLine.end = rightCenter2;

    // 왼쪽 초록색 선의 좌표를 설정합니다.
    leftGreenLine.start = leftCenter;
    leftGreenLine.end = leftCenter2;

    // 초록색 선을 frame 이미지에 그립니다.
    line(frame, rightGreenLine.start, rightGreenLine.end, Scalar(0, 255, 0), 2);
    line(frame, leftGreenLine.start, leftGreenLine.end, Scalar(0, 255, 0), 2);

    // 허프 변환 적용
    vector<Vec2f> lines;
    HoughLines(roiEdges, lines, 1, CV_PI / 180, 100);

    // 허프 변환으로 검출된 선들의 끝점들을 저장할 벡터
    vector<Point> lineEndpoints;

    // 허프 변환으로 검출된 선들을 처리하는 코드 추가
    for (const Vec2f& line : lines) {
	float rho = line[0];
	float theta = line[1];

	// 허프 선을 직선 좌표로 변환
	Point2f pt1, pt2;
	double a = cos(theta), b = sin(theta);
	double x0 = a * rho, y0 = b * rho;
	pt1.x = cvRound(x0 + 1000 * (-b));
	pt1.y = cvRound(y0 + 1000 * (a));
	pt2.x = cvRound(x0 - 1000 * (-b));
	pt2.y = cvRound(y0 - 1000 * (a));

	// 끝점들을 벡터에 추가
	lineEndpoints.push_back(pt1);
	lineEndpoints.push_back(pt2);
	
	 // 각도 계산
	float angleInDegrees = atan2(pt2.y - pt1.y, pt2.x - pt1.x) * 180 / CV_PI;
	// 각도가 음수인 경우 보정
	if (angleInDegrees < 0) {
	    angleInDegrees += 180;
	}
	
	if((angleInDegrees >= 115 && angleInDegrees <= 140)){
	    if(count_right == 3){
		// 검출된 선들을 시각화하기 위해 이미지에 선을 그립니다.
		cv::line(frame, pt1, pt2, Scalar(0, 0, 255), 2);
		angle_direction = 3;
		cout << "현재 각도 : (R)" << angleInDegrees << endl;
		count_right = 0;
	    } else{
		angle_direction = 1;
		count_right++;
	    }
	}	   
	
	else if((angleInDegrees >= 30 && angleInDegrees <= 50)){
	    if(count_left == 150){
		// 검출된 선들을 시각화하기 위해 이미지에 선을 그립니다.
		cv::line(frame, pt1, pt2, Scalar(0, 0, 255), 2);
		angle_direction = 2;
		cout << "현재 각도 : (L)" << angleInDegrees << endl;
		count_left = 0;
	    } else{
		angle_direction = 1;
		count_left++;
	    }

	} 
	else if(!(angleInDegrees >= 0 && angleInDegrees < 30) && !(angleInDegrees >= 160 && angleInDegrees <= 180)){
	    // 검출된 선들을 시각화하기 위해 이미지에 선을 그립니다.
	    cv::line(frame, pt1, pt2, Scalar(0, 0, 255), 2);
	    cout << "현재 각도 : " << angleInDegrees << endl;
	}
	
    }
    
    // 모든 끝점들의 평균값을 계산
    Point averageEndpoint(0, 0);
    for (const Point& endpoint : lineEndpoints) {
	averageEndpoint.x += endpoint.x;
	averageEndpoint.y += endpoint.y;
    }
    averageEndpoint.x /= lineEndpoints.size();
    averageEndpoint.y /= lineEndpoints.size();

    // 평균값을 0부터 1280 범위로 조정
    int adjustedAverageX = std::max(0, std::min(1280, averageEndpoint.x));

    // adjustedAverageX 값을 배열에 저장하고 인덱스를 갱신
    averagedXCoordinates[currentIndex] = adjustedAverageX;
    currentIndex = (currentIndex + 1) % numFramesToAverage;

    // 배열에 값이 있는 경우 평균을 계산
    sumX = 0;
    validFrames = 0;
    for (int i = 0; i < numFramesToAverage; ++i) {
	if (averagedXCoordinates[i] != 0) {
	    sumX += averagedXCoordinates[i];
	    validFrames++;
	}
    }

    // 평균값 계산
    int averageX = 0;
    if (validFrames > 0) {
	averageX = sumX / validFrames;
    }

    // 오른쪽과 왼쪽 초록색 선의 x축 좌표 출력
    cout << "오른쪽 초록색 선의 x좌표 : " << rightGreenLine.start.x << endl;
    cout << "왼쪽 초록색 선의 x좌표 : " << leftGreenLine.start.x << endl;
    cout << "검출된 선들의 평균 x좌표 : " << averageEndpoint.x << endl;
    cout << "평균화된 x좌표 값 : " << adjustedAverageX << endl;

    // 평균화된 x값을 기준으로 우회전, 좌회전, 직진 결정
    if (adjustedAverageX == 0) {
	direction_state = 0; // 정지
    }  else if (adjustedAverageX < leftGreenLine.start.x || angle_direction == 2) {
	direction_state = 2; // 좌회전
    } else if (adjustedAverageX > rightGreenLine.start.x || angle_direction == 3) {
	direction_state = 3; // 우회전
    } else if (adjustedAverageX >= leftGreenLine.start.x && adjustedAverageX <= rightGreenLine.start.x) {
	direction_state = 1; // 직진
    }

    //검출된 선들의 평균 x좌표 음수일 경우
    if(averageEndpoint.x < 0){
	direction_state = 2; // 좌회전
    }
    // 연속된 0 값이 2회 이상 나왔을 경우 이전의 값을 사용 (차선 검출 실패 할 때를 대비하여 작성)
    if (direction_state == 0) {
	if (zeroCount >= 1) {
	    direction_state = previousDirectionState; // 이전 출력 유지
	    adjustedAverageX = previousNonZeroX;
	    zeroCount = 0;
	}
	zeroCount++;
    } else {
	previousNonZeroX = adjustedAverageX;
	previousDirectionState = direction_state; // 이전 출력 저장
	zeroCount++;
	//cout << "zeroCount : " << zeroCount << endl;
	//cout << "previousDirectionState : " << previousDirectionState << endl;
    }

    
    return direction_state;
}

#endif // LANE_DETECT_H
