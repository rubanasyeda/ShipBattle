//Rubana Syeda
//Tony Kusalik CMPT 214 (03)
//A5Q2

//Macros
#define RESULT_HIT 1 
#define RESULT_MISS 0
#define RESULT_REPEAT -1


// A board position .
typedef struct board_position {
	unsigned int row ; 		// Row coordinate ; for a gameboard with
							// size width x height , must not greater
							// than ( board height ) -1.
	unsigned int col ; 		// Column coordiante ; for a gameboard with
							// size width x height , must not be greater
							// than ( board width ) -1.
} BoardPosition ;

typedef struct _gameboard{
	uint8_t width; //5-26 inclusive; width of the gameboard
	uint8_t height; //5-26 inlusive; height of the gameboard
	uint8_t num_ships; //5-10 inclusive; number of hidden ships
	unsigned int current_shots_taken = 0; number of shots taken
	int *game_state; // a pointer to the first element of a 1D view of a 2D
					//array with height rows and width columns, initially initialized to 0
					// -1 means player guessed and missed; 0 means it is an empty location
					// 1 means the position has a ship but not guessed; 2 means players guessed the ship and hit
}Gameboard;

/**
* Returns a pointer to a newly creater and initialized Gameboard that include randomly determined
* number of ships and randomly determined ship positions
* @param width width of the gameboard
* @param height height of the gameboard
* @precondition width and height must be between 5-26 inclusive on both end
* @return pointer to a newly initialized Gamebaord
*/
Gameboard* create_gameboard(uint8_t width, uint8_t height);

/**
* The function prints to stdout a grid that represents the state of the game given by gboard
* '.' means a position the player has not shot yet. 'o' means a shot and miss. 'X' menas a hit
* @param gboard a pointer to a Gameboard instance
*/
void print_game_state(Gameboard* ptrgboard);

/**
* If the BoardPosition is valid, the fuction increments shots_taken in the given Gamebaord
* and returns value of 3 macros RESULT_HIT,  RESULT_MISS, RESULT_REPEAT
* @param ptrgboard a pointer to a Gameboard instance
* @param ptrbpositions a pointer to a BoardPosition instance
* @return RESULT_HIT value if the given boardposition contains a ship and was not shot at before
*   	  RESULT_MISS if the given board position does not contain a ship and was not shot at before
*		  RESULT_REPEAT if the given board position was previously shot at
*/
int check_for_hit(Gamebaord* ptrgboard, BoardPosition* ptrbpositions);

/**
* If the board position is valid, this function must record in the game state that
* a hit has occurred at the given game board position
* @param ptrgboard a pointer to a Gameboard instance
* @param ptrbpositions a pointer to a BoardPosition instance
*/
void record_hit(Gamebaord* ptrgboard, BoardPosition* ptrbpositions);

/**
* If the board position is valid, this function must record in the game state that
* a miss has occurred at the given game board position
* @param ptrgboard a pointer to a Gameboard instance
* @param ptrbpositions a pointer to a BoardPosition instance
*/
void record_miss(Gamebaord* ptrgboard, BoardPosition* ptrbpositions);

/**
* If all ships have been sunk by the player, this function should return non-zero;
* otherwise, it returns zero
* @param ptrgboard a pointer to a Gameboard instance
* @return 1 all ships have been sunk, 0 if there are still ships not sunk
*/
int is_victory(Gameboard* ptrgboard);