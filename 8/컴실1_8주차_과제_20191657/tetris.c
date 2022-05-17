#include "tetris.h"

static struct sigaction act, oact;
NODE *head = NULL;
int rnum;

/*typedef struct _Node{
	int level;
	int accumulatedScore;
	char recField[HEIGHT][WIDTH];
	int recField_block[HEIGHT][WIDTH];
	struct _Node **child;
	int childsize;
	int curBlockID;
	int recBlockX, recBlockY, recBlockRotate;
	struct _Node *parent;
} Node;*/

void DrawRecommend(int, int, int, int);
void makeTree(RecNode *);
void recommendBlockDown();

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	recRoot = (RecNode *)malloc(sizeof(RecNode));
	recRoot->score = 0;
	recRoot->lv = -1;
	recRoot->f = field;
	makeTree(recRoot);

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		case MENU_RANK: rank(); break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void makeTree(RecNode *root){
	RecNode **t = root->c;
	for(int i = 0;i < CHILDREN_MAX;i++){
		*(t + i) = (RecNode *)malloc(sizeof(RecNode));
		t[i]->score = 0;
		t[i]->lv = root->lv + 1;
		t[i]->f = (char (*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);
		
		if(t[i]->lv < VISIBLE_BLOCKS - 1) makeTree(t[i]);
	}
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawRecommend(recommendY, recommendX, blockID, recommendR);
	DrawShadow(y, x, blockID, blockRotate);
}

void InitTetris(){
	int i,j;
	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	/*nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;*/

	for(i = 0;i < VISIBLE_BLOCKS;i++)
		nextBlock[i] = rand() % 7;
	recommend(recRoot);

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;
	
	/*Root = (Node *)malloc(sizeof(Node));
	Root->level = 0;
	Root->accumulatedScore = 0;
	for(i = 0;i < HEIGHT;i++){
		for(j = 0;j < WIDTH;j++){
			Root->recField[i][j] = field[i][j];
			Root->recField[i][j] = 0;
		}
	}
	Root->child = (Node **)malloc(sizeof(Node *) * Root->childsize);
	Root->childsize = 40;
	Root->parent = NULL;
	Root->recBlockX = 0;
	Root->recBlockY = 0;
	Root->recBlockRotate = 0;

	recommend(Root);*/

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	for(i = 0;i < 4;i++){
		move(10 + i, WIDTH + 13);
		for(j = 0;j < 4;j++){
			if(block[nextBlock[2]][0][i][j]){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0 && !field[y + i][x + j]){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			if(block[currentBlock][blockRotate][i][j] == 1){
				if(f[blockY + i][blockX + j] == 1) return 0;
				else if((blockY + i) >= HEIGHT) return 0;
				else if((blockX + j) < 0 || (blockX + j) >= WIDTH) return 0;
			}
		}
	}
	return 1;	
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
	int x, y, ro;
	if(command == KEY_UP){
		x = blockX;
		y = blockY;
		ro = (blockRotate + 3) % 4;
	}
	else if(command == KEY_DOWN){
		x = blockX;
		y = blockY - 1;
		ro = blockRotate;
	}
	else if(command == KEY_LEFT){
		x = blockX + 1;
		y = blockY;
		ro = blockRotate;
	}
	else{
		x = blockX - 1;
		y = blockY;
		ro = blockRotate;
	}

	/*for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			if(block[currentBlock][ro][i][j]){
				move(i + y + 1, j + x + 1);
				printw(".");
			}
		}
	}*/
	
	DrawField();
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	move(HEIGHT, WIDTH + 10);
}

void BlockDown(int sig){
	// user code
	if(CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)){
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else{
		if(blockY == -1) gameOver = 1;
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		PrintScore(score);

		/*nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = nextBlock[3];*/
		for(int i = 0;i < VISIBLE_BLOCKS - 1;i++)
			nextBlock[i] = nextBlock[i + 1];
		nextBlock[VISIBLE_BLOCKS - 1] = rand() % 7;
		
		recommend(recRoot);

		blockY = -1;
		blockX = WIDTH / 2 - 2;
		blockRotate = 0;

		DrawNextBlock(nextBlock);
		DrawField();
		DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	}
	//강의자료 p26-27의 플로우차트를 참고한다.
	timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int s = 0;
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			if(block[currentBlock][blockRotate][i][j]){
				if(0 <= blockY + i && blockY + i <= HEIGHT){
					if(0 <= blockX + j && blockX + j <= WIDTH)
						f[blockY + i][blockX + j] = 1;
				if(f[blockY + i + 1][blockX + j] || blockY + i + 1 == HEIGHT) s++;
			
				}
			}
		}
	}

	return s * 10;
	//Block이 추가된 영역의 필드값을 바꾼다.
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	int i, j, de = 0, a = 0;
	for(i = 0;i < HEIGHT;i++){
		for(j = 0;j < WIDTH;j++){
			if(!f[i][j]){
				a = 1;
				break;
			}
		}
		if(j == WIDTH || !a){
			de++;
			for(int k = i - 1;k >= 0;k--){
				for(j = WIDTH - 1;j >= 0;j--)
					f[k + 1][j] = f[k][j];
			}
		}
	}
	return de * de * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	for(;y < HEIGHT && CheckToMove(field, blockID, blockRotate, y + 1, x);y++);
	DrawBlock(y, x, blockID, blockRotate, '/');
}

