#include <iostream>
#include <opencv.hpp>

using namespace std;
using namespace cv;

const char* left_win = "Left window";
const char* right_win = "Right window";
const char* result_win = "Result window";

vector<Point2f> left_image;
vector<Point2f> right_image;
Mat imageMain, imageApp;


void showResult() {
	
	Mat H = findHomography(right_image, left_image);	// find transform matrix
	int col, row;
	row = imageMain.rows;
	
	
	float x_after = 0;
	for(int i = 0; i < H.rows; i++) {
		uchar* imagePtr = H.ptr(i);
		
		
		if(i == 0) {
			x_after += float(*imagePtr) * imageApp.cols;
			imagePtr++;
			x_after += float(*imagePtr) * imageApp.rows/2;
			imagePtr++;
			x_after += float(*imagePtr) * 1;
		}
	}
	col = x_after;
	
	// TODO: 用H算右圖最右邊的點的橫坐標當col，imageMain的rows當row -> Size(col, row)
	
	Mat src2;
	warpPerspective(imageApp, src2, H, Size(imageMain.cols*2, row));		// do transformation
	Mat src1 = Mat(src2.size(), src2.type());
	
	for(int i = 0; i < imageMain.rows; i++) {
		uchar *mainPtr = imageMain.ptr(i);
		uchar *srcPtr = src1.ptr(i);
		for(int j = 0; j < imageMain.cols; j++) {
			srcPtr[0] = mainPtr[0];
			srcPtr[1] = mainPtr[1];
			srcPtr[2] = mainPtr[2];
			srcPtr+=3;
			mainPtr+=3;
		}
	}
 /*
	Mat gray, gray_inv, src1final, src2final;
	cvtColor(src2, gray, CV_BGR2GRAY);
	threshold(gray, gray, 0, 255, CV_THRESH_BINARY);
	bitwise_not(gray, gray_inv);
	
	//src1.copyTo(src1final,gray_inv);
	//src2.copyTo(src2final,gray);
	
	*/
	Mat result = src1 + src2;
	
	namedWindow(result_win, WINDOW_NORMAL);
	imshow(result_win, result);
}

void onMouseLeft(int event,int x,int y,int flags,void *ptr);
void onMouseRight(int event,int x,int y,int flags,void *ptr);

int main() {
	
	// Load images from arguments passed.
	imageMain = imread("../../../practice0/TestImg/30.jpg", CV_LOAD_IMAGE_COLOR);
	imageApp = imread("../../../practice0/TestImg/31.jpg", CV_LOAD_IMAGE_COLOR);
	if(imageMain.empty() || imageApp.empty()) {
		cout << "Open image failed." << endl;
		return 0;
	}
 
	namedWindow(left_win, WINDOW_NORMAL);
	imshow(left_win, imageMain);
	setMouseCallback(left_win, onMouseLeft);
	namedWindow(right_win, WINDOW_NORMAL);
	imshow(right_win, imageApp);
	setMouseCallback(right_win, onMouseRight);
 
	while(true) {
		if(waitKey() == 27)
			break;
		else if(waitKey() == 13)		//13 -> enter
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

