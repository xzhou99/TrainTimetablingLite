// railView.cpp : implementation of the CRailView class
//

#include "stdafx.h"
#include "rail.h"

#include "railDoc.h"
#include "railView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPen ThickPen(PS_SOLID,3,RGB(0,255,0));
CPen DoublePen(PS_SOLID,2,RGB(0,255,0));
CPen NormalPen(PS_SOLID,0,RGB(0,255,0));
CPen Normal2Pen(PS_SOLID,2,RGB(0,0,255));
CPen BlackPen(PS_SOLID,1,RGB(0,0,0));
CPen RedPen(PS_SOLID,1,RGB(255,0,0));

CPen DashPen(PS_DASH,0,RGB(0,255,0));
CPen DoubleDashPen(PS_DASH,2,RGB(0,255,0));
CPen SelectPen(PS_DASH,2,RGB(255,0,0));
CPen ConflictPen(PS_SOLID,1,RGB(255,0,0));


CRailView* g_pRailViewUp = NULL;
CRailView* g_pRailViewDown = NULL;
/////////////////////////////////////////////////////////////////////////////
// CRailView

IMPLEMENT_DYNCREATE(CRailView, CView)

   BEGIN_MESSAGE_MAP(CRailView, CView)
   //{{AFX_MSG_MAP(CRailView)
   ON_WM_ERASEBKGND()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRailView construction/destruction


CRailView::CRailView()
{

   if(g_pRailViewUp == NULL)
   {
      m_ViewNo = 0;
      g_pRailViewUp = this;
   }
   else
   {
      m_ViewNo = 1;
      g_pRailViewDown = this;

   }


   m_TmLeft=0;
   m_TmRight=60 * 8;
   m_Left=20;
   m_Right=20;
   m_Top=20;
   m_Bottom=20;




   for(int k=0;k<MAX_STATIONSIZE;k++)
   {
      g_SectionRunTime[k][0]=10;
      g_SectionRunTime[k][1]=18;

      g_StationDistance[k] = k * 100;
   }
   g_TravelTime[0] = g_SectionRunTime[0][0]*g_SectionSize;
   g_TravelTime[1] = g_SectionRunTime[0][1]*g_SectionSize;


   m_GridFlag = 1;
}

CRailView::~CRailView()
{
}

BOOL CRailView::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRailView drawing

void CRailView::OnDraw(CDC* pDC)
{

   DrawTimeTab(pDC);
   DrawConflicts(pDC);

}

/////////////////////////////////////////////////////////////////////////////
// CRailView diagnostics

#ifdef _DEBUG
void CRailView::AssertValid() const
{
   CView::AssertValid();
}

void CRailView::Dump(CDumpContext& dc) const
{
   CView::Dump(dc);
}

