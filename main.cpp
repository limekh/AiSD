#include <iostream>
#include <utility>
#include <chrono>

struct TNode {
	int info;
	TNode* left; 
	TNode* right;

	TNode(int value) : info(value), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
	TNode* root;

	void clear(TNode* root) {
		if (!root) return; 
		
		clear(root->left);
		clear(root->right);
		delete root;
	}

	TNode* copy(const TNode* root) const {
		if (!root) return nullptr;
		auto* newNode = new TNode(root->info);
		newNode->left = copy(root->left);
		newNode->right = copy(root->right);
		return newNode;
	}

	bool insert(TNode*& root, int key) {
		if (!root) {
			root = new TNode(key);
			return true;
		}
		if (key < root->info) return insert(root->left, key);
		if (key > root->info) return insert(root->right, key);
		return false;
	}

	TNode* findMin(TNode* root) const{
		while (root && root->left) root = root->left;
		return root;
	}

	bool erase(TNode*& root, int key) {
		if (!root) return false;

		if (key < root->info) return erase(root->left, key);
		if (key > root->info) return erase(root->right, key);

		if (!root->left && !root->right) { // Нет потомков
			delete root;
			root = nullptr;
		}
		else if (!root->left) { // Только правый потомок
			TNode* temp = root->right;
			delete root;
			root = temp;
		}
		else if (!root->right) { // Только левый потомок
			TNode* temp = root->left;
			delete root;
			root = temp;
		}
		else { // Два потомка
			TNode* minRight = findMin(root->right);
			root->info = minRight->info;
			erase(root->right, minRight->info);
		}
		return true;
	}

	bool contains(TNode* root, int key) const {
		if (!root) return false;
		if (key < root->info) return contains(root->left, key);
		if (key > root->info) return contains(root->right, key);
		return true;
	}

	void print(TNode* root) {
		if (!root) return;
		print(root->left);
		std::cout << root->info << " ";
		print(root->right);
	}

public:
	BinaryTree() : root(nullptr) {}

	BinaryTree(const BinaryTree& other) : root(copy(other.root)) {}

	~BinaryTree() { clear(root); }

	BinaryTree& operator=(BinaryTree other) {
		std::swap(root, other.root);
		return *this;
	}

	bool insert(int key) {
		return insert(root, key);
	}

	bool contains(int key) const {
		return contains(root, key);
	}

	bool erase(int key) {
		return erase(root, key);
	}

	void print() {
		print(root);
		std::cout << "\n";
	}

	TNode* getRoot() {
		return root;
	}

	int findMin() {
		TNode* r = findMin(root);
		return r -> info;
	}

};

size_t lcg() {
	static size_t x = 0;
	x = (1021 * x + 24631) % 116640;
	return x;
}

double getInsertTime(BinaryTree& tree, int count) {
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < count; ++i) {
		tree.insert(lcg());
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> res = end - start;

	return res.count();
}

double getSearchTime(BinaryTree& tree, int attempts) {
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < attempts; ++i) {

		tree.contains(lcg());

	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> res = end - start;

	return res.count();
}

double getDeleteTime(BinaryTree& tree, int attempts) {
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < attempts; ++i) {

		tree.erase(lcg());

	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> res = end - start;

	return res.count();
}

double getInsertTime(std::vector<int> vec, int count) {
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < count; ++i) {
		vec.push_back(lcg());
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> res = end - start;

	return res.count();

}

double getSearchTime(std::vector<int> vec, int attempts) {
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < attempts; ++i) {

		auto item = find(vec.begin(), vec.end(), lcg());

	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> res = end - start;

	return res.count();
}

