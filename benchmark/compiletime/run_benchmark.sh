#!/bin/bash
# Run compile-time benchmark

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
INCLUDE_PATH="${SCRIPT_DIR}/../../include"
CPP_FILE="${SCRIPT_DIR}/compile_time_benchmark.cpp"
ANALYZE_SCRIPT="${SCRIPT_DIR}/analyze.py"

REPORT_FILE="${SCRIPT_DIR}/benchmark_report.md"

# Ensure build directory exists and is clean
rm -rf "${BUILD_DIR}"
rm -f "${REPORT_FILE}"
mkdir -p "${BUILD_DIR}"

# Check required files
for f in "${CPP_FILE}" "${ANALYZE_SCRIPT}"; do
    if [ ! -f "$f" ]; then
        echo "Error: $f not found!" >&2
        exit 1
    fi
done

# Function to extract total microseconds from analyze.py output
# analyze.py prints: "Total time: 123,456 us (123.456 ms)"
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

# Run the benchmark and report result.
# Modified: returns the extracted microseconds via stdout,
#           and prints verbose info to stderr.
run() {
  # Print raw output to stderr (so it won't be captured)
  clang++ ${CPP_FILE} \
    -std=c++11 -I${INCLUDE_PATH} \
    -ftime-trace -c \
    -o ${BUILD_DIR}/compile_time_benchmark.o

  echo "### Case $1" >> ${REPORT_FILE}
  raw_output=$(python3 ${ANALYZE_SCRIPT} ${BUILD_DIR}/compile_time_benchmark.json)
  echo "${raw_output}" >&2   # to stderr for debugging
  analyze_result=$(extract_total_us "${raw_output}")
  echo "Using \`${analyze_result}\` us to instantiate 'ebd::*'" >> ${REPORT_FILE}
  
  # Output the numerical result (stdout) for collection
  echo "${analyze_result}"
}

# Array to store results from each run
results=()

for i in {1..30}; do
    us=$(run ${i})          # Capture the returned microseconds
    results+=("${us}")      # Store into array
done

# Calculate average using awk (handles integer arithmetic safely)
# Also handle empty or zero values.
avg=$(printf '%s\n' "${results[@]}" | awk '{sum+=$1; count++} END {if(count>0) printf "%.2f", sum/count; else print "0"}')

# Append average and explanation to report file
{
    echo ""
    echo "## Summary"
    echo "Average compile time over 10 runs: **${avg} us**"
    echo "> Note: The values above are extracted from \`analyze.py\` output (microseconds)."
    echo "> The average is computed using awk, ignoring any non-numeric lines."
} >> "${REPORT_FILE}"

echo "Benchmark finished. Report saved to ${REPORT_FILE}"
