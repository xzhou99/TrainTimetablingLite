#ifndef CNODE_H
#define CNODE_H
#define MAX_CONFLICTSIZE 5
#include <afxtempl.h>
class CTask: public CObject{
public:
   CTask(int TrainNo,int SectionNo,int ReleaseTime, int CompletionTime);
   int  m_TrainNo;
   int m_SectionNo;
   int m_ReleaseTime;
   int m_CompletionTime;
};

typedef CTypedPtrArray <CObArray, CTask*> CTaskAry;

class CNode: public CObject{
public:
   int GetAdditionalDelay();
   void SortConflictSet();
   int GetUpperbound();

   //Globle evaluation
   CObArray m_TreeNodeAry;
   int m_GlobleEst;
   bool m_GlobleEstFlag;

   int m_ConflictTrainA[50];
   int m_ConflictTrainB[50];
   int m_ConflictSection[50];
   int m_ConflictCompleteTime[50];
   int m_ConflictDelay[50];
   int m_ConflictSize;



   void SortChildren();
   void UpdateCutsetData();
   bool CutSetRule(int Node1,int Node2);
   void Update();

   int m_EstDelay;
   int m_ChildID[10];
   int m_EarlyCompletionTime;
   int m_EarlyStartTime;


   int m_Level;
   int m_DecisionFlag;
   void ASGenerate();
   void AddActiveTask(int TrainNo,int SectionNo,int ReleaseTime,int CompletionTime);
   void Bound();
   int GetSectionNo(int TrainNo, int Index);
   void GetLevelNodeID();
   int  m_LevelNodeID;
   short m_Table[MAX_TRAINSIZE][MAX_STATIONSIZE][4];	// 0 Arrive, 1, Departure, 2 Dwell 3 DoneFlag
   bool m_TrainFinishFlag[MAX_TRAINSIZE];
   int m_TrainActiveSectionIndex[MAX_TRAINSIZE];
   int m_TrainActiveRelease[MAX_TRAINSIZE];



   void ExtendJob(int TrainNo, int ONo);
   // DoneFlag : 0 Undone 1: Done

   int GetEstDelay();
   void DisableAllChildren();
   int GetTotalConflicts();
   bool CheckTrainFeasible(int TrainNo, int StationNo,bool CrossingFlag);

   // Unique ID
   int m_NodeID;
   CNode * pParent;
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

   bool Intersect(CPoint Point);
   void DrawRelation(CDC* pDC);
   void DrawOptRelation(CDC* pDC);
   void LocateCenter();
   bool CheckIfConflict(int StationNo,int TrainAry[5], int &TrainSize, int &ConflictTime);


   ////////////////// Drawing Function
   void Draw(CDC* pDC,bool TextFlag);
   bool m_bSelectFlag;

   CNode(int Layer);
   ~CNode();

   void InitializeRoot();

   void RemoveTable();

   void VisitTimeTable();
   void Initilize();


   void UpdateTimeTable();
};

extern CObArray g_TreeNodeAry;
extern CObArray g_LiveNodeAry;
extern int g_TreeLayer;
#endif