void createRankList(){
	FILE *finput = fopen("rank.txt", "r");
	char name[NAMELEN];
	int rscore;

	if(!finput){
		printw("File does not EXIST\n");
		return;
	}
	else{
		fscanf(finput, "%d", &rnum);
		for(int i = 0;i < rnum;i++){
			fscanf(finput, "%s", name);
			fscanf(finput, "%d", &rscore);
			
			NODE *newnode = (NODE *)malloc(sizeof(NODE));
			newnode->score = rscore;
			strcpy(newnode->username, name);
			newnode->link = NULL;

			if(!head)
				head = newnode;
			else{
				NODE *curr = head;
				while(!curr->link)
					curr = curr->link;
				curr->link = newnode;
			}
		}
	}
	// user code
}

void rank(){
	// user code
	int x = -1, y = -1, de, success = 0;
	char name[NAMELEN];
	clear();
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	switch(wgetch(stdscr)){
		case '1':
			echo();
			printw("X: ");
			scanw("%d", &x);
			if(x == -1) x = 0;
			printw("Y: ");
			scanw("%d", &y);
			if(y == -1) y = 0;
			noecho();

			printw("\t name \t|  score  \n");
			printw("------------------------------------\n");

			if(y < 0 || x < 0 || y < x || x > rnum || y > rnum || !head)
				printw("\nsearch failure: no rank in the list\n");
			else{
				if(!x) x = 1;
				if(!y) y = rnum;

				NODE *curr = head;
				for(int i = x;i <= y;i++){
					printw("%10s\t|%8d\n", curr->username, curr->score);
					curr = curr->link;
				}
			}
			break;

		case '2':
			printw("input the name: ");
			echo();
			getstr(name);
			noecho();
			
			printw("\t name \t|  score  \n");
			printw("------------------------------------\n");
			NODE *curr;
			for(curr = head;curr != NULL;curr = curr->link){
				if(!strcmp(curr->username, name)){
					printw("%10s\t|%8d\n", curr->username, curr->score);
					success = 1;
				}
			}
			if(!success)
				printw("\nsearch failure: no name in the list\n");
			break;

		case '3':
			printw("input the rank: ");
			echo();
			scanw("%d", &de);
			noecho();

			if(de < 1 || de > rnum)
				printw("\nsearch failure: the rank not in the list\n");
			else{
				rnum--;
				NODE *ptr = head, *pre = head;
				if(de == 1){
					head = ptr->link;
					free(ptr);
				} //delete head
				else{
					for(int i = 1;i < de - 1;i++){
						ptr = ptr->link;
						pre = ptr;
					}
					if(ptr->link){
						//delete = ptr;
						ptr = ptr->link;
						pre->link = ptr->link;
						//free(delete);
					}
					else{
						pre->link = ptr->link;
						free(ptr);
					}
				}
				printw("\nresult: the rank deleted\n");
				writeRankFile();
			}
			break;

		default: break;
	}
	getch();
}

void writeRankFile(){
	// user code
	FILE *finput = fopen("rank.txt", "w");
	if(!finput) printw("File does not exist\n");
	else{
		NODE *curr = head;
		fprintf(finput, "%d\n", rnum);
		
		for(int i = 0;i < rnum;i++){
			fprintf(finput, "%s %d\n", curr->username, curr->score);
			curr = curr->link;
		}
	}
	fclose(finput);
}

