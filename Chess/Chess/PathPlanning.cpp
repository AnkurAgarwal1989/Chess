#pragma once
#include<string.h>
#include<string>
#include"Board.h"
#include<iostream>
#include<chrono>

bool validateSequence(Board<std::string>& B, const std::vector<Position>& testPath, bool printLayout);

void printPath(const std::vector<Position>& P)
{
	for (auto p : P) {
		std::cout << "(" << p.X << ", " << p.Y << ")\n";
	}
}

//Recursive function to search for the best path to the End Goal.
//if useDistanceHeuristic is false, only the cost of the node is considered.
//If A* is used, the cost also includes the distance from the End goal.
bool findPathAhead(Board<std::string>& B, BoardData<Move>& visited, Moves& openNodes, Position K, bool useDistanceHeuristic)
{
	Moves possMoves;
	int moveCntr = 0;
	openNodes.push_back({ Cost{ 0,0 }, B.getStart() });
	bool goalReached = false;
	bool noMovesLeft = false;
	while (!openNodes.empty() && !goalReached) {
		
		std::sort(openNodes.begin(), openNodes.end(), sortMovesDesc);// We will pop the nodes the end.Smallest cost nodes popped first
		Move parent = openNodes[openNodes.size() - 1]; //Get element with smallest cost
		openNodes.pop_back();							//Remove the node from the open list;

		//if goal reached...break
		if (parent.second == B.getEnd()) {
			goalReached = true;
			std::cout << "goal reached\n";
			return true;
		}

		int costAtCurrentNode = visited[parent.second.Y][parent.second.X].first.first;

		//get a list of valid moves
		B.getValidMoves(parent.second, possMoves, useDistanceHeuristic); //A*

		for (auto move : possMoves) {
			int costOfNextNode = move.first.first;
			if (visited[move.second.Y][move.second.X].first.first > -1 &&									//if already visited and 
				visited[move.second.Y][move.second.X].first.first <= costAtCurrentNode + costOfNextNode) {	//new cost worse than old cost, continue
				continue;
			}
			//We have a better way of getting here
			else {
				visited[move.second.Y][move.second.X].first.second = moveCntr++;
				visited[move.second.Y][move.second.X].first.first = costAtCurrentNode + costOfNextNode;
				visited[move.second.Y][move.second.X].second = parent.second;
				openNodes.push_back(move);
			}
		}
	}
	return false;
}

//Function to initiate Best Path search.
//Returns true if found a path
//Returns false if no path found
bool solveForBestPath(Board<std::string>& B, std::vector<Position>& bestPath, bool useDistanceHeuristic) {
	Moves openNodes;
	//Initialise the visited data with -1 cost and out of bounds positions
	BoardData<Move> visited = BoardData<Move>{ B.height , std::vector<Move>{B.width, Move(Cost{-1, 0}, Position{ B.width, B.height }) } };

	//Cost of Start is 0
	visited[B.getStart().Y][B.getStart().X].first.first = 0;

	if (findPathAhead(B, visited, openNodes, B.getStart(), useDistanceHeuristic)) {
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
	return false;
}

void task2(std::vector<Position>& bestPath) {
	size_t BOARD_HEIGHT = 8;
	size_t BOARD_WIDTH = 8;
	size_t Start_X = 0;
	size_t Start_Y = 0;
	size_t End_X = BOARD_HEIGHT - 1;
	size_t End_Y = BOARD_HEIGHT - 1;

	Board<std::string> KB{BOARD_HEIGHT, BOARD_WIDTH};

	KB.readGameFile("map.txt", ' ');

	bool ret = KB.setStart(Start_X, Start_Y);
	ret &= KB.setEnd(End_X, End_Y);
	if (ret) {
		if (KB.getStart() == KB.getEnd()) {
			std::cout << "Starting and Ending points are the same. \n";
			exit(1);
		}
		std::cout << "Initial State of Knight Board \n";
		//validateSequence(KB);
	}
	else {
		std::cout << "Start or End Point is OUTSIDE checkerboard area. Exiting";
		exit(1);
	}

	//KB.setPoint(0, 4, "B");
	/*KB.setPoint(4, 4, "B");
	KB.setPoint(1, 4, "W");
	KB.setPoint(2, 4, "B");
	KB.setPoint(3, 4, "W");
	KB.setPoint(5, 3, "B");
	KB.setPoint(4, 5, "B");
	KB.setPoint(5, 4, "B");
	KB.setPoint(4, 6, "B");
	KB.addTeleportPoints(4, 3, 5, 6);*/
	KB.printBoardState();
	std::cout << "\n";
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	//std::vector<Position> bestPath;
	solveForBestPath(KB, bestPath, true); //True for using distance heuristic
	validateSequence(KB, bestPath, true);
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	std::cout << duration;
	
	std::cin.get();

}

