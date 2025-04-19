#!/bin/bash

function lock_and_execute()
{
    local filename=$1
    shift
    local commands="$@"

    # Check if the lock file exists
    if [ ! -f "$filename" ]; then
        echo "Lock file does not exist. Creating it."
        touch "$filename"
    fi

    # Dynamically allocate an available FD
    exec {fd}>>"$filename"

    # Use flock to acquire a lock on the file (blocking mode)
    (
        flock "$fd"  # Wait until the lock is available
        eval "$commands"  # Execute the set of commands
    )
    local returned_code=$? 

    # Explicitly close the FD to release the lock
    exec {fd}>&-
    return $returned_code
}

# aaaa contains
# 1
# 2
# 3
# ... 
# 100

# the task with alone number is available to pick up
# there are multiple users trying to pick up the task simultaneously
# if one user picks up the task, that user has to replace the task like "1 running"
# another user can pick another available task

for i in {1..100}
do
    lock_and_execute "aaaa" "$(cat <<EOF
        if ! grep -q '^$i$' aaaa; then
            # there is no available task
            exit 1
        fi
        # replace the task with '1 running'
        sed -i 's/^$i$/$i running/' aaaa
EOF
)"

    if [ $? -ne 0 ]; then
        continue
    fi

    # Simulate task processing
    echo "Processing task $i..."
    sleep 1
    echo "Task $i completed."
    # Mark the task as completed

    lock_and_execute "aaaa" "$(cat <<EOF
        sed -i 's/^$i running$/$i completed/' aaaa
EOF
)"

done