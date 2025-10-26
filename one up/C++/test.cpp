#include <iostream>
#include <vector>

// 盤子編號：1 (最小) 到 N (最大)
// 塔編號：0 (Current), 1 (Target), 2 (Idle)

// 假設 Tower 是 std::vector<int> Tower[3]; 的指標傳遞
void Hanoi(std::vector<int> Tower[], int n_disks, int CurrentTower, int TargetTower, int IdleTower)
{
    // 終止條件：如果只需要移動最小的盤子 (即盤子數量 n = 1)
    if (n_disks == 1)
    {
        // 1. 取得最小盤子（在當前塔的最頂端）
        // 由於我們假設 Tower[] 儲存的盤子是從下到上排序，所以最頂端盤子就是 .back()
        if (Tower[CurrentTower].empty()) return; // 錯誤處理：塔為空

        int disk_value = Tower[CurrentTower].back();

        // 2. 移動盤子
        Tower[CurrentTower].pop_back();
        Tower[TargetTower].push_back(disk_value);

        // 3. 輸出移動步驟
        std::cout << "Disk " << disk_value 
                  << " moving from Tower " << CurrentTower + 1 
                  << " to Tower " << TargetTower + 1 << std::endl;
        return;
    }

    // 遞迴步驟：移動 n 塊盤子從 CurrentTower 到 TargetTower

    // 步驟一：將 n-1 塊盤子從 CurrentTower 移到 IdleTower (輔助塔)
    Hanoi(Tower, n_disks - 1, CurrentTower, IdleTower, TargetTower);

    // 步驟二：將最大的第 n 塊盤子從 CurrentTower 移到 TargetTower
    Hanoi(Tower, 1, CurrentTower, TargetTower, IdleTower);

    // 步驟三：將 n-1 塊盤子從 IdleTower 移到 TargetTower
    Hanoi(Tower, n_disks - 1, IdleTower, TargetTower, CurrentTower);
}

int main() {
    // 假設有 3 個塔，從 1 號塔 (索引 0) 移動到 3 號塔 (索引 2)
    const int NUM_TOWERS = 3;
    std::vector<int> Tower[NUM_TOWERS];

    // 初始盤子：假設 3 個盤子 (3, 2, 1) 從底到頂
    Tower[0] = {3, 2, 1}; 
    int total_disks = Tower[0].size();

    // 呼叫函式：移動 3 塊盤子，從塔 0 到塔 2，塔 1 為輔助
    Hanoi(Tower, total_disks, 0, 2, 1);

    // 檢查結果
    std::cout << "\nTower 3 (Target) final content: ";
    for (int disk : Tower[2]) {
        std::cout << disk << " "; // 輸出: 3 2 1
    }
    std::cout << std::endl;

    return 0;
}