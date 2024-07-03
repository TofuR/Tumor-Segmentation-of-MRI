#pragma once
#include "atlimage.h"

#include <complex>
#include "11121115ddf_MTDoc.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#define Pi 3.1415926535897932354626
#define Ei 2.71828

// CDib用于读取一个16位raw文件，并以8位显示其中的图像
class CDib :
	public CImage
{
public:
	CDib(void);
	CDib(CDib& Dib);
	~CDib(void);

public:
	enum chan_color { chan_red = 1, chan_green, chan_blue, chan_black };
	enum HSIChannel { HUE = 0, SATURATION, INTENSITY };
	enum current_img_pos { transverse = 0, sagittal, coronal };
	enum SegmentMethod {
		NO_SEGMENTATION = 0,
		FLAIR_SEGMENTATION,
		T1_SEGMENTATION,
		T1CE_SEGMENTATION,
		T2_SEGMENTATION
		// 其他分割方法
	};

public:
	int GetImgNum() const;
	int getWidth() const;
	int getHeight() const;
	unsigned char* GetDibBits() const;
	uint16_t* GetRawBuffer() const;

public:
	void BilinearInterpolate(uint16_t* bilinear, uint16_t* normal, int width, int height, float ratio);
	long* GrayValueCount();

public:
	void ShowTransversePlane(int CurImgNum);
	void ShowCoronalPlane(int CurImgNum);
	void ShowSagittalPlane(int CurImgNum);

	void ShowNextPage();
	void ShowFormerPage();

public:
	void LoadFile(LPCTSTR, int FileNum, int ImageHeight, int ImageWidth);
	void CreateDisplayDib(uint16_t*, int, int, int);
	void UpdateDisplayDib(int ImgNum);

public:
	void Segmentation(SegmentMethod method);
	void Recover();

public:
	SegmentMethod m_SegmentMethod;
	bool m_IsSegmented;

public:
	int m_nCurrentImgNum_Transverse;
	int m_nCurrentImgNum_Sagittal;
	int m_nCurrentImgNum_Coronal;
	int m_current_img_pos;	// 当前显示的位置：0横截面1矢状2冠状

public:
	// 用于显示图像
	unsigned char* m_pDibBits;	// 指向一个dib的指针
	int m_nWidthBytes;	// 每行的字节数（240*16）
	long m_nWidth;	// 显示的图像宽度（240）
	long m_nHeight;	// 显示的图像高度（240）
	int m_nBitCount;	// 显示的每个像素的位数（8/24）

public:
	// 用于存储图像
	uint16_t* m_pRawBuffer;	// 指向一个raw文件的指针
	int m_nRawWidth;	// 原始图像宽度（240）
	int m_nRawHeight;	// 原始图像高度（240）
	int m_nRawNum;	// 原始raw文件数量（1）
	int m_nImgNum;	// 一个raw文件中的图像数量（155）

private:
	long* m_pGrayValueCount;
};

