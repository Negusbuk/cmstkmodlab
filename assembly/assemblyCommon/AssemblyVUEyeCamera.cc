#include <iostream>

#include <QApplication>
#include <QString>

#include <QImage>
#include <QLabel>
#include <QPainter>

#include <nqlogger.h>

#include "AssemblyVUEyeCamera.h"
#include <iostream>
#include <cmath>

#include <ApplicationConfig.h>


//motion
#include "LStepExpressModel.h"
#include "LStepExpressSettings.h"
#include "LStepExpressMotionManager.h"
#include "LStepExpressMotionThread.h"
#include "LStepExpressSettingsWidget.h"
#include "LStepExpressWidget.h"
#include "LStepExpressJoystickWidget.h"



#include <unistd.h> 
#include <ueye.h>
#include <uEye.h>
//#include <uEye_tools.h>
#include <ueye_deprecated.h>


using namespace std;

AssemblyVUEyeCamera::AssemblyVUEyeCamera(QObject *parent)
    : QObject(parent),
      cameraState_(State::OFF)
{

}

AssemblyVUEyeCamera::~AssemblyVUEyeCamera()
{

}

size_t AssemblyVUEyeCamera::getCurrentPixelClockIndex() const
{
    size_t idx = 0;
    for (std::vector<unsigned int>::const_iterator it = pixelClocks_.begin();
         it != pixelClocks_.end();
         ++it) {
        if (*it == currentPixelClock_) {
            return idx;
        }
        idx++;
    }
    return idx;
}



void AssemblyVUEyeCamera::pickup()
{
    NQLog("AssemblyVUEyeCamera") << "Running Pickup Routine" ;
    emit updateStatus("Moving to pickup area",20.0);
    NQLog("AssemblyVUEyeCamera") << ": going to pickup position ";

    ApplicationConfig* config = ApplicationConfig::instance();
    LStepExpressModel* lStepExpressModel_ = new LStepExpressModel(config->getValue<std::string>("LStepExpressDevice").c_str(),1000, 100);
    LStepExpressSettings* lStepExpressSettings_ = new LStepExpressSettings(lStepExpressModel_);
    LStepExpressMotionManager* motionManager_ = new LStepExpressMotionManager(lStepExpressModel_);
    connect(this, SIGNAL(moveAbsolute(double,double,double,double)), motionManager_, SLOT(moveAbsolute(double,double,double,double)));

    lStepExpressModel_->initialize(); 
      emit moveAbsolute(0.0,0.0,0.0,0.0);
    //emit moveAbsolute(175.96,0.0,0.0,0.0);
    // emit moveAbsolute(175.96,0.0,87.4,0.0);
    //emit moveAbsolute(175.96,140.00,87.4,0.0);



    //emit moveAbsolute(175.96,140.00,87.4,0.0);



    
}




