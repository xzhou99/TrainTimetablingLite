#include "stdafx.h"
#include "rail.h"
#include "node.h"
#include "math.h"
int g_KNodesLimit = 10000;

bool g_SubOptimalEst = false;
void g_SetSubOptimalValue();
int g_SubOptimalSize =0;
int g_CurrentLevel = 0;
int g_BeamWidth = 5;
int g_FilterWidth = 10;
int g_TotalRunTime = 0;
double g_TrainTypeRatio = 0.2;
double g_SpeedRatio=10.0;
int g_RuleNo=0;
double g_Expon(double mean);
int g_Arlang(double Ave,int Order);
bool g_FirstFeasible;
int g_SearchFlag;
double g_Variance;
int g_ComputingNo=0;
int g_EstDelay[MAX_ProblemSize];
bool g_ComputingFlag[MAX_ProblemSize];
int g_ComSuccessFlag[MAX_ProblemSize];
double g_ComputingVariance[MAX_ProblemSize];
int g_ComputingConflict[MAX_ProblemSize];
int g_ComputingSolution[MAX_ProblemSize];
int g_TravelTime[2];
int g_NodeSize[MAX_ProblemSize];
int g_Seed[MAX_ProblemSize];
int g_ComputingTime[MAX_ProblemSize];
int g_OptValue[MAX_ProblemSize];
int g_DelayValue[MAX_ProblemSize];
int g_MethodSize = 4;
int g_CurComputeMethodNo = 0;
int g_ReleaseTimeInterval = 10;
int g_ReleaseTimeOrder=0;
int g_ReleaseTimeUp=0;
int g_ReleaseTimeDown=0;

int g_ComputingTrainSize;
int g_MaxTrainCost=140;
int g_MAX_NodeSize =1000;
int g_TotalConflict;
int g_SelectNodeNo = -1;
int g_SelectTrainNo = -1;
int g_OptTotalCost =99999;
int g_OptTotalCostNodeID =0;
bool g_RemoveTableFlag = true;
char g_InfoStr[500];

int g_CompareFlag = 1;
bool g_bCutSetFlag = true;
bool g_bBestSearch = false;
bool g_LowerBoundFlag = true;

int g_TimeStage = 200;
int g_FirstConflictTime=0;
int g_ConflictTrainSet[2];
int g_FirstConflictStationNo=0;
int g_ConflictFlag = 0;

int g_FirstConflictTime2=0;
int g_ConflictTrainSet2[2];
int g_FirstConflictStationNo2=0;
int g_ConflictFlag2 = 0;

double g_LayerHeight;
double g_LayerWidth;

int g_StationDistance[MAX_STATIONSIZE]={0, 90,220,310,390,490,630,710,830,900};
int g_SectionRunTime[MAX_STATIONSIZE][2];
int g_TotalDistance;


int g_Table[MAX_TRAINSIZE][MAX_STATIONSIZE][4];
int g_Table2[MAX_TRAINSIZE][MAX_STATIONSIZE][4];
int g_TrainSize=0;

int g_TrainDirection[MAX_TRAINSIZE];
int g_TrainWeight[MAX_TRAINSIZE];
int g_TrainType[MAX_TRAINSIZE];
int g_StartTime[MAX_TRAINSIZE];

int g_I=5;
int g_SectionSize =8;

CNode* pNodeRoot;
bool g_ExpendTreeByLayer(int Layer);
void g_CompareTwoNode(int Node1,int Node2);
void g_ShowOptimum();
CNode* g_BeamSort();
CDWordArray g_ComputeAry;

CNode* pWatchNode =NULL;
bool g_ShowMessageFlag = true;

