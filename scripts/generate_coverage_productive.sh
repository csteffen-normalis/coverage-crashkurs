#!/usr/bin/env bash
set -euo pipefail

lcov \
    --capture \
    --directory . \
    --output-file coverage.info

lcov \
    --remove coverage.info \
    '/usr/*' \
    '*/test/*' \
    -o coverage_productive.info

genhtml \
    coverage_productive.info \
    --output-directory coverage_productive_report

echo "Report generated: coverage_productive_report/index.html"
