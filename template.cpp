//https://github.com/kuro00128/atcoder-library
//上がatcoderのライブラリ置き場です
#pragma region header
#include <bits/stdc++.h>
#include <atcoder/modint>
#include <atcoder/dsu>
#include <atcoder/fenwicktree>
#include <atcoder/segtree>
#include <atcoder/lazysegtree>
// other ACL: e.g. #include <atcoder/maxflow>

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
#pragma endregion

#pragma region my library

mt19937_64 rng(10);
constexpr int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1};
template<class = void> ll ipow(ll x, ll n){
  ll ret = 1;
  while(n > 0){
    if(n & 1) ret *= x;
    x *= x, n >>= 1;
  }
  return ret;
}
template<class = void> ll powmod(ll x, ll n, ll m){
  ll ret = 1 % m;
  x %= m;
  if(x < 0) x += m;
  while(n > 0){
    if(n & 1) ret = ret * x % m;
    x = x * x % m;
    n >>= 1;
  }
  return ret;
}
template<class = void> vector<pair<char, ll>> rle(string s){
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
template<class = void> vector<pair<ll, ll>> rle(vector<ll> s){
  vector<pair<ll, ll>> res;
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
  // weight(y) - weight(x) = w
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

template<class = void> ll dot(Point a, Point b){ return a.x * b.x + a.y * b.y; }

template<class = void> ll cross(Point a, Point b){ return a.x * b.y - a.y * b.x; }

template<class = void> ll dist2(Point a, Point b){
  ll dx = a.x - b.x;
  ll dy = a.y - b.y;
  return dx * dx + dy * dy;
}
template<class = void> bool intersect(Point A, Point B, Point C, Point D){
  ll c1 = cross(B - A, C - A);
  ll c2 = cross(B - A, D - A);
  ll c3 = cross(D - C, A - C);
  ll c4 = cross(D - C, B - C);
  return ((c1 > 0 and c2 < 0) or (c1 < 0 and c2 > 0)) and ((c3 > 0 and c4 < 0) or (c3 < 0 and c4 > 0));
}
// combination: call comb_init(n) before use
vector<mint> fact, fact_inv, inv;
template<class = void> void comb_init(int n){
  fact.assign(n + 1, 1);
  fact_inv.assign(n + 1, 1);
  inv.assign(n + 1, 1);
  for(int i = 2; i <= n; i++){
    fact[i] = fact[i - 1] * i;
    inv[i] = -inv[mint::mod() % i] * (mint::mod() / i);
    fact_inv[i] = fact_inv[i - 1] * inv[i];
  }
}
template<class = void> mint nCr(ll n, ll r){
  if(n < 0 || r < 0 || r > n) return 0;
  return fact[n] * fact_inv[r] * fact_inv[n - r];
}
template<class = void> mint nPr(ll n, ll r){
  if(n < 0 || r < 0 || r > n) return 0;
  return fact[n] * fact_inv[n - r];
}
template<class = void> mint nHr(ll n, ll r){
  if(n == 0 && r == 0) return 1;
  return nCr(n + r - 1, r);
}
// rolling hash (mod 2^61-1, random base)
template<class = void> struct RollingHash_t{
  using u64 = unsigned long long;
  static constexpr u64 MOD = (1ULL << 61) - 1;
  static u64 mul(u64 a, u64 b){
    __uint128_t t = (__uint128_t)a * b;
    u64 r = (u64)(t >> 61) + (u64)(t & MOD);
    if(r >= MOD) r -= MOD;
    if(r >= MOD) r -= MOD;
    return r;
  }
  static u64 add(u64 a, u64 b){ a += b; return a >= MOD ? a - MOD : a; }
  static inline u64 base = 0;
  static inline vector<u64> pw{1};
  static void ensure(int n){
    if(base == 0){
      mt19937_64 r(chrono::steady_clock::now().time_since_epoch().count());
      base = r() % (MOD - 2) + 2;
    }
    while((int)pw.size() <= n) pw.push_back(mul(pw.back(), base));
  }
  vector<u64> h;
  template<class S> RollingHash_t(const S& s){
    int n = s.size();
    ensure(n);
    h.assign(n + 1, 0);
    for(int i = 0; i < n; i++) h[i + 1] = add(mul(h[i], base), (u64)s[i] % MOD + 1);
  }
  int size() const { return (int)h.size() - 1; }
  u64 get(int l, int r) const { return add(h[r], MOD - mul(h[l], pw[r - l])); }
  static u64 connect(u64 h1, u64 h2, int len2){ ensure(len2); return add(mul(h1, pw[len2]), h2); }
};
using RollingHash = RollingHash_t<>;
template<class = void> int lcp(const RollingHash& a, int i, const RollingHash& b, int j){
  int lo = 0, hi = min(a.size() - i, b.size() - j);
  while(lo < hi){
    int m = (lo + hi + 1) / 2;
    if(a.get(i, i + m) == b.get(j, j + m)) lo = m;
    else hi = m - 1;
  }
  return lo;
}
#pragma endregion


int main(){
}