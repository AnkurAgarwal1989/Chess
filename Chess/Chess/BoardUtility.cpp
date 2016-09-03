#include"BoardUtility.h"

//Function to sort on cost of Move in Ascending order.
//Sorting by sum of cost and heuristic
bool sortMoves(Move& lhs, Move& rhs) {
	return lhs.cost.G + lhs.cost.H < rhs.cost.G + rhs.cost.H;
};

//Function to sort on cost of Move in Descending order.
//Sorting by sum of cost and heuristic
bool sortMovesDesc(Move& lhs, Move& rhs) {
	//return (lhs.first.first + lhs.first.second) > (rhs.first.first + rhs.first.second);
	if ((int)lhs.cost.G + lhs.cost.H == (int)rhs.cost.G + rhs.cost.H)  ///if cost is same return one with lowest cost G
		return lhs.cost.G > rhs.cost.G;
	return ((int)lhs.cost.G + lhs.cost.H > (int)rhs.cost.G + rhs.cost.H);
};


//Tempalte spec to print the Visited data
template <>
void printBoardData<Move>(BoardData<Move>& data) {

	//The G part of this data struct marks a node visited.
	std::cout << "\nNode Visited?\n";
	for (auto y_iter : data) {
		for (auto x_iter : y_iter) {
			std::cout << x_iter.cost.G << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	//The H part of this data struct holds the cost of full path for reaching the node.
	std::cout << "\nPath-to-Node Cost\n";
	for (auto y_iter : data) {
		for (auto x_iter : y_iter) {
			std::cout << x_iter.cost.H << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	//The 'pos' part of this data struct holds the best location to reach the cell from.
	std::cout << "\nBest parent positions\n";
	for (auto y_iter : data) {
		for (auto x_iter : y_iter) {
			std::cout << "(" << x_iter.pos.X << "," << x_iter.pos.Y << ")";
		}
		std::cout << "\n";
	}
}

//Function to pretty print the final path
void printPath(const std::vector<Position>& P)
{
	std::cout << "\nStart\n";
	for (auto p : P) {
		std::cout << "(" << p.X << ", " << p.Y << ")\n";
	}
	std::cout << "End\n";
}