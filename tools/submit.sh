#!/bin/bash
# usage: submit.sh ~/atcoder/abc400/a/main.cpp
f="$1"
dir=$(dirname "$f")
task=$(basename "$dir")
contest=$(basename "$(dirname "$dir")")
url="https://atcoder.jp/contests/${contest}/submit?taskScreenName=${contest}_${task}"
/mnt/c/Windows/System32/clip.exe < "$f"
(cd /mnt/c && /mnt/c/Windows/System32/cmd.exe /c start "" "$url")
echo "Copied. Opening: ${contest}_${task}"
exit 0