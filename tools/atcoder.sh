# ============================================================
#  AtCoder / competitive programming commands
#  loaded from ~/.bashrc:  source ~/atcoder/tools/atcoder.sh
# ============================================================

# ---------- settings (uncomment to change defaults) ----------
# export ATC_OPEN=0     # do not open the browser by default (-o to open)
# export ATC_NOACL=1    # use the no-ACL template by default (-a for ACL)
export ASAN_OPTIONS=handle_abort=1

ATC_DIR=~/atcoder
ATC_LETTERS=abcdefghijklmnopqrstuvwxyz
ATC_CXXFLAGS="-include $HOME/pch/all.h -Winvalid-pch -std=c++20 -O0 -g -fno-omit-frame-pointer -Wall -Wextra -Wno-sign-compare -Wno-unknown-pragmas -DLOCAL -D_GLIBCXX_DEBUG -fsanitize=address,undefined -I $HOME/ac-library -I $ATC_DIR/lib"

# ============================================================
#  internal helpers
# ============================================================

# options: -n no browser / -o open browser / -x no-ACL template / -a ACL template
_atc_opts() {
  ATC_DO_OPEN=${ATC_OPEN:-1}
  ATC_TPL=$ATC_DIR/template.cpp
  [ "${ATC_NOACL:-0}" = 1 ] && ATC_TPL=$ATC_DIR/template_noacl.cpp
  ATC_ARGS=()
  for a in "$@"; do
    case "$a" in
      -n) ATC_DO_OPEN=0 ;;
      -o) ATC_DO_OPEN=1 ;;
      -x) ATC_TPL=$ATC_DIR/template_noacl.cpp ;;
      -a) ATC_TPL=$ATC_DIR/template.cpp ;;
      *) ATC_ARGS+=("$a") ;;
    esac
  done
}

# open a URL in the Windows browser
_atc_url() {
  (cd /mnt/c && /mnt/c/Windows/System32/cmd.exe /c start "" "$1")
}
_atc_open() {
  [ "$ATC_DO_OPEN" = 1 ] && _atc_url "$1"
}

# AtCoder login cookie (saved in ~/.atcoder_session)
_atc_cookie() {
  [ -f ~/.atcoder_session ] && echo "REVEL_SESSION=$(cat ~/.atcoder_session)"
}

# list task paths of a contest in order
_atc_tasks() {
  curl -s -b "$(_atc_cookie)" "https://atcoder.jp/contests/$1/tasks" \
    | grep -o "/contests/$1/tasks/[A-Za-z0-9_]*" | awk '!seen[$0]++'
}

# download samples of a task URL into ./test
_atc_samples() {
  curl -s -b "$(_atc_cookie)" "$1" | python3 $ATC_DIR/tools/samples.py
}

# ============================================================
#  create problems
# ============================================================

# practice: ac abc403 a [-n] [-x]
ac() {
  _atc_opts "$@"; set -- "${ATC_ARGS[@]}"
  [ -z "$1" ] && { echo "usage: ac abc403 a [-n] [-x]"; return 1; }
  local c=$1 t=${2:-a}
  local i=$(expr index "$ATC_LETTERS" "$t")
  local path=$(_atc_tasks "$c" | sed -n "${i}p")
  [ -z "$path" ] && { echo "task not found: $c $t"; return 1; }
  local url="https://atcoder.jp$path"
  local dir=$ATC_DIR/practice/$c/$t
  mkdir -p "$dir" && cd "$dir"
  echo "$path" > .task
  [ -f main.cpp ] || cp "$ATC_TPL" main.cpp
  [ -d test ] || _atc_samples "$url"
  _atc_open "$url"
  code main.cpp
}

# contest: con abc430 [-n] [-x]
con() {
  _atc_opts "$@"; set -- "${ATC_ARGS[@]}"
  [ -z "$1" ] && { echo "usage: con abc430 [-n] [-x]"; return 1; }
  local c=$1 base=$ATC_DIR/contest/$1 i=0
  local paths=$(_atc_tasks "$c")
  [ -z "$paths" ] && { echo "contest not found or not started: $c"; return 1; }
  for path in $paths; do
    local t=${ATC_LETTERS:$i:1}
    i=$((i + 1))
    mkdir -p "$base/$t"
    echo "$path" > "$base/$t/.task"
    [ -f "$base/$t/main.cpp" ] || cp "$ATC_TPL" "$base/$t/main.cpp"
    ( cd "$base/$t" && { [ -d test ] || _atc_samples "https://atcoder.jp$path"; } )
  done
  cd "$base/a"
  _atc_open "https://atcoder.jp/contests/$c/tasks"
  code main.cpp
}

# other sites (no-ACL template by default): other cf2050a [-a]
other() {
  _atc_opts -x "$@"; set -- "${ATC_ARGS[@]}"
  [ -z "$1" ] && { echo "usage: other name [-a]"; return 1; }
  local dir=$ATC_DIR/other/$1
  mkdir -p "$dir" && cd "$dir"
  [ -f main.cpp ] || cp "$ATC_TPL" main.cpp
  code main.cpp
}

# scratch: sc / sc name [-x]
sc() {
  _atc_opts "$@"; set -- "${ATC_ARGS[@]}"
  local dir=$ATC_DIR/scratch${1:+/$1}
  mkdir -p "$dir" && cd "$dir"
  [ -f main.cpp ] || cp "$ATC_TPL" main.cpp
  code main.cpp
}

# ============================================================
#  move between problems
# ============================================================

