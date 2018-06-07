#include <iostream>
#include <iomanip> 
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int n, m, k;
const int N = 10000;
vector<int> flow, capacity, c, head, nxt, edge_reverse, edge_ind, edge, parent1, parent2, p, d;
vector<bool> mark;
int fre = 0;

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
		cout << edge_ind[parent2[current]] << " ";
		return;
	}
	cout << sum << endl;
}

bool count_dist() {
	for (int i = 0; i < n; ++i) {
		p[i] = d[i];
		d[i] = 1e9;
		parent1[i] = -1;
		parent2[i] = -1;
	}
	d[0] = 0;
	priority_queue<pair<int, int>> q;
	q.push(make_pair(0, 0));
	while (!q.empty()) {
		int v = q.top().second;
		int dist = -q.top().first;
		q.pop();
		if (d[v] == dist) {
			int ind = head[v];
			while (ind != -1) {
				if (flow[ind] < capacity[ind]) {
					int dist_to = d[v] + c[ind] + p[v] - p[edge[ind]];
					if (dist_to < d[edge[ind]]) {
						d[edge[ind]] = dist_to;
						parent1[edge[ind]] = v;
						parent2[edge[ind]] = edge_reverse[ind];
						q.push(make_pair(-dist_to, edge[ind]));
					}
				}
				ind = nxt[ind];
			}
		}
	}
	return (d[n - 1] != 1e9);
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

int main() {
	cin >> n >> m >> k;

	flow.resize(N);
	capacity.resize(N);
	c.resize(N);
	head.resize(N);
	nxt.resize(N);
	edge_reverse.resize(N);
	edge_ind.resize(N);
	edge.resize(N);
	mark.resize(N);
	parent1.resize(N);
	parent2.resize(N);
	p.resize(N);
	d.resize(N);
	for (int i = 0; i < n; i++) {
		head[i] = -1;
	}

	for (int i = 0; i < m; i++) {
		int x, y, w;
		cin >> x >> y >> w;

		edge[fre] = y - 1;
		c[fre] = w;
		capacity[fre] = 1;
		edge_ind[fre] = i + 1;
		edge_reverse[fre] = fre + 1;
		nxt[fre] = head[x - 1];
		head[x - 1] = fre;
		++fre;

		edge[fre] = x - 1;
		c[fre] = -w;
		edge_ind[fre] = i + 1;
		edge_reverse[fre] = fre - 1;
		nxt[fre] = head[y - 1];
		head[y - 1] = fre;
		++fre;

		edge[fre] = x - 1;
		c[fre] = w;
		capacity[fre] = 1;
		edge_ind[fre] = i + 1;
		edge_reverse[fre] = fre + 1;
		nxt[fre] = head[y - 1];
		head[y - 1] = fre;
		++fre;

		edge[fre] = y - 1;
		c[fre] = -w;
		edge_ind[fre] = i + 1;
		edge_reverse[fre] = fre - 1;
		nxt[fre] = head[x - 1];
		head[x - 1] = fre;
		++fre;
	}

	int f = 0, sum = 0;
	while (count_dist()) {
		sum += upd(n - 1);
		++f;
		if (f == k)
			break;
	}
	if (f < k) {
		cout << -1 << endl;
	}
	else {
		cout << fixed << setprecision(6) << sum * 1.0 / k << endl;
		for (int i = 0; i < k; ++i) {
			for (int j = 0; j < n; ++j) {
				mark[j] = false;
				parent1[j] = -1;
				parent2[j] = -1;
			}
			findpath(0);
			writepath(n - 1, 0);
			cout << endl;
		}
	}
	return 0;
}