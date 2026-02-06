// ldd: -lncurses
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>


int board[4][4];
int score = 0;

typedef enum{
  LEFT,
  RIGHT,
  UP,
  DOWN,
  BACKSPACE, //fin de partie
} key;


void init_board () { //remplir le plateau de cases vides
	for(int ligne=0; ligne<4;ligne++){
		for(int col=0;col<4;col++){
			board[ligne][col]=0;
		}
	}
}


void display_board () { //afficher le plateau de jeu
  	move(0,0);
	printw("== 2048 =============================\n");
	printw("====================== score: %7i\n", score);
	printw("\n");
	for (int ligne=0; ligne<4; ligne++){
		printw("+--------+--------+--------+--------+\n");
		for (int col = 0; col < 4; col++) {
		printw("|%7d ", board[ligne][col]);
		}
	printw("|\n");
	}
	printw("+--------+--------+--------+--------+\n");
	refresh();//affiche sur le terminal les résultat des appels à printw
}

int count_empty(){ //nb cases libre sur le plateau
	int count = 0;
	for(int ligne=0; ligne<4; ligne++){
		for(int col=0; col<4; col++){
			if(board[ligne][col]==0){
        		count++;
			}
		}
	}
	return count; 
}

void add_two(int empty){ //insère le nombre 2 sur le plateau sur la n-ième case vide entre 0 et empty-1
	int pos = rand() % empty;
  	int compteur = 0;
	for(int ligne=0; ligne<4; ligne++){
		for(int col=0; col<4; col++){
			if(board[ligne][col]==0){
				if(compteur == pos ){
					board[ligne][col]=2;
				}
					compteur++;
			}
		}
	}
}

int game_over(int add){
	int empty= count_empty();
	if(empty==0){
		printw("============= GAME OVER =============\n");
		printw("============ (press a key) ============");
		getch();
		return 1;
    
	}else if(add){
		add_two(empty);
		display_board();
		return 0;
	}else{
	return 0;
  }
}


void INIT_2048 () {
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	srand(time(NULL));
}

void DONE_2048() {
	endwin();
	exit(0);
}

int shift_board() {//décale le plateau à gauche
    int move = 0 ;
    for(int ligne=0 ; ligne <4; ligne++){
      int pos=0;
        for(int col=0 ; col<4 ; col++){
            if (board[ligne][col] != 0) {
              if (col!=pos){
                board[ligne][pos] = board[ligne][col];
                board[ligne][col]=0;
                move=1;
              }
              pos++;
        	}
		}
	}
	return move ;
}

int update_board() {
    bool move=0;
    shift_board();
    for (int ligne=0; ligne<4; ligne++) {
        for (int colonne=0; colonne<3; colonne++) {
            if (board[ligne][colonne] == board[ligne][colonne + 1] && board[ligne][colonne] != 0) {
                board[ligne][colonne] *= 2;
                score += board[ligne][colonne];
                board[ligne][colonne+1] = 0;
                move= 1;
            }
        }
    }
      shift_board();//compresser les cases vidées par une addition.
    return move; //vrai si un entier a bougé ou a été additionné
}


key get_key(){
  while (1)
  {
    switch (getch())
    {
    case KEY_UP:
      return UP;

    case KEY_DOWN:
      return DOWN;

    case KEY_LEFT:
      return LEFT;

    case KEY_RIGHT:
      return RIGHT;

    case KEY_BACKSPACE:
      return BACKSPACE;

    }
  }
}


void swap(int* a, int* b) { // échange deux entiers
	int temp = *a;
	*a = *b;
	*b = temp;
}


void mirror_board(){ //échange le contenu de chaque ligne du plateau
  for (int  ligne=0; ligne<4; ligne++){
    for (int col=0; col<2; col++){
      swap(&board[ligne][col],&board[ligne][4-col-1]);
    }
  }
}


void pivot_board(){ //échange chaque case (i, j) du plateau avec la case (j, i)
  for (int i =0; i<4; i++){
    for (int j =i+1; j<4; j++){
      swap(&board[i][j], &board[j][i]);
    }
  }
}

int play(key dir){
  int move_true = 0;
  switch(dir){
    case LEFT:
      move_true= update_board();
      break;
    case RIGHT:
      mirror_board();
      move_true = update_board();
      mirror_board();
      break;
    case UP:
      pivot_board();
      move_true = update_board();
      pivot_board();
      break;
    case DOWN:
      pivot_board(); 
      mirror_board();
      move_true = update_board();
      mirror_board();
      pivot_board(); 
      break;
  }

  return move_true;
}

int main() {
   int add = 1;
   INIT_2048();
   init_board();
   display_board();
   key touche;
   while (!game_over(add)) {
      touche=get_key();
      if (touche == BACKSPACE){
       break; }
     else{
      int direction=play(touche);
	}
}
    DONE_2048();
}
