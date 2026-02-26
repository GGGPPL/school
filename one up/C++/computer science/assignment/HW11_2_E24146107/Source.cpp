#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>

#include "E24146107.h"
#include "Poor_AI.h"
#define STUDENT 2
using namespace std;

bool isGameover(vector < vector<char> >, int, char);        //Examine if this round ends
bool sbWin(vector < vector<char> >, char);                  //Examine if anyone wins at last move
bool isFull(vector < vector<char> >);                       //Examine if the board is full
bool legalMove(vector < vector<char> >, int);               //Check if the last move is legal
vector < vector<char> > move(vector < vector<char> >, int, char); //Drop a new disc in the board
void output(vector < vector<char> >);                       //Show the board

int count_win1 = 0;   //Win count of player1
int count_win2 = 0;   //Win count of player2
int count_draw = 0;   //Win count of draw

bool skip_animation = false; //Skip all animations after a round ends

void promptSkip() { //Ask user to press Enter or 's' to skip
	cout << " [Enter] next game / [s] skip all animations: ";
	string choice;
	getline(cin, choice);
	if (choice == "s" || choice == "S") skip_animation = true;
}

int main() {
	const vector < vector<char> > board(6, vector<char>(7, '\0'));  //Store the board in 6x7 vector
	vector < vector<char> > p;                                      //Store the board temporarily
	int col;                                                        //Store the column AI determines
	char disc[2] = { 'O', 'X' };

	int(*op[STUDENT])(const vector < vector<char> > &map, char D1, char D2);
	op[0] = E24146107; //AI's function name
	op[1] = poorAI;

	int t = 0;

	for (int k = 0; k < 100; k++) { //Play 100 games
		//New Game Starts
		p = board;

		if (!skip_animation) {
			system("cls");
			cout << "---------------New Game---------------" << endl << endl;
		}

		//Player1: Student, Player2: Poor_AI
		//Player1's disc: O, Player2's disc: X
		if (k % 2 == 0) { //Player1 drops discs first
			t = 0;
		}
		else { //Player2 drops discs first
			t = 1;
		}

		while (true) {
			if (!skip_animation) {
				system("cls");
				cout << " << Player" << t + 1 << "'s turn >>\n" << endl;
			}

			col = op[t](p, disc[t], disc[!t]);

			if (isGameover(p, col, disc[t])) {
				break;
			}

			p = move(p, col, disc[t]);

			if (!skip_animation) {
				output(p);

				
				if (!sbWin(p, disc[t]) && !isFull(p)) {
					system("pause");
				}
			}


			t = !t;
		}

		//After a round ends, allow skip
		if (!skip_animation) {
			promptSkip();
		}
		else {
			if ((k + 1) % 10 == 0) {
				cout << "Processing: " << (k + 1) << "/100" << endl;
			}
		}
	}

	system("cls");
	cout << "Player1 wins " << count_win1 << " times" << endl;
	cout << "Player2 wins " << count_win2 << " times" << endl;
	cout << "Draw " << count_draw << " times" << endl;

	if (count_win1 > count_win2) {
		cout << "Player1 is the winner!!!" << endl;
	}
	else if (count_win1 == count_win2) {
		cout << "Both Player1 and Player2 are winners!!!" << endl;
	}
	else {
		cout << "Player2 is the winner!!!" << endl;
	}

	system("pause");
	return 0;
}

bool isGameover(vector < vector<char> > bb, int c, char d) {
	//Illegal move: out of range
	if (c < 0 || c > 6) {
		if (!skip_animation) {
			cout << "The column must between 0 to 6" << endl;
		}
		if (d == 'O') {
			if (!skip_animation) cout << "Player2 wins!" << endl;
			count_win2++;
		}
		else {
			if (!skip_animation) cout << "Player1 wins!" << endl;
			count_win1++;
		}
		return 1;
	}

	//Illegal move: column full
	if (!legalMove(bb, c)) {
		if (d == 'O') {
			if (!skip_animation) cout << "Player2 wins!" << endl;
			count_win2++;
		}
		else {
			if (!skip_animation) cout << "Player1 wins!" << endl;
			count_win1++;
		}
		return 1;
	}

	//Simulate the move to check win/draw
	bb = move(bb, c, d);

	//Someone wins
	if (sbWin(bb, d)) {
		if (!skip_animation) {
			output(bb);
			if (d == 'O') cout << "Player1 wins!" << endl;
			else cout << "Player2 wins!" << endl;
		}
		if (d == 'O') count_win1++;
		else count_win2++;
		return 1;
	}

	//Draw
	if (isFull(bb)) {
		if (!skip_animation) {
			output(bb);
			cout << "Draw Game!" << endl;
		}
		count_draw++;
		return 1;
	}

	return 0;
}

bool sbWin(vector < vector<char> > b1, char d1) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			if (b1[i][j] != d1) continue;

			if (j + 3 < 7) {
				if (b1[i][j + 1] == d1 && b1[i][j + 2] == d1 && b1[i][j + 3] == d1) return 1;
			}
			if (i + 3 < 6) {
				if (b1[i + 1][j] == d1 && b1[i + 2][j] == d1 && b1[i + 3][j] == d1) return 1;
			}
			if ((j + 3 < 7) && (i + 3 < 6)) {
				if (b1[i + 1][j + 1] == d1 && b1[i + 2][j + 2] == d1 && b1[i + 3][j + 3] == d1) return 1;
			}
			if ((j - 3 >= 0) && (i + 3 < 6)) {
				if (b1[i + 1][j - 1] == d1 && b1[i + 2][j - 2] == d1 && b1[i + 3][j - 3] == d1) return 1;
			}
		}
	}
	return 0;
}

bool isFull(vector < vector<char> > b2) {
	for (int j = 0; j < 7; j++) {
		if (b2[0][j] == '\0') return 0;
	}
	return 1;
}

bool legalMove(vector < vector<char> > o, int c1) {
	if (c1 < 0 || c1 > 6) return 0;
	if (o[0][c1] == '\0') {
		return 1;
	}
	else {
		if (!skip_animation) {
			cout << "This column is full" << endl;
		}
		return 0;
	}
}

vector < vector<char> > move(vector < vector<char> > b3, int c2, char d2) {
	for (int i = 5; i >= 0; i--) {
		if (b3[i][c2] == '\0') {
			b3[i][c2] = d2;
			break;
		}
	}
	return b3;
}

void output(vector < vector<char> > b4) {
	cout << "      0   1   2   3   4   5   6 " << endl;
	cout << "    +---+---+---+---+---+---+---+" << endl;
	for (int i = 0; i < 6; i++) {
		cout << "  " << i << " | ";
		for (int j = 0; j < 7; j++) {
			if (b4[i][j] == '\0') cout << "  | ";
			else cout << b4[i][j] << " | ";
		}
		cout << endl;
		cout << "    +---+---+---+---+---+---+---+" << endl;
	}
}
