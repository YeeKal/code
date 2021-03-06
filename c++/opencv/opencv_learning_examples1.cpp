/*****************************************************
support: opencv 3.4.1   ubuntu 16.04

example1  trakbar to ajust the brightness and contrast value
example2  open the camera and save to <avi> file
example3  open the camera and process with Sobel operator
example4  mouse event and drawing function
example5  object detection with background difference
example6  houg transformation to detect line and circles
example7  Features2D + Homography to find a known object


*****************************************************/

/*
example1  trakbar to ajust the brightness and contrast value
*/
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
using namespace std;
using namespace cv;

Mat img,img_a;
int threshval=160;
int g_nContrastValue;
int g_nBrightValue;
static void on_trackbar(int, void*);
static void ContrastAndBright(int,void *);
int main(int argc, char** argv){
    String imageName("imgs/flower123.jpeg");
    if(argc >1){
        imageName=argv[1];
    }
    img=imread("imgs/kaola.jpg");
    img_a=Mat::zeros(img.size(),img.type());
    g_nContrastValue=80;
    g_nBrightValue=80;
    namedWindow("result",WINDOW_AUTOSIZE);
    
    createTrackbar("contrast","result",&g_nContrastValue,300,ContrastAndBright);
    createTrackbar("brightness","result",&g_nBrightValue,200,ContrastAndBright);
    ContrastAndBright(g_nContrastValue,0);
    ContrastAndBright(g_nBrightValue,0);

    cout<<endl<<"That's all right;\n"<<"<q> to quit.\n"<<endl;
    while(char(waitKey(1))!='q') {}

    return 0;
}

static void on_trackbar(int, void*)  
{  
    Mat bw = threshval < 128 ? (img < threshval) : (img > threshval);  
  
    //定义点和向量  
    vector<vector<Point> > contours;  
    vector<Vec4i> hierarchy;  
  
    //查找轮廓  
    findContours( bw, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );  
    //初始化dst  
    Mat dst = Mat::zeros(img.size(), CV_8UC3);  
    //开始处理  
    if( !contours.empty() && !hierarchy.empty() )  
    {  
        //遍历所有顶层轮廓，随机生成颜色值绘制给各连接组成部分  
        int idx = 0;  
        for( ; idx >= 0; idx = hierarchy[idx][0] )  
        {  
            Scalar color( (rand()&255), (rand()&255), (rand()&255) );  
            //绘制填充轮廓  
            drawContours( dst, contours, idx, color, CV_FILLED, 8, hierarchy );  
        }  
    }  
    //显示窗口  .cols;
    imshow( "Connected Components", dst );  
}  

static void ContrastAndBright(int,void *){
    namedWindow("origin_img",1);

    //visit each pixel image.at<Vec3b>(y,x)[c]
    for(int y=0;y<img.rows;y++)
    {
        for(int x=0;x<img.cols;x++)
        {
            for (int c=0;c<3;c++)
            {
                img_a.at<Vec3b>(y,x)[c]=saturate_cast<uchar>((g_nContrastValue*0.01)*(img.at<Vec3b>(y,x)[c])+g_nBrightValue);
            }
        }
    }
    imshow("origin_img",img);
    imshow("result",img_a);
}

/*
example2  open the camera and save to <avi> file
*/

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
    VideoCapture cap(0);
    VideoWriter wri;
    Mat frame;
    int frameInt=0;

    string outFile="imgs/output.avi";

    //get the width and height of each frame
    int frameWidth, frameHeight;
    //flags for get(): https://docs.opencv.org/3.4.1/d4/d15/group__videoio__flags__base.html#gaeb8dd9c89c10a5c63c139bf7c4f5704d
    frameWidth=cap.get(CAP_PROP_FRAME_WIDTH);
    frameHeight=cap.get(CAP_PROP_FRAME_HEIGHT);
    cout<<"width: "<<frameWidth<<"  height: "<<frameHeight<<endl;
    Size frameSize(frameWidth,frameHeight);
    //frame rate
    double frameRate=cap.get(CAP_PROP_FPS);
    cout<<"frame rate: "<<frameRate<<endl;

    int codec=CV_FOURCC('M','J','P','G');//jpg
    wri.open(outFile,codec,frameRate,frameSize,true);
    if(!wri.isOpened()){
        cout<<"error! VideoWriter failed to initialize."<<endl;
        return -1;
    }

    cap.open(0);//cap.open(<videoFile>);
    if(!cap.isOpened()){
        cout<<"error! unable to open the camera.";
        return -1;
    }
    while(char(waitKey(5))!='q'){
        cap.read(frame);
        if(frame.empty()){
            cout<<"error! no frame grabbed.";
            return -1;
        }
        imshow("Live",frame);
        wri<<frame;//wri.write(frame);
        frameInt++;
    }
    cout<<"frame numbers grabbed:"<<frameInt<<endl;

    return 0;
}