CNode * GetNextNode(int SearchFlag)
{
   bool bBeamSearchFlag=false;
   if(SearchFlag == 2)	//Best first search
   {
      if(!g_FirstFeasible)
	 SearchFlag = 3;
      else
	 SearchFlag = 0;
   }

   if(SearchFlag == 3)	//Priority rules
   {
      if(g_FirstFeasible)   //If get the first feasible solution, stop.
	 return NULL;
      else
	 SearchFlag = 0;

   }


   if(SearchFlag == 4)	//Beam Search
   {
      SearchFlag = 1;  //Depth first search
      bBeamSearchFlag = true;
   }

   int CurrentSize = g_TreeNodeAry.GetSize ();


   if(SearchFlag == 0)
   {
      for(int i=CurrentSize-1; i>=0 ; i--)
      {
	 CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
	 if(pNode->m_ActiveFlag)
	 {
	    //		    TRACE("\n%d",i);
	    return pNode;
	 }

      }
   }

   if(SearchFlag == 1)
   {
      for(int i=0; i<CurrentSize ; i++)
      {
	 CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
	 if(pNode->m_ActiveFlag)
	 {
	    if(bBeamSearchFlag)
	    {
	       if(pNode->m_TreeLayer > g_CurrentLevel) // Computing new level
	       {
		  g_CurrentLevel =  pNode->m_TreeLayer;
		  return g_BeamSort();

	       }

	    }

	    return pNode;

	 }

      }
   }		  return NULL;


   return NULL;
}

CNode* g_BeamSort()
{
   int CurrentSize = g_TreeNodeAry.GetSize ();
   int LiveNodeSize = 0;
   int i;

   double Cost[1000];
   int NodeID[1000];

   for(i=0; i<CurrentSize ; i++)
   {
      CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
      if(pNode->m_ActiveFlag)
      {
	 if(g_RuleNo==0) //Default
	    Cost[LiveNodeSize] = i ;
	 if(g_RuleNo==1) //Best cost
	    Cost[LiveNodeSize] =  pNode->m_TotalCost;
	 if(g_RuleNo==2) // FIFO
	    Cost[LiveNodeSize] =  pNode->m_EarlyStartTime;
	 if(g_RuleNo==3) // FIFO2
	    Cost[LiveNodeSize] =  pNode->m_EarlyCompletionTime;
	 if(g_RuleNo==4) // Minimal processing time
	    Cost[LiveNodeSize] =  pNode->m_EarlyCompletionTime -  pNode->m_EarlyStartTime;

	 if(g_RuleNo==5) // Global evaluation
	    Cost[LiveNodeSize] =  pNode->GetUpperbound();

	 NodeID[LiveNodeSize]= i;
	 LiveNodeSize++;
      }
   }

   if(LiveNodeSize > g_FilterWidth ) //Need beam cut
   {
      TRACE("\nLiveNodeSize at %d :%d",g_CurrentLevel, LiveNodeSize);
      int NodeNo[1000];
      double MinCost= 10000;
      int MinID;
      for(i =0; i< LiveNodeSize; i++)
      {
	 MinCost= 10000;
	 for(int k =0; k< LiveNodeSize; k++)
	 {
	    if(MinCost > Cost[k])
	    {
	       MinCost = Cost[k];
	       MinID = k;
	    }
	 }
	 NodeNo[i] = MinID;
	 TRACE("\nNo.%d = %f",NodeNo[i], Cost[NodeNo[i]]);
	 Cost[MinID] = 20000;
      }

      for(int s= g_BeamWidth; s< LiveNodeSize; s++)
      {
	 CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (NodeID[NodeNo[s]]);
	 ASSERT(pNode->m_ActiveFlag);
	 pNode->m_ActiveFlag = false;
      }
   }


   for(i=0; i<CurrentSize ; i++)
   {
      CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
      if(pNode->m_ActiveFlag)
	 return pNode;

   }

   return NULL;
}

bool g_ComputeLiveNode2()
{
   int CurrentSize = g_TreeNodeAry.GetSize () + g_SubOptimalSize;

   //	    g_MAX_NodeSize = 1000;
   if(CurrentSize >= g_MAX_NodeSize)
   {
      g_OptValue[g_CurComputeMethodNo] = -1;
      g_DelayValue[g_CurComputeMethodNo] = -1;
      g_ComSuccessFlag [g_CurComputeMethodNo] = 0;
      return false;

   }

   if(CurrentSize >= g_KNodesLimit)
   {
      return false;
   }




   CNode* pNode = GetNextNode(g_SearchFlag);
   if(pNode!=NULL)
   {
      //      TRACE("\n Generate from  %d nodes. at %d",pNode->m_NodeID,pNode->m_FirstConflictTime );

      pNode->Bound();
      pNode->ASGenerate ();


      return true;
   }


   return false;
}

