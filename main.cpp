#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

std::string generateRandomRoman();
int romanToDecimal(const std::string& roman);

template<typename K, typename T>
class HashTable {
private:
	struct Node {
		K key;
		T value;
		Node* next;

		Node(const K& k, const T& v, Node* n) : key(k), value(v), next(n) {}
	};

	Node** array;
	size_t size;
public:
	HashTable(size_t size) : size(size) {
		array = new Node * [size];
		for (size_t i = 0; i < size; ++i) {
			array[i] = nullptr;
		}
	}

	HashTable(size_t table_size, size_t num_elements) : HashTable(table_size) {
		if constexpr (std::is_same_v<K, std::string> && std::is_same_v<T, int>) {
			for (size_t i = 0; i < num_elements; ++i) {
				std::string roman = generateRandomRoman();
				int decimal = romanToDecimal(roman);
				insert(roman, decimal);
			}
		}
	}

	~HashTable() {
		clear();
		delete[] array;
	}

	HashTable(const HashTable& other) {
		array = new Node * [size];
		for (size_t i = 0; i < size; ++i) {
			array[i] = nullptr;
			Node* current = other.array[i];
			while (current) {
				insert(current->key, current->value);
				current = current->next;
			}
		}
	}

	size_t hash_function(const K& key) const {
		size_t a = 3795218735u;

		double hashValue = (static_cast<double>(a) / 32) * key;
		hashValue -= static_cast<int>(hashValue);
		return static_cast<size_t>(size * hashValue) % size;
	}

	void insert(K k, const T& val) {
		size_t index = hash_function(k);
		Node* p = array[index];
		while (p) {
			if (p->key == k) throw("Keys equals!");
			p = p->next;
		}
		array[index] = new Node(k, val, array[index]);
	}

	T& operator[](K k) {
		size_t index = hash_function(k);
		Node* p = array[index];
		while (p && p->key != k) p = p->next;
		if (!p) throw("key not found");
		else return p->value;
	}

	void print() {
		for (size_t i = 0; i < size; ++i) {
			std::cout << "Bucket " << i << ": ";
			Node* p = array[i];
			while (p) {
				std::cout << "[" << p->key << ": " << p->value << "] ";
				p = p->next;
			}
			std::cout << "\n";
		}
	}

	void insert_or_assign(K k, T& val) {
		size_t index = hash_function(k);
		Node* p = array[index];
		while (p) {
			if (p->key == k) {
				p->value = val;
				return;
			}
			p = p->next;
		}
		array[index] = new Node(k, val, array[index]);
	}

	bool contains(T& val) {
		for (size_t i = 0; i < size; ++i) {
			Node* p = array[i];
			while (p) {
				if (p->value == val) return true;
				p = p->next;
			}
		}
		return false;
	}

	T* search(K key) {
		size_t index = hash_function(key);
		Node* p = array[index];
		while (p) {
			if (p->key == key) return &(p->value);
			p = p->next;
		}
		return nullptr;
	}

	bool erase(K k) {
		size_t index = hash_function(k);
		Node* p = array[index];
		Node* prev = nullptr;

		while (p) {
			if (p->key == k) {
				if (prev) {
					prev->next = p->next;
				}
				else {
					array[index] = p->next;
				}
				delete p;
				return true;
			}
			prev = p;
			p = p->next;
		}
		return false;
	}

	int count(K k) {
		size_t index = hash_function(k);
		int cnt = 0;
		Node* p = array[index];
		while (p) {
			++cnt;
			p = p->next;
		}
		return cnt;
	}

	void clear() {
		for (size_t i = 0; i < size; ++i) {
			Node* p = array[i];
			while (p) {
				Node* temp = p;
				p = p->next;
				delete temp;
			}
			array[i] = nullptr;
		}
	}

	HashTable& operator=(const HashTable& other) {
		if (this != &other) {
			clear();
			delete[] array;

			size = other.size;
			array = new Node * [size];

			for (size_t i = 0; i < size; ++i) {
				array[i] = nullptr;
				Node* current = other.array[i];
				Node* prev = nullptr;

				while (current) {
					Node* newNode = new Node(current->key, current->value, nullptr);

					if (prev) {
						prev->next = newNode;
					}
					else {
						array[i] = newNode;
					}

					prev = newNode;
					current = current->next;
				}
			}
		}
		return *this;
	}
};

int romanCharToValue(char c) {
	switch (toupper(c)) {
		case 'I': return 1;
		case 'V': return 5;
		case 'X': return 10;
		case 'L': return 50;
		case 'C': return 100;
		case 'D': return 500;
		case 'M': return 1000;
		default: throw std::invalid_argument(std::string("Invalid Roman numeral") + c);
	}
}

