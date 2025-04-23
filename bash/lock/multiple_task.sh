#!/bin/bash

DATAFILE="data.txt"

for i in {1..100}; do
    ./hardlink.sh "$DATAFILE" "$(cat <<EOF
        if grep -q "^$i" "$DATAFILE"; then
           echo "Task $i is already done"
           exit 1
        fi

        echo "Task $i is free to take"
        echo "$i" >> "$DATAFILE"
        exit 0
EOF
)"
    if [ $? -ne 0 ]; then
        continue
    fi
    sleep 1
    
    ./hardlink.sh "$DATAFILE" "$(cat <<EOF
        sed -i "s/^$i/$i done/" "$DATAFILE"
EOF
)"
done
