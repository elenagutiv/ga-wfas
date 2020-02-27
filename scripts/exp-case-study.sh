#!/bin/bash

echo "Experiment Case of Study..."
cd ../bin/

specification_file="../tests/paren/spec.txt" 
diff1="diff1"
diff2="diff2"

file="../tests/paren/paren.txt"

res_file=$(echo $file| cut -d "/" -f 4)
res_file=$(echo $res_file| cut -d "." -f 1)

./main "$file" "difference" "10" "4" "0" "1" "../res/ga/paren/exp-paren/$res_file-$diff1.txt" "$specification_file" "rand" "none"
./main "$specification_file" "difference" "10" "4" "0" "1" "../res/ga/paren/exp-paren/$res_file-$diff2.txt" "$file" "rand" "none"