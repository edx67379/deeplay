#include "Controller.h"

int Controller::check_priority(int pos) {
	bool accept_trio = false;
	int result = 0;
	int pos_num = -1;
	for (int i = 0; i < 6; i++) {
		int priority = 0;
		for (int k = 0; k < 3; k++) {
			pos_num = trios[i][k];
			if (pos_num == pos) accept_trio = true;
			else if (field[pos_num / 3][pos_num % 3] == CROSS + NOUGHT - current_turn_symbol) {
				priority += 10;
				if (k % 2 == 0) priority += 10;
			}
		}
		if (accept_trio) result += priority;
		accept_trio = false;
	}
	return result;
}

bool Controller::check_win(char symbol) {
	int result = 0;
	int pos_num = -1;
	for (int i = 0; i < 8; i++) {
		for (int k = 0; k < 3; k++) {
			pos_num = trios[i][k];
			if (field[pos_num / 3][pos_num % 3] == symbol) {
				result++;
			}
		}
		if (result == 3)
			return true;
		result = 0;
	}
	return false;
}

bool Controller::check_win_or_hinder(int& pos, char symbol) {
	int result = 0;
	bool have_empty_slot = false;
	int pos_num = -1;
	for (int i = 0; i < 8; i++) {
		for (int k = 0; k < 3; k++) {
			pos_num = trios[i][k];
			if (field[pos_num / 3][pos_num % 3] == symbol) {
				result++;
			}
			else if (field[pos_num / 3][pos_num % 3] == ' ') {
				pos = pos_num;
				have_empty_slot = true;
			}
		}
		if (result == 2 && have_empty_slot)
			return true;
		result = 0;
		have_empty_slot = false;

	}
	return false;
}

bool Controller::check_can_win(int& pos) {
	return check_win_or_hinder(pos, current_turn_symbol);
}

bool Controller::check_danger(int& pos) {
	return check_win_or_hinder(pos, CROSS + NOUGHT - current_turn_symbol);
}

bool Controller::check_center(int& pos) {
	if (field[1][1] == ' ') {
		pos = 4;
		return true;
	}
	pos = -1;
	return false;
}

bool Controller::check_corner(int& pos) {
	int priority = -1;
	bool result = false;
	for (int i = 0; i < 5; i++) {
		if (i == 2) continue;
		if (field[(i * 2) / 3][(i * 2) % 3] == ' ') {
			int corner_priority = check_priority(i * 2);
			if (corner_priority >= priority) {
				pos = i * 2;
				priority = corner_priority;
			}
			if (priority == 40) {
				return false;
			}
			result = true;
		}
	}
	return result;
}

void Controller::check_other(int& pos) {
	for (int i = 1; i <= 7; i += 2) {
		if (field[i / 3][i % 3] == ' ') {
			pos = i;
			break;
		}
	}
}

int Controller::advanced_logic() {
	int pos = -1;
	bool check = check_can_win(pos) || check_danger(pos) || check_center(pos) || check_corner(pos);
	if (!check) check_other(pos);

	return pos;
}

int Controller::random_turn() {
	int pos = rand() % empty_slots.size();
	int elem = empty_slots[pos];
	field[elem / 3][elem % 3] = current_turn_symbol;
	empty_slots.erase(empty_slots.begin() + pos);
	return check_win(current_turn_symbol) ? RANDOM_AI : DRAW;
}

int Controller::advanced_turn() {
	int pos = advanced_logic();
	field[pos / 3][pos % 3] = current_turn_symbol;
	int vec_size = empty_slots.size();
	for (int i = 0; i < vec_size; i++) {
		if (empty_slots[i] == pos) {
			empty_slots.erase(empty_slots.begin() + i);
			break;
		}
	}
	return check_win(current_turn_symbol) ? ADVANCED_AI : DRAW;
}

void Controller::print_csv() {
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			char c = (field[i][k] == ' ') ? '%' : field[i][k];
			stream << c << ';';
		}
		stream << '\n';
	}
	stream << '\n';
}

Controller::Controller() {
	for (int i = 0; i < 9; i++) {
		trios[i / 3][i % 3] = i; // 012, 345, 678
		trios[i % 3 + 3][i / 3] = i;// 036, 147, 258
	}

	for (int i = 0; i < 6; i++) {
		int z = (i * 4) * ((5 - i) / 3) + ((i - 2) * 2) * (i / 3);
		trios[i % 2 + 6][i % 3] = z; // 048, 246
	}
	stream.open("log0.csv");
}

int Controller::turn() {
	if (current_turn_player == RANDOM_AI) {
		return random_turn();
	}
	else {
		return advanced_turn();
	}
}

bool Controller::game_over() {
	return empty_slots.empty();
}

void Controller::swap() {
	current_turn_player = (current_turn_player + 1) % 2;
	current_turn_symbol = CROSS + NOUGHT - current_turn_symbol;
	print_csv();
}

void Controller::restart(int i) {
	current_turn_symbol = CROSS;
	current_turn_player = i % 2 == 0 ? RANDOM_AI : ADVANCED_AI;

	if (i % 10000 == 0) {
		stream.close();
		stream.open("log" + std::to_string(i / 10000) + ".csv");
	}

	stream << i << ")--------------------------------" << '\n';
	if (current_turn_player == 0)
		stream << "Random AI starts - X, Advanced AI - O";
	else
		stream << "Advanced AI starts - X, Random AI - O";
	stream << '\n' << '\n';

	empty_slots.clear();
	empty_slots.reserve(10);
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			field[i][k] = ' ';
			empty_slots.push_back(i * 3 + k);
		}
	}
}

void Controller::write_result(int iteration, int winner) {
	std::string str = (winner == -1) ? "The match is drawn" : (winner == 0) ? "Random AI win" : "Advanced AI win";
	stream << str << '\n' << '\n';
}

Controller::~Controller() {
	stream.close();
}