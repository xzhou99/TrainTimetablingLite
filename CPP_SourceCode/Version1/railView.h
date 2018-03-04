// railView.h : interface of the CRailView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAILVIEW_H__40C3DFFA_4EA4_11D4_91E5_0060B0FBBAC9__INCLUDED_)
#define AFX_RAILVIEW_H__40C3DFFA_4EA4_11D4_91E5_0060B0FBBAC9__INCLUDED_

#if _MSC_VER > 1000
#pragma	once
#endif // _MSC_VER > 1000


class CRailView : public CView
{
protected: // create from serialization only
   CRailView();
   DECLARE_DYNCREATE(CRailView)

      // Attributes
      public:
      int m_ViewNo;
   CRailDoc* GetDocument();
   int m_TmLeft, m_TmRight;
   int m_Top,m_Bottom;
   int m_Left,m_Right;
   double m_XScale,m_YScale;
   int m_GridFlag ;

   CDC *pViewDC;


   // Operations
public:

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CRailView)
public:
   virtual void OnDraw(CDC* pDC);  // overridden to draw this view
   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
   //}}AFX_VIRTUAL

   // Implementation
public:
   void DrawConflicts(CDC* pDC);
   void DrawTrain(int TrainNo, bool bSelectFlag=false);
   void DrawConflicts2(CDC* pDC);
   void DrawTrain2(int TrainNo, bool bSelectFlag=false);
   void Draw(int Sta0, int Time0, int Sta1,int Time1);
   void DrawTimeTab(CDC* pDC);
   virtual ~CRailView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:

   // Generated message map functions
protected:
   //{{AFX_MSG(CRailView)
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG	// debug version in railView.cpp
inline CRailDoc* CRailView::GetDocument()
{ return (CRailDoc*)m_pDocument; }
#endif

extern CRailView* g_pRailViewUp;
extern CRailView* g_pRailViewDown;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAILVIEW_H__40C3DFFA_4EA4_11D4_91E5_0060B0FBBAC9__INCLUDED_)
