#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <utility>
#include <queue>

using namespace std;

struct pairCompare {
    bool operator()(const pair<int, int> &a, const pair<int, int> &b) {
        if (a.second == b.second)
            return b.first > a.first;
        else return a.second > b.second;
    }
};

struct pairCompare2 {
    bool operator()(const pair<int, int> &a, const pair<int, int> &b) {
        if (a.first == b.first)
            return b.second < a.second;
        else return a.first < b.first;
    }
};

__attribute__((optimize("Os"))) int main() {
    long long int pubCnt = 0, beerCnt = 0, dummy = 0, dummy2 = 0;

    cin >> pubCnt;
    cin >> beerCnt;

    vector<long long int> beers(beerCnt);
    vector<long long int> pubs(pubCnt);

    set<pair<long long int, long long int>, pairCompare2> sortedPrices;
    vector<long long int> degrees(pubCnt, 0);

    // graph
    map<long long int, set<long long int>> edges;
    vector<bool> visited(pubCnt, false);

    // nodes sorted by bfs opening time
    set<pair<long long int, long long int>, pairCompare> order;

    vector<pair<pair<long long int, long long int>, pair<long long int, long long int >>> bestRes(pubCnt, {{-1, -1}, {-1, -1}});
    // weights init
    for (long long int i = 0; i < beerCnt; i++) {
        cin >> beers[i];
        sortedPrices.emplace(beers[i], i);
    }
    for (long long int i = 0; i < pubCnt; i++) {
        cin >> pubs[i];
    }

    // graph init
    while (cin >> dummy >> dummy2) {
        --dummy;
        --dummy2;

        if (edges.find(dummy) == edges.end())
            edges.insert({dummy, {dummy2}});
        else edges[dummy].insert(dummy2);
        degrees[dummy]++;

        if (edges.find(dummy2) == edges.end())
            edges.insert({dummy2, {dummy}});
        else edges[dummy2].insert(dummy);
        degrees[dummy2]++;
    }

    // bfs
    queue<pair<long long int, long long int>> q;
    pair<long long int, long long int> holder;
    q.push({0, 0});
    visited[0] = true;
    while (!q.empty()) {
        holder = q.front();
        q.pop();
        order.emplace(holder.first, holder.second);
        for (const auto &iter : edges[holder.first]) {
            if (!visited[iter]) {
                visited[iter] = true;
                q.emplace(iter, holder.second + 1);
            }
        }
    }

    visited = vector<bool>(pubCnt, false);
    for (const auto &item : order) {
         //cout << item.first << "[" << item.second << "]" << endl;
         //cout<<"values ["<<pubs[item.first]<<"] "<<endl;
        auto pricesIter = sortedPrices.begin();
        // for each neighbour
        for (long long int i = 0; i <min(degrees[item.first]+1, beerCnt); i++, pricesIter++) {
            long long int tmp = pubs[item.first]*pricesIter->first;

            for(auto const & x : edges[item.first]){
                if(visited[x]){
                    if(bestRes[x].first.second == pricesIter->second){
                        tmp += bestRes[x].second.first;
                    } else tmp += bestRes[x].first.first;
                }
            }
            // first best is empty
            if (bestRes[item.first].first.first == -1)
                bestRes[item.first].first = {tmp, pricesIter->second};
            // first is not empty
            else {
                // first has greater sum
                if (bestRes[item.first].first.first > tmp) {
                    bestRes[item.first].second = bestRes[item.first].first;
                    bestRes[item.first].first = {tmp, pricesIter->second};
                } // first has lesser sum
                else if (bestRes[item.first].second.first > tmp || bestRes[item.first].second.first == -1)
                    bestRes[item.first].second = {tmp, pricesIter->second};
            }

        }
        visited[item.first] = true;
    }
    long long int bestValue = bestRes[0].first.first;
    vector<long long int> final(pubCnt);
    visited = vector<bool>(pubCnt, false);
    long long int startIdx = -1;
    q.emplace(0, -1);
    while(!q.empty()){
        holder = q.front();
        q.pop();
        if(bestRes[holder.first].first.second == holder.second)
            final[holder.first] = bestRes[holder.first].second.second;
        else final[holder.first] = bestRes[holder.first].first.second;
        startIdx = final[holder.first];
        visited[holder.first] = true;
        for(auto const & neigh : edges[holder.first]){
            if(!visited[neigh])
                q.emplace(neigh, startIdx);
        }
    }
    cout<<bestValue<<endl;
    for(auto const & item: final)
        cout<<item+1<<" ";
    return 0;
}
