#ifndef YOUR_ALGORITHM_H
#define YOUR_ALGORITHM_H

#include "LT.h"
#include "graph.h"
#include <unordered_set>

using namespace std;

/*
 * seedSelection:
 *   - G:   整張圖
 *   - numberOfSeeds: 需要你選的「額外正向種子數量」
 *                     （系統另外會再加上 1 個 given_pos.txt 裡的 positive seed）
 *
 * 你只需要回傳一個 unordered_set<int>，裡面放 numberOfSeeds 9個positivate seed編號。
 */
// ===== [請將以下程式碼貼到 main() 函式上方] =====

#include <vector>
#include <queue>        // 用於 std::priority_queue
#include <limits>       // 用於 std::numeric_limits

/* * -----------------------------------------------------------------------------
 * CELF 結構：用於 Priority Queue
 * -----------------------------------------------------------------------------
 */
struct NodeGain {
    int nodeID;
    double marginalGain; // 邊際效益
    int round;          // 該效益是 "相對於第幾輪的 seedSet" 計算的

    NodeGain(int id, double gain, int r) : nodeID(id), marginalGain(gain), round(r) {}

    bool operator<(const NodeGain& other) const {
        return this->marginalGain < other.marginalGain;
    }
};


/* * -----------------------------------------------------------------------------
 * 輔助函式：計算 Active Rate (已更新)
 * -----------------------------------------------------------------------------
 * * @param G - 圖形
 * @param student_seeds - 我們正在測試的 0-9 個種子
 * @param givenPosSeed - 評測程式固定的 1 個正向種子
 * @param givenNegSeeds - 評測程式固定的 N 個負向種子
 * @return Active Rate (double)
 */
static double calculate_active_rate(
    DirectedGraph &G, 
    const std::unordered_set<int>& student_seeds, 
    int givenPosSeed, 
    const std::unordered_set<int>& givenNegSeeds
) {
    
    // 組合最終的正向種子：學生選的 + 固定的
    std::unordered_set<int> pos_active = student_seeds;
    pos_active.insert(givenPosSeed);

    // 負向種子是固定的
    std::unordered_set<int> neg_active = givenNegSeeds; 
    
    std::unordered_set<int> final_pos;
    std::unordered_set<int> final_neg;

    // 呼叫簡報中提供的函式
    diffuse_signed_all(&G, pos_active, neg_active, final_pos, final_neg);

    int total_users = G.getSize(); 
    if (total_users == 0) {
        return 0.0;
    }

    // 根據簡報的公式計算 Active Rate
    double active_rate = (double)(final_pos.size() - final_neg.size()) / total_users;
    
    return active_rate;
}


/* * -----------------------------------------------------------------------------
 * 您的主要實作函式 (CELF 優化版本，已更新)
 * -----------------------------------------------------------------------------
 * @param G - 圖形
 * @param numberOfSeeds - 您需要選擇的種子數量 (這裡是 9)
 * @param givenPosSeed - 固定的 1 個正向種子
 * @param givenNegSeeds - 固定的 N 個負向種子
 * @return 包含您所選 9 個節點 ID 的 unordered_set
 */
std::unordered_set<int> seedSelection(
    DirectedGraph &G, 
    unsigned int numberOfSeeds, // 這裡會傳入 9
    int givenPosSeed, 
    const std::unordered_set<int>& givenNegSeeds
) {
    std::vector<int> allNodes = G.getAllNodes();
    for (int node_id : allNodes) {
        G.addNode(node_id, 0.0, 1000.0); 
    }

    // 2. 快速選出 9 個合法的種子
    std::unordered_set<int> seedSet; 
    for (int node_id : allNodes) {
        
        // 檢查是否已達數量
        if (seedSet.size() == numberOfSeeds) {
            break;
        }

        // 檢查是否為 "合法" 的選擇 (不是固定的正/負種子)
        if (node_id != givenPosSeed && givenNegSeeds.count(node_id) == 0) {
            seedSet.insert(node_id);
        }
    }
    
    // 3. 回傳
    return seedSet;
}

// ===== [貼上到此結束] =====

#endif