void AssemblyVUEyeCamera::calibrateSettings()
{
    NQLog("AssemblyVUEyeCamera") << "Finding Marker (Circle Seed Algorithm)" ;

   loopFind_ = new AssemblySensorMarkerFinder();

   HIDS hCam = getCameraID();
   char* imgMem;
   int memId;

      if(is_InitCamera (&hCam, NULL)!= IS_SUCCESS){
      
   NQLog("AssemblyVUEyeCamera") << "CANT INIT CAMERA";

     }

   NQLog("AssemblyVUEyeCamera") << " INIT CAMERA";

    int img_width=2650, img_height=1920, img_bpp=24, img_step, img_data_size;
     is_AllocImageMem(hCam, img_width, img_height, img_bpp, &imgMem, &memId);
     is_SetImageMem (hCam, imgMem, memId);
     is_SetDisplayMode (hCam, IS_SET_DM_DIB);
     is_SetColorMode (hCam, IS_CM_RGB8_PACKED);
     is_SetImageSize (hCam, img_width, img_height);
 

     double enable = 1;
     double disable = 0;
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_GAIN, &enable, 0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_WHITEBALANCE, &enable, 0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_FRAMERATE, &disable, 0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SHUTTER, &disable, 0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &enable, 0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_WHITEBALANCE,&enable,0);
     is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SENSOR_SHUTTER, &disable, 0);

     double FPS,NEWFPS;
     FPS = 15;
     double fps;
     is_SetFrameRate(hCam,FPS,&NEWFPS);

     UINT uiCaps = 0;
 
     INT nRet = is_Focus (hCam, FDT_CMD_GET_CAPABILITIES, &uiCaps, sizeof (uiCaps) );
 
     if (nRet == IS_SUCCESS && (uiCaps & FOC_CAP_AUTOFOCUS_SUPPORTED))
     {
       NQLog("AssemblyVUEyeCamera") << "if supported, enable auto focus";

       //qDebug() << "If supported, enable auto us";
     nRet = is_Focus (hCam, FOC_CMD_SET_DISABLE_AUTOFOCUS, NULL, 0);
     }
    NQLog("AssemblyVUEyeCamera") << "  set focus";

     nRet = is_SetGamma(hCam,600);
     enable = 2;
     char filename[512];


    cv::Mat img_gs, img_rgb, img_edges;
    int radius;
    int  expectedCircleRadius_ = 89;
    double circleCenterDetectionThreshold_  = 35.0;
    double circleEdgeDetectionThreshold_  = 35.0;
    
    double linesCannyEdgeDetectionThreshold1_ =50;
    double linesCannyEdgeDetectionThreshold2_ =200;
    double linesCannyEdgeDetectionApertureSize_ =3;
    double linesCannyEdgeDetectionL2Gradient_ =1;
    double linesHoughDistanceResolution_ = 1.0;
    //    double linesHoughAngleResolution_*CV_PI/180,
    double linesHoughAngleResolution_ = 0.1;
    double linesHoughThreshold_ = 75.0;
    double linesHoughMinLineLength_=  150.0;
    double linesHoughMaxLineGap_ =25.0;
    bool  dist1_, dist2_, dist3_, doca_ ;
    double slope_final, ang_final;
    double distance,doca,x0,x1,x2, y0,y1,y2, x,y;
    
    std::vector<cv::Vec3f> circles;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > lines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > goodLines_;
    std::vector<std::pair<cv::Point2f,cv::Point2f> > mergedLines_;

    std::vector<cv::Point2f> intersections_;
    std::vector<cv::Point2f> goodIntersections_;
    std::vector<cv::Point2f> finalIntersections_;
    std::vector<cv::Point2f> finalIntersectionsUp_;
    std::vector<cv::Point2f> finalIntersectionsDown_;

    
    std::string str_ii, str_p , str_lt, str_pc, str_x, str_y, str_slope_final,str_ang_final;
    ostringstream convert;   // stream used for the conversion
   
    
  // img_gs = cv::imread("/Users/keaveney/assembly_dev/cmstkmodlab/share/assembly/sensor_24MHz_333ms_1.png", CV_LOAD_IMAGE_GRAYSCALE);
  //  img_gs = cv::imread("/Users/keaveney/Downloads/im_scan___Exp10___EdgeThr65___lt160.png", CV_LOAD_IMAGE_GRAYSCALE);
