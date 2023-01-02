#include "OpenCVFunctions.h"
#include <algorithm>
#include <cmath>

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
