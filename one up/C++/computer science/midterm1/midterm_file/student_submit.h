/**
 * ==========================================================================================
 * Project: Signed Network Influence Maximization (Final Competitive Solver)
 * File: student_submit.h
 * Strategy: Iterated Greedy with Dynamic Contribution Audit (IG-DCA)
 * Author: AI Solver (Grandmaster Edition)
 * Date: 2025-11-21
 * * [演算法設計哲學]
 * 針對有號網路 (Signed Network) 的影響力最大化問題，傳統的貪婪演算法 (Greedy) 容易因為
 * 負邊的連鎖效應而選出「陷阱解」。單純的隨機交換 (Random Swap) 則效率低落。
 * * 本演算法採用「迭代貪婪 (Iterated Greedy)」架構，並結合「動態審計」機制：
 * 1. 菁英候選池 (Smart Candidate Pool):
 * - 結合「攻擊潛力 (Out-Degree)」與「防禦價值 (Blocking Value)」。
 * - 防禦價值定義：該節點對「被負種子攻擊的受害者」的影響力總和。
 * * 2. 建構階段 (Construction):
 * - 使用隨機化貪婪策略快速生成初始解。
 * * 3. 審計與優化 (Audit & Optimize Loop):
 * - 審計 (Audit): 對當前手上的種子逐一進行「移除測試」，計算其邊際貢獻 (Marginal Contribution)。
 * - 剔除 (Purge): 移除貢獻度最低 (甚至為負) 的種子，因為它可能是引起負面連鎖的元兇。
 * - 重構 (Reconstruct): 在空出的位置上，嘗試從候選池中尋找最佳替代者填補。
 * * 4. 擾動 (Perturbation):
 * - 當陷入僵局時，隨機破壞部分解構，強制跳脫區域最佳解。
 * * [時間複雜度控制]
 * - 利用高精度計時器 (Chronometer) 嚴格控制模擬次數，確保在 28.5 秒內輸出最佳解。
 * ==========================================================================================
 */

#ifndef YOUR_ALGORITHM_H
#define YOUR_ALGORITHM_H

#include "LT.h"
#include "graph.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <cmath>
#include <queue>
#include <limits>
#include <set>
#include <random>
#include <map>
#include <iomanip>
#include <functional>
#include <numeric>

using namespace std;

// ==========================================================================================
// [Module 1] 全域配置 (Global Configuration)
// ==========================================================================================
namespace SolverConfig {
    // 總時間限制 (秒)
    const double TIME_LIMIT = 28.5;
    
    // 候選池大小 (Top-K)
    // 選取全圖分數最高的 K 個節點進入決賽圈
    // 縮小範圍是為了讓後續的 Audit 能跑更多輪
    const int CANDIDATE_POOL_SIZE = 120;

    // 審計頻率：每幾輪迭代進行一次完整的「最弱種子」檢查
    // 審計成本較高 (需要 K 次模擬)，不能每輪都做
    const int AUDIT_INTERVAL = 3;

    // 每次嘗試填補空缺時，隨機抽樣的候選人數量
    // 避免窮舉所有候選人導致超時
    const int REFILL_SAMPLE_SIZE = 20;
    
    // 防禦權重：在初選時，保護受害者的價值倍率
    const double DEFENSE_WEIGHT_MULTIPLIER = 2.5;
}

// ==========================================================================================
// [Module 2] 高精度計時器 (Chronometer)
// ==========================================================================================
class Chronometer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    double limit_seconds;

public:
    Chronometer(double limit = SolverConfig::TIME_LIMIT) : limit_seconds(limit) {
        reset();
    }

    void reset() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double getElapsed() const {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = now - start_time;
        return diff.count();
    }

    bool isTimeOut() const {
        return getElapsed() >= limit_seconds;
    }

    // 預判是否有足夠時間執行一次複雜操作 (例如審計)
    bool hasTimeForComplexOp() const {
        return getElapsed() < (limit_seconds - 1.0); 
    }
};

