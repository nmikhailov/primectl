#!/bin/sh
text=$(cat "$1")
echo "#pragma GCC diagnostic ignored \"-Wunused-but-set-variable\"" > "$1"
echo "$text" >> "$1"
echo "#pragma GCC diagnostic pop" >> "$1"
