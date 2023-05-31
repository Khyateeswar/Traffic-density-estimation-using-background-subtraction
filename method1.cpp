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
int x ;																		// skip number
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
    return imag_fin;
}

int queuedensity(Mat img1, Mat img)
{
    int ar = 0;
    for (int row = 0; row < 780; row++)
    {
        for (int col = 0; col <360; col++)
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
	
	double mrate=0;
	
	for(int y=1;y<5737;y++){
        int qdo = (queoriginal[y]-queoriginal[y-1]);
        double orate = ((double)(qdo*15))/((double)(ar));
        if((y%x==1) && (x!=1)){
            int qdm = (quemodified[(y/x)+1]-quemodified[(y/x)]);
            mrate = ((double)(qdm*15))/((double)(x*ar));
        }
        if(x==1){
            mrate=orate;
        }
		error=error+abs(mrate-orate);
	}
	error = error/5737;
	double utility = 1/error;
	cout<<"for "<<"x="<<x<<"utility is "<< utility <<endl;
}


int main(int argc,char *argv[]) {
    x= stoi(argv[1]);
	
    string mytext;
    ifstream mfile("baseline.txt");
    while(getline(mfile,mytext)){
        queoriginal.push_back(stoi(mytext));
    }
	
    int i = 0;
    int area = 280800;
    Mat empty = convertimg(imread("requiredimage.jpg"));
	
	auto start = high_resolution_clock::now();
	
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
		if(i%x==0){
			Mat que = convertimg(frame);
            quemodified.push_back(queuedensity(que, empty));
        }
    }
	giveerror(area);
    auto stop = high_resolution_clock::now();

    gap.release();
    destroyAllWindows();
	
	auto duration = duration_cast<microseconds>(stop - start);
	
	cout<<"for the "<<"x="<<x<< " run time is "<<(duration.count()/1000000)<<" seconds"<<endl;
    return 0;
}
