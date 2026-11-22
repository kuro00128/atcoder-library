#pragma once
#include <bits/stdc++.h>

namespace dbg {
using namespace std;

// ---- count fields of an aggregate struct ----
struct any_t { template<class T> operator T() const; };
template<class T, class... A> consteval int count_fields(){
  if constexpr (requires { T{A{}..., any_t{}}; }) return count_fields<T, A..., any_t>();
  else return sizeof...(A);
}

template <class T> void print(const T& x);
template <class A, class B> void print(const pair<A, B>& p);
template <class... T> void print(const tuple<T...>& t);
inline void print(const string& s) { cerr << '"' << s << '"'; }
inline void print(const char* s) { cerr << '"' << s << '"'; }
inline void print(char c) { cerr << '\'' << c << '\''; }
inline void print(bool b) { cerr << (b ? "true" : "false"); }

template <class... T> void print_list(const T&... a){
  int i = 0; ((cerr << (i++ ? ", " : ""), print(a)), ...);
}
template <class A, class B> void print(const pair<A, B>& p) {
  cerr << '('; print_list(p.first, p.second); cerr << ')';
}
template <class... T> void print(const tuple<T...>& t) {
  cerr << '('; apply([](const auto&... a){ print_list(a...); }, t); cerr << ')';
}
// struct (aggregate, up to 10 fields) -> {a, b, ...}
template <class T> void print_struct(const T& x){
  constexpr int n = count_fields<T>();
  cerr << '{';
  if constexpr (n == 1){ auto& [a] = x; print_list(a); }
  else if constexpr (n == 2){ auto& [a,b] = x; print_list(a,b); }
  else if constexpr (n == 3){ auto& [a,b,c] = x; print_list(a,b,c); }
  else if constexpr (n == 4){ auto& [a,b,c,d] = x; print_list(a,b,c,d); }
  else if constexpr (n == 5){ auto& [a,b,c,d,e] = x; print_list(a,b,c,d,e); }
  else if constexpr (n == 6){ auto& [a,b,c,d,e,f] = x; print_list(a,b,c,d,e,f); }
  else if constexpr (n == 7){ auto& [a,b,c,d,e,f,g] = x; print_list(a,b,c,d,e,f,g); }
  else if constexpr (n == 8){ auto& [a,b,c,d,e,f,g,h] = x; print_list(a,b,c,d,e,f,g,h); }
  else if constexpr (n == 9){ auto& [a,b,c,d,e,f,g,h,i] = x; print_list(a,b,c,d,e,f,g,h,i); }
  else if constexpr (n == 10){ auto& [a,b,c,d,e,f,g,h,i,j] = x; print_list(a,b,c,d,e,f,g,h,i,j); }
  else cerr << "?";
  cerr << '}';
}
template <class T> void print(const T& x) {
  if constexpr (requires { x.val(); }) {
    cerr << x.val();
  } else if constexpr (requires { cerr << x; }) {
    cerr << x;
  } else if constexpr (requires { begin(x); end(x); }) {
    using E = decay_t<decltype(*begin(x))>;
    if constexpr (requires(E e) { begin(e); }) {
      cerr << "\n";
      for (const auto& e : x) { cerr << "  "; print(e); cerr << "\n"; }
    } else {
      cerr << '{';
      bool first = true;
      for (const auto& e : x) { if (!first) cerr << ", "; first = false; print(e); }
      cerr << '}';
    }
  } else if constexpr (is_aggregate_v<T>) {
    print_struct(x);
  } else {
    cerr << "?";
  }
}

template <class... T> void out(const char* names, int line, const T&... xs) {
  cerr << "\033[36m[L" << line << "] " << names << " = ";
  int i = 0;
  ((cerr << (i++ ? " | " : ""), print(xs)), ...);
  cerr << "\033[0m\n";
}
}  // namespace dbg

#define debug(...) dbg::out(#__VA_ARGS__, __LINE__, __VA_ARGS__)