/*色彩影像實作技術_作業4: 人臉區域處理*/
//#include "stdafx.h" //如果專案標頭檔裡裡有stdafx.h，原始程式檔有stdafx.cpp，需加入這一行。如果是空專案，則不用加這一行。
#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>

using namespace cv; //宣告 opencv 函式庫的命名空間
using namespace std; //宣告 C++函式庫的命名空間

/** Function Headers */
void detectAndDisplay(void); //偵測人臉的函式宣告

/** Global variables */
String face_cascade_name = "data/haarcascade_frontalface_alt.xml";//正面人臉瀑布偵測訓練數據
//放專案下："haarcascade_frontalface_alt.xml”(跟sln檔放一起)
//相對路徑："data/haarcascade_frontalface_alt.xml”(放在下一層的data檔案夾)
//絕對路徑："D: / AAA / BBB / CCC / haarcascade_frontalface_alt.xml”(任意位置)

CascadeClassifier face_cascade; //建立瀑布分類器物件

Mat im; //輸入影像
int c; 
int option=3, width, height;

//定義滑鼠反應函式 mouse_callback
static void mouse_callback(int event, int x, int y, int, void *)
{
	if (event == CV_EVENT_LBUTTONDOWN) {
		if (y>=425 && y<=460)
		{
			if (x>=55 && x<=150)
			{
				cout << "Blur" << endl;
				option = 1;
			}

			if (x >= 280 && x <= 355)
			{
				cout << "Sharpen" << endl;
				option = 2;
			}

			if (x >= 470 && x <= 585)
			{
				cout << "Clear" << endl;
				option = 3;
			}
		}
	}
	// 當滑鼠按下左鍵，根據點選位置，得到選項 (option) 數值 
//....
}

/**
 * @function main
 */
