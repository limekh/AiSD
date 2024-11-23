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
};

int main() {



	return 0;
}