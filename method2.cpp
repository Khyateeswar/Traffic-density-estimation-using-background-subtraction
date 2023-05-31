#include "opencv2/opencv.hpp"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <chrono>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
using namespace cv;
using namespace std::chrono;
int X;															//width
int Y;															//height
vector<int> queoriginal;
vector<int> quemodified;
Mat convertimg(Mat input_img)
{
    Mat gray_img;
    cvtColor(input_img, gray_img, COLOR_BGR2GRAY);

    vector<Point2f> inputpts;
    inputpts.push_back(Point2f(976, 232));
    inputpts.push_back(Point2f(352, 976));
    inputpts.push_back(Point2f(1520, 1056));
    inputpts.push_back(Point2f(1264, 224));

    vector<Point2f> reqplanepts;
    reqplanepts.push_back(Point2f(472, 52));
    reqplanepts.push_back(Point2f(472, 832));
    reqplanepts.push_back(Point2f(800, 832));
    reqplanepts.push_back(Point2f(800, 52));

    Mat H = findHomography(inputpts, reqplanepts);
    Mat wrap_img;
    warpPerspective(gray_img, wrap_img, H, gray_img.size());
    Mat imag_fin;
    Rect crop_region(440, 52, 360, 780);
    imag_fin = wrap_img(crop_region);
	
	int wi = (360*X)/1920;
	int he = (780*Y)/1080;
	
	Size size(wi, he);
	resize(imag_fin,imag_fin,size);
    return imag_fin;
}

int queuedensity(Mat img1, Mat img)
{
    int ar = 0;
	int ab = (360*X/1920);
	int cd = (780*Y/1080);
	
    for (int row = 0; row <cd; row++)
    {
        for (int col = 0; col <ab; col++)
        {
            int a = static_cast<int>(img1.at<uchar>(row, col));
            int b = static_cast<int>(img.at<uchar>(row, col));
            if (abs(a - b) > 8) {
                ar = ar + 1;
            }
            else {
                ar = ar;
            }
        }
    }
	
    return ar;
}
void giveerror(int ar){
	
	double error=0.0;
	
	int wil = (360*X)/1920;
	int hel = (780*Y)/1080;
	int arl = wil*hel;
	
	for(int y=1;y<5737;y++){
		int qorg =(queoriginal[y]-queoriginal[y-1]);
		int qmod =(quemodified[y]-quemodified[y-1]);
		double orate = (double)(qorg*15)/(double)(ar);
		double mrate = (double)((qmod*15))/(double)(arl);
		error=error+abs(mrate-orate);
	}
	double utility = (5737/error);
	cout<<"for "<<"("<<X<<","<<Y<<")"<<" utility is "<<utility<<endl;
    return;
}

int main(int argc,char *argv[]) {
	X=stoi(argv[1]);
	Y=stoi(argv[2]);
	
    int area = 280800;
	
	ifstream fin("baseline.txt");
    string line;
	while(getline(fin,line)){
		if(line.size()>0){
			queoriginal.push_back(stoi(line));
		}
	}
	
	auto start = high_resolution_clock::now();
	
	
	Mat empty = convertimg(imread("requiredimage.jpg"));
	VideoCapture gap("trafficvideo.mp4");
	if (!gap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
	while (1) {
        Mat frame;
        gap >> frame;
        if (frame.empty())
            break;
		Mat que = convertimg(frame);
        quemodified.push_back(queuedensity(que, empty));
    }
	
	
	giveerror(area);
	auto stop = high_resolution_clock::now();
	
    gap.release();
    destroyAllWindows();
	
	auto duration = duration_cast<microseconds>(stop - start);
	
	cout<<"for "<<"("<<X<<","<<Y<<")"<< " run time is "<<(duration.count()/1000000)<<" seconds"<<endl;
    return 0;
}
