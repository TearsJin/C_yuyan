#include <conio.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITY 9999 //输入时导致错误,故用9999来代替无穷大
#define NoMidSpot -1
#define MAXVexNum 10
#define MAXGraphNum 10

typedef struct { //权值
    int length;
    int time;
    int fee;
} Weight;

typedef struct {
    char Spots[MAXVexNum][10];
    Weight weightmatrix[MAXVexNum][MAXVexNum];
    Weight Path[MAXVexNum][MAXVexNum];
    Weight D[MAXVexNum][MAXVexNum];
    int vexnum, arcnum;
} MGraph, *MGraphPtr; //图结构

typedef struct {
    int GraphMAX = MAXGraphNum;
    int CurrentGraphNum = 0;
} Maps; //储存地图相关数目信息

void SaveHead(Maps *MapPtr) { //保存已经变化的头文件（地图数目的变化）
    FILE *fp = fopen("MapHead.txt", "rb+");
    fwrite(MapPtr, sizeof(Maps), 1, fp);
    fclose(fp);
    printf("保存成功！\n");
}
void Floyd(MGraphPtr &GPtr) { // Floyd算法
    int i, j, k;
    for (i = 0; i < GPtr->vexnum; i++) {
        for (j = 0; j < GPtr->vexnum; j++) {
            GPtr->D[i][j].length = GPtr->weightmatrix[i][j].length;
            GPtr->Path[i][j].length = -1; //初始化
            GPtr->D[i][j].time = GPtr->weightmatrix[i][j].time;
            GPtr->Path[i][j].time = -1;
            GPtr->D[i][j].fee = GPtr->weightmatrix[i][j].fee;
            GPtr->Path[i][j].fee = -1;
        }
    }
    for (k = 0; k < GPtr->vexnum; k++) {
        for (i = 0; i < GPtr->vexnum; i++) {
            for (j = 0; j < GPtr->vexnum; j++) {
                if (GPtr->D[i][j].length >
                    GPtr->D[i][k].length + GPtr->D[k][j].length) {
                    GPtr->D[i][j].length =
                        GPtr->D[i][k].length + GPtr->D[k][j].length;
                    GPtr->Path[i][j].length = k;
                }
                if (GPtr->D[i][j].time >
                    GPtr->D[i][k].time + GPtr->D[k][j].time) {
                    GPtr->D[i][j].time =
                        GPtr->D[i][k].time + GPtr->D[k][j].time;
                    GPtr->Path[i][j].time = k;
                }
                if (GPtr->D[i][j].fee > GPtr->D[i][k].fee + GPtr->D[k][j].fee) {
                    GPtr->D[i][j].fee = GPtr->D[i][k].fee + GPtr->D[k][j].fee;
                    GPtr->Path[i][j].fee = k;
                }
            }
        }
    }
}
void CreateGraph(MGraphPtr &GPtr, Maps *MapPtr) { //新建地图
    int i, j;
    printf("请输入地图中地点的数目:");
    scanf("%d", &GPtr->vexnum);
    printf("请输入地点名称:\n");
    for (i = 0; i < GPtr->vexnum; i++) {
        printf("请输入地点%d的名称:", i + 1);
        scanf("%s", GPtr->Spots[i]);
        getchar();
    }
    for (i = 0; i < GPtr->vexnum; i++) {
        for (j = 0; j < GPtr->vexnum; j++) {
            printf("请输入%s到%s的路程、时间、费用:", GPtr->Spots[i],
                   GPtr->Spots[j]);
            scanf("%d %d %d", &GPtr->weightmatrix[i][j].length,
                  &GPtr->weightmatrix[i][j].time,
                  &GPtr->weightmatrix[i][j].fee);
            getchar();
            printf("\n");
        }
    }
    Floyd(GPtr);
    FILE *fp = fopen("MapHead.txt", "rb"); //读取头文件信息
    fread(MapPtr, sizeof(Maps), 1, fp);
    fclose(fp);
    if (MapPtr->CurrentGraphNum == 10) {
        printf("已经达到最大地图数，无法新建！\n");
    } else {
        FILE *fpG = fopen("Maps.txt", "ab+"); //追加地图
        fwrite(GPtr, sizeof(MGraph), 1, fpG);
        fclose(fpG);
        MapPtr->CurrentGraphNum = MapPtr->CurrentGraphNum + 1;
        SaveHead(MapPtr);
        printf("\n");
    }
}
int LocateSpots(MGraphPtr &GPtr, char *ch) {
    //根据顶点名称找到相应的下标。
    int i;
    for (i = 0; i < GPtr->vexnum; i++) {
        if (strcmp(ch, GPtr->Spots[i]) == 0)
            break;
    }
    return i;
}
void FindMidSpotLength(int Start, int End, MGraphPtr GPtr) { //中间点
    int MidSpot;
    MidSpot = GPtr->Path[Start][End].length;
    if (MidSpot == -1)
        return;
    FindMidSpotLength(Start, MidSpot, GPtr);
    printf("%s->", GPtr->Spots[MidSpot]);
    FindMidSpotLength(MidSpot, End, GPtr);
}
void DisPlayPathLength(int Start, int End, MGraphPtr GPtr) { //打印路径
    if (GPtr->Path[Start][End].length == -1 &&
        GPtr->weightmatrix[Start][End].length == 9999) {
        printf("没有路径\n");
    } else {
        printf("最短路径:%d\n", GPtr->D[Start][End].length);
        printf("路线:");
        printf("%s->", GPtr->Spots[Start]);
        FindMidSpotLength(Start, End, GPtr);
        printf("%s", GPtr->Spots[End]);
    }
}
void FindMidSpotTime(int Start, int End, MGraphPtr GPtr) { //中间点
    int MidSpot;
    MidSpot = GPtr->Path[Start][End].time;
    if (MidSpot == -1)
        return;
    FindMidSpotTime(Start, MidSpot, GPtr);
    printf("%s->", GPtr->Spots[MidSpot]);
    FindMidSpotTime(MidSpot, End, GPtr);
}
void DisPlayPathTime(int Start, int End, MGraphPtr GPtr) { //打印路径
    if (GPtr->Path[Start][End].time == -1 &&
        GPtr->weightmatrix[Start][End].time == 9999) {
        printf("没有路径\n");
    } else {
        printf("最短路径:%d\n", GPtr->D[Start][End].time);
        printf("路线:");
        printf("%s->", GPtr->Spots[Start]);
        FindMidSpotTime(Start, End, GPtr);
        printf("%s", GPtr->Spots[End]);
    }
}
void FindMidSpotFee(int Start, int End, MGraphPtr GPtr) { //中间点
    int MidSpot;
    MidSpot = GPtr->Path[Start][End].fee;
    if (MidSpot == -1)
        return;
    FindMidSpotFee(Start, MidSpot, GPtr);
    printf("%s->", GPtr->Spots[MidSpot]);
    FindMidSpotFee(MidSpot, End, GPtr);
}
void DisPlayPathFee(int Start, int End, MGraphPtr GPtr) { //打印路径
    if (GPtr->Path[Start][End].fee == -1 &&
        GPtr->weightmatrix[Start][End].fee == 9999) {
        printf("没有路径\n");
    } else {
        printf("最短路径:%d\n", GPtr->D[Start][End].fee);
        printf("路线:");
        printf("%s->", GPtr->Spots[Start]);
        FindMidSpotFee(Start, End, GPtr);
        printf("%s", GPtr->Spots[End]);
    }
}
void ReadOrChangeMap(MGraphPtr &GPtr, Maps *MapPtr) {
    int n;
    FILE *fp1 = fopen("MapHead.txt", "rb");
    FILE *fp2 = fopen("Maps.txt", "r");
    fread(MapPtr, sizeof(Maps), 1, fp1);
    if (MapPtr->CurrentGraphNum == 0) {
        printf("没有地图，请检查相关文件!\n");
        return;
    }
    printf("请输入要读取的地图的序号（1~%d）:", MapPtr->CurrentGraphNum);
    scanf("%d", &n);
    if (n > MapPtr->CurrentGraphNum) {
        printf("请输入符合范围的序号:");
        scanf("%d", &n);
    }
    fclose(fp1);
    fseek(fp2, (n - 1) * sizeof(MGraph), SEEK_SET); //移动指针
    fread(GPtr, sizeof(MGraph), 1, fp2); //读取想要更换的地图信息
    fclose(fp2);
}
void menu() { //打印菜单
    printf("==================================================================="
           "=============\n");
    printf("\t\t\t\t请输入序号选择功能\t\t\n");
    printf("\t\t\t\tA.读取地图或更换地图\t\t\n");
    printf("\t\t\t\tB.打印最短路径长度和路径\t\t\n");
    printf("\t\t\t\tC.打印最短时间花费和路径\t\t\n");
    printf("\t\t\t\tD.打印最短费用花费和路径\t\t\n");
    printf("\t\t\t\tE.退出系统\t\t\n");
    printf("==================================================================="
           "=============\n");
}
int main() {
    MGraphPtr GPtr;
    MGraph G;
    Maps Map;
    Maps *MapPtr;
    int Start, End;
    char Spot1[10], Spot2[10];
    GPtr = &G;
    MapPtr = &Map;
    MapPtr->GraphMAX = 10;
    while (1) {
        menu();
        char ch = _getch();
        switch (ch) {
        case 'F':
            CreateGraph(GPtr, MapPtr);
            break;
        case 'A':
            ReadOrChangeMap(GPtr, MapPtr);
            break;
        case 'B':
            printf("请输入起点:");
            scanf("%s", Spot1);
            getchar();
            printf("请输入终点:");
            scanf("%s", Spot2);
            getchar();
            Start = LocateSpots(GPtr, Spot1);
            End = LocateSpots(GPtr, Spot2);
            DisPlayPathLength(Start, End, GPtr);
            printf("\n");
            break;
        case 'C':
            printf("请输入起点:");
            scanf("%s", Spot1);
            getchar();
            printf("请输入终点:");
            scanf("%s", Spot2);
            getchar();
            Start = LocateSpots(GPtr, Spot1);
            End = LocateSpots(GPtr, Spot2);
            DisPlayPathTime(Start, End, GPtr);
            printf("\n");
            break;
        case 'D':
            printf("请输入起点:");
            scanf("%s", Spot1);
            getchar();
            printf("请输入终点:");
            scanf("%s", Spot2);
            getchar();
            Start = LocateSpots(GPtr, Spot1);
            End = LocateSpots(GPtr, Spot2);
            DisPlayPathFee(Start, End, GPtr);
            printf("\n");
            break;
        case 'E':
            printf("小组成员相关信息？");
            return 0;
            break;
        default:
            printf("输入有误，没有该功能！\n");
            system("pause");
            break;
        }
    }
}