//   img_gs = cv::imread("/Users/keaveney/assembly_dev/cmstkmodlab/share/assembly/sensor_36MHz_222ms_2.png", CV_LOAD_IMAGE_GRAYSCALE);

  
    
   for(int ii=10.0; ii<15.0; ii=ii+20.0)
   {

    convert << ii;      // insert the textual representation of 'Number' in the characters in the stream
    str_ii = convert.str();
       
    NQLog("AssemblyVUEyeCamera") << "  looping";

       if(is_FreezeVideo(hCam, IS_WAIT) == IS_SUCCESS){
   NQLog("AssemblyVUEyeCamera") << "  getting frame";
            void *pMemVoid; //pointer to where the image is stored
            is_GetImageMem (hCam, &pMemVoid);
            IplImage * img;
            img=cvCreateImage(cvSize(img_width, img_height), IPL_DEPTH_8S, 1); 
            img->nSize=112;
            img->ID=0;
            img->nChannels=3;
            img->alphaChannel=0;
            img->depth=8;
            img->dataOrder=0;
            img->origin=0;
            img->align=4;
            img->width=img_width;
            img->height=img_height;
            img->roi=NULL;
            img->maskROI=NULL;
            img->imageId=NULL;
            img->tileInfo=NULL;
	    // img->imageSize=3*img_width*img_height;
            img->imageData=(char*)pMemVoid;  //the pointer to imagaData
            img->widthStep=3*img_width;
            img->imageDataOrigin=(char*)pMemVoid; //and again
            //now you can use your img just like a normal OpenCV image
	    //    cvNamedWindow( "A", 1 );
            //cvShowImage("A",img);
            //cv::waitKey(1);

	    cv::Mat mat_img(img);
            cvtColor(mat_img, img_gs, CV_RGB2GRAY);
       }



    // for(int lt = 150; lt<300 ; lt=lt+10){
     for(int lt = 110; lt<120 ; lt=lt+20){

         linesCannyEdgeDetectionThreshold2_ =lt;

         for(int p = 145; p< 150; p=p+10){
             int pp  = p;
             circleEdgeDetectionThreshold_ = pp;

             lines_.clear();
             goodLines_.clear();
             intersections_.clear();
             goodIntersections_.clear();
             finalIntersections_.clear();
               NQLog("AssemblyVUEyeCamera") << "  running hough circles";
 
             cv::HoughCircles(img_gs, circles, CV_HOUGH_GRADIENT, 1, 20,
                     circleEdgeDetectionThreshold_, circleCenterDetectionThreshold_,
                     expectedCircleRadius_ - 15., expectedCircleRadius_ + 15.);

             //NQLog("AssemblyVUEyeCamera") << "  found circles = = "<<  circles.size();
             convert.str(" ");
             convert.clear();
             convert << p;
             str_p = convert.str();
             convert.str(" ");
             convert.clear();
             convert << lt;
             str_lt = convert.str();

             convert.str(" ");
             convert.clear();
   
             // std::cout<<" exposure  = "<<str_ii << "   threshold = " <<  str_p << std::endl;
             //   cv::Point2f center(cvRound(circleCenter_.x), cvRound(circleCenter_.y));
             //  circle center
             // cv::circle(local_image, center, 8, cv::Scalar(0, 255, 0), -1, CV_AA, 0);
             //circle outline
             //cv::circle(local_image, center, circleRadius_, cv::Scalar(0, 0, 255), 4, CV_AA, 0);
 
             cvtColor(img_gs, img_rgb , CV_GRAY2RGB);

            /// Draw the circles detected
            for( size_t i = 0; i < circles.size(); i++ )
                {
                    cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                    x = cvRound(circles[i][0]);
                    y = cvRound(circles[i][1]);
                    radius = cvRound(circles[i][2]);
                    // circle center
                    circle( img_rgb, center, 3, cv::Scalar(0,0,255), -1, 8, 0 );
                    //circle outline;
                    circle( img_rgb, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
                }
          

             cv::Canny(img_gs, img_edges,
	    		      linesCannyEdgeDetectionThreshold1_,
	    		      linesCannyEdgeDetectionThreshold2_,
	    		      linesCannyEdgeDetectionApertureSize_,
	    		      linesCannyEdgeDetectionL2Gradient_);

             std::vector<cv::Vec4i> tempLines;

               NQLog("AssemblyVUEyeCamera") << "  running hough lines";

             cv::HoughLinesP(img_edges, tempLines,
                    linesHoughDistanceResolution_,
                    linesHoughAngleResolution_*CV_PI/180,
                    linesHoughThreshold_,
                    linesHoughMinLineLength_,
                    linesHoughMaxLineGap_);

             
             for (size_t i = 0; i < tempLines.size(); i++ ) {
                 lines_.push_back(std::pair<cv::Point2f,cv::Point2f>(cv::Point2f(tempLines[i][0],tempLines[i][1]), cv::Point2f(tempLines[i][2],tempLines[i][3])));
             }
             
  
             cv::Point2f circleCenter_;
             if (circles.size() != 0){
                 circleCenter_.x = circles[0][0];
                 circleCenter_.y = circles[0][1];
             }
  

             std::cout <<  "merging lines..."<<std::endl;

             
             //find and merge duplicate lines (same slope and very similar start/end points.)
   /*        for (size_t y = 0;  y < lines_.size(); y++ ) {
                 for (size_t z = 0; z < lines_.size(); z++ ) {
                     if (y==z) continue;
                     
            double dist_start = std::sqrt(std::pow( lines_[y].first.x - lines_[z].first.x, 2.0) +std::pow(lines_[y].first.y - lines_[z].first.y, 2.0));
            double dist_end = std::sqrt(std::pow( lines_[y].second.x - lines_[z].second.x, 2.0) +std::pow(lines_[y].second.y - lines_[z].second.y, 2.0));
            double slope_diff  = (  lines_[y].second.y - lines_[y].first.y )   /  (  lines_[y].second.x - lines_[y].first.x  )   -   (  lines_[z].second.y - lines_[z].first.y )   /  (  lines_[z].second.x - lines_[z].first.x  ) ;
                     
                   //  std::cout <<  dist_start <<std::endl;
                    // if(dist_start < 3 && dist_end < 3 && slope_diff < 0.2){
                     if(fabs(dist_start) < 10 && fabs(slope_diff) < 0.05){
                         
                         cv::Point2f m_s(  (lines_[y].first.x + lines_[z].first.x)/2   ,  (lines_[y].first.y + lines_[z].first.y)/2 );
                         cv::Point2f m_e(  (lines_[y].second.x + lines_[z].second.x)/2   ,  (lines_[y].second.y + lines_[z].second.y)/2 );

                         std::pair<cv::Point2f,cv::Point2f> ml(m_s, m_e);
                         mergedLines_.push_back(ml);
                     }
                 }
             }
      */
             
             //select only the lines which have starting/ending points close to the circle centre and DOCA
             // characteristic of the L marker outline
             for (size_t i = 0; i < lines_.size(); i++ ) {
                 dist1_  = dist2_ = dist3_ = doca_ = false;
                 distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].first.x, 2.0) +
                             std::pow(circleCenter_.y - lines_[i].first.y, 2.0));
                 if (distance < expectedCircleRadius_*6.0) dist1_ = true;

                 distance = std::sqrt(std::pow(circleCenter_.x - lines_[i].second.x, 2.0) +
                 std::pow(circleCenter_.y - lines_[i].second.y, 2.0));
                 //  if (distance < expectedCircleRadius_*2.5) goodLines_.push_back(lines_[i]);
                 if (distance < expectedCircleRadius_*6.0)  dist2_ = true  ;
                 
                 x0 = circleCenter_.x;
                 y0 = circleCenter_.y;
                 x1 =  lines_[i].first.x;
                 y1 =  lines_[i].first.y;
                 x2 =  lines_[i].second.x;
                 y2 =  lines_[i].second.y;

                 doca =  std::fabs( x0*(y2-y1)  -  y0*(x2-x1) + x2*y1 -y2*x1)/ std::sqrt(   std::pow(y2 - y1,2.0)   +    std::pow(x2 - x1,2.0)      );

               
                 if ((doca <expectedCircleRadius_*5.8  && doca >expectedCircleRadius_*5.4)  || (doca <expectedCircleRadius_*1.3  && doca >expectedCircleRadius_*0.7)   ) doca_ = true;


                 if (dist1_ && dist2_ && doca_ ) goodLines_.push_back(lines_[i]);
                 // if ( doca_ == true) goodLines_.push_back(lines_[i]);
       }


    for (size_t i = 0; i < lines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(lines_[i].first.x), cvRound(lines_[i].first.y)),
                 cv::Point(cvRound(lines_[i].second.x), cvRound(lines_[i].second.y)),
                 cv::Scalar(255, 0, 0), 2, CV_AA);
    }

        for (size_t i = 0; i < goodLines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(goodLines_[i].first.x), cvRound(goodLines_[i].first.y)),
                     cv::Point(cvRound(goodLines_[i].second.x), cvRound(goodLines_[i].second.y)),
                 cv::Scalar(255, 0, 255), 4, CV_AA);
     }
             
             
   /*     for (size_t i = 0; i < mergedLines_.size(); i++ ) {
        cv::line(img_rgb,
                 cv::Point(cvRound(mergedLines_[i].first.x), cvRound(mergedLines_[i].first.y)),
                 cv::Point(cvRound(mergedLines_[i].second.x), cvRound(mergedLines_[i].second.y)),
                 cv::Scalar(0, 255, 0), 4, CV_AA);
             }
    */


	//find and draw all intersections between goodlines
    for (size_t i = 0; i < goodLines_.size(); i++) {
        for (size_t j = 0; j < goodLines_.size(); j++) {

            if (i == j ) continue;
        cv::Point2f theIntersection;
	    cv::Point2f o1,o2,p1,p2,r;
        double distance;
	    o1 = goodLines_[i].first;
	    p1 = goodLines_[i].second;
	    o2 = goodLines_[j].first;
	    p2 = goodLines_[j].second;

	    cv::Point2f x = o2 - o1;
	    cv::Point2f d1 = p1 - o1;
	    cv::Point2f d2 = p2 - o2;

	    double cross = d1.x*d2.y - d1.y*d2.x;
        double slope1 = (p1.y - o1.y) / (p1.x - o1.x);
        double slope2 = (p2.y - o2.y) / (p2.x - o2.x);
        double ang1 = atan (slope1) * 180 / 3.14;
        double ang2 = atan (slope2) * 180 / 3.14;
            
        double ang = fabs(ang2) - fabs(ang1);
        
       //if (fabs(ang) > 89.0  && fabs(ang) < 91.0 ) ;
            
       // cout <<" angle between lines: ang1 =  "<<  ang1  <<" ang2 = "<< ang2 <<" ang = "<<  ang<<endl;
            
            
          //  if (cv::norm(r - o1) > distance &&
          //      cv::norm(r - p1) > distance) continue;
            
          //  if (cv::norm(r - o2) > distance &&
          //      cv::norm(r - p2) > distance) continue;
          //  std::cout << "ang   " << ang     <<std::endl;
            
            if ( fabs(ang) > 80.0  && fabs(ang) < 100.0   &&   abs(cross) > /*EPS*/1e-7){
	    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
	    r = o1 + d1 * t1;
                intersections_.push_back(r);
                
                
       distance = std::sqrt(std::pow(circleCenter_.x - r.x, 2.0) +
                                     std::pow(circleCenter_.y - r.y, 2.0));
                //if (distance > 6.0*expectedCircleRadius_)continue;
            }
        }
    }
             
             cout <<"n intersections  = = "<< intersections_.size()<< endl;

             
             for(int  intsec = 0; intsec < intersections_.size(); intsec++ ){
                 bool goodIntersection = false;
             for(int  gl = 0; gl < goodLines_.size(); gl++ ){
              
                 double d_start = std::sqrt(std::pow(intersections_[intsec].x - goodLines_[gl].first.x, 2.0) + std::pow(intersections_[intsec].y - goodLines_[gl].first.y, 2.0));
                 double d_end = std::sqrt(std::pow(intersections_[intsec].x - goodLines_[gl].second.x, 2.0) + std::pow(intersections_[intsec].y - goodLines_[gl].second.y, 2.0));
                 if(d_start< 5.0 || d_end < 5.0) goodIntersection = true;
             }
                 if(goodIntersection == true) goodIntersections_.push_back(intersections_[intsec]);
             }
             
             
             for (int goodint =0; goodint < goodIntersections_.size(); goodint++){
                // circle( img_rgb, goodIntersections_[goodint], 10, cv::Scalar(0,255,0), 3, 8, 0 );
                 
                 //get the crucial two intersections by the circle
                 
                 if (std::sqrt(std::pow(circleCenter_.x - goodIntersections_[goodint].x, 2.0) + std::pow(circleCenter_.y - goodIntersections_[goodint].y, 2.0)) < expectedCircleRadius_*1.6) finalIntersections_.push_back(goodIntersections_[goodint]);
                // cout <<"distance of intersections ==  "<<  std::sqrt(std::pow(circleCenter_.x - goodIntersections_[goodint].x, 2.0) + std::pow(circleCenter_.y - goodIntersections_[goodint].y, 2.0)) << endl;
             }
             
             cout <<"n final intersection  = = "<< finalIntersections_.size()<< endl;
             
             
             //one typically ends up with multiple intersections at each corner due extra, spurious lines/
             // I divide the collection of final intersections into those abpve the circle's centre "up"
             // and below "down". I use the avergae of these to define the orientation.
             double running_total_x  =0;
             double running_total_y  =0;
             double av_x_up;
             double av_x_down;
             double av_y_up;
             double av_y_down;

             finalIntersectionsDown_.clear();
             finalIntersectionsUp_.clear();
             
             if (finalIntersections_.size() >0){
                 for (int finalint =0; finalint < finalIntersections_.size(); finalint++) {
                     cout <<"  "<<endl;
                     cout <<" final intersection y =  "<< finalIntersections_[finalint].y   <<   " circles centre = " <<    y  <<endl;
                     if ( y > finalIntersections_[finalint].y){
                         cout <<" down"<<endl;
                     finalIntersectionsDown_.push_back(finalIntersections_[finalint]);
                     }
                     
                     
                     else if ( y < finalIntersections_[finalint].y){
                         cout <<" up"<<endl;
                     finalIntersectionsUp_.push_back(finalIntersections_[finalint]);
                     }
                     
                 }

                 
                 for (int finalintup =0; finalintup < finalIntersectionsUp_.size(); finalintup++) {
                     running_total_x = running_total_x + finalIntersectionsUp_[finalintup].x;
                     running_total_y = running_total_y + finalIntersectionsUp_[finalintup].y;
                 }
                 
                 av_x_up = running_total_x/finalIntersectionsUp_.size();
                 av_y_up = running_total_y/finalIntersectionsUp_.size();
                 
                 running_total_x  =0;
                 running_total_y  =0;
                 
                 for (int finalintdown =0; finalintdown < finalIntersectionsDown_.size(); finalintdown++) {
                     running_total_x = running_total_x + finalIntersectionsDown_[finalintdown].x;
                     running_total_y = running_total_y + finalIntersectionsDown_[finalintdown].y;
                 }
                 
                 av_x_down = running_total_x/finalIntersectionsDown_.size();
                 av_y_down = running_total_y/finalIntersectionsDown_.size();

             
             
//             if (finalIntersections_.size() >0){
                 for (int finalintup =0; finalintup < finalIntersectionsUp_.size(); finalintup++) {
                     circle( img_rgb, finalIntersectionsUp_[finalintup], 10, cv::Scalar(0,255,255), 3, 8, 0 );
                 }
                 
                for (int finalintdown =0; finalintdown < finalIntersectionsDown_.size(); finalintdown++) {
                     circle( img_rgb, finalIntersectionsDown_[finalintdown], 10, cv::Scalar(0,255,0), 3, 8, 0 );
                 }
                 
             
                // cv::line(img_rgb,
                //          cv::Point(cvRound(finalIntersections_[0].x), cvRound(finalIntersections_[0].y)),
                //          cv::Point(cvRound(finalIntersections_[1].x), cvRound(finalIntersections_[1].y)),
                //          cv::Scalar(0, 255, 255), 4, CV_AA);
                 
                  cv::line(img_rgb,
                           cv::Point(cvRound(av_x_down), cvRound(av_y_down)),
                           cv::Point(cvRound(av_x_up), cvRound(av_y_up)),
                           cv::Scalar(0, 255, 255), 4, CV_AA);
                 
                 
                 circle( img_rgb, cv::Point(x,y), 10, cv::Scalar(255,255,0), 3, 8, 0 );
                 
                 
                 slope_final = (av_y_up -  av_y_down) / (av_x_up -  av_x_down);
                 ang_final = atan (slope_final) * 180 / 3.14;
                 
               // slope_final = (finalIntersections_[0].y - finalIntersections_[1].y) / (finalIntersections_[0].x - finalIntersections_[1].x);
               // ang_final = atan (slope_final) * 180 / 3.14;

             }
             
             
             cout <<"n up intersections " << finalIntersectionsUp_.size() <<endl;
             cout <<"n down intersections " << finalIntersectionsDown_.size() <<endl;

             
             convert.str(" ");
             convert.clear();
             convert << ang_final;
             str_ang_final = convert.str();
             convert.str(" ");
             convert.clear();
             convert << circleCenter_.x ;
             str_x = convert.str();
             convert.str(" ");
             convert.clear();
             convert << circleCenter_.y;
             str_y = convert.str();
             
             //std::string text_result = "Marker is at = (" +   +  ", " +   + ") \n with orientation = " +  + " degrees"
             std::string text_result = "Marker is at = (" +  str_x  +  ", " + str_y + "), with orientation = " + str_ang_final  + " degrees";
             //char text_result_c = text_result.c_str();
             
             
             putText(img_rgb, text_result, cvPoint(30,100),
                     cv::FONT_HERSHEY_COMPLEX_SMALL, 2.8, cvScalar(200,200,250), 1, CV_AA);
             std::string filename = "/home/keaveney/Desktop/calibration/set_expos/im_scan___Exp" + str_ii + "___EdgeThr"  + str_p +"___lt"+str_lt+".png";
             cv::imwrite(filename, img_rgb);

             
            


          NQLog("AssemblyVUEyeCamera") << "  found marker";
         }
         
         
         
        }
        }
    NQLog("AssemblyVUEyeCamera") << " obtained results  "<< x0<<" "<< y0 <<"  "<< ang_final;

    emit resultObtained(x0, y0, ang_final);
    
    
    
        }




