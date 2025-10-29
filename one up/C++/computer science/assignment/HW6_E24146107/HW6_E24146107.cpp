#include<iostream>
#include<vector>
#include<string>
#include<time.h>
#include<cstdlib>
#include<algorithm>

#define fir(d) for(int i=0; i<d; i++)
#define fjr(d) for(int j=0; j<d; j++)
#define fkr(d) for(int k=0; k<d; k++)
#define fr(a,b) for(int a=0; a<b; a++)
#define debug(d) cerr << "Line " << __LINE__ << " " << #d << ": " << d << endl
#define debugtext(d) cerr << "Line " << __LINE__ << d << endl

using namespace std;

int main()
{
    srand(time(NULL));

    //number of players
    int player_num;

    //make cards
    struct card
    {
        string type;
        string color;
    };

    //76 coloretto cards
    card coloretto[76]; 
    fir(76)
    {
        coloretto[i].type = "coloretto";
    }

    //9 cards each in 7 colors
    //orange, yellow, green, brown, blue, purple, red
    fjr(9)
    coloretto[j].color = "orange";
    fjr(9)
    coloretto[j+9].color = "yellow";
    fjr(9)
    coloretto[j+18].color = "green";
    fjr(9)
    coloretto[j+27].color = "brown";
    fjr(9)
    coloretto[j+36].color = "blue";
    fjr(9)
    coloretto[j+45].color = "purple";
    fjr(9)
    coloretto[j+54].color = "red";



    //3 joker
    coloretto[63].color = "joker";
    coloretto[64].color = "joker";
    coloretto[65].color = "joker";

    //10 +2 cards
    fir(10)
    coloretto[66+i].color = "+2";

    //5 area cards
    card area[5];
    fir(5)
    {
        area[i].type = "area";
    }

    //5 summary cards
    card summary[5];
    fir(5)
    {
        summary[i].type = "summary";
    }

    //1 lastround card
    card last_round;
    last_round.type = "last_round";
    
    
    //number of players
    cout << "number of players: ";
    cin >> player_num;

    //make supply deck
    vector<card> SupplyDeck;
    fir(sizeof(coloretto)/sizeof(coloretto[0]))
    {
        SupplyDeck.push_back(coloretto[i]);
    }
    

    //3 players
    if(player_num == 3)
    {
        
        //remove one color of coloretto card
        int erase_index = rand() % 7;
        SupplyDeck.erase(SupplyDeck.begin() + erase_index*9, SupplyDeck.begin() + erase_index*9 + 9);

    }

    fir(SupplyDeck.size())
    {
        debug(SupplyDeck[i].color);
    } cout << endl;

    //get each player one summary card
    const int onlyforvisualstudio = player_num;
    struct PlayerArea
    {
        vector<card> PlayerDeck;
        vector<card> SummaryCardDeck;
    } Player[onlyforvisualstudio];
    fir(player_num)
    {
        Player[i].SummaryCardDeck.push_back(summary[i]);
    }

    //get each player one coloretteo card
    //not joker or +2 and each coloretto cannot be the SAME COLOR
    fir(player_num)
    {
        
        
    }
    fir(player_num)
    {
        debug(Player[i].PlayerDeck[0].color);
    } cout << endl;
    
    //shuffle every coloretto card
    fir(100){
        fjr(SupplyDeck.size())
        {
            int index1 = rand() % SupplyDeck.size();
            int index2 = rand() % SupplyDeck.size();
            card temp = SupplyDeck[index1];
            SupplyDeck[index1] = SupplyDeck[index2];
            SupplyDeck[index2] = temp;
        }
    }
    fir(SupplyDeck.size())
    {
        //debug(SupplyDeck[i].color);
    } cout << endl;

    //put last round card at the 16th from the buttom

    //place area card as many as the number of players

    //start from a tandom player, clockwise



    
    
	
	return 0;
}