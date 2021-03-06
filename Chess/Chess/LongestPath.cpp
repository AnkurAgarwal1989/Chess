#pragma once
#include<string.h>
#include<string>
#include"Board.h"
#include<iostream>
#include<chrono>

bool validateSequence(Board<std::string>& B, const std::vector<Position>& testPath, bool printLayout);

//Recursive function to search for the best path to the End Goal.
bool findLongestPathAhead(Board<std::string>& B, BoardData<Move>& visited, Position K, std::vector<Position>& bestPath, int moveCntr, bool useDistanceHeuristic, int& bestMoves, unsigned int& watchdog)
{
	//Return conditions for recursion
	//If we have reached the Goal, return true;
	if (K == B.getEnd()) {

		//Everytime we reach the goal, increment watchdog
		++watchdog;

		if (moveCntr > bestMoves) { //We are doing better than previous times
			watchdog = 0;			//reset watchdog counter
			bestMoves = moveCntr;
			
			B.getPathStart2End(visited, bestPath);
			//std::cout << moveCntr << "\n";

			//Store the current best path. This will be updated next time we see a better path
			visited[K.Y][K.X].cost.G = -1; //Unvisit the node and try from other nodes.
			return false;
		}

		if (moveCntr == (B.height*B.width - 1)) { //Maximum number of moves possible for a board reached
			bestMoves = moveCntr;
			B.getPathStart2End(visited, bestPath);
			return true;
		}

		if (watchdog > (B.height * B.width)) { //It has taken too long and we have not found a better path...exit
			std::cout << "No change in " << watchdog << " iterations. Best value possibly reached. \n";
			return true;
		}
		else{							   //We are doing worse than previous attempts
			visited[K.Y][K.X].cost.G = -1;  //Mark the Goal non visited and go back
			return false;
		}
	}
		
	//Get a list of open positions from current position.
	Moves possMoves;
	B.getValidMoves(K, possMoves, useDistanceHeuristic);

	if (possMoves.size() == 0) //No valid moves exist
		return false;

	//Sort moves by cost of Node. Highest cost nodes refer to 
	//nodes with more distance from End goal. we want those.
	//Most important part of the search.
	//Early on, we want to move as far as possible from the End goal.
	//Then we want to circle around (sort of) the End goal till we finally reach.
	std::sort(possMoves.begin(), possMoves.end(), sortMovesDesc);
	
	for (auto move : possMoves) {
		std::vector<Position> path;
		
		//If this node has been visited, we can not go here
		if (visited[move.pos.Y][move.pos.X].cost.G > -1) {
			continue;
		}
		else {
			visited[move.pos.Y][move.pos.X].cost.G = moveCntr + 1;
			visited[move.pos.Y][move.pos.X].pos = K;

			//If End was reached either in max possible moves or watchdog counter was trigerred..return
			if (findLongestPathAhead(B, visited, move.pos, bestPath, moveCntr + 1, useDistanceHeuristic, bestMoves, watchdog)) {
				return true;
			}
			else {
				visited[move.pos.Y][move.pos.X].cost.G = -1;
			}
		}
	}
	return false;
}

//Function to initiate Longest Path search.
//Returns true if found a path
//Returns false if no path found
bool solveForLongestPath(Board<std::string>& B, std::vector<Position>& bestPath, bool useDistanceHeuristic) {

	//Initialise the visited data with -1 cost and out of bounds positions
	BoardData<Move> visited = BoardData<Move>{ B.height, std::vector<Move>{B.width, Move(Cost{-1, 0}, Position{ B.width, B.height }) } };

	int bestMoves = 0;
	unsigned int watchdog = 0;
	int moveCntr = 0;

	//Cost of Start is 0
	visited[B.getStart().Y][B.getStart().X].cost.G = 0;

	findLongestPathAhead(B, visited, B.getStart(), bestPath, moveCntr, useDistanceHeuristic, bestMoves, watchdog);

	if (bestPath.size() > 0) {
		//Path includes Start and End, so path length = size - 1
		std::cout << "\nPath found with" << bestPath.size() - 1 << " moves\n";
		return true;
	}

	else {
		std::cout << "No path found \n";
		return false;
	}
}

//Given a board, a start and an end, find longest path
//Longest path wihout repeating, is a path travelled by visiting every node.
//Function to calculate **longest** path from Start to End.
//if printKnightMoves is true, the knight's position is displayed on the board
void longestPath(Board<std::string>& KB, std::vector<Position>& longestPath) {
	
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	solveForLongestPath(KB, longestPath, true); //True for using distance heuristic. This is always required for longest path
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	std::cout << "Time taken = " << duration / (1000) << " millisecond(s)\n";

}