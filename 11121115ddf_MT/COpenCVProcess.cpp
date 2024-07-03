#include "pch.h"
#include "COpenCVProcess.h"
#include "Dib.h"


COpenCVProcess::COpenCVProcess(CDib* pDib)
{
	Dib2Mat(*pDib);
	cvimgbuffer = cvimg.clone();
}

COpenCVProcess::~COpenCVProcess(void)
{
	cvimg.release();
	cvimgbuffer.release();
}

void COpenCVProcess::SegmentFlair(Mat& src, Mat& dst)
{
	// 基于阈值分割和形态学操作
	//Mat binary;
	//threshold(src, binary, 100, 255, THRESH_BINARY);
	//Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	//morphologyEx(binary, binary, MORPH_OPEN, kernel);
	//dst = binary;
	Mat binary;
	threshold(src, binary, 100, 255, THRESH_BINARY);
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	morphologyEx(binary, binary, MORPH_OPEN, kernel);

	if (binary.channels() == 3)
	{
		cvtColor(binary, binary, COLOR_BGR2GRAY);
	}

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// 取最大的轮廓
	int maxArea = 0;
	int maxIndex = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		if (area > maxArea)
		{
			maxArea = area;
			maxIndex = i;
		}
	}

	dst = Mat::zeros(src.size(), CV_8UC3) + dst;
	drawContours(dst, contours, maxIndex, Scalar(0, 255, 0), 2);
}

void COpenCVProcess::SegmentT1(Mat& src, Mat& dst)
{
	// 区域生长或基于纹理的分割方法
	Mat gray;
	if (src.channels() > 1) {
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}
	else {
		gray = src.clone();
	}
	threshold(src, gray, 50, 255, THRESH_TOZERO_INV);
	threshold(gray, gray, 23, 255, THRESH_BINARY);
	// 对gray腐蚀
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	erode(gray, gray, element);
	erode(gray, gray, element);
	// 对gray进行膨胀
	dilate(gray, gray, element);
	dilate(gray, gray, element);

	//dst = gray;
	if (gray.channels() == 3)
	{
		cvtColor(gray, gray, COLOR_BGR2GRAY);
	}
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(gray, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	int maxArea = 0;
	int maxIndex = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		if (area > maxArea)
		{
			maxArea = area;
			maxIndex = i;
		}
	}
	dst = Mat::zeros(src.size(), CV_8UC3) + dst;
	drawContours(dst, contours, maxIndex, Scalar(0, 255, 0), 2);

	//Mat binary;
	//threshold(src, binary, 48, 255, THRESH_TOZERO_INV);
	//threshold(binary, binary, 35, 255, THRESH_BINARY);
	//Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	//morphologyEx(binary, binary, MORPH_OPEN, kernel);

	//if (binary.channels() == 3)
	//{
	//	cvtColor(binary, binary, COLOR_BGR2GRAY);
	//}
	//vector<vector<Point>> contours;
	//vector<Vec4i> hierarchy;
	//findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//int maxArea = 0;
	//int maxIndex = 0;
	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	int area = contourArea(contours[i]);
	//	if (area > maxArea)
	//	{
	//		maxArea = area;
	//		maxIndex = i;
	//	}
	//}
	//dst = Mat::zeros(src.size(), CV_8UC3) + dst;
	//drawContours(dst, contours, maxIndex, Scalar(0, 255, 0), 2);
}

