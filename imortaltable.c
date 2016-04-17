#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STATE_NUM 65536


char visited[STATE_NUM];
int visitedFrom[STATE_NUM];

int queue[STATE_NUM];
int queueHead = 0;
int queueTail = 0;
int toInt(char *hex) {
	char *reader = hex;
	int ans = 0;
	int i;
	for(i = 0; i < 4; i++) {
		if(hex[i] >= 48 && hex[i] <= 57) {
			ans = (ans << 4) + (hex[i] - 48);
		}
		else if(hex[i] >= 97 && hex[i] <= 102){
			ans = (ans << 4) + (hex[i] - 87);
		} else {
			return -1;
		}
	}
	return ans;
}

void enqueueState(int state) {
	queue[queueTail] = state;
	queueTail = (queueTail + 1) % STATE_NUM;
}
int dequeueState() {
	if(queueHead == queueTail)
		return -1;
	int ans = queue[queueHead];
	queueHead = (queueHead + 1) % STATE_NUM;
	return ans;
}
int changeState(int state, char action) {
	int rowFlip = action / 4;
	int columnFlip = action % 4;
	int flip = (0xf) << (rowFlip * 4);
	flip |= ((0x1111) << columnFlip);
	return state ^ flip;
}
char findAction(int stateBegin, int stateEnd) {
	int change = stateBegin ^ stateEnd;
	int i = -1;
	char read;
	char read_2 = -1;
	while(1) {
		i++;
		read = change & 0xf;
		change >>= 4;
		if(read == 0xf) {
			break;
		} else if(read_2 == -1) {
			read_2 = read;
		}
	}
	if(read_2 == -1) {
		read_2 = change & 0xf;
	}
	switch(read_2) {
		case 1:
			read_2 = 0;
			break;
		case 2:
			read_2 = 1;
			break;
		case 4:
			read_2 = 2;
			break;
		case 8:
			read_2 = 3;
			break;
	}
	return i * 4 + read_2;
	
	
}
void printToPath(int c) {
	while(c != visitedFrom[c]) {
		printf("%04X %d : %04X %d\n", c ^ visitedFrom[c], visited[c], visitedFrom[c], findAction(visitedFrom[c], c));
		c = visitedFrom[c];
	}
}
int test_changeState() {
	int i = 0;
	for(; i < 16; i++) {
		printf("%d %04X\n", i, changeState(0, i));
	}
	return 0;
}


int main() {
	test_changeState();
	memset(visited, 0, sizeof(visited));
	visited[0] = 1;
	visited[STATE_NUM - 1] = 1;
	memset(visitedFrom, 0, sizeof(visitedFrom));
	visitedFrom[0] = 0;
	visitedFrom[STATE_NUM - 1] = STATE_NUM - 1;
	enqueueState(0);
	enqueueState(STATE_NUM - 1);
	int state;
	int nextState;
	int i;
	while(0 <= (state = dequeueState())) {
		for(i = 0; i < 16; i++) {
			nextState = changeState(state, i);
			// printf("%d %04X\n", nextState, nextState);
			if(visited[nextState] == 0) {
				visited[nextState]++;
				visitedFrom[nextState] = state;
				enqueueState(nextState);
			}
		}
	}
	int c;
	char stateIn[13];

	while(1) {
		scanf("%s", stateIn);
		c = toInt(stateIn);
		if(c == -1)
			break;
		printToPath(c);
	}

}

