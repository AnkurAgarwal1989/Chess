#pragma once
#include<string.h>
#include<string>
#include"Board.h"
#include<iostream>
#include<chrono>

//Function to sort on the first value of the PossibleMoves
bool sortMovesNot(std::pair<int, Position>& lhs, std::pair<int, Position>& rhs) {
	return lhs.first < rhs.first;
};

void saveBestPath(Board<std::string>& B, BoardData<Move>& visited, Position K, std::vector<Position>& bestPath) {
	//Retrieve the best path for future use
	bestPath.clear();
	Position p{ K };
	bestPath.push_back(p);
	while (p != B.getStart()) {
		Position nextP = visited[p.Y][p.X].second;
		bestPath.push_back(nextP);
		p = nextP;
	}
	std::reverse(bestPath.begin(), bestPath.end());
}

//Recursive function to search for the best path to the End Goal.
bool findLongestPathAhead(Board<std::string>& B, BoardData<Move>& visited, Position K, std::vector<Position>& bestPath, int moveCntr, bool useDistanceHeuristic, int& bestMoves, unsigned int& watchdog)
{
	//Return TRUE condition for recursion
	//If we have reached the Goal, return true;
	//printBoardData(visited);
	if (K == B.getEnd()) {
		//Everytime we reach the goal, increment watchdog
		++watchdog;

		if (moveCntr == (B.height*B.width - 1)) { //Maximum number of moves possible for a board reached
			bestMoves = moveCntr;
			saveBestPath(B, visited, K, bestPath);
			return true;
		}

		if (moveCntr > bestMoves) { //We are doing better than previous times
			//reset watchdog counter
			watchdog = 0;
			bestMoves = moveCntr;
			saveBestPath(B, visited, K, bestPath);
			std::cout << moveCntr << "\n";
			//Retrieve the best path for future use
			visited[K.Y][K.X].first = -1; //Unvisit and try from other nodes
			return false;
		}

		if (watchdog > (B.height * B.width)*(B.height * B.width)) {
		std::cout << "No change in " << watchdog << " iterations. Best value probably reached \n";
			return true;
		}
		else{					//We are doing worse than previous attempts
			visited[K.Y][K.X].first = -1;  //MArk the Goal non visited and go back
			return false;
		}
	}
		
	//printBoardData(visited);
	Moves possMoves;
	//at position K.
	//Get a list of open positions
	B.getValidMoves(K, possMoves, 2);

	if (possMoves.size() == 0) //No valid moves exist
		return false;

	//Sort moves by cost of Node.
	std::sort(possMoves.begin(), possMoves.end(), sortMovesNot);
	
	for (auto move : possMoves) {
		std::vector<Position> path;
		//std::cout << move.second.X <<" " << move.second.Y <<"\n";
		//If a node has been visited or if the previous cost of reaching the node is less than current cost + node cost...go to next node
		int costOfNextNode = 0;
		//costOfNextNode = B.COST[B.boardData[move.second.Y][move.second.X]];

		//If this node has been visited, we can not go here
		if (visited[move.second.Y][move.second.X].first > -1) {
			continue;
		}
		else {
			visited[move.second.Y][move.second.X].first = moveCntr + 1;
			visited[move.second.Y][move.second.X].second = K;
			if (findLongestPathAhead(B, visited, move.second, bestPath, moveCntr + 1, useDistanceHeuristic, bestMoves, watchdog)) {
				return true;
			}
			else {
				visited[move.second.Y][move.second.X].first = -1;
			}
		}
	}
	return false;
}

void solveForLongestPath(Board<std::string>& B, bool useDistanceHeuristic) {
	BoardData<Move> visited = BoardData<Move>{ B.height, std::vector<Move>{B.width, Move(-1, Position{ B.width, B.height }) } };
	std::vector<Position> bestPath;
	
	int bestMoves = 0;
	unsigned int watchdog = 0;
	int moveCntr = 0;
	visited[B.getStart().Y][B.getStart().X].first = 0;

	//printBoardData(visited);

	findLongestPathAhead(B, visited, B.getStart(), bestPath, moveCntr, useDistanceHeuristic, bestMoves, watchdog);
	
	if (bestPath.size() > 0) {
		std::cout << "\nPath found with length " << bestPath.size() << "\nStart\n";
		printPath(bestPath);
		std::cout << "\nEnd\n";
	}
	
	else {
		std::cout << "No path found \n";
	}
}

//Given a board, a start and an end, find longest path
//Longest path wihout repeating, is a path travelled by visiting every node.
void task5() {
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
	solveForLongestPath(KB, false); //True for using distance heuristic
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	std::cout << duration;

	std::cin.get();

}