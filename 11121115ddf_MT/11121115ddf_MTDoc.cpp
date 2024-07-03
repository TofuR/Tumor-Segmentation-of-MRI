
// 11121115ddf_MTDoc.cpp: CMy11121115ddfMTDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "11121115ddf_MT.h"
#endif

#include "11121115ddf_MTDoc.h"

#include <propkey.h>

#include "Dib.h"
#include "CDlgFileCfg.h"
#include "CDlgShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMy11121115ddfMTDoc

IMPLEMENT_DYNCREATE(CMy11121115ddfMTDoc, CDocument)

BEGIN_MESSAGE_MAP(CMy11121115ddfMTDoc, CDocument)
	ON_COMMAND(ID_TRAVERSAL, &CMy11121115ddfMTDoc::OnTraversal)
	ON_COMMAND(ID_SAGITAL, &CMy11121115ddfMTDoc::OnSagital)
	ON_COMMAND(ID_CORONAL, &CMy11121115ddfMTDoc::OnCoronal)
	ON_COMMAND(ID_32774, &CMy11121115ddfMTDoc::OnFormerPage)
	ON_COMMAND(ID_32775, &CMy11121115ddfMTDoc::OnNextPage)
	ON_COMMAND(ID_SHOW, &CMy11121115ddfMTDoc::OnShow)
	ON_COMMAND(ID_README, &CMy11121115ddfMTDoc::OnReadme)
END_MESSAGE_MAP()


// CMy11121115ddfMTDoc 构造/析构

CMy11121115ddfMTDoc::CMy11121115ddfMTDoc() noexcept
{
	m_pDib = NULL;
	m_pBuffer = NULL;
	m_current_img_pos = 0;
}

CMy11121115ddfMTDoc::~CMy11121115ddfMTDoc()
{
	if (m_pDib != NULL) {
		delete m_pDib;
	}
	if (m_pBuffer != NULL) {
		delete m_pBuffer;
	}
}

BOOL CMy11121115ddfMTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CMy11121115ddfMTDoc 序列化

void CMy11121115ddfMTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CMy11121115ddfMTDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CMy11121115ddfMTDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CMy11121115ddfMTDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMy11121115ddfMTDoc 诊断

#ifdef _DEBUG
void CMy11121115ddfMTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMy11121115ddfMTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMy11121115ddfMTDoc 命令


BOOL CMy11121115ddfMTDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	if (m_pDib != NULL)
	{
		delete m_pDib;
		m_pDib = NULL;
	}
	if (m_pBuffer != NULL)
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}

	CDlgFileCfg dlg;
	if (dlg.DoModal() != IDOK)
	{
		return FALSE;
	}
	m_pDib = new CDib;
	m_pDib->LoadFile(lpszPathName, dlg.m_nImgNum, dlg.m_nImgHeight, dlg.m_nImgWidth);
	m_pBuffer = new CDib(*m_pDib);

	OnTraversal();
	UpdateAllViews(NULL);

	return TRUE;
}


void CMy11121115ddfMTDoc::OnTraversal()
{
	if (m_pDib != NULL)
	{
		m_pDib->ShowTransversePlane(m_pDib->m_nCurrentImgNum_Transverse);
		m_current_img_pos = 0;
		UpdateAllViews(NULL);
	}
}


void CMy11121115ddfMTDoc::OnSagital()
{
	if (m_pDib != NULL)
	{
		m_pDib->ShowSagittalPlane(m_pDib->m_nCurrentImgNum_Sagittal);
		m_current_img_pos = 1;
		UpdateAllViews(NULL);
	}
}


void CMy11121115ddfMTDoc::OnCoronal()
{
	if (m_pDib != NULL)
	{
		m_pDib->ShowCoronalPlane(m_pDib->m_nCurrentImgNum_Coronal);
		m_current_img_pos = 2;
		UpdateAllViews(NULL);
	}
}


void CMy11121115ddfMTDoc::OnNextPage()
{
	// TODO: 在此添加命令处理程序代码
	m_pDib->ShowNextPage();
	UpdateAllViews(NULL);
}


void CMy11121115ddfMTDoc::OnFormerPage()
{
	// TODO: 在此添加命令处理程序代码
	m_pDib->ShowFormerPage();
	UpdateAllViews(NULL);
}


void CMy11121115ddfMTDoc::OnShow()
{
	// TODO: 在此添加命令处理程序代码
	vector<CDib*> m_pDibList;
	//CDib* m_pDib_Flair = NULL;
	//CDib* m_pDib_T1 = NULL;
	//CDib* m_pDib_T1ce = NULL;
	//CDib* m_pDib_T2 = NULL;

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = _T("Select Folder Containing RAW Files");
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != NULL)
	{
		TCHAR folderPath[MAX_PATH];
		if (SHGetPathFromIDList(pidl, folderPath))
		{
			CoTaskMemFree(pidl);
			CString searchPath(folderPath);
			searchPath += _T("\\*.raw");
			WIN32_FIND_DATA findFileData;
			HANDLE hFind = FindFirstFile(searchPath, &findFileData);
			if (hFind == INVALID_HANDLE_VALUE)
			{
				AfxMessageBox(_T("No raw files found in the selected folder"));
				return;
			}

			int count = 0;
			do
			{
				CString filePath = folderPath;
				filePath += _T("\\");
				filePath += findFileData.cFileName;
				CDib* t_pDib = new CDib;
				t_pDib->LoadFile(filePath, 155, 240, 240);
				m_pDibList.push_back(t_pDib);
				count++;
			} while (FindNextFile(hFind, &findFileData) != 0 && count < 4);

			FindClose(hFind);
		}
		else
		{
			CoTaskMemFree(pidl);
			return;
		}
	}
	else
	{
		return;
	}
	CDlgShow dlg(m_pDibList);
	dlg.DoModal();
}


void CMy11121115ddfMTDoc::OnReadme()
{
	// TODO: 在此添加命令处理程序代码
	AfxMessageBox(_T("所有操作在SHOW窗口下进行，请先点击SHOW按钮，打开文件夹\n"
		"导入数据结构请将4个raw文件放在一个文件夹下打开文件夹（请勿改变命名后缀）\n"
		"raw文件的顺序是：flair/t1/t1ce/t2，改变顺序可能会导致分割不理想\n"
		"（没有做字符串匹配，只能在导入的时候就安排好顺序）\n"));
}
