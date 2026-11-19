"""HTML-покрытие C++: GoogleTest, Boost.Test и проверенные консольные сценарии."""
from pathlib import Path
import json
import os
import subprocess

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/coverage'
REPORT = ROOT / 'reports/cpp'


def run(args, **kwargs):
    return subprocess.run([str(x) for x in args], cwd=ROOT, check=True, **kwargs)


def main():
    BUILD.mkdir(parents=True, exist_ok=True)
    REPORT.mkdir(parents=True, exist_ok=True)
    profiles = []
    for method in ['google', 'boost']:
        profile = BUILD / (method + '.profraw')
        env = dict(os.environ, LLVM_PROFILE_FILE=str(profile))
        with (BUILD / (method + '.log')).open('w') as log:
            run([BUILD / method], env=env, stdout=log, stderr=subprocess.STDOUT)
        profiles.append(profile)

    cases = [
        ('1 EMPTY SIZE PRINT PUSH a PUSH b INSERT 1 c GET 1 SET 0 z DEL 1 PRINT EXIT', '[z, b]'),
        ('1 PUSH "hello world" GET 0 EXIT', 'hello world'),
        ('2 EMPTY PRINT PUSH a PUSH b FRONT z INSERT 1 c FIND c FIND x DEL 1 DEL_VALUE a POP_BACK POP_FRONT EXIT', 'FALSE'),
        ('3 EMPTY PRINT PUSH a PUSH b FRONT z INSERT 1 c FIND c FIND x DEL 1 DEL_VALUE a POP_BACK POP_FRONT EXIT', 'FALSE'),
        ('4 POP EMPTY PRINT PUSH a PUSH b POP PRINT POP EMPTY EXIT', 'TRUE'),
        ('5 POP EMPTY PRINT PUSH a PUSH b POP PRINT POP EMPTY EXIT', 'TRUE'),
        ('6 1 PRINT SET a red SET b blue GET a GET x HAS a HAS x DEL a PRINT EXIT', 'NOT_FOUND'),
        ('6 2 SET a red GET a DEL a HAS a EXIT', 'FALSE'),
        ('7 EMPTY CHECK PRINT INSERT 10 INSERT 20 INSERT 30 INSERT 40 FIND 40 FIND 99 PRE IN POST CHECK EXIT', '[40, 20, 30, 10]'),
    ]
    for prefix in ['1', '2', '3', '4', '5', '6 1', '7']:
        cases.append((prefix, ''))
    errors = ['', 'x', '9', '1 GET 9', '1 GET -1', '1 GET 1x', '1 PUSH',
              '1 INSERT', '1 SET 0 a', '1 WHAT', '2 WHAT', '3 WHAT', '4 WHAT',
              '5 WHAT', '6', '6 9', '6 1 WHAT', '7 WHAT', '7 INSERT x',
              '1 PUSH "unclosed', '1 PUSH "a"b']
    for data in errors:
        cases.append((data, None))
    for i, (data, expected) in enumerate(cases):
        profile = BUILD / f'cli-{i}.profraw'
        env = dict(os.environ, LLVM_PROFILE_FILE=str(profile))
        result = subprocess.run([str(BUILD / 'app')], cwd=ROOT, input=data + '\n',
                                text=True, capture_output=True, env=env)
        if expected is None:
            assert result.returncode != 0 and 'Ошибка:' in result.stderr, (data, result)
        else:
            assert result.returncode == 0 and expected in result.stdout, (data, result)
        profiles.append(profile)
    merged = BUILD / 'all.profdata'
    run(['xcrun', 'llvm-profdata', 'merge', '-sparse', *profiles, '-o', merged])
    ignore = '-ignore-filename-regex=(third_party|tests|Applications)'
    common = [BUILD / 'google', '-object=' + str(BUILD / 'boost'),
              '-object=' + str(BUILD / 'app'), '-instr-profile=' + str(merged), ignore]
    run(['xcrun', 'llvm-cov', 'show', *common, '-format=html',
         '-output-dir=' + str(REPORT), '-show-line-counts-or-regions', '-show-branches=count'])
    report = run(['xcrun', 'llvm-cov', 'report', *common], capture_output=True, text=True)
    (REPORT / 'summary.txt').write_text(report.stdout)
    result = run(['xcrun', 'llvm-cov', 'export', *common], capture_output=True, text=True)
    totals = json.loads(result.stdout)['data'][0]['totals']
    (REPORT / 'summary.json').write_text(json.dumps(totals, indent=2))
    print(report.stdout)
    print(f"C++: покрытие строк {totals['lines']['percent']:.2f}%; ветвей {totals['branches']['percent']:.2f}%")
    print(f'HTML: {REPORT / "index.html"}')
    if totals['lines']['percent'] < 85:
        raise SystemExit('Покрытие строк ниже 85%')


if __name__ == '__main__':
    main()
