#include"BoardUtility.h"

//Function to sort on cost of Move in Ascending order.
bool sortMoves(std::pair<int, Position>& lhs, std::pair<int, Position>& rhs) {
	return lhs.first < rhs.first;
};

//Function to sort on cost of Move in Descending order.
bool sortMovesDesc(std::pair<int, Position>& lhs, std::pair<int, Position>& rhs) {
	return lhs.first > rhs.first;
};


//Tempalte spec to print the Visited data
template <>
void printBoardData<Move>(BoardData<Move>& data) {
	for (auto y_iter : data) {
		for (auto x_iter : y_iter) {
			std::cout << x_iter.first << " ";
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