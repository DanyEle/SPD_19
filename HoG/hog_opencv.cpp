//Daniele Gadler
//University of Pisa
//Department of Computer Science
//Course: Programming Tools for Parallel and Distributed Systems
//Year: 2019
//Inspired from the guide at: https://www.learnopencv.com/histogram-of-oriented-gradients/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;


 
int main( int argc, char** argv ) {
  
  cv::Mat inputImage;
  //Requirement: input image has aspect ratio 1:2
  inputImage = cv::imread("bolt.jpg" , CV_LOAD_IMAGE_COLOR);
  cv::Mat imageResized;

  //1. step: resize the input image
  Size size(64,128);
  cv::resize(inputImage, imageResized, size);

  //2. step: gradient calculation for the x and y axes
  cv::Mat imageReColored;
  imageResized.convertTo(imageReColored, CV_32F, 1/255.0);

  //3. step: Apply the sobel filter to compute gradients over the X and Y axes
  //The gradient on the X and Y axes removed 'smooth' areas of the image, where
  //no abrupt change is present
  cv::Mat imgGradX, imgGradY;
  cv::Sobel(imageReColored, imgGradX, CV_32F, 1, 0, 1);
  cv::Sobel(imageReColored, imgGradY, CV_32F, 1, 0, 1);

  //4. step: Find the magnitude and direction of the gradient
  //The magnitude of gradient at a pixel is the maximum of the
  //magnitude of gradients of the three channels, and the
  //angle is the angle corresponding to the maximum gradient.
  cv::Mat imgMagnitude, imgAngle;
  cv::cartToPolar(imgGradY, imgGradY, imgMagnitude, imgAngle, 1);

  //5. Step: Compute the Histogram of Oriented Gradients
  //Need to divide the image into 8x8 cells to capture interesting features
  //in the input image (ex: face, top of head, etc..)

  for (int y = 0 ; y < imgAngle.rows ; y++)
  {

      for (int x = 0 ;x < imgAngle.cols ; x++)
      {
    	  if(y % 8 == 0 && x % 8 == 0)
    	  {
    		  std:: cout << imgAngle.at<Vec3b>(x,y);
    	  }
      }
  }



  //now process the input image
  if(! inputImage.data ) {
      std::cout <<  "Could not open or find the image" << std::endl ;
      return -1;
    }
  
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  //cv::imshow( "Display window", imgMagnitude);
  //cv::imshow( "Display window", imgAngle);

  
  //cv::waitKey(0);
  return 0;
}
