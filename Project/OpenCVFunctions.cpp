#include "OpenCVFunctions.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>

/*function converts RGB values to HSV values on frame*/
void OpenCVFunctions::RGBToHSV(cv::Mat& frame)
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

/*function converts frame with RGB values to GrayScale values*/
void OpenCVFunctions::RGBToGray(cv::Mat& frame)
{
    // Check if the input image is empty
    if (frame.empty())
    {
        std::cout << "Error: input image is empty!" << std::endl;
        return;
    }

    // Check if the input image is a 3-channel BGR image
    if (frame.type() != CV_8UC3)
    {
        std::cout << "Error: input image is not a 3-channel BGR image!" << std::endl;
        return;
    }

    // Create an output image with the same size and type as the input image
    cv::Mat output = cv::Mat::zeros(frame.size(), CV_8UC1);  // <-- changed the type to CV_8UC1

    // Loop through each pixel in the input image
    for (int y = 0; y < frame.rows; y++)
    {
        for (int x = 0; x < frame.cols; x++)
        {
            // Get the pixel value at (x, y)
            cv::Vec3b pixel = frame.at<cv::Vec3b>(y, x);

            // Convert the pixel to grayscale using the luminosity formula:
            // gray = 0.299*red + 0.587*green + 0.114*blue
            uint8_t gray = static_cast<uint8_t>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);

            // Set the output pixel value to the grayscale value
            uint8_t* output_ptr = output.ptr<uint8_t>(y, x);  // <-- get a pointer to the output pixel
            *output_ptr = gray;  // <-- dereference the pointer to set the pixel value
        }
    }

    // Replace the input image with the output image
    frame = output;
}

/*function leaves only the pixels in which the colors values are inside the bordares which we gave*/
void OpenCVFunctions::InRange(cv::Mat& frame, cv::Scalar lower, cv::Scalar upper)
{   
    if (frame.empty())
    {
        std::cout << "The input image is empty!" << std::endl;
    }

    // Filter the frame
    for (int y = 0; y < frame.rows; y++)
    {
        for (int x = 0; x < frame.cols; x++)
        {
            // Get the pixel value
            cv::Vec3b pixel = frame.at<cv::Vec3b>(y, x);

            // Check if the pixel is within the limits
            if (pixel[0] >= lower[0] && pixel[0] <= upper[0] &&
                pixel[1] >= lower[1] && pixel[1] <= upper[1] &&
                pixel[2] >= lower[2] && pixel[2] <= upper[2])
            {
                // Set the output pixel value to 255(white)
                frame.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
            }
            else
            {
                // Set the output pixel value to 0(black)
                frame.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
            }
        }
    }

    //converting to the right type so it won't cause errors after
    RGBToGray(frame);
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