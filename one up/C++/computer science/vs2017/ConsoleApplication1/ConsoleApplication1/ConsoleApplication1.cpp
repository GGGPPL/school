#include <iostream>
#include <vector>
#include <random>       
#include <chrono>       
#include <limits>       
#include <iomanip>      

//#define cout cout << "Line " << __LINE__ << " "

using namespace std;


int kadane(const vector<int>& arr, int& start, int& end, int M) //find the maximum sum of a continuous subarray of the given array
{
	int max_so_far = arr[0];
	int current_max = arr[0];
	start = 0;
	end = 0;
	int current_start = 0;

	for (int i = 1; i < M; ++i) {
		// 比較「目前元素」和「目前元素 + 之前的總和」
		// 如果目前元素更大，表示我們應該從這個元素重新開始一個新的子序列
		if (arr[i] > current_max + arr[i]) {
			current_max = arr[i];
			current_start = i;
		}
		else {
			// 否則，將目前元素加入到當前的子序列中
			current_max += arr[i];
		}

		// 如果當前的最大和 > 紀錄的全域最大和，就更新它
		if (current_max > max_so_far) {
			max_so_far = current_max;
			start = current_start;
			end = i;
		}
	}
	return max_so_far;
}

void findMaxSumSubMatrix(const vector<vector<int>>& matrix) {
	int M = matrix.size();
	int N = matrix[0].size();

	int max_sum = numeric_limits<int>::min(); //we can't use 0 here because if the matrix has multiple negetive numbers, kadane will return 0
	int final_left, final_right, final_top, final_bottom;

	// list every possible column of submatrix
	for (int left = 0; left < N; ++left) {

		vector<int> temp_row_sum(M, 0); //store the sum of column of each row of submatrix

		for (int right = left; right < N; ++right) {

			// 將 right 這一欄的數值加到 temp_row_sum 中
			for (int i = 0; i < M; ++i) {
				temp_row_sum[i] += matrix[i][right];
			}

			// 3. 對 1D 陣列 temp_row_sum 執行 Kadane 演算法
			int current_start_row = 0;
			int current_end_row = 0;
			int current_sum = kadane(temp_row_sum, current_start_row, current_end_row, M);

			// 4. 更新全域最大值
			if (current_sum > max_sum) {
				max_sum = current_sum;
				final_left = left;
				final_right = right;
				final_top = current_start_row;
				final_bottom = current_end_row;
			}
		}
	}

	// --- 輸出結果 ---


	cout << "和: " << max_sum << endl;
	cout << "\n子矩陣:"
		<< "\n";

	// 輸出子矩陣
	for (int i = final_top; i <= final_bottom; ++i) {
		for (int j = final_left; j <= final_right; ++j) {
			cout << setw(6) << matrix[i][j] << " ";
		}
		cout << endl;
	}
}


void printMatrix(const vector<vector<int>>& matrix) {
	if (matrix.empty()) return;
	cout << "\nMatrix inputed (" << matrix.size() << "x" << matrix[0].size() << ") :\n";
	for (const auto& row : matrix) {//auto: automatically deduce the type, ":" --> assign each row of matrix to "row"
		for (int val : row) {
			cout << setw(6) << val << " "; //setw(6) : set the output width to 6, from <iomanip>
		}
		cout << endl;
	}
}

int main() {
	int M, N;
	cout << "M: ";
	cin >> M;
	cout << "N: ";
	cin >> N;

	vector<vector<int>> matrix(M, vector<int>(N));

	char choice;
	cout << "Manually input (M) or Randomly generate (R)? (M/R): ";
	cin >> choice;

	if (choice == 'R' || choice == 'r') {
		// 使用目前時間作為種子，產生高品質隨機數
		mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());//mt19937 engine random number generator
		// 產生 -20 到 20 之間的數字 (類似範例)
		uniform_int_distribution<> distr(-127, 127);

		for (int i = 0; i < M; ++i) {
			for (int j = 0; j < N; ++j) {
				matrix[i][j] = distr(gen);
			}
		}
	}
	else {
		cout << "Please key in the" << M * N << " elements of the matrix:\n";
		for (int i = 0; i < M; ++i) {
			for (int j = 0; j < N; ++j) {
				cin >> matrix[i][j];
			}
		}
	}

	printMatrix(matrix);

	findMaxSumSubMatrix(matrix);

	return 0;
}