int romanToDecimal(const std::string& roman) {
	if (roman.empty()) {
		throw std::invalid_argument("Empty Roman numeral string");
	}

	int total = 0;
	int prevValue = 0;

	for (int i = roman.length() - 1; i >= 0; i--) {
		char currentChar = roman[i];
		int currentValue = romanCharToValue(currentChar);

		if (currentValue < prevValue) {
			total -= currentValue;
		}
		else {
			total += currentValue;
		}

		prevValue = currentValue;
	}

	return total;
}

std::string generateRandomRoman() {
	static const std::vector<std::pair<int, std::string>> values = {
		{1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
		{100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
		{10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
	};

	int num = 1 + rand() % 3999;
	std::string roman;

	for (const auto& [val, sym] : values) {
		while (num >= val) {
			roman += sym;
			num -= val;
		}
	}
	return roman;
}

template <>
size_t HashTable<std::string, int>::hash_function(const std::string& key) const {
	int decimalValue = romanToDecimal(key);
	size_t a = 3795218735u;

	double hashValue = (static_cast<double>(a) / 32) * decimalValue;
	hashValue -= static_cast<int>(hashValue);
	return static_cast<size_t>(size * hashValue) % size;
}

int main() {

	try {
		HashTable<int, std::string> ht(10);

		std::cout << "=== Testing insert ===" << std::endl;
		ht.insert(1, "one");
		ht.insert(2, "two");
		ht.insert(11, "eleven");
		ht.insert(12, "twelve");
		ht.print();

		std::cout << "\n=== Testing operator[] ===" << std::endl;
		std::cout << "ht[1] = " << ht[1] << std::endl;
		std::cout << "ht[2] = " << ht[2] << std::endl;

		std::cout << "\n=== Testing insert_or_assign ===" << std::endl;
		std::string one = "ONE";
		std::string three = "three";

		ht.insert_or_assign(1, one);
		ht.insert_or_assign(3, three);
		ht.print();

		std::cout << "\n=== Testing contains ===" << std::endl;
		std::string four = "four";

		std::cout << "Contains 'ONE': " << (ht.contains(one) ? "true" : "false") << std::endl;
		std::cout << "Contains 'four': " << (ht.contains(four) ? "true" : "false") << std::endl;


		std::cout << "\n=== Testing search ===" << std::endl;
		std::string* val = ht.search(2);
		if (val) {
			std::cout << "Found value for key 2: " << *val << std::endl;
		}
		else {
			std::cout << "Key 2 not found" << std::endl;
		}

		std::cout << "\n=== Testing count ===" << std::endl;
		std::cout << "Elements in bucket for key 1: " << ht.count(1) << std::endl;

		std::cout << "\n=== Testing erase ===" << std::endl;
		std::cout << "Erase key 1: " << (ht.erase(1) ? "success" : "failed") << std::endl;
		std::cout << "Erase key 5: " << (ht.erase(5) ? "success" : "failed") << std::endl;
		ht.print();

		std::cout << "\n=== Testing clear ===" << std::endl;
		ht.clear();
		ht.print();

		std::cout << "\n=== Testing operator= ===" << "\n";
		HashTable<int, std::string> ht1(5);
		ht1.insert(1, "one");
		ht1.insert(2, "two");

		HashTable<int, std::string> ht2(3);
		ht2 = ht1;
		ht1.print();
		ht2.print();

		std::cout << "\n=== Testing Roman Task ===" << "\n";
		std::srand(static_cast<unsigned>(std::time(nullptr)));

		try {
			HashTable<std::string, int> romanNumbers(20, 10);
			std::cout << "Hash table create with 10 random values.\n";

			while (true) {
				std::cout << "\nEnter Roman number (or 'exit' for exit): ";
				std::string input;
				std::cin >> input;

				if (input == "exit") break;

				try {
					int decimal = romanToDecimal(input);
					std::cout << "Decimal value: " << decimal << std::endl;

					try {
						romanNumbers.insert(input, decimal);
						std::cout << "Add in Hash table.\n";
					}
					catch (const std::exception& e) {
						std::cout << "Error to add: " << e.what() << "\n";
					}

				}
				catch (const std::exception& e) {
					std::cout << "Error: " << e.what() << "\n";
					std::cout << "Roman number exists only: I, V, X, L, C, D, M\n";
				}
			}

			std::cout << "\nFinal Hash table:\n";
			romanNumbers.print();

		}
		catch (const std::exception& e) {
			std::cerr << "Critical error: " << e.what() << std::endl;
			return 1;
		}

	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
