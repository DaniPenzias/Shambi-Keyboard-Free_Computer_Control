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
	//functions used inside public functions
	static void RGBToGray(cv::Mat& frame);
public:
	// A structuring element is a small matrix used in morphological operations
	struct StructuringElement {
		int rows;
		int cols;
		std::vector<std::vector<int>> data;
	};

	//functions
	static void RGBToHSV(cv::Mat& frame);
	static void FindContours(cv::Mat& frame, std::vector<std::vector<cv::Point>>& contours);
	static void InRange(cv::Mat& frame, const cv::Scalar lower, const cv::Scalar upper);
	static void MorphologyEx(cv::Mat& frame, const StructuringElement& se, int operation);
	static StructuringElement GetStructuringElement(int rows, int cols);
};
