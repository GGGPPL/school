#ifndef STUDENT_SUBMIT_H
#define STUDENT_SUBMIT_H

#include "LT.h"
#include "graph.h"
#include <unordered_set>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <map>
#include <iostream>
#include <queue>
#include <limits>
#include <set>

using namespace std;

/**
 * Standard interface function required by the problem statement.
 */
unordered_set<int> seedSelection(DirectedGraph& G,
                                     unsigned int numberOfSeeds,
                                     int givenPosSeed,
                                     const unordered_set<int>& givenNegSeeds);

class Solver {
private:
    DirectedGraph* graph;
    int targetPosSeed;
    unordered_set<int> negativeSeeds;
    
    // Data structures for features
    struct NodeFeatures {
        int id;
        vector<double> feats;
    };
    map<int, NodeFeatures> featureMap;
    
    // Scoring components
    vector<double> featureScores;
    vector<double> structuralScores;
    vector<double> negativeRiskScores;
    vector<double> finalScores;
    
    // Constants for tuning
    const double ALPHA_STRUCTURAL = 0.5;  // Weight for structural influence
    const double BETA_FEATURE = 0.3;      // Weight for feature similarity
    const double GAMMA_RISK = 2.0;        // Penalty multiplier for negative risk
    const double LAMBDA_DISCOUNT = 0.7;   // Discount factor for neighbors of chosen seeds

public:
    Solver(DirectedGraph* g, int posSeed, const unordered_set<int>& negSeeds) 
        : graph(g), targetPosSeed(posSeed), negativeSeeds(negSeeds) {
        int numNodes = 0;
        if (g!= nullptr) {
            // Determine maximum node ID for vector resizing
            vector<int> nodes = g->getAllNodes();
            for (int u : nodes) {
                numNodes = max(numNodes, u + 1);
            }
        } else {
            numNodes = 10000; // Fallback
        }
        
        featureScores.resize(numNodes, 0.0);
        structuralScores.resize(numNodes, 0.0);
        negativeRiskScores.resize(numNodes, 0.0);
        finalScores.resize(numNodes, 0.0);
    }

