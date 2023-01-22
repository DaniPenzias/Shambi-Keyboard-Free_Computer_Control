#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <vector>
#include "WinApiFunctions.h"
#include "OpenCVFunctions.h"
#include <exception>

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
		Mat frame, kernel;
		std::vector<std::vector<Point> > contours;
		cap >> frame;

		//cvtColor(frame, frame, COLOR_BGR2HSV);
		OpenCVFunctions::RGBToHSV(frame);

		//inRange(frame, Scalar(45, 70, 0), Scalar(90, 200, 125), frame); //green
		//inRange(frame, Scalar(0, 50, 145), Scalar(75, 255, 200), frame); //orange
		//inRange(frame, Scalar(160, 150, 90), Scalar(180, 255, 255), frame); //pink
		//inRange(frame, Scalar(170, 110, 120), Scalar(255, 180, 180), frame); //red
		//inRange(frame, Scalar(160, 110, 110), Scalar(255, 210, 210), frame);
		OpenCVFunctions::InRange(frame, Scalar(160, 110, 110), Scalar(255, 210, 210));

		/*kernel = getStructuringElement(cv::MORPH_RECT, Size(12, 12));
		morphologyEx(frame, frame, cv::MORPH_CLOSE, kernel);
		kernel = getStructuringElement(cv::MORPH_RECT, Size(12, 12));
		morphologyEx(frame, frame, cv::MORPH_OPEN, kernel);*/
		//bitwise_and(frame, frame, frame, frame);

		// Create a structuring element with the desired size
		OpenCVFunctions::StructuringElement se = OpenCVFunctions::GetStructuringElement(BLOCK_SIZE, BLOCK_SIZE);
		// Apply the morphological close operation to the frame
		OpenCVFunctions::MorphologyEx(frame, se, CLOSE_MORPH);
		// Create a new structuring element with the same size
		se = OpenCVFunctions::GetStructuringElement(BLOCK_SIZE, BLOCK_SIZE);
		// Apply the morphological open operation to the frame
		OpenCVFunctions::MorphologyEx(frame, se, OPEN_MORPH);

		try
		{
			//findContours(frame, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
			OpenCVFunctions::FindContours(frame, contours);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		std::cout << contours.size() << std::endl;
		drawContours(frame, contours, -1, (255, 255, 255), 3);
		//destroyWindow("frame"); //possible to have no window
		imshow("frame", frame);

		if (contours.size() > 0)
		{
			//getting amount of objects
			int objects = contours.size();
				
			//getting x and y values (affecting only when mouse movement is dedected)
			int x = contours.begin()->begin()->x;
			int y = contours.begin()->begin()->y;
			const int middlePoint = int(contours.begin()->size() / 2 + 0.5);
			int x1 = contours.begin()->at(middlePoint).x;
			int y2 = contours.begin()->at(middlePoint).y;
			
			//switch case on amount of objects
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
