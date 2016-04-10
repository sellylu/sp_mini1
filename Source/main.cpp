#include <iostream>
#include <opencv.hpp>
#include "opencv2/stitching.hpp"


using namespace std;
using namespace cv;

const char* left_win = "Left window";
const char* right_win = "Right window";

vector<Point2f> left_image;
vector<Point2f> right_image;
Mat imageMain, imageApp;


void showResult() {
	
	Mat H = findHomography(right_image, left_image);
	Mat src2;
	warpPerspective(imageApp, src2, H, imageMain.size());
 
	Mat gray,gray_inv,src1final,src2final;
	cvtColor(src2, gray, CV_BGR2GRAY);
	threshold(gray, gray, 0, 255, CV_THRESH_BINARY);
	//adaptiveThreshold(gray,gray,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,5,4);
	bitwise_not(gray, gray_inv);
	
	imageMain.copyTo(src1final,gray_inv);
	src2.copyTo(src2final,gray);
	Mat result = src1final + src2final;
	namedWindow("result", WINDOW_NORMAL);
	imshow("result", result);
}

void onMouseLeft(int event,int x,int y,int flags,void *ptr);
void onMouseRight(int event,int x,int y,int flags,void *ptr);

int main() {
	
	// Load images from arguments passed.
	imageMain = imread("../TestImg/30.jpg", CV_LOAD_IMAGE_COLOR);
	imageApp = imread("../TestImg/31.jpg", CV_LOAD_IMAGE_COLOR);
 
	namedWindow(left_win, WINDOW_NORMAL);
	imshow(left_win, imageMain);
	setMouseCallback(left_win, onMouseLeft);
	namedWindow(right_win, WINDOW_NORMAL);
	imshow(right_win, imageApp);
	setMouseCallback(right_win, onMouseRight);
 
	while(true) {
		if(waitKey() == 27)
			break;
		else if(waitKey() == 13)
			showResult();
	}
 
	return 0;
}

void onMouseLeft(int event, int x, int y, int flags, void *ptr) {
	if(left_image.size() < 4) {
		if (event == EVENT_LBUTTONDOWN) {
			left_image.push_back(Point2f(x, y));
			cout << x << " " << y <<endl;
		}
		if(flags == EVENT_FLAG_LBUTTON) {
			Point(x, y);
			circle(imageMain, Point(x,y), 10, Scalar(0,255,0), -1);
			imshow(left_win, imageMain);
		}
	}
}
void onMouseRight(int event, int x, int y, int flags, void *ptr) {
	if(right_image.size() < 4) {
		if (event == EVENT_LBUTTONDOWN ) {
			right_image.push_back(Point2f(x, y));
			cout << x << " " << y <<endl;
		}
		if(flags == EVENT_FLAG_LBUTTON) {
			Point(x, y);
			circle(imageApp, Point(x,y), 10, Scalar(0,255,0), -1);
			imshow(right_win, imageApp);
		}
	}
}
