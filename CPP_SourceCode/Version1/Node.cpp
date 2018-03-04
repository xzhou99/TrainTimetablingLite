#include "stdafx.h"
#include "rail.h"
#include "Node.h"
#include "define.h"
#include <malloc.h>
#include <memory.h>
#include <math.h>
extern CDWordArray g_ComputeAry;
extern int ge_CalculateUpperbound(CNode* pIniRoot);

////  To Save the memory
//
CPen g_PenNodeActive(PS_SOLID,1,RGB(0,255,0));
CPen g_PenNodeSelect(PS_SOLID,2,RGB(255,0,0));
CBrush NormalBrush(RGB(255,255,255));
CBrush ActiveBrush(RGB(0,255,0));
bool g_CompleteTreeDrawFlag = false;

CPen g_PenOptRelation(PS_DASH,1,RGB(255,255,255));


int g_TreeNodeID=0;
int g_TreeLayer = 0;

CObArray g_TreeNodeAry;

CTask::CTask(int TrainNo,int SectionNo,int ReleaseTime, int CompletionTime)
{
   m_TrainNo = TrainNo;
   m_SectionNo = SectionNo;
   m_ReleaseTime = ReleaseTime;
   m_CompletionTime = CompletionTime;
}


CNode::CNode(int Layer)
{
   m_GlobleEstFlag = false;
   m_ConflictTrainSize=0;
   m_bComparable = false;
   m_bTableExist = false;
   m_bSetupChildFlag = false;
   m_bComputeFlag = false;
   m_DecisionFlag=0;
   // Setup pointer for array


   m_bConflict = false;
   m_FirstConflictTime =  9999;
   m_ConflictSectionNo = -10;

   ///////////////////////////////////
   // Setup Node ID

   m_NodeID = g_TreeNodeID;

   if(m_NodeID == 0)
      InitializeRoot();


   g_TreeNodeID += 1;

   m_TreeLayer	= Layer;

   m_ActiveFlag = true;

   m_CompleteFlag = false;

   m_bSelectFlag = false;
   pParent = NULL;

   m_BetterNodeNo = -1;

   for( int i=0; i<g_TrainSize;i++)
   {
      m_FixingIndex [i]= 0;

   }

}

CNode:: ~CNode()
{
   RemoveTable();
   m_ChildrenAry.RemoveAll();
}

void CNode::SortChildren()
{
   int k;
   double Cost[10];


   for(k =0; k< m_ChildrenAry.GetSize(); k++)
      m_ChildID[k] = m_ChildrenAry.GetSize() - k -1;

   if(g_RuleNo==0)
      return;

   for(k =0; k< m_ChildrenAry.GetSize(); k++)
   {
      if(g_RuleNo==1) //Best cost
	 Cost[k] =  ((CNode*) (m_ChildrenAry.GetAt(k)))->m_TotalCost;
      if(g_RuleNo==2) // FIFO
	 Cost[k] =  ((CNode*) (m_ChildrenAry.GetAt(k)))->m_EarlyStartTime;
      if(g_RuleNo==3) // FIFO2
	 Cost[k] =  ((CNode*) (m_ChildrenAry.GetAt(k)))->m_EarlyCompletionTime;
      if(g_RuleNo==4) // Minimal processing time
	 Cost[k] =  ((CNode*) (m_ChildrenAry.GetAt(k)))->m_EarlyCompletionTime -  ((CNode*) (m_ChildrenAry.GetAt(k)))->m_EarlyStartTime;

      if(g_RuleNo==5) // Global evaluation
	 Cost[k] =  ((CNode*) (m_ChildrenAry.GetAt(k)))->GetUpperbound();




   }

   double MinCost= 0;
   int MinChildID;
   for(int i =0; i< m_ChildrenAry.GetSize(); i++)
   {
      MinCost= 10000;
      for(k =0; k< m_ChildrenAry.GetSize(); k++)
      {
	 if(MinCost > Cost[k])
	 {
	    MinCost = Cost[k];
	    MinChildID = k;
	 }
      }
      m_ChildID[i] = MinChildID;
      //   TRACE("\nNo.%d = %f",m_ChildID[i], Cost[m_ChildID[i]]);
      Cost[MinChildID] = 20000;

   }


   TRACE("");
}















