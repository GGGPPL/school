# HW9 戰鬥系統實作說明

## Setting 
* **玩家數量與等級**：預設為 2 位玩家，初始等級為 1。
* **職業選擇**：
    * 玩家 1: KnightPlayer (Arthur)
    * 玩家 2: MagicianPlayer (Merlin)
* **怪物種類與數量**：
    * GoblinMonster x1
    * ZombieMonster x1
* **回合制模式**：Entity-scale (每個實體依序行動)。

## 戰鬥進行流程與機制設計
* **攻擊傷害公式**：`max(0, 攻擊方的 Attack - 受攻擊方的 Defense)`。
* **怪物攻擊邏輯**：
    * 採用「優先攻擊第一位存活玩家」的策略。怪物會持續攻擊玩家列表中的第一個人，直到其死亡後才轉向下一位。
* **玩家行動機制**：
    * 玩家可選擇「一般攻擊」或「使用特殊技能」。
    * KnightPlayer 的技能：消耗 MP 回復 HP。
    * MagicianPlayer 的技能：消耗 HP 回復 MP。
* **戰後處理**：
    * 勝利後，存活與死亡玩家均會獲得該場戰鬥所有怪物的 EXP 與 Money 總和。
    * 死亡玩家會以 HP=1 復活（MP 保持死亡前的數值）。
    * 若 EXP 達到升級門檻，會自動升級並補滿 HP/MP，重新計算屬性。
* **繼承機制**：
    * 玩家的 LV、EXP、HP、MP、Money 均會保留至下一場戰鬥。

## 程式檔案結構
```
HW9_E24146107/
├── players/
│   ├── NovicePlayer.h/.cpp
│   ├── OrcPlayer.h/.cpp
│   ├── KnightPlayer.h/.cpp
│   └── MagicianPlayer.h/.cpp
├── monsters/
│   ├── BaseMonster.h/.cpp
│   ├── GoblinMonster.h/.cpp
│   ├── ZombieMonster.h/.cpp
│   └── JWMonster.h/.cpp
├── misc/
│   ├── Battle.h/.cpp
└── readme.md
```
