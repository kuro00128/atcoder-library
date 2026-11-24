//https://github.com/kuro00128/atcoder-library
//no-ACL version (ACL-compatible library included)
#pragma region header
#include <bits/stdc++.h>

using namespace std;
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
#pragma endregion

#pragma region ACL-compatible library

#pragma region math
template<class = void> ll safe_mod(ll x, ll m){ x %= m; if(x < 0) x += m; return x; }
// returns {g, x} : g = gcd(a, b), a * x = g (mod b), 0 <= x < b / g
template<class = void> pair<ll, ll> inv_gcd(ll a, ll b){
  a = safe_mod(a, b);
  if(a == 0) return {b, 0};
  ll s = b, t = a, m0 = 0, m1 = 1;
  while(t){
    ll u = s / t;
    s -= t * u; m0 -= m1 * u;
    swap(s, t); swap(m0, m1);
  }
  if(m0 < 0) m0 += b / s;
  return {s, m0};
}
template<class = void> ll pow_mod(ll x, ll n, int m){
  if(m == 1) return 0;
  ull r = 1, y = safe_mod(x, m);
  while(n){ if(n & 1) r = r * y % m; y = y * y % m; n >>= 1; }
  return (ll)r;
}
template<class = void> ll inv_mod(ll x, ll m){
  auto z = inv_gcd(x, m);
  assert(z.first == 1);
  return z.second;
}
// x = r[i] (mod m[i]) for all i -> {y, z} (x = y mod z), no solution -> {0, 0}
template<class = void> pair<ll, ll> crt(const vector<ll>& r, const vector<ll>& m){
  assert(r.size() == m.size());
  ll r0 = 0, m0 = 1;
  for(int i = 0; i < (int)r.size(); i++){
    ll r1 = safe_mod(r[i], m[i]), m1 = m[i];
    if(m0 < m1){ swap(r0, r1); swap(m0, m1); }
    if(m0 % m1 == 0){
      if(r0 % m1 != r1) return {0, 0};
      continue;
    }
    auto [g, im] = inv_gcd(m0, m1);
    ll u1 = m1 / g;
    if((r1 - r0) % g) return {0, 0};
    ll x = (r1 - r0) / g % u1 * im % u1;
    r0 += x * m0; m0 *= u1;
    if(r0 < 0) r0 += m0;
  }
  return {r0, m0};
}
template<class = void> ull floor_sum_unsigned(ull n, ull m, ull a, ull b){
  ull ans = 0;
  while(true){
    if(a >= m){ ans += n * (n - 1) / 2 * (a / m); a %= m; }
    if(b >= m){ ans += n * (b / m); b %= m; }
    ull y_max = a * n + b;
    if(y_max < m) break;
    n = y_max / m; b = y_max % m;
    swap(m, a);
  }
  return ans;
}
// sum_{i=0}^{n-1} floor((a * i + b) / m)
template<class = void> ll floor_sum(ll n, ll m, ll a, ll b){
  ull ans = 0;
  if(a < 0){ ull a2 = safe_mod(a, m); ans -= 1ULL * n * (n - 1) / 2 * ((a2 - a) / m); a = a2; }
  if(b < 0){ ull b2 = safe_mod(b, m); ans -= 1ULL * n * ((b2 - b) / m); b = b2; }
  return ans + floor_sum_unsigned(n, m, a, b);
}