double getDeleteTime(std::vector<int> vec, int attempts) {
	auto start = std::chrono::high_resolution_clock::now();

	for (size_t i = 0; i < attempts; ++i) {

		int key = lcg();
		auto item = find(vec.begin(), vec.end(), key);
		if (item != vec.end()) {
			vec.erase(item);
		}

	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> res = end - start;

	return res.count();
}

void getTime(BinaryTree& binTree) {
	std::cout << "/////////////////////////////" << "Getting average time (binary tree)" << "/////////////////////////////\n";

	const size_t insertionAttempts = 100;
	const size_t searchAttempts = 1000;
	const size_t deletionAttempts = 1000;

	std::vector<size_t> sizes = { 1000, 10000, 100000 };

	for (size_t size : sizes) {

		double totalInsertionTime = 0.0;
		double totalSearchTime = 0.0;
		double totalDeletionTime = 0.0;

		for (size_t i = 0; i < insertionAttempts; ++i) {

			BinaryTree tree;
			totalInsertionTime += getInsertTime(tree, size);

		}

		for (size_t i = 0; i < size; ++i) {

			binTree.insert(lcg());

		}

		for (size_t i = 0; i < deletionAttempts; ++i) {

			totalSearchTime += getSearchTime(binTree, 1);

		}

		for (size_t i = 0; i < searchAttempts; ++i) {

			totalDeletionTime += getDeleteTime(binTree, 1);

		}

		double avgInsertionTime = totalInsertionTime / insertionAttempts;
		double avgSearchTime = totalSearchTime / searchAttempts;
		double avgDeletionTime = totalDeletionTime / deletionAttempts;

		std::cout << "\n\nSize: " << size << "\n\n";

		std::cout << "Avg insertion time: " << avgInsertionTime << "\n";
		std::cout << "Avg search time: " << avgSearchTime << "\n";
		std::cout << "Avg deletion time: " << avgDeletionTime << "\n";
	}
}

void getTime(std::vector<int> vec) {
	std::cout << "/////////////////////////////" << "Getting average time (binary tree)" << "/////////////////////////////\n";

	const size_t insertionAttempts = 100;
	const size_t searchAttempts = 1000;
	const size_t deletionAttempts = 1000;

	std::vector<size_t> sizes = { 1000, 10000, 100000 };

	for (size_t size : sizes) {

		double totalInsertionTime = 0.0;
		double totalSearchTime = 0.0;
		double totalDeletionTime = 0.0;

		for (size_t i = 0; i < insertionAttempts; ++i) {

			std::vector<int> vec1;
			totalInsertionTime += getInsertTime(vec1, size);

		}

		for (size_t i = 0; i < size; ++i) {

			vec.push_back(lcg());

		}

		for (size_t i = 0; i < deletionAttempts; ++i) {

			totalSearchTime += getSearchTime(vec, 1);

		}

		for (size_t i = 0; i < searchAttempts; ++i) {

			totalDeletionTime += getDeleteTime(vec, 1);

		}

		double avgInsertionTime = totalInsertionTime / insertionAttempts;
		double avgSearchTime = totalSearchTime / searchAttempts;
		double avgDeletionTime = totalDeletionTime / deletionAttempts;

		std::cout << "\n\nSize: " << size << "\n\n";

		std::cout << "Avg insertion time: " << avgInsertionTime << "\n";
		std::cout << "Avg search time: " << avgSearchTime << "\n";
		std::cout << "Avg deletion time: " << avgDeletionTime << "\n";
	}
}

std::vector<int> getElements(const BinaryTree& tree) {
	std::vector<int> elements;

	BinaryTree temp(tree);

	while (true) {
		if (!temp.getRoot()) break;

		int minVal = temp.findMin();
		elements.push_back(minVal);
		temp.erase(minVal);
	}
	return elements;
}

// Объединение двух множеств
std::vector<int> unionSet(const BinaryTree& a, const BinaryTree& b) {
	BinaryTree result = a;
	auto bElements = getElements(b);
	for (int num : bElements) {
		result.insert(num);
	}
	return getElements(result);
}

// Пересечение двух множеств
std::vector<int> intersection(const BinaryTree& a, const BinaryTree& b) {
	std::vector<int> result;
	auto aElements = getElements(a);
	for (int num : aElements) {
		if (b.contains(num)) {
			result.push_back(num);
		}
	}
	return result;
}

int main() {
	BinaryTree tree, tree1;

	tree.insert(1403);
	tree.insert(1877);
	tree.insert(1878);
	tree.insert(1905);
	tree.insert(1914);

	tree1.insert(1877);
	tree1.insert(1914);
	tree1.insert(988);
	tree1.insert(2022);

	std::cout << "Tree: ";
	tree.print();

	BinaryTree copyTree(tree);
	std::cout << "Copy Tree: ";
	copyTree.print();

	std::cout << "Contains 1877? " << (tree.contains(1877) ? "Yes" : "No") << "\n";
	std::cout << "Contains 1234? " << (tree.contains(1234) ? "Yes" : "No") << "\n";

	tree.erase(1403);
	std::cout << "Tree after erase 1403: ";
	tree.print();

	BinaryTree testTree;
	getTime(testTree);
	std::vector<int> testVec;
	getTime(testVec);

	auto uni = unionSet(tree, tree1);
	auto inter = intersection(tree, tree1);

	std::cout << "\nUnion vector: ";
	for (size_t i = 0; i < uni.size(); ++i) {
		std::cout << uni[i] << " ";
	}

	std::cout << "\nIntersection vector: ";
	for (size_t i = 0; i < inter.size(); ++i) {
		std::cout << inter[i] << " ";
	}

	return 0;
}