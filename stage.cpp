#include "stage.h"
#include "mymath.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

#define ID2(A, B) ((A)*WIDTH+(B))
#define ID3(A, B, C) ((A)*HIGHT*WIDTH+ID2(B, C))

bool *road;
int *map;
bool *room;
int WIDTH;
int HIGHT;
const int TNUM = 100;
const int BIGROOM = 5;

void stinit(const int L) {
	srand((unsigned)time(NULL));
	fill(map, map + L, 0);
	fill(room, room + L, false);
	fill(road, road + L * 2, false);
}

void mprint() {
	printf("\n");
	for(int i = 0; i < HIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			printf(" %c", map[ID2(i, j)]? '.': '#');
		}
		printf("\n");
	}
}

bool testroom(const int SY, const int SX, const int TY, const int TX) {
	for(int i = SY - 1; i <= TY + 1; i++)
		for(int j = SX - 1; j <= TX + 1; j++)
			if(room[ID2(i, j)])
				return false;
	if(SY - 2 >= 0)
		for(int j = SX - 1; j <= TX + 1; j++)
			if(room[ID2(SY - 2, j)])
				return false;
	if(TY + 2 < HIGHT)
		for(int j = SX - 1; j <= TX + 1; j++)
			if(room[ID2(SY + 2, j)])
				return false;
	if(SX - 2 >= 0)
		for(int i = SY - 1; i <= TY + 1; i++)
			if(room[ID2(i, SX - 2)])
				return false;
	if(TX + 2 < WIDTH)
		for(int i = SY - 1; i <= TY + 1; i++)
			if(room[ID2(i, TX + 2)])
				return false;
	for(int i = SX + 1; i < TX; i++) 
		if(road[ID3(0, SY - 1, i)] || road[ID3(0, TY + 1, i)])
			return false;
	for(int i = SY + 1; i < TY; i++)
		if(road[ID3(1, i, SX - 1)] || road[ID3(1, i, TX + 1)])
			return false;
	return true;
}

void makeroom(const int SY, const int SX, const int TY, const int TX) {
	for(int i = SY; i <= TY; i++) {
		for(int j = SX; j <= TX; j++) {
			room[ID2(i, j)] = true;
			map[ID2(i, j)] = 1;
		}
	}
}

void tryroom(const int Y, const int X) {
	int by, bx, sy, sx, ty, tx;
	int cnt = 0;
	bool f = true;
	do {
		if(++cnt > TNUM) {
			f = false;
			break;
		}
		by = 4 + rand() % 4;
		bx = 4 + rand() % 4;
		if(!(rand() % BIGROOM)) {
			by *= 2;
			bx *= 2;
		}
		sy = Y - rand() % (by + 1);
		sx = X - rand() % (bx + 1);
		ty = sy + by;
		tx = sx + bx;
	}
	while(!inside_ex(sy, sx, HIGHT, WIDTH) || !inside_ex(ty, tx, HIGHT, WIDTH) || !testroom(sy, sx, ty, tx));
	if(f)
		makeroom(sy, sx, ty, tx);
}

bool testroad(int SY, int SX, int TY, int TX) {
	if(SY == TY) {
		if(SX > TX) {
			swap(SX, TX);
		}
		for(int i = SX + 1; i < TX; i++) {
			if(road[ID3(0, SY + 1, i)] || road[ID3(0, SY - 1, i)]) {
				return false;
			}
		}
	} else if(SX == TX) {
		if(SY > TY) {
			swap(SY, TY);
		}
		for(int i = SY + 1; i < TY; i++) {
			if(road[ID3(1, i, SX + 1)] || road[ID3(1, i, SX - 1)]) {
				return false;
			}
		}
	} else {
		return false;
	}
	return true;
}

void makeroad(int SY, int SX, int TY, int TX) {
	if(SY == TY) {
		if(SX > TX) {
			swap(SX, TX);
		}
		for(int i = SX; i <= TX; i++) {
			road[ID3(0, SY, i)] = true;
			map[ID2(SY, i)] = 1;
		}
	} else if(SX == TX) {
		if(SY > TY) {
			swap(SY, TY);
		}
		for(int i = SY; i <= TY; i++) {
			road[ID3(1, i, SX)] = true;
			map[ID2(i, SX)] = 1;
		}
	}
}

int makestage(int *M, bool *R, const int H, const int W, const int P) {
	map = M;
	room = R;
	road = new bool[H * W * 2];
	WIDTH = W;
	HIGHT = H;
	stinit(H * W);
	int py[P], px[P];
	int SIGMA[P + 1];
	int flag[P];
	int roadnum[P];
	fill(flag, flag + P, 0);
	fill(roadnum, roadnum + P, 0);
	SIGMA[0] = 0;
	for(int i = 0; i < P; i++) {
		SIGMA[i + 1] = SIGMA[i] + i + 1;
	}
	
	py[0] = 1 + rand() % (H - 2);
	px[0] = 1 + rand() % (W - 2);
	for(int i = 1; i < P; i++) {
		int pnum, dir, dist;
		do {
			int snum = rand() % i * (i + 1) / 2;
			for(pnum = 0; SIGMA[pnum + 1] <= snum; pnum++);
			dir = rand() % 4;
			dist = 2 + rand() % 15;
			py[i] = py[pnum] + NY[dir] * dist;
			px[i] = px[pnum] + NX[dir] * dist;
		}
		while((flag[pnum] & (1 << dir)) || roadnum[pnum] >= 3 || !inside_ex(py[i], px[i], H, W) || !testroad(py[pnum], px[pnum], py[i], px[i]));
		for(int y = py[pnum] + NY[dir], x = px[pnum] + NX[dir]; y != py[i] || x != px[i]; y += NY[dir], x += NX[dir])
			if(road[ID3(!(dir & 1), y, x)]) {
				py[i] = y;
				px[i] = x;
				break;
			}
		flag[pnum] |= (1 << dir);
		roadnum[pnum]++;
		makeroad(py[pnum], px[pnum], py[i], px[i]);
		for(int j = 0; j < 4; j++)
			if(road[ID3(j & 1, py[i] + NY[j], px[i] + NX[j])]) {
				flag[i] |= (1 << j);
				roadnum[i]++;
			}
	}
	for(int i = 0; i < P; i++)
		if(roadnum[i] <= 1)
			tryroom(py[i], px[i]);
	for(int i = 0; i < P; i++)
		if(roadnum[i] > 1 && flag[i] % 5 && rand() % 2 == 0)
			tryroom(py[i], px[i]);
	delete[] road;
//	mprint();
	return 0;
}
 
/*
int main() {
	int H = 45, W = 45;
	int P = 20;
	int map[H][W];
	bool room[H][W];
	makemap(map[0], room[0], H, W, P);
}
*/