void CNode::Initilize()
{
}


void CNode::VisitTimeTable()
{
   int i,j;
   for(i=0;i< g_TrainSize; i++)
   {
      TRACE("\n TrainNo.  %d  -----------------------\n",i);
      for (j=0; j< g_SectionSize; j++)
      {
	 TRACE("[%2d] %3d ->%3d    >>",i,m_Table[i][j][0],m_Table[i][j][1]);

      }
   }


}

void CNode::RemoveTable()
{
   return;
}

void CNode::InitializeRoot()
{
   m_Level=0;
   int i,j;

   for(i=0;i< g_TrainSize; i++)
   {
      for(j=0;j< g_SectionSize; j++)
      {
	 m_Table[i][j][2] = 0;	// Departure Time
	 m_Table[i][j][3] = 0;	// Fix Flag
      }
      ExtendJob(i,0);
   }


   for(j=0;j< g_TrainSize; j++)
   {
      m_FixingIndex [j]= 0;

   }

   for(i=0;i< g_TrainSize; i++)
   {
      int sta = GetSectionNo(i,0);
      AddActiveTask(i,sta,m_Table[i][sta][0],m_Table[i][sta][1]);
   }

}





bool CNode::CheckIfConflict(int StationNo,int TrainAry[MAX_CONFLICTSIZE], int &TrainSize, int &ConflictTime)
{
   if(m_NodeID == 95)
      TRACE("\n Node ID %d: ",m_NodeID);

   bool bVisitedFlag[MAX_TRAINSIZE];



   int i,j,k;

   // Sort the train according the start time
   int count=0;
   for(i=0; i<g_TrainSize; i++)
   {
      if(m_Table[i][StationNo][3]==0)
      {
	 bVisitedFlag[i] = false;
	 count+=1;
      }
      else
	 bVisitedFlag[i] = true;
   }


   int FirstTrainNo=-1;
   for(j=0;j< count-1; j++)
   {
      int Minimum=9999;

      for(i=0;i< g_TrainSize; i++)
      {
	 if(!bVisitedFlag[i] && m_Table[i][StationNo][1] < Minimum)
	 {
	    Minimum = m_Table[i][StationNo][1];
	    FirstTrainNo = i;
	 }
      }

      ASSERT(Minimum!= 9999);


      TrainSize=0;
      // Based on the generated sequence.

      int CompletionTime0 = m_Table[FirstTrainNo][StationNo][1];
      ConflictTime = m_Table[FirstTrainNo][StationNo][0];

      //Find the earliest release time:
      for(  k =0; k < g_TrainSize; k++)
      {
	 int ReleaseTime = m_Table[k][StationNo][0];
	 int CompletionTime = m_Table[k][StationNo][1];

	 if( !bVisitedFlag[k] && ReleaseTime< CompletionTime0+g_I)
	 {
	    TrainAry[TrainSize++] =k;
	    if(ReleaseTime < ConflictTime )
	       ConflictTime = ReleaseTime;
	 }
      }

      if(TrainSize>=2)
	 return true;

      bVisitedFlag[FirstTrainNo] = true;
   }
   return false;
}





void CNode::Draw(CDC *pDC,bool TextFlag)
{
   int x0,x1,y0,y1;




   if(m_NodeID == g_SelectNodeNo)
      pDC->SelectObject(&g_PenNodeSelect);
   else
      pDC->SelectObject(&g_PenNodeActive);





   if(g_TreeNodeAry.GetSize() < 1000)
   {
      int size = 5;

      if(m_ActiveFlag)
	 size = 7;



      x0 = m_X - size;
      x1 = m_X + size;
      y0 = m_Y - size;
      y1 = m_Y + size;
      CRect drawrect((int)x0,(int)y0,(int)x1,(int)y1);

      if(m_ActiveFlag)
	 pDC->Ellipse (drawrect);
      else
	 pDC->Rectangle (drawrect);


      if(TextFlag)
      {
	 char str[20];
	 wsprintf(str,"%d",m_NodeID);
	 pDC->DrawText(str,drawrect,DT_CENTER  );
      }

   }
   else
   {
      int size = 3;

      x0 = m_X - size;
      x1 = m_X + size;
      y0 = m_Y - size;
      y1 = m_Y + size;
      CRect drawrect((int)x0,(int)y0,(int)x1,(int)y1);

      pDC->Ellipse (drawrect);
   }

}