#pragma endregion
#pragma region modint
template<int MOD> struct static_modint{
  using mint = static_modint;
  unsigned int v;
  static constexpr int mod(){ return MOD; }
  static_modint() : v(0){}
  template<class T> static_modint(T x){ ll y = (ll)(x % (ll)MOD); if(y < 0) y += MOD; v = (unsigned int)y; }
  static mint raw(int x){ mint r; r.v = x; return r; }
  int val() const { return (int)v; }
  mint& operator++(){ if(++v == MOD) v = 0; return *this; }
  mint& operator--(){ if(v == 0) v = MOD; v--; return *this; }
  mint operator++(int){ mint r = *this; ++*this; return r; }
  mint operator--(int){ mint r = *this; --*this; return r; }
  mint& operator+=(const mint& o){ v += o.v; if(v >= (unsigned)MOD) v -= MOD; return *this; }
  mint& operator-=(const mint& o){ v -= o.v; if(v >= (unsigned)MOD) v += MOD; return *this; }
  mint& operator*=(const mint& o){ v = (unsigned int)((ull)v * o.v % MOD); return *this; }
  mint& operator/=(const mint& o){ return *this *= o.inv(); }
  mint operator+() const { return *this; }
  mint operator-() const { return mint() - *this; }
  mint pow(ll n) const { assert(n >= 0); mint r = 1, a = *this; while(n){ if(n & 1) r *= a; a *= a; n >>= 1; } return r; }
  mint inv() const { auto z = inv_gcd(v, MOD); assert(z.first == 1); return z.second; }
  friend mint operator+(mint a, const mint& b){ return a += b; }
  friend mint operator-(mint a, const mint& b){ return a -= b; }
  friend mint operator*(mint a, const mint& b){ return a *= b; }
  friend mint operator/(mint a, const mint& b){ return a /= b; }
  friend bool operator==(const mint& a, const mint& b){ return a.v == b.v; }
  friend bool operator!=(const mint& a, const mint& b){ return a.v != b.v; }
  friend ostream& operator<<(ostream& os, const mint& a){ return os << a.v; }
  friend istream& operator>>(istream& is, mint& a){ ll x; is >> x; a = mint(x); return is; }
};
template<int id> struct dynamic_modint{
  using mint = dynamic_modint;
  unsigned int v;
  static inline unsigned int m_ = 998244353;
  static int mod(){ return (int)m_; }
  static void set_mod(int m){ assert(m >= 1); m_ = m; }
  dynamic_modint() : v(0){}
  template<class T> dynamic_modint(T x){ ll y = (ll)(x % (ll)m_); if(y < 0) y += m_; v = (unsigned int)y; }
  static mint raw(int x){ mint r; r.v = x; return r; }
  int val() const { return (int)v; }
  mint& operator++(){ if(++v == m_) v = 0; return *this; }
  mint& operator--(){ if(v == 0) v = m_; v--; return *this; }
  mint operator++(int){ mint r = *this; ++*this; return r; }
  mint operator--(int){ mint r = *this; --*this; return r; }
  mint& operator+=(const mint& o){ v += o.v; if(v >= m_) v -= m_; return *this; }
  mint& operator-=(const mint& o){ v += m_ - o.v; if(v >= m_) v -= m_; return *this; }
  mint& operator*=(const mint& o){ v = (unsigned int)((ull)v * o.v % m_); return *this; }
  mint& operator/=(const mint& o){ return *this *= o.inv(); }
  mint operator+() const { return *this; }
  mint operator-() const { return mint() - *this; }
  mint pow(ll n) const { assert(n >= 0); mint r = 1, a = *this; while(n){ if(n & 1) r *= a; a *= a; n >>= 1; } return r; }
  mint inv() const { auto z = inv_gcd(v, m_); assert(z.first == 1); return z.second; }
  friend mint operator+(mint a, const mint& b){ return a += b; }
  friend mint operator-(mint a, const mint& b){ return a -= b; }
  friend mint operator*(mint a, const mint& b){ return a *= b; }
  friend mint operator/(mint a, const mint& b){ return a /= b; }
  friend bool operator==(const mint& a, const mint& b){ return a.v == b.v; }
  friend bool operator!=(const mint& a, const mint& b){ return a.v != b.v; }
  friend ostream& operator<<(ostream& os, const mint& a){ return os << a.v; }
  friend istream& operator>>(istream& is, mint& a){ ll x; is >> x; a = mint(x); return is; }
};
using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;
using modint = dynamic_modint<-1>;

//using mint = modint1000000007;
using mint = modint998244353;

