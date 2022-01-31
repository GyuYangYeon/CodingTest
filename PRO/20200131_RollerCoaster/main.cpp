#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

static const int CMD_INIT = 100;
static const int CMD_ADDRAIL = 200;
static const int CMD_DELRAIL = 300;

////////////////////////////////////////
#include <vector>
#include <unordered_map>
#define EMPTY 0
#define EXIST 1
//#define debug 0
using namespace std;

enum ABS_DIR{UP=0, RIGHT, DOWN, LEFT };
enum RELATIVE_DIR {REAR=0, FRONT};
int dx[4] = {0,1,0,-1};
int dy[4] = {-1,0,1,0};

struct STATION {
	bool isFront;
	int stationDir;
	int back_dir;
	int x;
	int y;
	
	STATION() {
		this->isFront = 0;
		this->stationDir = 0;
		this->x = 0;
		this->y = 0;
		this->back_dir = 0;
	}

	void update(int Front, int dir, int cx, int cy) {
		this->isFront = Front;
		this->stationDir = dir;
		this->x = cx;
		this->y = cy;
	}
};


std::vector <STATION> front;
std::vector <STATION> rear;
unordered_map <int, STATION> map;

bool isCycle() {
	int front_x = front.back().x;
	int front_y = front.back().y;

	int nxt_x = front_x + dx[front.back().stationDir];
	int nxt_y = front_y + dy[front.back().stationDir];

	//nxt of front station is rear,
	//and if they are face up each other,
	if ((nxt_x == rear.back().x) && (nxt_y == rear.back().y) && (((front.back().stationDir+2)%4) == rear.back().stationDir)) {
		return 1;
	}

	return 0;
}

bool isStationExist(int col, int row) {

	//if station exist in hashtable,
	if (map.count((row << 14) + col)) {
		return 1;
	}
	return 0;
}

bool inNxtStationExist(STATION cur) {
	
	int nxt_x = cur.x + dx[cur.stationDir];
	int nxt_y = cur.y + dy[cur.stationDir];

	//해쉬 테이블에 다음 승강장이 있을 경우,
	if (isStationExist(nxt_y, nxt_x)) {
		return 1;
	}

	return 0;
}

void init(int N)
{
	//init all container(vector, unordered_map)
	front.clear();
	rear.clear();
	map.clear();	

	//Adding front, tail station
	//add front
	STATION first_station;
	first_station.update(FRONT, RIGHT, N, N);
	first_station.back_dir = 3;
	front.push_back(first_station);
	//add rear
	first_station.update(REAR, LEFT, N, N);
	first_station.back_dir = 1;
	rear.push_back(first_station);

	//init hash table with first_station(key = row<<14 + col)
	first_station.update(-1, -1, N, N);
	map.insert(make_pair((N << 14) + N, first_station));
}

void addRail(int mFront, int mDirection)
{
	STATION addstation;
	int nx=-1, ny=-1;
	int direction;
	int back_dir;

	//add station info in vector
	if (mFront == FRONT) {
		back_dir = (front.back().stationDir+2)%4;
		direction = (front.back().stationDir + 3 + mDirection) % 4;
		nx = front.back().x + dx[front.back().stationDir];
		ny = front.back().y + dy[front.back().stationDir];
		addstation.update(FRONT, direction, nx, ny);
		addstation.back_dir = back_dir;

		front.push_back(addstation);
	}
	else if (mFront == REAR) {
		back_dir = (rear.back().stationDir + 2) % 4;
		direction = (rear.back().stationDir + 3 + mDirection) % 4;
		nx = rear.back().x + dx[rear.back().stationDir];
		ny = rear.back().y + dy[rear.back().stationDir];
		addstation.update(REAR, direction, nx, ny);
		addstation.back_dir = back_dir;
	
		rear.push_back(addstation);
	}

	//add in hash_table
	map.insert(make_pair((nx << 14) + ny, addstation));

#ifdef DEBUG
	printf("station add - x:%d y:%d\n", addstation.x, addstation.y);
	printf("station add - frontsize:%d rearsize:%d\n", front.size(), rear.size());
#endif

}

