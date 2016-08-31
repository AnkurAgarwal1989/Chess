#include"Board.h"
#include<iostream>
/*
bool getPath(Board<std::string>& B, BoardData<int>& visited, Position K, Position E, int moves)
{
	std::cout << "Current: " << K.X << ", " << K.Y << "\n";

	//Return TRUE condition for recursion
	//If we have reached the Goal, return true;
	if (K.X == E.X && K.Y == E.Y) { //Reached end...
		std::cout << "Path found in " << moves + 1 << " moves \n";
		return true;
	}

	printBoardState(visited);
	PossibleMoves PM;
	//at position K.
	//Get a list of open positions

	int bestPathCost = INT16_MAX;

	B.getValidMoves(K, PM);
	if (PM.size() == 0) //No valid moves exist
		return false;
	std::cout << "Size: " << PM.size() << "\n";
	for (auto move : PM) {
		std::cout << move.second.X << " " << move.second.Y << "\n";
	}
	std::sort(PM.begin(), PM.end(), sortMoves);
	for (auto move : PM) {
		//std::cout << move.second.X <<" " << move.second.Y <<"\n";
		if (visited[move.second.Y][move.second.X] > 0)
			continue;

		visited[move.second.Y][move.second.X] = moves;


	}
	if (getPath(B, visited, move.second, E, moves + 1)) {
		std::cout << "Move " << move.second.X << " " << move.second.Y << "\n";
		//return true;
	}
}
//if (pathFound)
//return true;
return false;
}

void solve(Board<std::string>& B, Position Start, Position End) {
	BoardData<int> visited = BoardData<int>{ B.height , std::vector<int>(B.width, 0) };
	int moves = 0;
	visited[Start.Y][Start.X] = 1;
	//printBoardState(visited);

	if (getPath(B, visited, Start, End, moves)) {
		std::cout << "\n Path Found";
	}

}*/