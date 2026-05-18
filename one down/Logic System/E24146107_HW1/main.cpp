#include <iostream>
#include <vector>
#include <string>
#include "Kmap.cpp"

using namespace std;

void printKmapSolution(KmapSolution *sol)
{
    int nRow, nCol;
    switch (sol->numVar) {
    case 2:
        nRow = 2; nCol = 2;
        break;
    case 3:
        nRow = 2; nCol = 4;
        break;
    case 4:
        nRow = 4; nCol = 4;
        break;
    }

    cout << "K-map:\n";
    for (int i = 0; i < nRow; ++i) {
        for (int j = 0; j < nCol; ++j)
            cout << sol->kmap[i][j] << " ";
        cout << endl;
    }

    cout << "Prime implicants: ";
    for (int i = 0; i < sol->primes.size(); ++i) {
        cout << sol->primes[i];
        if (i != sol->primes.size() - 1) cout << ", ";
    }
    cout << endl;

    cout << "Essential prime implicants: ";
    for (int i = 0; i < sol->essentials.size(); ++i) {
        cout << sol->essentials[i];
        if (i != sol->essentials.size() - 1) cout << ", ";
    }
    cout << endl;

    cout << "Minimum SOP: ";
    for (int i = 0; i < sol->minimumSop.size(); ++i) {
        cout << sol->minimumSop[i];
        if (i != sol->minimumSop.size() - 1) cout << " + ";
    }
    cout << endl;
}

int main() {
    // Test Case from PDF Page 2
    // numVar = 4
    // minterms = [1, 3, 5, 7, 9]
    // dontcares = [6, 12, 13]
    
    int numVar = 4;
    vector<int> minterms = {1, 3, 5, 7, 9};
    vector<int> dontcares = {6, 12, 13};

    cout << "--- Testing K-map Solver (Page 2 Example) ---" << endl;
    KmapSolution *sol = solveKmap(numVar, minterms, dontcares);
    printKmapSolution(sol);

    return 0;
}
