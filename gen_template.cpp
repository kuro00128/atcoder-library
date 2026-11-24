#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define rep(i, n) for (ll i = 0; i < (ll)(n); i++)
mt19937_64 rng;
ll rnd(ll l, ll r){ return uniform_int_distribution<ll>(l, r)(rng); }

int main(int argc, char* argv[]){
  rng.seed(argc > 1 ? atoll(argv[1]) : 0);
  // example: N, then N integers
  ll N = rnd(1, 5);
  cout << N << "\n";
  rep(i, N) cout << rnd(1, 10) << " \n"[i == N - 1];
}