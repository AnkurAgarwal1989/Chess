#pragma once
#include<iostream>
#include<stdio.h>
#include<vector>
#include<utility>
#include<string>
#include<assert.h>
#include<csignal>
#include"Board.h"

void signalHandler(int signum) {
	std::cout << "\nUser pressed Ctrl+C. Exiting \n";
	exit(signum);
}


//Given a sequence, validate if it is a series of plausible Knight moves
bool validateSequence_manual(Board<std::string>& B, bool printKnightMoves) {
	signal(SIGINT, signalHandler);
	std::cout << "Please number 2 numbers (x, y) position of the Knight. Ctrl+C to exit.\n";
	int nextK_x, nextK_y;
	
	Position K_prev{B.getStart()};
	if (B.canTeleport(K_prev))
		std::cout << "Teleporting \n";
	while (1) {
		std::cin >> nextK_x >> nextK_y;
		
		if (B.isValidKnightMove(K_prev, nextK_x, nextK_y)) {
			Position K_next{ nextK_x, nextK_y };
			if (B.canTeleport(K_next))
				std::cout << "Teleporting \n";
			//Since we have made sure the points are valid, we can static cast to unsigned int
			K_prev = K_next;
			std::cout << "Knight moved to location " << K_next.X << ", " << K_next.Y << "\n";
			if (printKnightMoves)
				B.printBoardState(true, K_prev);
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}

		else {
			std::cout << "Not a valid move \n";
			std::cout << "The knight can not move from " << "(" << K_prev.X << "," << K_prev.Y << ") to (" << nextK_x << "," << nextK_y << ")\n";
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
}

bool validateSequence(Board<std::string>& B, const std::vector<Position>& testPath, bool printKnightMoves) {

	if (testPath.size() < 2) {
		std::cout << "Not enough moves\n";
		return false;
	}
	std::cout << "\nStart\n";
	printPath(testPath);
	std::cout << "End\n";
	
	std::vector<Position> path = testPath;
	Position K_prev = path[0];
	if (B.canTeleport(K_prev))
		std::cout << "This path has a Teleport\n";

	for (size_t i = 1; i < path.size(); ++i) {
		Position K_next = path[i];
		if (B.isValidKnightMove(K_prev, K_next.X, K_next.Y)) {
			if (B.canTeleport(K_next)) {
				std::cout << "This path has a Teleport\n";
			}
			K_prev = K_next;
			if (printKnightMoves) {
				std::cout << "\nMove # "<< i << "\n";
				B.printBoardState(true, K_prev);
			}
		}
		else {
			std::cout << "Move # " << i << " is not a valid move \n";
			std::cout << "Can not move from " << "(" << K_prev.X << "," << K_prev.Y << ") to (" << K_next.X << "," << K_next.Y << ")\n";
			return false;
		}
	}
	return true;
}

//Function to setup a board
//Input is a reference to a board. The board already has a height and width
//If a filename is provided function reads the file and creates a layout on the board
void setupBoard(Board<std::string>& KB, size_t Start_X, size_t Start_Y, size_t End_X, size_t End_Y, const std::string& filename = std::string()) {

	if (filename != std::string()) {
		KB.readGameFile(filename, ' ');
	}

	bool ret = KB.setStart(Start_X, Start_Y);
	ret &= KB.setEnd(End_X, End_Y);
	if (ret) {
		if (KB.getStart() == KB.getEnd()) {
			std::cout << "Starting and Ending points are the same. \n";
			exit(1);
		}
		std::cout << "Initial State of Knight Board \n";
		KB.printBoardState();
	}
	else {
		std::cout << "Start or End Point is OUTSIDE checkerboard area. Exiting";
		exit(1);
	}
}