#pragma endregion
#pragma region convolution
template<class = void> int primitive_root_of(int m){
  if(m == 2) return 1;
  if(m == 998244353 || m == 167772161 || m == 469762049) return 3;
  if(m == 754974721) return 11;
  vector<ll> ps; ll x = m - 1;
  for(ll p = 2; p * p <= x; p++) if(x % p == 0){ ps.push_back(p); while(x % p == 0) x /= p; }
  if(x > 1) ps.push_back(x);
  for(int g = 2;; g++){
    bool ok = true;
    for(ll p : ps) if(pow_mod(g, (m - 1) / p, m) == 1){ ok = false; break; }
    if(ok) return g;
  }
}
template<int MOD> void ntt(vector<static_modint<MOD>>& a, bool invert){
  using M = static_modint<MOD>;
  int n = a.size();
  for(int i = 1, j = 0; i < n; i++){
    int bit = n >> 1;
    for(; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if(i < j) swap(a[i], a[j]);
  }
  static const int g = primitive_root_of(MOD);
  for(int len = 2; len <= n; len <<= 1){
    M w = M(g).pow((MOD - 1) / len);
    if(invert) w = w.inv();
    int half = len / 2;
    vector<M> ws(half); ws[0] = 1;
    for(int j = 1; j < half; j++) ws[j] = ws[j - 1] * w;
    for(int i = 0; i < n; i += len){
      for(int j = 0; j < half; j++){
        M u = a[i + j], v = a[i + j + half] * ws[j];
        a[i + j] = u + v; a[i + j + half] = u - v;
      }
    }
  }
  if(invert){ M ninv = M(n).inv(); for(auto& x : a) x *= ninv; }
}
template<int MOD> vector<static_modint<MOD>> convolution(vector<static_modint<MOD>> a, vector<static_modint<MOD>> b){
  using M = static_modint<MOD>;
  int n = a.size(), m = b.size();
  if(!n || !m) return {};
  if(min(n, m) <= 60){
    vector<M> c(n + m - 1);
    for(int i = 0; i < n; i++) for(int j = 0; j < m; j++) c[i + j] += a[i] * b[j];
    return c;
  }
  int z = 1; while(z < n + m - 1) z <<= 1;
  a.resize(z); b.resize(z);
  ntt(a, false); ntt(b, false);
  for(int i = 0; i < z; i++) a[i] *= b[i];
  ntt(a, true);
  a.resize(n + m - 1);
  return a;
}
template<int MOD = 998244353, class T, enable_if_t<is_integral_v<T>>* = nullptr>
vector<T> convolution(const vector<T>& a, const vector<T>& b){
  using M = static_modint<MOD>;
  vector<M> a2(a.begin(), a.end()), b2(b.begin(), b.end());
  auto c2 = convolution(a2, b2);
  vector<T> c(c2.size());
  for(int i = 0; i < (int)c.size(); i++) c[i] = c2[i].val();
  return c;
}
// exact convolution (result must fit in ll)
template<class = void> vector<ll> convolution_ll(const vector<ll>& a, const vector<ll>& b){
  int n = a.size(), m = b.size();
  if(!n || !m) return {};
  constexpr int M1 = 754974721, M2 = 167772161, M3 = 469762049;
  auto c1 = convolution<M1>(a, b), c2 = convolution<M2>(a, b), c3 = convolution<M3>(a, b);
  const ll i12 = inv_mod(M1, M2), i123 = inv_mod((ll)M1 * M2 % M3, M3);
  const __int128 M12 = (__int128)M1 * M2, MALL = M12 * M3;
  vector<ll> c(n + m - 1);
  for(int i = 0; i < n + m - 1; i++){
    ll x1 = safe_mod(c1[i], M1);
    ll x2 = safe_mod((safe_mod(c2[i], M2) - x1) % M2 * i12, M2);
    __int128 v12 = x1 + (__int128)x2 * M1;
    ll x3 = safe_mod((ll)((safe_mod(c3[i], M3) - (ll)(v12 % M3)) % M3) * i123, M3);
    __int128 v = v12 + (__int128)x3 * M12;
    if(v > MALL / 2) v -= MALL;
    c[i] = (ll)v;
  }
  return c;
}

#pragma endregion
#pragma region string
// s[i] in [0, upper]
template<class = void> vector<int> suffix_array(const vector<int>& s0, int upper){
  int n0 = s0.size();
  if(n0 == 0) return {};
  vector<int> s(n0 + 1);
  for(int i = 0; i < n0; i++) s[i] = s0[i] + 1;
  s[n0] = 0;
  int n = n0 + 1, alpha = upper + 2;
  vector<int> p(n), c(n), cnt(max(alpha, n), 0);
  for(int i = 0; i < n; i++) cnt[s[i]]++;
  for(int i = 1; i < alpha; i++) cnt[i] += cnt[i - 1];
  for(int i = 0; i < n; i++) p[--cnt[s[i]]] = i;
  c[p[0]] = 0; int classes = 1;
  for(int i = 1; i < n; i++){ if(s[p[i]] != s[p[i - 1]]) classes++; c[p[i]] = classes - 1; }
  vector<int> pn(n), cn(n);
  for(int h = 0; (1 << h) < n && classes < n; h++){
    for(int i = 0; i < n; i++){ pn[i] = p[i] - (1 << h); if(pn[i] < 0) pn[i] += n; }
    fill(cnt.begin(), cnt.begin() + classes, 0);
    for(int i = 0; i < n; i++) cnt[c[pn[i]]]++;
    for(int i = 1; i < classes; i++) cnt[i] += cnt[i - 1];
    for(int i = n - 1; i >= 0; i--) p[--cnt[c[pn[i]]]] = pn[i];
    cn[p[0]] = 0; classes = 1;
    for(int i = 1; i < n; i++){
      int a1 = c[p[i]], a2 = c[(p[i] + (1 << h)) % n];
      int b1 = c[p[i - 1]], b2 = c[(p[i - 1] + (1 << h)) % n];
      if(a1 != b1 || a2 != b2) classes++;
      cn[p[i]] = classes - 1;
    }
    c.swap(cn);
  }
  p.erase(p.begin());
  return p;
}
template<class T> vector<int> suffix_array(const vector<T>& s){
  int n = s.size();
  vector<int> idx(n);
  iota(idx.begin(), idx.end(), 0);
  sort(idx.begin(), idx.end(), [&](int a, int b){ return s[a] < s[b]; });
  vector<int> s2(n);
  int now = 0;
  for(int i = 0; i < n; i++){
    if(i && s[idx[i - 1]] != s[idx[i]]) now++;
    s2[idx[i]] = now;
  }
  return suffix_array(s2, now);
}
template<class = void> vector<int> suffix_array(const string& s){
  vector<int> s2(s.size());
  for(int i = 0; i < (int)s.size(); i++) s2[i] = (unsigned char)s[i];
  return suffix_array(s2, 255);
}
// lcp[i] = LCP(s[sa[i]..], s[sa[i+1]..]), size n-1
template<class S> vector<int> lcp_array(const S& s, const vector<int>& sa){
  int n = s.size();
  if(n == 0) return {};
  vector<int> rnk(n);
  for(int i = 0; i < n; i++) rnk[sa[i]] = i;
  vector<int> lcp(n - 1);
  int h = 0;
  for(int i = 0; i < n; i++){
    if(h > 0) h--;
    if(rnk[i] == 0) continue;
    int j = sa[rnk[i] - 1];
    for(; j + h < n && i + h < n; h++) if(s[j + h] != s[i + h]) break;
    lcp[rnk[i] - 1] = h;
  }
  return lcp;
}
// z[i] = LCP(s, s[i..]), z[0] = n
template<class S> vector<int> z_algorithm(const S& s){
  int n = s.size();
  if(n == 0) return {};
  vector<int> z(n);
  z[0] = n;
  for(int i = 1, l = 0, r = 0; i < n; i++){
    int k = (i < r ? min(r - i, z[i - l]) : 0);
    while(i + k < n && s[k] == s[i + k]) k++;
    z[i] = k;
    if(i + k > r){ l = i; r = i + k; }
  }
  return z;
}

#pragma endregion
#pragma region dsu
template<class = void> struct dsu_t{
  int n; vector<int> p;
  dsu_t(int n_ = 0) : n(n_), p(n_, -1){}
  int leader(int a){ return p[a] < 0 ? a : p[a] = leader(p[a]); }
  int merge(int a, int b){
    int x = leader(a), y = leader(b);
    if(x == y) return x;
    if(-p[x] < -p[y]) swap(x, y);
    p[x] += p[y]; p[y] = x;
    return x;
  }
  bool same(int a, int b){ return leader(a) == leader(b); }
  int size(int a){ return -p[leader(a)]; }
  vector<vector<int>> groups(){
    vector<vector<int>> g(n), r;
    for(int i = 0; i < n; i++) g[leader(i)].push_back(i);
    for(auto& v : g) if(!v.empty()) r.push_back(v);
    return r;
  }
};
using dsu = dsu_t<>;

#pragma endregion
#pragma region fenwick tree
template<class T> struct fenwick_tree{
  int n; vector<T> d;
  fenwick_tree(int n_ = 0) : n(n_), d(n_, T(0)){}
  void add(int p, T x){ for(p++; p <= n; p += p & -p) d[p - 1] += x; }
  T sum(int r){ T s = 0; for(; r > 0; r -= r & -r) s += d[r - 1]; return s; }
  T sum(int l, int r){ return sum(r) - sum(l); }
};

#pragma endregion
#pragma region segtree
template<class S, S (*op)(S, S), S (*e)()> struct segtree{
  int n, sz, lg; vector<S> d;
  segtree(int n_ = 0) : segtree(vector<S>(n_, e())){}
  segtree(const vector<S>& v) : n(v.size()){
    lg = 0; while((1 << lg) < n) lg++;
    sz = 1 << lg; d.assign(2 * sz, e());
    for(int i = 0; i < n; i++) d[sz + i] = v[i];
    for(int i = sz - 1; i >= 1; i--) update(i);
  }
  void update(int k){ d[k] = op(d[2 * k], d[2 * k + 1]); }
  void set(int p, S x){ p += sz; d[p] = x; for(int i = 1; i <= lg; i++) update(p >> i); }
  S get(int p) const { return d[p + sz]; }
  S prod(int l, int r) const {
    S sml = e(), smr = e();
    for(l += sz, r += sz; l < r; l >>= 1, r >>= 1){
      if(l & 1) sml = op(sml, d[l++]);
      if(r & 1) smr = op(d[--r], smr);
    }
    return op(sml, smr);
  }
  S all_prod() const { return d[1]; }
  template<bool (*f)(S)> int max_right(int l) const { return max_right(l, [](S x){ return f(x); }); }
  template<class G> int max_right(int l, G f) const {
    if(l == n) return n;
    l += sz; S sm = e();
    do{
      while(l % 2 == 0) l >>= 1;
      if(!f(op(sm, d[l]))){
        while(l < sz){ l = 2 * l; if(f(op(sm, d[l]))){ sm = op(sm, d[l]); l++; } }
        return l - sz;
      }
      sm = op(sm, d[l]); l++;
    } while((l & -l) != l);
    return n;
  }
  template<bool (*f)(S)> int min_left(int r) const { return min_left(r, [](S x){ return f(x); }); }
  template<class G> int min_left(int r, G f) const {
    if(r == 0) return 0;
    r += sz; S sm = e();
    do{
      r--;
      while(r > 1 && (r % 2)) r >>= 1;
      if(!f(op(d[r], sm))){
        while(r < sz){ r = 2 * r + 1; if(f(op(d[r], sm))){ sm = op(d[r], sm); r--; } }
        return r + 1 - sz;
      }
      sm = op(d[r], sm);
    } while((r & -r) != r);
    return 0;
  }
};

#pragma endregion
#pragma region lazy segtree
template<class S, S (*op)(S, S), S (*e)(), class F, S (*mapping)(F, S), F (*composition)(F, F), F (*id)()>
struct lazy_segtree{
  int n, sz, lg; vector<S> d; vector<F> lz;
  lazy_segtree(int n_ = 0) : lazy_segtree(vector<S>(n_, e())){}
  lazy_segtree(const vector<S>& v) : n(v.size()){
    lg = 0; while((1 << lg) < n) lg++;
    sz = 1 << lg; d.assign(2 * sz, e()); lz.assign(sz, id());
    for(int i = 0; i < n; i++) d[sz + i] = v[i];
    for(int i = sz - 1; i >= 1; i--) update(i);
  }
  void update(int k){ d[k] = op(d[2 * k], d[2 * k + 1]); }
  void all_apply(int k, F f){ d[k] = mapping(f, d[k]); if(k < sz) lz[k] = composition(f, lz[k]); }
  void push(int k){ all_apply(2 * k, lz[k]); all_apply(2 * k + 1, lz[k]); lz[k] = id(); }
  void push_range(int l, int r){
    for(int i = lg; i >= 1; i--){
      if(((l >> i) << i) != l) push(l >> i);
      if(((r >> i) << i) != r) push((r - 1) >> i);
    }
  }
  void set(int p, S x){ p += sz; for(int i = lg; i >= 1; i--) push(p >> i); d[p] = x; for(int i = 1; i <= lg; i++) update(p >> i); }
  S get(int p){ p += sz; for(int i = lg; i >= 1; i--) push(p >> i); return d[p]; }
  S prod(int l, int r){
    if(l == r) return e();
    l += sz; r += sz; push_range(l, r);
    S sml = e(), smr = e();
    for(; l < r; l >>= 1, r >>= 1){
      if(l & 1) sml = op(sml, d[l++]);
      if(r & 1) smr = op(d[--r], smr);
    }
    return op(sml, smr);
  }
  S all_prod(){ return d[1]; }
  void apply(int p, F f){ p += sz; for(int i = lg; i >= 1; i--) push(p >> i); d[p] = mapping(f, d[p]); for(int i = 1; i <= lg; i++) update(p >> i); }
  void apply(int l, int r, F f){
    if(l == r) return;
    l += sz; r += sz; push_range(l, r);
    for(int a = l, b = r; a < b; a >>= 1, b >>= 1){
      if(a & 1) all_apply(a++, f);
      if(b & 1) all_apply(--b, f);
    }
    for(int i = 1; i <= lg; i++){
      if(((l >> i) << i) != l) update(l >> i);
      if(((r >> i) << i) != r) update((r - 1) >> i);
    }
  }
  template<bool (*g)(S)> int max_right(int l){ return max_right(l, [](S x){ return g(x); }); }
  template<class G> int max_right(int l, G g){
    if(l == n) return n;
    l += sz;
    for(int i = lg; i >= 1; i--) push(l >> i);
    S sm = e();
    do{
      while(l % 2 == 0) l >>= 1;
      if(!g(op(sm, d[l]))){
        while(l < sz){ push(l); l = 2 * l; if(g(op(sm, d[l]))){ sm = op(sm, d[l]); l++; } }
        return l - sz;
      }
      sm = op(sm, d[l]); l++;
    } while((l & -l) != l);
    return n;
  }
  template<bool (*g)(S)> int min_left(int r){ return min_left(r, [](S x){ return g(x); }); }
  template<class G> int min_left(int r, G g){
    if(r == 0) return 0;
    r += sz;
    for(int i = lg; i >= 1; i--) push((r - 1) >> i);
    S sm = e();
    do{
      r--;
      while(r > 1 && (r % 2)) r >>= 1;
      if(!g(op(d[r], sm))){
        while(r < sz){ push(r); r = 2 * r + 1; if(g(op(d[r], sm))){ sm = op(d[r], sm); r--; } }
        return r + 1 - sz;
      }
      sm = op(d[r], sm);
    } while((r & -r) != r);
    return 0;
  }
};

#pragma endregion
#pragma region scc
template<class = void> struct scc_graph_t{
  int n; vector<vector<int>> g, rg;
  scc_graph_t(int n_ = 0) : n(n_), g(n_), rg(n_){}
  void add_edge(int from, int to){ g[from].push_back(to); rg[to].push_back(from); }
  // returns {number of components, id of each vertex (topological order)}
  pair<int, vector<int>> scc_ids(){
    vector<int> order; order.reserve(n);
    vector<char> vis(n, 0);
    vector<int> it(n, 0), st;
    for(int s = 0; s < n; s++){
      if(vis[s]) continue;
      vis[s] = 1; st.push_back(s);
      while(!st.empty()){
        int v = st.back();
        if(it[v] < (int)g[v].size()){
          int to = g[v][it[v]++];
          if(!vis[to]){ vis[to] = 1; st.push_back(to); }
        } else { order.push_back(v); st.pop_back(); }
      }
    }
    vector<int> ids(n, -1);
    int k = 0;
    for(int i = n - 1; i >= 0; i--){
      int s = order[i];
      if(ids[s] != -1) continue;
      ids[s] = k; st.push_back(s);
      while(!st.empty()){
        int v = st.back(); st.pop_back();
        for(int to : rg[v]) if(ids[to] == -1){ ids[to] = k; st.push_back(to); }
      }
      k++;
    }
    return {k, ids};
  }
  // groups in topological order
  vector<vector<int>> scc(){
    auto [k, ids] = scc_ids();
    vector<vector<int>> gr(k);
    for(int i = 0; i < n; i++) gr[ids[i]].push_back(i);
    return gr;
  }
};
using scc_graph = scc_graph_t<>;

#pragma endregion
#pragma region two sat
template<class = void> struct two_sat_t{
  int n; vector<bool> ans; scc_graph sg;
  two_sat_t(int n_ = 0) : n(n_), ans(n_), sg(2 * n_){}
  // (x_i == f) || (x_j == g)
  void add_clause(int i, bool f, int j, bool g){
    sg.add_edge(2 * i + (f ? 0 : 1), 2 * j + (g ? 1 : 0));
    sg.add_edge(2 * j + (g ? 0 : 1), 2 * i + (f ? 1 : 0));
  }
  bool satisfiable(){
    auto ids = sg.scc_ids().second;
    for(int i = 0; i < n; i++){
      if(ids[2 * i] == ids[2 * i + 1]) return false;
      ans[i] = ids[2 * i] < ids[2 * i + 1];
    }
    return true;
  }
  vector<bool> answer(){ return ans; }
};
using two_sat = two_sat_t<>;

#pragma endregion
#pragma region max flow
template<class Cap> struct mf_graph{
  struct edge{ int from, to; Cap cap, flow; };
  struct _edge{ int to, rev; Cap cap; };
  int n; vector<vector<_edge>> g; vector<pair<int, int>> pos;
  vector<int> level, iter;
  mf_graph(int n_ = 0) : n(n_), g(n_){}
  int add_edge(int from, int to, Cap cap){
    int m = pos.size();
    pos.push_back({from, (int)g[from].size()});
    int fi = g[from].size(), ti = g[to].size();
    if(from == to) ti++;
    g[from].push_back({to, ti, cap});
    g[to].push_back({from, fi, 0});
    return m;
  }
  edge get_edge(int i){
    auto& e = g[pos[i].first][pos[i].second];
    auto& re = g[e.to][e.rev];
    return {pos[i].first, e.to, e.cap + re.cap, re.cap};
  }
  vector<edge> edges(){ vector<edge> r; for(int i = 0; i < (int)pos.size(); i++) r.push_back(get_edge(i)); return r; }
  void change_edge(int i, Cap new_cap, Cap new_flow){
    auto& e = g[pos[i].first][pos[i].second];
    auto& re = g[e.to][e.rev];
    e.cap = new_cap - new_flow; re.cap = new_flow;
  }
  bool bfs(int s, int t){
    level.assign(n, -1); level[s] = 0;
    queue<int> q; q.push(s);
    while(!q.empty()){
      int v = q.front(); q.pop();
      for(auto& e : g[v]) if(e.cap > 0 && level[e.to] < 0){ level[e.to] = level[v] + 1; q.push(e.to); }
    }
    return level[t] >= 0;
  }
  Cap dfs(int v, int t, Cap f){
    if(v == t) return f;
    for(int& i = iter[v]; i < (int)g[v].size(); i++){
      auto& e = g[v][i];
      if(e.cap > 0 && level[v] < level[e.to]){
        Cap d = dfs(e.to, t, min(f, e.cap));
        if(d > 0){ e.cap -= d; g[e.to][e.rev].cap += d; return d; }
      }
    }
    return 0;
  }
  Cap flow(int s, int t){ return flow(s, t, numeric_limits<Cap>::max()); }
  Cap flow(int s, int t, Cap limit){
    Cap fl = 0;
    while(fl < limit && bfs(s, t)){
      iter.assign(n, 0);
      while(fl < limit){
        Cap f = dfs(s, t, limit - fl);
        if(f == 0) break;
        fl += f;
      }
    }
    return fl;
  }
  // vertices reachable from s in the residual graph
  vector<bool> min_cut(int s){
    vector<bool> vis(n); queue<int> q; q.push(s); vis[s] = true;
    while(!q.empty()){
      int v = q.front(); q.pop();
      for(auto& e : g[v]) if(e.cap > 0 && !vis[e.to]){ vis[e.to] = true; q.push(e.to); }
    }
    return vis;
  }
};

#pragma endregion
#pragma region min cost flow (cost >= 0)
template<class Cap, class Cost> struct mcf_graph{
  struct edge{ int from, to; Cap cap, flow; Cost cost; };
  struct _edge{ int to, rev; Cap cap; Cost cost; };
  int n; vector<vector<_edge>> g; vector<pair<int, int>> pos;
  mcf_graph(int n_ = 0) : n(n_), g(n_){}
  int add_edge(int from, int to, Cap cap, Cost cost){
    int m = pos.size();
    pos.push_back({from, (int)g[from].size()});
    int fi = g[from].size(), ti = g[to].size();
    if(from == to) ti++;
    g[from].push_back({to, ti, cap, cost});
    g[to].push_back({from, fi, 0, -cost});
    return m;
  }
  edge get_edge(int i){
    auto& e = g[pos[i].first][pos[i].second];
    auto& re = g[e.to][e.rev];
    return {pos[i].first, e.to, e.cap + re.cap, re.cap, e.cost};
  }
  vector<edge> edges(){ vector<edge> r; for(int i = 0; i < (int)pos.size(); i++) r.push_back(get_edge(i)); return r; }
  pair<Cap, Cost> flow(int s, int t){ return flow(s, t, numeric_limits<Cap>::max()); }
  pair<Cap, Cost> flow(int s, int t, Cap limit){ return slope(s, t, limit).back(); }
  vector<pair<Cap, Cost>> slope(int s, int t){ return slope(s, t, numeric_limits<Cap>::max()); }
  vector<pair<Cap, Cost>> slope(int s, int t, Cap limit){
    const Cost CINF = numeric_limits<Cost>::max();
    vector<Cost> h(n, 0), dist(n);
    vector<int> pv(n), pe(n);
    vector<pair<Cap, Cost>> res = {{0, 0}};
    Cap fl = 0; Cost cs = 0, prev_per = -1;
    while(fl < limit){
      fill(dist.begin(), dist.end(), CINF);
      dist[s] = 0;
      priority_queue<pair<Cost, int>, vector<pair<Cost, int>>, greater<>> pq;
      pq.push({0, s});
      while(!pq.empty()){
        auto [dv, v] = pq.top(); pq.pop();
        if(dist[v] < dv) continue;
        for(int i = 0; i < (int)g[v].size(); i++){
          auto& e = g[v][i];
          if(e.cap <= 0) continue;
          Cost nd = dv + e.cost + h[v] - h[e.to];
          if(nd < dist[e.to]){ dist[e.to] = nd; pv[e.to] = v; pe[e.to] = i; pq.push({nd, e.to}); }
        }
      }
      if(dist[t] == CINF) break;
      for(int v = 0; v < n; v++) if(dist[v] != CINF) h[v] += dist[v];
      Cap d = limit - fl;
      for(int v = t; v != s; v = pv[v]) d = min(d, g[pv[v]][pe[v]].cap);
      for(int v = t; v != s; v = pv[v]){
        auto& e = g[pv[v]][pe[v]];
        e.cap -= d; g[v][e.rev].cap += d;
      }
      Cost per = h[t] - h[s];
      fl += d; cs += (Cost)d * per;
      if(prev_per == per) res.pop_back();
      res.push_back({fl, cs});
      prev_per = per;
    }
    return res;
  }
};
#pragma endregion
#pragma endregion

#pragma region my library

mt19937_64 rng(10);
vector<int> dx = {1, 0, -1, 0}, dy = {0, 1, 0, -1};
template<class = void> ll ipow(ll x, ll n){
  ll ret = 1;
  while(n > 0){
    if(n & 1) ret *= x;
    n >>= 1;
    if(n > 0) x *= x;
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
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

}