#pragma once
#include<string.h>
#include<string>
#include"Board.h"
#include<iostream>
#include<fstream>
#include<chrono>

//Template spec to print the Visited data
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

void printPath(const std::vector<Position>& P)
{
	for (auto p : P) {
		std::cout << "(" << p.X << ", " << p.Y << ")\n";
	}
}

//Function to sort on the first value of the PossibleMoves
bool sortMoves(std::pair<int, Position>& lhs, std::pair<int, Position>& rhs) {
	return lhs.first < rhs.first;
};

//Recursive function to search for the best path to the End Goal.
//if useDistanceHeuristic is false, only the cost of the node is considered.
//If A* is used, the cost also includes the distance from the End goal.
bool findPathAhead(Board<std::string>& B, BoardData<Move>& visited, Position K, int moves, bool useDistanceHeuristic)
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
	B.getValidMoves(K, possMoves, useDistanceHeuristic);

	if (possMoves.size() == 0) //No valid moves exist...backtrack
		return false;

	//Sort moves by cost of Node.
	std::sort(possMoves.begin(), possMoves.end(), sortMoves);

	for (auto move : possMoves) {
		std::vector<Position> path;
		//std::cout << move.second.X <<" " << move.second.Y <<"\n";
		//If a node has been visited or if the previous cost of reaching the node is less than current cost + node cost...go to next node
		int costOfNextNode = 0;
		costOfNextNode = B.COST[B.boardData[move.second.Y][move.second.X]];
		if (visited[move.second.Y][move.second.X].first > -1 && visited[move.second.Y][move.second.X].first <= moves + costOfNextNode){
			continue;
		}
		else {
			visited[move.second.Y][move.second.X].first = moves + 1;
			visited[move.second.Y][move.second.X].second = K;
			findPathAhead(B, visited, move.second, moves + 1, useDistanceHeuristic);
			}
	}
	return false;
}

void solveForPath(Board<std::string>& B, bool useDistanceHeuristic) {
	BoardData<Move> visited = BoardData<Move>{ B.height , std::vector<Move>{B.width, Move(-1, Position{0,0}) } };
	//BoardData<Move> visited = BoardData<Move>{ B.height , std::vector<Move>{B.width, Move(-1, Position{0,0}) } };
	int moves = 0;
	visited[B.getStart().Y][B.getStart().X].first = 0;
	
	std::vector<Position> bestPath;
	int pathCost = 0;
	findPathAhead(B, visited, B.getStart(), moves, useDistanceHeuristic);
	Position K = B.getEnd();
	pathCost = visited[K.Y][K.X].first;
	//If the visited cell for END is marked, it means a path was found
	if (pathCost > -1){
		//If a valid path was found..we can take the path from the 'visited' datastructure;
		
		bestPath.push_back(K);
		while (K != B.getStart()) {
			Position P = visited[K.Y][K.X].second;
			bestPath.push_back(P);
			K = P;
		}
		std::cout << "\nPath found with length " << bestPath.size() - 1 <<" and cost " <<pathCost << "\nStart\n";
		std::reverse(bestPath.begin(), bestPath.end());

		printPath(bestPath);
		std::cout << "\nEnd\n";
	}
	else {
		std::cout << "No path found \n";
	}
}

void readGameFile(Board<std::string>& B, std::string filename) {
	std::ifstream fs(filename);
	std::string line;
	size_t id_x = 0;
	size_t id_y = 0;

	while (std::getline(fs, line))
	{
		for (auto c : line) {
			B.setPoint(id_x, id_y, std::string(1, c));
			++id_x;
			
			if (id_x == B.width - 1) {
				id_x = 0;
				++id_y;
			}
			if (id_y == B.height) {
				std::cout << "File has more data than Game Size. Only reading some part of file\n";
				break;
			}
			std::cout << c;
		}

	}
}

void task2() {
	size_t BOARD_HEIGHT = 8;
	size_t BOARD_WIDTH = 8;
	size_t Start_X = 2;
	size_t Start_Y = 2;
	size_t End_X = 7;
	size_t End_Y = 7;

	Board<std::string> KB{BOARD_HEIGHT, BOARD_WIDTH};
	readGameFile(KB, "map.txt");
	bool ret = KB.setStart(Start_X, Start_Y);
	ret &= KB.setEnd(End_X, End_Y);
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
	KB.setPoint(1, 4, "W");
	KB.setPoint(2, 4, "B");
	KB.setPoint(3, 4, "W");
	KB.setPoint(5, 3, "B");
	KB.setPoint(4, 5, "B");
	KB.setPoint(5, 4, "B");
	KB.setPoint(4, 6, "B");
	KB.addTeleportPoints(4, 3, 5, 6);
	KB.printBoardState();
	std::cout << "\n";
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	solveForPath(KB, true); //True for using distance heuristic
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	std::cout << duration;
	
	std::cin.get();

}