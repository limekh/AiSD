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
};

int main() {



	return 0;
}