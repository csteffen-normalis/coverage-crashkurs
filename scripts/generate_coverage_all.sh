#!/usr/bin/env bash
set -euo pipefail

lcov \
    --capture \
    --directory . \
    --output-file coverage_all.info

genhtml \
    coverage_all.info \
    --output-directory coverage_all_report

echo "Report generated: coverage_all_report/index.html"
