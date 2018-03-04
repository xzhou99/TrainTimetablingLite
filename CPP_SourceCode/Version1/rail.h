// rail.h : main header file for the RAIL application
//

#if !defined(AFX_RAIL_H__40C3DFF2_4EA4_11D4_91E5_0060B0FBBAC9__INCLUDED_)
#define AFX_RAIL_H__40C3DFF2_4EA4_11D4_91E5_0060B0FBBAC9__INCLUDED_

#if _MSC_VER > 1000
#pragma	once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRailApp:
// See rail.cpp for the implementation of this class
//

class CRailApp : public CWinApp
{
public:
   void Setup();
   CRailApp();

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CRailApp)
public:
   virtual BOOL InitInstance();
   //}}AFX_VIRTUAL

   // Implementation
   //{{AFX_MSG(CRailApp)
   afx_msg void OnAppAbout();
   // NOTE - the ClassWizard will add and remove member functions here.
   //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#include "define.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAIL_H__40C3DFF2_4EA4_11D4_91E5_0060B0FBBAC9__INCLUDED_)