void CNode::LocateCenter()
{


   int Y = int ((m_TreeLayer+1) * g_LayerHeight);

   int SubLaySize=0;
   int ThisLayNo = 0;

   /*
      for(int i=0; i< g_ComputeAry.GetSize();i++)
      {
      int NodeNo = g_ComputeAry.GetAt(i);
      CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (NodeNo);
      if(pNode->m_TreeLayer == m_TreeLayer)
      {
      if(pNode->m_NodeID == m_NodeID)
      {
      ThisLayNo = SubLaySize;

      }
      SubLaySize ++;
      }
      }

      */		ThisLayNo = m_LevelNodeID;
   SubLaySize = (int)(pow(2.0,m_TreeLayer));

   int X;
   double XUnit = g_LayerWidth / (SubLaySize +4);
   X = (int)((ThisLayNo + 2 ) * XUnit);

   m_X = X;
   m_Y = Y;

}

void CNode::DrawRelation(CDC *pDC)
{

   if(pParent != NULL)
   {
      int ParentX = pParent->m_X;
      int ParentY = pParent->m_Y;

      pDC->SelectObject(&g_PenNodeActive);

      pDC->MoveTo (ParentX,ParentY);
      pDC->LineTo (m_X,m_Y);
   }



}

void CNode::DrawOptRelation(CDC *pDC)
{


   if(m_BetterNodeNo == -1)
      return;

   CNode* pNode = (CNode*)g_TreeNodeAry.ElementAt (m_BetterNodeNo);

   if(pNode != NULL)
   {
      int ParentX = pNode->m_X;
      int ParentY = pNode->m_Y;

      pDC->SelectObject(&g_PenOptRelation);

      pDC->MoveTo (ParentX,ParentY);
      pDC->LineTo (m_X,m_Y);
   }



}
bool CNode::Intersect(CPoint Point)
{
   double MinDistance;
   MinDistance=6;    // The threshold for hitting range

   double dts;
   dts=pow(pow(m_X-Point.x,2.0)+pow(m_Y-Point.y,2.0),0.5);


   if(dts<MinDistance)
      return TRUE;
   else
      return FALSE;


}

void CNode::UpdateTimeTable()
{

   int i,j;
   for (i=0; i< g_SectionSize; i++)
      for(j=0;j< g_TrainSize; j++)
      {
	 g_Table[j][i][0]= m_Table[j][i][0];
	 g_Table[j][i][1]= m_Table[j][i][1];
	 g_Table[j][i][2]= m_Table[j][i][2];
	 g_Table[j][i][3]= m_Table[j][i][3];

      }

   g_FirstConflictTime = m_FirstConflictTime;
   g_ConflictTrainSet[0] = m_ConflictTrainSet[0];
   g_ConflictTrainSet[1] = m_ConflictTrainSet[1];

   g_FirstConflictStationNo = m_ConflictSectionNo;
   g_ConflictFlag = m_bConflict;

   CNode* pBetterNode;
   if(m_BetterNodeNo != -1)
      pBetterNode= (CNode*) g_TreeNodeAry.GetAt(m_BetterNodeNo);
   else
      pBetterNode = this;




   for (i=0; i< g_SectionSize; i++)
      for(j=0;j< g_TrainSize; j++)
      {
	 g_Table2[j][i][0]= pBetterNode->m_Table[j][i][0];
	 g_Table2[j][i][1]= pBetterNode->m_Table[j][i][1];
	 g_Table2[j][i][2]= pBetterNode->m_Table[j][i][2];
	 g_Table2[j][i][3]= pBetterNode->m_Table[j][i][3];

      }

   g_FirstConflictTime2 = pBetterNode->m_FirstConflictTime;
   g_ConflictTrainSet2[0] = pBetterNode->m_ConflictTrainSet[0];
   g_ConflictTrainSet2[1] = pBetterNode->m_ConflictTrainSet[1];

   g_FirstConflictStationNo2 = pBetterNode->m_ConflictSectionNo;
   g_ConflictFlag2 = pBetterNode->m_bConflict;
}








