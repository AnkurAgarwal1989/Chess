#pragma once
#include<string.h>
#include<string>
#include"Board.h"
#include"BoardUtility.h"
#include<iostream>

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

void printPath(std::vector<Position>& P)
{
	for (auto p : P) {
		std::cout << "(" << p.X << ", " << p.Y << ")\n";
	}
}

//Function to sort on the first value of the PossibleMoves
bool sortMoves(std::pair<int, Position>& lhs, std::pair<int, Position>& rhs) {
	return lhs.first < rhs.first;
};

bool getPath_Naiive(Board<std::string>& B, BoardData<Move>& visited, Position K, int moves)
{
	//Return TRUE condition for recursion
	//If we have reached the Goal, return true;
	if (K == B.getEnd()) {
		//std::cout << "Path found in " << moves + 1 << " moves \n";
		return true;
	}
	
	//printBoardData(visited);
	Moves possMoves;
	//at position K.
	//Get a list of open positions
	B.getValidMoves(K, possMoves);

	if (possMoves.size() == 0) //No valid moves exist...backtrack
		return false;

	//for (auto move : possMoves) {
	//	std::cout << move.second.X << " " << move.second.Y << "\n";
	//}
	//std::sort(possMoves.begin(), possMoves.end(), sortMoves);
	for (auto move : possMoves) {
		std::vector<Position> path;
		//std::cout << move.second.X <<" " << move.second.Y <<"\n";
		//If a node has been visited or if the previous cost of reaching the node is less than current cost + node cost...go to next node
		if (visited[move.second.Y][move.second.X].first > -1 && visited[move.second.Y][move.second.X].first <= moves + 1) {
			continue;
		}
		else {
			visited[move.second.Y][move.second.X].first = moves + 1;
			visited[move.second.Y][move.second.X].second = K;
			getPath_Naiive(B, visited, move.second, moves + 1);
			}
	}
	return false;
}

void solve(Board<std::string>& B) {
	BoardData<Move> visited = BoardData<Move>{ B.height , std::vector<Move>{B.width, Move(-1, Position{0,0}) } };
	//BoardData<Move> visited = BoardData<Move>{ B.height , std::vector<Move>{B.width, Move(-1, Position{0,0}) } };
	int moves = 0;
	visited[B.getStart().Y][B.getStart().X].first = 0;
	
	std::vector<Position> bestPath;

	getPath_Naiive(B, visited, B.getStart(), moves);
	Position K = B.getEnd();
	//If the visited cell for END is marked, it means a path was found
	if (visited[K.Y][K.X].first > -1){
		//If a valid path was found..we can take the path from the 'visited' datastructure;
		
		bestPath.push_back(K);
		while (K != B.getStart()) {
			Position P = visited[K.Y][K.X].second;
			bestPath.push_back(P);
			K = P;
		}
		std::cout << "\nPath found with length " << bestPath.size() - 1 << "\nStart\n";
		std::reverse(bestPath.begin(), bestPath.end());

		printPath(bestPath);
		std::cout << "\nEnd\n";
	}
	else {
		std::cout << "No path found \n";
	}


}

void task2() {
	size_t BOARD_HEIGHT = 7;
	size_t BOARD_WIDTH = 7;
	size_t Start_X = 2;
	size_t Start_Y = 2;
	size_t End_X = 6;
	size_t End_Y = 6;

	Board<std::string> KB{BOARD_HEIGHT, BOARD_WIDTH};
	bool ret = KB.setStart(Start_X, Start_Y);
	ret &= KB.setEnd(End_X, End_X);
	if (ret) {
		if (KB.getStart() == KB.getEnd()) {
			std::cout << "Starting and Ending points are the same. \n";
			exit(1);
		}
		std::cout << "Initial State of Knight Board \n";
		//KB.printBoardState();
		//validateSequence(KB);
	}
	else {
		std::cout << "Start or End Point is OUTSIDE checkerboard area. Exiting";
		exit(1);
	}

	//KB.setPoint(0, 4, "B");
	KB.setPoint(4, 4, "B");
	KB.setPoint(1, 4, "B");
	KB.setPoint(2, 4, "B");
	KB.setPoint(3, 4, "B");
	KB.setPoint(5, 3, "B");
	//KB.setPoint(6, 4, "B");

	KB.printBoardState();
	solve(KB);
	std::cin.get();

}