// ==========================================================================================
// [Module 3] 亂數引擎 (Random Engine)
// ==========================================================================================
class RandomEngine {
private:
    mt19937 rng;

public:
    RandomEngine() {
        random_device rd;
        rng.seed(rd());
    }

    // [min, max]
    int nextInt(int min, int max) {
        if (min >= max) return min;
        uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    double nextDouble() {
        uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng);
    }

    // 從 vector 中隨機選取 N 個不重複元素
    template<typename T>
    vector<T> sample(const vector<T>& source, int n) {
        if (source.size() <= n) return source;
        
        vector<T> result;
        vector<int> indices(source.size());
        iota(indices.begin(), indices.end(), 0);
        
        // 部分洗牌 (Partial Shuffle) - Fisher-Yates 變體
        for (int i = 0; i < n; ++i) {
            int j = nextInt(i, indices.size() - 1);
            swap(indices[i], indices[j]);
            result.push_back(source[indices[i]]);
        }
        return result;
    }
    
    template<typename T>
    T pickOne(const vector<T>& source) {
        if (source.empty()) return T();
        return source[nextInt(0, source.size() - 1)];
    }
};

// ==========================================================================================
// [Module 4] 模擬預言機 (Simulation Oracle)
// 封裝 LT 模型，提供統一的評估接口，並負責記憶體管理
// ==========================================================================================
class SimulationOracle {
private:
    DirectedGraph* G;
    const unordered_set<int>* negSeedsRef;

public:
    SimulationOracle(DirectedGraph& graph, const unordered_set<int>& negSeeds) 
        : G(&graph), negSeedsRef(&negSeeds) {}

    // 核心評估函數
    // Return: Positive Nodes - Negative Nodes
    double evaluate(const unordered_set<int>& posSeeds) {
        // 複製種子集合 (LT 擴散函數會修改它，必須傳入副本)
        // 這是效能瓶頸之一，但在 LT.h 介面限制下無法避免
        unordered_set<int> currentPos = posSeeds;
        unordered_set<int> currentNeg = *negSeedsRef;

        // 呼叫 LT.h 的擴散函式
        // 使用 diffuse_signed_all 確保達到穩定態 (Steady State)
        diffuse_signed_all(G, currentPos, currentNeg);

        double pCount = (double)currentPos.size();
        double nCount = (double)currentNeg.size();

        // 目標函數：最大化 (P - N)
        return pCount - nCount;
    }
};

// ==========================================================================================
// [Module 5] 智慧候選池 (Smart Candidate Pool)
// 負責初步篩選，將計算資源集中在最有希望的節點上
// ==========================================================================================
struct CandidateInfo {
    int id;
    double heuristicScore; // 啟發式評分
};

class SmartCandidatePool {
private:
    DirectedGraph* G;
    const unordered_set<int>* negSeedsRef;
    vector<int> pool; // 最終篩選出的菁英名單
    unordered_set<int> poolSet; // 用於 O(1) 查找

public:
    SmartCandidatePool(DirectedGraph& graph, const unordered_set<int>& negSeeds) 
        : G(&graph), negSeedsRef(&negSeeds) {}

