// DlgComputeSettings.cpp : implementation file
//

#include "stdafx.h"
#include "rail.h"
#include "TreeView.h"
#include "Node.h"
#include "DlgComputeSettings.h"
#include "railDoc.h"
#include "railView.h"
#include  <stdio.h>
#include  <math.h>

extern CObArray g_TreeNodeAry;
extern TreeView* g_pTreeView;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgComputeSettings dialog



CDlgComputeSettings::CDlgComputeSettings(CWnd* pParent /*=NULL*/)
   : CDialog(CDlgComputeSettings::IDD, pParent)
{
   m_ShowTree = false;
   //{{AFX_DATA_INIT(CDlgComputeSettings)
   m_Number = 10;
   m_Seed = 123;
   m_TypeRatio = (int)(100*g_TrainTypeRatio);
   m_SpeedRatio = (int)(100*g_SpeedRatio);
   m_BestSearch = FALSE;
   m_FilterWidth = 8;
   m_BeamWidth = 2;
   m_MinTrainNum=7;
   m_MaxTrainNum=8;
   g_SubOptimalEst = FALSE;
   m_K = 1000000;
   //}}AFX_DATA_INIT
}


void CDlgComputeSettings::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(CDlgComputeSettings)
   DDX_Control(pDX, IDC_COMBO4, m_Combo4);
   DDX_Control(pDX, IDC_LISTRule, m_RuleList);
   DDX_Control(pDX, IDC_COMBO3, m_Combo3);
   DDX_Control(pDX, IDC_SearchCOMBO, m_SearchCombo);
   DDX_Control(pDX, IDC_COMBO2, m_Combo2);
   DDX_Control(pDX, IDC_COMBO1, m_Combo);
   DDX_Control(pDX, IDC_LIST2, m_List2);
   DDX_Control(pDX, IDC_LIST1, m_List1);
   DDX_Text(pDX, IDC_EDITNum, m_Number);
   DDV_MinMaxInt(pDX, m_Number, 1, 1000);
   DDX_Text(pDX, IDC_Seed, m_Seed);
   DDV_MinMaxInt(pDX, m_Seed, 1, 100000);
   DDX_Text(pDX, IDC_TypeRatio, m_TypeRatio);
   DDV_MinMaxInt(pDX, m_TypeRatio, 0, 100);
   DDX_Text(pDX, IDC_SpeedRatio, m_SpeedRatio);
   DDV_MinMaxInt(pDX, m_SpeedRatio, 0, 200000);
   DDX_Text(pDX, IDC_BestSearchEDIT, m_BestSearch);
   DDX_Text(pDX, IDC_FilterWidth, m_FilterWidth);
   DDV_MinMaxInt(pDX, m_FilterWidth, 1, 100);
   DDX_Text(pDX, IDC_BeamWidth, m_BeamWidth);
   DDV_MinMaxInt(pDX, m_BeamWidth, 1, 100);
   DDX_Check(pDX, IDC_BestCHECK, m_SubOptimalEst);
   DDX_Text(pDX, IDC_KNum, m_K);
   DDV_MinMaxInt(pDX, m_K, 0, 1000000);
   //}}AFX_DATA_MAP

   g_BeamWidth = m_BeamWidth;
   g_FilterWidth = m_FilterWidth;
   g_SubOptimalEst = m_SubOptimalEst;
   g_KNodesLimit = m_K;

}


