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

// Find the external contours of a single-channel 8-bit image using CHAIN_APPROX_SIMPLE
// image: input image (single-channel 8-bit)
// contours: output vector of contours (each contour is a vector of points)
void OpenCVFunctions::FindContours(cv::Mat& frame, std::vector<std::vector<cv::Point>>& contours)
{
    // The width and height of the image
    int width = frame.cols;
    int height = frame.rows;

    // We'll use a simple breadth-first search to find all the contours
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::vector<cv::Point> neighbors{ {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Skip pixels that have already been visited or are not part of a contour
            if (visited[y][x] || frame.at<uchar>(y, x) == 0) continue;

            // This pixel is the starting point of a new contour
            std::vector<cv::Point> contour;
            std::vector<cv::Point> search_queue = { {x, y} };
            visited[y][x] = true;

            while (!search_queue.empty()) {
                cv::Point p = search_queue.back();
                search_queue.pop_back();
                contour.push_back(p);

                // Add all the unvisited neighbors of p to the search queue
                for (cv::Point n : neighbors) {
                    int nx = p.x + n.x;
                    int ny = p.y + n.y;
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height && !visited[ny][nx] && frame.at<uchar>(ny, nx) != 0) {
                        search_queue.push_back({ nx, ny });
                        visited[ny][nx] = true;
                    }
                }
            }

            // We've found a complete contour, so we add it to the output vector
            contours.push_back(contour);
        }
    }
}

/*function handles sturcturing elment which is used than in MorphologyEx function*/
OpenCVFunctions::StructuringElement OpenCVFunctions::GetStructuringElement(int rows, int cols)
{
    StructuringElement se;
    se.rows = rows;
    se.cols = cols;
    se.data.resize(rows, std::vector<int>(cols, 1));
    return se;
}

/**/
void OpenCVFunctions::MorphologyEx(cv::Mat& frame, const StructuringElement& se, int operation)
{
    // The width and height of the image
    int width = frame.cols;
    int height = frame.rows;

    std::vector<std::vector<int>> output(height, std::vector<int>(width));

    if (operation == cv::MORPH_CLOSE) {
        // Perform dilation followed by erosion
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Find the maximum value in the neighborhood defined by the structuring element
                int max_val = 0;
                for (int i = 0; i < se.rows; i++) {
                    for (int j = 0; j < se.cols; j++) {
                        int ny = y + i - se.rows / 2;
                        int nx = x + j - se.cols / 2;
                        if (ny >= 0 && ny < height && nx >= 0 && nx < width && se.data[i][j] != 0) {
                            max_val = std::max(max_val, int(frame.at<uchar>(ny, nx)));
                        }
                    }
                }
                output[y][x] = max_val;
            }
        }

        // Perform erosion
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Find the minimum value in the neighborhood defined by the structuring element
                int min_val = std::numeric_limits<int>::max();
                for (int i = 0; i < se.rows; i++) {
                    for (int j = 0; j < se.cols; j++) {
                        int ny = y + i - se.rows / 2;
                        int nx = x + j - se.cols / 2;
                        if (ny >= 0 && ny < height && nx >= 0 && nx < width && se.data[i][j] != 0) {
                            min_val = std::min(min_val, output[ny][nx]);
                        }
                    }
                }
                frame.at<uchar>(y, x) = min_val;
            }
        }
    }
    else if (operation == cv::MORPH_OPEN) {
        // Perform erosion followed by dilation
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Find the minimum value in the neighborhood defined by the structuring element
                int min_val = std::numeric_limits<int>::max();
                for (int i = 0; i < se.rows; i++) {
                    for (int j = 0; j < se.cols; j++) {
                        int ny = y + i - se.rows / 2;
                        int nx = x + j - se.cols / 2;
                        if (ny >= 0 && ny < height && nx >= 0 && nx < width && se.data[i][j] != 0) {
                            min_val = std::min(min_val, int(frame.at<uchar>(ny, nx)));
                        }
                    }
                }
                output[y][x] = min_val;
            }
        }

        // Perform dilation
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                // Find the maximum value in the neighborhood defined by the structuring element
                int max_val = 0;
                for (int i = 0; i < se.rows; i++) {
                    for (int j = 0; j < se.cols; j++) {
                        int ny = y + i - se.rows / 2;
                        int nx = x + j - se.cols / 2;
                        if (ny >= 0 && ny < height && nx >= 0 && nx < width && se.data[i][j] != 0) {
                            max_val = std::max(max_val, output[ny][nx]);
                        }
                    }
                }
                frame.at<uchar>(y, x) = max_val;
            }
        }
    }
}
