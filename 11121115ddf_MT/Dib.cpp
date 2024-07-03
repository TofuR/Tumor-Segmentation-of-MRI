#include "pch.h"
#include "Dib.h"
#include "COpenCVProcess.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <opencv2\opencv.hpp>

using namespace std;

CDib::CDib(void) :m_pDibBits(NULL), m_pRawBuffer(NULL), m_pGrayValueCount(NULL)
{
	// initialized variables
	m_nBitCount = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nWidthBytes = 0;

	m_nRawWidth = 0;
	m_nRawHeight = 0;
	m_nRawNum = 0;
	m_nImgNum = 0;

	m_nCurrentImgNum_Sagittal = 0;
	m_nCurrentImgNum_Transverse = 0;
	m_nCurrentImgNum_Coronal = 0;
	m_current_img_pos = 0;

	m_IsSegmented = false;
	m_SegmentMethod = NO_SEGMENTATION;
}

CDib::CDib(CDib& Dib) :m_pDibBits(NULL), m_pRawBuffer(NULL), m_pGrayValueCount(NULL)
{
	// initialized variables
	m_nBitCount = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nWidthBytes = 0;

	m_nRawWidth = 0;
	m_nRawHeight = 0;
	m_nRawNum = 0;
	m_nImgNum = 0;

	m_nCurrentImgNum_Sagittal = 0;
	m_nCurrentImgNum_Transverse = 0;
	m_nCurrentImgNum_Coronal = 0;
	m_current_img_pos = 0;

	m_IsSegmented = false;
	m_SegmentMethod = NO_SEGMENTATION;

	if (&Dib == NULL)
	{
		return;
	}
	if (!IsNull())
	{
		Destroy();
	}
	Create(Dib.GetWidth(), Dib.GetHeight(), Dib.GetBPP(), 0);
	m_nWidth = Dib.m_nWidth;
	m_nHeight = Dib.m_nHeight;
	if (IsIndexed())
	{
		int nColors = Dib.GetMaxColorTableEntries();
		if (nColors > 0)
		{
			RGBQUAD* pal = new RGBQUAD[nColors];
			Dib.GetColorTable(0, nColors, pal);	//调色板
			SetColorTable(0, nColors, pal);
			delete[] pal;
		}
	}
	m_nWidthBytes = abs(GetPitch());
	m_nBitCount = GetBPP();
	m_nImgNum = Dib.GetImgNum();

	m_nRawWidth = Dib.m_nRawWidth;
	m_nRawHeight = Dib.m_nRawHeight;
	m_nRawNum = Dib.m_nRawNum;

	m_nCurrentImgNum_Sagittal = Dib.m_nCurrentImgNum_Sagittal;
	m_nCurrentImgNum_Transverse = Dib.m_nCurrentImgNum_Transverse;
	m_nCurrentImgNum_Coronal = Dib.m_nCurrentImgNum_Coronal;
	m_current_img_pos = Dib.m_current_img_pos;

	m_IsSegmented = Dib.m_IsSegmented;
	m_SegmentMethod = Dib.m_SegmentMethod;
	//memcpy(m_pDibBits, Dib.m_pDibBits, m_nHeight * m_nWidthBytes);
	if (Dib.m_pDibBits)
	{
		//m_pDibBits = new unsigned char[m_nHeight * m_nWidthBytes];
		m_pDibBits = (unsigned char*)GetBits() + (m_nHeight - 1) * GetPitch();
		memcpy(m_pDibBits, Dib.m_pDibBits, m_nHeight * m_nWidthBytes);
	}
	if (Dib.m_pRawBuffer != NULL)
	{
		m_pRawBuffer = new uint16_t[m_nWidth * m_nHeight * m_nImgNum];
		memcpy(m_pRawBuffer, Dib.m_pRawBuffer, m_nWidth * m_nHeight * m_nImgNum * sizeof(uint16_t));
	}
}

CDib::~CDib(void)
{
	m_pDibBits = NULL;
	if (m_pGrayValueCount != NULL)
	{
		delete[] m_pGrayValueCount;
		m_pGrayValueCount = NULL;
	}
	if (m_pRawBuffer != NULL)
	{
		delete[] m_pRawBuffer;
		m_pRawBuffer = NULL;
	}
}