void g_TreeSearch2()
{
   CTime t1 = CTime::GetCurrentTime();

   long StartTimeCount=GetTickCount();

   pNodeRoot=new CNode(g_TreeLayer);
   g_TreeNodeAry.Add (pNodeRoot);
   pNodeRoot ->m_LevelNodeID =0;
   pNodeRoot->ASGenerate ();
   int PureRunTime = pNodeRoot->m_TotalCost ;


   int NodeSize =0;

   bool bFinishFlag;
   do{
      bFinishFlag = g_ComputeLiveNode2();
   } while(bFinishFlag);


   g_ShowOptimum();


   CTime t2 = CTime::GetCurrentTime();
   long EndTimeCount=GetTickCount();
   long ElapsedTime =  EndTimeCount - StartTimeCount;

   if(ElapsedTime==0)
      ElapsedTime=1;


   CTimeSpan ts = t2 - t1;

   g_ComputingFlag[g_ComputingNo] = true;
   g_NodeSize[g_ComputingNo]  = g_TreeNodeAry.GetSize () + g_SubOptimalSize;
   g_ComputingConflict[g_ComputingNo] = g_TotalConflict;
   g_ComputingVariance[g_ComputingNo] =g_Variance;
   g_ComputingTime[g_ComputingNo] = ElapsedTime;
   g_OptValue[g_ComputingNo] = g_OptTotalCost;
   g_DelayValue[g_ComputingNo] = g_OptTotalCost - PureRunTime;


   if(g_ShowMessageFlag)
   {
      TRACE("\n--------------------------------------\n");
      TRACE("Generate %d Nodes, %d layers",g_TreeNodeAry.GetSize (),g_TreeLayer);
      if(g_TreeNodeAry.GetSize () >= g_MAX_NodeSize)
	 wsprintf(g_InfoStr,"Exceed MAX Size!!! \n Generate %d Nodes , Currently %d Nodes exit for %d layers, \
		Total %d trains and %d conflicts, Spend %d secs, total %d millisecs",g_TreeNodeID, g_NodeSize[g_ComputingNo],g_TreeLayer,g_TrainSize, g_TotalConflict,ts.GetTotalSeconds (),ElapsedTime);
      else
	 wsprintf(g_InfoStr,"Generate %d Nodes , Currently %d Nodes exit for %d layers, \
		Total %d trains and %d conflicts, Spend %d secs, total %d millisecs",g_TreeNodeID, g_NodeSize[g_ComputingNo],g_TreeLayer,g_TrainSize, g_TotalConflict,ts.GetTotalSeconds (),ElapsedTime);
      AfxMessageBox(g_InfoStr);
   }

}
void g_TreeSearch()
{


}



void g_SetupTrain(int TrainType, int StageNo, int StartTime, int TrainDirection )
{


   int TrainNo = 	g_TrainSize++;

   //	TrainDirection = 0;
   TrainType =  g_GetTrainType(g_TrainTypeRatio);
   g_TrainType[TrainNo]=TrainType;
   g_TrainDirection[TrainNo]=TrainDirection;

   g_StartTime[TrainNo] = StartTime + StageNo * g_TimeStage ;  //Setup time


}


bool bComparedFlag[MAX_TRAINSIZE];

