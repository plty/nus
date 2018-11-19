#include <iostream>
#include <vector>
int calculate_minimum_halls(int N, std::vector<int> start, std::vector<int> end) {
	return 0;
}

int calculate_minimum_cancels(int N, int L, std::vector<int> start, std::vector<int> end) {
	return 0;
}

int main(){
	int testCases;
	std::cin >> testCases;
	for (int tc = 0; tc < testCases; tc++) {
		int N;
		std::cin >> N;
		int L;
		std::cin >> L;
		std::vector<int> start;
		for (int i = 0; i < N; ++i) {
			int startTime;
			std::cin >> startTime;
			start.push_back(startTime);
		}
		std::vector<int> end;
		for (int i = 0; i < N; ++i) {
			int endTime;
			std::cin >> endTime;
			end.push_back(endTime);
		}
		std::cout << calculate_minimum_halls(N, start, end) << std::endl;
		std::cout << calculate_minimum_cancels(N, L, start, end) << std::endl;
	}
	return 0;
}
