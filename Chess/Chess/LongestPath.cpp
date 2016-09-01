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

//Recursive function to search for the best path to the End Goal.
bool findLongestPathAhead(Board<std::string>& B, BoardData<Move>& visited, Position K, int moves, bool useDistanceHeuristic, int& maxMoves)
{
	//Return TRUE condition for recursion
	//If we have reached the Goal, return true;

	if (K == B.getEnd()) {
		if (moves > maxMoves || moves == (B.height*B.width - 1)) { //Can't do more than this
			maxMoves = moves;
			return true;
		}
		else if(moves <= maxMoves){
			visited[K.Y][K.X].first = -1;  //MArk the Goal non visited and go back
			return false;
		}
	}
		
	//printBoardData(visited);
	Moves possMoves;
	//at position K.
	//Get a list of open positions
	B.getValidMoves(K, possMoves, useDistanceHeuristic);

	if (possMoves.size() == 0) //No valid moves exist...backtrack
		return false;
	for (auto& move : possMoves) {
		//Find moves which are possible after the successor
		Moves futureMoves = Moves{};
		B.getValidMoves(move.second, futureMoves, useDistanceHeuristic);
		move.first = futureMoves.size();
		for (auto fM : futureMoves) {
			if (visited[fM.second.Y][fM.second.X].first > -1)
				--move.first;
		}
	}

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
			visited[move.second.Y][move.second.X].first = moves + 1;
			visited[move.second.Y][move.second.X].second = K;
			if (findLongestPathAhead(B, visited, move.second, moves + 1, useDistanceHeuristic, maxMoves)) {
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
	BoardData<Move> visited;// = BoardData<Move>{ B.height , std::vector<Move>{B.width, Move(-1, Position{ 9, 9 }) } };
	//BoardData<Move> visited = BoardData<Move>{ B.height , std::vector<Move>{B.width, Move(-1, Position{0,0}) } };

	std::vector<Position> bestPath;
	int pathCost = 0;
	int maxMoves = 0;
	int bestMoves = -1;
	int moves = 0;
	while (maxMoves > bestMoves && maxMoves < (B.height * B.width)) {
		moves = 0;
		visited = BoardData<Move>{ B.height , std::vector<Move>{B.width, Move(-1, Position{ B.width, B.height }) } };
		visited[B.getStart().Y][B.getStart().X].first = 0;
		bestMoves = maxMoves;
		std::cout << "Best: " << bestMoves;
		findLongestPathAhead(B, visited, B.getStart(), moves, useDistanceHeuristic, maxMoves);
	}
	printBoardData(visited);
	Position K = B.getEnd();
	pathCost = visited[K.Y][K.X].first;
	//If the visited cell for END is marked, it means a path was found
	if (pathCost > -1) {
		//If a valid path was found..we can take the path from the 'visited' datastructure;

		bestPath.push_back(K);
		while (K != B.getStart()) {
			Position P = visited[K.Y][K.X].second;
			bestPath.push_back(P);
			K = P;
		}
		std::cout << "\nPath found with length " << bestPath.size() - 1 << " and cost " << pathCost << "\nStart\n";
		std::reverse(bestPath.begin(), bestPath.end());

		printPath(bestPath);
		std::cout << "\nEnd\n";
		printBoardData(visited);
	}
	else {
		std::cout << "No path found \n";
	}
}

//Given a board, a start and an end, find longest path
//Longest path wihout repeating, is a path travelled by visiting every node.
void task5() {
	size_t BOARD_HEIGHT = 32;
	size_t BOARD_WIDTH = 32;
	size_t Start_X = 0;
	size_t Start_Y = 0;
	size_t End_X = 2;
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