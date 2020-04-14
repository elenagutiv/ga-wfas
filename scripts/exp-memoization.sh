#!/bin/bash

echo "Experiment memoization..."
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

		4) 	if (( $n_states > 12 ))
			then
				./main "$file" "memo" "20" "6" "1" "../res/ga/random/exp-memoization/memo/$res_file"
			else
				./main "$file" "memo" "20" "7" "1" "../res/ga/random/exp-memoization/memo/$res_file"
			fi
		./main "$file" "memo" "20" "1" "1" "../res/ga/random/exp-memoization/no-memo/$res_file"
		;;
		6)	if (( $n_states > 12 ))
			then
				./main "$file" "memo" "20" "5" "1" "../res/ga/random/exp-memoization/memo/$res_file"
			else
				./main "$file" "memo" "20" "6" "1" "../res/ga/random/exp-memoization/memo/$res_file"
			fi
		./main "$file" "memo" "20" "1" "1" "../res/ga/random/exp-memoization/no-memo/$res_file"
		;;
		10)	if (( $n_states > 12 ))
			then
				./main "$file" "memo" "20" "4" "1" "../res/ga/random/exp-memoization/memo/$res_file"
			else
				./main "$file" "memo" "20" "5" "1" "../res/ga/random/exp-memoization/memo/$res_file"
			fi
		./main "$file" "memo" "20" "1" "1" "../res/ga/random/exp-memoization/no-memo/$res_file"
		esac
	done
done