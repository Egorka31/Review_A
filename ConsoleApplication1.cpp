#include <iostream>
#include <iomanip> 
#include <vector>
#include <queue>
#include <algorithm>
class graph {
	int amount_of_cities, amount_of_roads, amount_of_sons;
	std::vector<int> flow, capacity, c, head, nxt, edge_reverse, edge_ind, edge, parent1, parent2, potentials, dist_to_node;
	std::vector<bool> mark;
	int fre = 0, f = 0, sum = 0;
public:
	void findpath(int current) {
		mark[current] = true;
		int ind = head[current];
		while (ind != -1) {
			if (!mark[edge[ind]]) {
				if (flow[ind] == capacity[ind] && capacity[ind] > 0) {
					parent1[edge[ind]] = current;
					parent2[edge[ind]] = edge_reverse[ind];
					findpath(edge[ind]);
				}
			}
			ind = nxt[ind];
		}
	}

	void writepath(int current, int sum) {
		if (parent1[current] != -1) {
			writepath(parent1[current], sum + 1);
			flow[parent2[current]] -= 1;
			int ind_norm = edge_reverse[parent2[current]];
			flow[ind_norm] += 1;
			std::cout << edge_ind[parent2[current]] << " ";
			return;
		}
		std::cout << sum << std::endl;
	}

	bool count_dist() {
		for (size_t i = 0; i < amount_of_cities; ++i) {
			potentials[i] = dist_to_node[i];
			dist_to_node[i] = 1e9;
			parent1[i] = -1;
			parent2[i] = -1;
		}
		dist_to_node[0] = 0;
		std::priority_queue<std::pair<int, int>> distance_queue;
		distance_queue.push(std::make_pair(0, 0));
		while (!distance_queue.empty()) {
			int v = distance_queue.top().second;
			int dist = -distance_queue.top().first;
			distance_queue.pop();
			if (dist_to_node[v] == dist) {
				int ind = head[v];
				while (ind != -1) {
					if (flow[ind] < capacity[ind]) {
						int dist_to = dist_to_node[v] + c[ind] + potentials[v] - potentials[edge[ind]];
						if (dist_to < dist_to_node[edge[ind]]) {
							dist_to_node[edge[ind]] = dist_to;
							parent1[edge[ind]] = v;
							parent2[edge[ind]] = edge_reverse[ind];
							distance_queue.push(std::make_pair(-dist_to, edge[ind]));
						}
					}
					ind = nxt[ind];
				}
			}
		}
		return (dist_to_node[amount_of_cities - 1] != 1e9);
	}

	int upd(int v) {
		int cur_c = 0;
		while (parent1[v] != -1) {
			cur_c += c[edge_reverse[parent2[v]]];
			flow[edge_reverse[parent2[v]]]++;
			flow[parent2[v]]--;

			v = parent1[v];
		}

		return cur_c;
	}

	void padding(int x, int w, int y, bool tpr, int i) {
		edge[fre] = x - 1;
		c[fre] = w;
		if (tpr) {
			capacity[fre] = 1;
			edge_reverse[fre] = fre + 1;
		}
		else {
			edge_reverse[fre] = fre - 1;
		}
		edge_ind[fre] = i + 1;
		nxt[fre] = head[y - 1];
		head[y - 1] = fre;
		++fre;
	}

	void read() {
		std::cin >> amount_of_cities >> amount_of_roads >> amount_of_sons;

		flow.resize(4 * amount_of_roads + 1);
		capacity.resize(4 * amount_of_roads + 1);
		c.resize(4 * amount_of_roads + 1);
		head.resize(4 * amount_of_roads + 1);
		nxt.resize(4 * amount_of_roads + 1);
		edge_reverse.resize(4 * amount_of_roads + 1);
		edge_ind.resize(4 * amount_of_roads + 1);
		edge.resize(4 * amount_of_roads + 1);
		mark.resize(4 * amount_of_roads + 1);
		parent1.resize(4 * amount_of_roads + 1);
		parent2.resize(4 * amount_of_roads + 1);
		potentials.resize(4 * amount_of_roads + 1);
		dist_to_node.resize(4 * amount_of_roads + 1);

		for (size_t i = 0; i < amount_of_cities; i++) {
			head[i] = -1;
		}

		for (size_t i = 0; i < amount_of_roads; i++) {
			int x, y, w;
			std::cin >> x >> y >> w;
			padding(y, w, x, true, i);
			padding(x, -w, y, false, i);
			padding(x, w, y, true, i);
			padding(y, -w, x, false, i);
		}
	}

	void solution(int &sum, int &f) {
		while (count_dist()) {
			sum += upd(amount_of_cities - 1);
			++f;
			if (f == amount_of_sons)
				break;
		}
	}

	void output(int &f, int &sum) {
		if (f < amount_of_sons) {
			std::cout << -1 << std::endl;
		}
		else {
			std::cout << std::fixed << std::setprecision(6) << sum * 1.0 / amount_of_sons << std::endl;
			for (size_t i = 0; i < amount_of_sons; ++i) {
				for (size_t j = 0; j < amount_of_cities; ++j) {
					mark[j] = false;
					parent1[j] = -1;
					parent2[j] = -1;
				}
				findpath(0);
				writepath(amount_of_cities - 1, 0);
				std::cout << std::endl;
			}
		}
	}
	void solve() {
		read();
		solution(sum, f);
		output(f, sum);
	}
};

int main() {
	graph web;
	web.solve();
	return 0;
}
