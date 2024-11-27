#include <iostream>

template <typename T>
struct Node {
	T data;
	Node* next;
	Node* prev;

	Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T>
class LinkedList {
	Node<T>* head;
	Node<T>* tail;
public:
	LinkedList() : head(nullptr), tail(nullptr) {}
	LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr) {
		Node<T>* p = other.head;
		while (p) {
			push_tail(p->data);
			p = p->next;
		}
	}

	LinkedList(size_t size, unsigned seed) : head(nullptr), tail(nullptr) {
		std::srand(seed);
		for (size_t i = 0; i < size; ++i) {
			push_tail(std::rand() % 10);
		}
	}

	~LinkedList() {
		while (head) {
			pop_head();
		}
	}

	Node<T>* get_head() const{
		return head;
	}
	Node<T>* get_tail() const{
		return tail;
	}

	void push_tail(const T& value) {
		Node<T>* new_node = new Node<T>(value);
		if (!tail) {
			head = new_node;
			tail = new_node;
		}
		else {
			tail->next = new_node;
			new_node->prev = tail;
			tail = new_node;
		}
	}

	void push_tail(const LinkedList& other) {
		Node<T>* current = other.head;
		while (current) {
			push_tail(current->data);
			current = current->next;
		}
	}

	void push_head(const T& value) {
		if (value == 0) return;
		Node<T>* newNode = new Node<T>(value);
		if (!head) {
			head = tail = newNode;
		}
		else {
			newNode->next = head;
			head->prev = newNode;
			head = newNode;
		}
	}

	void push_head(const LinkedList& other) {
		Node<T>* current = other.tail;
		while (current) {
			push_head(current->data);
			current = current->prev;
		}
	}

	void pop_head() {
		if (!head) return;
		Node<T>* temp = head;
		head = head->next;
		if (head) head->prev = nullptr;
		else tail = nullptr;
		delete temp;
	}

	void pop_tail() {
		if (!tail) return;
		Node<T>* temp = tail;
		tail = tail->prev;
		if (tail) tail->next = nullptr;
		else head = nullptr;
		delete temp;
	}

	void delete_node(const T& value) {
		Node<T>* p = head;
		while (p) {
			if (p->data == value) {
				Node<T>* for_delete = p;
				if (p->prev) p->prev->next = p->next;
				if (p->next) p->next->prev = p->prev;
				
				if (p == head) head = p->next;
				if (p == tail) tail = p->prev;
				p = p->next;
				delete for_delete;
			}
			else p = p->next;
		}
	}

	LinkedList& operator=(const LinkedList& other){
		if (this != other) {
			while (head) {
				pop_head();
			}
			Node<T>* p = other.head;
			while (p) {
				push_tail(p->data);
				p = p->next;
			}
		}
		return *this;
	}

	T& operator[](size_t index) {
		Node<T>* p = head;
		for (size_t i = 0; i < index; ++i) {
			if (!p) throw std::out_of_range("Wrong Index");
			p = p->next;
		}
		return p->data;
	}

	template <typename T>
	friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
		Node<T>* p = list.head;
		while (p) {
			os << p->data << " ";
			p = p->next;
		}
		os << "\n";
		return os;
	}
};

template <typename T>
LinkedList<T> sum_list(const LinkedList<T>& list, const LinkedList<T>& other) {
	LinkedList<T> result;
	Node<T>* p1 = list.get_tail();
	Node<T>* p2 = other.get_tail();

	int carry = 0;
	while (p1 || p2 || carry) {
		int sum = carry;
		if (p1) {
			sum += p1->data;
			p1 = p1->prev;
		}
		if (p2) {
			sum += p2->data;
			p2 = p2->prev;
		}
		result.push_head(sum % 10);
		carry = sum / 10;
	}
	return result;
}

int main() {

	LinkedList<int> list = LinkedList<int>(3, 1);
	LinkedList<int> copy = list;
	std::cout << "list: " << list;
	std::cout << "copy: " << copy;

	list.push_tail(3);
	list.push_tail(1);
	list.push_head(4);
	list.push_head(0);
	std::cout << "push_head and push_list: " << list;

	list.pop_head();
	list.pop_tail();
	std::cout << "pop_head and pop_list: " << list;

	list.delete_node(3);
	std::cout << "delete_node: " << list;

	std::cout << "SUM equals: " << sum_list(list, copy);

	return 0;
}