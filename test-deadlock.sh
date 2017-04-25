#!/bin/bash
total=$7
i=0

while [ $i -lt $total ]; do
	`./proj2 $1 $2 $3 $4 $5 $6 > /dev/null`
	printf "#"
	i=$(($i + 1))
done

echo ""
echo ""
echo "Finished"