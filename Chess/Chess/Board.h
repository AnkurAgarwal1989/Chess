#pragma once
#include<vector>
#include<algorithm>
#include<utility>
#include<map>
#include<array>
#include"BoardUtility.h"


template <typename T>
struct Board {
	size_t height;
	size_t width;
	BoardData<T> boardData;

	std::map<std::string, int> COST;

	//Arrays to keep account of the moves the knight can make
	std::array<int, 4> Dx{ 0, -1, 1, 0 };
	std::array<int, 4> Dy{ -1, 0, 0, 1 };

	std::array<int, 8> Mx{ 1, -1, -2, -2, 2, 2, 1, -1 };
	std::array<int, 8> My{ -2, -2, 1, -1, 1, -1, 2, 2 };

	
	Board(size_t boardHeight, size_t boardWidth){
		height = boardHeight;
		width = boardWidth;
		boardData = BoardData<T>{ height, { width, "." }};
		COST["."] = 1;
		COST["W"] = 2;
		COST["L"] = 5;
	}

	bool setStart(const unsigned int X, const unsigned int Y) {
		if (setPoint(X, Y, "S")) {
			Start.X = X;
			Start.Y = Y;
			return true;
		}
		return false;
	}

	const Position getStart() {
		return Start;
	}

	bool setEnd(const unsigned int X, const unsigned int Y) {
		if (setPoint(X, Y, "E")) {
			End.X = X;
			End.Y = Y;
			return true;
		}
		return false;
	}

	const Position getEnd() {
		return End;
	}

	//Utility function to add a point to the chessboard
	//Use this to only add Start, End or terrain points like W R B etc.
	//returns true if successful
	//returns false if not a valid point
	bool setPoint(const unsigned int X, const unsigned int Y, const std::string pointName) {
		if (pointWithinBoundary(X, Y)) {
			boardData[Y][X] = pointName;
			return true;
		}
		return false;
	}
	
	//Function to validate if a point is inside the board boundaries)
	//returns true if point is valid
	//Points wihtin the boundary can then be converted to unsigned ints
	bool pointWithinBoundary(const int X, const int Y) {  //We want to make these int to check for negative numbers 
		return (X >= 0 && X < static_cast<int>(width) && Y >= 0 && Y < static_cast<int>(height));
	}

	//Function to check if point lies within board bounadaries
	//Also checks if destination is a Rock or Barrier...we can not go to these
	bool validMove(const int X, const int Y) {
		if (pointWithinBoundary(X, Y)) {
			if (boardData[Y][X] == "R" || boardData[Y][X] == "B")
				return false;
			return true;
		}
		return false;
	}

	//Function to check if move is a valid Knight Move
	//Function will take 2 valid positions
	bool validKnightMove(const Position prevK, const int nextK_x, const int nextK_y) {
		if (validMove(nextK_x, nextK_y)) {
			Position nextK{ nextK_x, nextK_y };
			if (std::abs(static_cast<int>(prevK.X) - static_cast<int>(nextK.X)) * std::abs(static_cast<int>(prevK.Y) - static_cast<int>(nextK.Y)) == 2)
				return true;
		}
		return false;
	}

	//Function to check if we will hit the barrier when moving in a direction
	bool isBarrierInPath(Position K, const int dX, const int dY) {
		int newX = K.X + dX;
		int newY = K.Y + dY;
		if (pointWithinBoundary(newX, newY)) //if it is not a valid move...return true so the isBarrier can flag it
			if (boardData[newY][newX] == "B")
				return true;
			else
				return false;
		return true;
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
		printBoardData(boardData);
		if (printKnight) { //revert
			boardData[K.Y][K.X] = curVal_K;
		}
	}

	//print board state only without the knight position
	void printBoardState() {
		printBoardData(boardData);
	}

private:
	Position Start{ 0,0 };
	Position End{ 0,0 };
};
