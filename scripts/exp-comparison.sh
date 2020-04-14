#!/bin/bash

echo "Experiment comparison..."
cd ../bin/

N_SAMPLES=1 # Set up this value to 10 to reproduce experiments

for file in ../tests/random/Random/*.txt
do
	
	for n in $(seq 1 $N_SAMPLES)
	do
		res_file=$(echo $file| cut -d "/" -f 5)
		res_file=$(echo $res_file| cut -d "." -f 1)
		res_file=$res_file-$n.txt
		echo "Test: " $file
		echo "Result: " $res_file
		alphabet=$(echo $res_file| cut -d "-" -f 2)
		n_symbols=${#alphabet}

		n_states=$(echo $res_file| cut -d "-" -f 3)

		case "$n_symbols" in

		4)	if (( $n_states > 12 ))
			then
				./main "$file" "comparison" "20" "6" "1" "../res/ga/random/exp-comparison/$res_file"
				./random-search "$file" "comparison" "20" "6" "1" "../res/random-search/random/exp-comparison/$res_file"
			else
				./main "$file" "comparison" "20" "7" "1" "../res/ga/random/exp-comparison/$res_file"
				./random-search "$file" "comparison" "20" "7" "1" "../res/random-search/random/exp-comparison/$res_file"
			fi
		;;
		6)	if (( $n_states > 12 ))
			then
				./main "$file" "comparison" "20" "5" "1" "../res/ga/random/exp-comparison/$res_file"
				./random-search "$file" "comparison" "20" "5" "1" "../res/random-search/random/exp-comparison/$res_file"
			else
				./main "$file" "comparison" "20" "6" "1" "../res/ga/random/exp-comparison/$res_file"
				./random-search "$file" "comparison" "20" "6" "1" "../res/random-search/random/exp-comparison/$res_file"
			fi
		;;
		10)	if (( $n_states > 12 ))
			then
				./main "$file" "comparison" "20" "4" "1" "../res/ga/random/exp-comparison/$res_file"
				./random-search "$file" "comparison" "20" "4" "1" "../res/random-search/random/exp-comparison/$res_file"
			else
				./main "$file" "comparison" "20" "5" "1" "../res/ga/random/exp-comparison/$res_file"
				./random-search "$file" "comparison" "20" "5" "1" "../res/random-search/random/exp-comparison/$res_file"
			fi
		esac
	done
done