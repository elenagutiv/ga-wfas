#!/bin/bash

echo "Experiment comparison..."
cd ../bin/

N_SAMPLES=10 # Set up this value to 10 to reproduce experiments

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

		4)	if (( $n_states < 13 ))
			then
				/bf-search "$file" "bf-search" "14" "7" "1" "../res/bf-search/random/$res_file"
				./main "$file" "bf-search" "14" "7" "1" "../res/ga/random/exp-bf-search/$res_file"
			fi
		;;
		6)	if (( $n_states < 13 ))
			then
				./bf-search "$file" "bf-search" "11" "6" "1" "../res/bf-search/random/$res_file"
				./main "$file" "bf-search" "11" "6" "1" "../res/ga/random/exp-bf-search/$res_file"
			fi
		;;
		10)	if (( $n_states < 13 ))
			then
				./bf-search "$file" "bf-search" "9" "5" "1" "../res/bf-search/random/$res_file"
				./main "$file" "bf-search" "9" "5" "1" "../res/ga/random/exp-bf-search/$res_file"
			fi
		esac
	done
done