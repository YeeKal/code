#include <iostream>
#include <vector>
#include <string>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

void computeGradientX(const Mat& src, Mat& gx);
void computeGradientY(const Mat& src, Mat& gy);
void computeLaplacianX(const Mat& src,Mat& lx);
void computeLaplacianY(const Mat& src,Mat& ly);
void computeLapX(const Mat& gx,Mat& gxx);
void computeLapY(const Mat& gy,Mat& gyy);
void seamlessClone(const Mat& bg,const Mat& fg,const Mat& mask,int offsetX,int offsetY, Mat & general_mixed_img,Mat& poisson_mixed_img);

/*generate mask according to the foreground img by user*/
class MaskGenerate{
    public:
        Mat fore_img;
        Mat mask_img;
        Mat mask_final;
        string fg_window="mask generate";
        Point2i last_point;
        bool mouse_enable=true;
        MaskGenerate(Mat &fg);
        ~MaskGenerate(){}
        void getMask();
        static void mouseGetMask(int event, int x, int y, int flags, void* ptr);
        void drawMask(int event,int x,int y,int flags);   
};

int main(){
    //m1
    // string bg_path="img/ground.jpg";//img/wallheaven.jpg";//"img/1/bg.jpg";
    // string fg_path="img/cat2.jpg";//"img/kaola.jpg";//"img/1/fg.jpg";

    //m2
    string bg_path="img/1/bg.jpg";
    string fg_path="img/1/fg.jpg";

    // Mat grass=imread("img/skelon.jpg");
    // Mat ground;
    // resize(grass,ground,Size(grass.cols/5, grass.rows/5),0,0,INTER_LINEAR);
    // Rect ha=Rect(0,0,1000,1000);
    // imshow("ground",ground);
    // imwrite("img/robot_lab.jpg",ground);
    // waitKey();

    Mat bg,fg,mask;
    bg=imread(bg_path);
    fg=imread(fg_path);
    //mask=imread(mask_path);
    if(bg.empty() || fg.empty() ){
        cout<<"error! cannot open img."<<endl;
        return -1;
    }

    MaskGenerate maskGenerate(fg);//generate the mask by user
    mask=maskGenerate.mask_final;
    imshow("mask",mask);

    Mat general_mixed_img,poisson_mixed_img,compare_img;
    seamlessClone(bg, fg, mask, 50, 80, general_mixed_img,poisson_mixed_img);

    hconcat(general_mixed_img,poisson_mixed_img,compare_img);
    imshow("result",compare_img);
    waitKey(0);

    return 0;
}

//horizontal gradient gx(i,j)=img(i+1,j)-img(i,j)
void computeGradientX(const Mat& src, Mat& gx){
    int height=src.rows;
    int width=src.cols;
    Mat fm;
    src.convertTo(fm, CV_32FC3);
    Mat long_src=repeat(fm, 1, 2);
    Mat src_x=long_src(Rect(1,0,width,height));
    gx=src_x-fm;
    gx.col(width-1)=0;
}
//vertical gradient gy(i,j)=img(i,j+1)-img(i,j)
void computeGradientY(const Mat& src, Mat& gy){
    int height=src.rows;
    int width=src.cols;
    Mat fm;
    src.convertTo(fm, CV_32FC3);
    Mat long_src=repeat(fm, 2, 1);
    Mat src_y=long_src(Rect(0,1,width,height));
    gy=src_y-fm;
    gy.row(height-1)=0;
}
//gxx(i,j)=gx(i,j)-gx(i-1,j)
void computeLapX(const Mat& gx,Mat& gxx){
    int height=gx.rows;
    int width=gx.cols;
    Mat long_gx=repeat(gx, 1, 2);
    Mat gx_roi=long_gx(Rect(width-1,0,width,height));
    gxx=gx-gx_roi;
    gxx.col(0)=0;
}
//gyy(i,j)=gy(i,j)-gy(i,j-1)
void computeLapY(const Mat& gy,Mat& gyy){
    int height=gy.rows;
    int width=gy.cols;
    Mat long_gy=repeat(gy, 2, 1);
    Mat gy_roi=long_gy(Rect(0,height-1,width,height));
    gyy=gy-gy_roi;
    gyy.col(0)=0;
}

