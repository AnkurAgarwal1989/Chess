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


//Alias to hold Cost(first) and New Position (second) 
//We will sort on first for smallest cost.
using Move = std::pair<int, Position>;
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

void printPath(const std::vector<Position>& P);

template <>
void printBoardData<Move>(BoardData<Move>& data);
/***************************************************************/

