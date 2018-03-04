#ifndef DEFINE_H
#define DEFINE_H

#define MAX_STATIONSIZE 10
#define MAX_TRAINSIZE 40
#define MAX_ProblemSize 200

extern bool g_SubOptimalEst;
extern int g_BeamWidth;
extern int g_FilterWidth;
extern int g_MaxTrainCost;
extern int g_SelectNodeNo;
extern int g_SelectTrainNo;
extern	int g_TreeNodeID;
extern int g_TotalConflict;
extern bool g_RemoveTableFlag;
extern int g_OptTotalCost;
extern int g_OptTotalCostNodeID;
extern char g_InfoStr[500];
extern double g_LayerHeight;
extern double g_LayerWidth;
extern int g_SectionSize;
extern int g_StationDistance[MAX_STATIONSIZE];
extern int g_TotalDistance;

extern int g_SectionRunTime[MAX_STATIONSIZE][2];  //Two directions

extern   int g_ConflictFlag;
extern	 int g_FirstConflictTime;
extern	 int g_ConflictTrainSet[2];
extern	 int g_FirstConflictStationNo;

extern   int g_ConflictFlag2;
extern	 int g_FirstConflictTime2;
extern	 int g_ConflictTrainSet2[2];
extern	 int g_FirstConflictStationNo2;

extern int g_Table[MAX_TRAINSIZE][MAX_STATIONSIZE][4];	// 0 Arrive, 1, Departure, 2 Dwell
extern int g_Table2[MAX_TRAINSIZE][MAX_STATIONSIZE][4];	// 0 Arrive, 1, Departure, 2 Dwell
extern int g_TrainDirection[MAX_TRAINSIZE];
extern int g_TrainWeight[MAX_TRAINSIZE];
extern int g_TrainType[MAX_TRAINSIZE];
extern int g_TrainSize;
extern int g_StartTime[MAX_TRAINSIZE];

extern void g_SetupTimeTable(int Seed);


extern bool g_LowerBoundFlag;
extern bool g_bBestSearch;
extern int g_I;



extern int g_MAX_NodeSize;
extern double g_Variance;

extern void g_CompareTwoNode(int Node1,int Node2);

extern void g_RemoveNodeAry();
extern bool g_CompleteTreeDrawFlag;
extern bool g_bCutSetFlag;
extern CDWordArray g_ComputeAry;


extern    bool g_ComputingFlag[MAX_ProblemSize];
extern    int g_ComSuccessFlag[MAX_ProblemSize];
extern    int g_NodeSize[MAX_ProblemSize];
extern    int g_Seed[MAX_ProblemSize];
extern    int g_ComputingTime[MAX_ProblemSize];
extern    int g_ComputingConflict[MAX_ProblemSize];
extern    int g_ComputingSolution[MAX_ProblemSize];
extern    double g_ComputingVariance[MAX_ProblemSize];

extern    double g_GetAverage(int value[MAX_ProblemSize],int Size);
extern    bool g_ComputingFlag[MAX_ProblemSize];

extern	int g_OptValue[MAX_ProblemSize];
extern	int g_DelayValue[MAX_ProblemSize];
extern	int g_MethodSize;
extern  int g_CurComputeMethodNo;
extern bool g_ShowMessageFlag;
extern int g_ComputingTrainSize;
extern int g_ReleaseTimeInterval;
extern int g_ReleaseTimeOrder;
extern int g_ReleaseTimeUp;
extern int g_ReleaseTimeDown;

extern int g_ComputingNo;
extern int g_SearchFlag;
extern bool g_FirstFeasible;
extern int g_ArlRand(double Ave,long *x,int Rank);
extern void g_TestRandomVariable();
extern int g_RuleNo;
extern int g_min(int value1, int value2);
extern double g_TrainTypeRatio;
extern double g_SpeedRatio;

extern int g_GetTrainType(double ratio);
extern int g_TravelTime[2];
extern int g_EstDelay[MAX_ProblemSize];
extern int g_SubOptimalSize;
extern int g_KNodesLimit;

extern double g_Average(int Size, int Array[MAX_ProblemSize]);
#endif