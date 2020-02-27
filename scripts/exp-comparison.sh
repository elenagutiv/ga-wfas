#!/bin/bash

echo "Experiment comparison..."
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

		4)	if (( $n_states > 12 ))
			then
				./main "$file" "aut" "20" "6" "0" "1" "../res/ga/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
				./random-search "$file" "aut" "20" "6" "0" "1" "../res/random-search/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
			else
				./main "$file" "aut" "20" "7" "0" "1" "../res/ga/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
				./random-search "$file" "aut" "20" "7" "0" "1" "../res/random-search/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
			fi
		;;
		6)	if (( $n_states > 12 ))
			then
				./main "$file" "aut" "20" "5" "0" "1" "../res/ga/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
				./random-search "$file" "aut" "20" "5" "0" "1" "../res/random-search/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
			else
				./main "$file" "aut" "20" "6" "0" "1" "../res/ga/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
				./random-search "$file" "aut" "20" "6" "0" "1" "../res/random-search/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
			fi
		;;
		10)	if (( $n_states > 12 ))
			then
				./main "$file" "aut" "20" "4" "0" "1" "../res/ga/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
				./random-search "$file" "aut" "20" "4" "0" "1" "../res/random-search/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
			else
				./main "$file" "aut" "20" "5" "0" "1" "../res/ga/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
				./random-search "$file" "aut" "20" "5" "0" "1" "../res/random-search/newrandp/exp-comparison/$res_file" "no-file" "rand" "none"
			fi
		esac
	done
done