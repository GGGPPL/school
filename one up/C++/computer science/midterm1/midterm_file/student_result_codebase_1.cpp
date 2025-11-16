/* * -----------------------------------------------------------------------------
 * 必要的標頭檔
 * -----------------------------------------------------------------------------
 */
#include <vector>
#include <unordered_set>
#include <limits>       // 用於 std::numeric_limits
#include <queue>        // 用於 std::priority_queue
#include <functional>   // (備用)

// 您的專案標頭檔 (根據簡報)
#include "graph.h"
#include "LT.h"



// 假設 "student_submit.h" 宣告了 seedSelection 函式
#include "student_submit.h" 


/* * -----------------------------------------------------------------------------
 * 輔助函式：計算 Active Rate
 * -----------------------------------------------------------------------------
 * 執行一次完整的擴散模擬並回傳 (pos - neg) / total 的分數。
 * (這與前一個版本完全相同)
 */
static double calculate_active_rate(DirectedGraph &G, const std::unordered_set<int>& positive_seeds) {
    
    std::unordered_set<int> pos_active = positive_seeds;
    std::unordered_set<int> neg_active; // 初始負向種子為空
    
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
 * CELF 結構：用於 Priority Queue
 * -----------------------------------------------------------------------------
 * 儲存節點的邊際效益，以及該效益是在第幾輪計算的。
 */
struct NodeGain {
    int nodeID;
    double marginalGain; // 邊際效益
    int round;          // 該效益是 "相對於第幾輪的 seedSet" 計算的

    // Constructor
    NodeGain(int id, double gain, int r) : nodeID(id), marginalGain(gain), round(r) {}

    // Overload '<' 運算子，讓 priority_queue 變成 Max-Heap
    bool operator<(const NodeGain& other) const {
        return this->marginalGain < other.marginalGain;
    }
};


/* * -----------------------------------------------------------------------------
 * 您的主要實作函式 (CELF 優化版本)
 * -----------------------------------------------------------------------------
 * @param G - 圖形
 * @param numberOfSeeds - 您需要選擇的種子數量
 * @return 包含您所選節點 ID 的 unordered_set
 */
std::unordered_set<int> seedSelection (DirectedGraph &G, unsigned int numberOfSeeds) {
    
    std::unordered_set<int> seedSet; // 最終要回傳的種子集合
    std::vector<int> allNodes = G.getAllNodes(); // 獲取所有節點 ID
    
    // --- 邊界情況處理 ---
    if (numberOfSeeds >= allNodes.size()) {
         for(int node : allNodes) {
             seedSet.insert(node);
         }
         return seedSet;
    }
    if (numberOfSeeds == 0) {
        return seedSet;
    }

    // --- CELF 演算法 ---
    
    // 1. 初始化優先佇列 (Max-Heap)
    std::priority_queue<NodeGain> pq;
    
    // 2. 計算空集合的分數 (基準分數)
    double current_score = calculate_active_rate(G, seedSet);

    // 3. (第一輪) 計算所有節點的初始邊際效益 (相對於 "空集合")
    //    這是 CELF 演算法中第一次 (也是唯一一次) 密集的計算
    for (int node : allNodes) {
        std::unordered_set<int> tempSet = {node};
        double gain = calculate_active_rate(G, tempSet) - current_score;
        
        // `round = 0` 表示這個效益是相對於 0 個種子 (空集合) 計算的
        pq.push(NodeGain(node, gain, 0)); 
    }

    // 4. 主迴圈：選出 k 個種子
    while (seedSet.size() < numberOfSeeds) {
        
        // 5. 從佇列頂端取出目前效益最高的節點
        NodeGain topNode = pq.top();
        pq.pop();

        // 6. --- 核心 "Lazy" 檢查 ---
        // 檢查這個節點的效益是否是 "最新" 的？
        // `topNode.round` 儲存的是它上次計算效益時，seedSet 的大小
        // `seedSet.size()` 是 "目前" seedSet 的大小
        
        if (topNode.round == seedSet.size()) {
            // **情況 A: 效益是最新的 (up-to-date)**
            // 太好了！這就是我們要的節點。
            seedSet.insert(topNode.nodeID);
            
            // 更新 "基準分數"
            current_score = current_score + topNode.marginalGain;
            
            // 進入下一輪 (while 迴圈繼續)
            
        } else {
            // **情況 B: 效益是 "過時的" (stale)**
            // (e.g. topNode.round 是 0, 但 seedSet.size() 已經是 1)
            
            // 我們必須重新計算這個節點 "相對於目前 seedSet" 的邊際效益
            
            // (保險起見，再次檢查)
            if (seedSet.count(topNode.nodeID)) {
                continue; // 該節點已被選過，跳過
            }

            std::unordered_set<int> tempSet = seedSet;
            tempSet.insert(topNode.nodeID);
            
            // **重新計算昂貴的邊際效益**
            double new_score = calculate_active_rate(G, tempSet);
            double new_gain = new_score - current_score;
            
            // 將 "更新" 過的節點放回佇列
            // `round` 設為 `seedSet.size()`，表示這是最新的效益
            pq.push(NodeGain(topNode.nodeID, new_gain, seedSet.size()));
            
            // 注意：我們 *還沒有* 選擇節點
            // 我們只是更新了 topNode 的效益，然後 `continue`
            // `while` 迴圈會重新開始，再次 `pq.top()` (也許是別的節點)
        }
    }
    
    // 提醒：請確保您的函式中沒有任何 cout 或 cin
    return seedSet;
}