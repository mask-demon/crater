#include "mymath.h"

bool inside_ex(const int Y, const int X, const int H, const int W) {
	return 0 < Y && Y < H - 1 && 0 < X && X < W - 1;
}

bool inside_in(const int Y, const int X, const int H, const int W) {
	return 0 <= Y && Y < H && 0 <= X && X < W;
}

bool inmin(int &a, const int b) {
	if(b < a) {
		a = b;
		return true;
	}
	return false;
}

bool inmax(int &a, const int b) {
	if(b > a) {
		a = b;
		return true;
	}
	return false;
}
