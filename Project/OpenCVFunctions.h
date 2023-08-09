#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#define BLOCK_SIZE 6 //change based on window size
#define OPEN_MORPH 2
#define CLOSE_MORPH 3

class OpenCVFunctions
{
public:
	// A structuring element is a small matrix used in morphological operations
	struct StructuringElement {
		int rows = 0;
		int cols = 0;
		std::vector<std::vector<int>> data;
	};

	//functions
	static void RGBToHSV(cv::Mat& frame);
	static void FindContours(cv::Mat& frame, std::vector<std::vector<cv::Point>>& contours);
	static void InRange(cv::Mat& frame, const cv::Scalar lower, const cv::Scalar upper);
	static void MorphologyEx(cv::Mat& frame, const StructuringElement& se, int operation);
	static StructuringElement GetStructuringElement(int rows, int cols);
private:
	//functions used inside public functions
	static void RGBToGray(cv::Mat& frame);
	static void dilate(const cv::Mat& input, cv::Mat& output, const StructuringElement& se);
	static void erode(const cv::Mat& input, cv::Mat& output, const StructuringElement& se);
};