void COpenCVProcess::SegmentT1ce(Mat& src, Mat& dst)
{
	// 自适应阈值分割和连通组件分析
	// 自适应阈值分割
	Mat gray;
	if (src.channels() > 1) {
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}
	else {
		gray = src.clone();
	}
	threshold(gray, gray, 115, 255, THRESH_TOZERO_INV);
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	GaussianBlur(gray, gray, Size(5, 5), 0, 0, BORDER_DEFAULT);
	//adaptiveThreshold(gray, gray, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 2);
	threshold(gray, gray, 48, 255, THRESH_BINARY);
	erode(gray, gray, element);
	erode(gray, gray, element);
	erode(gray, gray, element);
	dilate(gray, gray, element);
	//dst = gray;

	if (gray.channels() == 3)
	{
		cvtColor(gray, gray, COLOR_BGR2GRAY);
	}
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(gray, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	int maxArea = 0;
	int maxIndex = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		if (area > maxArea)
		{
			maxArea = area;
			maxIndex = i;
		}
	}
	dst = Mat::zeros(src.size(), CV_8UC3) + dst;
	drawContours(dst, contours, maxIndex, Scalar(0, 255, 0), 2);

	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	Scalar color = Scalar(0, 255, 0);
	//	drawContours(dst, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
	//}
	//threshold(gray, gray, 50, 255, THRESH_TOZERO_INV);
	//adaptiveThreshold(gray, gray, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 2);
	////dst = gray;
	//// 连通组件分析
	//Mat labels, stats, centroids;
	//int nLabels = connectedComponentsWithStats(gray, labels, stats, centroids);
	//// 取最大的连通组件
	//int maxArea = 0;
	//int maxIndex = 0;
	//for (int i = 1; i < nLabels; i++)
	//{
	//	int area = stats.at<int>(i, CC_STAT_AREA);
	//	if (area > maxArea)
	//	{
	//		maxArea = area;
	//		maxIndex = i;
	//	}
	//}
	//// 将最大的连通组件画出
	//dst = Mat::zeros(src.size(), CV_8UC3) + dst;
	//for (int i = 0; i < gray.rows; i++)
	//{
	//	for (int j = 0; j < gray.cols; j++)
	//	{
	//		if (labels.at<int>(i, j) == maxIndex)
	//		{
	//			dst.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
	//		}
	//	}
	//}
}

void COpenCVProcess::SegmentT2(Mat& src, Mat& dst)
{
	//// Canny边缘检测和形态学操作
	//Mat gray;
	//if (src.channels() > 1) {
	//	cvtColor(src, gray, COLOR_BGR2GRAY);
	//}
	//else {
	//	gray = src.clone();
	//}

	//// 使用canny算子检测边缘
	//Mat edges;
	//Canny(gray, edges, 10, 100);
	////// 使用形态学操作增强边缘
	////Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	////dilate(edges, edges, kernel);
	//dst = edges;
	Mat binary;
	threshold(src, binary, 28, 255, THRESH_TOZERO);
	threshold(binary, binary, 50, 255, THRESH_TOZERO_INV);
	threshold(binary, binary, 1, 255, THRESH_BINARY);
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	morphologyEx(binary, binary, MORPH_OPEN, kernel);
	//// 填充孔洞
	//Mat::zeros(binary.size(), CV_8UC1);
	//floodFill(binary, Point(0, 0), Scalar(255), 0, Scalar(10), Scalar(10), 4);

	if (binary.channels() == 3)
	{
		cvtColor(binary, binary, COLOR_BGR2GRAY);
	}
	//dst = binary;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// 当轮廓面积小于100时，将其删除
	for (size_t i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		if (area < 150)
		{
			contours.erase(contours.begin() + i);
			i--;
		}
	}
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(0, 255, 0);
		drawContours(dst, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
	}
}

void COpenCVProcess::DrawTumorContours(Mat& src, Mat& dst)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(src, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//// 取最内层的轮廓
	//int maxArea = 0;
	//int maxIndex = 0;
	//for (size_t i = 0; i < contours.size(); i++)
	//{
	//	int area = contourArea(contours[i]);
	//	if (area > maxArea)
	//	{
	//		maxArea = area;
	//		maxIndex = i;
	//	}
	//}

	//dst = Mat::zeros(src.size(), CV_8UC3) + dst;
	//drawContours(dst, contours, maxIndex, Scalar(0, 255, 0), 2);

	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(0, 255, 0);
		drawContours(dst, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
	}
}


void COpenCVProcess::Segment_Threshold(Mat& src, Mat& dst)
{
	Mat gray;
	if (src.channels() > 1) {
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}
	else {
		gray = src.clone();
	}

	threshold(gray, dst, 200, 255, THRESH_BINARY);
}

