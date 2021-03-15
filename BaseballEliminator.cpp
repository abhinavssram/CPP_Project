#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <climits>
#include <utility>

using namespace std;

//declaration of global variables..

// 'V' denotes the total vertices in the graph.
int V = 0 ;

// "mcp" is a vector that stores the vertices of the min-cut.
vector<int> mcp ;


//bfs is breadth first search algorithm used to find the shortest augumented path from source to target.
// below is my standard implementation of bfs code.
bool bfs(vector<vector<int> >& resGraph, vector<int>& parr , int s, int t) {

	vector<int> vt(V, 0);

	queue <int> tmp;
	tmp.push(s);
	parr[s] = -1 , vt[s] = 1 ;

	while (!tmp.empty())
	{
		int i = tmp.front();
		tmp.pop();

		for (int j = 0; j < V; j++)
		{
			if (resGraph[i][j] > 0 && vt[j] == 0 )
			{
				tmp.push(j);
				parr[j] = i;
				vt[j] = 1;
			}
		}
	}

	if (vt[t] == 1) {
		return true;
	}

	return false ;
}

// dfs is depth first search algorithm used to find mincut (the vertices in the subset).
// below is my standard implementation of dfs code.
void dfs(vector<vector<int> >& resGraph , vector<int>& v , int s) {
	v[s] = 1 ;
	for (int i = 0; i < V; i++) {
		if (!v[i] && resGraph[s][i]) {
			dfs(resGraph, v, i);
		}
	}
}

int opt_ff(vector<vector<int> >& act_m , int s, int t) {

	// s - source vertex , t - target vertex , act_m is our original graph

	// intialise the final maxflow with 0.
	int maxflow = 0;

	// Create a residual graph.
	vector<vector<int> > resGraph(V, vector<int>(V, 0));

	// Initialise residual graph with same values as original graph
	for (int u = 0; u < V; u++) {
		for (int v = 0; v < V; v++) {
			resGraph[u][v] = act_m[u][v];
		}
	}

	// Declare a parrent vector to store path
	vector<int> parr(V);


	// start bfs algorithm on the residual graph -
	// to find augemented paths and bottleneck values.
	// This continues until there are no more augumented paths in graph.
	while (bfs(resGraph, parr, s, t)) {

		// "bottle neck" value is the minimum most value in one augumented path.
		int bottleneck = INT_MAX;

		for (int v = t; v != s; v = parr[v])
		{
			int u = parr[v];
			bottleneck = min(bottleneck, resGraph[u][v]);
		}

		// update the residual graph (forward and backward edges).
		for (int v = t; v != s; v = parr[v])
		{
			int u = parr[v];
			resGraph[u][v] = resGraph[u][v] - bottleneck;
			resGraph[v][u] = resGraph[v][u] + bottleneck;
		}

		// add the bottle neck value that are found in one path
		maxflow = maxflow + bottleneck;
	}

	// find the mincut and the vertices of the mincut using dfs.
	vector<int> vec(V) ;

	dfs(resGraph, vec, s) ;

	for (int i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {

			if (vec[i] && !vec[j] && act_m[i][j]) {

				vector<int>::iterator it1 = find(mcp.begin(), mcp.end(), i) ;

				vector<int>::iterator it2 = find(mcp.begin(), mcp.end(), j) ;

				if (it1 == mcp.end()) mcp.push_back(i) ;

				if (it2 == mcp.end()) mcp.push_back(j) ;

			}
		}
	}
	// cout << "residualm: \n" ;
	// for (int nm = 0; nm < resGraph.size(); nm++) {
	// 	for (int lk = 0; lk < resGraph[0].size(); lk++) {
	// 		cout << resGraph[nm][lk] << " " ;
	// 	}
	// 	cout << "\n" ;
	// }
	return maxflow;
}

