#include"BoardUtility.h"

//Function to sort on cost of Move in Ascending order.
bool sortMoves(Move& lhs, Move& rhs) {
	return lhs.first.first + lhs.first.second < rhs.first.first + rhs.first.second;
};

//Function to sort on cost of Move in Descending order.
bool sortMovesDesc(Move& lhs, Move& rhs) {
	return lhs.first.first + lhs.first.second > rhs.first.first + rhs.first.second;
};


//Tempalte spec to print the Visited data
template <>
void printBoardData<Move>(BoardData<Move>& data) {
	for (auto y_iter : data) {
		for (auto x_iter : y_iter) {
			std::cout << x_iter.first.first << " ";
		}
		std::cout << "\n";
	}

	for (auto y_iter : data) {
		for (auto x_iter : y_iter) {
			std::cout << "(" << x_iter.second.X << "," << x_iter.second.Y << ")";
		}
		std::cout << "\n";
	}
}

 