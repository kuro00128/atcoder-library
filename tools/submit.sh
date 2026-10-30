#!/bin/bash
# usage: submit.sh ~/atcoder/practice/abc403/a/main.cpp
f="$1"
dir=$(dirname "$f")
if [ -f "$dir/.task" ]; then
  path=$(cat "$dir/.task")
  contest=$(echo "$path" | cut -d/ -f3)
  task=$(basename "$path")
else
  contest=$(basename "$(dirname "$dir")")
  task="${contest}_$(basename "$dir")"
fi
url="https://atcoder.jp/contests/${contest}/submit?taskScreenName=${task}"
/mnt/c/Windows/System32/clip.exe < "$f"
(cd /mnt/c && /mnt/c/Windows/System32/cmd.exe /c start "" "$url")
echo "Copied. Opening: ${task}"
exit 0