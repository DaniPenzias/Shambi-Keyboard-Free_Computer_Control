#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <vector>
#include "WinApiFunctions.h"
#include "OpenCVFunctions.h"

#define MOUSE_MOVEMENT 1
#define LEFT_CLICK 2
#define RIGHT_CLICK 3
#define VIRTUAL_KEYBOARD 4

using namespace cv;
int main()
{
	VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;
	namedWindow("frame", 1);

	while (true)
	{
		Mat frame, mask, kernel;
		std::vector<std::vector<Point> > contours;
		cap >> frame;

		//imshow("frame", frame);

		//cvtColor(frame, frame, COLOR_BGR2HSV);
		OpenCVFunctions::RGBToHSV(frame);

		//inRange(frame, Scalar(45, 70, 0), Scalar(90, 200, 125), mask); //green
		//inRange(frame, Scalar(0, 50, 145), Scalar(75, 255, 200), mask); //orange
		//inRange(frame, Scalar(160, 150, 90), Scalar(180, 255, 255), mask); //pink
		//inRange(frame, Scalar(170, 110, 120), Scalar(255, 180, 180), mask); //red
		inRange(frame, Scalar(160, 110, 110), Scalar(255, 210, 210), mask);

		kernel = getStructuringElement(cv::MORPH_RECT, Size(12, 12));
		morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);
		kernel = getStructuringElement(cv::MORPH_RECT, Size(12, 12));
		morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
		//bitwise_and(frame, frame, frame, mask);

		findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		std::cout << contours.size() << std::endl;
		drawContours(frame, contours, -1, (255, 0, 0), 3);
		//imshow("frame", frame);
		destroyWindow("frame");
		imshow("mask", mask);

		if (contours.size() > 0)
		{
			//my variables
			int x = contours.begin()->begin()->x;
			int y = contours.begin()->begin()->y;
			int objects = contours.size();

			switch (objects)
			{
			case MOUSE_MOVEMENT:
				//converting x and y's values to use in movement and clicks
				WinApiFunctions::convertValues(x, y);
				WinApiFunctions::moveMouse();
				break;
			case LEFT_CLICK:
				WinApiFunctions::leftClick();
				break;
			case RIGHT_CLICK:
				WinApiFunctions::rightClick();
				break;
			case VIRTUAL_KEYBOARD:
				WinApiFunctions::openCloseVirtualKeyboard();
				break;
			default:
				std::cout << "Too mant objects" << std::endl;
			}
		}

		if (waitKey(300) == '\x1B')//Wait 100 milisec. If user pressed escape break the loop.
		{
			cap.release();
			break;
		}
	}

	return  0;
}