bool CNode::CheckTrainFeasible(int TrainNo, int StationNo,bool CrossingFlag=false)
{
   for(int i=0; i< g_TrainSize; i++)
   {

      if(g_TrainDirection [TrainNo] != g_TrainDirection [i] && i!= TrainNo)
      {
	 //     TRACE("\n %d %d %d %d",m_Table [i][StationNo][0],m_Table [i][StationNo][1],m_Table [TrainNo][StationNo][0],m_Table [TrainNo][StationNo][1]);

	 if(m_Table [i][StationNo][0] <= m_Table [TrainNo][StationNo][1] && m_Table [i][StationNo][1] >= m_Table [TrainNo][StationNo][0] )
	    return false;

	 if(m_Table [i][StationNo][0] >= m_Table [TrainNo][StationNo][1] && m_Table [i][StationNo][1] <= m_Table [TrainNo][StationNo][0] )
	    return false;
      }

   }
   return true;
}

int CNode::GetTotalConflicts()
{
   int Number =0;


   for(int i=0; i<g_TrainSize; i++)
   {
      if( g_TrainDirection[i]== 0)
	 for( int j =0; j <g_SectionSize; j++)
	 { if(!CheckTrainFeasible(i,j))
	       Number ++;
	 }

   }
   g_TotalConflict = Number;
   return 1;
}




void CNode::DisableAllChildren()
{

   for(int i=0; i < m_ChildrenAry.GetSize (); i++)
   {
      CNode* pNode = (CNode*) (m_ChildrenAry.GetAt (i));
      pNode->m_ActiveFlag = false;
      pNode->DisableAllChildren ();

   }

}

int CNode::GetEstDelay()
{

   int Sum =0;
   int Number =0;
   int i,j,k;
   for(i=0; i<g_TrainSize; i++)
   {

      if( g_TrainDirection[i]== 0)
	 for(j =0; j <g_SectionSize; j++)
	 {
	    if(!CheckTrainFeasible(i,j,true))
	    {
	       Number ++;
	       //			TRACE("Train%d conflicts at Section %d",i, j);
	    }
	 }

   }

   // Construct conflict set
   m_ConflictSize =0;


   for(k =0; k <g_SectionSize; k++) //Section
      for(i=0; i<g_TrainSize; i++)  // Train
	 for(j=0; j<g_TrainSize; j++) // Train
	 {
	    if( g_TrainDirection[i]== 0 && g_TrainDirection[j]== 1)
	    {
	       if(m_Table [j][k][1] >= m_Table [i][k][0] && m_Table [i][k][1] >= m_Table [j][k][0] )
	       {
		  m_ConflictTrainA[m_ConflictSize] = i;
		  m_ConflictTrainB[m_ConflictSize] = j;
		  m_ConflictSection[m_ConflictSize] = k;
		  m_ConflictCompleteTime[m_ConflictSize] = __min(m_Table [i][k][1],m_Table [j][k][1]);
		  m_ConflictDelay[m_ConflictSize] = __min(m_Table [j][k][1]-m_Table [i][k][0],m_Table [i][k][1]-m_Table [j][k][0]);

		  TRACE("\n A:%d B:%d S:%d Time:%d D:%d",m_ConflictTrainA[m_ConflictSize],m_ConflictTrainB[m_ConflictSize],
			m_ConflictSection[m_ConflictSize],m_ConflictCompleteTime[m_ConflictSize],m_ConflictDelay[m_ConflictSize]);

		  m_ConflictSize++;

	       }
	    }

	 }





   if(Number == 0)
      Sum =0;
   else
      Sum = g_I * (Number );

   int AddSum = 0;
   AddSum = GetAdditionalDelay();
   Sum+= AddSum;
   TRACE("\nNodeID %d, %d, Add:%d", m_NodeID, Sum,AddSum);
   return Sum;

}

