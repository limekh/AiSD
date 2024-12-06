#include <iostream>
#include <vector>
#include <functional>

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



vector<int> random_vec(const size_t& size, unsigned seed) {
	vector<int> vec(size);
	srand(seed);
	for (size_t i = 0; i < size; ++i) {
		vec[i] = rand() % 1000;
	}
	return vec;
}

vector<int> sorted_vec(const size_t& size) {
	vector<int> vec(size);
	for (size_t i = 0; i < size; ++i) {
		vec[i] = i;
	}
	return vec;
}

vector<int> reverse_sorted_vec(const size_t& size) {
	vector<int> vec(size);
	for (size_t i = 0; i < size; ++i) {
		vec[i] = size - i;
	}
	return vec;
}

void num_for_rand() {
	stats st, result;
	vector<size_t> sizes = { 1000, 2000, 3000, 5000, 10000, 25000, 50000, 100000 };
	vector<function<stats(std::vector<int>&)>> sort_funcs = { bubble_sort, shells_sort, heap_sort };
	for (size_t size : sizes) {
		cout << "\nVector size: " << size << "\n";
		for(size_t i = 0; i < sort_funcs.size(); ++i){ 
			result.comparison_count = 0;
			result.copy_count = 0;
			for (size_t j = 0; j < 100; ++j) {
				vector<int> vec = random_vec(size, 1);
				st = sort_funcs[i](vec);
				result.comparison_count += st.comparison_count;
				result.copy_count += st.copy_count;
			}
			cout << "Average Comparison Count: " << result.comparison_count / 100 << "\n";
			cout << "Average Copy Count: " << result.copy_count / 100 << "\n";
		}
		
	}
}

void num_for_sorted() {
	stats st, result;
	vector<size_t> sizes = { 1000, 2000, 3000, 5000, 10000, 25000, 50000, 100000 };
	vector<function<stats(std::vector<int>&)>> sort_funcs = { bubble_sort, shells_sort, heap_sort };
	for (size_t size : sizes) {
		cout << "\nVector size: " << size << "\n";
		for (size_t i = 0; i < sort_funcs.size(); ++i) {
			result.comparison_count = 0;
			result.copy_count = 0;
			vector<int> vec = sorted_vec(size);
			st = sort_funcs[i](vec);
			result.comparison_count += st.comparison_count;
			result.copy_count += st.copy_count;
			cout << "Comparison Count: " << result.comparison_count << "\n";
			cout << "Copy Count: " << result.copy_count << "\n";
		}
	}
}

void num_for_reverse_sorted() {
	stats st, result;
	vector<size_t> sizes = { 1000, 2000, 3000, 5000, 10000, 25000, 50000, 100000 };
	vector<function<stats(std::vector<int>&)>> sort_funcs = { bubble_sort, shells_sort, heap_sort };
	for (size_t size : sizes) {
		cout << "\nVector size: " << size << "\n";
		for (size_t i = 0; i < sort_funcs.size(); ++i) {
			result.comparison_count = 0;
			result.copy_count = 0;
			vector<int> vec = reverse_sorted_vec(size);
			st = sort_funcs[i](vec);
			result.comparison_count += st.comparison_count;
			result.copy_count += st.copy_count;
			cout << "Comparison Count: " << result.comparison_count << "\n";
			cout << "Copy Count: " << result.copy_count << "\n";
		}
	}
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

	cout << "\n100 RANDOM VECTORS: \n";
	//num_for_rand();

	cout << "\nSORTED VECTORS: \n";
	num_for_sorted();

	cout << "\nREVERSE SORTED VECTORS: \n";
	num_for_reverse_sorted();

	return 0;
}