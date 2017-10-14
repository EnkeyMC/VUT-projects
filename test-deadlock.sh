#!/bin/bash
total=$1
i=0

while [ $i -lt $total ]; do
	`./cmake-build-debug/pgrep 3 "Ahoj.*" 1 "[0-9][0-9]* .*" 2 "[^0-9][^0-9]* .*" 3 > /dev/null 2> /dev/null < cmake-build-debug/input.txt`
	printf "#"
	i=$(($i + 1))
done

echo ""
echo ""
echo "Finished"