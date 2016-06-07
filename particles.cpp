#include <stdio.h>
#include <unistd.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include "particles.h"

using namespace cv;
using namespace std;


/* Function: GENERAGE 2-D  RAMDOM GAUSSIAN NUMBER 
 * Input:  P1 and P2 : 2 points define a bounding box 
 * Output: Position State (X,Y) in StateMatrix will be generated      */

void gaussian_generator(Mat &StateMatrix, Point P1, Point P2){

                Point mean = (P1+P2)/2 ;
                unsigned int width = P2.x - P1.x ;
                unsigned int height = P2.y - P1.y ;

                cout << "width = " << width << endl;
                randn(StateMatrix.row(0),mean.x,width/4);
                randn(StateMatrix.row(1),mean.y,height/4); 
                //cout<<StateMatrix.row(1) << endl ; 
} 
/* FUNC: PLOT PARTICLES
 * Input: StateMatrix consists of Position of particles in col 1 and 2
 * Output: image with plotted particles.  */ 

 void plot_particles(Mat image,Mat StateMatrix){
               Point_<double> cur_point;
               for(int j = 0 ; j < StateMatrix.cols ; j++){
                      cur_point.x = StateMatrix.at<double>(0,j); 
		      cur_point.y = StateMatrix.at<double>(1,j);  
//		      cout << cur_point << endl ;
		      circle(image,cur_point, 1, CV_RGB(255,0,0), 1) ;
               }
   imshow("DisplayVideo",image);

} 

void plot_rectangle(Mat image, Point_<double> P1, Point_<double> P2){

   rectangle(image,P1,P2,Scalar(0,0,255), 1,8,0) ;
   imshow("DisplayVideo",image);


}


void update_particles(Mat &StateMatrix, double PositionSTD, double VelocitySTD){ 

   Mat TempMatrix = Mat::zeros(1,StateMatrix.cols,CV_64FC1); 
   Mat UpdateMatrix = (Mat_<double>(4,4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1); 
  // cout << UpdateMatrix << endl;
   //update State Matrix 
   StateMatrix = UpdateMatrix*StateMatrix;     
   //Adding some noises to State Matrix 
   randn(TempMatrix,0,1) ;
   StateMatrix.row(0) = StateMatrix.row(0) + PositionSTD*TempMatrix;
   StateMatrix.row(1) = StateMatrix.row(1) + PositionSTD*TempMatrix;
   StateMatrix.row(2) = StateMatrix.row(2) + VelocitySTD*TempMatrix; 
   StateMatrix.row(3) = StateMatrix.row(3) + VelocitySTD*TempMatrix; 
 
}

void calc_likelihood(Mat &LMatrix, Mat StateMatrix, Mat image, Mat TrueColor, double TrueColorSTD){ 

double  m = 0 ; 
double n = 0 ;
Mat dis = Mat::zeros(1,3,CV_64FC1) ; 
double dis2 = 0 ; 
for(int k = 0; k < StateMatrix.cols ; k++){             // process for every particle

  m = round(StateMatrix.at<double>(0,k)); 
  n = round(StateMatrix.at<double>(1,k)); 
  //cout << m << " " << n << endl ; 
  
  if(m >= 0 && m <= image.cols && n >=0 && n <= image.rows){ 
    
    Vec3b intensity = image.at<Vec3b>(n,m) ; // BGR order  
    dis2 = pow(intensity.val[0] - TrueColor.at<uint8_t>(0),2)  
              + pow(intensity.val[1] - TrueColor.at<uint8_t>(1),2) 
              + pow(intensity.val[2] - TrueColor.at<uint8_t>(2),2) ;     
    //  cout << dis2 << endl; 
   // cout << "("<< n << "," << m << ")"<<  intensity <<  endl ;
     LMatrix.at<double>(k) = 1/sqrt(2*CV_PI*TrueColorSTD)*exp(-dis2/(2*pow(TrueColorSTD,2))) ;
    
 //     cout << LMatrix.at<double>(k) << endl ;  
 }else{ 
      LMatrix.at<double>(k) = 0 ;  
  } 

} 
 double Lsum = sum(LMatrix)[0] ; 
 // cout << Lsum << endl ;
  LMatrix = LMatrix*(1/Lsum) ;  
  
  cout << sum(LMatrix)[0] << endl ;
} 


void resample_particle(Mat LMatrix,Mat &StateMatrix, Point_<double> &P1, Point_<double> &P2){ 

double rec_width = P2.x - P1.x ; 
double rec_height = P2.y - P1.y ; 
Point_<double> rec_center ; 
rec_center.x =  round(sum(StateMatrix.row(0).mul(LMatrix))[0]); 
rec_center.y =  round(sum(StateMatrix.row(1).mul(LMatrix))[0]); 
cout << rec_center << endl; 
// updating P1 and P2. 
P1.x = round(rec_center.x - rec_width/2) ;
P1.y = round(rec_center.y - rec_height/2) ; 
P2.x = round(rec_center.x + rec_width/2) ; 
P2.y = round(rec_center.y + rec_height/2) ;  
 

}  
