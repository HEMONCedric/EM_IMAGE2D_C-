// TP6bis.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "image2D.h"
#include "point2D.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/ml.hpp"

using namespace cv;
using namespace cv::ml;

int main(int /*argc*/, char** /*argv*/){
    image2D image("D:/image1.bmp");
	//k est un multiple de 3
    image.esperance_max(6, "D:/image1.bmp");
    return 0;
}