int CDib::getWidth() const
{
	return m_nWidth;
}

int CDib::getHeight() const
{
	return m_nHeight;
}

int CDib::GetImgNum()  const
{
	return m_nImgNum;
}

unsigned char* CDib::GetDibBits() const
{
	return m_pDibBits;
}

uint16_t* CDib::GetRawBuffer() const
{
	return m_pRawBuffer;
}


void CDib::LoadFile(LPCTSTR lpszPathName, int ImgNum, int ImgHeight, int ImgWidth)
{
	//检查文件名

	CString strPathName(lpszPathName);
	int nDotIndex = strPathName.ReverseFind(_T('.'));
	if (nDotIndex == -1)
	{
		//文件名没有找到"."
		AfxMessageBox(_T("不支持的文件格式"));
		return;
	}

	CString strExtension = strPathName.Mid(nDotIndex + 1);//取出文件名"."以后的内容
	strExtension.MakeLower();//将扩展名转换为小写

	// 判断扩展名是否为"raw"
	if (strExtension == _T("raw"))
	{
		//取得图像文件的宽度、高度和图像数
		m_nImgNum = ImgNum;
		m_nWidth = ImgHeight;
		m_nHeight = ImgWidth;
		m_nRawNum = 1;
		m_nRawWidth = ImgWidth;
		m_nRawHeight = ImgHeight;
		//创建暂存文件像素值的缓冲区//16位，保存raw
		if (m_pRawBuffer != NULL)
		{
			delete[] m_pRawBuffer;
			m_pRawBuffer = NULL;
		}

		m_pRawBuffer = new uint16_t[m_nWidth * m_nHeight * m_nImgNum];
		memset(m_pRawBuffer, 0, m_nWidth * m_nHeight * m_nImgNum * sizeof(uint16_t));
		//使用fistream读取raw文件的像素值进buffer
		std::ifstream file(lpszPathName, std::ios::binary);
		file.read(reinterpret_cast<char*>(m_pRawBuffer), m_nWidth * m_nHeight * m_nImgNum * sizeof(uint16_t));
		//文件打开失败弹出警告，然后退出
		if (file.fail()) {
			AfxMessageBox(_T("读取文件失败"));
			return;
		}
		file.close();
		//因为读取进来的像素高8位和低8位顺序是反的，这里对像素高低8位进行对调，同时获取最大值
		for (int i = 0; i < m_nWidth * m_nHeight * m_nImgNum; i++) {
			m_pRawBuffer[i] = ((m_pRawBuffer[i] & 0xff00) >> 8) | ((m_pRawBuffer[i] & 0x00ff) << 8);
		}
		CreateDisplayDib(m_pRawBuffer, m_nWidth, m_nHeight, 24);
	}
	else
	{
		//不为raw，正常读取
		Load(lpszPathName);
		m_nWidth = GetWidth();
		m_nHeight = GetHeight();
		m_nWidthBytes = abs(GetPitch());
		m_nBitCount = GetBPP();
		m_pDibBits = (unsigned char*)GetBits() + (m_nHeight - 1) * GetPitch();
		m_nImgNum = 1;
	}
}


