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
			std::cout << "\nEnd reached\n";
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


//Function to calculate path from Start to End.
//if findShortestPath is true, A* algoithm is used
//if printKnightMoves is true, the knight's position is displayed on the board
void pathPlanning(Board<std::string>& KB, std::vector<Position>& bestPath, bool findShortestPath) {
	
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	//std::vector<Position> bestPath;
	solveForBestPath(KB, bestPath, findShortestPath); //True for using distance heuristic
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	std::cout << "Time taken = " << duration / (1000) << " milliseconds\n";

}

