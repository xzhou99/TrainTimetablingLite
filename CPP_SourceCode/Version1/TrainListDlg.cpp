// TrainListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rail.h"
#include "TrainListDlg.h"
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
// CTrainListDlg dialog


CTrainListDlg::CTrainListDlg(CWnd* pParent /*=NULL*/)
   : CDialog(CTrainListDlg::IDD, pParent)
{
   //{{AFX_DATA_INIT(CTrainListDlg)
   m_Sum = 0;
   m_BetterNodeNo = 0;
   m_ParentID = 0;
   m_Child1 = 0;
   m_Child2 = 0;
   m_Sum2 = 0;
   m_FinishedCost1 = 0;
   m_FinishedCost2 = 0;
   //}}AFX_DATA_INIT
}


void CTrainListDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CTrainListDlg)
   DDX_Control(pDX, IDC_LISTPO, m_POTrainList);
   DDX_Control(pDX, IDC_LIST2, m_NodeList);
   DDX_Control(pDX, IDC_LIST1, m_TrainList);
   DDX_Text(pDX, IDC_EDITSum, m_Sum);
   DDX_Text(pDX, IDC_EDIT1, m_BetterNodeNo);
   DDX_Text(pDX, IDC_EDITParent, m_ParentID);
   DDX_Text(pDX, IDC_Child1, m_Child1);
   DDX_Text(pDX, IDC_Child2, m_Child2);
   DDX_Text(pDX, IDC_EDITSum2, m_Sum2);
   DDX_Text(pDX, IDC_EDITFCost1, m_FinishedCost1);
   DDX_Text(pDX, IDC_EDITFCost2, m_FinishedCost2);
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrainListDlg, CDialog)
   //{{AFX_MSG_MAP(CTrainListDlg)
   ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeNodeList)
   ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
   ON_BN_CLICKED(IDC_BUTTONOpt, OnBUTTONOpt)
   ON_LBN_SELCHANGE(IDC_LISTPO, OnSelchangeListpo)
   //}}AFX_MSG_MAP
   END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainListDlg message handlers

BOOL CTrainListDlg::OnInitDialog()
{
   CDialog::OnInitDialog();

   char str[300];
   for(int i=0; i<g_TreeNodeAry.GetSize (); i++)
   {
      CNode* pNode = (CNode*)g_TreeNodeAry.GetAt (i);
      if(pNode->m_BetterNodeNo != -1)
	 wsprintf(str,"X No.%d Node",pNode->m_NodeID );
      else
	 wsprintf(str,"  No.%d Node",pNode->m_NodeID );
      //	ASSERT(pNode->m_NodeID == i);
      m_NodeList.AddString (str);
   }

   m_NodeList.SetCurSel(g_SelectNodeNo);
   UpdateList();
   return TRUE;	 // return TRUE unless you set the focus to a control
		 // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrainListDlg::OnSelchangeNodeList()
{
   g_SelectNodeNo = m_NodeList.GetCurSel();

   CNode* pNode = (CNode*)g_TreeNodeAry.GetAt (g_SelectNodeNo);
   pNode->UpdateTimeTable();
   g_pRailViewUp->Invalidate ();
   g_pRailViewDown->Invalidate ();


   UpdateList();


}

void CTrainListDlg::UpdateList()
{
   char str [300];

   m_TrainList.ResetContent ();
   m_POTrainList.ResetContent ();

   if( g_SelectNodeNo <0 )
      return;

   CNode* pNode = (CNode*)g_TreeNodeAry.GetAt (g_SelectNodeNo);
   CNode* pPONode;

   m_BetterNodeNo = pNode->m_BetterNodeNo ;

   if(m_BetterNodeNo == -1)
      pPONode = pNode;
   else
      pPONode = (CNode*)g_TreeNodeAry.GetAt (m_BetterNodeNo);


   m_Sum = pNode->m_TotalCost ;
   m_Sum2 = pPONode->m_TotalCost ;

   m_FinishedCost2 = pPONode->m_FinishCost ;
   m_FinishedCost1 = 0;

   int i;
   for( i=0;i < g_TrainSize; i++)
   {
      if(pPONode->m_TrainStatus [i] == 2)
      {
	 m_FinishedCost1 += pNode->m_CompletionTime [i];
      }
   }


   for(i=0;i < g_TrainSize; i++)
   {
      wsprintf(str,"# %d   Cost: %d",i,pNode->m_CompletionTime [i]);
      m_TrainList.AddString (str);
      m_POTrainList.AddString (str);
      /*
	 if(pNode->m_TrainStatus [i] == 2)
	 wsprintf(str,"# %d   Cost: %d",i,pNode->m_CompletionTime [i]);
	 if(pNode->m_TrainStatus [i] == 1)
	 wsprintf(str,"! %d   Cost: %d",i,pNode->m_CompletionTime [i]);
	 if(pNode->m_TrainStatus [i] == 0)
	 wsprintf(str," %d --  Cost: %d",i,pNode->m_CompletionTime [i]);

	 m_TrainList.AddString (str);

	 if(pPONode->m_TrainStatus [i] == 2)
	 wsprintf(str,"# %d   Cost: %d",i,pPONode->m_CompletionTime [i]);
	 if(pPONode->m_TrainStatus [i] == 1)
	 wsprintf(str,"! %d   Cost: %d",i,pPONode->m_CompletionTime [i]);
	 if(pPONode->m_TrainStatus [i] == 0)
	 wsprintf(str," %d --  Cost: %d",i,pPONode->m_CompletionTime [i]);
	 m_POTrainList.AddString (str);

	 */
   }


   if(pNode->m_NodeID != 0)
      m_ParentID = pNode->pParent ->m_NodeID ;

   m_Child1 = -1;
   m_Child2 = -1;

   if(pNode->m_ChildrenAry .GetSize() > 0)
   {
      CNode* pNode1 =  (CNode*)(pNode->m_ChildrenAry.GetAt(0));
      m_Child1 = pNode1->m_NodeID;

      if(pNode->m_ChildrenAry .GetSize() > 1)
      {
	 CNode* pNode2 =  (CNode*)(pNode->m_ChildrenAry.GetAt(1));
	 m_Child2 = pNode2->m_NodeID;
      }
   }

   UpdateData(false);

   g_pRailViewUp->Invalidate ();
   g_pRailViewDown->Invalidate ();
   g_pTreeView->Invalidate ();
}

void CTrainListDlg::OnSelchangeList1()
{
   g_SelectTrainNo	= m_TrainList.GetCurSel();
   m_POTrainList.SetCurSel(g_SelectTrainNo);
   g_pRailViewUp->Invalidate ();
   g_pRailViewDown->Invalidate ();

}

void CTrainListDlg::OnBUTTONOpt()
{
   g_SelectNodeNo = g_OptTotalCostNodeID;
   m_NodeList.SetCurSel(g_OptTotalCostNodeID);
   CNode* pNode = (CNode*)g_TreeNodeAry.GetAt (g_SelectNodeNo);
   pNode->UpdateTimeTable();
   UpdateList();
}

void CTrainListDlg::OnSelchangeListpo()
{
   g_SelectTrainNo	= m_POTrainList.GetCurSel();
   m_TrainList.SetCurSel(g_SelectTrainNo);
   g_pRailViewUp->Invalidate ();
   g_pRailViewDown->Invalidate ();

}