/*
example3  open the camera and process with Sobel operator
        the writer failed and commented, and I can't fix it
*/
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

cv::Mat imagePross(Mat img);

int main(int argc, char** argv){
    string outFile="imgs/output.avi";
    VideoCapture cap(0);
    VideoWriter wri;
    Mat frame,frameOut;
    int frameInt=0;

    //get the width and height of each frame
    int frameWidth, frameHeight;
    //flags for get(): https://docs.opencv.org/3.4.1/d4/d15/group__videoio__flags__base.html#gaeb8dd9c89c10a5c63c139bf7c4f5704d
    frameWidth=cap.get(CAP_PROP_FRAME_WIDTH);
    frameHeight=cap.get(CAP_PROP_FRAME_HEIGHT);
    cout<<"width: "<<frameWidth<<"  height: "<<frameHeight<<endl;
    Size frameSize(frameWidth,frameHeight);
    //frame rate
    double frameRate=cap.get(CAP_PROP_FPS);
    cout<<"frame rate: "<<frameRate<<endl;

    int codec=CV_FOURCC('M','J','P','G');//jpg
    wri.open(outFile,codec,frameRate,frameSize,true);
    if(!wri.isOpened()){
        cout<<"error! VideoWriter failed to initialize."<<endl;
        return -1;
    }

    //cap.open(0);//cap.open(<videoFile>);
    if(!cap.isOpened()){
        cout<<"error! unable to open the camera.";
        return -1;
    }
    while(char(waitKey(5))!='q'){
        cap.read(frame);
        if(frame.empty()){
            cout<<"error! no frame grabbed.";
            return -1;
        }
        frame=imagePross(frame);
        imshow("Live",frame);
        //wri.write(frame);
        frameInt++;
    }
    cout<<"frame numbers grabbed:"<<frameInt<<endl;

    return 0;
}

//sobel operator
cv::Mat imagePross(Mat img){
    int ddepth=CV_16S;
    Mat src,img_gray,out;
    Mat grad_x,grad_y;
    Mat abs_grad_x,abs_grad_y;
    //gaussian blur
    GaussianBlur(img,src,Size(3,3),0,0);
    //convert to grayscale
    cvtColor(src, img_gray, COLOR_BGR2GRAY);

    Sobel(img_gray,grad_x,ddepth,1,0);
    convertScaleAbs(grad_x, abs_grad_x );
    Sobel(img_gray,grad_y,ddepth,0,1);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, out);

    return out;
}

/*
example4 mouse event and drawing function
*/
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace std;
using namespace cv;
static void onMouse(int event,int x,int y,int,void *);

Mat imgProcessed;
Mat img;
int pic_info[4]={0,0,0,0};
int fi=0;

int main(int argc, char** argv){
    string imgFile="imgs/kaola.jpg";
    Mat imgROI;
    char c;

    imgProcessed=imread(imgFile,1);
    img=imgProcessed.clone();
    imshow("img",imgProcessed);
    cvSetMouseCallback("img", onMouse);
    c=waitKey(1);
    namedWindow("roi");
    while(c!='q'){
        if(c=='c'){
            imgROI=img(Rect(pic_info[0],pic_info[1],pic_info[2] - pic_info[0], pic_info[3] - pic_info[1]));
            imshow("roi", imgROI);
        }
        c=waitKey(1);
    }

    return 0;
}