void CNode::ExtendJob(int TrainNo, int ONo)
{
   int sta, time,k;
   for(k=ONo; k<g_SectionSize; k++)
   {

      sta = GetSectionNo(TrainNo,k);

      //	if(m_Table[TrainNo][sta][3] == 1)  // Fix
      //        continue;


      if (k ==0)
      {
	 time = g_StartTime[TrainNo];
	 m_Table[TrainNo][sta][0] =time;
      }
      else
      {
	 if( g_TrainDirection[TrainNo] == 0)
	    time = m_Table[TrainNo][sta-1][1];
	 else
	    time = m_Table[TrainNo][sta+1][1];
      }

      if( time < m_Table[TrainNo][sta][2])
	 time = m_Table[TrainNo][sta][2];

      m_Table[TrainNo][sta][0] = time ;
      m_Table[TrainNo][sta][1] = m_Table[TrainNo][sta][0] + g_SectionRunTime[sta][g_TrainType[TrainNo]];

   }


}









void CNode::GetLevelNodeID()
{
   CNode* pNode  = this;

   m_LevelNodeID = 0;
   int power =0;
   if(pNode->m_NodeID != 0)
   {
      do{


	 m_LevelNodeID += (int)(pow(2.0,power) * pNode->m_DecisionFlag );

	 pNode = pNode->pParent ;

	 if(pNode == NULL)
	    break;
	 power++;
      }
      while(pNode->m_NodeID != 0);
   }
   else
      m_LevelNodeID = 0;

   //		TRACE("\n Level NodeID %d for %d",m_LevelNodeID,m_NodeID);

}

int CNode::GetSectionNo(int TrainNo, int Index)
{
   int s;
   if( g_TrainDirection[TrainNo] == 0)
      s = Index;
   else
      s = g_SectionSize-1 - Index;

   return s;

}

void CNode::Bound()
{
   if(g_RuleNo >0)
      return;

   for(int k =0; k< g_TrainSize; k++)
   {
      double WaitTime = m_Table [k][g_SectionSize-1][1] - m_Table [k][0][0] - g_TravelTime[g_TrainDirection[k]];
      double UpBound = g_TravelTime[g_TrainType[k]]* g_SpeedRatio;
      if(  WaitTime> UpBound)
      {
	 //			TRACE("\n Node %d (TrainNo %d: WaitTime:%5.0f)Bounded by UpBound:%5.0f",m_NodeID,k,WaitTime,UpBound);
	 m_ActiveFlag = false;
	 return;
      }
   }

   if(g_bCutSetFlag)
   {
      int CurrentSize = g_TreeNodeAry.GetSize ();
      for(int i=CurrentSize-1; i>=0 ; i--)
      {
	 g_CompareTwoNode( m_NodeID, i);
	 g_CompareTwoNode( i,m_NodeID);

      }
   }

}

void CNode::AddActiveTask(int TrainNo, int SectionNo, int ReleaseTime, int CompletionTime)
{
   CompletionTime +=g_I;
   int Index=0;
   for(int i=0; i< m_ActiveTaskAry.GetSize();i++)
   {
      CTask* pTask= m_ActiveTaskAry[i];
      if(CompletionTime > pTask->m_CompletionTime )
	 Index ++;
      else
	 break;
   }

   CTask* pTask=new CTask(TrainNo, SectionNo, ReleaseTime, CompletionTime);
   m_ActiveTaskAry.InsertAt (Index,pTask);

}

