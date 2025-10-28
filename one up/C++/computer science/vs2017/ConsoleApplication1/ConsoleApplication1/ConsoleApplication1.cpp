#include<windows.h>
#include<iostream>
#include<time.h>
#include<cstdlib>
#include<string>

#define fr(d) for(int i=0; i<d; i++)

using namespace std;

void sleep(int seconds)
{
	clock_t start = clock();
	clock_t lay = (clock_t)seconds*CLOCKS_PER_SEC;
	while (clock() - start < lay);
}

void SetColor(int color = 7)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void ColoredText(int num = 0, int color = 7)
{
	SetColor(color);
	cout << num;
	SetColor();
}

void ClearScreen()
{
	for (int i = 0; i < 50; i++)
	{
		cout << endl;
	}
}

int UserInputting()
{
	int UserInput;
	int co_UserInput;
	while (1)
	{
		cin >> UserInput;

		co_UserInput = UserInput;

		int CheckList[10];
		bool InputError = false;

		memset(CheckList, 0, sizeof(CheckList));
		for (int i = 0; i < 4; i++)
		{
			if (co_UserInput % 10 == 0)
			{
				InputError = true;
				break;
			}

			if (CheckList[co_UserInput % 10] == 1)
			{
				InputError = true;
				break;
			}
			else
			{
				CheckList[co_UserInput % 10] = 1;
			}
			co_UserInput /= 10;
		}

		if (!InputError)
		{
			//cerr << "Input Pass" << endl;
			break;
		}
		else
		{
			//cerr << "Input Error" << endl;
			cout << "Input Error" << endl;
		}

	}
	return UserInput;
}

int main() //----------------------------main code start from here---------------------------------------
{

	cout << "guess a 4 digits number, each digit cannot be the same(no 0)" << endl;
	sleep(1);

	SetColor(160);
	cout << "green";
	SetColor();
	cout << " means correct number and position";
	sleep(1);

	cout << endl;
	SetColor(224);
	cout << "yellow";
	SetColor();
	cout << " means correct number but wrong position";
	sleep(1);

	cout << endl;
	SetColor(128);
	cout << "gray";
	SetColor();
	cout << " means wrong number";
	sleep(1);

	cout << "\nready and guess!!!" << endl;

	srand(time(NULL));

	//create a random number and each of the digit is not the same
	int digit_source[9];
	for (int i = 0; i < 9; i++) //a 1~9 array
	{
		digit_source[i] = i + 1;
	}

	int TheNumber = 0;

	for (int i = 0; i < 4; i++) //take the four digits from an array so there will not be two same digits
	{
		int index = rand() % 9;
		if (digit_source[index] != 0)
		{
			TheNumber = TheNumber * 10 + digit_source[index];
			digit_source[index] = 0;
		}
		else
		{
			i--;
		}
	}
	//TheNumber = 1942;
	//cerr << TheNumber << endl;


	//user input and examine the user input and output the "hint" until user guess the number
	int UserInput;
	bool success = false;
	while (!success) //loop until user guess the number
	{
		UserInput = UserInputting();
		//cerr << UserInput << endl;

		int UserDigit[4], AnswerDigit[4];
		int co_UserInput = UserInput, co_TheNumber = TheNumber;//ERROR HERE------------------------------
		fr(4)
		{
			UserDigit[i] = co_UserInput % 10;
			AnswerDigit[i] = co_TheNumber % 10;
			co_UserInput /= 10;
			co_TheNumber /= 10;
		}

		int ColorHint[4]; //0 = gray, 1 = yellow, 2 = green
		memset(ColorHint, 0, sizeof(ColorHint)); //green = A, yellow = B, gray = not in the number
		//first check the correct number and the correct position(Green)
		for (int i = 0; i < 4; i++)
		{
			if (UserDigit[i] == AnswerDigit[i])
			{
				ColorHint[i] = 2;
			}
		}

		//second check the correct number but not the correct position(Yellow)
		fr(4)
		{
			if (ColorHint[i] == 0)
			{
				for (int j = 0; j < 4; j++)
				{
					if (UserDigit[i] == AnswerDigit[j])
					{
						ColorHint[i] = 1;
						break;
					}
				}
			}
		}

		//output the hint
		cout << endl << endl;

		for (int i = 3; i >= 0; i--)
		{
			if (ColorHint[i] == 0)
			{

				ColoredText(UserDigit[i], 128);
			}
			else if (ColorHint[i] == 1)
			{
				ColoredText(UserDigit[i], 224);
			}
			else if (ColorHint[i] == 2)
			{
				//cout << " " << i << ": ";
				ColoredText(UserDigit[i], 160);
			}
		}
		cout << endl;

		//check if success
		int a = 0;
		fr(4)
		{
			a += ColorHint[i];
		}
		if (a == 8)
		{
			success = true;
		}
	}

	cout << "congratulations!!" << endl;


	return 0;
}