void newRank(int score){
	// user code
	char username[NAMELEN];
	clear();
	printw("your name: ");
	echo();
	scanw("%s", username);
	noecho();
	rnum++;

	NODE *newnode = (NODE *)malloc(sizeof(NODE));
	strcpy(newnode->username, username);
	newnode->score = score;
	newnode->link = NULL;
	
	NODE *pre = head, *ptr = head;
	if(!head)
		head = newnode;
	else{
		if(head->score < score){
			newnode->link = head;
			head = newnode;
		}
		else{
			while((ptr->score >= score) && (ptr->link)){
				pre = ptr;
				ptr = ptr->link;
			}
			if(!ptr->link && ptr->score > score)
				ptr->link = newnode;
			else{
				newnode->link = ptr;
				pre->link = newnode;
			}
		}
	}
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
	if(CheckToMove(field, blockID, recommendR, recommendY, recommendX)) DrawBlock(recommendY, recommendX, nextBlock[0], recommendR, 'R');
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int i, j = 0, r, h, w, eval, a;
	int l_bound, r_bound;
	int rec = 0, X, Y, R;
	
	int lv = root->lv + 1;
	RecNode **t = root->c;

	for(r = 0;r < NUM_OF_ROTATE;r++){
		l_bound = 3;
		for(h = 0;h < BLOCK_HEIGHT;h++){
			for(w = 0;w < BLOCK_WIDTH;w++){
				if(block[nextBlock[lv]][r][h][w])
					break;
			}
			if(w < l_bound)
				l_bound = w;
		}
		l_bound = 0 - l_bound;
		r_bound = 0;

		for(h = 0;h < BLOCK_HEIGHT;h++){
			for(w = BLOCK_WIDTH - 1;w > -1;w--){
				if(block[nextBlock[lv]][r][h][w])
					break;
			}
			if(w > r_bound)
				r_bound = w;
		}
		r_bound = WIDTH - 1 - r_bound;

		for(i = l_bound;i < r_bound + 1;i++){
			//if(i == WIDTH) break;
			for(h = 0;h < HEIGHT;h++){
				for(w = 0;w < WIDTH;w++)
					t[j]->f[h][w] = root->f[h][w];
			}
			//t[j]->lv = lv;

			a = 0; 
			if(CheckToMove(t[j]->f, nextBlock[lv], r, a, i)){
				while(CheckToMove(t[j]->f, nextBlock[lv], r, ++a, i));
				a--;
			}
			else continue;

			t[j]->score = root->score;
		   	t[j]->score += AddBlockToField(t[j]->f, nextBlock[lv], r, a, i);
			t[j]->score += DeleteLine(t[j]->f);
			
			if(lv < VISIBLE_BLOCKS - 1)
				eval = recommend(t[j]);
			else eval = t[j]->score;

			if(max < eval){
				rec = 1;
				X = i;
				Y = a;
				R = r;
				max = eval;
			}
			j++;
		}
	}

	if(!lv && rec){
		recommendR = R;
		recommendX = X;
		recommendY = Y;
	}
	// user code

	return max;
}

void recommendedPlay(){
	clear();
	time_t start = clock();
	act.sa_handler = recommendBlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();

	double end;
	int command;
	do{
		move(20, WIDTH + 10);
		printw("Time");
		DrawBox(21, WIDTH + 10, 1, 8);

		move(24, WIDTH + 10);
		printw("Score | Time");
		DrawBox(25, WIDTH + 10, 1, 8);
		move(22, WIDTH + 10);
		end = ((double)(clock() - start) / CLOCKS_PER_SEC);
		printw("%7.3lf", end);

		move(26, WIDTH + 10);
		printw("%7.0lf", (double)score / end);
		if(!timed_out){
			alarm(1);
			timed_out = 1;
		}

		command = wgetch(stdscr);
		switch(command){
			case 'q':
				command = QUIT;
				break;
			case 'Q':
				command = QUIT;
				break;
			default:
				command = NOTHING;
				break;
		}

		if(ProcessCommand(command) == QUIT){
			alarm(0);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			refresh();
			getch();
			return;
		}
	}while(!gameOver);

	alarm(0);
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("Good-bye!!");
	refresh();
	getch();
	
	// user code
}

void recommendBlockDown(int sig){
	if(CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)){
		blockY++;
		//DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else{
		if(blockY == -1) gameOver = 1;
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		PrintScore(score);

		for(int i = 0;i < VISIBLE_BLOCKS - 1;i++)
			nextBlock[i] = nextBlock[i + 1];
		nextBlock[VISIBLE_BLOCKS - 1] = rand() % 7;
		
		recommendedPlay();

		blockY = -1;
		blockX = WIDTH / 2 - 2;
		blockRotate = 0;

		DrawNextBlock(nextBlock);
		DrawField();
		DrawBlock(blockY, recommendX, nextBlock[0], recommendR, ' ');
		//DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	}
	//강의자료 p26-27의 플로우차트를 참고한다.
	timed_out = 0;	
}
