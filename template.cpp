//https://github.com/kuro00128/atcoder-library
//上がatcoderのライブラリ置き場です
#include <bits/stdc++.h>
#include <atcoder/modint>
#include <atcoder/dsu>
#include <atcoder/fenwicktree>
#include <atcoder/segtree>
#include <atcoder/lazysegtree>

using namespace std;
using namespace atcoder;
#ifdef LOCAL
#include "debug.hpp"
#else
#define debug(...)
#endif
typedef long long ll;
typedef unsigned long long ull;
typedef long double lb;
#define rep(i, n) for (ll i = 0; i < (ll)(n); i++)
static const double pi = 3.141592653589793;
const ll INF = 1LL << 60;
//const ll mod = 1000000007;
const ll mod = 998244353;
//using mint = modint1000000007;
using mint = modint998244353;
mt19937_64 rng(10);
vector<int> dx = {1, 0, -1, 0}, dy = {0, 1, 0, -1};
inline ll P(ll x, ll n){
  ll ret = 1;
  while(n > 0){
    if(n & 1) ret *= x;
    x *= x, n >>= 1;
  }
  return ret;
}
inline ll Pmod(ll x, ll n, ll mod){
  ll ret = 1;
  while(n > 0){
    if(n & 1) ret *= x, ret %= mod;
    x = (x % mod) * (x % mod) % mod;
    n >>= 1;
  }
  return ret;
}
inline vector<pair<char, ll>> rle(string s){
  vector<pair<char, ll>> res;
  ll cnt = 1;
  for(int i = 1; i < (int)s.size(); i++){
    if(s[i] != s[i - 1]){
      res.push_back({s[i - 1], cnt});
      cnt = 0;
    }
    cnt++;
  }
  res.push_back({s.back(), cnt});
  return res;
}
inline vector<pair<ll, ll>> rle(vector<ll> s){
  vector<pair<ll, ll>> res;
  ll cnt = 1;
  for(int i = 1; i < s.size(); i++){
    if(s[i] != s[i - 1]){
      res.push_back({s[i - 1], cnt});
      cnt = 0;
    }
    cnt++;
  }
  res.push_back({s.back(), cnt});
  return res;
}
template<class T> struct UnionFind{
  vector<int> par;
  vector<int> rank;
  vector<T> diff_weight;
  
  UnionFind(int n = 1, T SUM_UNITY = 0){
    init(n, SUM_UNITY);
  }
  void init(int n = 1, T SUM_UNITY = 0){
    par.resize(n); rank.resize(n); diff_weight.resize(n);
    rep(i, n){
      par[i] = i;
      rank[i] = 0;
      diff_weight[i] = SUM_UNITY;
    }
  }
  int root(int x){
    if(par[x] == x){
      return x;
    }
    else{
      int r = root(par[x]);
      diff_weight[x] += diff_weight[par[x]];
      return par[x] = r;
    }
  }
  T weight(int x){
    root(x);
    return diff_weight[x];
  }
  bool issame(int x, int y){
    return root(x) == root(y);
  }
  bool merge(int x, int y, T w){
    w += weight(x); w -= weight(y);
    x = root(x); y = root(y);
    if(x == y) return false;
    if(rank[x] < rank[y]) swap(x, y), w *= -1;
    if(rank[x] == rank[y]) rank[x]++;
    par[y] = x;
    diff_weight[y] = w;
    return true;
  }
  T diff(int x, int y){
    return weight(y) - weight(x);
  }
  int size(int x) {
    return -par[root(x)];
  }
};
//* added 2026/9/30 *//
struct Point{
  ll x, y;
  Point operator+(const Point &p) const{ return {x + p.x, y + p.y}; }
  Point operator-(const Point &p) const{ return {x - p.x, y - p.y}; }
  Point operator*(ll k) const{ return {x * k, y * k}; }
  bool operator<(const Point &p) const{
    if(x != p.x) return x < p.x;
    else return y < p.y;
  }
};

inline ll dot(Point a, Point b){ return a.x * b.x + a.y * b.y; }

inline ll cross(Point a, Point b){ return a.x * b.y - a.y * b.x; }

inline ll dist2(Point a, Point b){
  ll dx = a.x - b.x;
  ll dy = a.y - b.y;
  return dx * dx + dy * dy;
}
inline bool intersect(Point A, Point B, Point C, Point D){
  ll c1 = cross(B - A, C - A);
  ll c2 = cross(B - A, D - A);
  ll c3 = cross(D - C, A - C);
  ll c4 = cross(D - C, B - C);
  return ((c1 > 0 and c2 < 0) or (c1 < 0 and c2 > 0)) and ((c3 > 0 and c4 < 0) or (c3 < 0 and c4 > 0));
}
int main(){
}