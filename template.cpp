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
  vector<int> par, siz;
  vector<T> diff_weight;

  UnionFind(int n = 1, T SUM_UNITY = 0){
    init(n, SUM_UNITY);
  }
  void init(int n = 1, T SUM_UNITY = 0){
    par.resize(n); siz.assign(n, 1); diff_weight.assign(n, SUM_UNITY);
    rep(i, n) par[i] = i;
  }
  int root(int x){
    if(par[x] == x) return x;
    int r = root(par[x]);
    diff_weight[x] += diff_weight[par[x]];
    return par[x] = r;
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
    if(siz[x] < siz[y]) swap(x, y), w = -w;
    par[y] = x;
    siz[x] += siz[y];
    diff_weight[y] = w;
    return true;
  }
  T diff(int x, int y){
    return weight(y) - weight(x);
  }
  int size(int x){
    return siz[root(x)];
  }
};
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
vector<mint> fact, fact_inv, inv;
inline void comb_init(int n){
  fact.assign(n + 1, 1);
  fact_inv.assign(n + 1, 1);
  inv.assign(n + 1, 1);
  for(int i = 2; i <= n; i++){
    fact[i] = fact[i - 1] * i;
    inv[i] = -inv[mint::mod() % i] * (mint::mod() / i);
    fact_inv[i] = fact_inv[i - 1] * inv[i];
  }
}
inline mint nCr(ll n, ll r){
  if(n < 0 || r < 0 || r > n) return 0;
  return fact[n] * fact_inv[r] * fact_inv[n - r];
}
inline mint nPr(ll n, ll r){
  if(n < 0 || r < 0 || r > n) return 0;
  return fact[n] * fact_inv[n - r];
}
inline mint nHr(ll n, ll r){
  if(n == 0 && r == 0) return 1;
  return nCr(n + r - 1, r);
}
struct RollingHash {
  using u64 = unsigned long long;
  static constexpr u64 MOD = (1ULL << 61) - 1;
  static u64 mul(u64 a, u64 b) { return (u64)((__uint128_t)a * b % MOD); }
  static u64 base() {
    static const u64 B = std::mt19937_64(
        std::chrono::steady_clock::now().time_since_epoch().count())() % (MOD - 2) + 2;
    return B;
  }
 
  std::vector<u64> H, pw;
  RollingHash(const std::string &s) { init(s); }
  void init(const std::string &s) {
    int n = s.size();
    H.assign(n + 1, 0);
    pw.assign(n + 1, 1);
    for (int i = 0; i < n; i++) {
      H[i + 1] = (mul(H[i], base()) + s[i]) % MOD;
      pw[i + 1] = mul(pw[i], base());
    }
  }
  u64 get(int l, int r) const {
    return (H[r] + MOD - mul(H[l], pw[r - l])) % MOD;
  }
};
int main(){
}