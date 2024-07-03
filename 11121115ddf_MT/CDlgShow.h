#pragma once
#include "afxdialogex.h"
#include "Dib.h"
#include <vector>

// CDlgShow 对话框

class CDlgShow : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShow)

public:
	CDlgShow(CWnd* pParent = nullptr);   // 标准构造函数
	CDlgShow(vector<CDib*>, CWnd* pParent = nullptr);   // 重载构造函数
	virtual ~CDlgShow();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SHOW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	bool m_bIsSegmented;	// 是否显示分割图像

public:
	vector<CDib*> m_vpDib;	// 保存四个三维raw图像
	int m_current_img_pos;	// 当前显示的位置：0横截面1矢状2冠状

public:
	CDib* m_pDib;			// 保存当前显示三视图的raw图像
	int m_current_img;		// 当前显示的图像：0flair 1t1 2t1ce 3t2

public:
	CSliderCtrl m_cslider_all;

	CSliderCtrl m_cslider_tra;
	CSliderCtrl m_cslider_sag;
	CSliderCtrl m_cslider_cor;

	CButton m_cbutton_flair;
	CButton m_cbutton_t1;
	CButton m_cbutton_t1ce;
	CButton m_cbutton_t2;

	CButton m_cbutton_tra;
	CButton m_cbutton_seg;
	CButton m_cbutton_cor;

	CButton m_cbutton_segment;

	CButton m_cbutton_former;
	CButton m_cbutton_next;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnClickedButtonFlair();
	afx_msg void OnClickedButtonT1();
	afx_msg void OnClickedButtonT1ce();
	afx_msg void OnClickedButtonT2();
	afx_msg void OnClickedButtonTra();
	afx_msg void OnClickedButtonSeg();
	afx_msg void OnClickedButtonCor();
	afx_msg void OnClickedButtonSegment();

	afx_msg void OnClickedButtonFormer();
	afx_msg void OnClickedButtonNext();
};