static void onMouse(int event,int x,int y,int flags,void *ustc){
    if(event==CV_EVENT_LBUTTONDOWN){
        pic_info[0]=x;
        pic_info[1]=y;
    }
    else if(event==CV_EVENT_LBUTTONUP){
        pic_info[2]=x;
        pic_info[3]=y;
        rectangle(imgProcessed , cvPoint(pic_info[0], pic_info[1]), cvPoint(pic_info[2], pic_info[3]), cvScalar(255, 255, 0), LINE_4);
        imshow("img", imgProcessed);

    }
}

/*
example5 object detection with background difference
*/
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;
void imagePross(Mat img,Mat background);
//s very small partion won't be detected
double min_contour_area=2000;

int main(int argc, char** argv){

    VideoCapture cap(0);
    Mat frame,background;
    int frameInt=0;
    int frameWidth, frameHeight;

    frameWidth=cap.get(CAP_PROP_FRAME_WIDTH);
    frameHeight=cap.get(CAP_PROP_FRAME_HEIGHT);
    cout<<"width: "<<frameWidth<<"  height: "<<frameHeight<<endl;
    Size frameSize(frameWidth,frameHeight);

    //frame rate
    double frameRate=cap.get(CAP_PROP_FPS);
    cout<<"frame rate: "<<frameRate<<endl;

    if(!cap.isOpened()){
        cout<<"error! unable to open the camera.";
        return -1;
    }
    cap.read(background);
    Mat back_gray,back_blur;
    cvtColor(background, back_gray, COLOR_BGR2GRAY);
    GaussianBlur(back_gray,back_blur,Size(21,21),0,0);
    while(char(waitKey(5))!='q'){
        cap.read(frame);
        if(frame.empty()){
            cout<<"error! no frame grabbed.";
            return -1;
        }
        imagePross(frame,back_blur);

    }

    return 0;
}

