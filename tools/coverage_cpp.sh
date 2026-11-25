#!/bin/bash
set -eu
mkdir -p build/coverage reports/cpp
for method in google boost catch; do
 LLVM_PROFILE_FILE="build/coverage/$method.profraw" "build/coverage/$method" > "build/coverage/$method.log" 2>&1
done
check() {
 local id="$1" input="$2" expected="$3" status=0
 printf '%s\n' "$input" | LLVM_PROFILE_FILE="build/coverage/cli-$id.profraw" build/coverage/app > build/coverage/output.txt 2> build/coverage/error.txt || status=$?
 if [ "$expected" = '__ERROR__' ]; then
  [ "$status" -ne 0 ] && grep -q 'Ошибка:' build/coverage/error.txt
 else
  [ "$status" -eq 0 ] && grep -Fq -- "$expected" build/coverage/output.txt
 fi
}
check 0 '1 EMPTY SIZE PRINT PUSH a PUSH b INSERT 1 c GET 1 SET 0 z DEL 1 PRINT EXIT' '[z, b]'
check 1 '1 PUSH "hello world" GET 0 EXIT' 'hello world'
check 2 '2 EMPTY PRINT PUSH a PUSH b FRONT z INSERT 1 c FIND c FIND x DEL 1 DEL_VALUE a POP_BACK POP_FRONT EXIT' FALSE
check 3 '3 EMPTY PRINT PUSH a PUSH b FRONT z INSERT 1 c FIND c FIND x DEL 1 DEL_VALUE a POP_BACK POP_FRONT EXIT' FALSE
check 4 '4 POP EMPTY PRINT PUSH a PUSH b POP PRINT POP EMPTY EXIT' TRUE
check 5 '5 POP EMPTY PRINT PUSH a PUSH b POP PRINT POP EMPTY EXIT' TRUE
check 6 '6 1 PRINT SET a red SET b blue GET a GET x HAS a HAS x DEL a PRINT EXIT' NOT_FOUND
check 7 '6 2 SET a red GET a DEL a HAS a EXIT' FALSE
check 8 '7 EMPTY CHECK PRINT INSERT 10 INSERT 20 INSERT 30 INSERT 40 FIND 40 FIND 99 PRE IN POST CHECK EXIT' '[40, 20, 30, 10]'
check 9 '1 PUSH a PUSH b SAVE text build/coverage/archive-1-text LOAD text build/coverage/archive-1-text PRINT EXIT' OK
check 10 '1 PUSH a PUSH b SAVE binary build/coverage/archive-1-binary LOAD binary build/coverage/archive-1-binary PRINT EXIT' OK
check 11 '1 SAVE json unused' __ERROR__
check 12 '1 LOAD text build/missing-archive' __ERROR__
check 13 '2 PUSH a PUSH b SAVE text build/coverage/archive-2-text LOAD text build/coverage/archive-2-text PRINT EXIT' OK
check 14 '2 PUSH a PUSH b SAVE binary build/coverage/archive-2-binary LOAD binary build/coverage/archive-2-binary PRINT EXIT' OK
check 15 '2 SAVE json unused' __ERROR__
check 16 '2 LOAD text build/missing-archive' __ERROR__
check 17 '3 PUSH a PUSH b SAVE text build/coverage/archive-3-text LOAD text build/coverage/archive-3-text PRINT EXIT' OK
check 18 '3 PUSH a PUSH b SAVE binary build/coverage/archive-3-binary LOAD binary build/coverage/archive-3-binary PRINT EXIT' OK
check 19 '3 SAVE json unused' __ERROR__
check 20 '3 LOAD text build/missing-archive' __ERROR__
check 21 '4 PUSH a PUSH b SAVE text build/coverage/archive-4-text LOAD text build/coverage/archive-4-text PRINT EXIT' OK
check 22 '4 PUSH a PUSH b SAVE binary build/coverage/archive-4-binary LOAD binary build/coverage/archive-4-binary PRINT EXIT' OK
check 23 '4 SAVE json unused' __ERROR__
check 24 '4 LOAD text build/missing-archive' __ERROR__
check 25 '5 PUSH a PUSH b SAVE text build/coverage/archive-5-text LOAD text build/coverage/archive-5-text PRINT EXIT' OK
check 26 '5 PUSH a PUSH b SAVE binary build/coverage/archive-5-binary LOAD binary build/coverage/archive-5-binary PRINT EXIT' OK
check 27 '5 SAVE json unused' __ERROR__
check 28 '5 LOAD text build/missing-archive' __ERROR__
check 29 '6 2 SET apple red SAVE text build/coverage/archive-6-text LOAD text build/coverage/archive-6-text PRINT EXIT' OK
check 30 '6 2 SET apple red SAVE binary build/coverage/archive-6-binary LOAD binary build/coverage/archive-6-binary PRINT EXIT' OK
check 31 '6 2 SAVE json unused' __ERROR__
check 32 '6 2 LOAD text build/missing-archive' __ERROR__
check 33 '7 INSERT 10 INSERT 20 SAVE text build/coverage/archive-7-text LOAD text build/coverage/archive-7-text PRINT EXIT' OK
check 34 '7 INSERT 10 INSERT 20 SAVE binary build/coverage/archive-7-binary LOAD binary build/coverage/archive-7-binary PRINT EXIT' OK
check 35 '7 SAVE json unused' __ERROR__
check 36 '7 LOAD text build/missing-archive' __ERROR__
check 37 1 ''
check 38 2 ''
check 39 3 ''
check 40 4 ''
check 41 5 ''
check 42 '6 1' ''
check 43 7 ''
check 44 '' __ERROR__
check 45 x __ERROR__
check 46 9 __ERROR__
check 47 '1 GET 9' __ERROR__
check 48 '1 GET -1' __ERROR__
check 49 '1 GET 1x' __ERROR__
check 50 '1 PUSH' __ERROR__
check 51 '1 INSERT' __ERROR__
check 52 '1 SET 0 a' __ERROR__
check 53 '1 WHAT' __ERROR__
check 54 '2 WHAT' __ERROR__
check 55 '3 WHAT' __ERROR__
check 56 '4 WHAT' __ERROR__
check 57 '5 WHAT' __ERROR__
check 58 6 __ERROR__
check 59 '6 9' __ERROR__
check 60 '6 1 WHAT' __ERROR__
check 61 '7 WHAT' __ERROR__
check 62 '7 INSERT x' __ERROR__
check 63 '1 PUSH "unclosed' __ERROR__
check 64 '1 PUSH "a"b' __ERROR__
profiles=(build/coverage/google.profraw build/coverage/boost.profraw build/coverage/catch.profraw)
for ((i=0;i<65;i++)); do profiles+=("build/coverage/cli-$i.profraw"); done
xcrun llvm-profdata merge -sparse "${profiles[@]}" -o build/coverage/all.profdata
common=(build/coverage/google -object=build/coverage/boost -object=build/coverage/catch -object=build/coverage/app -instr-profile=build/coverage/all.profdata '-ignore-filename-regex=(third_party|tests|Applications)')
xcrun llvm-cov show "${common[@]}" -format=html -output-dir=reports/cpp -show-line-counts-or-regions -show-branches=count
xcrun llvm-cov report "${common[@]}" > reports/cpp/summary.txt
xcrun llvm-cov export "${common[@]}" | jq '.data[0].totals' > reports/cpp/summary.json
cat reports/cpp/summary.txt
jq -e '.lines.percent >= 85' reports/cpp/summary.json > /dev/null