void g_CompareTwoNode(int Node1,int Node2)
{
   // Check the two node have the same train set with different oreder
   int i;

   if(Node1 == Node2)
      return;

   if(Node2 == 0)
      return;


   CNode* pNode1 = (CNode*)g_TreeNodeAry.ElementAt (Node1);
   CNode* pNode2 = (CNode*)g_TreeNodeAry.ElementAt (Node2);


   if(Node1 == pNode2->pParent ->m_NodeID )
      return;


   //	TRACE("\n!!!COMPARE No.%d <> No.%d ",Node1,Node2);



   if(!pNode1->m_bComputeFlag  || !pNode2->m_bComputeFlag )
      return;

   if(pNode1->m_TotalCost >= pNode2->m_TotalCost )
      return;

   if(pNode1->m_TreeLayer != pNode2->m_TreeLayer)
      return;

   int SumFlag=0;

   int CompareSum=0;
   int CompleteCostSum1=0;
   int CompleteCostSum2=0;

   for(i=0; i<g_TrainSize; i++)
   {
      if(pNode1->m_FixingIndex[i] == g_SectionSize-1)
      {
	 bComparedFlag [i] =false;
	 CompleteCostSum1+= pNode1->m_CompletionTime[i];
	 CompleteCostSum2+= pNode2->m_CompletionTime[i];
      }
      else
      {
	 bComparedFlag[i] = true;
	 CompareSum +=1;
      }

   }


   // First conditon: Known data is better than option, otherwise return;
   if(CompleteCostSum1 > CompleteCostSum2) // conditon <
      return;


   //			TRACE("\nWill Compare %d trains",CompareSum);


   int Cost1,Cost2;

   for( i=0;i < g_TrainSize; i++)
   {

      bool bCanCompareFlag = false;

      if(bComparedFlag [i] )
      {

	 if(pNode1->m_FixingIndex [i] < pNode2->m_FixingIndex [i])
	    return;


	 for(int k = pNode2->m_FixingIndex[i]; k < g_SectionSize; k++)
	 {
	    int sta = pNode2->GetSectionNo(i,k);

	    if(pNode2->m_Table[i][sta][3]==0)
	    {
	       Cost1 = pNode1->m_Table[i][sta][0];  // feasible departure time
	       Cost2 = pNode2->m_Table[i][sta][0];  // proposed departure time

	       if(Cost1 > Cost2)
		  return;

	       break;
	    }
	 }



	 int CompleteCost1, CompleteCost2;

	 CompleteCost1 = pNode1->m_CompletionTime[i];
	 CompleteCost2 = pNode2->m_CompletionTime[i];

	 ASSERT(CompleteCost1 <= CompleteCost2);

      }

   }

   pNode2->m_ActiveFlag = false;
   pNode2->m_BetterNodeNo = Node1;
   pNode2->DisableAllChildren();

   //		TRACE("\n!!!Disable Node %d by Node %d \n",Node2,Node1);

}

