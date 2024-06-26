    #include <opencv2/opencv.hpp>
    #include <opencv2/imgproc.hpp>
    #include <opencv2/highgui.hpp>
    #include <opencv2/objdetect.hpp>
    #include <opencv2/imgcodecs.hpp>
    #include <iostream>
    #include "qrCode_C.h"
    
    using namespace cv;
    using namespace std;

    int read_QR(){
        VideoCapture cap(0); // Open the default camera
        if (!cap.isOpened()) {  // Check if camera opened successfully
            cout << "Error: Couldn't open the camera" << endl;
            VideoCapture temp_cap(0);
            cap = temp_cap;
        }

        QRCodeDetector qrDecoder;

            Mat frame;
            cap >> frame;  // Capture frame from camera

            if (frame.empty()) {
                cout << "Error: Captured frame is empty" << endl;
                return -1;  
            }
            cv::Mat brightenedImage;
            cv::cvtColor(frame, brightenedImage, cv::COLOR_BGR2GRAY);
            frame += cv::Scalar(50, 50, 50);
            // Detect and decode QR code
            vector <Point> points;
            string qrCodeText = qrDecoder.detectAndDecode(brightenedImage, points);

            if (qrCodeText.size() > 0){
                int num_qrCodeText = stoi(qrCodeText);
                return num_qrCodeText;
            }

            else
            {
                return -1;
            }

    }