BEGIN_MESSAGE_MAP(CDlgComputeSettings, CDialog)
   //{{AFX_MSG_MAP(CDlgComputeSettings)
   ON_BN_CLICKED(IDC_BUTTONCompute, OnBUTTONCompute)
   ON_BN_CLICKED(IDC_BUTTONComputeAll, OnBUTTONComputeAll)
   ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
   ON_BN_CLICKED(IDC_BUTTONCompute2, OnBUTTONCompute2)
   ON_CBN_EDITCHANGE(IDC_SearchCOMBO, OnEditchangeSearchCOMBO)
   ON_CBN_SELCHANGE(IDC_SearchCOMBO, OnSelchangeSearchCOMBO)
   ON_BN_CLICKED(IDC_ShowCHECK, OnShowCHECK)
   ON_BN_CLICKED(IDC_BUTTONText, OnBUTTONText)
   ON_BN_CLICKED(IDC_BUTTONShowOpt, OnBUTTONShowOpt)
   ON_BN_CLICKED(IDC_BUTTONCompute3, OnOutputToFile)
   ON_LBN_SELCHANGE(IDC_LISTRule, OnSelchangeLISTRule)
   ON_EN_CHANGE(IDC_TypeRatio, OnChangeTypeRatio)
   ON_EN_CHANGE(IDC_SpeedRatio, OnChangeSpeedRatio)
   ON_BN_CLICKED(IDC_OutputFile2, OnOutputFile2)
   ON_CBN_EDITCHANGE(IDC_COMBO4, OnEditchangeCombo4)
   ON_CBN_EDITCHANGE(IDC_COMBO1, OnEditchangeCombo1)
   ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
   ON_CBN_SELCHANGE(IDC_COMBO4, OnSelchangeCombo4)
   ON_EN_CHANGE(IDC_BestSearchEDIT, OnChangeBestSearchEDIT)
   ON_WM_CLOSE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgComputeSettings message handlers

void CDlgComputeSettings::OnOK()
{

   CDialog::OnOK();
   delete p_Textdlg;

}

