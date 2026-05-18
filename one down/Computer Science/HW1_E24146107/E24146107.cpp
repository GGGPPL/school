#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

// DSU 結構設計
struct DSU {
	vector<int> parent;
	vector<int> sz;
	int max_friend_group;

	DSU(int n) {
		parent.resize(n + 1);
		sz.resize(n + 1, 1); // 初始每個人自己一圈，人數為 1
		for (int i = 1; i <= n; i++) parent[i] = i;
		max_friend_group = (n > 0) ? 1 : 0;
	}

	// 尋找根節點（含路徑壓縮優化）
	int find(int i) {
		if (parent[i] == i) return i;
		return parent[i] = find(parent[i]);
	}

	// 合併兩個人的圈子
	void unite(int i, int j) {
		int root_i = find(i);
		int root_j = find(j);
		if (root_i != root_j) {
			// 將小圈子併入大圈子
			if (sz[root_i] < sz[root_j]) swap(root_i, root_j);
			parent[root_j] = root_i;
			sz[root_i] += sz[root_j];
			// 更新目前最大交友圈人數
			if (sz[root_i] > max_friend_group) {
				max_friend_group = sz[root_i];
			}
		}
	}
};


int main() {
	string filename;
	// 規定 1：直接讀取檔名，不輸出任何提示字串 
	if (!(cin >> filename)) return 0;

	// 規定 2：使用相對路徑讀檔 
	ifstream inputFile(filename);
	if (!inputFile) return 0;

	int T;
	if (!(inputFile >> T)) return 0;

	while (T--) {
		int N, M;
		if (!(inputFile >> N >> M)) break;

		DSU dsu(N);
		for (int i = 0; i < M; i++) {
			int u, v;
			inputFile >> u >> v;
			// 處理 N=0 或無效編號的邊界情況
			if (u >= 1 && u <= N && v >= 1 && v <= N) {
				dsu.unite(u, v);
			}
		}
		// 規定 4：每筆測資輸出最大人數並換行 
		cout << dsu.max_friend_group << endl;
	}

	inputFile.close();
	return 0;
}