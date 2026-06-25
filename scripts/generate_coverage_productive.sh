#!/usr/bin/env bash
set -euo pipefail

lcov \
    --capture \
    --rc lcov_branch_coverage=1 \
    --directory . \
    --output-file coverage.info

lcov \
    --remove coverage.info \
    --rc lcov_branch_coverage=1 \
    '/usr/*' \
    '*/test/*' \
    -o coverage_productive.info

genhtml \
    --branch-coverage \
    coverage_productive.info \
    --output-directory coverage_productive_report

echo "Report generated: coverage_productive_report/index.html"
