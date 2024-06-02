    #include <opencv2/opencv.hpp>
    #include <opencv2/imgproc.hpp>
    #include <opencv2/highgui.hpp>
    #include <opencv2/objdetect.hpp>
    #include <opencv2/imgcodecs.hpp>
    #include <iostream>
    #include "qrCode_C.h"

    using namespace cv;
    using namespace std;

    int qrCodedetect() {

        VideoCapture cap(0); // Open the default camera
        if (!cap.isOpened()) {  // Check if camera opened successfully
            cout << "Error: Couldn't open the camera" << endl;
        }

        QRCodeDetector qrDecoder;

        while (true) {
            Mat frame;
            cap >> frame;  // Capture frame from camera

            if (frame.empty()) {
                cout << "Error: Captured frame is empty" << endl;
                break;
            }
            cv::Mat brightenedImage;
            cv::cvtColor(frame, brightenedImage, cv::COLOR_BGR2GRAY);
            zq
            // Detect and decode QR code
            vector <Point> points;
            string qrCodeText = qrDecoder.detectAndDecode(brightenedImage, points);
            int num_qrCodeText=stoi(qrCodeText);
            cap.release();
            return num_qrCodeText;

        }


    }
