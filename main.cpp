#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <stack>
#include <queue>
#include <iostream>

template <typename Vertex, typename Distance = double>
class Graph {
public:
	struct Edge {
		Vertex from;
		Vertex to;
		Distance distance;

		Edge(const Vertex& f, const Vertex& t, const Distance& d) : from(f), to(t), distance(d) {}

		bool operator==(const Edge& other) const {
			return from == other.from && to == other.to && distance == other.distance;
		}
	};
private:
	std::unordered_set<Vertex> _vertices;
	std::unordered_multimap<Vertex, Edge> _edges;
public:
	bool has_vertex(const Vertex& v) const {
		return _vertices.find(v) != _vertices.end();
	}

	bool add_vertex(const Vertex& v) {
		return _vertices.insert(v).second;
	}

	void add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
		if (!has_vertex(from) || !has_vertex(to)) {
			throw std::invalid_argument("One or both vertices do not exists");
		}
		_edges.emplace(from, Edge(from, to, d));
	}

	void print_vertices() const {
		std::cout << "Vertices (" << _vertices.size() << "):\n";
		for (const auto& v : _vertices) {
			std::cout << "- " << v << "\n";
		}
	}

	void print_edges() const {
		std::cout << "Edges (" << _edges.size() << "):\n";
		for (const auto& [from, edge] : _edges) {
			std::cout << "- " << edge.from << " -> " << edge.to
				<< " (distance: " << edge.distance << ")\n";
		}
	}

	size_t order() const {
		return _vertices.size();
	}

	void print() const {
		std::cout << "Graph (" << order() << " vertices, " << _edges.size() << " edges):\n";
		print_vertices();
		print_edges();
	}

};

int main() {
	Graph<std::string> graph;
	graph.add_vertex("A");
	graph.add_vertex("B");
	graph.add_vertex("C");
	graph.add_edge("A", "B", 2.0);
	graph.add_edge("B", "C", 3.0);
	graph.add_edge("A", "C", 5.0);
	
	graph.print();

	return 0;
}