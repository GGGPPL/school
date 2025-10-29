#include <iostream>
#include <vector>
#include <cstring>  // C++ style header for memset (though we removed most uses)
#include <ctime>    // C++ style header for time()
#include <cstdlib>
#include <algorithm>
#include <map>
#include <limits>   // For input validation

#define fir(d) for (int i = 0; i < d; i++)
#define fjr(d) for (int j = 0; j < d; j++)
#define fkr(d) for (int k = 0; k < d; k++)
#define fr(a, b) for (int a = 0; a < b; a++)
#define debug(d) cerr << "Line " << __LINE__ << " " << #d << ": " << d << endl
#define debugtext(d) // cerr << "Line " << __LINE__ << d << endl
// #define cout cout << "Line " << __LINE__ << " "

using namespace std;

struct card
{
    string type;
    string color;
};

struct Table
{
    // C++ style: a vector of vectors
    vector<vector<card>> AreaCard; 
    vector<card> SupplyDeck;
} GamingTable;


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

/**
 * @brief 顯示桌面狀態
 * @param tab 遊戲桌
 * @param player_num 玩家人數
 * @param AreaExist 追蹤哪些列還存在 (未被拿走)
 */
void ShowTable(Table tab, int player_num, const vector<bool>& AreaExist)
{
    cout << "--- 桌面狀態 ---\n";
    fir(player_num)
    {
        cout << "[" << i + 1 << "]: ";
        if (AreaExist[i] == false)
        {
            cout << "(已被玩家 " << (i+1) << " 拿走)" << endl;
        }
        else if (tab.AreaCard[i].empty()) // 存在，但是空的
        {
            cout << "empty!" << endl;
        }
        else // 存在，且有卡片
        {
            for (int j = 0; j < tab.AreaCard[i].size(); j++)
            {
                cout << "[" << tab.AreaCard[i][j].color << "] ";
            }
            if (tab.AreaCard[i].size() == 3)
            {
                cout << "(Full)"; // 顯示已滿
            }
            cout << endl;
        }
    }
    cout << "------------------\n";
}

// 輔助函式：清除 cin 錯誤狀態
void clearCin()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


