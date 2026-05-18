#include <iostream>
#include <string>
#include "KnightPlayer.h"
#include "MagicianPlayer.h"
#include "NovicePlayer.h"
#include "OrcPlayer.h"
#include "GoblinMonster.h"
#include "ZombieMonster.h"
#include "JWMonster.h"

using namespace std;

int main()
{
    // =========================================================
    // 1. 玩家類別測試區 (你剛才跑過的部分)[cite: 3]
    // =========================================================
    NovicePlayer novice4(3, "testNovice");
    novice4.setLevel(novice4.getLevel() + 1);

    OrcPlayer Orc4(3, "testOrc");
    Orc4.setLevel(Orc4.getLevel() + 1);

    KnightPlayer Knight4(3, "testKnight");
    Knight4.setLevel(Knight4.getLevel() + 1);
    Knight4.setHp(Knight4.getHp() - 40);
    Knight4.specialSkill(); // 執行 Heal[cite: 4]

    MagicianPlayer Magician4(3, "testMagician");
    Magician4.setLevel(Magician4.getLevel() + 1);
    Magician4.setMp(Magician4.getMp() - 30);
    Magician4.specialSkill(); // 執行 Pray[cite: 6]

    cout << "Magician4 after praying:" << endl;
    Magician4.print();

    // =========================================================
    // 2. 玩家序列化測試 (Serialization)
    // =========================================================
    cout << "\n=== Player Serialization Test ===" << endl;
    string magicianSerialized = Magician4.serialize();
    cout << "Magician4 serialized: " << magicianSerialized << endl;

    NovicePlayer* Magician5 = MagicianPlayer::unserialize(magicianSerialized);
    cout << "Magician5 (unserialized from Magician4):" << endl;
    Magician5->print();
    delete Magician5;

    // =========================================================
    // 3. 玩家多型測試 (Polymorphism)
    // =========================================================
    cout << "\n=== Player Polymorphism Test ===" << endl;
    NovicePlayer* players[4];
    players[0] = new NovicePlayer(1, "PolyNovice");
    players[1] = new OrcPlayer(2, "PolyOrc");
    players[2] = new KnightPlayer(3, "PolyKnight");
    players[3] = new MagicianPlayer(4, "PolyMagician");

    for (int i = 0; i < 4; i++) {
        players[i]->setLevel(players[i]->getLevel() + 1);
        players[i]->specialSkill(); 
        players[i]->print();
    }
    for (int i = 0; i < 4; i++) delete players[i];

    // =========================================================
    // 4. 怪物類別測試 (Monster Series)
    // =========================================================
    cout << "\n=== Monster Testing ===" << endl;
    cout << "Initial monster count: " << BaseMonster::getInstanceCount() << endl;

    GoblinMonster goblin1;
    cout << "goblin1 name: " << goblin1.name << " (Attack: " << goblin1.attack << ")" << endl;
    
    // 怪物序列化測試[cite: 2]
    string goblinStr = goblin1.serialize();
    cout << "goblin1 serialized: " << goblinStr << endl;

    BaseMonster* goblin2 = GoblinMonster::unserialize(goblinStr);
    cout << "goblin2 hp (from unserialize): " << goblin2->getHP() << endl;
    delete goblin2;

    // 怪物數量與生命週期測試[cite: 2]
    {
        ZombieMonster zombie1;
        JWMonster jw1;
        cout << "Monster count in scope: " << BaseMonster::getInstanceCount() << endl;
    }
    cout << "Monster count after scope exit: " << BaseMonster::getInstanceCount() << endl;

    return 0;
}