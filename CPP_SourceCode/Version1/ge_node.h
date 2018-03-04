#ifndef CGENODE_H
#define CGENODE_H
#define MAX_CONFLICTSIZE 5
#include <afxtempl.h>
#include "node.h"


class CGENode: public CObject{
public:
   bool IsConflictTrainEnd();
   int GetCurrentTime();

   CGENode(int Layer);
   ~CGENode();

   //Globle evaluation
   CObArray m_TreeNodeAry;
   int m_GlobleEst;


   void Update();
   void SortChildren();
   int m_EstDelay;
   int m_ChildID[10];
   int m_EarlyCompletionTime;
   int m_EarlyStartTime;


   int m_Level;
   int m_DecisionFlag;
   void ASGenerate();
   void AddActiveTask(int TrainNo,int SectionNo,int ReleaseTime,int CompletionTime);
   int GetSectionNo(int TrainNo, int Index);
   void GetLevelNodeID();
   int  m_LevelNodeID;
   short m_Table[MAX_TRAINSIZE][MAX_STATIONSIZE][4];	// 0 Arrive, 1, Departure, 2 Dwell 3 DoneFlag
   bool m_TrainFinishFlag[MAX_TRAINSIZE];
   int m_TrainActiveSectionIndex[MAX_TRAINSIZE];
   int m_TrainActiveRelease[MAX_TRAINSIZE];
   int m_OriginalConflictTrainSet[10];
   int m_OriginalConflictTrainSize;


   void ExtendJob(int TrainNo, int ONo);
   // DoneFlag : 0 Undone 1: Done


   // Unique ID
   int m_NodeID;
   CGENode * pParent;
   int m_TreeLayer;
   int m_BetterNodeNo;

   CObArray m_ChildrenAry;
   CTaskAry m_ActiveTaskAry;

   // Compute Flag
   bool m_bComputeFlag;
   bool m_bSetupChildFlag;
   bool m_ActiveFlag;
   bool m_CompleteFlag;
   bool m_bTableExist;
   bool m_bComparable;

   bool m_bConflict;
   int m_FirstConflictTime;
   int m_ConflictTrainSet[MAX_CONFLICTSIZE];
   int m_ConflictTrainSize;
   int m_FixTrainIndex;

   // Cost
   int m_TotalCost;
   double m_AValue;
   int m_FinishCost;
   short m_FixingIndex[MAX_TRAINSIZE];	// 0: Station Sequece No, 1: Cost
   short m_CompletionTime[MAX_TRAINSIZE];
   char m_TrainStatus[MAX_TRAINSIZE];  // 0: Unstart, 1: In progress, 2 Finish
   int m_ConflictSectionNo;

   // Drawing Attributes
   int m_X;
   int m_Y;



   ////////////////// Drawing Function
   bool m_bSelectFlag;

   void VisitTimeTable();


};

extern CObArray ge_TreeNodeAry;
extern int ge_TreeLayer;
extern int ge_RuleNo;
#endif