bool g_TrainSetupFlag = false;
void g_SetupTimeTable(int Seed)
{
   g_SubOptimalSize =0;
   g_RemoveNodeAry();

   int IniOptimalValue = 999999;

   if(g_SubOptimalEst == true)
      g_SetSubOptimalValue();
   IniOptimalValue = g_OptTotalCost;
   TRACE("\n---------------\nSuboptimal:%d\n",g_OptTotalCost);
   g_RemoveNodeAry();

   srand((unsigned)Seed);


   g_OptTotalCost = IniOptimalValue;



   if(!g_TrainSetupFlag)
   {

      int TimeStage=0;
      g_TimeStage = 260;

      int sum =0;
      int variance =0;
      for(int i=0; i < g_ComputingTrainSize/2+1; i++)
      {
	 int UpInterval = (int)g_Arlang(g_ReleaseTimeInterval,g_ReleaseTimeOrder);
	 int DownInterval = (int)g_Arlang(g_ReleaseTimeInterval,g_ReleaseTimeOrder);

	 int mean_rv = g_ReleaseTimeInterval;
	 int rv = UpInterval;
	 sum += rv;
	 variance+=(rv- mean_rv)*(rv- mean_rv);

	 rv = DownInterval;
	 sum += rv;
	 variance+=(rv- mean_rv)*(rv- mean_rv);


	 g_Variance	 = variance*1.0/sum/g_ReleaseTimeInterval;


	 g_ReleaseTimeUp += UpInterval;
	 g_ReleaseTimeDown += DownInterval;

	 TRACE("\n%d    %d",UpInterval,DownInterval);
	 TRACE("     Average:     %d    %d",g_ReleaseTimeUp/(i+1),g_ReleaseTimeDown/(i+1));
	 g_SetupTrain(1,TimeStage,g_ReleaseTimeUp,0);
	 g_SetupTrain(1,TimeStage,g_ReleaseTimeDown,1);
      }


      g_TrainSetupFlag = true;
   }


   //
   g_TrainSize = g_ComputingTrainSize;
   if(g_CurComputeMethodNo==0)
   {
      g_LowerBoundFlag = false;
      g_bCutSetFlag = false;
   }

   if(g_CurComputeMethodNo==1)
   {
      g_LowerBoundFlag = false;
      g_bCutSetFlag = true;
   }

   if(g_CurComputeMethodNo==2)
   {
      g_LowerBoundFlag = true;
      g_bCutSetFlag = false;
   }

   if(g_CurComputeMethodNo==3)
   {
      g_LowerBoundFlag = true;
      g_bCutSetFlag = true;
   }

   /*	if(g_SearchFlag==2)
      {
      g_LowerBoundFlag = true;
      g_bCutSetFlag = false;
      }
      */
   g_TreeSearch2();
   g_SubOptimalSize = g_TreeNodeAry.GetSize ();
}


void g_SetSubOptimalValue()
{

   int old_SearchFlag = g_SearchFlag;
   int old_BeamWidth = g_BeamWidth;


   g_SearchFlag = 4; //Beam Search
   g_FilterWidth = 8;
   g_BeamWidth = g_FilterWidth;
   g_RuleNo = 1;

   g_RemoveNodeAry();

   if(!g_TrainSetupFlag)
   {

      int TimeStage=0;
      g_TimeStage = 260;

      int sum =0;
      int variance =0;
      for(int i=0; i < g_ComputingTrainSize/2+1; i++)
      {
	 int UpInterval = (int)g_Arlang(g_ReleaseTimeInterval,g_ReleaseTimeOrder);
	 int DownInterval = (int)g_Arlang(g_ReleaseTimeInterval,g_ReleaseTimeOrder);

	 int mean_rv = g_ReleaseTimeInterval;
	 int rv = UpInterval;
	 sum += rv;
	 variance+=(rv- mean_rv)*(rv- mean_rv);

	 rv = DownInterval;
	 sum += rv;
	 variance+=(rv- mean_rv)*(rv- mean_rv);


	 g_Variance	 = variance*1.0/sum/g_ReleaseTimeInterval;


	 g_ReleaseTimeUp += UpInterval;
	 g_ReleaseTimeDown += DownInterval;

	 TRACE("\n%d    %d",UpInterval,DownInterval);
	 TRACE("     Average:     %d    %d",g_ReleaseTimeUp/(i+1),g_ReleaseTimeDown/(i+1));
	 g_SetupTrain(1,TimeStage,g_ReleaseTimeUp,0);
	 g_SetupTrain(1,TimeStage,g_ReleaseTimeDown,1);
      }


      g_TrainSetupFlag = true;
   }


   //
   g_TrainSize = g_ComputingTrainSize;
   if(g_CurComputeMethodNo==0)
   {
      g_LowerBoundFlag = false;
      g_bCutSetFlag = false;
   }

   if(g_CurComputeMethodNo==1)
   {
      g_LowerBoundFlag = false;
      g_bCutSetFlag = true;
   }

   if(g_CurComputeMethodNo==2)
   {
      g_LowerBoundFlag = true;
      g_bCutSetFlag = false;
   }

   if(g_CurComputeMethodNo==3)
   {
      g_LowerBoundFlag = true;
      g_bCutSetFlag = true;
   }

   if(g_SearchFlag==2)
   {
      g_LowerBoundFlag = true;
      g_bCutSetFlag = true;
   }

   pNodeRoot=new CNode(g_TreeLayer);
   g_TreeNodeAry.Add (pNodeRoot);
   pNodeRoot ->m_LevelNodeID =0;
   pNodeRoot->ASGenerate ();
   int PureRunTime = pNodeRoot->m_TotalCost ;


   int NodeSize =0;

   bool bFinishFlag;
   do{
      bFinishFlag = g_ComputeLiveNode2();
   } while(bFinishFlag);

   g_SubOptimalSize = g_TreeNodeAry.GetSize ();
   //	g_SubOptimalSize = 0;
   g_SearchFlag = old_SearchFlag;
   g_BeamWidth = old_BeamWidth;
   g_FilterWidth = old_BeamWidth;
}

