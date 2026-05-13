#!/bin/bash
#SBATCH --job-name=test_minimal
#SBATCH --output=test_minimal_%j.out
#SBATCH --error=test_minimal_%j.err
#SBATCH --partition=compute
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=00:10:00

cd ~/TemporalStencil/2d-jacobi-9p-blocking

echo "=== Minimal test: NX=32 NY=32 T=8 ==="
echo "This should PASS"
OMP_NUM_THREADS=1 ./exe-2d-jacobi-9p-blocking 32 32 8 32 32 8 2>&1 | grep -E '(Correct|WRONG|vectime)'

echo ""
echo "=== Test: NX=64 NY=32 T=8 ==="
echo "This should PASS"
OMP_NUM_THREADS=1 ./exe-2d-jacobi-9p-blocking 64 32 8 32 32 8 2>&1 | grep -E '(Correct|WRONG|vectime)'

echo ""
echo "=== Test: NX=96 NY=32 T=8 ==="
echo "This should FAIL (starting from x=2)"
OMP_NUM_THREADS=1 ./exe-2d-jacobi-9p-blocking 96 32 8 32 32 8 2>&1 | grep -E '(Correct|WRONG|vectime|x = \[2\])' | head -10
