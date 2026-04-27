#!/bin/bash
# Compile-time benchmark: compare ebd:: instantiation time between
# ${BENCH_TESTSOURCE_x} ${BENCH_BASELINE_FILE}

set -euo pipefail

BENCH_BASELINE_FILE="benchmark_baseline.cpp"
BENCH_TESTSOURCE_1="benchmark_make_fn.cpp"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
INCLUDE_PATH="${SCRIPT_DIR}/../../include"
CPP_COMPILE="${SCRIPT_DIR}/${BENCH_TESTSOURCE_1}"
CPP_BASELINE="${SCRIPT_DIR}/${BENCH_BASELINE_FILE}"
ANALYZE_SCRIPT="${SCRIPT_DIR}/analyze.py"
REPORT_FILE="${SCRIPT_DIR}/benchmark_report.md"

# Clean previous builds and report
rm -rf "${BUILD_DIR}"
rm -f "${REPORT_FILE}"
mkdir -p "${BUILD_DIR}"

# Check required files
for f in "${CPP_COMPILE}" "${CPP_BASELINE}" "${ANALYZE_SCRIPT}"; do
    if [ ! -f "$f" ]; then
        echo "Error: $f not found!" >&2
        exit 1
    fi
done

# Extract total microsecond value from analyze.py output.
# Output format example: "Total time: 123,456 us (123.456 ms)"
extract_total_us() {
    local output="$1"
    if [[ $output =~ Total\ time:\ ([0-9,]+)\ us ]]; then
        local num="${BASH_REMATCH[1]//,/}"
        echo "$num"
        return 0
    else
        echo "0"
        return 1
    fi
}

# Run a benchmark for a given .cpp file.
# Arguments:
#   $1 - source file path
#   $2 - name for the output object (without extension, e.g. "compile")
# Returns: extracted microseconds (stdout)
run_benchmark() {
    local src_file="$1"
    local out_name="$2"
    local obj_file="${BUILD_DIR}/${out_name}.o"
    local json_file="${BUILD_DIR}/${out_name}.json"

    # Compile with -ftime-trace
    clang++ "${src_file}" \
        -std=c++11 -I"${INCLUDE_PATH}" \
        -ftime-trace -c \
        -o "${obj_file}" > /dev/null 2>&1

    # Run the analyzer on the generated JSON trace
    local raw_output
    raw_output="$(python3 "${ANALYZE_SCRIPT}" "${json_file}" 2>&1)"

    local us
    us="$(extract_total_us "${raw_output}")"
    echo "${us}"
}

# Number of measurement runs
RUNS=30

# Arrays to store results
compile_times=()
baseline_times=()

echo "Running ${RUNS} measurements for ${BENCH_TESTSOURCE_1} ..."
for i in $(seq 1 "${RUNS}"); do
    us="$(run_benchmark "${CPP_COMPILE}" "compile_${i}")"
    compile_times+=("${us}")
    echo "  Run ${i}: ${us} us"
done

echo ""
echo "Running ${RUNS} measurements for ${BENCH_BASELINE_FILE} ..."
for i in $(seq 1 "${RUNS}"); do
    us="$(run_benchmark "${CPP_BASELINE}" "baseline_${i}")"
    baseline_times+=("${us}")
    echo "  Run ${i}: ${us} us"
done

# Compute average using awk (handles integers safely)
avg_compile="$(printf '%s\n' "${compile_times[@]}" | awk '{sum+=$1; count++} END {if(count>0) printf "%.2f", sum/count; else print "0"}')"
avg_baseline="$(printf '%s\n' "${baseline_times[@]}" | awk '{sum+=$1; count++} END {if(count>0) printf "%.2f", sum/count; else print "0"}')"

# Compute ratio (compile / baseline)
ratio="N/A"
if [ "$(echo "${avg_baseline} > 0" | bc)" -eq 1 ]; then
    ratio="$(echo "scale=4; ${avg_compile} / ${avg_baseline}" | bc)"
fi

# Write report
{
    echo "# Compile-Time Benchmark Report"
    echo ""
    echo "## Methodology"
    echo "For each source file, the script compiles it 30 times with \`-ftime-trace\` and extracts the total time spent in template instantiations containing \`ebd::\` (root events only)."
    echo ""
    echo "## Results"
    echo ""
    echo "| Measurement | Average time (μs) |"
    echo "|-------------|------------------:|"
    echo "| \`${BENCH_TESTSOURCE_1}\` | ${avg_compile} |"
    echo "| \`${BENCH_BASELINE_FILE}\`     | ${avg_baseline} |"
    echo ""
    echo "**Ratio (compile / baseline)**: ${ratio}"
    echo ""
    echo "> The baseline represents the cost of a single `make_integer_sequence` instantiation (size 128)."
    echo "> The compile benchmark creates 200 function objects (20 calls × 10 instantiations each), measuring overall template metaprogramming overhead."
} > "${REPORT_FILE}"

echo ""
echo "Benchmark finished. Report saved to ${REPORT_FILE}"
echo "Compile average: ${avg_compile} μs"
echo "Baseline average: ${avg_baseline} μs"
echo "Ratio: ${ratio}"
