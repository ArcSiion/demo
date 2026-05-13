#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"

make big

LOG=${LOG:-result_big.txt}
WARMUP=${WARMUP:-1}
BIG_CHECK_RTOL=${BIG_CHECK_RTOL:-1e-10}
BIG_CHECK_ATOL=${BIG_CHECK_ATOL:-1e-12}
export BIG_CHECK_RTOL BIG_CHECK_ATOL

BIG_NX=${BIG_NX:-6144}
BIG_NY=${BIG_NY:-4096}
BIG_T=${BIG_T:-36000}
BIG_XB=${BIG_XB:-1536}
BIG_YB=${BIG_YB:-1024}
BIG_TB=${BIG_TB:-256}

{
	echo "===== big blocking test ====="
	date
	echo "OMP_NUM_THREADS=${OMP_NUM_THREADS:-default}"
	echo "OMP_PROC_BIND=${OMP_PROC_BIND:-unset}"
	echo "OMP_PLACES=${OMP_PLACES:-unset}"
	echo "BIG_CHECK_RTOL=$BIG_CHECK_RTOL"
	echo "BIG_CHECK_ATOL=$BIG_CHECK_ATOL"
	echo "log=$LOG"
	echo
} | tee "$LOG"

if [ "$WARMUP" != "0" ]; then
	{
		echo "===== warmup ====="
		echo "./exe-2d-shapiro-25p-blocking-big 4125 2125 1162 1536 1024 256"
	} | tee -a "$LOG"
	./exe-2d-shapiro-25p-blocking-big 4125 2125 1162 1536 1024 256 | tee -a "$LOG"
	echo | tee -a "$LOG"
fi

{
	echo "===== long run, expected to exceed 30 minutes on the current machine ====="
	echo "./exe-2d-shapiro-25p-blocking-big $BIG_NX $BIG_NY $BIG_T $BIG_XB $BIG_YB $BIG_TB"
} | tee -a "$LOG"

./exe-2d-shapiro-25p-blocking-big \
	"$BIG_NX" "$BIG_NY" "$BIG_T" "$BIG_XB" "$BIG_YB" "$BIG_TB" | tee -a "$LOG"

{
	echo
	date
	echo "===== done ====="
} | tee -a "$LOG"
