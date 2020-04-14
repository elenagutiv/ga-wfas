# Genetic Algorithm for the Weight Maximization Problem on Weighted Automata
We propose a **genetic-algorithm-based metaheuristic** to approximate the so-called *Weighted Maximization Problem (WMP)*, i.e., the problem of computing **the word with the highest weight** on a weighted automata with weights over the rationals.
Since the WMP is an undecidable problem [3], we look at the problem that results from bounding the length of the words in the search space by a fixed value k ≥ 1.
We call the later problem the *Bounded Weight Maximization Problem (WBMP)*  and this is the question our algorithm approximates.

Our algorithm is implemented in the C language. 
The procedure takes as input the matrix representation of a WFA A and a value k ≥ 1. 
For the code representation of the weights we use rationals of arbitrary precision by means of the library for arbitrary-precision arithmetic on rational numbers, GMP [1].
For the lookup table implementation we use uthash.h [2], a header file written in C that implements a hash table for handling C structures.


[1] *The GNU Multiple Precision Arithmetic Library. 1991. GNU Project. (1991). Retrieved January 28, 2020 from gmplib.org.*

[2] *Troy D. Hanson and Arthur O’Dwyer. 2006. Uthash. (2006). Retrieved January 28, 2020 from https://troydhanson.github.io/uthash/*

[3] *Azaria Paz. 1971. Introduction to Probabilistic Automata (Computer Science and Applied Mathematics). Academic Press, Inc., Orlando, FL, USA.*

# Folders
- bin: execution files
- inc: header files
- obj: object files
- plots: folder that stores plots generated by experiments
- res: folder that stores the .txt files generated by experiments
- scripts: script files to perform experiments
- src: source files
- test: benchmarks for experiments

# General
Broadly speaking, the algorithm takes as input a WFA matrix description and a positive value k that represents the bound in the length of words that are searched.
Further parameters relative to the genetic algorithm and experiment specification can be introduced in the right format by command line or using the header file inc/params.h

# Experiments
The following experiments can be carried out using our tool and the set of benchmarks *Random*, in the folder tests/newrandp/Random.
For further details on these experiments you can read our work on this problem, which will be soon available online and referenced in this repository.
Prior to performing the experiments, execute the following commands in the command line inside the folder 'ga-wfas':

    make env


## Genetic Algorithm vs Random Search experiment
This experiment compares our genetic-algorithm-based metaheuristic against random search for approximating a solution to the WMP.
We perform the comparison by plotting an histogram for each procedure and test case.
The histogram represents the distribution of weights observed by the two procedures.

Write the following commands in the command line inside the folder 'ga-wfas'.

    make clean
    make
    make random-search
    cd scripts
    ./exp-comparison.sh
    
 **Note:** 
- Fix variable `N_SAMPLES` in ./exp-comparison.sh as desired to repeat each execution `N_SAMPLES` times.
- Beware of space on your machine. The size of each file generated is 15-70MB.  

When the execution of `exp-comparison.sh` finishes, results can be found in ga-wfas/res/ga/newrandp/exp-comparison/ and ga-wfas/res/random-search/newrandp/exp-comparison/ for each algorithm ga and random search, respectively.

To process the results of the experiment, run from the folder scripts:

    python plotting-comparison.py
   
**Note:**
Please, change the value of variable `N_SAMPLES` in plotting-comparison.py to the value fixed in ./exp-comparison.sh

## Memoization Experiment
This experiment compares the use of a simple memoization technique for matrix multiplication operations to improve the algorithm efficiency against a no-memoization version of the algorithm.
We perform the comparison in terms of the number of words processed by the two versions.

Write the following commands in the command line inside the folder  'ga-wfas'.

    make clean
    make
    cd scripts
    ./exp-memoization.sh

**Note:**
- Fix variable `N_SAMPLES` in ./exp-memoization.sh as desired to repeat each execution `N_SAMPLES` times.
- Beware of space on your machine. The size of each file generated is 10-100KB.  

When the execution of exp-memoization.sh finishes, results can be found in ga-wfas/res/ga/newrandp/exp-memoization/
There are 2 folders there: 
- memo/
- no-memo/
Memo folder stores for each test file the number of words processed by the algorithm when using memoization, whereas the no-memo folder stores the number of words processed by the algorithm when not using such memoization.

To process the results of the experiment, run:

    python plotting-memoization.py
**Note:**
Please, change the value of variable `N_SAMPLES` in plotting-memoization.py to the value fixed in ./exp-memoization.sh

## Examples with known maximum weight
This experiments validates the performance of our algorithm by comparing its solution with the highest weight in the automaton.

To do so, we simply select a subset of 9 benchmarks of Random, located in tests/newrandp/Random-bf-search, and we perform *exhaustive search* to solve the BWMP precisely.
For this, we choose an appropiate length bound k, that makes this search feasible in a reasonable amount of time.
This value will vary depending on the size of the alphabet of the input WFA.
In addition, we run our algorithm on the same set of benchmarks and the same length bound k.

To repeat this experiment, run the following commands in the folder 'ga-wfas'

    make clean
    make
    make bf-search
    cd scripts
    ./exp-bf-search.sh

 **Note:** 
- Fix variable `N_SAMPLES` in ./exp-comparison.sh as desired to repeat each execution `N_SAMPLES` times.
-Beware of the space on your machine.

When the execution of exp-bf-search.sh finishes, results can be found in ga-wfas/res/ga/newrandp/exp-bf-search/

To process the results of the experiment, run:

    python plotting-bf-search.py
**Note:**
Please, change the value of variable `N_SAMPLES` in plotting-bf-search.py to the value fixed in ./exp-bf-search.sh

# Case of study
This experiment shows a case of study of the application of our tool for detecting misclassified input sequences in a Recurrent Neural Network (RNN).
We use our algorithm in combination with the procedure that extracts a WFA from a given RNN [4] to estimate the error between the extracted WFA and the WFA that describes the specification of the RNN over a bounded-length set of words.
This yields to an estimation of the error together with an evidence of why the network is not properly approximating its specification.
Given a WFA A_R (described in tests/paren/paren.txt) corresponding to the extracted WFA from a given RNN, and the specification of the RNN, the WFA A_E (described in tests/paren/spec.txt), we compute the maximum between the weight of the highest-weighted word of (A_R - A_E) and (A_E - A_R) over a bounded-length set of words.

Write the following commands in the command line from the folder containing 'ga-wfas'

    cd ga-wfas/
    make clean
    make
    cd scripts
    ./exp-case-study.sh

When the execution of the later script finishes, results will be stored in /ga-wfas/res/ga/paren/exp-paren/ in two files, one corresponding to (A_R - A_E) and the other one corresponding to (A_E - A_R)

To process the data of the experiment to create a plot, run:

    python plotting-case-study.py

The resulting plot (.pdf) is in plots/paren/exp-paren/

[4] *Takamasa Okudono, Masaki Waga, Taro Sekiyama, and Ichiro Hasuo. 2019. Weighted Automata Extraction from Recurrent Neural Networks via Regression on State Spaces. AAAI 2020, to appear. CoRR abs/1904.02931 (2019).*

# Updates
- Only *one* compatibility try is performed. This value is fixed and non-modifiable.
- TNU procedure removed
TODO:
- SNU property in params is ignores -> remove
- remove deprecated SNUCrossover
- Check SNU comp works
