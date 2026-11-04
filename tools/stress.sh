#!/bin/bash
# usage: stress [count]   (run in a problem folder)
n=${1:-1000}

# create naive.cpp and gen.cpp if missing
if [ ! -f naive.cpp ] || [ ! -f gen.cpp ]; then
  [ -f naive.cpp ] || cp ~/atcoder/template.cpp naive.cpp
  [ -f gen.cpp ] || cp ~/atcoder/gen_template.cpp gen.cpp
  echo "Created naive.cpp and gen.cpp. Write them, then run stress again."
  code naive.cpp gen.cpp
  exit 0
fi

CXX="g++ -std=c++20 -O2 -I $HOME/ac-library"
echo "Compiling..."
$CXX main.cpp -o main_s.out || exit 1
$CXX naive.cpp -o naive_s.out || exit 1
$CXX gen.cpp -o gen_s.out || exit 1

mkdir -p test
for ((i = 1; i <= n; i++)); do
  ./gen_s.out $i > stress.in
  timeout 5 ./main_s.out < stress.in > main.txt 2>/dev/null
  r=$?
  timeout 5 ./naive_s.out < stress.in > naive.txt 2>/dev/null
  if [ $r -ne 0 ] || ! diff -bq main.txt naive.txt > /dev/null; then
    k=1; while [ -f test/stress-$k.in ]; do k=$((k + 1)); done
    cp stress.in test/stress-$k.in
    cp naive.txt test/stress-$k.out
    echo
    echo "Mismatch at seed $i (saved as test/stress-$k)"
    [ $r -ne 0 ] && echo "(main exited with code $r)"
    echo "--- input ---"; cat stress.in
    echo "--- main ---";  cat main.txt
    echo "--- naive ---"; cat naive.txt
    exit 1
  fi
  printf "\rOK %d/%d" $i $n
done
echo
echo "All $n tests passed"