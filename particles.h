#ifndef PARTICLES_H
#define PARTICLES_H 

using namespace cv;
using namespace std;


void gaussian_generator(Mat &, Point , Point);

void plot_particles(Mat image,Mat StateMatrix);

void plot_rectangle(Mat image, Point_<double> P1, Point_<double> P2); 

void update_particles(Mat &StateMatrix, double PositionSTD, double VelocitySTD);

void calc_likelihood(Mat &LMatrix, Mat StateMatrix, Mat image, Mat TrueColor, double TrueColorSTD); 

void resample_particle(Mat LMatrix,Mat &StateMatrix, Point_<double> &P1, Point_<double> &P2) ; 
#endif 