void CNode::ASGenerate() //Task
{

   if(m_CompleteFlag || !m_ActiveFlag)
      return;

   int size = m_ActiveTaskAry.GetSize();
   if(size== 0)
   {
      m_CompleteFlag = true;
      m_ActiveFlag = false;

      if( m_TotalCost < g_OptTotalCost )
      {
	 TRACE("\nFind new optimal %d @ Node %d",m_TotalCost, m_NodeID);
	 g_FirstFeasible = true;
	 g_OptTotalCost = m_TotalCost;
	 g_OptTotalCostNodeID = m_NodeID;

      }
      return;
   }

   if(m_NodeID == 0)
      GetTotalConflicts();

   // Find the smallest completion time
   CTask* pTask = (CTask*)(m_ActiveTaskAry.GetAt(0));

   m_ConflictTrainSize = 0;

   int CompletionTime0 = pTask->m_CompletionTime ;
   m_ConflictSectionNo= pTask->m_SectionNo ;


   //Construct the conflict set

   int Actsize = m_ActiveTaskAry.GetSize();
   for( int s=0; s<Actsize; s++)
   {
      CTask* pOtherTask= m_ActiveTaskAry[s];
      //		 TRACE("\nTask%d, Train %d %d->%d",s,pOtherTask->m_TrainNo, pOtherTask->m_ReleaseTime, pOtherTask ->m_CompletionTime);

      if( pOtherTask->m_ReleaseTime < CompletionTime0 +g_I && pOtherTask->m_SectionNo == m_ConflictSectionNo)
      {
	 m_ConflictTrainSet[m_ConflictTrainSize++] =pOtherTask->m_TrainNo;
      }

   }

   // Total Cost:
   m_TotalCost = 0;
   for(int i=0;i< g_TrainSize; i++)
   {
      if(g_TrainDirection[i] == 0)
	 m_CompletionTime[i]= m_Table[i][g_SectionSize-1][1] ;
      else
	 m_CompletionTime[i]= m_Table[i][0][1] ;
      m_TotalCost+= m_CompletionTime[i];
   }

   //Setup the children
   if(m_ConflictTrainSize==1)
   {
      Update();
      return;
   }

   int k;
   for(k =0; k< m_ConflictTrainSize; k++)
   {
      CNode * pNode = new CNode(m_TreeLayer+1);
      pNode->m_DecisionFlag=k;
      pNode->pParent = this;
      pNode->GetLevelNodeID();

      if( m_TreeLayer+1 > g_TreeLayer-1)
	 g_TreeLayer = m_TreeLayer+1;


      int i,j;
      for(i=0;i< g_TrainSize; i++)
	 for(j=0;j< g_SectionSize; j++)
	 {
	    pNode->m_Table[i][j][0] = m_Table[i][j][0];
	    pNode->m_Table[i][j][1] = m_Table[i][j][1];
	    pNode->m_Table[i][j][2] = m_Table[i][j][2];
	    pNode->m_Table[i][j][3] = m_Table[i][j][3];
	 }


      int FixTrainNo = m_ConflictTrainSet[k];
      int CompletionTime = m_Table[FixTrainNo][m_ConflictSectionNo][1];

      pNode->m_EarlyCompletionTime = CompletionTime;
      pNode->m_EarlyStartTime = m_Table[FixTrainNo][m_ConflictSectionNo][0];

      pNode->m_Table[FixTrainNo][m_ConflictSectionNo][3] = 1;

	  int s;

      for(s=0; s< m_ConflictTrainSize;s++)
      {
	 if(s!=k)
	 {
	    int TrainNo= m_ConflictTrainSet[s];
	    pNode->m_Table[TrainNo][m_ConflictSectionNo][0]=CompletionTime+g_I;
	    pNode->m_Table[TrainNo][m_ConflictSectionNo][2]=CompletionTime+g_I;

	    if( g_TrainDirection[TrainNo] == 0)
	       pNode->ExtendJob(TrainNo,m_ConflictSectionNo);
	    else
	       pNode->ExtendJob(TrainNo,g_SectionSize-1-m_ConflictSectionNo);
	 }

      }


      // Low bound
      pNode->m_TotalCost = 0;
      for(i=0;i< g_TrainSize; i++)
      {
	 if(g_TrainDirection[i] == 0)
	    pNode->m_CompletionTime[i]= pNode->m_Table[i][g_SectionSize-1][1] ;
	 else
	    pNode->m_CompletionTime[i]= pNode->m_Table[i][0][1] ;
	 pNode->m_TotalCost+= pNode->m_CompletionTime[i];
      }

      int Delay = pNode->GetEstDelay();

      if(m_NodeID==0)
      {
	 m_EstDelay = Delay;
	 //	TRACE("\n EstDelay = %d",Delay);
	 g_EstDelay[g_ComputingNo] = Delay;
      }


      if(!g_LowerBoundFlag)
	 Delay = 0;


      if(pNode->m_TotalCost + Delay>= g_OptTotalCost)
      {
	 TRACE("\n Node %d Bounded by Node%d",m_NodeID,m_BetterNodeNo);
	 pNode->m_ActiveFlag = false;
	 pNode->m_BetterNodeNo = g_OptTotalCostNodeID;
      }


      // Update the ActiveTaskAry
      for( s=0; s<m_ActiveTaskAry.GetSize(); s++)
      {

	 CTask* pTask= m_ActiveTaskAry[s];

	 if(pTask->m_TrainNo != FixTrainNo)
	 {
	    int ReleaseTime = pNode->m_Table[pTask->m_TrainNo ][pTask->m_SectionNo ][0];
	    int CompletionTime = pNode->m_Table[pTask->m_TrainNo ][pTask->m_SectionNo ][1];
	    pNode->AddActiveTask (pTask->m_TrainNo ,pTask->m_SectionNo ,ReleaseTime,CompletionTime);
	 }
      }


      int sta;
      if(g_TrainDirection[FixTrainNo]==0)
	 sta = m_ConflictSectionNo+1;
      else
	 sta = m_ConflictSectionNo-1;

      if(sta>=0 && sta < g_SectionSize)
	 pNode->AddActiveTask(FixTrainNo,sta,m_Table[FixTrainNo][sta][0],m_Table[FixTrainNo][sta][1]);


      m_ChildrenAry.Add (pNode);
      g_ComputeAry.Add(pNode->m_NodeID );
      if(g_RuleNo==0)
      {
	 g_TreeNodeAry.Add (pNode);
      }

      pNode->UpdateCutsetData();
      //      TRACE("\n New ID:%d",pNode->m_Nod


   }

   if(g_RuleNo!=0)
   {
      SortChildren();
      for(k =0; k< m_ChildrenAry.GetSize(); k++)
      {

	 g_TreeNodeAry.Add (m_ChildrenAry.GetAt(m_ChildID[m_ChildrenAry.GetSize()-k-1]));

      }
   }
   m_ActiveFlag = false;

}