    void build(int k, int givenPosSeed) {
        // 1. 識別「受害者名單」 (Victims)
        // 這些是被負面種子直接攻擊的節點。
        // 策略：如果我們能反過來影響這些受害者，就能阻斷負面傳播 (Blocking)。
        unordered_set<int> victims;
        for (int neg : *negSeedsRef) {
            vector<int> targets = G->getNodeOutNeighbors(neg);
            for (int t : targets) {
                victims.insert(t);
            }
        }

        // 2. 計算所有節點的評分
        vector<CandidateInfo> allCandidates;
        auto allNodes = G->getAllNodes();

        for (int u : allNodes) {
            // 排除已存在的種子與負面種子
            if (u == givenPosSeed || negSeedsRef->count(u)) continue;

            double offenseScore = 0.0;
            double defenseScore = 0.0;
            
            vector<int> neighbors = G->getNodeOutNeighbors(u);
            for (int v : neighbors) {
                double weight = G->getEdgeInfluence(u, v);
                
                // 基礎攻擊分：我有多少影響力
                offenseScore += weight;

                // 防禦加成：如果我的鄰居是受害者，這個連結價值倍增
                if (victims.count(v)) {
                    defenseScore += (weight * SolverConfig::DEFENSE_WEIGHT_MULTIPLIER); 
                }
            }

            // 綜合評分
            // 對於有號網路，防禦往往比盲目進攻更重要 (因為負分很痛)
            double finalScore = offenseScore + defenseScore;
            
            // 過濾掉完全無效的節點 (孤立點或弱連結)
            if (finalScore > 1e-5) {
                allCandidates.push_back({u, finalScore});
            }
        }

        // 3. 排序並選取 Top K
        sort(allCandidates.begin(), allCandidates.end(), 
             [](const CandidateInfo& a, const CandidateInfo& b) {
            return a.heuristicScore > b.heuristicScore;
        });

        pool.clear();
        poolSet.clear();
        for (int i = 0; i < min((int)allCandidates.size(), k); ++i) {
            pool.push_back(allCandidates[i].id);
            poolSet.insert(allCandidates[i].id);
        }
    }

    const vector<int>& getAll() const { return pool; }
    
    bool contains(int id) const { return poolSet.count(id); }
};

// ==========================================================================================
// [Module 6] 迭代貪婪求解器 (Iterated Greedy Solver)
// 演算法核心邏輯控制器
// ==========================================================================================
class IteratedGreedySolver {
private:
    DirectedGraph* G;
    int givenPosSeed;
    const unordered_set<int>* negSeedsRef;
    int targetBudget;

    // Components
    Chronometer timer;
    RandomEngine rng;
    SimulationOracle oracle;
    SmartCandidatePool candidatePool;

    // State
    unordered_set<int> currentSolution;
    double currentScore;
    
    unordered_set<int> bestSolution;
    double bestScore;

public:
    IteratedGreedySolver(DirectedGraph& graph, int pSeed, const unordered_set<int>& nSeeds, int b)
        : G(&graph), givenPosSeed(pSeed), negSeedsRef(&nSeeds), targetBudget(b),
          oracle(graph, nSeeds), candidatePool(graph, nSeeds) {
        bestScore = -1e9; // 初始化為極小值
    }

    unordered_set<int> solve() {
        // 1. 初始化候選池
        candidatePool.build(SolverConfig::CANDIDATE_POOL_SIZE, givenPosSeed);
        
        // 2. 產生初始解 (Construction Phase)
        // 使用隨機化貪婪策略，多試幾次取最好的起點
        generateInitialSolution();

        int iteration = 0;
        int idleIterations = 0; // 連續未改進次數

        // 3. 主優化迴圈 (Optimization Phase)
        while (!timer.isTimeOut()) {
            iteration++;
            bool improved = false;

            // 策略分支：
            // 如果有足夠時間，且到了審計週期，執行深度審計 (Destruction & Reconstruction)
            if (iteration % SolverConfig::AUDIT_INTERVAL == 0 && timer.hasTimeForComplexOp()) {
                improved = performAuditAndRefine();
            } else {
                // 否則執行輕量級的隨機替換 (Random Swap)
                improved = performRandomSwap();
            }

            if (improved) {
                idleIterations = 0;
            } else {
                idleIterations++;
            }

            // 4. 擾動機制 (Perturbation)
            // 如果連續 15 輪沒進步，隨機破壞解結構，強制跳脫 Local Optima
            if (idleIterations >= 15 && timer.hasTimeForComplexOp()) {
                performPerturbation();
                idleIterations = 0;
            }
        }

        return formatOutput();
    }

private:
    // 產生初始解：隨機從候選池選滿，多試幾次取最佳
    void generateInitialSolution() {
        currentSolution.clear();
        currentSolution.insert(givenPosSeed);

        double maxInitScore = -1e9;
        unordered_set<int> maxInitSet;

        const vector<int>& candidates = candidatePool.getAll();
        
        // 嘗試 3 次隨機初始化
        for (int k = 0; k < 3; ++k) {
            if (timer.isTimeOut()) break;
            
            unordered_set<int> temp = currentSolution; // 包含 given seed
            // 隨機補滿剩餘預算
            vector<int> sample = rng.sample(candidates, targetBudget);
            for (int s : sample) temp.insert(s);

            double score = oracle.evaluate(temp);
            if (score > maxInitScore) {
                maxInitScore = score;
                maxInitSet = temp;
            }
        }

        // Fallback: 如果時間不夠跑完上面，或是出錯
        if (maxInitSet.empty()) { 
            maxInitSet.insert(givenPosSeed);
            for(int i=0; i<targetBudget && i<candidates.size(); ++i)
                maxInitSet.insert(candidates[i]);
            maxInitScore = oracle.evaluate(maxInitSet);
        }

        currentSolution = maxInitSet;
        currentScore = maxInitScore;
        updateGlobalBest(currentSolution, currentScore);
    }

