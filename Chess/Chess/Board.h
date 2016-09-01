#pragma once
#include<vector>
#include<algorithm>
#include<utility>
#include<map>
#include<array>
#include"BoardUtility.h"

using TeleportPoints = std::vector<std::pair<Position, Position>>;

template <typename T>
struct Board {
	size_t height;
	size_t width;
	BoardData<T> boardData;
	TeleportPoints listTeleport;
	std::map<std::string, int> COST;

	//Arrays to keep account of the moves the knight can make
	std::array<int, 4> Dx{ 0, -1, 1, 0 };
	std::array<int, 4> Dy{ -1, 0, 0, 1 };

	std::array<int, 8> Mx{ 1, -1, -2, -2, 2, 2, 1, -1 };
	std::array<int, 8> My{ -2, -2, 1, -1, 1, -1, 2, 2 };

	
	Board(size_t boardHeight, size_t boardWidth){
		height = boardHeight;
		width = boardWidth;
		boardData = BoardData<T>{ height, { width, "." }};
		COST["."] = 1;
		COST["W"] = 2;
		COST["L"] = 5;
		COST["T"] = 1;
	}

	bool setStart(const unsigned int X, const unsigned int Y) {
		if (setPoint(X, Y, "S")) {
			Start.X = X;
			Start.Y = Y;
			return true;
		}
		return false;
	}

	const Position getStart() {
		return Start;
	}

	bool setEnd(const unsigned int X, const unsigned int Y) {
		if (setPoint(X, Y, "E")) {
			End.X = X;
			End.Y = Y;
			return true;
		}
		return false;
	}

	const Position getEnd() {
		return End;
	}

	//Utility function to add a point to the chessboard
	//Use this to only add Start, End or terrain points like W R B etc.
	//returns true if successful
	//returns false if not a valid point
	bool setPoint(const int X, const int Y, const std::string pointName) {
		if (isPointWithinBoundary(X, Y)) {
			boardData[Y][X] = pointName;
			return true;
		}
		return false;
	}

	//Function to maintain a vector of teleport pairs
	bool addTeleportPoints(const int X1, const int Y1, const int X2, const int Y2) {
		if (setPoint(X1, Y1, "T") && setPoint(X2, Y2, "T")) {
			listTeleport.push_back(std::make_pair(Position{ X1, Y1 }, Position{ X2, Y2 }));
			return true;
		}
		return false;
	}

	//Function to check if current point is a Teleport point
	//if yes, this updates the point to teleport location
	bool canTeleport(Position& p) {
		if (boardData[p.Y][p.X] == "T") {
			///Check if the point is a Teleport point. if true this point needs to be made the teleport exit
			for (auto t : listTeleport) {
				if (p == t.first) {  //If point same as any of the points in a teleport pair, move this point to other point.
					p = t.second;
					return true;
				}
				else if (p == t.second) {
					p = t.first;
					return true;
				}
			}
		}
		return false;
	}
	
	//Function to validate if a point is inside the board boundaries)
	//returns true if point is valid
	//Points wihtin the boundary can then be converted to unsigned ints
	bool isPointWithinBoundary(const int X, const int Y) {  //We want to make these int to check for negative numbers 
		return (X >= 0 && X < static_cast<int>(width) && Y >= 0 && Y < static_cast<int>(height));
	}

	//Function to check if point lies within board bounadaries
	//Also checks if destination is a Rock or Barrier...we can not go to these
	bool isValidMove(const int X, const int Y) {
		if (isPointWithinBoundary(X, Y)) {
			if (boardData[Y][X] == "R" || boardData[Y][X] == "B")
				return false;
			return true;
		}
		return false;
	}

	//Function to check if move is a valid Knight Move
	//Function will take 2 valid positions
	bool isValidKnightMove(Position& prevK, const int nextK_x, const int nextK_y) {
		if (isValidMove(nextK_x, nextK_y)) {
			Position nextK{ nextK_x, nextK_y };
			if (std::abs(static_cast<int>(prevK.X) - static_cast<int>(nextK.X)) * std::abs(static_cast<int>(prevK.Y) - static_cast<int>(nextK.Y)) == 2) {
				return true;
			}
		}
		return false;
	}

	//Function to check if we will hit the barrier when moving in a direction
	bool isBarrierInPath(Position K, const int dX, const int dY) {
		int newX = K.X + dX;
		int newY = K.Y + dY;
		if (isPointWithinBoundary(newX, newY)) //if it is not a valid move...return true so the isBarrier can flag it
			if (boardData[newY][newX] == "B")
				return true;
			else
				return false;
		return true;
	}

	//Function to get the Path from the Visited data struct
	//Needs a reference to a vector of Positions
	void getPathStart2End(BoardData<Move>& visited, std::vector<Position>& bestPath) {
		//Retrieve the best path for future use
		bestPath.clear();
		Position p{ getEnd() };
		bestPath.push_back(p);
		while (p != getStart()) {
			Position nextP = visited[p.Y][p.X].second;
			canTeleport(nextP);
			bestPath.push_back(nextP);
			p = nextP;
		}

		//Needs to be reversed because we are going from End to Start. We want path from S to E.
		std::reverse(bestPath.begin(), bestPath.end());
	}

	//Function to get a vector of valid moves
	//The knight can move in 4 directions and can move to 8 points
	//If there is a wall in any of the 4 directions we can not move to those
	//If a direction is open for moving (no wall), we then check if the point to move to is valid
	//Updates the list of possible moves from the current knigth position
	// int costHeuristic: 0 is naiive (no extra cost), 1 is A*(manhattan distance from end), 
	// 2 is opposite of A* (used to find longest path) 
	void getValidMoves(const Position K, Moves& possibleMoves, bool useDistanceHeuristic) { 
		for (size_t direction = 0; direction < 4; ++direction) {
			if (isBarrierInPath(K, Dx[direction], Dy[direction])) //If we hit the barrier in this direction, move on to next
				continue;
			else {
				//There are 2 ways to move for each direction
				for (size_t i = 0; i < 2; ++i) {
					int newX = K.X + Mx[(direction * 2) + i];
					int newY = K.Y + My[(direction * 2) + i];
					if (isValidMove(newX, newY)) { //Is valid Move should return cost of the move
						Position newP{ newX, newY };

						if (canTeleport(newP))
							std::cout << "Teleporting \n";
						
						int costOfMove = COST[boardData[newP.Y][newP.X]];
						//If A* algo...the cost also includes the distance from END
						if (useDistanceHeuristic)
							costOfMove += std::abs(static_cast<int>(newP.X) - static_cast<int>(getEnd().X)) + std::abs(static_cast<int>(newP.Y) - static_cast<int>(getEnd().Y));
						possibleMoves.push_back({ costOfMove , newP });
					}
				}
			}
		}
	}

	//Function to print the current state of the board
	//if moveKnight is false...only S and E will be printed.
	//if moveKnight is true...K will also be printed. If K after moving lies on S or E, they will be replaced by K.
	void printBoardState(bool printKnight, const Position K) {
		std::string curVal_K;
		if (printKnight) {
			//Store the current state at K location so we can revert back after the print
			//assign "K" to the K location
			curVal_K = boardData[K.Y][K.X];
			boardData[K.Y][K.X] = "K";
		}
		printBoardData(boardData);
		if (printKnight) { //revert
			boardData[K.Y][K.X] = curVal_K;
		}
	}

	//print board state only without the knight position
	void printBoardState() {
		printBoardData(boardData);
	}

private:
	Position Start{ 0,0 };
	Position End{ 0,0 };
};
