#include "stdafx.h"
#include "rail.h"
#include "Node.h"
#include "define.h"
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include "ge_node.h"

CObArray ge_TreeNodeAry;
int ge_RuleNo=1;


int ge_TreeNodeID=0;
int ge_TreeLayer = 0;


CGENode::CGENode(int Layer)
{
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

   m_NodeID = ge_TreeNodeID;


   ge_TreeNodeID += 1;

   m_TreeLayer	= Layer;

   m_ActiveFlag = true;

   m_CompleteFlag = false;

   m_bSelectFlag = false;
   pParent = NULL;

   m_BetterNodeNo = -1;


}

CGENode:: ~CGENode()
{
   m_ChildrenAry.RemoveAll();
}





void CGENode::SortChildren()
{
   ASSERT(m_ChildrenAry.GetSize()!=0);


   int k;
   double Cost[10];


   for(k =0; k< m_ChildrenAry.GetSize(); k++)
      m_ChildID[k] = m_ChildrenAry.GetSize() - k -1;

   if(ge_RuleNo==0)
      return;

   for(k =0; k< m_ChildrenAry.GetSize(); k++)
   {
      if(ge_RuleNo==1) //Best cost
	 Cost[k] =  ((CNode*) (m_ChildrenAry.GetAt(k)))->m_TotalCost;
      if(ge_RuleNo==2) // FIFO
	 Cost[k] =  ((CNode*) (m_ChildrenAry.GetAt(k)))->m_EarlyStartTime;
      if(ge_RuleNo==3) // FIFO2
	 Cost[k] =  ((CNode*) (m_ChildrenAry.GetAt(k)))->m_EarlyCompletionTime;
      if(ge_RuleNo==4) // Minimal processing time
	 Cost[k] =  ((CNode*) (m_ChildrenAry.GetAt(k)))->m_EarlyCompletionTime -  ((CNode*) (m_ChildrenAry.GetAt(k)))->m_EarlyStartTime;
   }

   double MinCost= 0;
   int MinChildID=0;
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




void CGENode::VisitTimeTable()
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


void CGENode::ExtendJob(int TrainNo, int ONo)
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

void CGENode::GetLevelNodeID()
{
   CGENode* pNode  = this;

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

int CGENode::GetSectionNo(int TrainNo, int Index)
{
   int s;
   if( g_TrainDirection[TrainNo] == 0)
      s = Index;
   else
      s = g_SectionSize-1 - Index;

   return s;

}



void CGENode::AddActiveTask(int TrainNo, int SectionNo, int ReleaseTime, int CompletionTime)
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

void CGENode::ASGenerate() //Task
{

   if(m_CompleteFlag || !m_ActiveFlag)
      return;

   int size = m_ActiveTaskAry.GetSize();
   if(size== 0)
   {
      m_CompleteFlag = true;


      m_ActiveFlag = false;
      return;
   }


   // Find the smallest completion time
   CTask* pTask = (CTask*)(m_ActiveTaskAry.GetAt(0));

   m_ConflictTrainSize = 0;

   int CompletionTime0 = pTask->m_CompletionTime ;
   m_ConflictSectionNo= pTask->m_SectionNo ;


   //Construct the conflict set

   int s;
   int Actsize = m_ActiveTaskAry.GetSize();
   for(s=0; s<Actsize; s++)
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
      CGENode * pNode = new CGENode(m_TreeLayer+1);
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


      // completion time
      pNode->m_TotalCost = 0;
      for(i=0;i< g_TrainSize; i++)
      {
	 if(g_TrainDirection[i] == 0)
	    pNode->m_CompletionTime[i]= pNode->m_Table[i][g_SectionSize-1][1] ;
	 else
	    pNode->m_CompletionTime[i]= pNode->m_Table[i][0][1] ;
	 pNode->m_TotalCost+= pNode->m_CompletionTime[i];
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


   }

   SortChildren();
   for(k =0; k< m_ChildrenAry.GetSize(); k++)
   {
      ge_TreeNodeAry.Add (m_ChildrenAry.GetAt(m_ChildID[m_ChildrenAry.GetSize()-k-1]));
      //			TRACE("\nAdd No. %d nodes",ge_TreeNodeAry.GetSize());
   }

   m_ActiveFlag = false;

}

// No conflict:
void CGENode::Update()
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


}


CGENode* ge_GetNextNode()
{

   CGENode* pNode = NULL;
   int CurrentSize = ge_TreeNodeAry.GetSize ();

   for(int i=CurrentSize-1; i>=0 ; i--)
   {
      pNode = (CGENode*)ge_TreeNodeAry.ElementAt (i);

      ASSERT(pNode->m_ActiveFlag);
      return pNode;


   }

   return pNode;

}

int ge_LookaHead(int &Size)
{
   int MinCost = 99999;
   Size =0;
   for( int i=0; i<ge_TreeNodeAry.GetSize(); i++)
   {
      CGENode* pNode = (CGENode*)ge_TreeNodeAry.ElementAt (i);
      if(pNode->IsConflictTrainEnd() && pNode->m_TotalCost < MinCost)
      {
	 MinCost = pNode->m_TotalCost;
	 Size ++;

      }

   }

   return MinCost;

}

int ge_CalculateUpperbound(CNode* pIniRoot)
{

   int i,j;
   for(i=ge_TreeNodeAry.GetSize () -1; i >= 0; i--)
   {
      CGENode* pGENode = (CGENode*)ge_TreeNodeAry.ElementAt (i);
      delete pGENode;
   }

   ge_TreeNodeAry.RemoveAll( );


   CGENode* pGENode = new CGENode(0);


   for(i=0;i< g_TrainSize; i++)
      for(j=0;j< g_SectionSize; j++)
      {
	 pGENode->m_Table[i][j][0] = pIniRoot->m_Table[i][j][0];
	 pGENode->m_Table[i][j][1] = pIniRoot->m_Table[i][j][1];
	 pGENode->m_Table[i][j][2] = pIniRoot->m_Table[i][j][2];
	 pGENode->m_Table[i][j][3] = pIniRoot->m_Table[i][j][3];
      }

   /*	  pGENode->m_OriginalConflictTrainSize = pIniRoot->pParent ->m_ConflictTrainSize;

      for( i=0; i<pIniRoot->pParent->m_ConflictTrainSize; i++)
      pGENode->m_OriginalConflictTrainSet[i] = pIniRoot->pParent ->m_ConflictTrainSet[i];
      */

   int Size = pIniRoot->m_ActiveTaskAry.GetSize();
   for(int k=0; k<Size;k++)
   {
      CTask* pTask0 = (CTask*)pIniRoot->m_ActiveTaskAry.GetAt(k);
      CTask* pTask= new CTask(pTask0->m_TrainNo , pTask0->m_SectionNo , pTask0->m_ReleaseTime ,  pTask0->m_CompletionTime);
      pGENode->m_ActiveTaskAry.Add(pTask);
   }

   ge_TreeNodeAry.Add(pGENode);

   CGENode* pNode =NULL;
   do {

      pNode = (CGENode*)ge_TreeNodeAry.ElementAt (ge_TreeNodeAry.GetSize()-1);
      if(pNode->m_CompleteFlag)
      {

	 /*		int Size =0;
	    int Cost = ge_LookaHead(Size);
	    int TotalSize = ge_TreeNodeAry.GetSize();
	    g_SubOptimalSize += Size;
	    */
	 return pNode->m_TotalCost;

      }


      ASSERT(pNode->m_ActiveFlag);


      pNode->ASGenerate ();

   }while (pNode!=NULL);

   return 100000;

}

int CGENode::GetCurrentTime()
{

   if(m_ActiveTaskAry.GetSize()==0)
      return 99999;

   CTask* pTask= m_ActiveTaskAry[0];
   return pTask->m_ReleaseTime;

}

bool CGENode::IsConflictTrainEnd()
{

   int Actsize = m_ActiveTaskAry.GetSize();

   for( int s=0; s<Actsize; s++)
   {
      CTask* pTask= m_ActiveTaskAry[s];
      for(int j=0; j<m_OriginalConflictTrainSize; j++)
      {
	 if( pTask->m_TrainNo == m_OriginalConflictTrainSet[j])	//Original conflict is still in schedulable set.
	    return false;
      }
   }

   return true;
}
