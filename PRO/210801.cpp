#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
#endif

#include<stdio.h>

#define MAX_N    100
#define MAX_TOOL 50

#define ENDCASE 0
#define ADDLOT  1
#define SIMUL   2

//////////////////////////////////////////////////////////////////////////////////////
#define MAX_N    100
#define MAX_TOOL 50
#define ui unsigned int
#define si short int
#define uc unsigned char
#define HEAP_SIZE 15000000
int size = 0;

typedef struct _Heap{
	int EndTime;
	int StepNO;
	int ToolNo;
	_Heap(){
		EndTime = 0;
		StepNO = 0;
	}

	_Heap(int a, int b, int c){
		EndTime = a;
		StepNO = b;
		ToolNo = c;
	}

}HEAP;
HEAP h[HEAP_SIZE];

void swap(HEAP*a, HEAP*b){
	HEAP tmp = *a;
	*a = *b;
	*b = tmp;
}

int comp(HEAP A, HEAP B){
	if (A.EndTime < B.EndTime)
		return 1;
	if ((A.EndTime == B.EndTime) && (A.StepNO > B.StepNO))
		return 1;
	return 0;
}

void Upheapify(int index){
	if (index == 1) return;

	if (comp(h[index], h[index / 2])){
		swap(&h[index], &h[index / 2]);
		Upheapify(index / 2);
	}
}

void Push(int EndTime, int StepNO, int toolNO){

	HEAP tmp(EndTime, StepNO, toolNO);

	h[++size] = tmp;
	Upheapify(size);
}

int top_EndTime(){
	return h[1].EndTime;
}

void Downheapify(int index){

	//when Leaf
	if (index * 2 > size)
		return;

	int tmp = 0;
	//when shape of complete Heap(existing left&right)
	if (index * 2 + 1 <= size){
		if (comp(h[index * 2], h[index * 2 + 1])){
			tmp = index * 2;
		}
		else
			tmp = index * 2 + 1;
	}
	else{
		tmp = index * 2;
	}

	if (comp(h[tmp], h[index])){
		swap(&h[tmp], &h[index]);
		Downheapify(tmp);
	}

}

HEAP pop(){
	HEAP tmp = h[1];

	h[1] = h[size];
	h[size].EndTime = 0;
	h[size--].StepNO = 0;

	Downheapify(1);

	return tmp;
}

struct TOOL{
	si pt[MAX_TOOL];
	uc available[MAX_TOOL];
	si tool_cnt;

	TOOL(){
		tool_cnt = 0;
		for (int i = 0; i < MAX_TOOL; i++){
			pt[i] = 0;
			available[i] = 0;
		}
	}
};

struct PLANT{
	ui wait;
	ui Running;
	ui Done;
	PLANT(){
		wait = 0;
		Running = 0;
		Done = 0;
	}
};

TOOL tool[MAX_N];
PLANT plant[MAX_N];

ui warehouse;
uc step;

void init(int N) {
	step = N;
	warehouse = 0;

	//Heap index variable
	size = 0;
	
	for (register int i = 0; i < step; i++){
		for (int j = 0; j < tool[i].tool_cnt; j++){
			tool[i].pt[j] = 0;
			tool[i].available[j] = 0;
		}
		tool[i].tool_cnt = 0;
		plant[i].wait = 0;
		plant[i].Running = 0;
		plant[i].Done = 0;
	}
}
void quickSort_Tool(int first, int last, int step)
{
	int pivot;
	int i;
	int j;
	int temp;

	if (first < last)
	{
		pivot = first;
		i = first;
		j = last;

		while (i < j)
		{
			while (tool[step].pt[i] <= tool[step].pt[pivot] && i < last)
			{
				i++;
			}
			while (tool[step].pt[j] > tool[step].pt[pivot])
			{
				j--;
			}
			if (i < j)
			{
				temp = tool[step].pt[i];
				tool[step].pt[i] = tool[step].pt[j];
				tool[step].pt[j] = temp;
			}
		}

		temp = tool[step].pt[pivot];
		tool[step].pt[pivot] = tool[step].pt[j];
		tool[step].pt[j] = temp;

		quickSort_Tool(first, j - 1, step);
		quickSort_Tool(j + 1, last, step);
	}
}

void setupTool(int T, int stepNo[5000], int procTime[5000]) {	
	//Tool init
	for (int i = 0; i < T; i++){
		tool[stepNo[i]].available[  tool[stepNo[i]].tool_cnt  ] = 1;
		tool[stepNo[i]].pt[tool[stepNo[i]].tool_cnt++] = procTime[i];
	}

	//Tool incremental sort
	for (int i = 0; i < step; i++){
		quickSort_Tool(0, (tool[i].tool_cnt-1), i);
	}

}