void COpenCVProcess::Segment_Canny(Mat& src, Mat& dst)
{
	Mat gray;
	if (src.channels() > 1) {
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}
	else {
		gray = src.clone();
	}

	Canny(gray, dst, 100, 200);
}

void COpenCVProcess::RegionGrowing(Mat& src, Mat& dst)
{
	Mat gray;
	if (src.channels() > 1) {
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}
	else {
		gray = src.clone();
	}

	Mat binary;
	threshold(gray, binary, 100, 255, THRESH_BINARY);
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	morphologyEx(binary, binary, MORPH_OPEN, kernel);

	if (binary.channels() == 3)
	{
		cvtColor(binary, binary, COLOR_BGR2GRAY);
	}

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	// 取最大的轮廓
	int maxArea = 0;
	int maxIndex = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		if (area > maxArea)
		{
			maxArea = area;
			maxIndex = i;
		}
	}

	// 从最大的轮廓中随机选取一个点作为种子点
	RNG rng(12345);
	int seed = rng.uniform(0, contours[maxIndex].size());
	Point seedPoint = contours[maxIndex][seed];

	// 区域生长
	Mat mask = Mat::zeros(src.size(), CV_8UC1);
	floodFill(src, mask, seedPoint, Scalar(255), 0, Scalar(10), Scalar(10), 4);

	dst = mask;
}

void COpenCVProcess::OtsuThresholding(Mat& src, Mat& dst)
{
	Mat gray;
	if (src.channels() > 1) {
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}
	else {
		gray = src.clone();
	}

	threshold(gray, dst, 0, 255, THRESH_BINARY | THRESH_OTSU);
}

void COpenCVProcess::WatershedSegmentation(Mat& src, Mat& dst)
{
	Mat gray;
	if (src.channels() > 1) {
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}
	else {
		gray = src.clone();
	}

	// 二值化
	Mat binary;
	threshold(gray, binary, 48, 255, THRESH_BINARY);

	// 距离变换
	Mat dist;
	distanceTransform(binary, dist, DIST_L2, 3, 5);

	// 归一化
	normalize(dist, dist, 0, 1, NORM_MINMAX);

	// 寻找种子点
	Mat dist_8u;
	dist.convertTo(dist_8u, CV_8U);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(dist_8u, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<Point> seeds;
	for (size_t i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		if (area > 100)
		{
			seeds.push_back(contours[i][0]);
		}
	}

	// 分水岭算法
	Mat markers = Mat::zeros(src.size(), CV_32S);
	for (size_t i = 0; i < seeds.size(); i++)
	{
		circle(markers, seeds[i], 5, Scalar(i + 1), -1);
	}
	watershed(src, markers);

	// 显示结果
	dst = Mat::zeros(src.size(), CV_8UC3);
	for (int i = 0; i < markers.rows; i++)
	{
		for (int j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if (index == -1)
			{
				dst.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
			else if (index <= 0 || index > seeds.size())
			{
				dst.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
			else
			{
				dst.at<Vec3b>(i, j) = Vec3b(255 * (index - 1) / seeds.size(), 255 * (index - 1) / seeds.size(), 255 * (index - 1) / seeds.size());
			}
		}
	}
}

void COpenCVProcess::KMeansSegmentation(Mat& src, Mat& dst, int K)
{
	Mat samples(src.rows * src.cols, 3, CV_32F);
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
			for (int z = 0; z < 3; z++)
				samples.at<float>(y + x * src.rows, z) = src.at<Vec3b>(y, x)[z];

	Mat labels;
	Mat centers;
	kmeans(samples, K, labels, TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0), 3, KMEANS_PP_CENTERS, centers);

	dst = Mat::zeros(src.size(), src.type());
	for (int y = 0; y < src.rows; y++)
		for (int x = 0; x < src.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * src.rows, 0);
			dst.at<Vec3b>(y, x)[0] = centers.at<float>(cluster_idx, 0);
			dst.at<Vec3b>(y, x)[1] = centers.at<float>(cluster_idx, 1);
			dst.at<Vec3b>(y, x)[2] = centers.at<float>(cluster_idx, 2);
		}

}

void COpenCVProcess::GrabCutSegmentation(Mat& src, Mat& dst)
{
	Mat bgModel, fgModel;
	Rect rect(10, 10, src.cols - 20, src.rows - 20);
	Mat mask;

	grabCut(src, mask, rect, bgModel, fgModel, 5, GC_INIT_WITH_RECT);
	compare(mask, GC_PR_FGD, mask, CMP_EQ);
	dst = mask & 1;
}



void COpenCVProcess::Dib2Mat(CDib& dib)
{
	if (true == dib.IsNull())
	{
		return;
	}
	int nChannels = dib.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = dib.GetWidth();
	int nHeight = dib.GetHeight();


	//重建mat
	if (1 == nChannels)
	{
		cvimg.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		cvimg.create(nHeight, nWidth, CV_8UC3);
	}

	//拷贝数据
	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)dib.GetBits();		//指向数据区的指针
	int nStep = dib.GetPitch();						//每行的字节数,注意这个返回值有正有负

	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (cvimg.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}
}

