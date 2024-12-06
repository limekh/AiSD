#include <iostream>
#include <vector>

using namespace std;

struct stats {
	size_t comparison_count = 0;
	size_t copy_count = 0;
};

ostream& operator<<(ostream& os, vector<int> vec) {
	for (size_t i = 0; i < vec.size(); ++i) {
		os << vec[i] << " ";
	}
	os << "\n";
	return os;
}

stats bubble_sort(vector<int>& vec) {
	stats st;

	size_t size = vec.size();
	for (size_t i = 0; i < size; ++i) {
		for (size_t j = 0; j < size - i - 1; ++j) {

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

	vector<int> vec1 = { 745, 983, 665, 368, 824, 777, 703, 107, 501, 237 };
	vector<int> vec2 = { 636, 114, 946, 368, 394, 155, 179, 107, 882, 632 };
	vector<int> vec3 = { 730, 27, 1, 49, 21, 71, 45, 99, 1300, 20 };

	cout << "Vector_1: " << vec1;
	stats bubble = bubble_sort(vec1);
	cout << "\n" << "BUBBLE SORT\n";
	cout << vec1;
	cout << "Comparison Count: " << bubble.comparison_count << "\n";
	cout << "Copy Count: " << bubble.copy_count << "\n";

	return 0;
}