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
	// ������ֵ�ָ����̬ѧ����
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
	// ȡ��������
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
	// �����������������ķָ��
	Mat gray;
	if (src.channels() > 1) {
		cvtColor(src, gray, COLOR_BGR2GRAY);
	}
	else {
		gray = src.clone();
	}
	threshold(src, gray, 50, 255, THRESH_TOZERO_INV);
	threshold(gray, gray, 23, 255, THRESH_BINARY);
	// ��gray��ʴ
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	erode(gray, gray, element);
	erode(gray, gray, element);
	// ��gray��������
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
	// ����Ӧ��ֵ�ָ����ͨ�������
	// ����Ӧ��ֵ�ָ�
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
	//// ��ͨ�������
	//Mat labels, stats, centroids;
	//int nLabels = connectedComponentsWithStats(gray, labels, stats, centroids);
	//// ȡ������ͨ���
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
	//// ��������ͨ�������
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
	//// Canny��Ե������̬ѧ����
	//Mat gray;
	//if (src.channels() > 1) {
	//	cvtColor(src, gray, COLOR_BGR2GRAY);
	//}
	//else {
	//	gray = src.clone();
	//}

	//// ʹ��canny���Ӽ���Ե
	//Mat edges;
	//Canny(gray, edges, 10, 100);
	////// ʹ����̬ѧ������ǿ��Ե
	////Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	////dilate(edges, edges, kernel);
	//dst = edges;
	Mat binary;
	threshold(src, binary, 28, 255, THRESH_TOZERO);
	threshold(binary, binary, 50, 255, THRESH_TOZERO_INV);
	threshold(binary, binary, 1, 255, THRESH_BINARY);
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	morphologyEx(binary, binary, MORPH_OPEN, kernel);
	//// ���׶�
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

	// ���������С��100ʱ������ɾ��
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
	//// ȡ���ڲ������
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
	// ȡ��������
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

	// ���������������ѡȡһ������Ϊ���ӵ�
	RNG rng(12345);
	int seed = rng.uniform(0, contours[maxIndex].size());
	Point seedPoint = contours[maxIndex][seed];

	// ��������
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

	// ��ֵ��
	Mat binary;
	threshold(gray, binary, 48, 255, THRESH_BINARY);

	// ����任
	Mat dist;
	distanceTransform(binary, dist, DIST_L2, 3, 5);

	// ��һ��
	normalize(dist, dist, 0, 1, NORM_MINMAX);

	// Ѱ�����ӵ�
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

	// ��ˮ���㷨
	Mat markers = Mat::zeros(src.size(), CV_32S);
	for (size_t i = 0; i < seeds.size(); i++)
	{
		circle(markers, seeds[i], 5, Scalar(i + 1), -1);
	}
	watershed(src, markers);

	// ��ʾ���
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


	//�ؽ�mat
	if (1 == nChannels)
	{
		cvimg.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		cvimg.create(nHeight, nWidth, CV_8UC3);
	}

	//��������
	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)dib.GetBits();		//ָ����������ָ��
	int nStep = dib.GetPitch();						//ÿ�е��ֽ���,ע���������ֵ�����и�

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

	//�ؽ�CDib
	dib.Destroy();
	dib.Create(nWidth, nHeight, 8 * nChannels);

	//��������
	uchar* pucRow;									//ָ������������ָ��
	uchar* pucImage = (uchar*)dib.GetBits();		//ָ����������ָ��
	int nStep = dib.GetPitch();					//ÿ�е��ֽ���,ע���������ֵ�����и�
	dib.m_nWidth = nWidth;
	dib.m_nHeight = nHeight;
	dib.m_nWidthBytes = abs(nStep);
	dib.m_nBitCount = 8 * nChannels;
	dib.m_pDibBits = (unsigned char*)dib.GetBits() + (nHeight - 1) * dib.GetPitch();

	if (1 == nChannels)								//���ڵ�ͨ����ͼ����Ҫ��ʼ����ɫ��
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
//	// ��ԭͼ���ȥ��ʴͼ��õ���Ե
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
//	Mat imgErode;
//	erode(cvimg, imgErode, element);
//	cvimg = cvimg - imgErode;
//
//}
//
//void COpenCVProcess::OpenCVFindContours()
//{
//	// ��ԭ��ɫͼ������ȡ����������ɫ����
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
