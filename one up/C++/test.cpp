#include<iostream>
#include<chrono>

using namespace std;

int main()
{
    while(1)
    {
        cout << chrono::steady_clock::now().time_since_epoch().count() << endl;
    }
    return 0;
}