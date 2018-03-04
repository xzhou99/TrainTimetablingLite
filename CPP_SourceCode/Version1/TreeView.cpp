// TreeView.cpp : implementation file
//

#include "stdafx.h"
#include "rail.h"
#include "TreeView.h"
#include "Node.h"
#include "define.h"
#include "railDoc.h"
#include "railView.h"
#include "TrainListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TreeView
extern TreeView* g_pTreeView = NULL;
CPen g_PenGreen(PS_SOLID,1,RGB(0,255,0));

IMPLEMENT_DYNCREATE(TreeView, CView)

   TreeView::TreeView()
{
   g_pTreeView = this;
   m_DrawFlag = 0;
   m_bDraw = true;
}

TreeView::~TreeView()
{

}


BEGIN_MESSAGE_MAP(TreeView, CView)
   //{{AFX_MSG_MAP(TreeView)
   ON_WM_LBUTTONDBLCLK()
ON_WM_ERASEBKGND()
ON_WM_LBUTTONDOWN()
ON_COMMAND(ID_TreeViewType, OnTreeViewType)
   ON_COMMAND(ID_TreeView_Complete, OnTreeViewComplete)
   ON_COMMAND(ID_TreeView_Computint, OnTreeViewComputing)
   ON_UPDATE_COMMAND_UI(ID_TreeView_Computint, OnUpdateTreeViewComputing)
   ON_UPDATE_COMMAND_UI(ID_TreeView_Complete, OnUpdateTreeViewComplete)
   ON_UPDATE_COMMAND_UI(ID_TreeViewType, OnUpdateTreeViewType)
   //}}AFX_MSG_MAP
   END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TreeView drawing

void TreeView::OnDraw(CDC* pDC)
{

   CRect client;
   GetWindowRect(client);

   long Width=client.Width();
   long Height=client.Height();

   g_LayerHeight = Height*1.0 /(g_TreeLayer+2);
   g_LayerWidth = Width *1.0 ;

   CBitmap bitmap;
   CBitmap* pOldBitmap;
   CRect ViewRect(0,0,0,0);

   CDC dc;
   pDrawDC = pDC;

   // only paint the rect that needs repainting
   pDC->GetClipBox(client);
   CRect rect = client;
   //	DocToClient(rect);

   if (!pDC->IsPrinting())
   {
      // draw to offscreen bitmap for fast looking repaints
      if (dc.CreateCompatibleDC(pDC))
      {
	 if(!((ViewRect.Width()==rect.Width())&&
	      (ViewRect.Height()==rect.Height())))
	 {
	    bitmap.DeleteObject();
	    bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	    ViewRect=rect;
	 }
	 OnPrepareDC(&dc, NULL);
	 pDrawDC = &dc;

	 // offset origin more because bitmap is just piece of the whole drawing
	 dc.OffsetViewportOrg(-rect.left, -rect.top);
	 pOldBitmap = dc.SelectObject(&bitmap);
	 dc.SetBrushOrg(rect.left % 8, rect.top % 8);

	 // might as well clip to the same rectangle
	 dc.IntersectClipRect(client);
      }
   }

   CBrush brush;
   if (!brush.CreateSolidBrush(RGB(0,0,0)))
      return;

   brush.UnrealizeObject();
   pDrawDC->FillRect(client, &brush);


   CRect drawrect;
   pDC->GetClipBox(drawrect);
   drawrect.NormalizeRect();
   pDrawDC->SetBkMode(TRANSPARENT);
   COLORREF TextColor=RGB(0,255,0);
   pDrawDC->SetTextColor(TextColor);

   if(m_bDraw)
      DrawAllObjects(pDrawDC,drawrect);


   if (pDrawDC != pDC)
   {
      pDC->SetViewportOrg(0, 0);
      pDC->SetWindowOrg(0,0);
      pDC->SetMapMode(MM_TEXT);
      dc.SetViewportOrg(0, 0);
      dc.SetWindowOrg(0,0);
      dc.SetMapMode(MM_TEXT);
      pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		  &dc, 0, 0, SRCCOPY);
      dc.SelectObject(pOldBitmap);
   }

}

/////////////////////////////////////////////////////////////////////////////
// TreeView diagnostics

#ifdef _DEBUG
void TreeView::AssertValid() const
{
   CView::AssertValid();
}