    // [核心策略 A] 審計與精煉 (Audit & Refine)
    // 1. 找出當前解中的「老鼠屎」(對分數貢獻最低，甚至為負者)。
    // 2. 移除它。
    // 3. 尋找更強的替補填入空缺。
    bool performAuditAndRefine() {
        // 1. 找出可移除的種子 (排除 given seed)
        vector<int> removable;
        for (int s : currentSolution) {
            if (s != givenPosSeed) removable.push_back(s);
        }
        if (removable.empty()) return false;

        // 2. 計算每個種子的「邊際貢獻 (Marginal Contribution)」
        // Contribution(s) = CurrentScore - Score(Solution \ {s})
        int worstSeed = -1;
        double minContribution = 1e9; // 越小代表越沒用，甚至是有害的

        for (int s : removable) {
            if (!timer.hasTimeForComplexOp()) break;

            unordered_set<int> temp = currentSolution;
            temp.erase(s);
            double scoreWithoutS = oracle.evaluate(temp);
            
            double contrib = currentScore - scoreWithoutS;

            if (contrib < minContribution) {
                minContribution = contrib;
                worstSeed = s;
            }
        }

        if (worstSeed == -1) return false;

        // 3. 移除最差種子 (Destruction)
        currentSolution.erase(worstSeed);
        
        // 4. 填補 (Reconstruction)：嘗試從候選池中找一個能讓分數提升最多的
        int bestCandidate = -1;
        double bestNewScore = -1e9;

        // 從還沒被選的候選人中採樣
        vector<int> candidatesToTry;
        const vector<int>& pool = candidatePool.getAll();
        for (int c : pool) {
            if (currentSolution.find(c) == currentSolution.end()) {
                candidatesToTry.push_back(c);
            }
        }
        // 只測試採樣的子集，以節省時間
        vector<int> sample = rng.sample(candidatesToTry, SolverConfig::REFILL_SAMPLE_SIZE);

        for (int c : sample) {
            // 簡單的剪枝：如果時間快不夠了，就別試了
            if (!timer.hasTimeForComplexOp()) break;

            unordered_set<int> temp = currentSolution;
            temp.insert(c);
            double val = oracle.evaluate(temp);

            if (val > bestNewScore) {
                bestNewScore = val;
                bestCandidate = c;
            }
        }

        // 5. 決策
        // 如果填補後的分數比原本(還沒移除前)還高，則接受變更
        if (bestCandidate != -1 && bestNewScore > currentScore) {
            // 成功升級！
            currentSolution.insert(bestCandidate);
            currentScore = bestNewScore;
            updateGlobalBest(currentSolution, currentScore);
            return true;
        } else {
            // 失敗，沒找到更好的，把原本被冤枉的種子加回去 (還原)
            currentSolution.insert(worstSeed);
            return false;
        }
    }

