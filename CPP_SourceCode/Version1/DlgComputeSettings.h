#if !defined(AFX_DLGCOMPUTESETTINGS_H__D34252E7_5F43_11D4_91F2_0060B0FBBADA__INCLUDED_)
#define AFX_DLGCOMPUTESETTINGS_H__D34252E7_5F43_11D4_91F2_0060B0FBBADA__INCLUDED_

#if _MSC_VER > 1000
#pragma	once
#endif // _MSC_VER > 1000
// DlgComputeSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgComputeSettings dialog
#include "DlgText.h"

class CDlgComputeSettings : public CDialog
{
   // Construction
public:
   void TestOneRun(int g_SearchFlag , int g_CurComputeMethodNo,int ComputeNumber );
   CDlgText* p_Textdlg;
   bool m_ShowTree;
   void UpdateList();
   CDlgComputeSettings(CWnd* pParent = NULL);	// standard constructor
   int m_SelNo;
   double m_Memory1;
   double m_Memory2;
   double m_Memory3;
   double m_Memory4;

   CString m_strText;
   int m_MinTrainNum;
   int m_MaxTrainNum;


   // Dialog Data
   //{{AFX_DATA(CDlgComputeSettings)
   enum { IDD = IDD_ComputeSettingsDIALOG };
   CComboBox	m_Combo4;
   CListBox	m_RuleList;
   CComboBox	m_Combo3;
   CComboBox	m_SearchCombo;
   CComboBox	m_Combo2;
   CComboBox	m_Combo;
   CListBox	m_List2;
   CListBox	m_List1;
   int		m_Number;
   int		m_Seed;
   int		m_TypeRatio;
   int		m_SpeedRatio;
   BOOL	m_BestSearch;
   int		m_FilterWidth;
   int		m_BeamWidth;
   BOOL	m_SubOptimalEst;
   int		m_K;
   //}}AFX_DATA


   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CDlgComputeSettings)
protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
						       //}}AFX_VIRTUAL

   // Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CDlgComputeSettings)
   virtual void OnOK();
   virtual BOOL OnInitDialog();
   afx_msg void OnBUTTONCompute();
   afx_msg void OnBUTTONComputeAll();
   afx_msg void OnSelchangeList1();
   afx_msg void OnBUTTONCompute2();
   afx_msg void OnEditchangeSearchCOMBO();
   afx_msg void OnSelchangeSearchCOMBO();
   afx_msg void OnShowCHECK();
   afx_msg void OnBUTTONText();
   afx_msg void OnBUTTONShowOpt();
   afx_msg void OnOutputToFile();
   afx_msg void OnSelchangeLISTRule();
   afx_msg void OnChangeTypeRatio();
   afx_msg void OnChangeSpeedRatio();
   afx_msg void OnOutputFile2();
   afx_msg void OnEditchangeCombo4();
   afx_msg void OnEditchangeCombo1();
   afx_msg void OnSelchangeCombo1();
   afx_msg void OnSelchangeCombo4();
   afx_msg void OnChangeBestSearchEDIT();
   afx_msg void OnClose();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOMPUTESETTINGS_H__D34252E7_5F43_11D4_91F2_0060B0FBBADA__INCLUDED_)
