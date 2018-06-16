#include <iostream>
#include <iomanip> 
#include <vector>
#include <queue>
#include <algorithm>

struct task {
	int amount_of_cities, amount_of_roads, amount_of_sons;
	std::vector<int> flow, capacity, c, head, nxt, edge_reverse, edge_ind, edge, parent1, parent2, potentials, dist_to_node;
	std::vector<bool> mark;
	int fre = 0;
};
void findpath(int current, task &web) {
	web.mark[current] = true;
	int ind = web.head[current];
	while (ind != -1) {
		if (!web.mark[web.edge[ind]]) {
			if (web.flow[ind] == web.capacity[ind] && web.capacity[ind] > 0) {
				web.parent1[web.edge[ind]] = current;
				web.parent2[web.edge[ind]] = web.edge_reverse[ind];
				findpath(web.edge[ind], web);
			}
		}
		ind = web.nxt[ind];
	}
}

void writepath(int current, int sum, task &web) {
	if (web.parent1[current] != -1) {
		writepath(web.parent1[current], sum + 1, web);
		web.flow[web.parent2[current]] -= 1;
		int ind_norm = web.edge_reverse[web.parent2[current]];
		web.flow[ind_norm] += 1;
		std::cout << web.edge_ind[web.parent2[current]] << " ";
		return;
	}
	std::cout << sum << std::endl;
}

bool count_dist(task& web) {
	for (size_t i = 0; i < web.amount_of_cities; ++i) {
		web.potentials[i] = web.dist_to_node[i];
		web.dist_to_node[i] = 1e9;
		web.parent1[i] = -1;
		web.parent2[i] = -1;
	}
	web.dist_to_node[0] = 0;
	std::priority_queue<std::pair<int, int>> distance_queue;
	distance_queue.push(std::make_pair(0, 0));
	while (!distance_queue.empty()) {
		int v = distance_queue.top().second;
		int dist = -distance_queue.top().first;
		distance_queue.pop();
		if (web.dist_to_node[v] == dist) {
			int ind = web.head[v];
			while (ind != -1) {
				if (web.flow[ind] < web.capacity[ind]) {
					int dist_to = web.dist_to_node[v] + web.c[ind] + web.potentials[v] - web.potentials[web.edge[ind]];
					if (dist_to < web.dist_to_node[web.edge[ind]]) {
						web.dist_to_node[web.edge[ind]] = dist_to;
						web.parent1[web.edge[ind]] = v;
						web.parent2[web.edge[ind]] = web.edge_reverse[ind];
						distance_queue.push(std::make_pair(-dist_to, web.edge[ind]));
					}
				}
				ind = web.nxt[ind];
			}
		}
	}
	return (web.dist_to_node[web.amount_of_cities - 1] != 1e9);
}

int upd(int v, task &web) {
	int cur_c = 0;
	while (web.parent1[v] != -1) {
		cur_c += web.c[web.edge_reverse[web.parent2[v]]];
		web.flow[web.edge_reverse[web.parent2[v]]]++;
		web.flow[web.parent2[v]]--;

		v = web.parent1[v];
	}

	return cur_c;
}

void padding(int x, int w, int y, bool tpr, int i, task &web) {
	web.edge[web.fre] = x - 1;
	web.c[web.fre] = w;
	if (tpr) {
		web.capacity[web.fre] = 1;
		web.edge_reverse[web.fre] = web.fre + 1;
	}
	else {
		web.edge_reverse[web.fre] = web.fre - 1;
	}
	web.edge_ind[web.fre] = i + 1;
	web.nxt[web.fre] = web.head[y - 1];
	web.head[y - 1] = web.fre;
	++web.fre;
}

void read(task &web) {
	std::cin >> web.amount_of_cities >> web.amount_of_roads >> web.amount_of_sons;

	web.flow.resize(10000);
	web.capacity.resize(10000);
	web.c.resize(10000);
	web.head.resize(10000);
	web.nxt.resize(10000);
	web.edge_reverse.resize(10000);
	web.edge_ind.resize(10000);
	web.edge.resize(10000);
	web.mark.resize(10000);
	web.parent1.resize(10000);
	web.parent2.resize(10000);
	web.potentials.resize(10000);
	web.dist_to_node.resize(10000);

	for (size_t i = 0; i < web.amount_of_cities; i++) {
		web.head[i] = -1;
	}

	for (size_t i = 0; i < web.amount_of_roads; i++) {
		int x, y, w;
		std::cin >> x >> y >> w;
		padding(y, w, x, true, i, web);
		padding(x, -w, y, false, i, web);
		padding(x, w, y, true, i, web);
		padding(y, -w, x, false, i, web);
	}
}
void solution(int &sum, int &f, task &web) {
	while (count_dist(web)) {
		sum += upd(web.amount_of_cities - 1, web);
		++f;
		if (f == web.amount_of_sons)
			break;
	}
}
void output(int &f, task &web, int &sum) {
	if (f < web.amount_of_sons) {
		std::cout << -1 << std::endl;
	}
	else {
		std::cout << std::fixed << std::setprecision(6) << sum * 1.0 / web.amount_of_sons << std::endl;
		for (size_t i = 0; i < web.amount_of_sons; ++i) {
			for (size_t j = 0; j < web.amount_of_cities; ++j) {
				web.mark[j] = false;
				web.parent1[j] = -1;
				web.parent2[j] = -1;
			}
			findpath(0, web);
			writepath(web.amount_of_cities - 1, 0, web);
			std::cout << std::endl;
		}
	}
}
int main() {
	task web;
	read(web);
	int f = 0, sum = 0;
	solution(sum, f, web);
	output(f, web, sum);
	return 0;
}
