#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iomanip>
#include <iterator>

using namespace std;

// 修改：將 Itemset 定義為 string 的 vector
typedef vector<string> Itemset;
typedef vector<Itemset> Database;

struct Rule {
	Itemset lhs;
	Itemset rhs;
	double confidence;
};

map<Itemset, int> global_support_counts;

// ---------------------------------------------------------
// 1. 讀取資料 (針對 A,C,D 格式修正)
// ---------------------------------------------------------
Database loadData(string filename) {
	Database db;
	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "錯誤: 無法開啟檔案 " << filename << endl;
		exit(1);
	}

	string line;
	while (getline(file, line)) {
		if (line.empty()) continue;

		stringstream ss(line);
		string id, items_str;

		// 讀取第一欄 ID (例如 00001)
		ss >> id;

		// 讀取後面的商品字串 (例如 A,C,D)
		// 注意：有些檔案可能 ID 後面直接接商品，有些有空白，這裡假設用空白分隔 ID 與商品
		if (ss >> items_str) {
			// 將逗號 ',' 替換為空白 ' ' 以便分割
			for (char &c : items_str) {
				if (c == ',') c = ' ';
			}

			stringstream ss_items(items_str);
			string item;
			Itemset transaction;

			while (ss_items >> item) {
				transaction.push_back(item);
			}

			// 排序並去重 (A~Z)
			sort(transaction.begin(), transaction.end());
			transaction.erase(unique(transaction.begin(), transaction.end()), transaction.end());

			if (!transaction.empty()) {
				db.push_back(transaction);
			}
		}
	}
	file.close();
	return db;
}

// ---------------------------------------------------------
// 2. Apriori 核心 (邏輯不變，僅配合 string 調整)
// ---------------------------------------------------------

map<Itemset, int> countSupport(const Database& db, const vector<Itemset>& candidates) {
	map<Itemset, int> counts;
	for (const auto& transaction : db) {
		for (const auto& candidate : candidates) {
			if (includes(transaction.begin(), transaction.end(),
				candidate.begin(), candidate.end())) {
				counts[candidate]++;
			}
		}
	}
	return counts;
}

vector<Itemset> generateCandidates(const vector<Itemset>& Lk, int k) {
	vector<Itemset> candidates;
	int n = Lk.size();

	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			bool joinable = true;
			for (int x = 0; x < k - 1; ++x) {
				if (Lk[i][x] != Lk[j][x]) {
					joinable = false;
					break;
				}
			}

			if (joinable) {
				Itemset new_cand = Lk[i];
				new_cand.push_back(Lk[j].back());
				candidates.push_back(new_cand);
			}
		}
	}
	return candidates;
}

void generateSubsets(const Itemset& itemset, Itemset& subset, int index, vector<Itemset>& all_subsets) {
	if (index == itemset.size()) {
		if (!subset.empty() && subset.size() < itemset.size()) {
			all_subsets.push_back(subset);
		}
		return;
	}
	generateSubsets(itemset, subset, index + 1, all_subsets);
	subset.push_back(itemset[index]);
	generateSubsets(itemset, subset, index + 1, all_subsets);
	subset.pop_back();
}

bool compareRules(const Rule& a, const Rule& b) {
	// 規則 1: 左邊元素數量多到少
	if (a.lhs.size() != b.lhs.size()) {
		return a.lhs.size() > b.lhs.size();
	}
	// 規則 2: A~Z 排序
	return a.lhs < b.lhs;
}

// ---------------------------------------------------------
// 主程式
// ---------------------------------------------------------
int main() {
	string filename;
	double min_support, min_confidence;

	cout << "Input file name: ";
	cin >> filename;
	cout << "Input min_supp: ";
	cin >> min_support;
	cout << "Input min_conf: ";
	cin >> min_confidence;

	// 提醒助教
	cout << fixed << setprecision(2);
	// cout << "\n[Config] File: " << filename << ", Supp: " << min_support << ", Conf: " << min_confidence << endl;

	Database db = loadData(filename);
	int total_transactions = db.size();
	int min_sup_count = (int)(total_transactions * min_support);

	// --- Apriori Start ---
	vector<Itemset> L_current;

	// L1
	map<string, int> item_counts;
	for (const auto& t : db) {
		for (const string& item : t) item_counts[item]++;
	}

	for (const auto& pair : item_counts) {
		if (pair.second >= min_sup_count) {
			Itemset s = { pair.first };
			L_current.push_back(s);
			global_support_counts[s] = pair.second;
		}
	}
	sort(L_current.begin(), L_current.end());

	int k = 1;
	while (!L_current.empty()) {
		vector<Itemset> candidates = generateCandidates(L_current, k);
		if (candidates.empty()) break;

		map<Itemset, int> counts = countSupport(db, candidates);
		vector<Itemset> L_next;

		for (const auto& pair : counts) {
			if (pair.second >= min_sup_count) {
				L_next.push_back(pair.first);
				global_support_counts[pair.first] = pair.second;
			}
		}

		L_current = L_next;
		sort(L_current.begin(), L_current.end());
		k++;
	}

	// --- Generate Rules ---
	vector<Rule> final_rules;
	for (const auto& entry : global_support_counts) {
		Itemset I = entry.first;
		int support_I = entry.second;

		if (I.size() < 2) continue;

		Itemset subset_temp;
		vector<Itemset> subsets;
		generateSubsets(I, subset_temp, 0, subsets);

		for (const auto& S : subsets) {
			int support_S = global_support_counts[S];
			double conf = (double)support_I / support_S;

			if (conf >= min_confidence) {
				Rule r;
				r.lhs = S;
				// RHS = I - S
				set_difference(I.begin(), I.end(), S.begin(), S.end(),
					inserter(r.rhs, r.rhs.begin()));
				r.confidence = conf;
				final_rules.push_back(r);
			}
		}
	}

	// --- Output ---
	sort(final_rules.begin(), final_rules.end(), compareRules);

	cout << final_rules.size() << endl;

	for (const auto& r : final_rules) {
		for (size_t i = 0; i < r.lhs.size(); ++i) {
			cout << r.lhs[i] << (i == r.lhs.size() - 1 ? "" : " ");
		}
		cout << " => ";
		for (size_t i = 0; i < r.rhs.size(); ++i) {
			cout << r.rhs[i] << (i == r.rhs.size() - 1 ? "" : " ");
		}
		cout << endl;
	}

	return 0;
}