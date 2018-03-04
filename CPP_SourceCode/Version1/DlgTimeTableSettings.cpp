// DlgTimeTableSettings.cpp : implementation file
//

#include "stdafx.h"
#include "rail.h"
#include "DlgTimeTableSettings.h"
#include "TreeView.h"
#include "Node.h"
#include "define.h"
#include "railDoc.h"
#include "railView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTimeTableSettings dialog


CDlgTimeTableSettings::CDlgTimeTableSettings(CWnd* pParent /*=NULL*/)
   : CDialog(CDlgTimeTableSettings::IDD, pParent)
{
   //{{AFX_DATA_INIT(CDlgTimeTableSettings)
   // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
}


void CDlgTimeTableSettings::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CDlgTimeTableSettings)
   DDX_Control(pDX, IDC_LISTGrid, m_ListGrid);
   DDX_Control(pDX, IDC_LIST3, m_RightList);
   DDX_Control(pDX, IDC_LIST1, m_LeftList);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTimeTableSettings, CDialog)
   //{{AFX_MSG_MAP(CDlgTimeTableSettings)
   ON_BN_CLICKED(IDApply, OnApply)
   ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
   ON_LBN_SELCHANGE(IDC_LIST3, OnSelchangeList3)
   ON_LBN_SELCHANGE(IDC_LISTGrid, OnSelchangeLISTGrid)
   //}}AFX_MSG_MAP
   END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTimeTableSettings message handlers

BOOL CDlgTimeTableSettings::OnInitDialog()
{

   CDialog::OnInitDialog();

   char str[20];
   for(int i =0; i< 24; i++)
   {
      wsprintf(str,"%d",i);
      m_LeftList.AddString (str);

      wsprintf(str,"%d",i+1);
      m_RightList.AddString (str);

   }


   m_LeftList.SetCurSel(0);
   m_RightList.SetCurSel(3);


   m_ListGrid.AddString("2 Min");
   m_ListGrid.AddString("10 Min");
   m_ListGrid.AddString("60 Min");
   m_ListGrid.SetCurSel(1);



   return TRUE;	 // return TRUE unless you set the focus to a control
		 // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTimeTableSettings::UpdateView()
{
   int LeftTime = m_LeftList.GetCurSel();
   int RightTime = m_RightList.GetCurSel() + 1 ;

   if(LeftTime >= RightTime)
      return;


   g_pRailViewUp ->m_TmLeft = LeftTime * 60;
   g_pRailViewUp ->m_TmRight = RightTime * 60;
   g_pRailViewUp ->m_GridFlag = m_ListGrid.GetCurSel();
   g_pRailViewUp->Invalidate ();

   g_pRailViewDown ->m_TmLeft = LeftTime * 60;
   g_pRailViewDown ->m_TmRight = RightTime * 60;
   g_pRailViewDown ->m_GridFlag = m_ListGrid.GetCurSel();
   g_pRailViewDown->Invalidate ();
}

void CDlgTimeTableSettings::OnApply()
{
   UpdateView();

}

void CDlgTimeTableSettings::OnSelchangeList1()
{
   UpdateView();

}

void CDlgTimeTableSettings::OnSelchangeList3()
{
   UpdateView();
}

void CDlgTimeTableSettings::OnSelchangeLISTGrid()
{
   UpdateView();

}
