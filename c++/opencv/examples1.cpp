/*****************************************************
support: opencv 3.4.1   ubuntu 16.04
example1  trakbar to ajust the brightness and contrast value
example2    open the camera and save to <avi> file
example3    open the camera and process with Sobel operator
example4    mouse event and drawing function
example5    object detection with background difference

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
