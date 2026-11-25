#!/bin/sh
set -eu
files=$(find go -path go/vendor -prune -o -name '*.go' -print)
result=$(gofmt -l $files)
if [ -n "$result" ]; then printf '%s\n' "$result"; exit 1; fi
echo 'gofmt: все собственные файлы оформлены правильно'
