#include <iostream>
#include <string.h>
#include <vector>
#include <random>       
#include <chrono>       
#include <limits>       
#include <iomanip>      

#define fir(d) for (int i = 0; i < d; i++)
#define fjr(d) for (int j = 0; j < d; j++)
//#define cout cout << "Line " << __LINE__ << " "

using namespace std;

struct position2
{
    int row;
    int column;
};

int main()
{
    //user input the dwarf position
    position2 dwarf_pos;
    cin >> dwarf_pos.row >> dwarf_pos.column;

    int row[7];
    memset(row, 0, sizeof(row));

    //先輸出擺放第一個矮人後一個可能的解
    fir(7)//create the matrix that contains the first dwarf and thus the other 6 dwarf
    {
        row[(dwarf_pos.row+i)%7] = (dwarf_pos.column+2*(i))%7;
    }
    
    //output the matrix with the first fixed dwarf
    cout << "fixed first dwarf:" << endl;
    fir(7)
    {
        if(i<6) cout << row[i] << ", ";
            else cout << row[i];
    }
    cout << endl << endl;

    //再輸出沒有限制第一個矮人時，6種非對稱的解
    cout << "all seven possible solution:" << endl;
    fir(7)
    {
        fjr(7)
        {
            row[j] = (dwarf_pos.column+2*(j)+i)%7;
            if(j<6) cout << row[j] << ", ";
            else cout << row[j];
        }
        cout << endl;
    }

    return 0;
}