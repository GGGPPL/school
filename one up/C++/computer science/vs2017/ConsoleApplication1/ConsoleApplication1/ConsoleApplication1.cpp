#include <iostream>
#include <vector>
#include <string.h>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <stdio.h>
#include <map>

#define fir(d) for (int i = 0; i < d; i++)
#define fjr(d) for (int j = 0; j < d; j++)
#define fkr(d) for (int k = 0; k < d; k++)
#define fr(a, b) for (int a = 0; a < b; a++)
#define debug(d) //cerr << "Line " << __LINE__ << " " << #d << ": " << d << endl
#define debugtext(d) // cerr << "Line " << __LINE__ << d << endl
#define cout cout << "Line " << __LINE__ << " "

using namespace std;

char p[100];
bool AreaFull[5];
bool AreaExist[5];

char* string2char(string str)
{

	fir(str.length())
	{
		p[i] = str[i];
	}
	p[str.length()] = '\0';
	return p;
}

struct card
{
	string type;
	string color;
};

struct Table
{
	vector<card> AreaCard[5];
	vector<card> SupplyDeck;
} GamingTable;

bool a = 0;

int scoreFromCount(int count)
{
	if (count <= 0) return 0;
	if (count == 1) return 1;
	if (count == 2) return 3;
	if (count == 3) return 6;
	if (count == 4) return 10;
	if (count == 5) return 15;
	return 21; // more than 6
}


int CalculateScore(const vector<card>& PlayerDeck)
{
	map<string, int> colorCounts;
	int jokerCount = 0;
	int plusTwoCount = 0;

	// Tally all cards
	for (const auto& c : PlayerDeck)
	{
		if (c.color == "joker")
		{
			jokerCount++;
		}
		else if (c.color == "+2")
		{
			plusTwoCount++;
		}
		else if (c.type == "coloretto") // Ensure we only count coloretto color cards
		{
			colorCounts[c.color]++;
		}
	}

	// Handle Jokers: Add them to the most numerous color stack
	if (jokerCount > 0 && !colorCounts.empty())
	{
		// Find the element in the map with the largest "value" (count)
		auto maxColorIt = max_element(colorCounts.begin(), colorCounts.end(),
			[](const pair<string, int>& a, const pair<string, int>& b) {
			return a.second < b.second; // Compare the "value" (the int count)
		});

		// Add all Jokers to that color
		maxColorIt->second += jokerCount;
	}
	// If a player has no color cards, Jokers count for 0 points (colorCounts is empty)


	// Convert card "counts" into "scores" and store them in a vector
	vector<int> finalScores;
	for (const auto& pair : colorCounts)
	{
		finalScores.push_back(scoreFromCount(pair.second));
	}

	// Sort the scores (from high to low)
	// rbegin() and rend() stand for "reverse begin/end", 
	// which easily achieves descending order sorting.
	sort(finalScores.rbegin(), finalScores.rend());

	// Calculate the total score
	int totalScore = 0;

	// Add points from +2 cards
	totalScore += (plusTwoCount * 2);

	// Add positive scores (top 3) and subtract negative scores (all others)
	for (size_t i = 0; i < finalScores.size(); ++i)
	{
		if (i < 3)
		{
			totalScore += finalScores[i]; // Positive score
		}
		else
		{
			totalScore -= finalScores[i]; // Negative score
		}
	}

	return totalScore;
}

void ShowTable(Table tab, int player_num)
{
	fir(player_num)
	{
		if (tab.AreaCard[i].size() == 0 && AreaExist[i] == true)
		{
			cout << "[" << i + 1 << "]: Empty!" << endl;
		}
		else if (tab.AreaCard[i].size() == 0 && AreaExist[i] == false)
		{
			cout << "[" << i + 1 << "]: Taken!" << endl;
		}
		else
		{
			cout << "[" << i + 1 << "]: ";
			for (int j = 0; j < tab.AreaCard[i].size(); j++)
			{
				cout << "[" << string2char(tab.AreaCard[i][j].color) << "] ";
			}
			cout << endl;
		}
	}
}

