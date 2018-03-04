#if !defined(AFX_TRAINLISTDLG_H__BD46A927_55A6_11D4_BCB9_00C04FA0E28F__INCLUDED_)
#define AFX_TRAINLISTDLG_H__BD46A927_55A6_11D4_BCB9_00C04FA0E28F__INCLUDED_

#if _MSC_VER > 1000
#pragma	once
#endif // _MSC_VER > 1000
// TrainListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrainListDlg dialog

class CTrainListDlg : public CDialog
{
   // Construction
public:
   void UpdateList();
   CTrainListDlg(CWnd* pParent = NULL);	  // standard constructor

   // Dialog Data
   //{{AFX_DATA(CTrainListDlg)
   enum { IDD = IDD_TrainListDIALOG };
   CListBox	m_POTrainList;
   CListBox	m_NodeList;
   CListBox	m_TrainList;
   int		m_Sum;
   int		m_BetterNodeNo;
   int		m_ParentID;
   int		m_Child1;
   int		m_Child2;
   int		m_Sum2;
   int		m_FinishedCost1;
   int		m_FinishedCost2;
   //}}AFX_DATA


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CTrainListDlg)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
						       //}}AFX_VIRTUAL

   // Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CTrainListDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnSelchangeNodeList();
   afx_msg void OnSelchangeList1();
   afx_msg void OnBUTTONOpt();
   afx_msg void OnSelchangeListpo();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAINLISTDLG_H__BD46A927_55A6_11D4_BCB9_00C04FA0E28F__INCLUDED_)
