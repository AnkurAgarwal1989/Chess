#pragma once
#include<iostream>
#include<stdio.h>
#include<vector>
#include<utility>
#include<string>
#include<assert.h>
#include<csignal>
#include"Board.h"


void task2();
void task5();

int test1() {
	unsigned int BOARD_HEIGHT = 7;  // Number of ROWS on baord
	unsigned int BOARD_WIDTH = 7;	// Number of COLUMNS on board
	bool ret;
	Board<std::string> KB{ BOARD_HEIGHT, BOARD_WIDTH };

	std::cout << "Test1: \n";
	ret = KB.setPoint(2, 2, "S");
	ret &= KB.setPoint(6, 4, "E");
	if (ret){
		KB.printBoardState();
	}
	else {
		std::cout << "Start point OUTSIDE checkerboard area";
	}
	std::cout << "\n";
	return ret;
}

int test2() {
	unsigned int BOARD_HEIGHT = 7;  // Number of ROWS on baord
	unsigned int BOARD_WIDTH = 7;	// Number of COLUMNS on board
	bool ret;
	Board<std::string> KB{ BOARD_HEIGHT, BOARD_WIDTH };
	std::cout << "Test2: \n";
	ret = KB.setPoint(9, 2, "S");
	ret &= KB.setPoint(6, 4, "E");
	if (ret) {
		KB.printBoardState();
	}
	else {
		std::cout << "Start point OUTSIDE checkerboard area";
	}
	return ret;
}

bool test3() {
	unsigned int BOARD_HEIGHT = 7;  // Number of ROWS on baord
	unsigned int BOARD_WIDTH = 7;	// Number of COLUMNS on board
	bool ret = true;

	Board<std::string> KB{ BOARD_HEIGHT, BOARD_WIDTH };
	std::cout << "Test3: \n";
	
	std::vector<int> Kseq{ 2,6, 10,2, 3,4, 5,6 };
	for (unsigned int i = 0; i < Kseq.size() - 1; i+=2) {
		if (KB.isValidMove(Kseq[i], Kseq[i+1])) {
			//Since we have made sure the points are valid, we can static cast to unsigned int
			Position KP{ static_cast<unsigned int>(Kseq[i]), static_cast<unsigned int>(Kseq[i + 1])}; //This maintains curent position of the knight on the board
			KB.printBoardState(true, KP);
			ret &= true;
		}
		else {
			std::cout << "The knight can not be placed at the location: (" << Kseq[i] << ", " << Kseq[i+1] << ")\n";
			ret &= false;
		}
	}
	
	if (!ret)
		std::cout << "Sequence does not contain valid knight moves";
	return ret;
}

void runTests() {

	assert(test1());
	assert(!test2());
	assert(test3());
}

void printHelp(){
	std::cout << "Task1 Usage [Board Height] [Board Width] [Start_X] [Start_Y] [End_X] [End_Y]\n";
	std::cout << "Required Arguments: Board Height Board Width  Start_X  Start_Y End_X End_Y \n";
	std::cout << "Optional Argument (after required): full path to file containing CSV knight positions \n";
}

void signalHandler(int signum) {
	std::cout << "\nUser pressed Ctrl+C. Exiting \n";
	exit(signum);
}


//Given a sequence, validate if it is a series of plausible Knight moves
bool validateSequence(Board<std::string>& B) {
	signal(SIGINT, signalHandler);
	std::cout << "Please number 2 numbers (x, y) position of the Knight. Ctrl+C to exit.\n";
	int nextK_x, nextK_y;
	
	Position K_prev{B.getStart()};
	if (B.canTeleport(K_prev))
		std::cout << "Teleporting /n";
	while (1) {
		std::cin >> nextK_x >> nextK_y;
		
		if (B.isValidKnightMove(K_prev, nextK_x, nextK_y)) {
			Position K_next{ nextK_x, nextK_y };
			if (B.canTeleport(K_next))
				std::cout << "Teleporting /n";
			//Since we have made sure the points are valid, we can static cast to unsigned int
			K_prev = K_next;
			std::cout << "Knight moved to location " << K_next.X << ", " << K_next.Y << "\n";
			B.printBoardState(true, K_prev);
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}

		else {
			std::cout << "The knight can not be placed at the location: (" << nextK_x << ", " << nextK_y << ")\n";
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');
		}
	}
}

int main(int argc, char* argv[]) {

	task5();
	task2();
	/*
	if (argc < 2) {
		printHelp();
	}

	if (argc > 2 && argc < 7) {
		std::cout << "Too few arguments \n";
		printHelp();
	}

	if (argc == 7) {
		size_t BOARD_HEIGHT = atoi(argv[1]);
		size_t BOARD_WIDTH = atoi(argv[2]);
		size_t Start_X = atoi(argv[3]);
		size_t Start_Y = atoi(argv[4]);
		size_t End_X = atoi(argv[5]);
		size_t End_Y = atoi(argv[6]);
		Board<std::string> KB{ BOARD_HEIGHT, BOARD_WIDTH };
		bool ret = KB.setStart(Start_X, Start_Y);
		ret &= KB.setEnd(End_X, End_X);
		if (ret) {
			std::cout << "Initial State of Knight Board \n";
			KB.addTeleportPoints(4, 3, 5, 5);
			KB.printBoardState();
			validateSequence(KB);
		}
		else {
			std::cout << "Start or End Point is OUTSIDE checkerboard area. Exiting";
			exit(1);
		}
	}
	*/
	std::cin.get();


	return 1;
}
