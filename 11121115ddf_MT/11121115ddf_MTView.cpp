
// 11121115ddf_MTView.cpp: CMy11121115ddfMTView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "11121115ddf_MT.h"
#endif

#include "11121115ddf_MTDoc.h"
#include "11121115ddf_MTView.h"
#include "Dib.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy11121115ddfMTView

IMPLEMENT_DYNCREATE(CMy11121115ddfMTView, CView)

BEGIN_MESSAGE_MAP(CMy11121115ddfMTView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMy11121115ddfMTView 构造/析构

CMy11121115ddfMTView::CMy11121115ddfMTView() noexcept
{
	// TODO: 在此处添加构造代码

}

CMy11121115ddfMTView::~CMy11121115ddfMTView()
{
}

BOOL CMy11121115ddfMTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy11121115ddfMTView 绘图

void CMy11121115ddfMTView::OnDraw(CDC* /*pDC*/)
{
	CMy11121115ddfMTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (pDoc->m_pDib != NULL)
	{
		pDoc->m_pDib->Draw(GetDC()->m_hDC, 0, 0);
	}
}


// CMy11121115ddfMTView 打印

BOOL CMy11121115ddfMTView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMy11121115ddfMTView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMy11121115ddfMTView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMy11121115ddfMTView 诊断

#ifdef _DEBUG
void CMy11121115ddfMTView::AssertValid() const
{
	CView::AssertValid();
}

void CMy11121115ddfMTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy11121115ddfMTDoc* CMy11121115ddfMTView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy11121115ddfMTDoc)));
	return (CMy11121115ddfMTDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy11121115ddfMTView 消息处理程序
