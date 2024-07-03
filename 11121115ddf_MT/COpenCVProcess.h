#pragma once
//--------------------------------------------------------------
//COpenCVProcess类,用于OpenCV图像处理
//--------------------------------------------------------------
using namespace cv;
class CDib;

class COpenCVProcess
{
public:
	COpenCVProcess(CDib* pDib);
	~COpenCVProcess(void);

public:
	Mat cvimg, cvimgbuffer;

public:
	void SegmentFlair(Mat& src, Mat& dst);
	void SegmentT1(Mat& src, Mat& dst);
	void SegmentT1ce(Mat& src, Mat& dst);
	void SegmentT2(Mat& src, Mat& dst);
	void DrawTumorContours(Mat& src, Mat& dst);

private:
	// 分割方法

	void Segment_Threshold(Mat& src, Mat& dst);
	void Segment_Canny(Mat& src, Mat& dst);
	void RegionGrowing(Mat& src, Mat& dst);
	void OtsuThresholding(Mat& src, Mat& dst);
	void WatershedSegmentation(Mat& src, Mat& dst);
	void KMeansSegmentation(Mat& src, Mat& dst, int K = 2);
	void GrabCutSegmentation(Mat& src, Mat& dst);

public:
	//transfering between CDib and cv::Mat
	void Dib2Mat(CDib& dib);
	void Mat2Dib(CDib& dib);
};

