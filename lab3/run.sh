N1=900
N2=58000
let "delta = (N2 - N1)/10"

outputPath="./outputs/output.txt"

rm -f $outputPath

for ((j=2;j<=32;j*=2))
do
	# echo 'threads count = '$j':'
	# for ((i=$N1; i<=$N2;i+=$delta))
	# do
		echo '$j' >> $outputPath
		echo $j'/900'

		./bin/lab3 900 $j >> $outputPath

		echo $j'/29450'
		./bin/lab3 29450 $j >> $outputPath
		
		echo $j'/58000'
		./bin/lab3 58000 $j >> $outputPath

		# echo "$i/$N2"
	# done
done