// No conflict:
void CNode::Update()
{


   int FixTrainNo = m_ConflictTrainSet[0];
   m_Table[FixTrainNo][m_ConflictSectionNo][3] = 1;


   // Update the ActiveTaskAry
   // 0: this is the smallest task

   CTask* pTask= m_ActiveTaskAry[0];
   delete pTask;
   m_ActiveTaskAry.RemoveAt(0);

   int sta;
   if(g_TrainDirection[FixTrainNo]==0)
      sta = m_ConflictSectionNo+1;
   else
      sta = m_ConflictSectionNo-1;

   if(sta>=0 && sta < g_SectionSize)
      AddActiveTask(FixTrainNo,sta,m_Table[FixTrainNo][sta][0],m_Table[FixTrainNo][sta][1]);

   UpdateCutsetData();
}

bool CNode::CutSetRule(int Node1, int Node2)
{
   int i;

   if(Node1 == Node2)
      return false;

   CNode* pNode1 = (CNode*)g_TreeNodeAry.ElementAt (Node1);
   CNode* pNode2 = (CNode*)g_TreeNodeAry.ElementAt (Node2);


   //	TRACE("\n!!!COMPARE No.%d <> No.%d ",Node1,Node2);

   if(pNode1->m_TreeLayer != pNode2->m_TreeLayer)
      return false;

   int SumFlag=0;



   int CompleteCostSum1=0;
   int CompleteCostSum2=0;


   for(i=0; i<g_TrainSize; i++)
   {
      if(pNode1->m_TrainFinishFlag [i])
      {
	 CompleteCostSum1+= pNode1->m_CompletionTime[i];
	 CompleteCostSum2+= pNode2->m_CompletionTime[i];
      }
      else
      {
	 if(pNode1->m_TrainActiveSectionIndex [i] > pNode2->m_TrainActiveSectionIndex [i])
	    return false;

	 if(pNode1->m_Table[i][pNode2->m_TrainActiveSectionIndex [i]][0] > pNode2->m_Table[i][pNode2->m_TrainActiveSectionIndex [i]][0])
	    return false;

      }
   }


   // First conditon: Known data is better than option, otherwise return;
   if(CompleteCostSum1 <= CompleteCostSum2  ) // conditon <
   {
      pNode2->m_ActiveFlag = false;
      pNode2->m_BetterNodeNo = Node1;
      pNode2->DisableAllChildren();

      return true;
   }


   return false;
}

