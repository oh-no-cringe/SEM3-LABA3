"""Запускает тесты и выводит процент успешных проверок из XML-отчёта."""

from pathlib import Path
import subprocess
import sys
import tempfile
import xml.etree.ElementTree as ET


def main():
    mode = sys.argv[1]
    with tempfile.TemporaryDirectory(prefix="lr3-tests-") as folder:
        report = Path(folder) / "report.xml"
        if mode == "google":
            command = ["./build/unit_tests", "--gtest_output=xml:" + str(report)]
        else:
            command = ["./build/boost_tests", "--report_format=XML",
                       "--report_level=short", "--report_sink=" + str(report)]

        result = subprocess.run(command, check=False)
        if not report.exists():
            print("Нет отчёта: процент успешных тестов не рассчитан.", flush=True)
            return result.returncode or 1

        root = ET.parse(report).getroot()
        if mode == "google":
            cases = root.findall(".//testcase")
            total = len(cases)
            passed = 0
            for case in cases:
                if (case.get("status") == "run"
                        and case.get("result") != "skipped"
                        and case.find("skipped") is None
                        and case.find("failure") is None
                        and case.find("error") is None):
                    passed += 1
        else:
            suite = root.find("TestSuite")
            passed = int(suite.get("test_cases_passed", "0"))
            failed = int(suite.get("test_cases_failed", "0"))
            skipped = int(suite.get("test_cases_skipped", "0"))
            total = passed + failed + skipped

        if total == 0:
            print("Тесты не запущены: процент не рассчитан.", flush=True)
        else:
            percent = passed * 100 / total
            print(f"\nУспешно пройдено тестов: {passed}/{total} ({percent:.2f}%).",
                  flush=True)
        return result.returncode


if __name__ == "__main__":
    sys.exit(main())
