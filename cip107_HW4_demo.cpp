/*��m�v����@�޳N_�@�~4: �H�y�ϰ�B�z*/
//#include "stdafx.h" //�p�G�M�׼��Y�ɸ̸̦�stdafx.h�A��l�{���ɦ�stdafx.cpp�A�ݥ[�J�o�@��C�p�G�O�űM�סA�h���Υ[�o�@��C
#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>

using namespace cv; //�ŧi opencv �禡�w���R�W�Ŷ�
using namespace std; //�ŧi C++�禡�w���R�W�Ŷ�

/** Function Headers */
void detectAndDisplay(void); //�����H�y���禡�ŧi

/** Global variables */
String face_cascade_name = "data/haarcascade_frontalface_alt.xml";//�����H�y�r�������V�m�ƾ�
//��M�פU�G"haarcascade_frontalface_alt.xml��(��sln�ɩ�@�_)
//�۹���|�G"data/haarcascade_frontalface_alt.xml��(��b�U�@�h��data�ɮק�)
//������|�G"D: / AAA / BBB / CCC / haarcascade_frontalface_alt.xml��(���N��m)

CascadeClassifier face_cascade; //�إ��r������������

Mat im; //��J�v��
int c; 
int option=3, width, height;

//�w�q�ƹ������禡 mouse_callback
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
	// ��ƹ����U����A�ھ��I���m�A�o��ﶵ (option) �ƭ� 
//....
}

/**
 * @function main
 */
int main( void )
{    
	//VideoCapture cap("data/sleepy.mpg"); //Ū���v���ά۾�
	VideoCapture cap(0);
	//��M�פU�G"sleepy.mpg��(��sln�ɩ�@�_)
	//�۹���|�G"data/sleepy.mpg��(��b�U�@�h��data�ɮק�)
	//������|�G"D: / AAA / BBB / CCC / sleepy.mpg��(���N��m)
	
	if (!cap.isOpened()) return 0; //����Ū���T���B�z

	//�e�B�z
	int width = cap.get(CAP_PROP_FRAME_WIDTH);//Ū�����T���e width
	int height = cap.get(CAP_PROP_FRAME_HEIGHT);//Ū�����T���� height
	cout << "�v���e=" << width << endl;//��ܵ��T���e width
	cout << "�v����=" << height << endl;//��ܵ��T���� height


    //�פJ�H�y�r�������V�m�ƾ�
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); waitKey(0); return -1; };

	while (char(waitKey(1)) != 27 && cap.isOpened()) //����L�S�� Esc�A�H�ε��T���󦨥\�}�ҮɡA������� while �j��
    {
		cap >> im; //������T���e��
        if( im.empty() ) //�p�G�S��� �e��
        {
            printf(" --(!) No captured im -- Break!");  //��ܿ��~�T��
            break;
        }
		//�w�q�����W�� namedWindow
		namedWindow("window");

		//�]�w�ƹ������禡 setMouseCallback
		setMouseCallback("window", mouse_callback);
        
		/*�����H�y�A�����AR�Ϲ��ĦX���G*/
        detectAndDisplay();
    }
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(void)
{
	/*�H�y��������*/
	vector<Rect> faces; //�إߤH�yROI �V�q
	Mat im_gray; //�Ƕ��v������

	cvtColor(im, im_gray, COLOR_BGR2GRAY); //�m��v����Ƕ�
	equalizeHist(im_gray, im_gray); //�Ƕ��Ȥ�ϵ���(���۰ʼW�j)�C�Y���T�~��n�A�i����

	//�H�y�r������
	face_cascade.detectMultiScale(im_gray, faces, 1.1, 4, 0, Size(80, 80));

	//��o�̤j�H�y�� ROI�ƾ�
	if (faces.size() > 0) {
		int largest_area = -999;
		int largest_i;
		for (int i = 0; i < faces.size(); i++) //�ΰj��Ū���Ҧ��H�y ROI
		{
			//�w�q�v������ ROI
			if (largest_area < faces[i].height)
			{
				largest_area = faces[i].height;
				largest_i = i;
			}
		}

		Rect faceROI = faces[largest_i]; //�N�̤j�H�y�� ROI�ƾڦs�J faceROI

		// �y����jROI�A�Ϸs�y��������л\���T�����H�y(�i����)
		int d = 25;
		faceROI.x = faceROI.x - d;
		faceROI.y = faceROI.y - d;
		faceROI.width = faceROI.width + 2 * d;
		faceROI.height = faceROI.height + 2 * d;

		// ø�s�H�y�ϰ�x�ή�  (point���W & point �﨤)
		rectangle(im, Point(faceROI.x, faceROI.y), Point(faceROI.x+faceROI.width, faceROI.y+faceROI.height), Scalar(175, 5, 39), 2, 8, 0);
		putText(im, string("M10715068"), Point(faceROI.x-5, faceROI.y-5), 0, 1, Scalar(175, 5, 39), 2);
		putText(im, string("M10715068"), Point(faceROI.x-7, faceROI.y-7), 0, 1, Scalar(215, 215, 215), 2);
		//ø�swindow�U�誺�ﶵ��r
		putText(im, string("Blur"), Point(im.cols*0.1, im.rows*0.95), 0, 1, Scalar(175, 25, 39), 2);
		putText(im, string("Blur"), Point(im.cols*0.1-2, im.rows*0.95-2), 0, 1, Scalar(215, 215, 215), 2);

		putText(im, string("Sharp"), Point(im.cols*0.4, im.rows*0.95), 0, 1, Scalar(175, 25, 39), 2);
		putText(im, string("Sharp"), Point(im.cols*0.4-2, im.rows*0.95 - 2), 0, 1, Scalar(215, 215, 215), 2);

		putText(im, string("Clear"), Point(im.cols*0.75, im.rows*0.95), 0, 1, Scalar(175, 25, 39), 2);
		putText(im, string("Clear"), Point(im.cols*0.75-2, im.rows*0.95 - 2), 0, 1, Scalar(215, 215, 215), 2);
		

		//�ھ� option �ﶵ�A�B�zROI�v��
		if (option == 1) // Blur
		{
			Mat blur_im;
			medianBlur(im, blur_im, 11);

			Mat ROIimage = im(cv::Rect(faceROI.x, faceROI.y, faceROI.width, faceROI.height));
			blur_im = blur_im(cv::Rect(faceROI.x, faceROI.y, faceROI.width, faceROI.height));
			blur_im.copyTo(ROIimage); // ��bitwise_img copy �iROIimage�A�N���P��copy��local_src���۹諸�O�����m
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
	//��ܼv��
	imshow("window", im);
}

