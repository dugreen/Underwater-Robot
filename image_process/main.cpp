#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "iostream"
#include <stdio.h>

using namespace cv;
using namespace std;

//-----------------------------------【全局变量声明部分】--------------------------------------
//		描述：全局变量的声明
//-----------------------------------------------------------------------------------------------
int g_nThresholdValue = 25;
int g_nThresholdType = 0;

//-----------------------------------【全局函数声明部分】--------------------------------------
//		描述：全局函数的声明
//-----------------------------------------------------------------------------------------------
static void ShowHelpText();//输出帮助文字

void drawDetectLines(Mat& frame, const vector<Vec4i>& lines, Scalar & color)
{
	// 将检测到的直线在图上画出来
	vector<Vec4i>::const_iterator it = lines.begin();
	while (it != lines.end())
	{
		Point pt1((*it)[0], (*it)[1]);
		Point pt2((*it)[2], (*it)[3]);
		line(frame, pt1, pt2, color, 2); //  线条宽度设置为2
		++it;
	}
}

void main()
{
	char str[25];
	int i = 0;
	//读入视频
	VideoCapture capture;
	//capture.open(0);
	capture.open("url")
	//灰度图像
	Mat edges;

	vector<int>compression_params;

	//此句代码的OpenCV3版为：
	compression_params.push_back(IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
		
	//循环显示每一帧
	for (int j = 1;; j++)
	{
		//定义一个Mat变量，用于存储每一帧的图像
		Mat frame;
		//读取当前帧
		capture >> frame;
		//若视频播放完成，退出循环
		if (frame.empty())
		{
			break;
		}
		if (j % 2 == 0)
		{
			//图片存储
			imwrite("原图" + (String)str + ".png", frame, compression_params);

			//显示当前帧
			imshow("读取视频", frame);

			//转换灰度图
			Mat dstImage;
			cvtColor(frame, dstImage, CV_BGR2GRAY);
			//imwrite("灰度" + (String)str + ".png", dstImage, compression_params);
			//进行canny处理
			Mat contours;
			Canny(dstImage, contours, 125, 350);
			threshold(contours, contours, 128, 255, THRESH_BINARY);
			//imwrite("Canny图" + (String)str + ".png", contours, compression_params);
			vector<Vec4i> lines;
			// 检测直线，最小投票为90，线条不短于50，间隙不小于10
			HoughLinesP(contours, lines, 1, CV_PI / 180, 80, 50, 10);



			//筛选可疑的图片，并进行存储
			int numbers = lines.size();
			
			if (numbers > 0)
			{
				drawDetectLines(frame, lines, Scalar(0, 255, 0));
				imwrite("可疑图" + (String)str + ".png", frame, compression_params);
			}
			
			waitKey(20);
			++i;
			sprintf(str, "%d", i);
		}	
	}

}
