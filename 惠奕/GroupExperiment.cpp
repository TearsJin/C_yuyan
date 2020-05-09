#include <malloc.h>
#include <stdio.h>
#include <string.h>

#define INFINITY 9999 //输入时导致错误
#define NoMidSpot -1
#define SpotNum 5
typedef struct {
    int length;
    int time;
    int fee;
} Weight, **WeightPtr;

typedef struct {
    char Spots[5][10] = {"地点一", "地点二", "地点三", "地点四", "地点五"};
    Weight **arcs;
    int vexnum, arcnum;
} MGraph, *MGraphPtr;

void CreateGraph(MGraphPtr &GPtr) {
    int i, j;
    GPtr->arcs = (Weight **)malloc(SpotNum * sizeof(Weight *)); //建立邻接矩阵
    for (i = 0; i < SpotNum; i++) {
        (GPtr->arcs)[i] = (Weight *)malloc(SpotNum * sizeof(Weight));
    }
    for (i = 0; i < SpotNum; i++) {
        for (j = 0; j < SpotNum; j++) {
            printf("请输入%s到%s的路程、时间、费用:", GPtr->Spots[i],
                   GPtr->Spots[j]);
            scanf("%d %d %d", &GPtr->arcs[i][j].length, &GPtr->arcs[i][j].time,
                  &GPtr->arcs[i][j].fee);
            getchar();
            printf("\n");
        }
    }
    GPtr->vexnum = SpotNum;
}
void Floyd(MGraphPtr &GPtr, WeightPtr &DPtr, WeightPtr &PathPtr) {
    int i, j, k;
    DPtr = (WeightPtr)malloc((GPtr->vexnum) * sizeof(Weight *));
    PathPtr = (WeightPtr)malloc((GPtr->vexnum) * sizeof(Weight *));
    for (i = 0; i < GPtr->vexnum; i++) {
        DPtr[i] = (Weight *)malloc((GPtr->vexnum) * sizeof(Weight));
    }
    for (i = 0; i < GPtr->vexnum; i++) {
        PathPtr[i] = (Weight *)malloc((GPtr->vexnum) * sizeof(Weight));
    }
    for (i = 0; i < GPtr->vexnum; i++) {
        for (j = 0; j < GPtr->vexnum; j++) {
            DPtr[i][j].length = GPtr->arcs[i][j].length;
            PathPtr[i][j].length = -1; //初始化
            DPtr[i][j].time = GPtr->arcs[i][j].time;
            PathPtr[i][j].time = -1;
            DPtr[i][j].fee = GPtr->arcs[i][j].fee;
            PathPtr[i][j].fee = -1;
        }
    }
    for (k = 0; k < GPtr->vexnum; k++) {
        for (i = 0; i < GPtr->vexnum; i++) {
            for (j = 0; j < GPtr->vexnum; j++) {
                if (DPtr[i][j].length > DPtr[i][k].length + DPtr[k][j].length) {
                    DPtr[i][j].length = DPtr[i][k].length + DPtr[k][j].length;
                    PathPtr[i][j].length = k;
                }
                if (DPtr[i][j].time > DPtr[i][k].time + DPtr[k][j].time) {
                    DPtr[i][j].time = DPtr[i][k].time + DPtr[k][j].time;
                    PathPtr[i][j].time = k;
                }
                if (DPtr[i][j].fee > DPtr[i][k].fee + DPtr[k][j].fee) {
                    DPtr[i][j].fee = DPtr[i][k].fee + DPtr[k][j].fee;
                    PathPtr[i][j].fee = k;
                }
            }
        }
    }
}
void FindMidSpotLength(WeightPtr PathPtr, int Start, int End) { //中间点
    int MidSpot;
    MidSpot = PathPtr[Start][End].length;
    if (MidSpot == -1)
        return;
    FindMidSpotLength(PathPtr, Start, MidSpot);
    printf("%d", MidSpot);
    FindMidSpotLength(PathPtr, MidSpot, End);
}
void DisPlayPathLength(WeightPtr PathPtr, int Start, int End) { //打印路径
    if (PathPtr[Start][End].length == -1) {
        printf("没有路径\n");
    } else {
        printf("%d", Start);
        FindMidSpotLength(PathPtr, Start, End);
        printf("%d", End);
    }
}
void FindMidSpotTime(WeightPtr PathPtr, int Start, int End) { //中间点
    int MidSpot;
    MidSpot = PathPtr[Start][End].time;
    if (MidSpot == -1)
        return;
    FindMidSpotTime(PathPtr, Start, MidSpot);
    printf("%d", MidSpot);
    FindMidSpotTime(PathPtr, MidSpot, End);
}
void DisPlayPathTime(WeightPtr PathPtr, int Start, int End) { //打印路径
    if (PathPtr[Start][End].time == -1) {
        printf("没有路径\n");
    } else {
        printf("%d", Start);
        FindMidSpotTime(PathPtr, Start, End);
        printf("%d", End);
    }
}
void FindMidSpotFee(WeightPtr PathPtr, int Start, int End) { //中间点
    int MidSpot;
    MidSpot = PathPtr[Start][End].fee;
    if (MidSpot == -1)
        return;
    FindMidSpotFee(PathPtr, Start, MidSpot);
    printf("%d", MidSpot);
    FindMidSpotFee(PathPtr, MidSpot, End);
}
void DisPlayPathFee(WeightPtr PathPtr, int Start, int End) { //打印路径
    if (PathPtr[Start][End].fee == -1) {
        printf("没有路径\n");
    } else {
        printf("%d", Start);
        FindMidSpotFee(PathPtr, Start, End);
        printf("%d", End);
    }
}
int main() {
    WeightPtr PathPtr, DPtr;
    MGraphPtr GPtr;
    MGraph G;
    int Start = 0, End = 2;
    GPtr = &G;
    CreateGraph(GPtr);
    Floyd(GPtr, DPtr, PathPtr);
    DisPlayPath(PathPtr, Start, End);
}