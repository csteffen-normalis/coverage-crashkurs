#!/usr/bin/env bash
set -euo pipefail

rm -f coverage.info
rm -f coverage_all.info
rm -f coverage_productive.info

rm -rf coverage_all_report
rm -rf coverage_productive_report

find . -name "*.gcda" -delete
find . -name "*.gcno" -delete
find . -name "*.gcov" -delete
