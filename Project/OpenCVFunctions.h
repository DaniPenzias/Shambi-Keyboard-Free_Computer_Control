#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

class OpenCVFunctions
{
public:
	static void RGBToHSV(cv::Mat& frame);
};
