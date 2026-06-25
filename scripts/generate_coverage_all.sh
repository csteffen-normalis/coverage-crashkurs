#!/usr/bin/env bash
set -euo pipefail

lcov \
    --capture \
    --rc lcov_branch_coverage=1 \
    --directory . \
    --output-file coverage_all.info

genhtml \
    --branch-coverage \
    coverage_all.info \
    --output-directory coverage_all_report

echo "Report generated: coverage_all_report/index.html"