void CNode::UpdateCutsetData()
{
   for(int i=0; i<g_TrainSize;i++)
      m_TrainFinishFlag[i]=true;

   for(int s=0; s<m_ActiveTaskAry.GetSize(); s++)
   {
      CTask* pTask= m_ActiveTaskAry[s];
      int TrainNo = pTask->m_TrainNo ;
      m_TrainFinishFlag[TrainNo]= false;
      m_TrainActiveRelease[TrainNo] = pTask->m_ReleaseTime ;
      m_TrainActiveSectionIndex[TrainNo] = pTask->m_SectionNo;
   }


   if(g_bCutSetFlag)
   {
      int CurrentSize = g_TreeNodeAry.GetSize ();
      for(int i=CurrentSize-1; i>=0 ; i--)
      {
	 if(!this->m_ActiveFlag)
	    break;
	 CutSetRule(m_NodeID,i);
	 CutSetRule(i,m_NodeID);
      }
   }
}


int CNode::GetUpperbound()
{
   m_GlobleEst =100000;


   if(m_GlobleEstFlag)
      m_GlobleEst = m_TotalCost;
   else
   {
      m_GlobleEstFlag = true;
      m_GlobleEst = ge_CalculateUpperbound(this);

      /*		if( m_GlobleEst < g_OptTotalCost )
	 {
	 g_OptTotalCost = m_GlobleEst;

	 }

	 */
   }


   TRACE2("\nNode%d, Global =%d",m_NodeID, m_GlobleEst);


   return m_GlobleEst;

}



void CNode::SortConflictSet()
{

   int MinCompleteTime =m_ConflictCompleteTime[0];
   int MinID=0;

   int i,j;

   for(i=0; i<m_ConflictSize-1;i++)
   {
      MinCompleteTime =m_ConflictCompleteTime[i];
      for(j=i+1; j<m_ConflictSize;j++)
      {
	 if(m_ConflictCompleteTime[j] < MinCompleteTime)
	 {
	    MinCompleteTime = m_ConflictCompleteTime[j];
	    MinID = j;
	 }
      }
      // Swap i and MinID
      int A=m_ConflictTrainA[i];
      int B=m_ConflictTrainB[i];
      int S=m_ConflictSection[i];
      int C=m_ConflictCompleteTime[i];
      int D=m_ConflictDelay[i];

      m_ConflictTrainA[i] = m_ConflictTrainA[MinID];
      m_ConflictTrainB[i] = m_ConflictTrainB[MinID];
      m_ConflictSection[i] = m_ConflictSection[MinID];
      m_ConflictCompleteTime[i] = m_ConflictCompleteTime[MinID];
      m_ConflictDelay[i] = m_ConflictDelay[MinID];

      m_ConflictTrainA[MinID] =A;
      m_ConflictTrainB[MinID] = B;
      m_ConflictSection[MinID] =S;
      m_ConflictCompleteTime[MinID] =C;
      m_ConflictDelay[MinID] = D;

      TRACE("\n No.%d Conflict: Time:%d D:%d",i, m_ConflictCompleteTime[i],m_ConflictDelay[i]);
   }
}

int CNode::GetAdditionalDelay()
{
   bool TrainModifiedFlag[30];

   SortConflictSet();

   int i;
   int Sum = 0;
   for(i=0; i< g_TrainSize; i++)
      TrainModifiedFlag[i] = false;


   for(i=0;i<m_ConflictSize;i++)
   {
      if(!TrainModifiedFlag[m_ConflictTrainA[i]] && !TrainModifiedFlag[m_ConflictTrainB[i]] ) // Have not been modified
      {
	 Sum += m_ConflictDelay[i];
	 TrainModifiedFlag[m_ConflictTrainA[i]] = true;
	 TrainModifiedFlag[m_ConflictTrainB[i]] = true;
      }
   }
   TRACE("\n Additional Delay is %d", Sum);
   return Sum;
}
