#!/bin/bash
text1=$(nm -C -D $1 | awk '{ if($2 == "T" || $2 == "i") print $3 }' | sort)
text2=$(nm -C -D $2 | awk '{ if($2 == "T" || $2 == "i") print $3 }' | sort)

diff -w <(echo $text1) <(echo $text2)
