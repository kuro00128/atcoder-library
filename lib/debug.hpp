#pragma once
#include <bits/stdc++.h>

namespace dbg {
using namespace std;

template <class T> void print(const T& x);
template <class A, class B> void print(const pair<A, B>& p);
template <class... T> void print(const tuple<T...>& t);
inline void print(const string& s) { cerr << '"' << s << '"'; }
inline void print(const char* s) { cerr << '"' << s << '"'; }
inline void print(char c) { cerr << '\'' << c << '\''; }
inline void print(bool b) { cerr << (b ? "true" : "false"); }

template <class A, class B> void print(const pair<A, B>& p) {
  cerr << '('; print(p.first); cerr << ", "; print(p.second); cerr << ')';
}
template <class... T> void print(const tuple<T...>& t) {
  cerr << '(';
  apply([](const auto&... a) { int i = 0; ((cerr << (i++ ? ", " : ""), print(a)), ...); }, t);
  cerr << ')';
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