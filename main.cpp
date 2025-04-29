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

		Edge() = default;

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

	bool remove_vertex(const Vertex& v) {
		if (!has_vertex(v)) return false;
		_vertices.erase(v);
		_edges.erase(v);

		for (auto i = _edges.begin(); i != _edges.end(); ) {
			if (i->second.to == v) i = _edges.erase(i);
			else ++i;
		}
		return true;
	}

	std::vector<Vertex> vertices() const {
		return std::vector<Vertex>(_vertices.begin(), _vertices.end());
	}

	void add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
		if (!has_vertex(from) || !has_vertex(to)) {
			throw std::invalid_argument("One or both vertices do not exists");
		}
		_edges.emplace(from, Edge(from, to, d));
	}

	bool remove_edge(const Vertex& from, const Vertex& to) {
		if (!has_edge(from, to)) return false;
		auto range = _edges.equal_range(from);
		for (auto i = range.first; i != range.second; ) {
			if (i->second.to == to) {
				i = _edges.erase(i);
				return true;
			}
			else ++i;
		}
		return false;
	}

	bool remove_edge(const Edge& e) {
		auto range = _edges.equal_range(e.from);
		for (auto i = range.first; i != range.second; ++i) {
			if (i->second == e) {
				_edges.erase(i);
				return true;
			}
		}
		return false;
	}

	bool has_edge(const Vertex& from, const Vertex& to) const {
		if (!has_vertex(from) || !has_vertex(to)) {
			return false;
		}
		auto range = _edges.equal_range(from);
		return std::any_of(range.first, range.second, [&to](const auto& pair) { return pair.second.to == to; });
	}

	bool has_edge(const Edge& e) const {
		if (!has_vertex(e.from) || !has_vertex(e.to)) {
			return false;
		}
		auto range = _edges.equal_range(e.from);
		return std::any_of(range.first, range.second, [&e](const auto& pair) { return pair.second == e; });
	}

	std::vector<Edge> edges(const Vertex& vertex) {
		if (!has_vertex(vertex)) {
			throw std::invalid_argument("Vertex does not exist");
		}
		std::vector<Edge> result;
		auto range = _edges.equal_range(vertex);
		std::transform(range.first, range.second, std::back_inserter(result), [](const auto& pair) { return pair.second; });
		return result;
	}

	size_t order() const {
		return _vertices.size();
	}

	size_t degree(const Vertex& v) const {
		if (!has_vertex(v)) {
			throw std::invalid_argument("Vertex does not exist");
		}
		return _edges.count(v);
	}

	bool is_connected() const {
		if (_vertices.empty()) {
			return true;
		}

		auto all_vertices = vertices();
		Vertex start = all_vertices.front();

		// Проверяем достижимость всех вершин из start
		if (!is_reachable_from(start)) {
			return false;
		}

		// Создаем транспонированный граф
		auto transposed = create_transposed();

		// Проверяем достижимость всех вершин в транспонированном графе
		return transposed->is_reachable_from(start);
	}

	std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
		if (!has_vertex(from) || !has_vertex(to)) {
			throw std::invalid_argument("One or both vertices do not exist");
		}

		auto [distances, predecessors] = bellman_ford(from);

		// Проверка на отрицательные циклы
		if (has_negative_cycle(distances)) {
			throw std::runtime_error("Graph contains a negative-weight cycle");
		}

		// Восстановление пути
		if (distances.at(to) == std::numeric_limits<Distance>::max()) {
			return {};
		}

		return reconstruct_path(from, to, predecessors);
	}

	std::vector<Vertex> walk(const Vertex& start_vertex) const {
		if (!has_vertex(start_vertex)) {
			throw std::invalid_argument("Vertex does not exist");
		}

		std::vector<Vertex> result;
		std::unordered_set<Vertex> visited;
		std::stack<Vertex> stack;
		stack.push(start_vertex);

		while (!stack.empty()) {
			Vertex current = stack.top();
			stack.pop();

			if (visited.insert(current).second) {
				result.push_back(current);

				// Получаем соседей в обратном порядке для правильного обхода
				auto neighbors = get_neighbors(current);
				std::reverse(neighbors.begin(), neighbors.end());
				for (const auto& neighbor : neighbors) {
					if (visited.find(neighbor) == visited.end()) {
						stack.push(neighbor);
					}
				}
			}
		}

		return result;
	}

	Vertex find_optimal_warehouse() const {
		if (!is_connected()) {
			throw std::runtime_error("Graph is not connected");
		}

		auto all_vertices = vertices();
		std::vector<std::pair<Vertex, Distance>> avg_distances;

		std::transform(all_vertices.begin(), all_vertices.end(), std::back_inserter(avg_distances), [this](const Vertex& v) { return std::make_pair(v, calculate_average_distance(v)); });

		return std::min_element(avg_distances.begin(), avg_distances.end(), [](const auto& a, const auto& b) { return a.second < b.second; })->first;
	}

	void print_vertices() const {
		std::cout << "Vertices (" << order() << "):\n";
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

	void print() const {
		std::cout << "\nGraph (" << order() << " vertices, " << _edges.size() << " edges):\n";
		print_vertices();
		print_edges();
	}

	Distance calculate_average_distance(const Vertex& v) const {
		Distance total = Distance();
		size_t count = 0;

		for (const auto& u : _vertices) {
			if (v == u) continue;

			auto path = shortest_path(v, u);
			if (!path.empty()) {
				total += std::accumulate(path.begin(), path.end(), Distance(),
					[](Distance sum, const Edge& e) { return sum + e.distance; });
				count++;
			}
		}

		return count > 0 ? total / count : std::numeric_limits<Distance>::max();
	}

private:
	std::vector<Vertex> get_neighbors(const Vertex& v) const {
		std::vector<Vertex> neighbors;
		auto range = _edges.equal_range(v);
		std::transform(range.first, range.second, std::back_inserter(neighbors),
			[](const auto& pair) { return pair.second.to; });
		return neighbors;
	}

	bool is_reachable_from(const Vertex& start) const {
		std::unordered_set<Vertex> visited;
		std::stack<Vertex> stack;
		stack.push(start);

		while (!stack.empty()) {
			Vertex current = stack.top();
			stack.pop();

			if (visited.insert(current).second) {
				for (const auto& neighbor : get_neighbors(current)) {
					if (visited.find(neighbor) == visited.end()) {
						stack.push(neighbor);
					}
				}
			}
		}

		return visited.size() == _vertices.size();
	}

	std::unique_ptr<Graph> create_transposed() const {
		auto transposed = std::make_unique<Graph>();

		for (const auto& v : _vertices) {
			transposed->add_vertex(v);
		}

		for (const auto& [from, edge] : _edges) {
			transposed->add_edge(edge.to, edge.from, edge.distance);
		}

		return transposed;
	}

	std::pair<std::unordered_map<Vertex, Distance>, std::unordered_map<Vertex, Edge>> bellman_ford(const Vertex& start) const {
		std::unordered_map<Vertex, Distance> distances;
		std::unordered_map<Vertex, Edge> predecessors;

		for (const auto& v : _vertices) {
			distances[v] = std::numeric_limits<Distance>::max();
		}
		distances[start] = Distance();

		// Релаксация ребер
		for (size_t i = 1; i < _vertices.size(); ++i) {
			for (const auto& [from, edge] : _edges) {
				if (distances[from] != std::numeric_limits<Distance>::max() &&
					distances[edge.to] > distances[from] + edge.distance) {
					distances[edge.to] = distances[from] + edge.distance;
					predecessors[edge.to] = edge;
				}
			}
		}

		return { distances, predecessors };
	}

	bool has_negative_cycle(const std::unordered_map<Vertex, Distance>& distances) const {
		for (const auto& [from, edge] : _edges) {
			if (distances.at(from) != std::numeric_limits<Distance>::max() &&
				distances.at(edge.to) > distances.at(from) + edge.distance) {
				return true;
			}
		}
		return false;
	}

	std::vector<Edge> reconstruct_path(const Vertex& from, const Vertex& to, const std::unordered_map<Vertex, Edge>& predecessors) const {
		std::vector<Edge> path;
		Vertex current = to;

		while (current != from) {
			auto it = predecessors.find(current);
			if (it == predecessors.end()) {
				return {};
			}
			path.push_back(it->second);
			current = it->second.from;
		}

		std::reverse(path.begin(), path.end());
		return path;
	}
};