void CDib::CreateDisplayDib(uint16_t* RawBuffer, int nWidth, int nHeight, int nBitCount)
{
	//Create new Dib
	if (!IsNull())
	{
		Destroy();
	}
	Create(nWidth, nHeight, nBitCount, 0);
	m_nWidth = GetWidth();
	m_nHeight = GetHeight();
	m_nWidthBytes = abs(GetPitch());
	m_nBitCount = GetBPP();
	m_pDibBits = (unsigned char*)GetBits() + (m_nHeight - 1) * GetPitch();

	if (IsIndexed())
	{
		int nColors = GetMaxColorTableEntries();
		if (nColors > 0)
		{
			RGBQUAD* pal = new RGBQUAD[nColors];
			for (int i = 0; i < nColors; i++)
			{
				pal[i].rgbBlue = i;
				pal[i].rgbGreen = i;
				pal[i].rgbRed = i;
			}
			SetColorTable(0, nColors, pal);
			delete[] pal;
		}
		//将buffer中的像素值写入Dib
		for (int i = 0; i < m_nHeight; i++)
		{
			for (int j = 0; j < m_nWidth; j++)
			{
				*(m_pDibBits + i * m_nWidthBytes + j) = (BYTE)(RawBuffer[i * m_nWidth + j] >> 4);
			}
		}
	}
	if (m_nBitCount == 24)
	{
		//将buffer中的像素值写入Dib
		for (int i = 0; i < m_nHeight; i++)
		{
			for (int j = 0; j < m_nWidth; j++)
			{
				*(m_pDibBits + i * m_nWidthBytes + j * 3) = (BYTE)(RawBuffer[i * m_nWidth + j] >> 4);
				*(m_pDibBits + i * m_nWidthBytes + j * 3 + 1) = (BYTE)(RawBuffer[i * m_nWidth + j] >> 4);
				*(m_pDibBits + i * m_nWidthBytes + j * 3 + 2) = (BYTE)(RawBuffer[i * m_nWidth + j] >> 4);
			}
		}
	}
	if (m_IsSegmented)
	{
		COpenCVProcess process(this);

		// 将灰度图像转换为彩色图像以便绘制轮廓
		Mat colorImage;
		if (process.cvimg.channels() == 1) {
			cvtColor(process.cvimg, colorImage, COLOR_GRAY2BGR);
		}
		else {
			colorImage = process.cvimg.clone();
		}

		Mat result;
		switch (m_SegmentMethod)
		{
		case FLAIR_SEGMENTATION:
			process.SegmentFlair(process.cvimg, colorImage);	// result
			break;
		case T1_SEGMENTATION:
			process.SegmentT1(process.cvimg, colorImage);
			break;
		case T1CE_SEGMENTATION:
			process.SegmentT1ce(process.cvimg, colorImage);
			break;
		case T2_SEGMENTATION:
			process.SegmentT2(process.cvimg, colorImage);
			break;
		default:
			result = process.cvimg.clone();
			break;
		}

		if (result.channels() == 3)
		{
			cvtColor(result, result, COLOR_BGR2GRAY);
		}

		// 绘制肿瘤轮廓
		if (!result.empty())
			process.DrawTumorContours(result, colorImage);
		process.cvimg = colorImage;
		process.Mat2Dib(*this);
	}

}

void CDib::UpdateDisplayDib(int imgnum)
{
	//for (int i = 0; i < m_nHeight; i++)
	//{
	//	for (int j = 0; j < m_nWidth; j++)
	//	{
	//		*(m_pDibBits + i * m_nWidthBytes + j) = (BYTE)(m_pRawBuffer[imgnum * m_nWidth * m_nHeight + i * m_nWidth + j] >> 4);
	//	}
	//}
	if (m_pRawBuffer == NULL)
	{
		return;
	}
	if (imgnum >= m_nImgNum)
	{
		imgnum = 0;
	}
	// 创建用于存储轴位面切片的缓冲区
	uint16_t* pTemp = new uint16_t[m_nRawHeight * m_nRawWidth];
	for (int y = 0; y < m_nRawHeight; ++y) {
		for (int x = 0; x < m_nRawWidth; ++x) {
			// 计算在原始缓冲区中的索引
			int index = y * m_nRawWidth + x + imgnum * m_nRawHeight * m_nRawWidth;
			// 提取像素值
			pTemp[x + y * m_nRawWidth] = m_pRawBuffer[index];
		}
	}
	CreateDisplayDib(pTemp, m_nRawWidth, m_nRawHeight, 24);
	delete[] pTemp;
}

void CDib::Segmentation(SegmentMethod method)
{
	m_SegmentMethod = method;
	m_IsSegmented = true;
}

void CDib::Recover()
{
	m_IsSegmented = false;
}

long* CDib::GrayValueCount()	//获取图像中包含的不同灰度各自的数量
{
	long nColors = GetMaxColorTableEntries();
	if (nColors == 0)
	{
		return NULL;
	}
	long* pGrayValueCount = new long[nColors];
	memset(pGrayValueCount, 0, nColors * sizeof(long));
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			pGrayValueCount[*(m_pDibBits + i * m_nWidthBytes + j)]++;
		}
	}
	return pGrayValueCount;
}

