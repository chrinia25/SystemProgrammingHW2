    #include <opencv2/opencv.hpp>
    #include <opencv2/imgproc.hpp>
    #include <opencv2/highgui.hpp>
    #include <opencv2/objdetect.hpp>
    #include <opencv2/imgcodecs.hpp>
    #include <iostream>

    using namespace cv;
    using namespace std;

    int main() {
        VideoCapture cap(0); // Open the default camera
        if (!cap.isOpened()) {  // Check if camera opened successfully
            cout << "Error: Couldn't open the camera" << endl;
            return -1;
        }

        // QR code detector
        QRCodeDetector qrDecoder;

        while (true) {
            Mat frame;
            cap >> frame;  // Capture frame from camera

            if (frame.empty()) {
                cout << "Error: Captured frame is empty" << endl;
                break;
            }
            cv::Mat brightenedImage;
            cv::cvtColor(frame, brightenedImage,cv::COLOR_BGR2GRAY);zq
            // Detect and decode QR code
            vector<Point> points;
            string qrCodeText = qrDecoder.detectAndDecode(brightenedImage, points);

            if (!qrCodeText.empty()) {
                cout << "Decoded QR Code: " << qrCodeText << endl;

                // Draw bounding box around the QR code
                for (int i = 0; i < points.size(); i++) {
                    line(brightenedImage, points[i % points.size()], points[(i + 1) % points.size()], Scalar(0, 0, 255), 2);
                }
            }

            // Display the frame
            imshow("Frame", brightenedImage);

            // Press 'q' to exit the loop
            if (waitKey(1) == 'q') {
                break;
            }
        }

        // Release the camera
        cap.release();
        destroyAllWindows();
        
        return 0;
    }
