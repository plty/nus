#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> ii;
int main() {
    int nQ; scanf("%d", &nQ);
    while (nQ--) {
        int n, l; scanf("%d%d", &n, &l);
        vector<pair<int, int>> lectures(n);
        for (int i = 0; i < n; i++)
            scanf("%d", &lectures[i].first);

        for (int i = 0; i < n; i++)
            scanf("%d", &lectures[i].second);

        sort(lectures.begin(), lectures.end(), 
                [](ii a, ii b) { return a.second < b.second; });

        vector<ii> timeline;
        for (int i = 0; i < n; i++) {
            timeline.emplace_back(lectures[i].first, 1);
            timeline.emplace_back(lectures[i].second, -1);
        }

        sort(timeline.begin(), timeline.end());

        int globalmax = 0, cur = 0;
        for (int i = 0; i < 2 * n; i++) {
            cur += timeline[i].second;
            globalmax = max(globalmax, cur);
        }

        multiset<ii> next_vacant;
        for (int i = 0; i < l; i++) {
            next_vacant.emplace(0, i);
        }

        int cancelled = 0;
        for (int i = 0; i < n; i++) {
            auto x = next_vacant.lower_bound(make_pair(lectures[i].first, 1 << 30));
            if (x == next_vacant.begin()) {
                cancelled++;
                continue;
            }
            x--;
            int hall = x->second;
            next_vacant.erase(x);
            next_vacant.emplace(lectures[i].second, hall);
        }

        printf("%d\n%d\n", globalmax, cancelled);
    }
}
