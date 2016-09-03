#pragma once
#include<string.h>
#include<string>
#include"Board.h"
#include<iostream>
#include<chrono>

bool validateSequence(Board<std::string>& B, const std::vector<Position>& testPath, bool printLayout);

//Recursive function to search for the best path to the End Goal.
//if useDistanceHeuristic is false, only the cost of the node is considered.
//If A* is used, the cost also includes the distance from the End goal.
bool findPathAhead(Board<std::string>& B, BoardData<Move>& visited, Moves& openNodes, bool useDistanceHeuristic)
{
	//If the start is a teleport point. mark the start as visited and start from teleport point
	Position S = B.getStart();
	if (B.canTeleport(S)) {
		std::cout << "----Starting from a Teleport location\n";
		visited[S.Y][S.X].cost.G = -1;  //Don't mark this node visited if it's a teleport
		B.setStart(S.X, S.Y);
		B.setPoint(S.X, S.Y, "T");
	}
	else {
		visited[S.Y][S.X].cost.G = 0;  //Start node can be marked visited
	}

	Moves possMoves;
	int moveCntr = 0;
	Move start;
	start.pos = B.getStart();
	openNodes.push_back(start);
	
	bool goalReached = false;
	bool noMovesLeft = false;
	while (!openNodes.empty()) {
		std::sort(openNodes.begin(), openNodes.end(), sortMovesDesc);// Descending. We will pop the nodes from the end. Smallest cost nodes popped first
		//printBoardData(visited);
		Move parent = openNodes[openNodes.size() - 1]; //Get element with smallest cost
		openNodes.pop_back();							//Remove the node from the open list;

		//if goal reached...break
		if (parent.pos == B.getEnd()) {
			goalReached = true;
			std::cout << "\nEnd reached\n";
			return true;
		}

		float costAtCurrentNode = visited[parent.pos.Y][parent.pos.X].cost.H;

		//get a list of valid moves
		B.getValidMoves(parent.pos, possMoves, useDistanceHeuristic); //A*

		for (auto move : possMoves) {
			int costOfNextNode = move.cost.G;
			if (visited[move.pos.Y][move.pos.X].cost.G > -1 &&									//if already visited and 
				visited[move.pos.Y][move.pos.X].cost.H <= costAtCurrentNode + costOfNextNode) {	//new cost worse than old cost, continue
				continue;
			}
			//We have a better way of getting here
			else {
				visited[move.pos.Y][move.pos.X].cost.G = 0;
				visited[move.pos.Y][move.pos.X].cost.H = costAtCurrentNode + costOfNextNode;
				visited[move.pos.Y][move.pos.X].pos = parent.pos;
				move.cost.G = costAtCurrentNode + costOfNextNode; //Update the cost of getting to this node, Heuristic should not be used for sorting moves on open nodes
				//move.cost.H = visited[move.pos.Y][move.pos.X].cost.H;
				openNodes.push_back(move);							   //Add to list of explorable nodes
			}
		}
	}
	return goalReached;
}

//Function to initiate Best Path search.
//Returns true if found a path
//Returns false if no path found
bool solveForBestPath(Board<std::string>& B, std::vector<Position>& bestPath, bool useDistanceHeuristic) {
	Moves openNodes;
	//Initialise the visited data with -1 cost and out of bounds positions
	//For visited, G&H are repurposed to be visitedFlag and cost, resp.
	BoardData<Move> visited = BoardData<Move>{ B.height , std::vector<Move>{B.width, Move(Cost{-1, 0.0}, Position{ B.width, B.height }) } };
	if (findPathAhead(B, visited, openNodes, useDistanceHeuristic)) {
		int pathCost = 0;
		pathCost = B.getPathStart2End(visited, bestPath);
		std::cout << "\nPath found with " << bestPath.size() - 1 << " moves and cost " << pathCost << "\n";
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

	std::cout << "Time taken = " << duration / (1000) << " millisecond(s)\n";

}

