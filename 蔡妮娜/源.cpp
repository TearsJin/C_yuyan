#include <conio.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITY 9999 //����ʱ���´���,����9999�����������
#define NoMidSpot -1
#define MAXVexNum 10
#define MAXGraphNum 10

typedef struct { //Ȩֵ
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
} MGraph, * MGraphPtr; //ͼ�ṹ

typedef struct {
	int GraphMAX = MAXGraphNum;
	int CurrentGraphNum = 0;
} Maps; //�����ͼ�����Ŀ��Ϣ

void SaveHead(Maps* MapPtr) { //�����Ѿ��仯��ͷ�ļ�����ͼ��Ŀ�ı仯��
	FILE* fp;
	errno_t err1=fopen_s(&fp,"MapHead.txt", "rb+");
	if (fp == NULL) { printf("�ļ���ʧ��"); exit(0); }
	fwrite(MapPtr, sizeof(Maps), 1, fp);
	fclose(fp);
	printf("����ɹ���\n");
}
void Floyd(MGraphPtr& GPtr) { // Floyd�㷨
	int i, j, k;
	for (i = 0; i < GPtr->vexnum; i++) {
		for (j = 0; j < GPtr->vexnum; j++) {
			GPtr->D[i][j].length = GPtr->weightmatrix[i][j].length;
			GPtr->Path[i][j].length = -1; //��ʼ��
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
void CreateGraph(MGraphPtr& GPtr, Maps* MapPtr) { //�½���ͼ
	int i, j;
	printf("�������ͼ�еص����Ŀ:");
	scanf_s("%d", &GPtr->vexnum);
	printf("������ص�����:\n");
	for (i = 0; i < MAXVexNum; i++) {
		GPtr->Spots[i][0] = '\0';
		for (j = 0; j < MAXVexNum; j++) {
			GPtr->weightmatrix[i][j].length = -1;
			GPtr->weightmatrix[i][j].time = -1;
			GPtr->weightmatrix[i][j].fee = -1;
		}
	}
	for (i = 0; i < GPtr->vexnum; i++) {
		printf("������ص�%d������:", i + 1);
		scanf_s("%s", GPtr->Spots[i],10);
		getchar();
	}
	for (i = 0; i < GPtr->vexnum; i++) {
		for (j = 0; j < GPtr->vexnum; j++) {
			if (j == i) {
				GPtr->weightmatrix[i][j].length = 0;
				GPtr->weightmatrix[i][j].time = 0;
				GPtr->weightmatrix[i][j].fee = 0;
				continue;
			};
			printf("������%s��%s��·�̡�ʱ�䡢����:", GPtr->Spots[i],
				GPtr->Spots[j]);
			scanf_s("%d %d %d", &GPtr->weightmatrix[i][j].length,
				&GPtr->weightmatrix[i][j].time,
				&GPtr->weightmatrix[i][j].fee);
			getchar();
			printf("\n");
		}
	}
	Floyd(GPtr);
	FILE* fp;
	errno_t err2 = fopen_s(&fp,"MapHead.txt", "rb");
	if (fp == NULL) { printf("�ļ���ʧ��"); exit(0); }//��ȡͷ�ļ���Ϣ
	fread(MapPtr, sizeof(Maps), 1, fp);
	fclose(fp);
	if (MapPtr->CurrentGraphNum == 10) {
		printf("�Ѿ��ﵽ����ͼ�����޷��½���\n");
	}
	else {
		FILE* fpG;
		errno_t err2= fopen_s(&fpG,"Maps.txt", "ab+"); //׷�ӵ�ͼ
		fwrite(GPtr, sizeof(MGraph), 1, fpG);
		fclose(fpG);
		MapPtr->CurrentGraphNum = MapPtr->CurrentGraphNum + 1;
		SaveHead(MapPtr);
		printf("\n");
	}
}
int LocateSpots(MGraphPtr& GPtr, char* ch) {
	//���ݶ��������ҵ���Ӧ���±ꡣ
	int i;
	for (i = 0; i < GPtr->vexnum; i++) {
		if (strcmp(ch, GPtr->Spots[i]) == 0)
			break;
	}
	return i;
}
void FindMidSpotLength(int Start, int End, MGraphPtr GPtr) { //�м��
	int MidSpot;
	MidSpot = GPtr->Path[Start][End].length;
	if (MidSpot == -1)
		return;
	FindMidSpotLength(Start, MidSpot, GPtr);
	printf("%s->", GPtr->Spots[MidSpot]);
	FindMidSpotLength(MidSpot, End, GPtr);
}
void DisPlayPathLength(int Start, int End, MGraphPtr GPtr) { //��ӡ·��
	if (GPtr->Path[Start][End].length == -1 &&
		GPtr->weightmatrix[Start][End].length == 9999) {
		printf("û��·��\n");
	}
	else {
		printf("���·��:%d\n", GPtr->D[Start][End].length);
		printf("·��:");
		printf("%s->", GPtr->Spots[Start]);
		FindMidSpotLength(Start, End, GPtr);
		printf("%s", GPtr->Spots[End]);
	}
}
void FindMidSpotTime(int Start, int End, MGraphPtr GPtr) { //�м��
	int MidSpot;
	MidSpot = GPtr->Path[Start][End].time;
	if (MidSpot == -1)
		return;
	FindMidSpotTime(Start, MidSpot, GPtr);
	printf("%s->", GPtr->Spots[MidSpot]);
	FindMidSpotTime(MidSpot, End, GPtr);
}
void DisPlayPathTime(int Start, int End, MGraphPtr GPtr) { //��ӡ·��
	if (GPtr->Path[Start][End].time == -1 &&
		GPtr->weightmatrix[Start][End].time == 9999) {
		printf("û��·��\n");
	}
	else {
		printf("���·��:%d\n", GPtr->D[Start][End].time);
		printf("·��:");
		printf("%s->", GPtr->Spots[Start]);
		FindMidSpotTime(Start, End, GPtr);
		printf("%s", GPtr->Spots[End]);
	}
}
void FindMidSpotFee(int Start, int End, MGraphPtr GPtr) { //�м��
	int MidSpot;
	MidSpot = GPtr->Path[Start][End].fee;
	if (MidSpot == -1)
		return;
	FindMidSpotFee(Start, MidSpot, GPtr);
	printf("%s->", GPtr->Spots[MidSpot]);
	FindMidSpotFee(MidSpot, End, GPtr);
}
void DisPlayPathFee(int Start, int End, MGraphPtr GPtr) { //��ӡ·��
	if (GPtr->Path[Start][End].fee == -1 &&
		GPtr->weightmatrix[Start][End].fee == 9999) {
		printf("û��·��\n");
	}
	else {
		printf("���·��:%d\n", GPtr->D[Start][End].fee);
		printf("·��:");
		printf("%s->", GPtr->Spots[Start]);
		FindMidSpotFee(Start, End, GPtr);
		printf("%s", GPtr->Spots[End]);
	}
}
void ReadOrChangeMap(MGraphPtr& GPtr, Maps* MapPtr) {
	int n;
	FILE* fp1;
	errno_t err3= fopen_s(&fp1,"MapHead.txt", "rb");
	if (fp1 == NULL) { printf("�ļ���ʧ��"); exit(0); }
	FILE* fp2;
	errno_t err4= fopen_s(&fp2,"Maps.txt", "r");
	if (fp2 == NULL) { printf("�ļ���ʧ��"); exit(0); }
	fread(MapPtr, sizeof(Maps), 1, fp1);
	if (MapPtr->CurrentGraphNum == 0) {
		printf("û�е�ͼ����������ļ�!\n");
		return;
	}
	printf("������Ҫ��ȡ�ĵ�ͼ����ţ�1~%d��:", MapPtr->CurrentGraphNum);
	scanf_s("%d", &n);
	if (n > MapPtr->CurrentGraphNum) {
		printf("��������Ϸ�Χ�����:");
		scanf_s("%d", &n);
	}
	fclose(fp1);
	fseek(fp2, (n - 1) * sizeof(MGraph), SEEK_SET); //�ƶ�ָ��
	fread(GPtr, sizeof(MGraph), 1, fp2); //��ȡ��Ҫ�����ĵ�ͼ��Ϣ
	fclose(fp2);
}
void menu() { //��ӡ�˵�
	printf("==================================================================="
		"=============\n");
	printf("\t\t\t\t���������ѡ����\t\t\n");
	printf("\t\t\t\tA.��ȡ��ͼ�������ͼ\t\t\n");
	printf("\t\t\t\tB.��ӡ���·�����Ⱥ�·��\t\t\n");
	printf("\t\t\t\tC.��ӡ���ʱ�仨�Ѻ�·��\t\t\n");
	printf("\t\t\t\tD.��ӡ��̷��û��Ѻ�·��\t\t\n");
	printf("\t\t\t\tE.�˳�ϵͳ\t\t\n");
	printf("==================================================================="
		"=============\n");
}
int main() {
	MGraphPtr GPtr;
	MGraph G;
	Maps Map;
	Maps* MapPtr;
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
		case 'f':
			CreateGraph(GPtr, MapPtr);
			break;
		case 'A':
		case 'a':
			ReadOrChangeMap(GPtr, MapPtr);
			break;
		case 'B':
		case 'b':
			printf("���������:");
			scanf_s("%s", Spot1,10);
			getchar();
			printf("�������յ�:");
			scanf_s("%s", Spot2,10);
			getchar();
			Start = LocateSpots(GPtr, Spot1);
			End = LocateSpots(GPtr, Spot2);
			DisPlayPathLength(Start, End, GPtr);
			printf("\n");
			break;
		case 'C':
		case 'c':
			printf("���������:");
			scanf_s("%s", Spot1,10);
			getchar();
			printf("�������յ�:");
			scanf_s("%s", Spot2,10);
			getchar();
			Start = LocateSpots(GPtr, Spot1);
			End = LocateSpots(GPtr, Spot2);
			DisPlayPathTime(Start, End, GPtr);
			printf("\n");
			break;
		case 'D':
		case 'd':
			printf("���������:");
			scanf_s("%s", Spot1,10);
			getchar();
			printf("�������յ�:");
			scanf_s("%s", Spot2,10);
			getchar();
			Start = LocateSpots(GPtr, Spot1);
			End = LocateSpots(GPtr, Spot2);
			DisPlayPathFee(Start, End, GPtr);
			printf("\n");
			break;
		case 'E':
		case 'e':
			printf("\nС���Ա�����Ϣ\n�����     �ο�     �¿�ΰ      ������\n");
			return 0;
			break;
		default:
			printf("��������û�иù��ܣ�\n");
			system("pause");
			break;
		}
		printf("\n");
		system("pause");
		system("cls");
	}
}