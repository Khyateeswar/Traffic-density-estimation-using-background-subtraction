#include "opencv2/opencv.hpp"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
using namespace cv;
using namespace std::chrono;
vector<int> queoriginal;
Mat convertimg(Mat input_img)
{
    Mat gray_img;
    cvtColor(input_img, gray_img, COLOR_BGR2GRAY);
    //Size size(480, 270);
    //resize(gray_img, gray_img, size);

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

int main() {
    auto start = high_resolution_clock::now();
    VideoCapture cap("trafficvideo.mp4");
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    int area = 280800;
    Mat empty = convertimg(imread("requiredimage.jpg"));
    while (1) {
        Mat frame;
        cap >> frame;
        if (frame.empty())
            break;
		Mat que = convertimg(frame);
		queoriginal.push_back(queuedensity(que,empty));
    }
	ofstream myfile("baseline.txt");
	for(int i=0;i<queoriginal.size();i++){
		myfile<<to_string(queoriginal[i])+"\n";
	}
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
	cout<<"for the "<< " run time is "<<(duration.count()/1000000)<<" seconds"<<endl;
    return 0;
}
