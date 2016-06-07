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

// Global variables 
Point_<double> P1(0,0); 
Point_<double> P2(0,0); 

// Declare functions 
void on_mouse(int, int, int, int,  void*) ;

int main(int argc, char** argv)
{
    int delay_time = 0 ;                                      // delay_time for get key pressed and configure fps. 
    int start = 0 ; 
    int Nparticles = 4000;                                    // number of particles
    double PositionSTD = 25 ; 
    double VelocitySTD = 5  ; 
    double TrueColorSTD = 50 ;  
    Mat TrueColor = (Mat_<uint8_t>(1,3) <<-0,0,255); // BGR order 

 
    Mat StateMatrix  = Mat::zeros(4,Nparticles,CV_64FC1);   // Initilize ramdom particles matrix 
    Mat LMatrix      = Mat::zeros(1,Nparticles,CV_64FC1);   // Initilize Likelihood Matrix 
    
    vector<String> filenames; 
    String folder = "/home/manh.huynh/Research/Caviar/EnterExitCrossingPaths1cor/JPEGS";
    //String folder = "/home/manh.huynh/Research/Red Person/" ;
    glob(folder, filenames); 
    namedWindow("DisplayVideo", WINDOW_AUTOSIZE );

    for(int i=30 ; i < filenames.size() ; i++) {
    	Mat image  = imread(filenames[i]);
	if(!image.data) {
        	printf("No image data \n");
        	return -1;
         } 
         imshow("DisplayVideo", image);

	if(delay_time==0){ 					// select person in very first frame 
	
		if(start == 0) { 
		   // Wait to select Bounding Box 
	           setMouseCallback("DisplayVideo",on_mouse,NULL);
                   waitKey(0); 
		  // Plot Bouding Box 
                  plot_rectangle(image,P1,P2);
	          //Generate Particles    
	          gaussian_generator(StateMatrix,P1,P2);
	          plot_particles(image,StateMatrix);
	          start = 1;
		  waitKey(0);  
		}
		// Update Particles 
		update_particles(StateMatrix, PositionSTD, VelocitySTD);
		
		//Calculate Likelyhood 
		calc_likelihood(LMatrix, StateMatrix, image, TrueColor, TrueColorSTD);

		//Resample Particles 
		resample_particle(LMatrix,StateMatrix,P1,P2) ;
               
                // Updating Bounding Box 
                plot_rectangle(image,P1,P2);                 
                 
	       //Re-generate particles 
               gaussian_generator(StateMatrix,P1,P2); 

               // Re-generate particles.
	        plot_particles(image,StateMatrix);  
		
      } //end if 
       	waitKey(0); 
     } // end for
    return 0;
}

/* ON_MOUSE - CALL BACK FUNC
 * INPUT: Current Frame 
 * OUTPUT: Coordinations of 2 corner of rectangle
 */

void on_mouse(int event, int x, int y, int d, void *){ 

switch(event){

        case  CV_EVENT_LBUTTONDOWN  :

                                        P1.x=x;
                                        P1.y=y;
                                        P2.x=x;
                                        P2.y=y;
                                        break;

        case  CV_EVENT_LBUTTONUP    :
                                        P2.x=x;
                                        P2.y=y;
                                        break;
        default                     :   break;


    }
}

/* GENERATE RANDOM GAUSSIAN DISTIBUTION NUMBERS 
 * input: 1xN_particles X1 matrix 
 *        1xN_particles X2 matrix 
 *        P1 and P2 : 2 points define a bounding box 
 * output: X1 and X2  */ 
/*  
void gaussian_generator(Mat &particles_mat, Point P1, Point P2){ 
  
	        Point mean = (P1+P2)/2 ;      
		unsigned int width = P2.x - P1.x ; 
		unsigned int heigth = P2.y - P1.y ; 
		
		cout << "width = " << width << endl; 	
                randn(particles_mat.row(0),mean.x,width/4);
		randn(particles_mat.row(1),mean.y,heigth/4); 
              //  cout << particles_mat << endl ;
 }

void plot_particles(Mat &image,Mat particles_mat){ 
	
        Point cur_point; 
	for(int j = 0 ; j < particles_mat.cols ; j++){
		cur_point = (Point)particles_mat.col(j); 
		circle(image,cur_point, 1, CV_RGB(255,0,0), 1) ; 
	} 

}*/ 
