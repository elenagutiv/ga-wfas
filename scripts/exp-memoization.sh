#!/bin/bash

echo "Experiment memoization..."
cd ../bin/

N_SAMPLES=1 # Set up this value to 10 to reproduce experiments


for file in ../tests/newrandp/Random/*.txt
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
				./main "$file" "aut" "20" "6" "0" "1" "../res/ga/newrandp/exp-memoization/word-memo/$res_file" "no-file" "rand" "../res/ga/newrandp/exp-memoization/memo/$res_file"
			else
				./main "$file" "aut" "20" "7" "0" "1" "../res/ga/newrandp/exp-memoization/word-memo/$res_file" "no-file" "rand" "../res/ga/newrandp/exp-memoization/memo/$res_file"
			fi
		./main "$file" "aut" "20" "1" "0" "1" "../res/ga/newrandp/exp-memoization/word-no-memo/$res_file" "no-file" "rand" "../res/ga/newrandp/exp-memoization/no-memo/$res_file"
		;;
		6)	if (( $n_states > 12 ))
			then
				./main "$file" "aut" "20" "5" "0" "1" "../res/ga/newrandp/exp-memoization/word-memo/$res_file" "no-file" "rand" "../res/ga/newrandp/exp-memoization/memo/$res_file"
			else
				./main "$file" "aut" "20" "6" "0" "1" "../res/ga/newrandp/exp-memoization/word-memo/$res_file" "no-file" "rand" "../res/ga/newrandp/exp-memoization/memo/$res_file"
			fi
		./main "$file" "aut" "20" "1" "0" "1" "../res/ga/newrandp/exp-memoization/word-no-memo/$res_file" "no-file" "rand" "../res/ga/newrandp/exp-memoization/no-memo/$res_file"
		;;
		10)	if (( $n_states > 12 ))
			then
				./main "$file" "aut" "20" "4" "0" "1" "../res/ga/newrandp/exp-memoization/word-memo/$res_file" "no-file" "rand" "../res/ga/newrandp/exp-memoization/memo/$res_file"
			else
				./main "$file" "aut" "20" "5" "0" "1" "../res/ga/newrandp/exp-memoization/word-memo/$res_file" "no-file" "rand" "../res/ga/newrandp/exp-memoization/memo/$res_file"
			fi
		./main "$file" "aut" "20" "1" "0" "1" "../res/ga/newrandp/exp-memoization/word-no-memo/$res_file" "no-file" "rand" "../res/ga/newrandp/exp-memoization/no-memo/$res_file"
		esac
	done
done