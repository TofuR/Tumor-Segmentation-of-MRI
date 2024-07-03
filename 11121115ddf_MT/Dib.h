#pragma once
#include "atlimage.h"

#include <complex>
#include "11121115ddf_MTDoc.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#define Pi 3.1415926535897932354626
#define Ei 2.71828

// CDib���ڶ�ȡһ��16λraw�ļ�������8λ��ʾ���е�ͼ��
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
		// �����ָ��
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
	int m_current_img_pos;	// ��ǰ��ʾ��λ�ã�0�����1ʸ״2��״

public:
	// ������ʾͼ��
	unsigned char* m_pDibBits;	// ָ��һ��dib��ָ��
	int m_nWidthBytes;	// ÿ�е��ֽ�����240*16��
	long m_nWidth;	// ��ʾ��ͼ���ȣ�240��
	long m_nHeight;	// ��ʾ��ͼ��߶ȣ�240��
	int m_nBitCount;	// ��ʾ��ÿ�����ص�λ����8/24��

public:
	// ���ڴ洢ͼ��
	uint16_t* m_pRawBuffer;	// ָ��һ��raw�ļ���ָ��
	int m_nRawWidth;	// ԭʼͼ���ȣ�240��
	int m_nRawHeight;	// ԭʼͼ��߶ȣ�240��
	int m_nRawNum;	// ԭʼraw�ļ�������1��
	int m_nImgNum;	// һ��raw�ļ��е�ͼ��������155��

private:
	long* m_pGrayValueCount;
};

