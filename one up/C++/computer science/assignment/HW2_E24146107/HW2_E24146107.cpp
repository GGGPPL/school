//#include <bits/stdc++.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <iostream>

#define printfln(d) \
    printf(d);      \
    printf("\n")

using namespace std;

void printnamecard()
{
    printfln("=======================================");
    printfln("|   Name: WANG, YI-CHUAN              |");
    printfln("|   Birthday: 1989/06/04              |");
    printfln("|   Email: E24146107@gs.ncku.edu.tw   |");
    printfln("|   Office: 404                       |");
    printfln("=======================================");
}

void ClickMouse(long x, long y)
{
    INPUT Inputs[5] = {0};

    Inputs[0].type = INPUT_MOUSE;
    Inputs[0].mi.dx = x; // desired X coordinate
    Inputs[0].mi.dy = y; // desired Y coordinate
    Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    Inputs[1].type = INPUT_MOUSE;
    Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    Inputs[2].type = INPUT_MOUSE;
    Inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    Inputs[3].type = INPUT_MOUSE;
    Inputs[3].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    Inputs[4].type = INPUT_MOUSE;
    Inputs[4].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(5, Inputs, sizeof(INPUT));
}

int main()  //--------------------------------------------主程式開始
{
    long num;

    srand(time(NULL));

    printf("Please enter a number from 0 to 5(NO OTHER INPUT ALLOWED!!!): ");
    //scanf("%ld", &num);
    std::cin >> num;
    //使用者輸入


    if (!num)                          //是0的情況
    {
        printnamecard();
    }
    else if (num <= 5)                 //1~5的情況
    {
        for (long i = 0; i < num; i++) //印星星
        {
            printf("*");
        }
        printf("\n");
    }
    else                               //輸入在允許範圍之外的情況
    {
        int a = num * 10;              //告知使用者有誤
        int max = 0;
        int b;

        if(a<0) a = -a;
        while (a--)
        {
            ClickMouse(rand()*600000/32768, rand()*700000/32768);
            printf("Only 0~5 allowed.\n");
            for (long i = 0; i < a; i++)
            {
                printf("*");
            }
            printf("\n");
        }
    }

    return 0;
}
