N1=900
N2=18000
let "delta = (N2 - N1)/10"

seqPath="./outputs/cc-seq-output.txt"
parPath="./outputs/cc-par-output.txt"

rm -f $seqPath
rm -f $parPath

for ((i=$N1; i<=$N2;i+=$delta))
do
	./bin/cc/lab1-seq $i >> $seqPath

	{
		for((j = 2; j <= 4; j += 1))
		do
			./bin/cc/lab1-par-$j $i
		done
	} >> $parPath
	echo -e '\n' >> $parPath

	echo "$i/$N2"
done