void imagePross(Mat img,Mat background){
    Mat src,img_gray,img_diff,out,out_dilate;
    Mat element=getStructuringElement(MORPH_ELLIPSE, Size(5,5));

    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    GaussianBlur(img_gray,src,Size(21,21),0,0);
  
    absdiff(src, background, img_diff);

    threshold(img_diff, out, 25, 255,THRESH_BINARY );
    dilate(out, out_dilate, element,Point(-1,-1),2);

    //contours
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    findContours(out_dilate, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    size_t contours_num=contours.size();
    for(int i=0;i<contours_num;i++){
        if(contourArea(contours.at(i))<min_contour_area)
            continue;
        Rect rec=boundingRect(contours.at(i));
        rectangle(img, rec, Scalar(200,200,0),LINE_4);
    }

    imshow("Live",img); 
    imshow("gray",out_dilate);

}

/*
example6  houg transformation to detect line and circles
*/
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

int main(){
    Mat img=imread("imgs/house.jpg");
    Mat img_circle=imread("imgs/circle.jpeg");

    Mat midImg,destImg;
    Mat midImg_circle,destImg_circle;

    Canny(img, midImg, 50, 200,3);
    cvtColor(midImg,destImg ,COLOR_GRAY2BGR);
    imshow("img",img);

    vector<Vec2f> lines;
    HoughLines(midImg, lines, 1, CV_PI/180, 200);
    cout<<"num of lines:"<<lines.size()<<endl;
    for(size_t i=0;i<lines.size();i++){
        float rho=lines[i][0],theta=lines[i][1];
        Point pt1,pt2;
        double a=cos(theta),b=sin(theta);
        double x0=a*rho, y0=b*rho;
        pt1.x=round(x0+1000*(-b));
        pt1.y=round(y0+1000*(a));
        pt2.x=round(x0-1000*(-b));
        pt2.y=round(y0-1000*(a));
        line(destImg,pt1,pt2,Scalar(30,30,255),1,CV_AA);
    }
    imshow("houg",destImg);

    //houghlines p
    vector<Vec4i> plines;
    HoughLinesP(midImg, plines, 1, CV_PI/180, 100,30,5);
    for(size_t i;i<plines.size();i++){
        line(destImg,Point(plines[i][0],plines[i][1]),
            Point(plines[i][2],plines[i][3]),Scalar(30,30,255),3,8);
    }
    
    //hough circle
    imshow("img circle",img_circle);
    cvtColor(img_circle,midImg_circle,COLOR_RGB2GRAY);
    GaussianBlur(midImg_circle, destImg_circle, Size(9,9), 2,2);
    vector<Vec3f> circles;
    HoughCircles(destImg_circle, circles, HOUGH_GRADIENT, 2, 10,200,100);
    for(size_t i=0;i<circles.size();i++){
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(img_circle,center,3,Scalar(0,255,0),-1,8,0);
        circle(img_circle,center,radius,Scalar(0,0,255),3,8,0);
    }
    imshow("hough gray",destImg_circle);
    imshow("hougp",destImg);
    while(char(waitKey(5))!='q'){}
    return 0;
}

/*
example7  Features2D + Homography to find a known object
*/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <pthread.h>
#include <vector>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
int main(){

    Mat sift1=imread("imgs/sift1.jpeg");
    Mat sift2=imread("imgs/sift2.jpg");
    if(!sift1.data || !sift2.data){
        cout<<"Error reading imgs!"<<endl;
    }
    
    // imshow("sift1", sift1);
    // imshow("sift2", sift2);
    
    int min_hessian=400;

    Ptr<SURF> detector=SURF::create();
    detector->setHessianThreshold(min_hessian);
    vector<KeyPoint> keypoint1,keypoint2;
    Mat descriptors1,descriptors2;

    detector->detectAndCompute(sift1,Mat(),keypoint1,descriptors1);
    detector->detectAndCompute(sift2,Mat(),keypoint2,descriptors2);

    //matching
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors1,descriptors2,matches);

    double max_dist=0;
    double min_dist=100;

    //find max/min dist
    for(int i=0;i<descriptors1.rows;i++){
        double dist=matches[i].distance;
        if(dist<min_dist) min_dist=dist;
        if(dist>max_dist) max_dist=dist;
    }
    cout<<"max dist:"<<max_dist<<endl<<"min dist"<<min_dist<<endl;

    //draw good matches dist<2*min_dist
    vector<DMatch> good_matches;
    for(int i=0;i<descriptors1.rows;i++){
        if(matches[i].distance<=max(2*min_dist,0.02))
            good_matches.push_back(matches[i]);
    }
    Mat img_matches;
    drawMatches(sift1, keypoint1, sift2, keypoint2, good_matches, img_matches,
                    Scalar::all(-1),Scalar::all(-1),vector<char>(),
                    DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    namedWindow("matches");
    

    //localize the object
    vector<Point2f> obj;
    vector<Point2f> scene;
    for(size_t i=0;i<good_matches.size();i++){
        obj.push_back(keypoint1[good_matches[i].queryIdx].pt);
        scene.push_back(keypoint2[good_matches[i].trainIdx].pt);
    }
    Mat H=findHomography(obj, scene, RANSAC);
    //get the corners from sift1
    vector<Point2f> obj_corner(4);
    obj_corner[0]=cvPoint(0,0);
    obj_corner[1]=cvPoint(sift1.cols,0);
    obj_corner[2]=cvPoint(sift1.cols,sift1.rows);
    obj_corner[3]=cvPoint(0,sift1.rows);
    vector<Point2f> scene_corner(4);
    perspectiveTransform(obj_corner, scene_corner, H);

    line(img_matches,scene_corner[0]+Point2f(sift1.cols, 0),scene_corner[1]+Point2f(sift1.cols, 0),Scalar(0,255,0),4);
    line(img_matches,scene_corner[1]+Point2f(sift1.cols, 0),scene_corner[2]+Point2f(sift1.cols, 0),Scalar(0,255,0),4);
    line(img_matches,scene_corner[2]+Point2f(sift1.cols, 0),scene_corner[3]+Point2f(sift1.cols, 0),Scalar(0,255,0),4);
    line(img_matches,scene_corner[3]+Point2f(sift1.cols, 0),scene_corner[0]+Point2f(sift1.cols, 0),Scalar(0,255,0),4);
    imshow("matches",img_matches);
    while(char(waitKey(5))!='q'){}
    return 0;
}   