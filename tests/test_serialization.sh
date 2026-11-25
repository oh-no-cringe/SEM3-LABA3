#!/bin/bash
set -eu
mkdir -p data/serialization
check() {
 local kind="$1" prefix="$2" populate="$3" expected="$4"
 for lang in cpp go; do
  app=lr3; other=lr3-go
  if [ "$lang" = go ]; then app=lr3-go; other=lr3; fi
  for format in text binary; do
   ext=txt; if [ "$format" = binary ]; then ext=bin; fi
   path="data/serialization/$lang-$kind.$ext"
   printf '%s\n' "$prefix $populate SAVE $format $path EXIT" | "build/$app" > /dev/null
   printf '%s\n' "$prefix LOAD $format $path PRINT EXIT" | "build/$other" > build/exchange.txt
   grep -Fq -- "$expected" build/exchange.txt
   printf '%s\n' "$prefix LOAD $format $path SAVE $format build/copy.$ext EXIT" | "build/$other" > /dev/null
   cmp "$path" "build/copy.$ext"
  done
 done
}
check array 1 'PUSH "" PUSH "hello world" PUSH яблоко' '[, hello world, яблоко]'
check forward 2 'PUSH first PUSH last' '[first, last]'
check list 3 'PUSH first PUSH last' '[first, last]'
check queue 4 'PUSH first PUSH last' '[first, last]'
check stack 5 'PUSH first PUSH last' '[last, first]'
check hash '6 2' 'SET apple red' apple
check tree 7 'INSERT 10 INSERT -20 INSERT 30 INSERT 40' '[10, -20, 30, 40]'
echo 'сериализация: 28/28 проверок (100%).'
