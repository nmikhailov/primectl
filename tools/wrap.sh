#!/bin/sh
for file in $1
do
    text=$(cat "$file")
    echo "#pragma GCC diagnostic ignored \"-Wunused-but-set-variable\"" > "$file"
    echo "#pragma GCC diagnostic ignored \"-pedantic\"" >> "$file"
    echo "$text" >> "$file"
    echo "#pragma GCC diagnostic pop" >> "$file"
done