int main()
{
    srand(time(NULL));

    // number of players
    int player_num;

    // make cards

    // 76 coloretto cards
    card coloretto[76];
    fir(76)
    {
        coloretto[i].type = "coloretto";
    }

    // 9 cards each in 7 colors
    // orange, yellow, green, brown, blue, purple, red
    fjr(9) coloretto[j].color = "orange";
    fjr(9) coloretto[j + 9].color = "yellow";
    fjr(9) coloretto[j + 18].color = "green";
    fjr(9) coloretto[j + 27].color = "brown";
    fjr(9) coloretto[j + 36].color = "blue";
    fjr(9) coloretto[j + 45].color = "purple";
    fjr(9) coloretto[j + 54].color = "red";

    // 3 joker
    coloretto[63].color = "joker";
    coloretto[64].color = "joker";
    coloretto[65].color = "joker";

    // 10 +2 cards
    fir(10) coloretto[66 + i].color = "+2";

    // 5 area cards
    card area[5];
    fir(5) { area[i].type = "area"; }

    // 5 summary cards
    card summary[5];
    fir(5) { summary[i].type = "summary"; }

    // 1 lastround card
    card last_round;
    last_round.type = "last_round";
    last_round.color = "last_round";

    // number of players
    while (true)
    {
        cout << "number of players (3-5): ";
        if (cin >> player_num && player_num >= 3 && player_num <= 5)
        {
            break; // 合法輸入
        }
        cout << "輸入無效，請輸入 3 到 5 之間的數字。\n";
        clearCin();
    }
    
    // *** VS 修正: 使用 vector 取代 VLA ***
    // const int onlyforvisualstudio = player_num; // <-- 移除
    
    // *** VS 修正: 調整 GamingTable 大小 ***
    GamingTable.AreaCard.resize(player_num);


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
        GamingTable.SupplyDeck.erase(GamingTable.SupplyDeck.begin() + erase_index * 9, GamingTable.SupplyDeck.begin() + erase_index * 9 + 9);
    }

    // (Debug 輸出)
    // fir(GamingTable.SupplyDeck.size()) { debug(GamingTable.SupplyDeck[i].color); }
    // cout << endl;

    // get each player one summary card
    struct PlayerArea
    {
        vector<card> PlayerDeck;
        vector<card> SummaryCardDeck;
    };
    
    // *** VS 修正: 使用 vector 取代 VLA ***
    vector<PlayerArea> Player(player_num); 

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
                fjr(i) // 檢查是否與"已發出"的牌 (玩家 0 到 i-1) 重複
                {
                    // debug(Player[j].PlayerDeck[0].color);
                    // debug(GamingTable.SupplyDeck[index].color);
                    if (Player[j].PlayerDeck[0].color == GamingTable.SupplyDeck[index].color)
                    {
                        WrongCard = true;
                        break;
                    }
                }
                if (WrongCard == false)
                {
                    // debug(GamingTable.SupplyDeck[index].color);
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

    // shuffle every coloretto card (改用 C++ 標準 <algorithm> 的 shuffle)
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(GamingTable.SupplyDeck.begin(), GamingTable.SupplyDeck.end(), g);

    // (Debug 輸出)
    // fir(GamingTable.SupplyDeck.size()) { debug(GamingTable.SupplyDeck[i].color); }
    // cout << endl;

    // put last round card at the 16th from the buttom
    // ** 規則修正: 應該是 end() - 15 **
    if (GamingTable.SupplyDeck.size() > 15)
    {
        GamingTable.SupplyDeck.insert(GamingTable.SupplyDeck.end() - 15, last_round);
    }
    else
    {
        GamingTable.SupplyDeck.push_back(last_round); // 牌不夠就放最後
    }

    // start from a random player, clockwise
    bool GameOver = false;
    int CurrentPlayer = rand() % player_num;

    // *** VS 修正: 建立 AreaExist (在第一次 ShowTable 之前) ***
    vector<bool> AreaExist(player_num, true);
    ShowTable(GamingTable, player_num, AreaExist);


    while (GameOver == false)
    {
        // *** VS 修正: 使用 vector 並在每回合重設 ***
        vector<bool> AreaFull(player_num, false);
        AreaExist.assign(player_num, true); // 重設 vector 內所有值為 true
        vector<bool> SkipList(player_num, false);

        bool TableFull = false;
        bool RoundOver = false;
        bool LastRound = false; // 這變數其實沒用到，GameOver 才重要
        
        int playersSkipped = 0; // 追蹤多少玩家已拿牌

        while (RoundOver == false)
        {
            // 檢查是否所有人都跳過了
            if (playersSkipped == player_num)
            {
                RoundOver = true;
                continue;
            }
            
            // 如果當前玩家已拿牌，跳過
            if (SkipList[CurrentPlayer] == true)
            {
                CurrentPlayer = (CurrentPlayer + 1) % player_num;
                continue; 
            }

            if (CurrentPlayer == 0)
            {
                cout << "Your turn!!!" << endl;
            }
            else
            {
                cout << "Player " << CurrentPlayer + 1 << "'s turn!!!" << endl;
            }

            // 檢查是否所有列都滿了
            TableFull = true;
            fjr(player_num)
            {
                if (AreaExist[j] == true && AreaFull[j] == false) // 如果有任何一列 "存在" 且 "未滿"
                {
                    TableFull = false;
                    break;
                }
            }

            // player decides what to do
            cout << "Do something (0 draw, 1 collect)" << endl;
            int UserInput;

            if (TableFull == true)
            {
                cout << "Table is full, you must collect an area." << endl;
                UserInput = 1; // 強制拿牌
            }
            else if (CurrentPlayer == 0)
            {
                while (true)
                {
                    if (cin >> UserInput && (UserInput == 0 || UserInput == 1))
                    {
                        break;
                    }
                    cout << "輸入無效，請輸入 0 (抽牌) 或 1 (拿牌)。\n";
                    clearCin();
                }
            }
            else
            {
                UserInput = rand() % 2;
                cout << UserInput << endl;
            }

            // debug(UserInput);


            // draw card
            if (UserInput == 0)
            {
                // 抽牌
                if (GamingTable.SupplyDeck.empty())
                {
                    cout << "牌堆已空! 這是最後一局。\n";
                    GameOver = true;
                    RoundOver = true; // 強制結束這回合
                    continue;
                }
                
                card drawnCard = GamingTable.SupplyDeck.back(); // 從牌堆底抽牌
                GamingTable.SupplyDeck.pop_back();

                if (drawnCard.color == "last_round")
                {
                    LastRound = true; // (這變數其實沒用到)
                    cout << "Last Round!!" << endl;
                    
                    // *** BUG 修正: == 是比較, = 才是賦值 ***
                    GameOver = true; 

                    if (GamingTable.SupplyDeck.empty())
                    {
                        cout << "抽到最後一回合卡，但牌堆已空，無法再抽。\n";
                        continue; // 跳過這回合
                    }
                    
                    cout << "再抽一張...\n";
                    drawnCard = GamingTable.SupplyDeck.back();
                    GamingTable.SupplyDeck.pop_back();
                }

                cout << "draw card" << endl;
                cout << "[" << drawnCard.color << "]" << endl;
                
                int TargetAreaCard;
                
                // *** 修正: 使用健壯的 while(true) 迴圈 ***
                while(true)
                {
                    cout << "target area card (1-" << player_num << "): "; // where to put the card
                    if (CurrentPlayer == 0)
                    {
                        if (!(cin >> TargetAreaCard))
                        {
                            cout << "輸入無效，請輸入數字。\n";
                            clearCin();
                            continue;
                        }
                    }
                    else
                    {
                        // *** BUG 修正: AI 應根據 player_num 選擇 ***
                        TargetAreaCard = (rand() % player_num) + 1;
                        cout << TargetAreaCard << endl;
                    }
                    
                    TargetAreaCard -= 1; // 轉成 0-based index

                    // 檢查輸入
                    if (TargetAreaCard < 0 || TargetAreaCard >= player_num)
                    {
                        cout << "無效的列號。\n";
                    }
                    else if (AreaExist[TargetAreaCard] == false)
                    {
                        cout << "這列已被拿走。\n";
                    }
                    else if (AreaFull[TargetAreaCard] == true) // the areadeck is full
                    {
                        cout << "area deck is full" << endl;
                    }
                    else
                    {
                        // 合法輸入
                        break;
                    }
                }

                GamingTable.AreaCard[TargetAreaCard].push_back(drawnCard);

                if (GamingTable.AreaCard[TargetAreaCard].size() == 3)
                {
                    AreaFull[TargetAreaCard] = true;
                }
                ShowTable(GamingTable, player_num, AreaExist);
            }

            // collect cards
            else // (UserInput == 1)
            {
                SkipList[CurrentPlayer] = true; // can no longer do anything until next round
                playersSkipped++;
                
                cout << "collect cards" << endl;
                int TargetAreaCard;

                // *** 修正: 使用健壯的 while(true) 迴圈 ***
                while(true)
                {
                    cout << "target area card (1-" << player_num << "): "; // which area to collect
                    if (CurrentPlayer == 0)
                    {
                        if (!(cin >> TargetAreaCard))
                        {
                            cout << "輸入無效，請輸入數字。\n";
                            clearCin();
                            continue;
                        }
                    }
                    else
                    {
                        // *** BUG 修正: AI 應根據 player_num 選擇 ***
                        TargetAreaCard = (rand() % player_num) + 1;
                        cout << TargetAreaCard << endl;
                    }

                    TargetAreaCard -= 1; // 轉成 0-based index

                    if (TargetAreaCard < 0 || TargetAreaCard >= player_num)
                    {
                         cout << "無效的列號。\n";
                    }
                    else if (AreaExist[TargetAreaCard] == false)
                    {
                        cout << "area deck is already collected!" << endl;
                        if(CurrentPlayer != 0) // 防止 AI 卡住
                        {
                            TargetAreaCard = (TargetAreaCard + 1) % player_num; // 試著拿下一列
                        }
                    }
                    else
                    {
                        break; // 合法輸入
                    }
                }

                // merge area card to player's deck
                Player[CurrentPlayer].PlayerDeck.insert(Player[CurrentPlayer].PlayerDeck.end(), 
                                                        GamingTable.AreaCard[TargetAreaCard].begin(), 
                                                        GamingTable.AreaCard[TargetAreaCard].end());
                GamingTable.AreaCard[TargetAreaCard].clear();
                AreaExist[TargetAreaCard] = false; // 標記為 "已被拿走"

                ShowTable(GamingTable, player_num, AreaExist);
            }

            // 檢查是否所有人都拿牌了 (RoundOver 的另一種條件)
            if (playersSkipped == player_num)
            {
                RoundOver = true;
            }

            if (!RoundOver)
            {
                // 輪到下一位玩家
                CurrentPlayer = (CurrentPlayer + 1) % player_num;
            }
            else
            {
                cout << "Round over" << endl;
            }
        } // end of while(RoundOver == false)
    } // end of while(GameOver == false)

    // Game Over
    // Calculate Score
    // *** VS 修正: 使用 vector 取代 VLA ***
    vector<int> Score(player_num);

    fir(player_num)
    {
        // *** BUG 修正: 應計算 Player[i] 而非 CurrentPlayer ***
        Score[i] = CalculateScore(Player[i].PlayerDeck);
    }
    
    cout << "\n--- 最終分數 ---\n";
    fir(player_num)
    {
        cout << "Player " << i + 1 << " score: " << Score[i] << endl;
    }

    return 0;
}