void CDib::BilinearInterpolate(uint16_t* bilinear, uint16_t* normal, int width, int height, float ratio)
{
	// 双线性插值
	// bilinear: 插值后的像素值
	// normal: 原始像素值
	// width: 原始图像宽度
	// height: 原始图像高度
	// ratio: 缩放比例
	int newWidth = width;
	int newHeight = static_cast<int>(height * ratio);
	for (int i = 0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
		{
			// 计算在原始图像中的坐标
			float x = j;
			float y = i / ratio;
			// 计算在原始图像中的四个像素坐标
			int x1 = static_cast<int>(x);
			int y1 = static_cast<int>(y);
			int x2 = x1 + 1;
			int y2 = y1 + 1;
			// 计算插值系数
			float dx = x - x1;
			float dy = y - y1;
			// 双线性插值
			if (x2 < width && y2 < height)
			{
				bilinear[i * newWidth + j] = static_cast<uint16_t>((1 - dx) * (1 - dy) * normal[y1 * width + x1] +
					dx * (1 - dy) * normal[y1 * width + x2] +
					(1 - dx) * dy * normal[y2 * width + x1] +
					dx * dy * normal[y2 * width + x2]);
			}
		}
	}
}

void CDib::ShowTransversePlane(int CurImgNum)
{
	m_current_img_pos = 0;
	m_nCurrentImgNum_Transverse = CurImgNum;
	if (m_pRawBuffer == NULL)
	{
		return;
	}
	if (CurImgNum >= m_nImgNum)
	{
		CurImgNum = 0;
	}
	// 创建用于存储轴位面切片的缓冲区
	uint16_t* pTemp = new uint16_t[m_nRawHeight * m_nRawWidth];
	for (int y = 0; y < m_nRawHeight; ++y) {
		for (int x = 0; x < m_nRawWidth; ++x) {
			// 计算在原始缓冲区中的索引
			int index = y * m_nRawWidth + x + CurImgNum * m_nRawHeight * m_nRawWidth;
			// 提取像素值
			pTemp[x + y * m_nRawWidth] = m_pRawBuffer[index];
		}
	}
	CreateDisplayDib(pTemp, m_nRawWidth, m_nRawHeight, 24);
	delete[] pTemp;
}

void CDib::ShowCoronalPlane(int CurImgNum)
{
	//transform the coronal plane to the target coronal plane with bilinear interpolation
	m_current_img_pos = 2;
	m_nCurrentImgNum_Coronal = CurImgNum;
	//定义新图像的行列比例，如果行列比例不为1，则需要进行插值
	//const float row_col_ratio = 1.0f / 0.68359375f;
	const float row_col_ratio = 1.0f;

	int newWidth = m_nRawWidth;
	int newHeight = static_cast<int>(m_nImgNum * row_col_ratio);
	if (m_pRawBuffer == NULL)
	{
		return;
	}
	uint16_t* pTemp = new uint16_t[m_nRawWidth * m_nImgNum];
	memset(pTemp, 255, m_nRawWidth * m_nImgNum * sizeof(uint16_t));
	for (int z = 0; z < m_nImgNum; ++z) {
		for (int x = 0; x < m_nRawWidth; ++x) {
			int index = z * m_nRawWidth * m_nRawHeight + x + CurImgNum * m_nRawWidth;
			pTemp[x + z * m_nRawWidth] = m_pRawBuffer[index];
		}
	}
	uint16_t* pbi_Temp = new uint16_t[m_nRawWidth * newHeight];
	memset(pbi_Temp, 255, newWidth * newHeight * sizeof(uint16_t));
	if (row_col_ratio != 1.0f)
	{
		//如果行列比例不为1，则需要进行插值
		BilinearInterpolate(pbi_Temp, pTemp, m_nRawWidth, m_nImgNum, row_col_ratio);
	}
	else {
		//如果行列比例为1，则直接复制
		memcpy(pbi_Temp, pTemp, m_nRawWidth * m_nImgNum * sizeof(uint16_t));
	}
	CreateDisplayDib(pbi_Temp, newWidth, newHeight, 24);
	delete[] pbi_Temp;
	delete[] pTemp;
}

