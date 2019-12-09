N1=900
N2=18000
let "delta = (N2 - N1)/10"

outputPath="./outputs/output.txt"

rm -f $outputPath

for ((j=1;j<=4;j++))
do
	echo 'threads count = '$j':'
	for ((i=$N1; i<=$N2;i+=$delta))
	do
		./bin/lab2 $i $j >> $outputPath

		echo "$i/$N2"
	done
done