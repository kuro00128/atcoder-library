#include <bits/stdc++.h>
using namespace std;
using ll = long long;
mt19937_64 rng;
ll rnd(ll l, ll r){ return uniform_int_distribution<ll>(l, r)(rng); }

int main(int argc, char* argv[]){
  rng.seed(argc > 1 ? atoll(argv[1]) : 0);
  // example: N, then N integers
  ll N = rnd(1, 5);
  cout << N << "\n";
  for(ll i = 0; i < N; i++) cout << rnd(1, 10) << " \n"[i == N - 1];
}