#include<vector>

template <typename T>
using BoardData = std::vector< std::vector<T> >;


struct Position {
	unsigned int X;
	unsigned int Y;

	Position(unsigned int x, unsigned int y) :X(x), Y(y) {};
	
};


template <typename T>
struct Board {
	size_t height;
	size_t width;
	BoardData<T> boardData;

	Board(size_t boardHeight, size_t boardWidth){
		height = boardHeight;
		width = boardWidth;
		boardData = BoardData<T>{ height, { width, "." }};
	}

	//Function to validate if a point if valid (inside the board boundaries)
	//returns true if point is valid
	bool validPoint(const int X, const int Y) {  //We want to make these int to check for negative numbers 
		return (X >= 0 && X < width && Y >= 0 && Y < height);
	}

	//Utility function to add a point to the chessboard
	//Use this to only add Start, End or terrain points like W R B etc.
	//returns true if successful
	//returns false if not a valid point
	bool setPoint(const unsigned int X, const unsigned int Y, const std::string pointName) {
		if (validPoint(X, Y)) {
			boardData[Y][X] = pointName;
			return true;
		}
		return false;
	}

	//Function to print the current state of the board
	//if moveKnight is false...only S and E will be printed.
	//if moveKnight is true...K will also be printed. If K after moving lies on S or E, they will be replaced by K.
	void printBoardState(bool printKnight, const Position K) {
		std::string curVal_K;
		if (printKnight) {
			//Store the current state at K location so we can revert back after the print
			//assign "K" to the K location
			curVal_K = boardData[K.Y][K.X];
			boardData[K.Y][K.X] = "K";
		}
		for (auto y_iter : boardData) {
			for (auto x_iter : y_iter) {
				std::cout << x_iter << " ";
			}
			std::cout << "\n";
		}
		if (printKnight) { //revert
			boardData[K.Y][K.X] = curVal_K;
		}
	}

	//print board state only without the knight position
	void printBoardState() {
		printBoardState(false, { 0,0 });
	}
};