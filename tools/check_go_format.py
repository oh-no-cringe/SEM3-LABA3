"""Проверяет gofmt без изменения исходников и сторонних библиотек."""
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[1]
files = [str(p) for p in (root / 'go').rglob('*.go') if 'vendor' not in p.parts]
result = subprocess.run(['gofmt', '-l', *files], check=True, text=True, capture_output=True)
if result.stdout:
    raise SystemExit('Нужно выполнить gofmt для файлов:\n' + result.stdout)
print('gofmt: все собственные Go-файлы оформлены правильно')
