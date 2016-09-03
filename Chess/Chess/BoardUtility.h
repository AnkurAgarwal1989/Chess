#pragma once
#include <vector>
#include <iostream>
#include <math.h>
#include <utility>
#include <map>

/***************************************************************/
//Struct to handle position values
//This Position should only hold values that can fit on the board
struct Position {
	unsigned int X;
	unsigned int Y;

	Position(unsigned int x, unsigned int y) :X(x), Y(y) {};
	Position(int x, int y) :X(static_cast<unsigned int>(x)), Y(static_cast<unsigned int>(y)) {};

	const bool operator==(const Position& rhs) const {
		return (X == rhs.X && Y == rhs.Y);
	}

	const bool operator!=(const Position& rhs) const {
		return (X != rhs.X || Y != rhs.Y);
	}
};
/***************************************************************/


//Struct to hold Cost of cell (G)
//and heuristic cost (H)
//We will sort on first for smallest cost.
struct Cost {
	int G;  //Cost of Cell
	float H;	//Heuristic Cost

	Cost() : G(0), H(0) {};
	Cost(int g, float h) : G(g), H(h) {};
};

//Struct to define a Move
//A move is defined by the cost of the new location and the position of the new location
struct Move {
	Cost cost;
	Position pos;

	Move() : cost(), pos(0,0) {};
	Move(Cost c, Position p) : cost(c), pos(p) {};
};

using Moves = std::vector< Move >;


/***************************************************************/

//Type to contain data on board.
template <typename T>
using BoardData = std::vector< std::vector<T> >;

template <typename T>
void printBoardData(BoardData<T>& data) {
	for (auto y_iter : data) {
		for (auto x_iter : y_iter) {
			std::cout << x_iter << " ";
		}
		std::cout << "\n";
	}
}

template <>
void printBoardData<Move>(BoardData<Move>& data);

void printPath(const std::vector<Position>& P);

bool sortMoves(Move& lhs, Move& rhs);

bool sortMovesDesc(Move& lhs, Move& rhs);

/***************************************************************/