void TreeView::Dump(CDumpContext& dc) const
{
   CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// TreeView message handlers

void TreeView::DrawAllObjects(CDC *pDC, CRect drawrect)
{
   CNode* pNode ;
   for(int i=0; i< g_TreeNodeAry.GetSize(); i++)
   {
      pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
      pNode->LocateCenter ();
   }

   bool LargeSizeFlag = false;
   if (g_TreeNodeAry.GetSize() >=1000)
      LargeSizeFlag = true;

   if (g_TreeNodeAry.GetSize() >=5000)
      m_bDraw = false;

   int step =1;
   //	if(LargeSizeFlag)
   //		step = g_TreeNodeAry.GetSize() / 1000;

   int i;

   if( m_DrawFlag== 0)
   {

      for(i=0; i< g_TreeNodeAry.GetSize(); i+=step)
      {

	 pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
	 pNode->DrawOptRelation(pDC);
	 pNode->DrawRelation(pDC);
	 pNode->Draw (pDC,!LargeSizeFlag);
      }
      if( g_SelectNodeNo != -1)
      {
	 pNode = (CNode*)g_TreeNodeAry.GetAt (0);
	 pNode->Draw (pDC,true);
      }
   }
   if( m_DrawFlag== 1)
   {

      pDC->SelectObject (&g_PenGreen);

      pNode = (CNode*)g_TreeNodeAry.ElementAt (0);
      pDC->MoveTo (pNode->m_X,pNode->m_Y);

      for(i=1; i< g_ComputeAry.GetSize(); i++)
      {
	 pNode = (CNode*)g_TreeNodeAry.ElementAt (g_ComputeAry.GetAt (i));

	 pDC->LineTo (pNode->m_X,pNode->m_Y);

      }

      if( g_SelectNodeNo != -1)
      {
	 pNode = (CNode*)g_TreeNodeAry.GetAt (g_SelectNodeNo);
	 pNode->Draw (pDC,LargeSizeFlag);
      }
   }
}

void TreeView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
   /*	for(int i=0; i< g_TreeNodeAry.GetSize(); i++)
      {
      CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
      pNode->m_bSelectFlag = false;
      if(pNode->Intersect (point))
      {
      pNode->m_bSelectFlag = true;
      pNode->UpdateTimeTable();

      }

      }

      Invalidate();
      g_pRailView->Invalidate ();
      */
   for(int i=0; i< g_TreeNodeAry.GetSize(); i++)
   {
      CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
      pNode->m_bSelectFlag = false;
      if(pNode->Intersect (point))
      {
	 pNode->m_bSelectFlag = true;
	 g_SelectNodeNo = i;
	 break;

      }

   }

   CTrainListDlg dlg;

   dlg.DoModal();
}

BOOL TreeView::OnEraseBkgnd(CDC* pDC)
{
   return true;
}

void TreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
   //    pDrawDC->DPtoLP(&point);
   g_SelectNodeNo = -1;
   int i;
   for( i=0; i< g_TreeNodeAry.GetSize(); i++)
   {
      CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
      pNode->m_bSelectFlag = false;
   }
   
   for(i=0; i< g_TreeNodeAry.GetSize(); i++)
   {
      CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
      if(pNode->Intersect (point))
      {
	 pNode->m_bSelectFlag = true;
	 g_SelectNodeNo = i;
	 pNode->UpdateTimeTable();
	 break;

      }

   }


   Invalidate();
   g_pRailViewUp->Invalidate ();
   g_pRailViewDown->Invalidate ();
}

void TreeView::OnTreeViewType()
{
   m_DrawFlag = 1;
   Invalidate();
}

void TreeView::OnTreeViewComplete()
{
   m_DrawFlag = 0;
   g_CompleteTreeDrawFlag = true;
   Invalidate();

}

void TreeView::OnTreeViewComputing()
{
   m_DrawFlag = 0;
   g_CompleteTreeDrawFlag = false;
   Invalidate();

}

void TreeView::OnUpdateTreeViewComputing(CCmdUI* pCmdUI)
{
   pCmdUI->SetCheck(m_DrawFlag == 0 && !g_CompleteTreeDrawFlag);


}

void TreeView::OnUpdateTreeViewComplete(CCmdUI* pCmdUI)
{
   pCmdUI->SetCheck(m_DrawFlag == 0 && g_CompleteTreeDrawFlag)	;
}

void TreeView::OnUpdateTreeViewType(CCmdUI* pCmdUI)
{
   pCmdUI->SetCheck(m_DrawFlag == 1);
}
