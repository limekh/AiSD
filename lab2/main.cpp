#include <iostream>

template <typename T>
class LinkedList {

	struct Node {
		T data;
		Node* next;
		Node* prev;

		Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
	};
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
			push_tail(std::rand() % 1000);
		}
	}

	~LinkedList() {
		while (head) {
			pop_head();
		}
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
		if (!list) return;
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
				if (for_delete->next) for_delete->prev->next = for_delete->next;
				if (for_delete->prev) for_delete->next->prev = for_delete->prev;
				if (for_delete == head) head = for_delete->next;
				if (for_delete == tail) tail = for_delete->prev;
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
};

int main() {



	return 0;
}