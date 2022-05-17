#include "maze.h"

int main() {
	scanf("%d %d", &n, &m);
	width = 2 * n + 1;
	height = 2 * m + 1;
	Makemaze();
	Printmaze();
	return 0;
}

void Makemaze() {
	srand(time(NULL));
	int i, j;
	maze = (int **)malloc(sizeof(int *) * height);

	for(i = 0;i < height;i++)
		maze[i] = (int *)malloc(sizeof(int) * width);
	for (i = 0;i < height;i++) {
		for (j = 0;j < width;j++) {
			if (i % 2 == 0 && j % 2 == 0)
				maze[i][j] = '+';
			else if (i % 2 == 0)
				maze[i][j] = '-';
			else if (j % 2 == 0)
				maze[i][j] = '|';
			else
				maze[i][j] = ' ';
		}
	}
	
	int roomnum = 1;
	/* step 1. lst line setup */
	for (j = 1;j < width;j += 2) {
		maze[1][j] = roomnum++;
	}
	//Printmaze();

	/* step 2. line setup */
	for (i = 1;i < height - 2;) {
		// current line 벽 없애기 (i = 홀수)
		for (j = 1;j < width - 2;j += 2) {
			if (maze[i][j] != maze[i][j + 2] && rand() % 2) {
				maze[i][j + 2] = maze[i][j];
				maze[i][j + 1] = ' ';
			} // remove wall
			/*else if (maze[i][j] == maze[i][j + 2]) {
				maze[i][j + 1] = ' ';
			}*/
		}

		// 다음 i의 수직 경로 만들기
		int remove_flag = 0;
		char currentnum = maze[i][1];
		for (j = 1;j < width;) {
			//srand(time(NULL));
			if (maze[i][j] == currentnum && rand() % 2) {
				remove_flag = 1;
				maze[i + 1][j] = ' ';
				maze[i + 2][j] = maze[i][j];
				j += 2;
				continue;
			} // remove wall
			else if (maze[i][j] != currentnum) {
				if (!remove_flag) {
					maze[i + 1][j - 2] = ' ';
					maze[i + 2][j - 2] = currentnum;
				} // check
				currentnum = maze[i][j];
				remove_flag = 0;
			} // new roomnum
			else {
				j += 2;
				continue;
			}
		}
		if (!remove_flag) {
			maze[i + 1][j - 2] = ' ';
			maze[i + 2][j - 2] = currentnum;
		}

		i += 2;
		// next line 나머지 방들 구체화
		for (j = 1;j < width;j += 2) {
			if (maze[i][j] == ' ')
				maze[i][j] = roomnum++;
			/*else if (j < width - 2 && maze[i][j] == maze[i][j + 2])
				maze[i][j + 1] = ' ';*/
		}
		
	}

	/* step 3. last line setup */
	for (j = 1;j < width - 2;j += 2) {
		if (maze[height - 2][j] > maze[height - 2][j + 2]) {
			maze[height - 2][j + 1] = ' ';
			maze[height - 2][j] = maze[height - 2][j + 2];
		}
		else if (maze[height - 2][j] < maze[height - 2][j + 2]) {
			maze[height - 2][j + 1] = ' ';
			maze[height - 2][j + 2] = maze[height - 2][j];
		}
	}
}

void Printmaze() {
	FILE *foutput = fopen("maze.maz", "w");
	for (int i = 0;i < height;i++) {
		for (int j = 0;j < width;j++) {
			if (i % 2 && j % 2)
				fprintf(foutput, " ");
			else
				fprintf(foutput, "%c", maze[i][j]);
		}
		fprintf(foutput, "\n");
	}

	/*printf("\n");
	for (int i = 0;i < height;i++) {
		for (int j = 0;j < width;j++) {
			if (i % 2 && j % 2)
				printf(" ", maze[i][j]);
			else
				printf("%c", maze[i][j]);
		}
		printf("\n");
	}*/
}