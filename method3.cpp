#include "opencv2/opencv.hpp"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <chrono>
#include <algorithm>
#include <vector>
#include <string>
#include <pthread.h>
#include <cstdlib>
#include <fstream>
using namespace std;
using namespace cv;
using namespace std::chrono;
int x ;											//no.of threads//
vector<int> queoriginal;
int quemodified[5737];
int arr[100];
struct thread_data {
	int id;
	Mat	im1;
	Mat im2;
};

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
void *queuedensity(void *thd)
{
	struct thread_data *my_data;
	my_data = (struct thread_data *) thd;
	Mat img1 = my_data->im1;
	Mat img	= my_data->im2;
	int k = my_data->id;
    int ar = 0;
	int row = k;
    while (row<780)
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
		row =row+x;
    }
	arr[k]=ar;
	pthread_exit(NULL);
	
}
void giveerror(int ar){
	double error=0.0;
	int h=1;
	for(int y=1;y<5737;y++){
		double mrate = double((quemodified[y]-quemodified[y-1])*15)/double(ar);
		double orate = double((queoriginal[y]-queoriginal[y-1])*15)/double(ar);
		error=error+abs(mrate-orate);
	}
	double utility = (error);
	cout<<"for x="<<x<<" utility is "<< utility<<endl;
}

int main(int argc,char *argv[]) {
    int n = stoi(argv[1]);
	x=n;
	//------------------------------//
	string line;
	ifstream fin("baseline.txt");
	while(getline(fin,line)){																//baseline values//
		if(line.size()>0){
			queoriginal.push_back(stoi(line));
		}
	}
	//------------------------------//
	int area = 280800;
	
	auto start = high_resolution_clock::now();
	
    Mat empty = convertimg(imread("requiredimage.jpg"));
	VideoCapture gap("trafficvideo.mp4");
	if (!gap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
	
	int i=0;
	pthread_t threads[x];
	struct thread_data td[x];
	while (1) {
        Mat frame;
        gap >> frame;
        if (frame.empty())
            break;
		Mat que = convertimg(frame);
		for(int r=0;r<x;r++){
			td[r].id = r;
			td[r].im1 = que;
			td[r].im2 = empty;
			pthread_create(&threads[r], NULL, queuedensity, (void *)&td[r]);
		}
		for(int r = 0;r<x;r++){
			pthread_join(threads[r],NULL);
		}
		int add=0;
		for(int j=0;j<x;j++){
			add=add+arr[j];
		}
		quemodified[i]=add;
		i=i+1;
				
    }
	
	//------------------------------//
	auto stop = high_resolution_clock::now();
	
	giveerror(area);
	
    gap.release();
    destroyAllWindows();
	
	auto duration = duration_cast<microseconds>(stop - start);
	cout<<"for x= "<<x<< " run time is "<<(duration.count()/1000000)<<" seconds"<<endl;
    return 0;
}
