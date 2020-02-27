import glob
import os
import matplotlib.pyplot as plt
import matplotlib.colors
import numpy as np
import statistics

N_SAMPLES = 1 # Change this value to 10 to reproduce experiments
n_ops_memo = [0.0 for i in range(N_SAMPLES)]
n_ops_nomemo = [0.0 for i in range(N_SAMPLES)]
n_potential_ops_memo = [0.0 for i in range(N_SAMPLES)]

times_mul_memo = [0.0 for i in range(N_SAMPLES)]
times_mul_nomemo = [0.0 for i in range(N_SAMPLES)]
times_init_memo = [0.0 for i in range(N_SAMPLES)]
times_init_nomemo = [0.0 for i in range(N_SAMPLES)]
lens_memo = [0.0 for i in range(N_SAMPLES)]
lens_nomemo = [0.0 for i in range(N_SAMPLES)]


n_op_memo = 0.0
n_potential_op_memo = 0.0
time_mul_memo = 0.0
time_init_memo = 0.0

n_op_nomemo = 0.0
time_mul_nomemo = 0.0
time_init_nomemo = 0.0

len_memo = 0.0
len_nomemo = 0.0

def initLists():

	global n_ops_memo
	global n_ops_nomemo
	global n_potential_ops_memo
	global times_mul_memo
	global times_mul_nomemo
	global times_init_memo
	global times_init_nomemo
	global lens_memo
	global lens_nomemo

	n_ops_memo = [0.0 for i in range(N_SAMPLES)]
	n_ops_nomemo = [0.0 for i in range(N_SAMPLES)]
	n_potential_ops_memo = [0.0 for i in range(N_SAMPLES)]
	times_mul_memo = [0.0 for i in range(N_SAMPLES)]
	times_mul_nomemo = [0.0 for i in range(N_SAMPLES)]
	times_init_memo = [0.0 for i in range(N_SAMPLES)]
	times_init_nomemo = [0.0 for i in range(N_SAMPLES)]
	lens_memo = [0.0 for i in range(N_SAMPLES)]
	lens_nomemo = [0.0 for i in range(N_SAMPLES)]

def initValues():

	global n_op_memo
	global n_potential_op_memo
	global time_mul_memo
	global time_init_memo

	global n_op_nomemo
	global time_mul_nomemo
	global time_init_nomemo
	global len_memo
	global len_nomemo

	n_op_memo = 0.0
	n_potential_op_memo = 0.0
	time_mul_memo = 0.0
	time_init_memo = 0.0

	n_op_nomemo = 0.0
	time_mul_nomemo = 0.0
	time_init_nomemo = 0.0

	len_memo = 0.0
	len_nomemo = 0.0

def name(sample):

	if(sample.split("-")[0] == "11"): letter = 'A'
	elif (sample.split("-")[0] == "12"): letter = 'B'
	elif (sample.split("-")[0] == "6"): letter = 'C'
	elif (sample.split("-")[0] == "3"): letter = 'D'
	elif (sample.split("-")[0] == "1"): letter = 'E'
	elif (sample.split("-")[0] == "5"): letter = 'F'
	elif (sample.split("-")[0] == "2"): letter = 'G'
	elif (sample.split("-")[0] == "28"): letter = 'H'
	elif (sample.split("-")[0] == "17"): letter = 'I'
	elif (sample.split("-")[0] == "18"): letter = 'J'
	elif (sample.split("-")[0] == "19"): letter = 'K'
	elif (sample.split("-")[0] == "20"): letter = 'L'

	if(sample.split("-")[1] == "abcd"): alph = '4'
	elif(sample.split("-")[1] == "abcdef"): alph = '6'
	elif(sample.split("-")[1] == "abcdefghij"): alph = '10'

	states = sample.split("-")[2]
	return letter, alph, states


memo_path = "../res/ga/newrandp/exp-memoization/memo/"
nomemo_path = "../res/ga/newrandp/exp-memoization/no-memo/"

memo_word_path = "../res/ga/newrandp/exp-memoization/word-memo/"
nomemo_word_path = "../res/ga/newrandp/exp-memoization/word-no-memo/"

full_path_sample_list = glob.glob(os.path.join(os.getcwd(), memo_path, "*.txt"))
sample_list = [elem.split("/")[-1] for elem in full_path_sample_list]

file_path = "../tests/newrandp/Random/"

full_path_file_list = glob.glob(os.path.join(os.getcwd(), file_path, "*.txt"))
file_list = [elem.split("/")[-1] for elem in full_path_file_list]


# Printing table
print "No-Memo	Memo"
print "name-file w/s w/s"

for file in file_list:
	for n in xrange(N_SAMPLES):

		filename = file[:-4] + "-" + str(n+1) + ".txt"

		memo_samplename = memo_path + filename[:-4] + ".txt"
		nomemo_samplename = nomemo_path + filename[:-4] + ".txt"

		memo_word_samplename = memo_word_path + filename[:-4] + ".txt"
		nomemo_word_samplename = nomemo_word_path + filename[:-4] + ".txt"

		memo_word, memo_word_weights = np.loadtxt(memo_word_samplename, dtype = (str, float), delimiter='\t', unpack=True)
		nomemo_word, nomemo_word_weights = np.loadtxt(nomemo_word_samplename, dtype = (str, float), delimiter='\t', unpack=True)

		lens_memo[n] = len(memo_word)
		lens_nomemo[n] = len(nomemo_word)


		memo_field, memo_weights = np.loadtxt(memo_samplename, dtype = (str, float), delimiter='\t', unpack=True)
		nomemo_field, nomemo_weights = np.loadtxt(nomemo_samplename, dtype = (str, float), delimiter='\t', unpack=True)
		
		n_ops_memo[n] = float(memo_weights[0])
		n_potential_ops_memo[n] = float(memo_weights[1])
		times_mul_memo[n] = float(memo_weights[2])
		times_init_memo[n] = float(memo_weights[3])

		n_ops_nomemo[n] = float(nomemo_weights[0])
		times_mul_nomemo[n] = float(nomemo_weights[2])
		times_init_nomemo[n] = float(nomemo_weights[3])

	for n in xrange(N_SAMPLES):
		n_op_memo = n_op_memo + (n_ops_memo[n]/N_SAMPLES)
		n_potential_op_memo = n_potential_op_memo + (n_potential_ops_memo[n]/N_SAMPLES)
		n_op_nomemo = n_op_nomemo + (n_ops_nomemo[n]/N_SAMPLES)

		time_mul_memo = time_mul_memo + (times_mul_memo[n]/N_SAMPLES)
		time_mul_nomemo = time_mul_nomemo + (times_mul_nomemo[n]/N_SAMPLES)

		time_init_memo = time_init_memo + (times_init_memo[n]/N_SAMPLES)
		time_init_nomemo = time_init_nomemo + (times_init_nomemo[n]/N_SAMPLES)

		len_memo = len_memo + (lens_memo[n]/N_SAMPLES)
		len_nomemo = len_nomemo + (lens_nomemo[n]/N_SAMPLES)

	letter, alph, states = name(file[:-4])

	print letter + "(" + alph + "," + states+ ")",  str(round(len_nomemo/120000,2)), str(round(len_memo/120000,2))
	
	initLists()
	initValues()

# Printing plots


