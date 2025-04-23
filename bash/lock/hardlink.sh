#!/bin/bash
# nfs_lock.sh — Run a command with NFS-safe locking on a target file

LOCK_RETRY_DELAY=0.1  # seconds
LOCK_TIMEOUT=5     # optional overall timeout (can be enhanced to param later)

target_file=$1
cmd=$2

lockdir="$(dirname "$target_file")/.locks"
lockname="$(basename "$target_file")"
lockfile="$lockdir/${lockname}.lock"
mylock="$lockdir/${lockname}.$$"
start_time=$(date +%s)

trap 'echo "trap called" ; rm -f "$lockfile" "$mylock"' EXIT

echo "target $target_file"
echo "lockdir $lockdir"
echo "lockname $lockname"
echo "cmd $cmd"
mkdir -p "$lockdir"
touch $mylock

while ! ln "$mylock" "$lockfile" 2>/dev/null; do
# Check for stale lock
now=$(date +%s)

# Timeout handling
if (( now - start_time > LOCK_TIMEOUT )); then
    echo "❌ Timeout waiting for lock on $target_file"
    exit 1
fi

sleep "$LOCK_RETRY_DELAY"
done

echo "✅ Lock acquired on $target_file by PID $$"
echo "Running command: $cmd"
eval "$cmd"
