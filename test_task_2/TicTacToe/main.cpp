#include "Controller.h"

void start(int num) {
	Controller controller;
	for (int i = 0; i < num; i++) {
		int winner = DRAW;
		controller.restart(i);
		do {
			winner = controller.turn();
			controller.swap();
		} while (!controller.game_over() && winner == DRAW);

		controller.write_result(i, winner);
	}
}

int main()
{
	srand(time(NULL));
	int num = 0;
	char c = 0;

	printf("Number of parties: ");
	while (scanf_s("%d", &num) != 1 || num <= 0) {
		system("cls");
		scanf_s("%*[^\n]");
		printf("Incorrect data, try again: ");
	}

	system("cls");
	printf("Press SPACE for start %d games\n\n", num);
	while (c = _getch() != ' ');

	start(num);

	printf("The series of games is over. You can see the results in the file /log.csv\n");
	printf("Press SPACE to quit the application");
	while (c = _getch() != ' ');
}

