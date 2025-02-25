#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

std::vector<int> Hierholzer(const std::vector<std::vector<int>>& g) {
    int n = (int)g.size() - 1;
    if (n == 0) {
        return {};
    }
    std::vector<std::map<int, int>> num(n + 1, std::map<int, int>());
    std::vector<int> in(n + 1, 0);
    for (int u = 1; u <= n; ++u) {
        for (int v : g[u]) {
            ++num[u][v];
            ++in[v];
        }
    }
    int begin = 1;
    for (int u = 2; u <= n; ++u) {
        if ((int)g[begin].size() == 0 && (int)g[u].size() != 0) {
            begin = u;
        } else if ((int)g[begin].size() != in[begin] + 1 && (int)g[u].size() == in[u] + 1) {
            begin = u;
        }
    }
    std::vector<int> ans;
    std::vector<int> idx(n + 1, 0);
    std::vector<int> st;
    st.push_back(begin);
    while (!st.empty()) {
        int u = st.back();
        if (idx[u] < (int)g[u].size()) {
            int v = g[u][idx[u]];
            if (num[u][v] > 0) {
                --num[u][v];
                st.push_back(v);
            }
            ++idx[u];
        } else {
            ans.push_back(u);
            st.pop_back();
        }
    }
    std::reverse(ans.begin(), ans.end());
    return ans;
}

bool check(const std::vector<std::vector<int>>& g, const std::vector<int>& path) {
    int n = (int)g.size() - 1;
    int m = 0;
    std::vector<std::map<int, int>> num(n + 1, std::map<int, int>());
    for (int u = 1; u <= n; ++u) {
        for (int v : g[u]) {
            ++num[u][v];
            ++m;
        }
    }
    int ps = (int)path.size();
    for (int i = 0; i + 1 < ps; ++i) {
        int u = path[i];
        int v = path[i + 1];
        if (u < 1 || u > n || v < 1 || v > n || num[u][v] == 0) {
            return false;
        }
        --num[u][v];
        --m;
    }
    return m == 0;
}