bool isLastStaion(int mRow, int mCol) {
	int res = 0;
	if ((front.back().x == mCol) && (front.back().y==mRow)) {
		res = 1;
	}
	else if ((rear.back().x==mCol) && (rear.back().y==mRow)) {
		res = 1;
	}

	return res;
}

int delRail(int mRow, int mCol)
{
	int cnt = 0;
	//isstationExist 1 -> station exist
	//				 0 -> station is empty
	if (isStationExist(mRow,mCol) == EXIST) {

		if (isCycle()) {
			//in case of cycle & last station
			if (isLastStaion(mRow, mCol)) {
				STATION* curstation = &map[(mCol << 14) + mRow];
				if (curstation->isFront) {
					cnt++;
					map.erase((mCol << 14) + mRow);
					front.pop_back();
				}
				else {
					cnt++;
					map.erase((mCol << 14) + mRow);
					rear.pop_back();
				}
			}
			//in case of cycle & not last station,
			//just delete like
			else {
				STATION* curstation = &map[(mCol << 14) + mRow];
				if (curstation->isFront) {
					while (!((front.back().x == mCol) && (front.back().y == mRow))) {
						front.back().isFront = REAR;
						int tmp = front.back().stationDir;
						front.back().stationDir = front.back().back_dir;
						front.back().back_dir = tmp;
						map[(front.back().x << 14) + front.back().y] = front.back();
						rear.push_back(front.back());
						front.pop_back();
					}
					cnt++;
					front.pop_back();
					map.erase((mCol << 14) + mRow);
				}
				else {
					while (!((rear.back().x == mCol) && (rear.back().y == mRow))) {
						rear.back().isFront = FRONT;
						int tmp = rear.back().stationDir;
						rear.back().stationDir = rear.back().back_dir;
						rear.back().back_dir = tmp;
						map[(rear.back().x << 14) + rear.back().y] = rear.back();
						front.push_back(rear.back());
						rear.pop_back();
					}
					cnt++;
					rear.pop_back();
					map.erase((mCol << 14) + mRow);
				}
			}
		}
		//in case of not cycle
		else
		{
			STATION *curstation = &map[(mCol << 14) + mRow];
			//if is front station
			if (curstation->isFront) {
				//until same, keep pop
				while (!((front.back().x == mCol) && (front.back().y == mRow))) {
					cnt++;
					map.erase((front.back().x << 14) + front.back().y);
					front.pop_back();
				}
				cnt++;
				map.erase((front.back().x << 14) + front.back().y);
				front.pop_back();
			}
			//if it is rear station
			else {
				while (!((rear.back().x == mCol) && (rear.back().y == mRow))) {
					cnt++;
					map.erase((rear.back().x << 14) + rear.back().y);
					rear.pop_back();
				}
				cnt++;
				map.erase((rear.back().x << 14) + rear.back().y);
				rear.pop_back();
			}
		}
	}

#ifdef DEBUG
	printf("station del - cnt:%d, front station:%d, rear station:%d\n", cnt, front.size(), rear.size());
#endif

	return cnt;
}
////////////////////////////////////////
static int run(void)
{
	int ret = 0;
	int query_cnt, cmd;
	int usr, ans;
	int N, mRow, mCol, mFront, mDirection;

	scanf("%d", &query_cnt);

	for (int q = 0; q < query_cnt; q++)
	{
		if (q == 443) {
			printf("it's gonna be crash!\n");
		}
		scanf("%d", &cmd);
		switch (cmd) {
		case CMD_INIT:
			scanf("%d", &N);
			init(N);
			ret = 1;
			break;
		case CMD_ADDRAIL:
			scanf("%d %d", &mFront, &mDirection);
 			addRail(mFront, mDirection);
			break;
		case CMD_DELRAIL:
			scanf("%d %d", &mRow, &mCol);
			usr = delRail(mRow, mCol);
			scanf("%d", &ans);
			if (usr != ans)
				ret = 0;
			break;
		default:
			ret = 0;
			break;
		}
	}

	return ret;
}

int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input_H2123_RollerCoaster.txt", "r", stdin);
	int tc, MARK;

	scanf("%d %d", &tc, &MARK);

	for (int t = 1; t <= tc; t++)
	{
		if (t == 9) {
			printf("TC9 start!\n");
		}
		int score = run() ? MARK : 0;
		printf("#%d %d\n", t, score);
	}

	return 0;
}