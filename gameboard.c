//Rubana Syeda
//Tony Kusalik CMPT 214 (03)
//A5Q2

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include "gameboard.h"
#include "gameplay.h"
#include "rand_num.h"

typedef struct _gameboard{
	uint8_t width; //5-26 inclusive; width of the gameboard
	uint8_t height; //5-26 inlusive; height of the gameboard
	uint8_t num_ships; //5-10 inclusive; number of hidden ships
	unsigned int shots_taken; //number of shots taken
	int *game_state;  // a pointer to the first element of a 1D view of a 2D
					 //array with height rows and width columns, initially initialized to 0
					// -1 means player guessed and missed; 0 means it is an empty location with no ship
				   // 1 means the position has a ship but not guessed; 2 means players guessed the ship and hit
}Gameboard;

// A board position .
typedef struct board_position {
	unsigned int row ; 		// Row coordinate ; for a gameboard with
							// size width x height , must not greater
							// than ( board height ) -1.
	unsigned int col ; 		// Column coordiante ; for a gameboard with
							// size width x height , must not be greater
							// than ( board width ) -1.
} BoardPosition ;

/**
* Returns a pointer to a newly creater and initialized Gameboard that include randomly determined
* number of ships and randomly determined ship positions
* @param width width of the gameboard
* @param height height of the gameboard
* @precond width and height must be between 5-26 inclusive on both end
* @return pointer to a newly initialized Gamebaord
*/
Gameboard* create_gameboard(uint8_t width, uint8_t height){
	if (width<5 || width>26 || height<5 || height>26){
		errx(1, "Height and Width must be between 5 and 26 inclusive");
	}
	
	Gameboard* a_game = (Gameboard*) malloc(sizeof(Gameboard));
	if( a_game == NULL){
		errx(1,"Error allocating memory for Gameboard");
	}
	a_game -> width = width;
	a_game -> height = height;
	
	uint8_t rand_num_ships;
	while((rand_num_ships = (uint8_t) rand_num(10)) < 5){
		rand_init();
	}
	a_game-> num_ships = rand_num_ships;
	
	a_game -> current_shots_taken = 0;	
	
	// randomly determining the location on the ships
		// creating 1D view of a 2D array all initiallized to 0

	a_game -> game_state = (int*) calloc(width*height, sizeof(int));
	if(a_game -> game_state == NULL){
		errx(1,"Error allocating memory for game_state in Gamebaord");
	}

	for( int i=0; i<rand_num_ships; i++){
		rand_init();
		int row;
		int col;
		do{
			row = rand_num(height-1);
			col = rand_num(width-1);
		}
		while(a_game-> game_state[row*(width-1)+col] != 0);
		printf("%d  %d\n",row, col);
		a_game-> game_state[row*(width-1)+col] = 1;
	}
	
	return a_game;
}

/**
* The function prints to stdout a grid that represents the state of the game given by gboard
* '.' means a position the player has not shot yet. 'o' means a shot and miss. 'X' menas a hit
* @param gboard a pointer to a Gameboard instance
*/
void print_game_state(Gameboard* ptrgboard){
	Gameboard theboard = *ptrgboard;
	int num_row = theboard.height -1;
	int num_col = theboard.width -1;
	
	// printing the col header
	printf("  ");  //the left corner space
	for(int i=1; i<num_col; i++){
		printf("%d ",i);
	}
	printf("\n");
	
	// printing the board positions
	for(int i=0; i<num_row;i++){
		printf("%c ",65+i);
		for(int j=0; j<num_col; j++){
			int cur_pos = theboard.game_state[i*num_col+j];
			switch(cur_pos){
				case -1:
					printf("o ");
					break;
				case 0,1:
					printf(". ");
					break;
				case 2:
					printf("X ");
					break;
				default:
					break;					
			}
			
		}
		printf("\n");
	}
}	

/**
* If the BoardPosition is valid, the fuction increments shots_taken in the given Gamebaord
* and returns value of 3 macros RESULT_HIT,  RESULT_MISS, RESULT_REPEAT
* @param ptrgboard a pointer to a Gameboard instance
* @param ptrbpositions a pointer to a BoardPosition instance
* @return RESULT_HIT value if the given boardposition contains a ship and was not shot at before
*   	  RESULT_MISS if the given board position does not contain a ship and was not shot at before
*		  RESULT_REPEAT if the given board position was previously shot at
*/
int check_for_hit(Gamebaord* ptrgboard, BoardPosition* ptrbpositions){
	BoardPosition bPositions = *ptrbpositions;
	Gameboard board = *ptrbgboard;
	
	// check for valid position
	if(bPositions.row<0 || bPositions.row>board.height-1 || bPositions.col<0 || bPositions.col>board.width-1){
		errx(1, "The board position is not valid");
	}
	board.shots_taken += 1;
	//linear index for Gamrboard using the board position
	int linIndex = bPositions.row*(board.width-1) + bPositions.col;
	if(board.game_state[linIndex] == 1){
		return 1; //value for RESULT_HIT indicates gameplay module that it was a hit
	}
		
	else if(board.game_state[linIndex] == 0){

		return 0; //value for RESULT_MISS 
	}
	else{
		return -1; //value for RESULT_REPEAT		
	}
}

/**
* If the board position is valid, this function must record in the game state that
* a hit has occurred at the given game board position
* @param ptrgboard a pointer to a Gameboard instance
* @param ptrbpositions a pointer to a BoardPosition instance
*/
void record_hit(Gamebaord* ptrgboard, BoardPosition* ptrbpositions){
	BoardPosition bPositions = *ptrbpositions;
	Gameboard board = *ptrgboard;
	
	int linIndex = bPositions.row*(board.width-1) + bPositions.col;
	board.game_state[linIndex] = 2;
}

/**
* If the board position is valid, this function must record in the game state that
* a miss has occurred at the given game board position
* @param ptrgboard a pointer to a Gameboard instance
* @param ptrbpositions a pointer to a BoardPosition instance
*/
void record_miss(Gamebaord* ptrgboard, BoardPosition* ptrbpositions){
	BoardPosition bPositions = *ptrbpositions;
	Gameboard board = *ptrgboard;
	
	int linIndex = bPositions.row*(board.width-1) + bPositions.col;
	board.game_state[linIndex] = -1;
}

/**
* If all ships have been sunk by the player, this function should return non-zero;
* otherwise, it returns zero
* @param ptrgboard a pointer to a Gameboard instance
* @return 1 all ships have been sunk, 0 if there are still ships not sunk
*/
int is_victory(Gameboard* ptrgboard){
	Gameboard gboard = *ptrbgboard;
	int num_of_positions = (gboard.width-1) * (gboard.height-1);
	for (int i=0; i< num_of_positions;i++){
		if(gboard.game_state[i] == 1){
			return 0; // false: means there is ship left that have not been sunk
		}
	}
	// if user wins, it will free all the dynamically allocated memory before returing the win
	free(gboard.game_state);
	free(ptrbgboard);
	return 1;	
}

int main(){
	Gameboard* board = create_gameboard(5,5);
	printf("%u\n",board ->num_ships);
	
	free(board -> game_state);
	free(board);
	return 0;
}