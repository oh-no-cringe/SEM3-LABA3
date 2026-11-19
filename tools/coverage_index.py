"""Общая стартовая страница двух HTML-отчётов."""
from pathlib import Path
import datetime
import json

root = Path(__file__).resolve().parents[1] / 'reports'
cpp = json.loads((root / 'cpp/summary.json').read_text())
go = json.loads((root / 'go/summary.json').read_text())
(root / 'index.html').write_text(f'''<!doctype html>
<html lang="ru"><meta charset="utf-8"><meta name="viewport" content="width=device-width">
<title>Покрытие тестами ЛР3</title>
<style>body{{font:18px/1.6 system-ui,sans-serif;max-width:850px;margin:60px auto;padding:0 24px;color:#202630;background:#f7f8fa}}h1{{font-size:32px}}table{{border-collapse:collapse;width:100%;background:white}}td,th{{text-align:left;padding:16px;border:1px solid #ccd2da}}a{{color:#185bc0}}small{{color:#535e6c}}</style>
<h1>ЛР3 — покрытие тестами</h1>
<p>Собственные исходники C++ и Go, включая консольную часть. Библиотеки и тестовые файлы не входят в расчёт.</p>
<table><tr><th>Версия</th><th>Метрика</th><th>Покрытие</th></tr>
<tr><td><a href="cpp/index.html">C++ — открыть отчёт</a></td><td>Исполняемые строки</td><td>{cpp['lines']['percent']:.2f}%</td></tr>
<tr><td><a href="go/index.html">Go — открыть отчёт</a></td><td>Исполняемые операторы</td><td>{go['statements']['percent']:.2f}%</td></tr></table>
<p>Порог для каждой версии — 85%. Это покрытие кода, а не процент успешных тестов. Метрики языков различаются.</p>
<p>C++: GoogleTest, Boost.Test и консольные сценарии. Go: стандартный testing с проверками testify.</p>
<p><a href="cpp/summary.txt">Сводка C++</a> · <a href="go/summary.txt">Сводка Go</a></p>
<small>Повторная генерация из папки проекта: <code>make coverage</code>.</small></html>''')
print(root / 'index.html')
