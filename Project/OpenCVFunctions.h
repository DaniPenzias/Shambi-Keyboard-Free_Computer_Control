#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

class OpenCVFunctions
{
private:
	static void RGBToGray(cv::Mat& frame);
public:
	static void RGBToHSV(cv::Mat& frame);
	static void FindContours(cv::Mat& frame, std::vector<std::vector<cv::Point>>& contours);
	static void InRange(cv::Mat& frame, const cv::Scalar lower, const cv::Scalar upper);
	static void MorphologyEx(const cv::Mat& frameIn, const cv::Mat& frameOut, int actionType, cv::Mat kernel);
};
