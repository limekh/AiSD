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
	for (size_t i = 0; i < vec.size(); ++i) {
		for (size_t j = 0; j < vec.size() - i - 1; ++j) {

			++st.comparison_count;

			if (vec[j] > vec[j + 1]) {
				swap(vec[j], vec[j + 1]);
				st.copy_count += 3;
			}
		}
	}

	return st;
}



stats shells_sort(vector<int>& vec) {
	stats st;
	int temp = 0;
	size_t j = 0;
	size_t step = vec.size() / 2;

	while (step > 0) {
		for (size_t i = step; i < vec.size(); ++i) {
			temp = vec[i];
			j = i;
			st.comparison_count++;
			while (j >= step && vec[j - step] > temp) {
				vec[j] = vec[j - step];
				j -= step;
				st.copy_count++;
				st.comparison_count++;
			}
			vec[j] = temp;
			st.copy_count++;
		}
		step /= 2;
	}

	return st;
}



void heapify(vector<int>& vec, int n, int i, size_t& comparison, size_t& copy) {
	int largest = i;
	int left = 2 * i + 1; 
	int right = 2 * i + 2; 

	comparison++;
	if (left < n && vec[left] > vec[largest]) {
		largest = left;
	}

	comparison++;
	if (right < n && vec[right] > vec[largest]) {
		largest = right;
	}

	if (largest != i) {
		swap(vec[i], vec[largest]);
		copy += 3;
		heapify(vec, n, largest, comparison, copy);
	}
}
stats heap_sort(vector<int>& vec) {
	stats st;

	for (int i = vec.size() / 2 - 1; i >= 0; --i) {
		heapify(vec, vec.size(), i, st.comparison_count, st.copy_count);
	}

	for (int i = vec.size() - 1; i >= 1; --i) {
		swap(vec[0], vec[i]);
		st.copy_count += 3;
		heapify(vec, i, 0, st.comparison_count, st.copy_count);
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

	cout << "\nVector_2: " << vec2;
	stats shells = shells_sort(vec2);
	cout << "\n" << "SHELLS SORT\n";
	cout << vec2;
	cout << "Comparison Count: " << shells.comparison_count << "\n";
	cout << "Copy Count: " << shells.copy_count << "\n";

	cout << "\nVector_3: " << vec3;
	stats heap = heap_sort(vec3);
	cout << "\n" << "HEAP SORT\n";
	cout << vec3;
	cout << "Comparison Count: " << heap.comparison_count << "\n";
	cout << "Copy Count: " << heap.copy_count << "\n";

	return 0;
}