# go abc403 b / go abc403 / go scratch
go() {
  local target=""
  if [ "$1" = scratch ]; then
    target=$ATC_DIR/scratch
  else
    for base in $ATC_DIR/practice $ATC_DIR/contest $ATC_DIR/other; do
      if [ -d "$base/$1/${2:-a}" ]; then target="$base/$1/${2:-a}"; break; fi
      if [ -z "$2" ] && [ -f "$base/$1/main.cpp" ]; then target="$base/$1"; break; fi
    done
  fi
  [ -z "$target" ] && { echo "not found: $1 ${2:-a}"; return 1; }
  cd "$target" && code main.cpp
}

# next / previous problem in the same contest
_atc_move() {
  local cur=$(basename "$PWD") parent=$(dirname "$PWD")
  local i=$(expr index "$ATC_LETTERS" "$cur")
  [ ${#cur} -ne 1 ] || [ "$i" -eq 0 ] && { echo "not in a problem folder"; return 1; }
  local j=$((i - 1 + $1))
  [ "$j" -lt 0 ] && { echo "no problem there"; return 1; }
  local t=${ATC_LETTERS:$j:1}
  [ -d "$parent/$t" ] || { echo "no problem there"; return 1; }
  cd "$parent/$t" && code main.cpp
}
nx() { _atc_move 1; }
pv() { _atc_move -1; }

# ============================================================
#  build / run / test
# ============================================================

# compile & run main.cpp:  cr / cr < in.txt
cr()  { g++ $ATC_CXXFLAGS main.cpp -o a.out && ./a.out "$@"; }
# compile & run naive.cpp: crn / crn < test/stress-1.in
crn() { g++ $ATC_CXXFLAGS naive.cpp -o naive.out && ./naive.out "$@"; }
alias r='./a.out'
alias ri='./a.out < in.txt'

# random test (main.cpp vs naive.cpp): stress [count]
alias stress="$ATC_DIR/tools/stress.sh"

# reset main.cpp to the template: tpl / tpl -x
tpl() {
  _atc_opts "$@"
  cp "$ATC_TPL" main.cpp && echo "reset: $PWD/main.cpp ($(basename "$ATC_TPL"))"
}
tpln() { tpl -x; }

# copy main.cpp to Windows Downloads (for file upload): todl / todl answer.cpp
todl() {
  local win=$(wslpath "$(cd /mnt/c && cmd.exe /c 'echo %USERPROFILE%' 2>/dev/null | iconv -f cp932 -t utf-8 | tr -d '\r')")
  local dst="$win/Downloads/${1:-main.cpp}"
  cp main.cpp "$dst" && echo "Copied to: $dst"
}

# ============================================================
#  delete
# ============================================================

# del abc403 / del abc403 b
del() {
  [ -z "$1" ] && { echo "usage: del abc403 [b]"; return 1; }
  local found=0
  for base in $ATC_DIR/practice $ATC_DIR/contest $ATC_DIR/other; do
    local target="$base/$1${2:+/$2}"
    if [ -d "$target" ]; then
      found=1
      read -p "delete $target ? [y/N] " a
      if [ "$a" = y ]; then
        case "$PWD" in "$target"*) cd $ATC_DIR ;; esac
        rm -rf "$target" && echo "deleted: $target"
      fi
    fi
  done
  [ $found -eq 0 ] && echo "not found: $1 $2"
}

# clean practice|contest|other|scratch  (scratch keeps scratch/main.cpp)
clean() {
  case "$1" in practice|contest|other|scratch) ;; *) echo "usage: clean practice|contest|other|scratch"; return 1 ;; esac
  local dir=$ATC_DIR/$1 keep=""
  [ "$1" = scratch ] && keep="$dir/main.cpp"
  local n=$(find "$dir" -mindepth 1 -maxdepth 1 ! -path "$keep" 2>/dev/null | wc -l)
  [ "$n" -eq 0 ] && { echo "$dir is already clean"; return 0; }
  read -p "delete $n items in $dir ? [y/N] " a
  [ "$a" = y ] || { echo "canceled"; return 0; }
  case "$PWD" in "$dir"/*) cd $ATC_DIR ;; esac
  find "$dir" -mindepth 1 ! -path "$keep" -delete && echo "cleaned: $dir"
}

# ============================================================
#  Tab completion
# ============================================================
_atc_names() {
  local names=$(basename -a $ATC_DIR/practice/*/ $ATC_DIR/contest/*/ $ATC_DIR/other/*/ 2>/dev/null | sort -u)
  COMPREPLY=($(compgen -W "scratch $names" -- "${COMP_WORDS[1]}"))
}
_atc_sc_names() {
  local names=$(cd $ATC_DIR/scratch 2>/dev/null && ls -d */ 2>/dev/null | tr -d /)
  COMPREPLY=($(compgen -W "$names" -- "${COMP_WORDS[1]}"))
}
complete -F _atc_names go
complete -F _atc_names del
complete -F _atc_sc_names sc
complete -W "practice contest other scratch" clean

# ============================================================
#  help: atc
# ============================================================
atc() {
  cat << 'EOF'
create   ac abc403 a [-n][-x]   1 problem (practice)
         con abc430 [-n][-x]    all problems (contest)
         other name [-a]        other sites (no-ACL)
         sc [name] [-x]         scratch
move     go abc403 b / nx / pv
run      cr / cr < in.txt / r / ri / crn
test     stress [count]
reset    tpl [-x] / tpln
file     todl [name]
delete   del abc403 [b] / clean practice|contest|other|scratch
options  -n no browser  -o open browser  -x no-ACL  -a ACL
EOF
}