// Formulation of graph and taking inputs happen from below lines of code.
int main() {

#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin) ;
	freopen("output.txt", "w", stdout);
#endif

	int n ;
	cin >> n;
	string s ;
	int w , l, r , tmp;
	vector<string> team ;
	vector<int> wins;
	vector<int> loss;
	vector<int> left;
	vector<vector<int> > adj_m(n, vector<int>(n, 0)) ;
	for (int i = 0; i < n; i++) {
		cin >> s >> w >> l >> r ;
		team.push_back(s);
		wins.push_back(w);
		loss.push_back(l);
		left.push_back(r);

		for (int j = 0; j < n; j++) {
			cin >> tmp;
			adj_m[i][j] += tmp ;
		}
	}

	// st is the total no of vertices in our maxflow formulation graph
	// vertices are like 0(source) 0-1,0-2,1-2... ,st-1(target) numbered along matrix.

	int st = ((n * (n - 1)) / 2) + 2 ;
	// cout << "st: " << st << "\n" ;

	for (int id = 0; id < n; id++) {

		// Representing graph in form of adjacency matrix.
		// act_m denotes actual matrix ie maxflow formulated graph of size st.
		vector<vector<int> > act_m(st, vector<int>(st, 0)) ;

		// filling the entries of act_m .
		int j = 1 ;

		// vp is to store pairs like 0-1,0-2 etc
		vector<pair<int, int> > vp ;

		// vsd is to store actual index's of the teams in act_m.
		vector<int> vsd ;

		vsd.push_back(0) ;

		int in_flow = 0;

		// iterating over all vertices which are teams to find maxflows

		for (int i = 0 ; i < n; i++) {
			if (i != id) {
				for (int k = 0; k < n ; k++) {
					if (k != id) {
						if (k > i) {
							act_m[0][j] = adj_m[i][k] ;

							in_flow = in_flow + adj_m[i][k] ;

							vsd.push_back(-1) ;

							vp.push_back(make_pair(i, k)) ;

							j++ ;
						}
					}
				}
			}
		}


		for (int lk = 0; lk < n; lk++) {
			if (lk != id) {
				vsd.push_back(lk) ;
			}
		}

		vsd.push_back(st - 1) ;

		int i = 1, c = j;

		for (int k = j; k < st - 2 ; k++) {
			for (int l = k; l < st - 1; l++) {

				if (k < l) {

					act_m[i][k] = INT_MAX ;
					act_m[i][l] = INT_MAX ;
					i++ ;
				}
			}
		}

		vector<int> ctid ;
		for (int ct = 0; ct < n; ct++)
		{

			if (ct != id) {
				ctid.push_back(ct) ;
			}
		}

		int e = 0 ;
		for ( ; j < st - 1; j++) {
			if (e < ctid.size()) {
				act_m[j][st - 1] = wins[id] + left[id] - wins[ctid[e]] ;
				e++ ;
			}
		}
		// end of filling entries of act_m.

		// cout << "act_m: \n" ;
		// for (int nm = 0; nm < act_m.size(); nm++) {
		// 	for (int lk = 0; lk < act_m[0].size(); lk++) {
		// 		cout << act_m[nm][lk] << " " ;
		// 	}
		// 	cout << "\n" ;
		// }

		// V is the total numer of vertices in maxflow formulated graph(act_m).
		V = st ;

		// calling the ford fulkersion algorithm on act_m .
		// returns maxflow for particular team id(0 to n-1).
		int ansd = opt_ff(act_m, 0, st - 1) ;

		// cout << "The Maxflow is: " << ansd << "\n" ;

		// cout << "in_flow: " << in_flow << "\n" ;


		// elimination process
		// two stages :
		// 1. directly eliminated (wins[required team]+ left[required team]< wins[any team])
		// 2. checking the subsets of vertices formed from the mincut.

		// first stage of elimination

		int flag = 0 ;
		for (int i = 0; i < n; i++) {
			if (wins[id] + left[id] < wins[i]) {
				cout << team[id] << " is eliminated.\n" ;
				cout << "They can win at most " << wins[id] << " + " << left[id] << " = " << wins[id] + left[id] << " games.\n" ;
				cout << team[i] << " has won a total of " << wins[i] << " games.\n" ;
				cout << "They play each other " << adj_m[i][i] << " times.\n" ;
				cout << "So on average, each of the teams in this group wins " << wins[i] << "/1" << " = " << wins[i] << " games.\n" ;
				flag = 1 ;
				break ;
			}
		}// end of first stage of elimination.


		int ulti = 0;
		// second stage of elimination
		if (!flag && ansd != in_flow) {
			vector<int> tids ;

			// R is the vector that stores actual subset of mincut vertices.
			// the idea is to include vertices that have wins + left > wins[id] + left[id]
			// R will have unique elements.

			vector<int> R ;

			for (int i = 0; i < mcp.size(); i++) {
				if (mcp[i] != 0 && mcp[i] != st - 1) {
					tids.push_back(mcp[i]) ;
				}
			}

			for (int i = 0; i < tids.size(); i++) {

				if (tids[i] <= vp.size()) {
					int p1 = vp[tids[i] - 1].first ;
					int p2 = vp[tids[i] - 1].second ;

					if (wins[p1] + left[p1] >= wins[id] + left[id]) {
						vector<int>::iterator it = find(R.begin(), R.end(), p1) ;
						if (it == R.end())
							R.push_back(p1) ;
					}
					if (wins[p2] + left[p2] >= wins[id] + left[id]) {
						vector<int>::iterator it = find(R.begin(), R.end(), p2) ;
						if (it == R.end())
							R.push_back(p2) ;
					}
				} else {
					int p3 = vsd[tids[i]] ;
					if (wins[p3] + left[p3] >= wins[id] + left[id]) {
						vector<int>::iterator it = find(R.begin(), R.end(), p3) ;
						if (it == R.end())
							R.push_back(p3) ;
					}
				}
			}

			// winR is totals wins of elements in R
			// leftR is totals left games of elements in R

			int winR = 0 , leftR = 0;

			for (int i = 0; i < R.size(); i++) {
				winR += wins[R[i]] ;
			}

			for (int i = 0; i < R.size(); i++) {
				for (int j = 0; j < R.size(); j++) {
					if (i < j) {
						leftR += adj_m[R[i]][R[j]] ;
					}
				}
			}

			ulti = 1 ;
			cout << team[id] << " is eliminated.\n" ;

			cout << "They can win at most " << wins[id] << " + " << left[id] << " = " << wins[id] + left[id] << " games.\n" ;

			if (R.size() > 2) {
				for (int i = 0; i < R.size() - 2; i++) {
					cout << team[R[i]] << ", " ;
				}
				cout << team[R[R.size() - 2]] << " and " << team[R[R.size() - 1]] << " has won a total of " << winR << " games.\n" ;
			} else {
				cout << team[0] << " and " << team[1] << " has won a total of " << winR << " games.\n" ;
			}

			cout << "They play each other " << leftR << " times.\n" ;

			cout << "So on average, each of the teams in this group wins " << winR + leftR << "/" << R.size() << " = " << (float)(winR + leftR) / R.size() << " games.\n" ;

			cout << "\n" ;
		} // second stage of elimination ended

		// use if to check which teams are not eliminated.
		// if (!ulti) cout << team[id] << " team is not eliminated.\n" ;

	}// end of each team maxflow finding loop


	return 0; //for main function.

}