void CDib::ShowSagittalPlane(int CurImgNum)
{
	m_current_img_pos = 1;
	m_nCurrentImgNum_Sagittal = CurImgNum;
	//定义新图像的行列比例，如果行列比例不为1，则需要进行插值
	//const float row_col_ratio = 1.0f / 0.68359375f;
	const float row_col_ratio = 1.0f;

	int newWidth = m_nRawHeight;
	int newHeight = static_cast<int>(m_nImgNum * row_col_ratio);
	if (m_pRawBuffer == NULL)
	{
		return;
	}
	uint16_t* pTemp = new uint16_t[m_nImgNum * m_nRawHeight];
	memset(pTemp, 255, m_nRawHeight * m_nImgNum * sizeof(uint16_t));
	for (int z = 0; z < m_nImgNum; ++z) {
		for (int y = 0; y < m_nRawHeight; ++y) {
			int index = z * m_nWidth * m_nRawHeight + y * m_nWidth + CurImgNum;
			pTemp[y + z * m_nRawHeight] = m_pRawBuffer[index];
		}
	}
	uint16_t* pbi_Temp = new uint16_t[newHeight * newWidth];
	memset(pbi_Temp, 255, newWidth * newHeight * sizeof(uint16_t));
	if (row_col_ratio != 1.0f)
	{
		//如果行列比例不为1，则需要进行插值
		BilinearInterpolate(pbi_Temp, pTemp, m_nRawWidth, m_nImgNum, row_col_ratio);
	}
	else {
		//如果行列比例为1，则直接复制
		memcpy(pbi_Temp, pTemp, m_nRawWidth * m_nImgNum * sizeof(uint16_t));
	}
	CreateDisplayDib(pbi_Temp, newWidth, newHeight, 24);
	delete[] pbi_Temp;
	delete[] pTemp;
}

void CDib::ShowNextPage()
{
	switch (m_current_img_pos)
	{
	case 0:
		if (++m_nCurrentImgNum_Transverse > m_nImgNum || m_nCurrentImgNum_Transverse < 0)
			m_nCurrentImgNum_Transverse = m_nImgNum;
		ShowTransversePlane(m_nCurrentImgNum_Transverse);
		break;
	case 1:
		if (++m_nCurrentImgNum_Sagittal > m_nWidth || m_nCurrentImgNum_Sagittal < 0)
			m_nCurrentImgNum_Sagittal = m_nWidth;
		ShowSagittalPlane(m_nCurrentImgNum_Sagittal);
		break;
	case 2:
		if (++m_nCurrentImgNum_Coronal > m_nHeight || m_nCurrentImgNum_Coronal < 0)
			m_nCurrentImgNum_Coronal = m_nHeight;
		ShowCoronalPlane(m_nCurrentImgNum_Coronal);
		break;
	default:
		AfxMessageBox(_T("Unknown Mode"));
	}
}

void CDib::ShowFormerPage()
{
	switch (m_current_img_pos)
	{
	case 0:
		if (--m_nCurrentImgNum_Transverse > m_nImgNum || m_nCurrentImgNum_Transverse < 0)
			m_nCurrentImgNum_Transverse = 0;
		ShowTransversePlane(m_nCurrentImgNum_Transverse);
		break;
	case 1:
		if (--m_nCurrentImgNum_Sagittal > m_nWidth || m_nCurrentImgNum_Sagittal < 0)
			m_nCurrentImgNum_Sagittal = 0;
		ShowSagittalPlane(m_nCurrentImgNum_Sagittal);
		break;
	case 2:
		if (--m_nCurrentImgNum_Coronal > m_nHeight || m_nCurrentImgNum_Coronal < 0)
			m_nCurrentImgNum_Coronal = 0;
		ShowCoronalPlane(m_nCurrentImgNum_Coronal);
		break;
	default:
		AfxMessageBox(_T("Unknown Mode"));
	}
}


