#include <iostream>
#include <vector>

using namespace std;

struct stats {
	size_t comparison_count = 0;
	size_t copy_count;
};

stats bubble_sort(vector<int>& vec) {
	stats st;

	size_t size = vec.size();
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size; ++j) {

			++st.comparison_count;

			if (vec[j] > vec[j + 1]) {
				swap(vec[j], vec[j + 1]);
				st.copy_count += 3;
			}
		}
	}

	return st;
}

int main() {



}