int simulate(int time, int wip[MAX_N]) {

	if(time == 0) return 0;

	//Process Event
	while (top_EndTime() <= time){
		HEAP cur = pop();

		//Last Step
		if (cur.StepNO == step - 1){
			plant[cur.StepNO].Running--;
			warehouse++;
			tool[cur.StepNO].available[cur.ToolNo] = 1;
		}
		//Middle step process
		else{

			int wait_flag = 1;
			for (int i = 0; i<tool[cur.StepNO + 1].tool_cnt; i++){
				//not avaliable
				if (!tool[cur.StepNO + 1].available[i])
					continue;

				//avaliable
				plant[cur.StepNO].Running--;
				tool[cur.StepNO].available[cur.ToolNo] = 1;
				plant[cur.StepNO+1].Running++;
				tool[cur.StepNO+1].available[i] = 0;
				Push(cur.EndTime + tool[cur.StepNO + 1].pt[i], cur.StepNO + 1, i);
				wait_flag = 0;

				if (!plant[cur.StepNO].wait)
					break;

				//if wating item is existing,
				plant[cur.StepNO].wait--;
				plant[cur.StepNO].Running++;
				tool[cur.StepNO].available[cur.ToolNo] = 0;
				Push(cur.EndTime + tool[cur.StepNO].pt[cur.ToolNo], cur.StepNO, cur.ToolNo);
				break;
			}
			//next stage is full
			if (wait_flag){
				plant[cur.StepNO].Running--;
				tool[cur.StepNO].available[cur.ToolNo] = 1;
				plant[cur.StepNO + 1].wait++;

				//if wating item is existing,
				if (plant[cur.StepNO].wait){
					plant[cur.StepNO].wait--;
					plant[cur.StepNO].Running++;
					tool[cur.StepNO].available[cur.ToolNo] = 0;
					Push(cur.EndTime + tool[cur.StepNO].pt[cur.ToolNo], cur.StepNO, cur.ToolNo);
				}
			}

		}
	}

	int res = 0;
	for (int i = 0; i < step; i++){
		if (wip == 0) break;
		*wip++ = plant[i].Running + plant[i].wait;
	}

	return warehouse;
}

void addLot(int time, int number) {

	simulate(time, 0);

	plant[0].wait += number;

	for (int i = 0; i < tool[0].tool_cnt; i++){

		if (!tool[0].available[i])
			continue;

		//avaliable
		plant[0].wait--;
		plant[0].Running++;
		tool[0].available[i] = 0;
		Push(time + tool[0].pt[i], 0, i);

		if (plant[0].wait == 0)
			break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////

static int run() {
	int totalStep = 0, totalTool = 0, stepID[MAX_N * MAX_TOOL], procTime[MAX_N * MAX_TOOL];
	int event, time, number;
	int userOut, userWip[MAX_N], resultOut, resultWip[MAX_N];
	int score = 100;

	for (int j = 0; j < MAX_N; j++) {
		userWip[j] = resultWip[j] = 0;
	}

	scanf("%d %d", &totalStep, &totalTool);
	init(totalStep);

	for (register int i = 0; i < totalTool; i++) {
		scanf("%d %d", stepID + i, procTime + i);
	}
	setupTool(totalTool, stepID, procTime);

	while (1) {
		scanf("%d", &event); 
		if (event == ADDLOT) {
			scanf("%d %d", &time, &number);
 			addLot(time, number);
		}

		else if (event == SIMUL) {
			scanf("%d", &time);
			for (int j = 0; j < totalStep; j++) {
				userWip[j] = 0;
				scanf("%d", &resultWip[j]);
			}
			scanf("%d", &resultOut);

			userOut = simulate(time, userWip);

  			for (int j = 0; j < totalStep; j++) {
				if (userWip[j] != resultWip[j]) {
					score = 0;
					break;
				}
			}
			if (userOut != resultOut) {
				score = 0;
			}
		}
		else {
			return score;
		}
	}
	return score;
}

int main() {

	setbuf(stdout, NULL);
	int TestCase;

	freopen("sample_input_10.txt", "r", stdin); 
	
	scanf("%d", &TestCase);
	for (int t = 1; t <= TestCase; ++t)
		printf("#%d %d\n", t, run());
	return 0;
}
