#include"BoardUtility.h"

//Function to sort on cost of Move in Ascending order.
bool sortMoves(Move& lhs, Move& rhs) {
	return lhs.cost.G + lhs.cost.H < rhs.cost.G + rhs.cost.H;
};

//Function to sort on cost of Move in Descending order.
//Sorting by sum of cost and heuristic
bool sortMovesDesc(Move& lhs, Move& rhs) {
	//return (lhs.first.first + lhs.first.second) > (rhs.first.first + rhs.first.second);
	return lhs.cost.G + lhs.cost.H > rhs.cost.G + rhs.cost.H;
};


//Tempalte spec to print the Visited data
template <>
void printBoardData<Move>(BoardData<Move>& data) {
	for (auto y_iter : data) {
		for (auto x_iter : y_iter) {
			std::cout << x_iter.cost.G << " ";
		}
		std::cout << "\n";
	}

	for (auto y_iter : data) {
		for (auto x_iter : y_iter) {
			std::cout << "(" << x_iter.pos.X << "," << x_iter.pos.Y << ")";
		}
		std::cout << "\n";
	}
}

 