// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "rail.h"
#include "TreeView.h"
#include "define.h"
#include "railDoc.h"
#include "railView.h"
#include "DlgTimeTableSettings.h"
#include "DlgComputeSettings.h"
#include "TrainListDlg.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

   BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
   //{{AFX_MSG_MAP(CMainFrame)
   ON_COMMAND(ID_MENU001, OnMenu001)
   ON_COMMAND(ID_CreateTree, OnCreateTree)
   ON_COMMAND(ID_TimeTableSettings, OnTimeTableSettings)
   ON_COMMAND(ID_Result, OnResult)
   ON_COMMAND(ID_BUTTONTrainList, OnBUTTONTrainList)
   ON_COMMAND(ID_TestRV, OnTestRV)
   //}}AFX_MSG_MAP
   END_MESSAGE_MAP()

static UINT indicators[] =
{
   ID_SEPARATOR,	   // status line indicator
   ID_INDICATOR_CAPS,
   ID_INDICATOR_NUM,
   ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
   // TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   if( !CFrameWnd::PreCreateWindow(cs) )
      return FALSE;
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
   CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
   CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnMenu001()
{

}
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
   CRect FrameRect;
   GetWindowRect(FrameRect);
   long Width=FrameRect.Width();
   long Height=FrameRect.Height();


   BOOL	rtn=m_wndSplitter.CreateStatic(this,1,	2 );
   rtn|=m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CSplitterWnd),
				 CSize(Width/2,700 ),  // TODO: adjust the minimum pane size
				 pContext);
   rtn|=m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(TreeView),
				 CSize(Width/2, 700),  // TODO: adjust the minimum pane size
				 pContext);


   rtn=m_wndSplitter2.CreateStatic(&m_wndSplitter,2,	1 );
   rtn|=m_wndSplitter2.CreateView(0,0,RUNTIME_CLASS(CRailView),
				  CSize(Width/2,Height/2 ),  // TODO: adjust the minimum pane size
				  pContext);
   rtn|=m_wndSplitter2.CreateView(1,0,RUNTIME_CLASS(CRailView),
				  CSize(Width/2,Height/2),  // TODO: adjust the minimum pane size
				  pContext);



   if (!m_wndToolBar.Create(this) ||
       !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
   {
      TRACE0("Failed to create toolbar\n");
      return -1;      // fail to create
   }

   if (!m_wndStatusBar.Create(this) ||
       !m_wndStatusBar.SetIndicators(indicators,
				     sizeof(indicators)/sizeof(UINT)))
   {
      TRACE0("Failed to create status bar\n");
      return -1;      // fail to create
   }

   return rtn;
}

void CMainFrame::OnCreateTree()
{
   CDlgComputeSettings dlg;
   dlg.DoModal ();

}

void CMainFrame::OnTimeTableSettings()
{
   CDlgTimeTableSettings   	dlg;
   dlg.DoModal ();
}

void CMainFrame::OnResult()
{
   AfxMessageBox(g_InfoStr);

}

BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{

   return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CMainFrame::OnBUTTONTrainList()
{
   CTrainListDlg dlg;

   dlg.DoModal();

}

void CMainFrame::OnTestRV()
{
   g_TestRandomVariable();

}