    // ---------------------------------------------------------
    // Module 1: Feature Processing
    // ---------------------------------------------------------
    void loadAndProcessFeatures(const string& filename) {
        ifstream infile(filename);
        if (!infile.is_open()) {
            return;
        }

        string line;
        // Skip header/count line
        getline(infile, line); 
        
        while (getline(infile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            int id;
            double f1, f2;
            if (ss >> id >> f1 >> f2) {
                NodeFeatures nf;
                nf.id = id;
                nf.feats.push_back(f1);
                nf.feats.push_back(f2);
                featureMap[id] = nf;
            }
        }
        infile.close();

        // Calculate Similarity to Target Seed
        if (featureMap.find(targetPosSeed) != featureMap.end()) {
            // *ERROR SOURCE: featureMap is not FeatureMap. The member name is 'featureMap'.
            // The member access needs to be node's features, not the map's features.
            const auto& targetNodeFeats = featureMap.at(targetPosSeed).feats;
            
            for (auto const& [id, nodeFeat] : featureMap) {
                if (id == targetPosSeed) continue;
                double dist = euclideanDistance(targetNodeFeats, nodeFeat.feats);
                // Similarity is inverse of distance. 
                if (id < featureScores.size()) {
                    featureScores[id] = 1.0 / (1.0 + dist);
                }
            }
        }
    }

    double euclideanDistance(const vector<double>& v1, const vector<double>& v2) {
        double sum = 0.0;
        for (size_t i = 0; i < v1.size() && i < v2.size(); ++i) {
            double diff = v1[i] - v2[i];
            sum += diff * diff;
        }
        return sqrt(sum);
    }

    // ---------------------------------------------------------
    // Module 2: Risk Assessment (Negative Influence Analysis)
    // ---------------------------------------------------------
    void calculateNegativeRisk() {
        
        // Initialize risk with 0
        fill(negativeRiskScores.begin(), negativeRiskScores.end(), 0.0);

        int maxDepth = 3;
        
        for (int negSeed : negativeSeeds) {
            queue<pair<int, int>> q;
            
            // localImpact 應在每個種子迴圈內初始化
            map<int, double> localImpact;
            
            // 修正 2: 初始化 map 結構
            localImpact[negSeed] = 1.0; 
            q.push({negSeed, 0});
            
            while(!q.empty()) {
                pair<int, int> curr = q.front();
                q.pop();
                int u = curr.first;
                int depth = curr.second;

                if (depth >= maxDepth) continue;

                double currentForce = localImpact[u];
                if (currentForce < 0.01) continue; 

                vector<int> neighbors = graph->getNodeOutNeighbors(u);
                for (int v : neighbors) {
                    double weight = graph->getEdgeInfluence(u, v);
                    double passedInfluence = currentForce * weight;

                    // 累積 global risk
                    if (negativeRiskScores.size() > v) {
                        negativeRiskScores[v] += passedInfluence;
                    }

                    // 傳播
                    if (localImpact.find(v) == localImpact.end() && v < negativeRiskScores.size()) {
                        localImpact[v] = passedInfluence;
                        q.push({v, depth + 1});
                    }
                }
            }
        }
        
        // Normalize Risk Scores
        double maxRisk = 0.0;
        for (double r : negativeRiskScores) maxRisk = max(maxRisk, r);
        if (maxRisk > 0) {
            for (double &r : negativeRiskScores) r /= maxRisk;
        }
    }

    // ---------------------------------------------------------
    // Module 3: Structural Power Analysis (Local Influence)
    // ---------------------------------------------------------
    void calculateStructuralPower() {
        vector<int> allNodes = graph->getAllNodes();
        
        for (int u : allNodes) {
            if (negativeSeeds.count(u) || u == targetPosSeed) continue;

            double power = 0.0;
            vector<int> outNeighbors = graph->getNodeOutNeighbors(u);
            
            for (int v : outNeighbors) {
                double weight = graph->getEdgeInfluence(u, v);
                
                double value = weight;
                
                // 1. 懲罰 v 的高風險
                double v_risk = (negativeRiskScores.size() > v)? negativeRiskScores[v] : 0.0;
                value *= (1.0 - v_risk * 0.8); 
                
                // 2. 獎勵 v 的高出度 (2-hop)
                double v_outdegree = (double)graph->getNodeOutNeighbors(v).size();
                value *= (1.0 + log(1.0 + v_outdegree) * 0.1);

                power += value;
            }
            
            if (structuralScores.size() > u) {
                structuralScores[u] = power;
            }
        }

        // Normalize Structural Scores
        double maxStruct = 0.0;
        for (double s : structuralScores) maxStruct = max(maxStruct, s);
        if (maxStruct > 0) {
            for (double &s : structuralScores) s /= maxStruct;
        }
    }

    // ---------------------------------------------------------
    // Module 4: Composite Scoring & Selection
    // ---------------------------------------------------------
    void computeFinalScores() {
        vector<int> allNodes = graph->getAllNodes();
        for (int u : allNodes) {
            
            // 修正 1: 修正邏輯 OR 運算子
            if (negativeSeeds.count(u) || u == targetPosSeed) { 
                finalScores[u] = -1000.0; // Invalid candidates
                continue;
            }

            // Score Formula
            double s_score = structuralScores[u];
            double f_score = (featureScores.size() > u)? featureScores[u] : 0.0;
            double r_score = (negativeRiskScores.size() > u)? negativeRiskScores[u] : 0.0;

            // Score Formula: High Structural, high Feature Sim, low Risk
            double total = (ALPHA_STRUCTURAL * s_score) + 
                           (BETA_FEATURE * f_score) - 
                           (GAMMA_RISK * r_score);
            
            finalScores[u] = total;
        }
    }

    unordered_set<int> selectSeeds(unsigned int k) {
        unordered_set<int> selectedSeeds;
        vector<bool> isSelected(finalScores.size(), false);
        
        // Mark pre-existing seeds as selected to trigger discount logic correctly
        if (isSelected.size() > targetPosSeed) isSelected[targetPosSeed] = true;
        for (int neg : negativeSeeds) {
            if (isSelected.size() > neg) isSelected[neg] = true;
        }

        // Iterative Greedy Selection with Discount
        for (unsigned int i = 0; i < k; ++i) {
            int bestNode = -1;
            double maxVal = -999999.0;

            // Find node with max current score
            vector<int> nodes = graph->getAllNodes();
            for (int u : nodes) {
                if (!isSelected[u] && u < finalScores.size() && finalScores[u] > maxVal) {
                    // Note: No need to recheck negativeSeeds.count(u) since it was already -1000 in computeFinalScores
                    maxVal = finalScores[u];
                    bestNode = u;
                }
            }

            if (bestNode!= -1) {
                selectedSeeds.insert(bestNode);
                isSelected[bestNode] = true;

                // Apply Discount to neighbors (Spatial Diversity)
                vector<int> neighbors = graph->getNodeOutNeighbors(bestNode); 
                for (int v : neighbors) {
                    if (!isSelected[v] && finalScores.size() > v) {
                        finalScores[v] *= LAMBDA_DISCOUNT;
                    }
                }
            } else {
                break; 
            }
        }
        
        // Final sanity check for size (rarely needed for this method but robust)
        if (selectedSeeds.size() < k) {
            vector<int> nodes = graph->getAllNodes();
            for (int u : nodes) {
                if (selectedSeeds.size() == k) break;
                if (!isSelected[u]) selectedSeeds.insert(u);
            }
        }

        return selectedSeeds;
    }
};

/**
 * Standard interface function required by the problem statement.
 */
inline unordered_set<int> seedSelection(DirectedGraph& G,
                                         unsigned int numberOfSeeds,
                                         int givenPosSeed,
                                         const unordered_set<int>& givenNegSeeds) {
    // 1. Initialize Solver
    Solver solver(&G, givenPosSeed, givenNegSeeds);

    // 2. Load Features (Assuming 'node_features.txt' is the file name)
    // Note: The structure of the features file is inferred. This might need adjustment.
    solver.loadAndProcessFeatures("node.txt"); 

    // 3. Calculate Risks (Pre-computation)
    solver.calculateNegativeRisk();

    // 4. Calculate Potentials
    solver.calculateStructuralPower();

    // 5. Combine Scores
    solver.computeFinalScores();

    // 6. Select Seeds
    return solver.selectSeeds(numberOfSeeds);
}

#endif // STUDENT_SUBMIT_H