void g_ShowOptimum()
{
   TRACE("\n-------Show Optimum!--------\n");
   int sum =0;
   for(int i=0; i< g_TreeNodeAry.GetSize (); i++)
   {
      CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
      if(pNode->m_ActiveFlag )
      {
	 TRACE("\nOptimum: Node %d",i);
	 pNode->UpdateTimeTable ();
	 sum +=1;
      }
   }
   if(g_RuleNo >0)
      g_OptTotalCostNodeID = g_TreeNodeAry.GetSize()-1;

   g_SelectNodeNo = g_OptTotalCostNodeID;



   TRACE("\n Computing Size = %d ",g_ComputeAry.GetSize ());


}


void g_RemoveNodeAry()
{
   for(int i=g_TreeNodeAry.GetSize () -1; i >= 0; i--)
   {
      CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (i);
      delete pNode;
   }

   g_TreeNodeAry.RemoveAll( );
   g_ComputeAry.RemoveAll();

   g_TrainSize = 0;
   g_TrainSetupFlag = false;
   g_TreeNodeID=0;
   g_TreeLayer = 0;

   g_CurrentLevel = 0;
   g_SelectNodeNo = -1;
   g_SelectTrainNo = -1;
   g_OptTotalCost =999999;
   g_FirstFeasible = false;
   g_OptTotalCostNodeID =0;


   g_ReleaseTimeUp=0;
   g_ReleaseTimeDown=0;
   g_TimeStage = 200;
}




double g_Expon(double mean)
{
   double z;	 // Uniform random number from 0 to 1

   // Pull a uniform RV (0 < z < 1)
   do
   {
      z = ((double) rand() / RAND_MAX);
   }
   while ((z == 0) || (z == 1));

   return (-mean * log(z));
}




int g_Arlang(double Ave,int Order)
{
   double product=1;

   for(int i=0; i< Order; i++)
   {

      double z;
      do
      {
	 z = ((double) rand() / RAND_MAX);
      }
      while ((z == 0) || (z == 1));


      product *= z;
   }

   if(Order==10)
      return int(Ave);
   else
      return int((-log(product)*Ave)/Order);
}


void g_TestRandomVariable()
{
   int SizeSamples = 5000;
   int mean_rv=40;
   int sum=0;
   int variance=0;
   int order=1;
   for (int i=0; i<SizeSamples; i++)
   {
      int exp_rv =(int) g_Arlang(mean_rv,order);
      sum += exp_rv;
      variance+=(exp_rv- mean_rv)*(exp_rv- mean_rv);

      double k = variance*1.0/sum/mean_rv;
      TRACE("\n No.%d,  %d, Mean=%d  Var=%d   k=%f",i, exp_rv, sum/(i+1), variance/(i+1),k);

   }

   TRACE("");
}



int g_min(int value1, int value2)
{
   int t;

   if(value1 <= value2)
      t = value1;
   else
      t = value2;

   return t;

}

int g_GetTrainType(double ratio)
{
   double    z = ((double) rand() / RAND_MAX);

   if (z< ratio)
      return 0;
   else
      return 1;

}

double g_Average(int Size, int Array[MAX_ProblemSize])
{
   double sum =0 ;
   for(int i=0; i<Size; i++)
   {
      sum += Array[i];
   }

   return sum/Size;

}