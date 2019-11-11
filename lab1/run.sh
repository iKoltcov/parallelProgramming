N1=14000
N2=192500
let "delta = (N2 - N1)/10"

echo 'gcc run'

seqPath="./outputs/gcc-seq-output.txt"
parPath="./outputs/gcc-par-output.txt"

rm -f $seqPath
rm -f $parPath

for ((i=$N1; i<=$N2;i+=$delta))
do
	./bin/gcc/lab1-seq $i >> $seqPath

	{
		for((j = 2; j <= 20; j += 2))
		do
			./bin/gcc/lab1-par-$j $i
		done
	} >> $parPath
	echo -e '\n' >> $parPath

	echo "$i/$N2"
done

echo 'cc run'

seqPath="./outputs/cc-seq-output.txt"
parPath="./outputs/cc-par-output.txt"

rm -f $seqPath
rm -f $parPath

for ((i=$N1; i<=$N2;i+=$delta))
do
	./bin/cc/lab1-seq $i >> $seqPath

	{
		for((j = 2; j <= 20; j += 2))
		do
			./bin/cc/lab1-par-$j $i
		done
	} >> $parPath
	echo -e '\n' >> $parPath

	echo "$i/$N2"
done

echo 'icc run'

seqPath="./outputs/icc-seq-output.txt"
parPath="./outputs/icc-par-output.txt"

rm -f $seqPath
rm -f $parPath

for ((i=$N1; i<=$N2;i+=$delta))
do
	./bin/icc/lab1-seq $i >> $seqPath

	{
		for((j = 2; j <= 20; j += 2))
		do
			./bin/icc/lab1-par-$j $i
		done
	} >> $parPath
	echo -e '\n' >> $parPath

	echo "$i/$N2"
done

echo "Done"
