#!/bin/sh

header=$(cat <<EOF
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wextra-semi"
#pragma clang diagnostic ignored "-Wmismatched-tags"
#else // gcc
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-pedantic"
#endif
EOF
)

footer=$(cat <<EOF
#ifdef __clang__
#pragma clang diagnostic pop
#else
#pragma GCC diagnostic pop
#endif
EOF
)

for file in $@
do
    text=$(cat "$file")

    echo "$header" > "$file"
    echo "$text" >> "$file"
    echo "$footer" >> "$file"
done
