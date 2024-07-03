// CDlgShow.cpp: 实现文件
//

#include "pch.h"
#include "11121115ddf_MT.h"
#include "afxdialogex.h"
#include "CDlgShow.h"


// CDlgShow 对话框

IMPLEMENT_DYNAMIC(CDlgShow, CDialogEx)

CDlgShow::CDlgShow(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SHOW, pParent),
	m_vpDib(4, NULL),
	m_pDib(NULL),
	m_current_img_pos(0),
	m_bIsSegmented(false),
	m_current_img(0)
{
}

CDlgShow::CDlgShow(vector<CDib*> vpDib, CWnd* pParent)
	: CDialogEx(IDD_DLG_SHOW, pParent),
	m_vpDib(vpDib),
	m_pDib(NULL),
	m_current_img_pos(0),
	m_bIsSegmented(false),
	m_current_img(0)
{
	m_pDib = new CDib(*m_vpDib[0]);
}

CDlgShow::~CDlgShow()
{
	// 释放m_vpDib中的CDib*
	for (int i = 0; i < m_vpDib.size(); i++)
	{
		if (m_vpDib[i] != NULL)
		{
			delete m_vpDib[i];
			m_vpDib[i] = NULL;
		}
	}
	// 释放m_pDib
	if (m_pDib != NULL)
	{
		delete m_pDib;
		m_pDib = NULL;
	}
}

void CDlgShow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_ALL, m_cslider_all);
	DDX_Control(pDX, IDC_SLIDER_TRA, m_cslider_tra);
	DDX_Control(pDX, IDC_SLIDER_SAG, m_cslider_sag);
	DDX_Control(pDX, IDC_SLIDER_COR, m_cslider_cor);
	DDX_Control(pDX, IDC_BUTTON_FLAIR, m_cbutton_flair);
	DDX_Control(pDX, IDC_BUTTON_T1, m_cbutton_t1);
	DDX_Control(pDX, IDC_BUTTON_T1CE, m_cbutton_t1ce);
	DDX_Control(pDX, IDC_BUTTON_T2, m_cbutton_t2);
	DDX_Control(pDX, IDC_BUTTON_TRA, m_cbutton_tra);
	DDX_Control(pDX, IDC_BUTTON_SEG, m_cbutton_seg);
	DDX_Control(pDX, IDC_BUTTON_COR, m_cbutton_cor);
	DDX_Control(pDX, IDC_BUTTON_SEGMENT, m_cbutton_segment);
	DDX_Control(pDX, IDC_BUTTON_FORMER, m_cbutton_former);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_cbutton_next);
}


BEGIN_MESSAGE_MAP(CDlgShow, CDialogEx)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_FLAIR, &CDlgShow::OnClickedButtonFlair)
	ON_BN_CLICKED(IDC_BUTTON_T1, &CDlgShow::OnClickedButtonT1)
	ON_BN_CLICKED(IDC_BUTTON_T1CE, &CDlgShow::OnClickedButtonT1ce)
	ON_BN_CLICKED(IDC_BUTTON_T2, &CDlgShow::OnClickedButtonT2)
	ON_BN_CLICKED(IDC_BUTTON_TRA, &CDlgShow::OnClickedButtonTra)
	ON_BN_CLICKED(IDC_BUTTON_SEG, &CDlgShow::OnClickedButtonSeg)
	ON_BN_CLICKED(IDC_BUTTON_COR, &CDlgShow::OnClickedButtonCor)
	ON_BN_CLICKED(IDC_BUTTON_SEGMENT, &CDlgShow::OnClickedButtonSegment)
	ON_BN_CLICKED(IDC_BUTTON_FORMER, &CDlgShow::OnClickedButtonFormer)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CDlgShow::OnClickedButtonNext)
END_MESSAGE_MAP()


// CDlgShow 消息处理程序


void CDlgShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	//CRect rect;

	//CWnd* pWnd = GetDlgItem(IDC_PIC_TRA);
	//pWnd->GetWindowRect(&rect);
	//ScreenToClient(&rect);
	//CDC* pDC = &dc;
	//pWnd->UpdateWindow();
	//pDC->Rectangle(rect);
	//m_pDib->Draw(pDC->m_hDC, rect.left, rect.top, rect.Width(), rect.Height());

	// lambda表达式，用于绘制图像
	auto DrawPic = [&](int controlID, CDib* pDib) {
		CRect rect;
		CWnd* pWnd = GetDlgItem(controlID);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pWnd->UpdateWindow();
		dc.Rectangle(rect);
		if (pDib) {
			pDib->Draw(dc.m_hDC, rect.left, rect.top, rect.Width(), rect.Height());
		}
		};

	// 改变四个图像的显示位置
	for (auto& p : m_vpDib)
	{
		p->m_current_img_pos = m_current_img_pos;

	}
	if (m_current_img_pos == 0)
	{
		for (auto& p : m_vpDib)
		{
			p->ShowTransversePlane(p->m_nCurrentImgNum_Transverse);
		}
	}
	else if (m_current_img_pos == 1)
	{
		for (auto& p : m_vpDib)
		{
			p->ShowSagittalPlane(p->m_nCurrentImgNum_Sagittal);
		}
	}
	else if (m_current_img_pos == 2)
	{
		for (auto& p : m_vpDib)
		{
			p->ShowCoronalPlane(p->m_nCurrentImgNum_Coronal);
		}
	}
	DrawPic(IDC_PIC_FLAIR, m_vpDib[0]);
	DrawPic(IDC_PIC_T1, m_vpDib[1]);
	DrawPic(IDC_PIC_T1CE, m_vpDib[2]);
	DrawPic(IDC_PIC_T2, m_vpDib[3]);

	m_pDib->ShowTransversePlane(m_pDib->m_nCurrentImgNum_Transverse);
	DrawPic(IDC_PIC_TRA, m_pDib);
	m_pDib->ShowSagittalPlane(m_pDib->m_nCurrentImgNum_Sagittal);
	DrawPic(IDC_PIC_SAG, m_pDib);
	m_pDib->ShowCoronalPlane(m_pDib->m_nCurrentImgNum_Coronal);
	DrawPic(IDC_PIC_COR, m_pDib);

}


BOOL CDlgShow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 设置滑块的范围
	m_cslider_all.SetRange(0, m_vpDib[0]->m_nImgNum - 1);
	m_cslider_tra.SetRange(0, m_vpDib[0]->m_nImgNum - 1);
	m_cslider_sag.SetRange(0, m_vpDib[0]->m_nRawHeight - 1);
	m_cslider_cor.SetRange(0, m_vpDib[0]->m_nRawWidth - 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgShow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

	// 如果是m_cslider_all滑块发生了滚动，四个图像flair/t1/t1ce/t2更新到对应的图像位置
	if (pScrollBar == (CScrollBar*)&m_cslider_all)
	{
		int nPos = m_cslider_all.GetPos();
		for (auto& p : m_vpDib)
		{
			if (m_current_img_pos == 0)
			{
				p->m_nCurrentImgNum_Transverse = nPos;
			}
			else if (m_current_img_pos == 1)
			{
				p->m_nCurrentImgNum_Sagittal = nPos;
			}
			else if (m_current_img_pos == 2)
			{
				p->m_nCurrentImgNum_Coronal = nPos;
			}
		}
	}
	else if (pScrollBar == (CScrollBar*)&m_cslider_tra)
	{
		int nPos = m_cslider_tra.GetPos();
		m_pDib->m_nCurrentImgNum_Transverse = nPos;
	}
	else if (pScrollBar == (CScrollBar*)&m_cslider_sag)
	{
		int nPos = m_cslider_sag.GetPos();
		m_pDib->m_nCurrentImgNum_Sagittal = nPos;
	}
	else if (pScrollBar == (CScrollBar*)&m_cslider_cor)
	{
		int nPos = m_cslider_cor.GetPos();
		m_pDib->m_nCurrentImgNum_Coronal = nPos;
	}
	Invalidate();
}


void CDlgShow::OnClickedButtonFlair()
{
	// TODO: 在此添加控件通知处理程序代码
	int	t_CurImgNum_Tra = m_pDib->m_nCurrentImgNum_Transverse;
	int	t_CurImgNum_Sag = m_pDib->m_nCurrentImgNum_Sagittal;
	int	t_CurImgNum_Cor = m_pDib->m_nCurrentImgNum_Coronal;

	if (m_pDib)
	{
		delete m_pDib;
		m_pDib = NULL;
	}
	m_pDib = new CDib(*m_vpDib[0]);
	m_current_img = 0;

	m_pDib->m_nCurrentImgNum_Transverse = t_CurImgNum_Tra;
	m_pDib->m_nCurrentImgNum_Sagittal = t_CurImgNum_Sag;
	m_pDib->m_nCurrentImgNum_Coronal = t_CurImgNum_Cor;

	Invalidate();
}


void CDlgShow::OnClickedButtonT1()
{
	// TODO: 在此添加控件通知处理程序代码
	int	t_CurImgNum_Tra = m_pDib->m_nCurrentImgNum_Transverse;
	int	t_CurImgNum_Sag = m_pDib->m_nCurrentImgNum_Sagittal;
	int	t_CurImgNum_Cor = m_pDib->m_nCurrentImgNum_Coronal;

	if (m_pDib)
	{
		delete m_pDib;
		m_pDib = NULL;
	}
	m_pDib = new CDib(*m_vpDib[1]);
	m_current_img = 1;

	m_pDib->m_nCurrentImgNum_Transverse = t_CurImgNum_Tra;
	m_pDib->m_nCurrentImgNum_Sagittal = t_CurImgNum_Sag;
	m_pDib->m_nCurrentImgNum_Coronal = t_CurImgNum_Cor;

	Invalidate();
}