int main()
{
	srand(time(NULL));

	// number of players
	int player_num;

	memset(AreaFull, false, sizeof(AreaFull));
	memset(AreaExist, true, sizeof(AreaExist));

	// make cards

	// 76 coloretto cards
	card coloretto[76];
	fir(76)
	{
		coloretto[i].type = "coloretto";
	}

	// 9 cards each in 7 colors
	// orange, yellow, green, brown, blue, purple, red
	fjr(9)
		coloretto[j]
		.color = "orange";
	fjr(9)
		coloretto[j + 9]
		.color = "yellow";
	fjr(9)
		coloretto[j + 18]
		.color = "green";
	fjr(9)
		coloretto[j + 27]
		.color = "brown";
	fjr(9)
		coloretto[j + 36]
		.color = "blue";
	fjr(9)
		coloretto[j + 45]
		.color = "purple";
	fjr(9)
		coloretto[j + 54]
		.color = "red";

	// 3 joker
	coloretto[63].color = "joker";
	coloretto[64].color = "joker";
	coloretto[65].color = "joker";

	// 10 +2 cards
	fir(10)
		coloretto[66 + i]
		.color = "+2";

	// 5 area cards
	card area[5];
	fir(5)
	{
		area[i].type = "area";
	}

	// 5 summary cards
	card summary[5];
	fir(5)
	{
		summary[i].type = "summary";
	}

	// 1 lastround card
	card last_round;
	last_round.type = "last_round";
	last_round.color = "last_round";

	// number of players
	cout << "number of players(3~5): ";
	cin >> player_num;

	// setting table

	fir(sizeof(coloretto) / sizeof(coloretto[0]))
	{
		GamingTable.SupplyDeck.push_back(coloretto[i]);
	}

	// 3 players
	if (player_num == 3)
	{

		// remove one color of coloretto card
		int erase_index = rand() % 7;
		cout << "remove color: " << string2char(GamingTable.SupplyDeck[erase_index * 9].color) << endl;
		GamingTable.SupplyDeck.erase(GamingTable.SupplyDeck.begin() + erase_index * 9, GamingTable.SupplyDeck.begin() + erase_index * 9 + 9);
	}

	fir(GamingTable.SupplyDeck.size())
	{
		debug(GamingTable.SupplyDeck[i].color);
	}
	cout << endl;

	// get each player one summary card
	struct PlayerArea
	{
		vector<card> PlayerDeck;
		vector<card> SummaryCardDeck;
	} Player[5];
	fir(player_num)
	{
		Player[i].SummaryCardDeck.push_back(summary[i]);
	}

	// get each player one coloretteo card
	// not joker or +2 and each coloretto cannot be the SAME COLOR
	fir(player_num)
	{
		bool WrongCard;
		do
		{
			WrongCard = false;
			int index = rand() % GamingTable.SupplyDeck.size();
			if (GamingTable.SupplyDeck[index].color != "joker" && GamingTable.SupplyDeck[index].color != "+2")
			{
				fjr(i)
				{
					debug(Player[j].PlayerDeck[0].color);
					debug(GamingTable.SupplyDeck[index].color);
					if (Player[j].PlayerDeck[0].color == GamingTable.SupplyDeck[index].color)
					{
						WrongCard = true;
						break;
					}
				}
				if (WrongCard == false)
				{
					debug(GamingTable.SupplyDeck[index].color);
					Player[i].PlayerDeck.push_back(GamingTable.SupplyDeck[index]);
					GamingTable.SupplyDeck.erase(GamingTable.SupplyDeck.begin() + index);
				}
			}
			else
			{
				WrongCard = true;
			}
		} while (WrongCard);
	}
	fir(player_num)
	{
		debug(Player[i].PlayerDeck[0].color);
	}
	cout << endl;

	// shuffle every coloretto card
	fir(100)
	{
		fjr(GamingTable.SupplyDeck.size())
		{
			int index1 = rand() % GamingTable.SupplyDeck.size();
			int index2 = rand() % GamingTable.SupplyDeck.size();
			card temp = GamingTable.SupplyDeck[index1];
			GamingTable.SupplyDeck[index1] = GamingTable.SupplyDeck[index2];
			GamingTable.SupplyDeck[index2] = temp;
		}
	}
	fir(GamingTable.SupplyDeck.size())
	{
		debug(GamingTable.SupplyDeck[i].color);
	}
	cout << endl;

	// put last round card at the 16th from the buttom
	GamingTable.SupplyDeck.insert(GamingTable.SupplyDeck.begin() + 15, last_round);
	debug(GamingTable.SupplyDeck[15].type);

	// place area card as many as the number of players
	// already on the table

	// start from a random player, clockwise
	bool GameOver = false;
	int CurrentPlayer = rand() % player_num;

	ShowTable(GamingTable, player_num);

	while (GameOver == false)
	{

		memset(AreaFull, false, sizeof(AreaFull));
		memset(AreaExist, true, sizeof(AreaExist));
		bool TableFull = false;
		bool RoundOver = false;
		bool LastRound = false;
		bool SkipList[5];
		memset(SkipList, false, sizeof(SkipList));

		while (RoundOver == false)
		{
			cout << endl;
			if (CurrentPlayer == 0)
			{
				cout << "Your turn!!!" << endl;
			}
			else
			{
				cout << "Player " << CurrentPlayer + 1 << "'s turn!!!" << endl;
			}

			// player decides what to do

			cout << "Do something (\"0\" to draw, \"1\" to collect)" << endl;
			int UserInput;

			if (CurrentPlayer == 0)
			{
				cin >> UserInput;
			}
			else
			{
				UserInput = rand() % 2;
				cout << UserInput << endl;
			}

			debug(UserInput);

			fjr(player_num) // whether there is space for more cards
			{
				TableFull = true;
				if (AreaFull[j] == false && AreaExist[j] == true)
				{
					TableFull = false;
					break;
				}
			}
			if (TableFull == true)
			{
				cout << "table is full, please collect an area" << endl;
			}

			// draw card
			if (UserInput == 0 && TableFull == false)
			{
				if (GamingTable.SupplyDeck[GamingTable.SupplyDeck.size() - 1].color == "last_round")
				{
					LastRound = true;
					cout << "Last Round!!" << endl;
					GameOver = true;
					GamingTable.SupplyDeck.pop_back();
				}

				cout << "draw card" << endl;
				cout << "[" << string2char(GamingTable.SupplyDeck[GamingTable.SupplyDeck.size() - 1].color) << "]" << endl;
				int TargetAreaCard;
				do
				{

					cout << "target area card: "; // where to put the card
					if (CurrentPlayer == 0)
					{
						cin >> TargetAreaCard;
					}
					else
					{
						TargetAreaCard = rand() % player_num + 1;
						cout << TargetAreaCard << endl;

					}
					TargetAreaCard -= 1;
					if (AreaFull[TargetAreaCard] == true) // the areadeck is full
					{
						cout << "area deck is full" << endl;
					}
					if (AreaExist[TargetAreaCard] == false) // the areadeck is taken away
					{
						cout << "area deck has been taken away!" << endl;
					}
				} while (AreaExist[TargetAreaCard] == false || AreaFull[TargetAreaCard] == true); // choose another deck

				GamingTable.AreaCard[TargetAreaCard].push_back(GamingTable.SupplyDeck[GamingTable.SupplyDeck.size() - 1]);
				GamingTable.SupplyDeck.pop_back();

				if (GamingTable.AreaCard[TargetAreaCard].size() >= 3)
				{
					AreaFull[TargetAreaCard] = true;
				}
				ShowTable(GamingTable, player_num);
			}

			// collect cards
			if (UserInput == 1)
			{
				SkipList[CurrentPlayer] = true; // can no longer do anything until next round
				cout << "collect cards" << endl;
				int TargetAreaCard;

				do
				{
					cout << "target area card: "; // which area to collect
					if (CurrentPlayer == 0)
					{
						cin >> TargetAreaCard;
					}
					else
					{
						TargetAreaCard = rand() % player_num + 1;
						cout << TargetAreaCard << endl;

					}
					TargetAreaCard -= 1;
					if (AreaExist[TargetAreaCard] == false)
					{
						cout << "area deck is already collected!" << endl;
					}
					else
					{
						break;
					}
				} while (AreaExist[TargetAreaCard] == false);

				// merge area card to player's deck
				Player[CurrentPlayer].PlayerDeck.insert(Player[CurrentPlayer].PlayerDeck.end(), GamingTable.AreaCard[TargetAreaCard].begin(), GamingTable.AreaCard[TargetAreaCard].end());
				GamingTable.AreaCard[TargetAreaCard].clear();
				AreaExist[TargetAreaCard] = false;

				ShowTable(GamingTable, player_num);
			}

			fjr(player_num) // Is all area deck collected?
			{
				RoundOver = true;
				if (AreaExist[j] == true)
				{
					RoundOver = false;
					break;
				}
			}

			if (!RoundOver)
			{
				do
				{
					CurrentPlayer = (CurrentPlayer + 1) % player_num;
				} while (SkipList[CurrentPlayer] == true);
			}
			else
			{
				cout << "Round over" << endl;
			}
		}
	}

	// Game Over
	// Calculate Score
	int Score[5];
	fir(player_num)
	{
		Score[i] = CalculateScore(Player[CurrentPlayer].PlayerDeck);
	}
	fir(player_num)
	{
		cout << "Player " << i + 1 << " score: " << Score[i] << endl;
	}

	return 0;
}
