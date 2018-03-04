#if !defined(AFX_DLGTIMETABLESETTINGS_H__BB705C71_5745_11D4_BCBA_00C04FA0E28F__INCLUDED_)
#define AFX_DLGTIMETABLESETTINGS_H__BB705C71_5745_11D4_BCBA_00C04FA0E28F__INCLUDED_

#if _MSC_VER > 1000
#pragma	once
#endif // _MSC_VER > 1000
// DlgTimeTableSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTimeTableSettings dialog

class CDlgTimeTableSettings : public CDialog
{
   // Construction
public:
   void UpdateView();
   CDlgTimeTableSettings(CWnd* pParent = NULL);	  // standard constructor

   // Dialog Data
   //{{AFX_DATA(CDlgTimeTableSettings)
   enum { IDD = IDD_TimeTableSettingsDIALOG };
   CListBox	m_ListGrid;
   CListBox	m_RightList;
   CListBox	m_LeftList;
   //}}AFX_DATA


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CDlgTimeTableSettings)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
						       //}}AFX_VIRTUAL

   // Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CDlgTimeTableSettings)
   virtual BOOL OnInitDialog();
   afx_msg void OnApply();
   afx_msg void OnSelchangeList1();
   afx_msg void OnSelchangeList3();
   afx_msg void OnSelchangeLISTGrid();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTIMETABLESETTINGS_H__BB705C71_5745_11D4_BCBA_00C04FA0E28F__INCLUDED_)