int main( void )
{    
	//VideoCapture cap("data/sleepy.mpg"); //讀取影片或相機
	VideoCapture cap(0);
	//放專案下："sleepy.mpg”(跟sln檔放一起)
	//相對路徑："data/sleepy.mpg”(放在下一層的data檔案夾)
	//絕對路徑："D: / AAA / BBB / CCC / sleepy.mpg”(任意位置)
	
	if (!cap.isOpened()) return 0; //不能讀視訊的處理

	//前處理
	int width = cap.get(CAP_PROP_FRAME_WIDTH);//讀取視訊的寬 width
	int height = cap.get(CAP_PROP_FRAME_HEIGHT);//讀取視訊的高 height
	cout << "影像寬=" << width << endl;//顯示視訊的寬 width
	cout << "影像高=" << height << endl;//顯示視訊的高 height


    //匯入人臉瀑布偵測訓練數據
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); waitKey(0); return -1; };

	while (char(waitKey(1)) != 27 && cap.isOpened()) //當鍵盤沒按 Esc，以及視訊物件成功開啟時，持續執行 while 迴圈
    {
		cap >> im; //抓取視訊的畫面
        if( im.empty() ) //如果沒抓到 畫面
        {
            printf(" --(!) No captured im -- Break!");  //顯示錯誤訊息
            break;
        }
		//定義視窗名稱 namedWindow
		namedWindow("window");

		//設定滑鼠反應函式 setMouseCallback
		setMouseCallback("window", mouse_callback);
        
		/*偵測人臉，並顯示AR圖像融合結果*/
        detectAndDisplay();
    }
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(void)
{
	/*人臉偵測部分*/
	vector<Rect> faces; //建立人臉ROI 向量
	Mat im_gray; //灰階影像物件

	cvtColor(im, im_gray, COLOR_BGR2GRAY); //彩色影像轉灰階
	equalizeHist(im_gray, im_gray); //灰階值方圖等化(對比自動增強)。若視訊品質好，可不用

	//人臉瀑布偵測
	face_cascade.detectMultiScale(im_gray, faces, 1.1, 4, 0, Size(80, 80));

	//獲得最大人臉的 ROI數據
	if (faces.size() > 0) {
		int largest_area = -999;
		int largest_i;
		for (int i = 0; i < faces.size(); i++) //用迴圈讀取所有人臉 ROI
		{
			//定義影像中的 ROI
			if (largest_area < faces[i].height)
			{
				largest_area = faces[i].height;
				largest_i = i;
			}
		}

		Rect faceROI = faces[largest_i]; //將最大人臉的 ROI數據存入 faceROI

		// 稍為放大ROI，使新臉能夠完整覆蓋視訊中的人臉(可不做)
		int d = 25;
		faceROI.x = faceROI.x - d;
		faceROI.y = faceROI.y - d;
		faceROI.width = faceROI.width + 2 * d;
		faceROI.height = faceROI.height + 2 * d;

		// 繪製人臉區域矩形框  (point左上 & point 對角)
		rectangle(im, Point(faceROI.x, faceROI.y), Point(faceROI.x+faceROI.width, faceROI.y+faceROI.height), Scalar(175, 5, 39), 2, 8, 0);
		putText(im, string("M10715068"), Point(faceROI.x-5, faceROI.y-5), 0, 1, Scalar(175, 5, 39), 2);
		putText(im, string("M10715068"), Point(faceROI.x-7, faceROI.y-7), 0, 1, Scalar(215, 215, 215), 2);
		//繪製window下方的選項文字
		putText(im, string("Blur"), Point(im.cols*0.1, im.rows*0.95), 0, 1, Scalar(175, 25, 39), 2);
		putText(im, string("Blur"), Point(im.cols*0.1-2, im.rows*0.95-2), 0, 1, Scalar(215, 215, 215), 2);

		putText(im, string("Sharp"), Point(im.cols*0.4, im.rows*0.95), 0, 1, Scalar(175, 25, 39), 2);
		putText(im, string("Sharp"), Point(im.cols*0.4-2, im.rows*0.95 - 2), 0, 1, Scalar(215, 215, 215), 2);

		putText(im, string("Clear"), Point(im.cols*0.75, im.rows*0.95), 0, 1, Scalar(175, 25, 39), 2);
		putText(im, string("Clear"), Point(im.cols*0.75-2, im.rows*0.95 - 2), 0, 1, Scalar(215, 215, 215), 2);
		

		//根據 option 選項，處理ROI影像
		if (option == 1) // Blur
		{
			Mat blur_im;
			medianBlur(im, blur_im, 11);

			Mat ROIimage = im(cv::Rect(faceROI.x, faceROI.y, faceROI.width, faceROI.height));
			blur_im = blur_im(cv::Rect(faceROI.x, faceROI.y, faceROI.width, faceROI.height));
			blur_im.copyTo(ROIimage); // 把bitwise_img copy 進ROIimage，就等同於copy到local_src中相對的記憶體位置
			putText(im, string("_____"), Point(im.cols*0.1, im.rows*0.95+5), 0, 1, Scalar(0, 0, 255), 2);
		}
		if (option == 2)
		{
			Mat bimage;
			cv::GaussianBlur(im, bimage, cv::Size(0, 0), 3);
			cv::addWeighted(im, 3, bimage, -2, 0, bimage);

			Mat ROIimage = im(cv::Rect(faceROI.x, faceROI.y, faceROI.width, faceROI.height));
			bimage = bimage(cv::Rect(faceROI.x, faceROI.y, faceROI.width, faceROI.height));
			bimage.copyTo(ROIimage);
			putText(im, string("_____"), Point(im.cols*0.4, im.rows*0.95 + 5), 0, 1, Scalar(0, 0, 255), 2);
		}

		if (option == 3) 
		{
			putText(im, string("_____"), Point(im.cols*0.75, im.rows*0.95+5), 0, 1, Scalar(0, 0,255), 2);
		}
		
	}
	//顯示影像
	imshow("window", im);
}

