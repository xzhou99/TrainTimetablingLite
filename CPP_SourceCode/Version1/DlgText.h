#if !defined(AFX_DLGTEXT_H__6946AB14_7AFA_11D4_80E8_0050DA5DBC9D__INCLUDED_)
#define AFX_DLGTEXT_H__6946AB14_7AFA_11D4_80E8_0050DA5DBC9D__INCLUDED_

#if _MSC_VER > 1000
#pragma	once
#endif // _MSC_VER > 1000
// DlgText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgText dialog

class CDlgText : public CDialog
{
   // Construction
public:
   CDlgText(CWnd* pParent = NULL);   // standard constructor

   // Dialog Data
   //{{AFX_DATA(CDlgText)
   enum { IDD = IDD_TextDIALOG };
   CListBox	m_List;
   //}}AFX_DATA


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CDlgText)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
						       //}}AFX_VIRTUAL

   // Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CDlgText)
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEXT_H__6946AB14_7AFA_11D4_80E8_0050DA5DBC9D__INCLUDED_)
