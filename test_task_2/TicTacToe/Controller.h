#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <string>
#include <set>

const char CROSS = 'x';
const char NOUGHT = 'o';
const int DRAW = -1;
const int RANDOM_AI = 0;
const int ADVANCED_AI = 1;

class Controller {
	char field[3][3];
	int trios[8][3];
	char current_turn_symbol;
	int current_turn_player;
	std::vector<int> empty_slots;
	std::ofstream stream;

	int check_priority(int pos);
	bool check_win(char symbol);
	bool check_win_or_hinder(int& pos, char symbol);
	bool check_can_win(int& pos);
	bool check_danger(int& pos);
	bool check_center(int& pos);
	bool check_corner(int& pos);
	void check_other(int& pos);
	int advanced_logic();
	int random_turn();
	int advanced_turn();
	void print_csv();
public:
	Controller();
	int turn();
	bool game_over();
	void swap();
	void restart(int i);
	void write_result(int iteration, int winner);
	~Controller();
};

#endif CONTROLLER_H