int main() {
	Graph<std::string> graph;
	graph.add_vertex("A");
	graph.add_vertex("B");
	graph.add_vertex("C");
	graph.add_vertex("D");
	graph.add_edge("A", "B", 2.0);
	graph.add_edge("B", "C", 3.0);
	graph.add_edge("A", "C", 5.0);
	graph.add_edge("A", "D", 1.0);
	graph.add_edge("D", "C", 4.1);
	graph.add_edge("C", "A", 5.2);
	graph.print();

	graph.remove_vertex("D");
	graph.remove_edge("C", "A");
	graph.print();

	std::cout << "\nGraph has 'A'? : " << (graph.has_vertex("A") ? "Yes\n" : "No\n");
	std::cout << "Graph has 'D'? : " << (graph.has_vertex("D") ? "Yes\n" : "No\n");
	std::cout << "Edge A->B exists? : " << (graph.has_edge("A", "B") ? "Yes\n" : "No\n");
	std::cout << "Edge B->A exists? : " << (graph.has_edge("B", "A") ? "Yes\n" : "No\n");
	std::cout << "Degree of A: " << graph.degree("A") << "\n";

	auto edges_from_A = graph.edges("A");
	std::cout << "\n-----Test edges()-----\nEdges from A:\n";
	for (const auto& e : edges_from_A) {
		std::cout << "- " << e.from << "->" << e.to << " (d = " << e.distance << ")\n";
	}
	
	graph.add_vertex("D");
	graph.add_edge("A", "D", 0.1);
	graph.add_edge("D", "C", 4.1);
	graph.print();
	auto path = graph.shortest_path("A", "C");
	std::cout << "\n-----Test shortest_path()-----\nPath from A to C:\n";
	for (const auto& e : path) {
		std::cout << "- " << e.from << "->" << e.to << " (d = " << e.distance << ")\n";
	}

	auto visited = graph.walk("A");
	std::cout << "\n-----Test walk()-----\nVisited vertices: ";
	for (const auto& v : visited) std::cout << v << " ";
	std::cout << "\n";

	graph.add_edge("C", "A", 5.2);
	graph.add_edge("C", "B", 3.7);
	graph.add_edge("C", "A", 5.2);
	std::cout << "\n\n=== Trade Network ===" << std::endl;
	graph.print();
	std::cout << "\nThe graph is connected: " << std::boolalpha
		<< graph.is_connected() << std::endl;

	// 3. Решение задачи (с использованием calculate_average_distance)
	std::cout << "\n=== Finding the optimal location for a warehouse ===" << std::endl;
	try {
		std::string best_location = graph.find_optimal_warehouse();
		std::cout << "The best place for a warehouse: " << best_location << std::endl;

		// Проверка средних расстояний через метод класса
		std::cout << "\nAverage distances:" << std::endl;
		for (const auto& point : graph.vertices()) {
			double avg_dist = graph.calculate_average_distance(point);
			std::cout << "- " << point << ": " << avg_dist << std::endl;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}
