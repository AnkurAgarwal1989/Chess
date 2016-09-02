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
			visited[K.Y][K.X].first.first = -1; //Unvisit the node and try from other nodes.
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
			visited[K.Y][K.X].first.first = -1;  //Mark the Goal non visited and go back
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
		if (visited[move.second.Y][move.second.X].first.first > -1) {
			continue;
		}
		else {
			visited[move.second.Y][move.second.X].first.first = moveCntr + 1;
			visited[move.second.Y][move.second.X].second = K;

			//If End was reached either in max possible moves or watchdog counter was trigerred..return
			if (findLongestPathAhead(B, visited, move.second, bestPath, moveCntr + 1, useDistanceHeuristic, bestMoves, watchdog)) {
				return true;
			}
			else {
				visited[move.second.Y][move.second.X].first.first = -1;
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
	visited[B.getStart().Y][B.getStart().X].first.first = 0;

	findLongestPathAhead(B, visited, B.getStart(), bestPath, moveCntr, useDistanceHeuristic, bestMoves, watchdog);

	if (bestPath.size() > 0) {
		//Path includes Start and End, so path length = size - 1
		std::cout << "\nPath found with length " << bestPath.size() - 1 << "\nStart\n";
		printPath(bestPath);
		std::cout << "\nEnd\n";
		return true;
	}

	else {
		std::cout << "No path found \n";
		return false;
	}
}

//Given a board, a start and an end, find longest path
//Longest path wihout repeating, is a path travelled by visiting every node.
void task5(std::vector<Position>& longestPath) {
	size_t BOARD_HEIGHT = 16;
	size_t BOARD_WIDTH = 16;
	size_t Start_X = 0;
	size_t Start_Y = 0;
	size_t End_X = 1;
	size_t End_Y = 1;

	Board<std::string> KB{ BOARD_HEIGHT, BOARD_WIDTH };
	bool ret = KB.setStart(Start_X, Start_Y);
	ret &= KB.setEnd(End_X, End_Y);
	if (ret) {
		if (KB.getStart() == KB.getEnd()) {
			std::cout << "Starting and Ending points are the same. \n";
			exit(1);
		}
		std::cout << "Initial State of Knight Board \n";
	}
	else {
		std::cout << "Start or End Point is OUTSIDE checkerboard area. Exiting";
		exit(1);
	}

	KB.printBoardState();
	std::cout << "\n";
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	solveForLongestPath(KB, longestPath, true); //True for using distance heuristic
	validateSequence(KB, longestPath, true);
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	std::cout << duration;

	std::cin.get();

}