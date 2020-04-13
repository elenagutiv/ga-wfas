###########################################################
Experiment Comparison with random search (Section 5.1)
###########################################################

# Write the following commands in the command line from the folder containing 'genetic-alg-wfas'
# Note: In order to reproduce the same experiments, and in particular, to repeat each executions 10 times, change variable N_SAMPLES in ./exp-comparison.sh 
# **Note**: Beware of space on your machine. The size of each file generated is 15-70MB.  


cd genetic-alg-wfas/
make clean
make
make random-search
cd scripts
./exp-comparison.sh

# When the execution of the later script finishes, results will be stored in genetic-alg-wfas/res/ga/newrandp/exp-comparison/ and genetic-alg-wfas/res/random-search/newrandp/exp-comparison/ 


# Note that, in order to reproduce the same experiments, you may need to change the value of variable N_SAMPLES in plotting-memoization.py to 10
# To process the results of the experiment, run:

python plotting-comparison.py


############################################
Experiment Memoization (Section 5.2)
############################################

# Write the following commands in the command line from the folder containing 'genetic-alg-wfas'
# Note: In order to reproduce the same experiments, and in particular, to repeat each executions 10 times, change variable N_SAMPLES in ./exp-memoization.sh 
# **Note**: Beware of space on your machine. The size of each file generated is 10-100KB.  

cd genetic-alg-wfas/
make clean
make
cd scripts
./exp-memoization.sh

# When the execution of the later script finishes, results will be stored in genetic-alg-wfas/res/ga/newrandp/exp-memoization/
# There are 4 folders there: 
	- memo/ 
	- no-memo/
	- word-memo/
	- word-no-memo/

# Note that, in order to reproduce the same experiments, you need to change the value of variable N_SAMPLES in plotting-memoization.py to 10
# To process the results of the experiment, run:

python plotting-memoization.py


############################################
Experiment Case of Study (Section 6)
############################################
# Write the following commands in the command line from the folder containing 'genetic-alg-wfas'
# Note: In order to reproduce the same experiments, go to inc/params.h and set macro TIMEOUT to 240 s

cd genetic-alg-wfas/
make clean
make
cd scripts
./exp-case-study.sh

# When the execution of the later script finishes, results will be stored in /genetic-alg-wfas/res/ga/paren/exp-paren/ in two files, one corresponding to (A_R - A_E) and the other one corresponding to (A_E - A_R)

# To process the data of the experiment to create a plot, run:

python plotting-case-study.py

# The resulting plot (.pdf) is in plots/paren/exp-paren/
