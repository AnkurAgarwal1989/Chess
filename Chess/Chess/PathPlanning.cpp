#pragma once
#include<string.h>
#include<string>
#include"Board.h"
#include<iostream>
#include<chrono>

void printPath(const std::vector<Position>& P)
{
	for (auto p : P) {
		std::cout << "(" << p.X << ", " << p.Y << ")\n";
	}
}

//Recursive function to search for the best path to the End Goal.
//if useDistanceHeuristic is false, only the cost of the node is considered.
//If A* is used, the cost also includes the distance from the End goal.
bool findPathAhead(Board<std::string>& B, BoardData<Move>& visited, Position K, int moveCntr, bool useDistanceHeuristic)
{
	//Return TRUE condition for recursion
	//If we have reached the Goal, return true;
	if (K == B.getEnd()) {
		//std::cout << "Path found in " << moves + 1 << " moves \n";
		return true;
	}
	
	printBoardData(visited);
	Moves possMoves;
	//at position K.
	//Get a list of open positions
	B.getValidMoves(K, possMoves, useDistanceHeuristic); //A*

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
		if (visited[move.second.Y][move.second.X].first > -1 && visited[move.second.Y][move.second.X].first <= moveCntr + costOfNextNode){
			continue;
		}
		else {
			visited[move.second.Y][move.second.X].first = moveCntr + 1;
			visited[move.second.Y][move.second.X].second = K;
			//find a path from this location. Don't return here.
			if (findPathAhead(B, visited, move.second, moveCntr + 1, useDistanceHeuristic) == true)
				return true;
			}
	}
	return false;
}

//Function to initiate Best Path search.
//Returns true if found a path
//Returns false if no path found
bool solveForBestPath(Board<std::string>& B, std::vector<Position>& bestPath, bool useDistanceHeuristic) {

	//Initialise the visited data with -1 cost and out of bounds positions
	BoardData<Move> visited = BoardData<Move>{ B.height , std::vector<Move>{B.width, Move(-1, Position{ B.width, B.height }) } };

	int moveCntr = 0;

	//Cost of Start is 0
	visited[B.getStart().Y][B.getStart().X].first = 0;

	if (findPathAhead(B, visited, B.getStart(), moveCntr, useDistanceHeuristic)) {
		int pathCost = 0;
		pathCost = B.getPathStart2End(visited, bestPath);
		//Path contains Start and End, so number of moves = size - 1
		std::cout << "\nPath found with length " << bestPath.size() - 1 << " and cost " << pathCost << "\nStart\n";
		printPath(bestPath);
		std::cout << "\nEnd\n";
		return true;
	}
	else {
		std::cout << "No path found \n";
		return false;
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
	std::vector<Position> bestPath;
	solveForBestPath(KB, bestPath, true); //True for using distance heuristic
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	std::cout << duration;
	
	std::cin.get();

}