void seamlessClone(const Mat& bg,
                   const Mat& fg,
                   const Mat& mask,
                   int offsetX,int offsetY, 
                   Mat & general_mixed_img,Mat& poisson_mixed_img)
{
    poisson_mixed_img=bg.clone();
    general_mixed_img=bg.clone();

    Mat mask_gray=Mat(mask.size(),CV_8UC1);
    Mat mask_binary;
    if(mask.channels() !=1) //mask need to have only one channel
        cvtColor(mask,mask_gray,COLOR_BGR2GRAY);
    else
        mask_gray=mask;
    threshold(mask_gray,mask_binary,125,255,THRESH_BINARY);

    Rect rect_bg=Rect(offsetX,offsetY,fg.cols,fg.rows);
    Rect rect_fg=Rect(0,0,fg.cols,fg.rows);

    
    Mat test_mask_value=general_mixed_img(rect_bg);  
    //shallow copy
    fg.copyTo(test_mask_value,mask_binary);

    Mat gx_bg,gy_bg,gx_fg,gy_fg;
    
    computeGradientX(bg, gx_bg);
    computeGradientY(bg, gy_bg);
    computeGradientX(fg, gx_fg);
    computeGradientY(fg, gy_fg);
    gx_fg.copyTo(gx_bg(rect_bg),mask_binary);
    gy_fg.copyTo(gy_bg(rect_bg),mask_binary);
    gx_fg.release();
    gy_fg.release();

    Mat lap_mixed,lap_mask;
    Mat gxx,gyy;
    gxx=Mat::zeros(bg.size(),CV_32FC3);
    gyy=Mat::zeros(bg.size(),CV_32FC3);
    lap_mixed=Mat::zeros(bg.size(),CV_32FC3);
    computeLapX(gx_bg, gxx);
    computeLapY(gy_bg, gyy);
    
    lap_mixed=gxx+gyy;
    lap_mixed(rect_bg).copyTo(lap_mask,mask_binary);

    //construct matrix
    
    std::vector<Point2i> pts_index;//the order of the point:index to point
    Mat index_map=Mat(mask_binary.size(),CV_16UC1);//store index:point to index
    Eigen::MatrixXi map_eigen=Eigen::MatrixXi::Zero(mask_binary.rows,mask_binary.cols);
    for(int i=0;i<mask_binary.cols;i++){
        for(int j=0;j<mask_binary.rows;j++){
            if( mask_binary.at<uchar>(j,i)){
                pts_index.push_back(Point2i(i,j));
                map_eigen(j,i)=pts_index.size();
            }
        }
    }
    
    cout<<"inside point size:"<<pts_index.size()<<endl;

    //construct AX=b
    
    //add boundary point
    Mat nesw=(Mat_<char>(4,2)<<0,-1,1,0,0,1,-1,0);
    int pts_size=pts_index.size();
    int extra=0;
    for (int i = 0; i < pts_size; ++i)
    {   
        
        for(int k=0;k<4;k++){       //consider the four direction NESW
            int h=pts_index[i].y+nesw.at<char>(k,1);
            int w=pts_index[i].x+nesw.at<char>(k,0);
            if(!map_eigen.coeffRef(h,w))
            {   
                extra++;
                map_eigen(h,w)=pts_size+extra;//update map
                pts_index.push_back(Point2i(w,h));//update pt
            }
        }
    }
    cout<<"all point size:"<<pts_index.size()<<endl;

    //construct A/b
    Eigen::MatrixXf b=Eigen::MatrixXf::Zero(pts_index.size(),3);
    std::vector<Eigen::Triplet<float>> coefficients;
    Mat rect_bg_value=poisson_mixed_img(rect_bg);
    for(int i=0;i<pts_index.size();i++){
        int h=pts_index[i].y;
        int w=pts_index[i].x;
        if(map_eigen.coeffRef(h,w))
        {
            if(map_eigen.coeffRef(h,w)>pts_size){  //boundary point
                //draw point
                test_mask_value.at<Vec3b>(h,w)[0]=0;
                test_mask_value.at<Vec3b>(h,w)[1]=0;
                test_mask_value.at<Vec3b>(h,w)[2]=255;
                //construct A
                coefficients.push_back(Eigen::Triplet<float>(i, i, 1));
                //construct b
                float div[3]={0,0,0};
                for(int channel=0;channel<bg.channels();channel++){  //construct b
                    div[channel]=(float)(rect_bg_value.at<Vec3b>(pts_index[i].y,pts_index[i].x)[channel]);
                }
                b.row(i)<<div[0],div[1],div[2];
            }
            else{                               //insider point
                //construct A
                coefficients.push_back(Eigen::Triplet<float>(i, i, -4));
                for(int k=0;k<4;k++){       //consider the four direction NESW
                    int h=pts_index[i].y+nesw.at<char>(k,1);
                    int w=pts_index[i].x+nesw.at<char>(k,0);
                    coefficients.push_back(Eigen::Triplet<float>(i, map_eigen.coeffRef(h,w)-1,1.0));
                }
                //construct b
                float div[3]={0,0,0};
                for(int channel=0;channel<bg.channels();channel++){  
                    div[channel]=lap_mask.at<Vec3f>(pts_index[i].y,pts_index[i].x)[channel];
                }
                b.row(i)<<div[0],div[1],div[2];
            }
        }
    }

    Eigen::SparseMatrix<float> A(pts_index.size(),pts_index.size());
    //cout<<"ha.1"<<endl;
    A.setFromTriplets(coefficients.begin(),coefficients.end());
    //cout<<"ha.2"<<endl;
    //solve AX=b
    Eigen::MatrixXf X(pts_index.size(),3);
    Eigen::SparseLU< Eigen::SparseMatrix<float> > solver;
    //Eigen::SimplicialLDLT<  Eigen::SparseMatrix<float> > solver;
    solver.analyzePattern(A);
    solver.factorize(A);
    if(solver.info()!=Eigen::Success){
        cout<<"Error!. Sparsse LU.Compute."<<endl;
        return;
    }
    for(int i=0;i<fg.channels();i++){
        X.col(i)=solver.solve(b.col(i));
        if(solver.info()!=Eigen::Success){
            cout<<"Error!. Sparsse LU.Solve."<<endl;
            return;
        }
    }

    //fill back in the dst img
    for (int i = 0; i < pts_index.size(); ++i){
        for(int j=0;j<poisson_mixed_img.channels();j++){
            float pix=X(i,j);
            if(pix<0){
                //pix=0; this is not perferred than the follower
                pix=(int)(-pix);
            }
            else if(pix>255){//not found 
                //cout<<(int)(uchar)pix<<endl;
                pix=255;
            }
            rect_bg_value.at<Vec3b>(pts_index[i].y,pts_index[i].x)[j]=(uchar)pix;
        }
    }
}
MaskGenerate::MaskGenerate(Mat& fg){
    fore_img=fg.clone();
    mask_img=Mat::zeros(fg.size(),CV_8UC1);
    getMask();
}
void MaskGenerate::getMask(){
    imshow(fg_window, fore_img);
    //imshow("mask", mask_img);
    cout<<"HINT: drag your mouse to draw a close curve, and right-click inside the curve!"<<endl;
    setMouseCallback(fg_window,MaskGenerate::mouseGetMask,this);
    waitKey(0);
}
void MaskGenerate::mouseGetMask(int event, int x, int y, int flags, void* ptr){
    MaskGenerate * p=(MaskGenerate*) ptr;
    p->drawMask(event,x,y,flags);
    
}
void MaskGenerate::drawMask(int event,int x,int y,int flags){
    if(flags==CV_EVENT_FLAG_LBUTTON && mouse_enable){
        if(!last_point.x){
            last_point=Point2i(x,y);
            circle(fore_img, Point2i(x,y), 2, Scalar(0,0,255),FILLED);
            circle(mask_img,  Point2i(x,y), 2,Scalar(100),FILLED );
            imshow(fg_window, fore_img);
            //imshow("mask", mask_img);
        }
        else{
            line(fore_img,last_point,Point2i(x,y),Scalar(0,0,255),2,FILLED);
            line(mask_img,last_point,Point2i(x,y),Scalar(100),2,FILLED);
            imshow(fg_window, fore_img);
            //imshow("mask", mask_img);
            last_point=Point2i(x,y);
        }
    }
    if(event==CV_EVENT_RBUTTONDOWN && mouse_enable){
        floodFill(mask_img, Point2i(x,y), Scalar(255));
        Mat mask_temp;
        Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
        dilate(mask_img, mask_temp, element);
        threshold(mask_temp, mask_final, 125, 255, THRESH_BINARY);
        //destroyWindow(fg_window);
        //destroyAllWindows();
        mouse_enable=false;
    }
    //cout<<x<<endl;
}
