#pragma once
#include<iostream>
#include<stdio.h>
#include<vector>
#include<utility>
#include<string>
#include<assert.h>
#include<csignal>
#include<io.h>
#include"Board.h"

void pathPlanning(Board<std::string>&, std::vector<Position>&, bool);
void longestPath(Board<std::string>&, std::vector<Position>&);
bool validateSequence_manual(Board<std::string>&, bool);
bool validateSequence(Board<std::string>&, const std::vector<Position>&, bool);
void setupBoard(Board<std::string>&, size_t, size_t, size_t, size_t, const std::string&);

void printUsage(std::string name) {
	std::cout << "Usage: " << name << "\n"
		<< "Options:\n"
		<< "\t-h,--help\t\tShow this help message\n\n"
		<< "\tLevel[1-5] [Height] [Width] DisplayBoard[T/F] [Start_X] [Start_Y] [End_X] [End_Y] [Full path to layout file (optional)]\n\n"
		<< "\tSample Usage: To run program for Level 2 with an 8x8 board. Starting at (0,0), Ending at (7,7) and displaying board state\n"
		<< "\t" << name<< " 2 8 8 T 0 0 7 7\n"
		<< std::endl;
}

//Task1: Sequence validation

int main(int argc, char* argv[]) {
	
	size_t L = 0;
	size_t BOARD_HEIGHT = 8;
	size_t BOARD_WIDTH = 8;
	bool printKnightMoves = false;
	size_t Start_X = 0;
	size_t Start_Y = 0;
	size_t End_X = 7;
	size_t End_Y = 7;
	std::string layoutFfilename = "";
	
	if (argc < 2) {
		printUsage(argv[0]);
		return 1;
	}

	if (argc > 2 && argc < 9) {
		std::cout << "Too few arguments \n";
		printUsage(argv[0]);
		return 1;
	}

	if (argc >= 9) {
		L = std::abs(atoi(argv[1]));
		BOARD_HEIGHT = std::abs(atoi(argv[2]));
		BOARD_WIDTH = std::abs(atoi(argv[3]));
		if (*argv[4] == 'T' || *argv[4] == 't')
			printKnightMoves = true;

		Start_X = std::abs(atoi(argv[5]));
		Start_Y = std::abs(atoi(argv[6]));
		End_X = std::abs(atoi(argv[7]));
		End_Y = std::abs(atoi(argv[8]));
		if (argc > 9) { //read the provided filename
			layoutFfilename = argv[9];
		}
	}

	if (BOARD_HEIGHT <= 0 || BOARD_WIDTH <= 0) {
		std::cout << "Board dimensions can not be 0. Exiting. \n";
		exit(1);
	}

	//Set up a board with legit dimensions
	Board<std::string> KB{ BOARD_HEIGHT, BOARD_WIDTH };
	std::vector<Position> path;
	bool findShortestPath = true;

	setupBoard(KB, Start_X, Start_Y, End_X, End_Y, layoutFfilename);
	std::cout << "1.Initial state of Knight Board \n";
	KB.printBoardState();

	switch (L)
	{
	case 1:
		validateSequence_manual(KB, true);
		break;

	case 2:
		pathPlanning(KB, path, !findShortestPath);
		validateSequence(KB, path, printKnightMoves);
		break;

	case 3:
		pathPlanning(KB, path, findShortestPath);
		validateSequence(KB, path, printKnightMoves);
		break;

	case 4:
		pathPlanning(KB, path, findShortestPath);
		validateSequence(KB, path, printKnightMoves);
		break;

	case 5:
		longestPath(KB, path);
		validateSequence(KB, path, printKnightMoves);
		break;

	default:
		printUsage(argv[0]);
		break;
	}

	std::cout << "\nDone \n";
	std::cin.get();
	return 1;
}