void CDlgShow::OnClickedButtonT1ce()
{
	// TODO: 在此添加控件通知处理程序代码
	int	t_CurImgNum_Tra = m_pDib->m_nCurrentImgNum_Transverse;
	int	t_CurImgNum_Sag = m_pDib->m_nCurrentImgNum_Sagittal;
	int	t_CurImgNum_Cor = m_pDib->m_nCurrentImgNum_Coronal;

	if (m_pDib)
	{
		delete m_pDib;
		m_pDib = NULL;
	}
	m_pDib = new CDib(*m_vpDib[2]);
	m_current_img = 2;

	m_pDib->m_nCurrentImgNum_Transverse = t_CurImgNum_Tra;
	m_pDib->m_nCurrentImgNum_Sagittal = t_CurImgNum_Sag;
	m_pDib->m_nCurrentImgNum_Coronal = t_CurImgNum_Cor;

	Invalidate();
}


void CDlgShow::OnClickedButtonT2()
{
	// TODO: 在此添加控件通知处理程序代码
	int	t_CurImgNum_Tra = m_pDib->m_nCurrentImgNum_Transverse;
	int	t_CurImgNum_Sag = m_pDib->m_nCurrentImgNum_Sagittal;
	int	t_CurImgNum_Cor = m_pDib->m_nCurrentImgNum_Coronal;

	if (m_pDib)
	{
		delete m_pDib;
		m_pDib = NULL;
	}
	m_pDib = new CDib(*m_vpDib[3]);
	m_current_img = 3;

	m_pDib->m_nCurrentImgNum_Transverse = t_CurImgNum_Tra;
	m_pDib->m_nCurrentImgNum_Sagittal = t_CurImgNum_Sag;
	m_pDib->m_nCurrentImgNum_Coronal = t_CurImgNum_Cor;

	Invalidate();
}


void CDlgShow::OnClickedButtonTra()
{
	// TODO: 在此添加控件通知处理程序代码
	m_current_img_pos = 0;

	int nPos = m_vpDib[0]->m_nCurrentImgNum_Transverse;
	m_cslider_all.SetRange(0, m_vpDib[0]->m_nImgNum - 1);
	m_cslider_all.SetPos(nPos);

	Invalidate();
}


void CDlgShow::OnClickedButtonSeg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_current_img_pos = 1;

	int nPos = m_vpDib[0]->m_nCurrentImgNum_Sagittal;
	m_cslider_all.SetRange(0, m_vpDib[0]->m_nRawHeight - 1);
	m_cslider_all.SetPos(nPos);

	Invalidate();
}


void CDlgShow::OnClickedButtonCor()
{
	// TODO: 在此添加控件通知处理程序代码
	m_current_img_pos = 2;

	int nPos = m_vpDib[0]->m_nCurrentImgNum_Coronal;
	m_cslider_all.SetRange(0, m_vpDib[0]->m_nRawWidth - 1);
	m_cslider_all.SetPos(nPos);

	Invalidate();
}


void CDlgShow::OnClickedButtonSegment()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsSegmented = !m_bIsSegmented;
	if (m_bIsSegmented)
	{
		m_vpDib[0]->Segmentation(CDib::FLAIR_SEGMENTATION);
		m_vpDib[1]->Segmentation(CDib::T1_SEGMENTATION);
		m_vpDib[2]->Segmentation(CDib::T1CE_SEGMENTATION);
		m_vpDib[3]->Segmentation(CDib::T2_SEGMENTATION);
		m_pDib->Segmentation((CDib::SegmentMethod)(m_current_img + 1));
	}
	else
	{
		for (auto& p : m_vpDib)
		{
			p->Recover();
		}
		m_pDib->Recover();
	}
	Invalidate();
}


void CDlgShow::OnClickedButtonFormer()
{
	// TODO: 在此添加控件通知处理程序代码
	int pos = m_cslider_all.GetPos();
	int min = m_cslider_all.GetRangeMin();
	pos--;
	if (pos < min)
	{
		return;
	}
	m_cslider_all.SetPos(pos);
	for (auto& p : m_vpDib)
	{
		if (m_current_img_pos == 0)
		{
			p->m_nCurrentImgNum_Transverse = pos;
		}
		else if (m_current_img_pos == 1)
		{
			p->m_nCurrentImgNum_Sagittal = pos;
		}
		else if (m_current_img_pos == 2)
		{
			p->m_nCurrentImgNum_Coronal = pos;
		}
	}
	Invalidate();
}


void CDlgShow::OnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	int pos = m_cslider_all.GetPos();
	int max = m_cslider_all.GetRangeMax();
	pos++;
	if (pos >= max)
	{
		return;
	}
	m_cslider_all.SetPos(pos);
	for (auto& p : m_vpDib)
	{
		if (m_current_img_pos == 0)
		{
			p->m_nCurrentImgNum_Transverse = pos;
		}
		else if (m_current_img_pos == 1)
		{
			p->m_nCurrentImgNum_Sagittal = pos;
		}
		else if (m_current_img_pos == 2)
		{
			p->m_nCurrentImgNum_Coronal = pos;
		}
	}
	Invalidate();
}
