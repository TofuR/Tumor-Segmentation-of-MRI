
// 11121115ddf_MTView.h: CMy11121115ddfMTView 类的接口
//

#pragma once


class CMy11121115ddfMTView : public CView
{
protected: // 仅从序列化创建
	CMy11121115ddfMTView() noexcept;
	DECLARE_DYNCREATE(CMy11121115ddfMTView)

// 特性
public:
	CMy11121115ddfMTDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMy11121115ddfMTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 11121115ddf_MTView.cpp 中的调试版本
inline CMy11121115ddfMTDoc* CMy11121115ddfMTView::GetDocument() const
   { return reinterpret_cast<CMy11121115ddfMTDoc*>(m_pDocument); }
#endif

