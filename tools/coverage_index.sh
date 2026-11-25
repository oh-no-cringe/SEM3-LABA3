#!/bin/sh
set -eu
cpp=$(jq -r '.lines.percent' reports/cpp/summary.json)
go=$(jq -r '.statements.percent' reports/go/summary.json)
cat > reports/index.html <<EOF
<!doctype html><html lang="ru"><meta charset="utf-8"><title>Покрытие ЛР3</title>
<style>body{font:20px/1.6 sans-serif;max-width:850px;margin:60px auto}td,th{padding:16px;border:1px solid #aaa}table{border-collapse:collapse}</style>
<h1>Покрытие тестами ЛР3</h1><p>Собственные исходники, включая меню. Тесты и библиотеки исключены.</p>
<table><tr><th>Версия</th><th>Метрика</th><th>Покрытие</th></tr>
<tr><td><a href="cpp/index.html">C++</a></td><td>Строки</td><td>$cpp%</td></tr>
<tr><td><a href="go/index.html">Go</a></td><td>Операторы</td><td>$go%</td></tr></table>
<p>Порог — 85%. Повторная генерация: make coverage.</p></html>
EOF