CRailDoc* CRailView::GetDocument() // non-debug version is inline
{
   ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRailDoc)));
   return (CRailDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRailView message handlers

void CRailView::DrawTimeTab(CDC *pDC)
{
   pViewDC = pDC;
   CBrush brush;
   if (!brush.CreateSolidBrush(RGB(255,255,255)))
      return;

   CRect drawrect;
   GetClientRect(drawrect);

   brush.UnrealizeObject();
   pDC->FillRect(drawrect, &brush);

   drawrect.NormalizeRect();

   double m_SizeX= drawrect.Width () - m_Left -m_Right;
   double m_SizeY= drawrect.Height () - m_Top -m_Bottom;

   g_TotalDistance = g_StationDistance[g_SectionSize];

   m_XScale = m_SizeX *1.0 / (m_TmRight - m_TmLeft)  ;
   m_YScale = m_SizeY *1.0 / g_TotalDistance  ;

   // Draw Framework for TimeTab
   pDC->SelectObject (&ThickPen);

   Draw(0,m_TmLeft,0,m_TmRight);
   Draw(g_SectionSize,m_TmLeft,g_SectionSize,m_TmRight);

   Draw(0,m_TmLeft,g_SectionSize,m_TmLeft);
   Draw(0,m_TmRight,g_SectionSize,m_TmRight);

   int i;
   // Draw station line
   pDC->SelectObject (&DoublePen);
   for(i=0; i< g_SectionSize;i++)
      Draw(i,m_TmLeft,i,m_TmRight);

   // Draw time  line per two minutes
   pDC->SelectObject (&NormalPen);

   if(m_GridFlag <= 0)
   {
      for(i=m_TmLeft; i<m_TmRight;i+=2)
	 Draw(0,i,g_SectionSize,i);
   }

   // Draw time  line per ten minutes
   if(m_GridFlag <= 1)
   {
      pDC->SelectObject (&DoublePen);
      for(i=m_TmLeft; i<m_TmRight;i+=10)
	 Draw(0,i,g_SectionSize,i);
   }

   if(m_GridFlag <= 2){
      for(i=m_TmLeft; i<m_TmRight;i+=60)
      {
	 pDC->SelectObject (&Normal2Pen);
	 Draw(0,i,g_SectionSize,i);
      }
   }

   if(m_ViewNo == 0)
   {
      // Draw Train running lines
      for(i=0;i< g_TrainSize; i++)
	 DrawTrain(i);
   }
   else
   {
      // Draw Train running lines
      for(i=0;i< g_TrainSize; i++)
	 DrawTrain2(i);
   }



}

void CRailView::Draw(int Sta0, int Time0, int Sta1,int Time1)
{
   int x0,y0,x1,y1;
   x0 = m_Left + int ((Time0 - m_TmLeft) * m_XScale);
   x1 = m_Left + int ((Time1 - m_TmLeft) * m_XScale);
   y0 = int (( g_TotalDistance - g_StationDistance [Sta0] )* m_YScale ) + m_Top;
   y1 = int (( g_TotalDistance - g_StationDistance [Sta1] )* m_YScale ) + m_Top;

   pViewDC->MoveTo (x0,y0);
   pViewDC->LineTo (x1,y1);

}

void CRailView::DrawTrain(int TrainNo, bool bSelectFlag)
{
   int i;


   //Loop for each section
   for(i=0;i<g_SectionSize;i++)
   {

      int DepartureTime = g_Table[TrainNo][i][0];
      int ArrivalTime = g_Table[TrainNo][i][1];

      pViewDC->SelectObject (&NormalPen);

      if(g_ConflictFlag==1)
      {

	 if(i == g_FirstConflictStationNo &&( (TrainNo == g_ConflictTrainSet[0]) || (TrainNo == g_ConflictTrainSet[1])))
	    pViewDC->SelectObject(&ConflictPen);

      }

      if(g_Table[TrainNo][i][3] == 1 )
	 pViewDC->SelectObject(&BlackPen);

      if(g_Table[TrainNo][i][3] == 1 && g_TrainType[TrainNo]==0)
	 pViewDC->SelectObject(&RedPen);

      if(g_SelectTrainNo == TrainNo)
	 pViewDC->SelectObject (&SelectPen);

      if( g_TrainDirection[TrainNo] == 0)
	 Draw(i,DepartureTime,i+1,ArrivalTime);
      else
	 Draw(i+1,DepartureTime,i,ArrivalTime);


   }


}

void CRailView::DrawTrain2(int TrainNo, bool bSelectFlag)
{
   int i;


   //Loop for each section
   for(i=0;i<g_SectionSize;i++)
   {

      int DepartureTime = g_Table2[TrainNo][i][0];
      int ArrivalTime = g_Table2[TrainNo][i][1];

      pViewDC->SelectObject (&NormalPen);

      if(g_ConflictFlag==1)
      {

	 if(i == g_FirstConflictStationNo &&( (TrainNo == g_ConflictTrainSet[0]) || (TrainNo == g_ConflictTrainSet[1])))
	    pViewDC->SelectObject(&ConflictPen);

      }

      if(g_Table2[TrainNo][i][3] == 1 && g_TrainType[TrainNo]==1)
	 pViewDC->SelectObject(&BlackPen);
      if(g_Table2[TrainNo][i][3] == 1 && g_TrainType[TrainNo]==0)
	 pViewDC->SelectObject(&RedPen);

      if(g_SelectTrainNo == TrainNo)
	 pViewDC->SelectObject (&SelectPen);

      if( g_TrainDirection[TrainNo] == 0)
	 Draw(i,DepartureTime,i+1,ArrivalTime);
      else
	 Draw(i+1,DepartureTime,i,ArrivalTime);


   }


}

void CRailView::DrawConflicts(CDC* pDC)
{
   if(g_ConflictFlag == 1)
   {
      pDC->SelectObject(&ConflictPen);
      Draw(g_FirstConflictStationNo,g_FirstConflictTime,g_FirstConflictStationNo+1,g_FirstConflictTime);
   }

}

void CRailView::DrawConflicts2(CDC* pDC)
{
   if(g_ConflictFlag2 == 1)
   {
      pDC->SelectObject(&ConflictPen);
      Draw(g_FirstConflictStationNo2,g_FirstConflictTime2,g_FirstConflictStationNo2+1,g_FirstConflictTime2);
   }

}

BOOL CRailView::OnEraseBkgnd(CDC* pDC)
{

   return false;
}
