// DlgText.cpp : implementation file
//

#include "stdafx.h"
#include "rail.h"
#include "DlgText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgText dialog


CDlgText::CDlgText(CWnd* pParent /*=NULL*/)
   : CDialog(CDlgText::IDD, pParent)
{
   //{{AFX_DATA_INIT(CDlgText)
   //}}AFX_DATA_INIT
}


void CDlgText::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CDlgText)
   DDX_Control(pDX, IDC_LIST1, m_List);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgText, CDialog)
   //{{AFX_MSG_MAP(CDlgText)
   //}}AFX_MSG_MAP
   END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgText message handlers

void CDlgText::OnOK()
{

   CDialog::OnOK();
}
