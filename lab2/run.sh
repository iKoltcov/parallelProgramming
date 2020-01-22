N1=900
N2=58000
let "delta = (N2 - N1)/10"

outputPath="./outputs/fw-output.txt"

rm -f $outputPath

for ((j=2;j<=32;j*=2))
do
	echo 'threads count = '$j':'
	for ((i=$N1; i<=$N2;i+=$delta))
	do
		./bin/lab2 $i $j >> $outputPath
		echo "$i/$N2"
	done
done