BOOL CDlgComputeSettings::OnInitDialog()
{
   p_Textdlg = new CDlgText;
   p_Textdlg->Create(IDD_TextDIALOG,this);



   CDialog::OnInitDialog();

   m_SearchCombo.AddString("Depth First Search");
   m_SearchCombo.AddString("Breadth First Search");
   m_SearchCombo.AddString("Best First Search");
   m_SearchCombo.AddString("Priority Rule");
   m_SearchCombo.AddString("Beam Search");

   m_SearchCombo.SetCurSel(0);
   m_List1.AddString("Base Search");
   m_List1.AddString("Search with Cutset");
   m_List1.AddString("Search with LowerBound");
   m_List1.AddString("Search with LowerBound + Cutset");
   m_List1.SetCurSel(0);

   m_RuleList.AddString("Random");
   m_RuleList.AddString("BestCost");
   m_RuleList.AddString("LArrival");
   m_RuleList.AddString("LDeparture");
   m_RuleList.AddString("Min processing time");
   m_RuleList.AddString("Global Evaluation");

   m_RuleList.SetCurSel(g_RuleNo);
   CString strFreeMemory;
   CString strFmt;

   // Fill available memory
   MEMORYSTATUS MemStat;
   MemStat.dwLength = sizeof(MEMORYSTATUS);
   GlobalMemoryStatus(&MemStat);
   strFmt.LoadString(CG_IDS_PHYSICAL_MEM);
   m_Memory1 = MemStat.dwTotalPhys;
   strFreeMemory.Format(strFmt, MemStat.dwTotalPhys / 1024L);
   SetDlgItemText(IDC_PHYSICAL_MEM, strFreeMemory);

   strFreeMemory.Format(strFmt, MemStat.dwAvailPhys  / 1024L);
   SetDlgItemText(IDC_PHYSICAL_MEM2, strFreeMemory);
   m_Memory2 = MemStat.dwAvailPhys;

   double Size = 800 * 250;
   g_MAX_NodeSize = (int)(m_Memory2 /3600 * 0.85);
   strFmt.Format ("%d K",(int)Size);
   SetDlgItemText(IDC_NodeSize2, strFmt);

   int i;
   for(i =1; i< MAX_TRAINSIZE; i++)
   {
      char str[20];
      wsprintf(str,"%d",i);
      m_Combo.AddString(str);
      m_Combo4.AddString(str);

   }
   m_Combo.SetCurSel(6);
   m_Combo4.SetCurSel(6);


   for( i =1; i<= 10; i++)
   {
      char str[20];
      wsprintf(str,"%d",i);
      m_Combo3.AddString(str);

   }
   m_Combo3.SetCurSel(3);

   for(i =1; i<= 12; i++)
   {
      char str[20];
      wsprintf(str,"%d",i*10);
      m_Combo2.AddString(str);
   }

   m_Combo2.SetCurSel(5);

   UpdateList();

   return TRUE;	 // return TRUE unless you set the focus to a control
		 // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgComputeSettings::OnBUTTONCompute()
{
   UpdateData(true);
   srand((unsigned)m_Seed);

   CWaitCursor cursor;


   g_ComputingTrainSize = m_Combo.GetCurSel() +1;
   g_ReleaseTimeInterval = (m_Combo2.GetCurSel()+1) * 10;
   g_ReleaseTimeOrder = m_Combo3.GetCurSel()+1;

   g_ShowMessageFlag = true;

   CString strFreeMemory;
   CString strFmt;



   m_SelNo = m_List1.GetCurSel();
   g_CurComputeMethodNo = m_SelNo;

   g_SetupTimeTable(m_Seed);
   g_pTreeView ->Invalidate ();

   MEMORYSTATUS MemStat;
   MemStat.dwLength = sizeof(MEMORYSTATUS);
   GlobalMemoryStatus(&MemStat);
   strFreeMemory.Format("%d KB", MemStat.dwAvailPhys  / 1024L);
   //	SetDlgItemText(IDC_PHYSICAL_MEM3, strFreeMemory);
   m_Memory3 = MemStat.dwAvailPhys;
   m_Memory4 = m_Memory3 - m_Memory2;
   strFreeMemory.Format ("%d KB",m_Memory4 / 1024L);
   //	SetDlgItemText(IDC_PHYSICAL_MEM4, strFreeMemory);

   int Size = g_TreeNodeAry.GetSize();

   if(Size> 0)
   {
      strFmt.Format ("%d",Size);
      //	SetDlgItemText(IDC_NodeSize, strFmt);

   }

   UpdateList();
   m_List2.SetCurSel(m_SelNo);
}

void CDlgComputeSettings::UpdateList()
{
   m_List2.ResetContent();
   for(int i =0; i< g_MethodSize;i++)
   {
      char str[300];


      if(g_ComputingFlag[i])
      {
	 if(g_ComputingFlag[0])
	    sprintf(str,"Space: %d (%4.3f%)  ,Time: %d (sec), Opt: %d",g_NodeSize[i],g_NodeSize[i]*1.0/g_NodeSize[0],g_ComputingTime[i],g_OptValue[i]);
	 else
	    wsprintf(str,"Space: %d  ,Time: %d (sec), Opt: %d",g_NodeSize[i],g_ComputingTime[i],g_OptValue[i]);

	 p_Textdlg->m_List.AddString(str);


      }
      else
	 wsprintf(str,"Not Available.");
      m_List2.AddString(str);

   }
}

void CDlgComputeSettings::OnBUTTONComputeAll()
{
   UpdateData(true);
   srand((unsigned)m_Seed);

   g_ComputingNo =0;
   g_ComputingTrainSize = m_Combo.GetCurSel() +1;
   g_ReleaseTimeInterval = (m_Combo2.GetCurSel()+1) * 10;
   g_ReleaseTimeOrder = m_Combo3.GetCurSel()+1;

   CWaitCursor cursor;

   g_ShowMessageFlag = false;
   for(int i =0; i<g_MethodSize; i++)
   {
      g_ComputingNo = i;
      g_CurComputeMethodNo = i;
      g_SetupTimeTable(m_Seed);
   }

   g_pTreeView ->Invalidate ();

   UpdateList();

}

void CDlgComputeSettings::OnSelchangeList1()
{
   m_List2.SetCurSel(m_List1.GetCurSel());
}

void CDlgComputeSettings::OnBUTTONCompute2()
{
   UpdateData(true);


   g_ShowMessageFlag = false;
   g_ComputingNo =0;
   int MaxTrainSize = m_Combo.GetCurSel() +1;
   g_ReleaseTimeInterval = (m_Combo2.GetCurSel()+1) * 10;
   g_ReleaseTimeOrder = m_Combo3.GetCurSel()+1;

   int ComputeNumber = m_Number;

   CWaitCursor cursor;

   g_MethodSize = MaxTrainSize;

   int SuccessNum=ComputeNumber;
   int i;
   for(i =0; i<ComputeNumber; i++)
   {
      srand((unsigned)m_Seed+i*100);
      g_ComputingNo = i;
      g_ComputingTrainSize = MaxTrainSize;
      g_RemoveNodeAry();
      g_CurComputeMethodNo = m_List1.GetCurSel();
      g_SetupTimeTable(m_Seed+i*100);

      int NodeSize = g_TreeNodeAry.GetSize ();

      if(NodeSize >= g_MAX_NodeSize)
      {
	 SuccessNum = i;
	 //   break;
      }


   }
   //	g_RemoveNodeAry();

   char str1[800];
   char str2[800];
   char str3[800];
   char str4[800];

   sprintf(str1," ");
   sprintf(str2," ");
   sprintf(str3," ");
   sprintf(str4," ");
   for(i =0; i<ComputeNumber; i++)
   {

      sprintf(str1,"%s %d ",str1,g_NodeSize[i]);
      sprintf(str2,"%s %d",str2,g_ComputingTime[i]);
      sprintf(str3,"%s %d ",str3,g_DelayValue[i]);
      sprintf(str4,"%s %d ",str4,g_ComputingConflict[i]);
   }

   p_Textdlg->m_List.AddString(str1);
   p_Textdlg->m_List.AddString(str2);
   p_Textdlg->m_List.AddString(str3);

   /*	 sprintf(str1," %s \n   %10.5f",str1,g_Average(ComputeNumber,g_NodeSize));
      sprintf(str2," %s \n   %10.5f",str2,g_Average(ComputeNumber,g_ComputingConflict));
      sprintf(str3," %s \n   %10.5f",str3,g_Average(ComputeNumber,g_DelayValue));
      */

   SetDlgItemText(IDC_Result,str1);
   SetDlgItemText(IDC_Result2,str2);
   SetDlgItemText(IDC_Result3,str3);
   SetDlgItemText(IDC_Result4,str4);
}

void CDlgComputeSettings::OnEditchangeSearchCOMBO()
{
}

void CDlgComputeSettings::OnSelchangeSearchCOMBO()
{
   g_SearchFlag = m_SearchCombo.GetCurSel();

}

void CDlgComputeSettings::OnShowCHECK()
{

   m_ShowTree = !m_ShowTree;
   g_pTreeView->m_bDraw  = m_ShowTree;
   g_pTreeView->Invalidate(false);

   UpdateData(true);


}

void CDlgComputeSettings::OnBUTTONText()
{

   p_Textdlg->ShowWindow(SW_SHOW);
}

void CDlgComputeSettings::OnBUTTONShowOpt()
{
   g_SelectNodeNo = g_OptTotalCostNodeID;
   CNode* pNode = (CNode*)g_TreeNodeAry.GetAt (g_SelectNodeNo);
   pNode->UpdateTimeTable();
   g_pRailViewUp->Invalidate();

   g_pRailViewDown->Invalidate();


}

double g_GetAverage(int value[MAX_ProblemSize],int Size)
{
   double sum=0;
   int  count=0;

   for(int i=0;i<Size;i++)
   {
      if(g_ComSuccessFlag[i]==1)
      {
	 sum+= value[i];
	 count += 1;

      }

   }

   double Average;
   if(count > 0)
      Average= sum / count;
   else
      Average= 0;

   return Average;
}

double g_GetDeviation(int value[MAX_ProblemSize],int Size)
{
   double sum=0;
   int  count=0;

   double mean = g_GetAverage(value,Size);

   for(int i=0;i<Size;i++)
   {
      if(g_ComSuccessFlag[i]==1)
      {
	 sum+= (value[i]-mean)*(value[i]-mean);
	 count += 1;

      }

   }

   double Deviation;

   if(count > 0)
      Deviation = pow(sum / count,0.5);
   else
      Deviation = 0;

   return Deviation;
}

double g_GetSuccRatio(int value[MAX_ProblemSize],int Size)
{
   double sum=0;
   int  count=0;

   for(int i=0;i<Size;i++)
   {
      if(g_ComSuccessFlag[i]==1)
      {
	 sum+= value[i];
	 count += 1;

      }

   }

   double Average = sum / Size;
   return Average;
}

void CDlgComputeSettings::OnOutputToFile()
{
   FILE * pfile;
   pfile = fopen("TestSpace.dat","w" );
   fclose(pfile);
   pfile = fopen("TestTime.dat","w" );
   fclose(pfile);
   pfile = fopen("TestConflict.dat","w" );
   fclose(pfile);
   pfile = fopen("TestSummary.dat","w");
   fclose(pfile);
   pfile = fopen("TestSolution.dat","w");
   fclose(pfile);


   UpdateData(true);
   srand((unsigned)m_Seed);

   g_ShowMessageFlag = false;
   g_ComputingNo =0;

   int MaxTrainSize = m_Combo.GetCurSel() +1;
   g_ReleaseTimeInterval = (m_Combo2.GetCurSel()+1) * 10;
   g_ReleaseTimeOrder = m_Combo3.GetCurSel()+1;
   m_SelNo = m_List1.GetCurSel();
   g_CurComputeMethodNo = m_SelNo;

   int ComputeNumber = m_Number;

   CWaitCursor cursor;
   /*
      g_bBestSearch = false;

      TestOneRun(g_SearchFlag ,  g_CurComputeMethodNo,ComputeNumber );
      */

   TestOneRun(g_SearchFlag ,  g_CurComputeMethodNo,ComputeNumber );

   MessageBox(" Complete. ");

}


void CDlgComputeSettings::TestOneRun(int g_SearchFlag , int g_CurComputeMethodNo,int ComputeNumber )
{

   FILE * pfile;

   char str1[30];
   char str2[30];

   char title[200];

   m_SearchCombo.GetLBText(g_SearchFlag,str1);


   m_List1.GetText(g_CurComputeMethodNo,str2);

   if(g_bBestSearch)
      wsprintf(title, " \n-----------------------------------------\n Test %s AND %s with Best Search \n\n",str1,str2);
   else
      wsprintf(title, " \n-----------------------------------------\n Test %s AND %s without Best Search \n\n",str1,str2);

   pfile = fopen("TestSpace.dat","a" );
   fprintf(pfile, "%s",title);
   fclose(pfile);

   pfile = fopen("TestTime.dat","a" );
   fprintf(pfile, "%s",title);
   fclose(pfile);

   pfile = fopen("TestConflict.dat","a" );
   fprintf(pfile, "%s",title);
   fclose(pfile);

   pfile = fopen("TestSummary.dat","a" );
   fprintf(pfile, "%s",title);
   fclose(pfile);

   pfile = fopen("TestSolution.dat","a" );
   fprintf(pfile, "%s",title);
   fclose(pfile);
   int t;
   int i;
   for(t = m_MinTrainNum; t <= m_MaxTrainNum; t+=1)
   {

      UpdateData(true);
      srand((unsigned)m_Seed);

      for(i =0; i<ComputeNumber; i++)
      {
	 g_ComSuccessFlag [i] = 1;
	 g_ComputingNo = i;
	 g_ComputingTrainSize = t;
	 g_CurComputeMethodNo = m_List1.GetCurSel();
	 g_SetupTimeTable(m_Seed);

	 int NodeSize = g_TreeNodeAry.GetSize ();

	 if(NodeSize >= g_MAX_NodeSize)
	 {
	    NodeSize = 9999999;
	    g_NodeSize[i]=9999999;
	    g_ComputingTime[i] = 9999999;
	    g_ComputingConflict[i]= 9999999;
	    g_ComSuccessFlag [i] = 0;
	    g_OptValue[i]=9999999;

	 }

      }


      pfile = fopen("TestSpace.dat","a" );
      fprintf(pfile, "#%d    ",t);
      for(i =0; i<ComputeNumber; i++)
	 fprintf(pfile, "%10d ",g_NodeSize[i]);
      fprintf(pfile, "\n");
      fclose( pfile );

      pfile = fopen("TestTime.dat","a");
      fprintf(pfile, "#%d    ",t);
      for(i =0; i<ComputeNumber; i++)
	 fprintf(pfile, "%10.3f ",g_ComputingTime[i]/1000.0);
      fprintf(pfile, "\n");
      fclose( pfile );

      pfile = fopen("TestConflict.dat","a");
      fprintf(pfile, "#%d    ",t);
      for(i =0; i<ComputeNumber; i++)
	 fprintf(pfile, "%10d ",g_ComputingConflict[i]);
      fprintf(pfile, "\n");
      fclose( pfile );

      pfile = fopen("TestSolution.dat","a");
      fprintf(pfile, "#%d    ",t);
      for(i =0; i<ComputeNumber; i++)
	 fprintf(pfile, "%10d ",g_OptValue[i]);
      fprintf(pfile, "\n");
      fclose( pfile );

      pfile = fopen("TestSummary.dat","a");
      fprintf(pfile, "#%2d Success:%10.3f  >> Average  space:%10.3f   time:%10.3f   conflict:%10.3f    >> Deviation:  space:%10.3f   time:%10.3f   conflict:%10.3f   \n",
	      t,g_GetSuccRatio(g_ComSuccessFlag,ComputeNumber),g_GetAverage(g_NodeSize,ComputeNumber),g_GetAverage(g_ComputingTime,ComputeNumber),g_GetAverage(g_ComputingConflict,ComputeNumber),
	      g_GetDeviation(g_NodeSize,ComputeNumber),g_GetDeviation(g_ComputingTime,ComputeNumber),g_GetDeviation(g_ComputingConflict,ComputeNumber));
      fclose( pfile );


   }


}

void CDlgComputeSettings::OnSelchangeLISTRule()
{
   g_RuleNo = m_RuleList.GetCurSel();

}

void CDlgComputeSettings::OnChangeTypeRatio()
{
   UpdateData(true);
   g_TrainTypeRatio = m_TypeRatio / 100.0;
}

void CDlgComputeSettings::OnChangeSpeedRatio()
{
   UpdateData(true);
   g_SpeedRatio = m_SpeedRatio / 100.0;

}

void CDlgComputeSettings::OnOutputFile2()
{
   UpdateData(true);
   srand((unsigned)m_Seed);

   g_ShowMessageFlag = false;
   g_ComputingNo =0;

   int MaxTrainSize = m_Combo.GetCurSel() +1;
   g_ReleaseTimeInterval = (m_Combo2.GetCurSel()+1) * 10;
   g_ReleaseTimeOrder = m_Combo3.GetCurSel()+1;

   int ComputeNumber = m_Number;

   CWaitCursor cursor;

   FILE * pfile;

   char str1[30];
   char str2[30];

   char title[200];

   m_SearchCombo.GetLBText(g_SearchFlag,str1);
   m_List1.GetText(g_CurComputeMethodNo,str2);

   wsprintf(title, " \n-----------------------------------------\nTest %s AND %s \n\n",str1,str2);

   pfile = fopen("TestSpeedRatio.dat","w" );
   fprintf(pfile, "%s   %d trains",title, MaxTrainSize);
   fprintf(pfile, "       Error:          Derivation:            Space:           Time:          \n\n");
   fclose( pfile );



   for(int t = m_MinTrainNum; t<= m_MaxTrainNum; t++)
   {
      bool SuccessFlag[1000];
      double ExactSolution[100];
      double ExactTime[1000];
      double ExactSpace[1000];

      fprintf(pfile, "\n\n\n---------TrainSize:%d-------------\n\n", t);


      ComputeNumber = 30;

      int Count=0;
      for (int s=0; s<=20; s++)
      {
	 int FeaCount=0;

	 pfile = fopen("TestSpeedRatio.dat","a" );

	 fprintf(pfile, "#%3.2f  ",(20-s)/10.0);
	 fclose( pfile );

	 g_SpeedRatio = 2-s/10.0;
	 srand((unsigned)m_Seed);

	 double SumDerivation=0;
	 double SumError=0;
	 double SumSpace=0;
	 double SumTime=0;

	 for(int i =0; i<ComputeNumber; i++)
	 {

	    if(s==0)
	       SuccessFlag[i] = true;

	    g_ComSuccessFlag [i] = 1;
	    g_ComputingNo = i;
	    g_ComputingTrainSize = t;
	    g_CurComputeMethodNo = m_List1.GetCurSel();
	    g_SetupTimeTable(m_Seed);

	    int NodeSize = g_TreeNodeAry.GetSize ();



	    if(NodeSize >= g_MAX_NodeSize && !SuccessFlag[i])
	    {
	       if(s==0)
	       {
		  SuccessFlag[i] = false;
	       }

	       NodeSize = 9999999;
	       g_NodeSize[i]=9999999;
	       g_ComputingTime[i] = 9999999;
	       g_ComputingConflict[i]= 9999999;
	       g_ComSuccessFlag [i] = 0;
	       g_OptValue[i]=9999999;

	    }

	    if(s==0)
	    {
	       ExactSolution[i]= g_OptValue[i];
	       ExactTime[i]= g_ComputingTime[i];
	       ExactSpace[i]= g_NodeSize[i];
	       ASSERT(g_ComputingTime[i]>0);
	    }

	    if(s==0 && SuccessFlag[i])
	       Count +=1;

	    pfile = fopen("TestSpeedRatio.dat","a" );

	    if(SuccessFlag[i])
	    {
	       if( g_FirstFeasible)
	       {
		  SumDerivation+=g_OptValue[i]*1.0/ExactSolution[i];
		  SumTime+=g_ComputingTime[i]*1.0/ExactTime[i];
		  SumSpace+=g_NodeSize[i]*1.0/ExactSpace[i];

		  FeaCount+=1;
	       }
	       else
		  SumError+=1;
	    }


	    if(SuccessFlag[i])
	    {
	       if( !g_FirstFeasible)
		  fprintf(pfile, "%10.5f ",999.9);
	       else
		  fprintf(pfile, "%10.5f ",g_ComputingTime[i]*1.0);
	    }
	    else
	       fprintf(pfile, "%10.5f ",999.9);


	    fclose( pfile );
	 }

	 pfile = fopen("TestSpeedRatio.dat","a" );

	 if(Count>0)
	    fprintf(pfile, "      %10.5f",SumError/Count);
	 else
	    fprintf(pfile, "      %10.5f",-1.0);

	 if(FeaCount>0)
	    fprintf(pfile, "            %10.5f       %10.5f      %10.5f ",SumDerivation/FeaCount,SumSpace/FeaCount,SumTime/FeaCount);
	 else
	    fprintf(pfile, "            %10.5f       %10.5f      %10.5f ",-1.0,-1.0,-1.0);

	 fprintf(pfile, "\n");
	 fclose( pfile );

      }
   }

   MessageBox("                     Game is over!                   ");

}

void CDlgComputeSettings::OnEditchangeCombo4()
{

}

void CDlgComputeSettings::OnEditchangeCombo1()
{

}

void CDlgComputeSettings::OnSelchangeCombo1()
{
   m_MinTrainNum = m_Combo4.GetCurSel()+1;
   m_MaxTrainNum = m_Combo.GetCurSel()+1;

}

void CDlgComputeSettings::OnSelchangeCombo4()
{
   m_MinTrainNum = m_Combo4.GetCurSel()+1;
   m_MaxTrainNum = m_Combo.GetCurSel()+1;

}

void CDlgComputeSettings::OnChangeBestSearchEDIT()
{
   UpdateData(true);
   g_bBestSearch =m_BestSearch;
}

void CDlgComputeSettings::OnClose()
{

   //	CDialog::OnClose();
}
