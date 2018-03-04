#if !defined(AFX_TREEVIEW_H__00E8CB34_54D8_11D4_BCC1_00C04FA0E29A__INCLUDED_)
#define AFX_TREEVIEW_H__00E8CB34_54D8_11D4_BCC1_00C04FA0E29A__INCLUDED_

#if _MSC_VER > 1000
#pragma	once
#endif // _MSC_VER > 1000
// TreeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TreeView view

class TreeView : public CView
{
protected:
   TreeView();		 // protected constructor used by dynamic creation
   DECLARE_DYNCREATE(TreeView)

      // Attributes
      public:
      CDC* pDrawDC;
   int m_DrawFlag;
   bool m_bDraw;

   // Operations
public:
   void DrawAllObjects(CDC* pDC, CRect drawrect);

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(TreeView)
protected:
   virtual void OnDraw(CDC* pDC);      // overridden to draw this view
				       //}}AFX_VIRTUAL

   // Implementation
protected:
   virtual ~TreeView();
#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

   // Generated message map functions
protected:
   //{{AFX_MSG(TreeView)
   afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnTreeViewType();
   afx_msg void OnTreeViewComplete();
   afx_msg void OnTreeViewComputing();
   afx_msg void OnUpdateTreeViewComputing(CCmdUI* pCmdUI);
   afx_msg void OnUpdateTreeViewComplete(CCmdUI* pCmdUI);
   afx_msg void OnUpdateTreeViewType(CCmdUI* pCmdUI);
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
extern TreeView* g_pTreeView;
extern CDWordArray g_ComputeAry;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_TREEVIEW_H__00E8CB34_54D8_11D4_BCC1_00C04FA0E29A__INCLUDED_)
