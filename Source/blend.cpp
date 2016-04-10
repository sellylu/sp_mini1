#include <iostream>
#include <opencv.hpp>
using namespace std;
using namespace cv;

const string input_path = "../TestImg/";
const string output_path = "../Result/";
const int KSIZE = 11;
const int DEV = 25;
const int LEVEL = 9;

vector<Mat> lapA, lapB;
Mat gauA, gauB;
Mat srcA, srcB;
uchar *pic_ptr;
 
int main() {
	
	srcA = imread(input_path + "apple.jpg");
	srcB = imread(input_path + "orange.jpg");
	if(srcA.empty() || srcB.empty()) {
		cout << "Open image failed." << endl;
		return 0;
	}
	
	gauA = srcA.clone();
	gauB = srcB.clone();
	for(int i = 0; i < LEVEL; i++) {
		Mat dst, tmp;
		GaussianBlur(gauA, dst, Size(KSIZE,KSIZE), DEV, DEV);
		resize(dst, tmp, Size(dst.cols/2, dst.rows/2));
		lapA.push_back(gauA-dst);
		gauA = tmp;
		dst.release();
		tmp.release();
		GaussianBlur(gauB, dst, Size(KSIZE,KSIZE), DEV, DEV);
		resize(dst, tmp, Size(dst.cols/2, dst.rows/2));
		lapB.push_back(gauB-dst);
		gauB = tmp;
	}
	
	Mat result, append, last;
	for(int k = 0; k < LEVEL+1; k++) {
		if(k <= 0) {
			result = gauA;
			append = gauB;
		} else {
			result = lapA.back();
			lapA.pop_back();
			append = lapB.back();
			lapB.pop_back();
		}
		for(int i = 0; i < result.rows; i++) {
			uchar *Aptr = result.ptr(i);
			uchar *Bptr = append.ptr(i);
			for(int j = 0; j < result.cols/2; j++) {
				Bptr[0] = Aptr[0];
				Bptr[1] = Aptr[1];
				Bptr[2] = Aptr[2];
				Aptr+=3;
				Bptr+=3;
			}
		}
		if(k > 0)
			result += last;
		last.release();
		if(k>=LEVEL)
			resize(result, last, Size(result.cols*2, result.rows*2));
		else
			resize(result, last, Size(lapA.back().cols, lapB.back().rows));
		imshow("result",  result);
		imshow("last", last);
		waitKey();
	}
	
	namedWindow("result", WINDOW_NORMAL);
	imshow("result", last);
	imwrite(output_path + "mutiblend5.png", result);
	waitKey();

	return 0;
}