void COpenCVProcess::Mat2Dib(CDib& dib)
{
	if (0 == cvimg.total())
	{
		return;
	}
	int nChannels = cvimg.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = cvimg.cols;
	int nHeight = cvimg.rows;

	//重建CDib
	dib.Destroy();
	dib.Create(nWidth, nHeight, 8 * nChannels);

	//拷贝数据
	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)dib.GetBits();		//指向数据区的指针
	int nStep = dib.GetPitch();					//每行的字节数,注意这个返回值有正有负
	dib.m_nWidth = nWidth;
	dib.m_nHeight = nHeight;
	dib.m_nWidthBytes = abs(nStep);
	dib.m_nBitCount = 8 * nChannels;
	dib.m_pDibBits = (unsigned char*)dib.GetBits() + (nHeight - 1) * dib.GetPitch();

	if (1 == nChannels)								//对于单通道的图像需要初始化调色板
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		dib.GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		dib.SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}

	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (cvimg.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}
}


//void COpenCVProcess::OpenCVGaussianBlur()
//{
//	//Mat tmp;
//	GaussianBlur(cvimg, cvimg, Size(7, 7), 0, 0, BORDER_DEFAULT);
//	//cvimg = tmp;
//}
//
//void COpenCVProcess::OpenCVBinarization(int nThreshold)
//{
//	cvtColor(cvimg, cvimg, CV_BGR2GRAY);
//	threshold(cvimg, cvimg, nThreshold, 255, CV_THRESH_BINARY);
//}
//
//void COpenCVProcess::OpenCVInvert()
//{
//	cvimg = 255 - cvimg;
//}
//
//void COpenCVProcess::OpenCVErode()
//{
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	erode(cvimg, cvimg, element);
//}
//
//void COpenCVProcess::OpenCVDilate()
//{
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	dilate(cvimg, cvimg, element);
//}
//
//void COpenCVProcess::OpenCVEdge()
//{
//	// 用原图像减去腐蚀图像得到边缘
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	Mat imgErode;
//	erode(cvimg, imgErode, element);
//	cvimg = cvimg - imgErode;
//
//}
//
//void COpenCVProcess::OpenCVFindContours()
//{
//	// 将原彩色图像中提取的轮廓用绿色画出
//	Mat gray;
//	if (cvimg.channels() > 1) {
//		cvtColor(cvimg, gray, COLOR_BGR2GRAY);
//	}
//	else {
//		gray = cvimg.clone();
//	}
//
//	threshold(gray, gray, 218, 255, THRESH_BINARY);
//
//	vector<vector<Point>> contours;
//	vector<Vec4i> hierarchy;
//	findContours(gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
//	for (size_t i = 0; i < contours.size(); i++)
//	{
//		Scalar color = Scalar(0, 255, 0);
//		drawContours(cvimg, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
//	}
//}