    // [核心策略 B] 快速隨機交換 (Random Swap)
    // 成本低，用於快速探索鄰域
    bool performRandomSwap() {
        vector<int> removable;
        for (int s : currentSolution) {
            if (s != givenPosSeed) removable.push_back(s);
        }
        
        // 隨機挑選一個移除
        int nodeToRemove = rng.pickOne(removable);
        
        // 隨機挑選一個加入
        int nodeToAdd = -1;
        const vector<int>& pool = candidatePool.getAll();
        
        // 嘗試 5 次尋找合法的加入對象
        for(int k=0; k<5; ++k) {
            int c = rng.pickOne(pool);
            if (currentSolution.find(c) == currentSolution.end()) {
                nodeToAdd = c;
                break;
            }
        }

        if (nodeToAdd == -1) return false;

        // 模擬評估
        unordered_set<int> temp = currentSolution;
        temp.erase(nodeToRemove);
        temp.insert(nodeToAdd);

        double newScore = oracle.evaluate(temp);

        if (newScore > currentScore) {
            currentSolution = temp;
            currentScore = newScore;
            updateGlobalBest(currentSolution, currentScore);
            return true;
        }

        return false;
    }

    // [核心策略 C] 擾動 (Perturbation)
    // 隨機移除 M 個種子，並隨機補滿，強制跳脫 Local Optima
    void performPerturbation() {
        int seedsToRemove = 2; // 每次隨機重置 2 個位置
        vector<int> removable;
        for (int s : currentSolution) {
            if (s != givenPosSeed) removable.push_back(s);
        }

        if (removable.size() < seedsToRemove) return;

        vector<int> removed = rng.sample(removable, seedsToRemove);
        for (int r : removed) {
            currentSolution.erase(r);
        }

        // 隨機補滿
        const vector<int>& pool = candidatePool.getAll();
        while (currentSolution.size() < targetBudget + 1) { // +1 for given seed
            int c = rng.pickOne(pool);
            if (currentSolution.find(c) == currentSolution.end()) {
                currentSolution.insert(c);
            }
        }
        
        // 更新分數 (注意：擾動後分數通常會變差，這是正常的，為了探索新路徑)
        currentScore = oracle.evaluate(currentSolution);
        // 不更新 GlobalBest，因為現在可能很爛
    }

    void updateGlobalBest(const unordered_set<int>& sol, double score) {
        if (score > bestScore) {
            bestScore = score;
            bestSolution = sol;
        }
    }

    // 格式化輸出，確保符合題目要求 (正好 9 個，不含 given seed)
    unordered_set<int> formatOutput() {
        unordered_set<int> result;
        for (int s : bestSolution) {
            if (s != givenPosSeed) result.insert(s);
        }
        
        // 安全檢查 1：如果數量不足 (極端情況)，補滿
        while (result.size() < targetBudget) {
            const vector<int>& pool = candidatePool.getAll();
            for (int c : pool) {
                if (result.size() >= targetBudget) break;
                if (c != givenPosSeed && result.find(c) == result.end() && negSeedsRef->find(c) == negSeedsRef->end()) {
                    result.insert(c);
                }
            }
        }
        
        // 安全檢查 2：如果數量過多 (程式邏輯錯誤)，刪除
        while (result.size() > targetBudget) {
            result.erase(result.begin());
        }
        return result;
    }
};

// ==========================================================================================
// [Main Interface] seedSelection
// 題目規定的進入點
// ==========================================================================================
/*
 * seedSelection:
 * - G:   整張圖
 * - numberOfSeeds: 需要你選的「額外正向種子數量」
 *
 * 回傳: 一個 unordered_set<int>，包含 numberOfSeeds 個正向種子編號。
 */
unordered_set<int> seedSelection(DirectedGraph& G,
    unsigned int numberOfSeeds,
    int givenPosSeed,
    const unordered_set<int>& givenNegSeeds) {
    
    // 建立 IG-DCA 求解器並執行
    IteratedGreedySolver solver(G, givenPosSeed, givenNegSeeds, numberOfSeeds);
    
    return solver.solve();
}

#endif // YOUR_ALGORITHM_H