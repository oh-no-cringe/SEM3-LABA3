"""Проверяет реальное покрытие исполняемых операторов Go."""
from pathlib import Path
import json

root = Path(__file__).resolve().parents[1]
covered = total = 0
for line in (root / 'reports/go/coverage.out').read_text().splitlines()[1:]:
    position, statements, count = line.split()
    statements = int(statements)
    total += statements
    if int(count) > 0:
        covered += statements
percent = covered * 100 / total if total else 0
(root / 'reports/go/summary.json').write_text(json.dumps({
    'statements': {'count': total, 'covered': covered, 'percent': percent}}, indent=2))
print(f'Go: покрытие операторов {percent:.2f}% ({covered}/{total})')
if percent < 85:
    raise SystemExit('Покрытие Go ниже 85%')
