#include <opencv2/opencv.hpp>

int main() {
    // Open the default camera (camera index 0)
    cv::VideoCapture cap(1);

    // Check if the camera opened successfully
    if (!cap.isOpened()) {
        std::cout << "Error: Could not open camera" << std::endl;
        return -1;
    }

    // Create a window to display the video
    cv::namedWindow("Camera Feed", cv::WINDOW_NORMAL);

    while (true) {
        cv::Mat frame;
        // Capture a new frame from the camera
        cap >> frame;

        // Check if the frame is empty
        if (frame.empty()) {
            std::cout << "Error: Blank frame grabbed" << std::endl;
            break;
        }

        // Display the frame
        cv::imshow("Camera Feed", frame);

        // Break the loop if the user presses the 'q' key
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Release the camera
    cap.release();

    // Destroy all OpenCV windows
    cv::destroyAllWindows();

    return 0;
}
