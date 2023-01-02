#include "OpenCVFunctions.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>

// Convert RGB values to HSV values
// r, g, b: input RGB values (between 0 and 255)
// h, s, v: output HSV values (h: between 0 and 360, s and v: between 0 and 1)
void OpenCVFunctions::RGBToHSV(cv::Mat& frame)//int r, int g, int b, float& h, float& s, float& v
{
    // Convert the image from RGB to HSV color space
    for (int y = 0; y < frame.rows; y++)
    {
        for (int x = 0; x < frame.cols; x++)
        {
            // Get the pixel value at (x, y)
            cv::Vec3b pixel = frame.at<cv::Vec3b>(y, x); //maybe need to write after

            // Convert the pixel from RGB to HSV color space
            float r = pixel[0] / 255.0f;
            float g = pixel[1] / 255.0f;
            float b = pixel[2] / 255.0f;

            float h, s, v;
            float minVal = std::min(r, std::min(g, b));
            float maxVal = std::max(r, std::max(g, b));
            float delta = maxVal - minVal;

            v = maxVal;
            if (delta == 0)
            {
                h = 0;
                s = 0;
            }
            else
            {
                s = delta / maxVal;
                if (r == maxVal)
                {
                    h = (g - b) / delta;
                }
                else if (g == maxVal)
                {
                    h = 2 + (b - r) / delta;
                }
                else
                {
                    h = 4 + (r - g) / delta;
                }
                h *= 60;
                if (h < 0)
                {
                    h += 360;
                }
            }

            // Store the result in the frame
            frame.at<cv::Vec3b>(y, x) = cv::Vec3b(h, s * 255, v * 255);
        }
    }
}

void OpenCVFunctions::InRange(const cv::Mat& frameIn, const cv::Mat& frameOut, cv::Scalar lower, cv::Scalar upper)
{
    /*cv::Mat lowerbMat = lower.getMat(), upperbMat = upper.getMat();
    if (lowerbMat.type() != upperbMat.type())
    {
        std::cout << "Lower and upper bound arrays must have the same data type" << std::endl;
    }
    if (lowerbMat.cols != upperbMat.cols || lowerbMat.rows != upperbMat.rows)
    {
        std::cout << "Lower and upper bound arrays must have the same size" << std::endl;
    }
    
    frameIn.getMat().copyTo(frameOut);
    frameOut.setTo(0, frameIn < lowerbMat);
    frameOut.setTo(0, frameIn > upperbMat);*/
}

//struct Point
//{
//    int x;
//    int y;
//
//    Point(int x_, int y_) : x(x_), y(y_) {}
//};

// Find the external contours of a single-channel 8-bit image using CHAIN_APPROX_SIMPLE
// image: input image (single-channel 8-bit)
// contours: output vector of contours (each contour is a vector of points)
void OpenCVFunctions::FindContours(const cv::Mat& frame, std::vector<std::vector<cv::Point>>& contours)
{
    //// Initialize the output vector
    //contours.clear();

    //// Check if the image is empty
    //if (frame.empty())
    //{
    //    return;
    //}

    //// Check if the image is single-channel
    //if (frame.channels() != 1)
    //{
    //    throw std::invalid_argument("Image must be single-channel");
    //}

    //// Check if the image is 8-bit
    //if (frame.depth() != CV_8U)
    //{
    //    throw std::invalid_argument("Image must be 8-bit");
    //}

    //// Create a copy of the image
    //cv::Mat imageCopy = frame.clone();

    //// Threshold the image to create a binary image
    //cv::threshold(imageCopy, imageCopy, 128, 255, cv::THRESH_BINARY);

    //// Create a visited map to mark which pixels have been visited
    //cv::Mat visited(frame.rows, frame.cols, CV_8U, cv::Scalar(0));

    //// Find the external contours using a breadth-first search
    //for (int y = 0; y < frame.rows; y++)
    //{
    //    for (int x = 0; x < frame.cols; x++)
    //    {
    //        // Check if the pixel has not been visited and its value is non-zero
    //        if (visited.at<uchar>(y, x) == 0 && imageCopy.at<uchar>(y, x) != 0)
    //        {
    //            // Initialize the contour and the queue
    //            std::vector<Point> contour;
    //            std::queue<Point> queue;

    //            // Add the current pixel to the queue and mark it as visited
    //            queue.push(Point(x, y));
    //            visited.at<uchar>(y, x) = 1;

    //            // Perform a breadth-first search to find the contour
    //            while (!queue.empty())
    //            {
    //                // Get the next pixel in the queue
    //                Point point = queue.front();
    //                queue.pop();

    //                // Add the pixel to the contour
    //                contour.push_back(cv::Point(point.x, point.y));

    //                // Check the neighbors of the pixel
    //                for (int dy = -1; dy <= 1; dy++)
    //                {
    //                    for (int dx = -1; dx <= 1; dx++)
    //                    {
    //                        // Skip the pixel itself
    //                        if (dx == 0 && dy == 0)
    //                        {
    //                            continue;
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}
}

void OpenCVFunctions::MorphologyEx(const cv::Mat& frameIn, const cv::Mat& frameOut, int actionType, cv::Mat kernel)
{
}