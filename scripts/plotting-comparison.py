import glob
import os
import matplotlib.pyplot as plt
import matplotlib.colors
import numpy as np
import statistics

matplotlib.rcParams['pdf.fonttype'] = 42
matplotlib.rcParams['ps.fonttype'] = 42
plt.rc('legend',fontsize='small') # using a named size

N_SAMPLES = 1 # Change this value to 10 to reproduce experiments

avgs_ga = [0.0 for i in range(N_SAMPLES)]
avgs_rand = [0.0 for i in range(N_SAMPLES)]
stdevs_ga = [0.0 for i in range(N_SAMPLES)]
stdevs_rand = [0.0 for i in range(N_SAMPLES)]
maxs_ga = [0.0 for i in range(N_SAMPLES)]
maxs_rand = [0.0 for i in range(N_SAMPLES)]
mins_ga = [0.0 for i in range(N_SAMPLES)]
mins_rand = [0.0 for i in range(N_SAMPLES)]

lens_ga_words = [0 for i in range(N_SAMPLES)]
lens_rand_words = [0 for i in range(N_SAMPLES)]


avg_ga = 0.0
stdev_ga = 0.0
max_ga = 0.0
min_ga = 0.0

avg_rand = 0.0
stdev_rand = 0.0
max_rand = 0.0
min_rand = 0.0

len_ga_words = 0.0
len_rand_words = 0.0


def Average(lst):
    return sum(lst) / len(lst)

def initLists():

	global avgs_ga
	global avgs_rand
	global stdevs_ga
	global stdevs_rand
	global maxs_ga
	global maxs_rand
	global mins_ga
	global mins_rand
	global lens_ga_words
	global lens_rand_words

	global N_SAMPLES

	avgs_ga = [0.0 for i in range(N_SAMPLES)]
	avgs_rand = [0.0 for i in range(N_SAMPLES)]
	stdevs_ga = [0.0 for i in range(N_SAMPLES)]
	stdevs_rand = [0.0 for i in range(N_SAMPLES)]
	maxs_ga = [0.0 for i in range(N_SAMPLES)]
	maxs_rand = [0.0 for i in range(N_SAMPLES)]
	mins_ga = [0.0 for i in range(N_SAMPLES)]
	mins_rand = [0.0 for i in range(N_SAMPLES)]

	lens_ga_words = [0 for i in range(N_SAMPLES)]
	lens_rand_words = [0 for i in range(N_SAMPLES)]



def initValues():

	global avg_ga
	global stdev_ga
	global max_ga
	global min_ga

	global avg_rand
	global stdev_rand
	global max_rand
	global min_rand

	global len_ga_words
	global len_rand_words

	avg_ga = 0.0
	stdev_ga = 0.0
	max_ga = 0.0
	min_ga = 0.0

	avg_rand = 0.0
	stdev_rand = 0.0
	max_rand = 0.0
	min_rand = 0.0

	len_ga_words = 0.0
	len_rand_words = 0.0

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

ga_path = "../res/ga/newrandp/exp-comparison/"
rand_path = "../res/random-search/newrandp/exp-comparison/"

full_path_sample_list = glob.glob(os.path.join(os.getcwd(), ga_path, "*.txt"))
sample_list = [elem.split("/")[-1] for elem in full_path_sample_list]

file_path = "../tests/newrandp/Random/"

full_path_file_list = glob.glob(os.path.join(os.getcwd(), file_path, "*.txt"))
file_list = [elem.split("/")[-1] for elem in full_path_file_list]


# Printing table
print "Printing table"

print "Random Search	Genetic Algorithm"
print "name-file	weights-obs.	w/s	weights-obs.	w/s"

for file in file_list:
	for n in xrange(N_SAMPLES):

		filename = file[:-4] + "-" + str(n+1) + ".txt"

		ga_samplename = ga_path + filename[:-4] + ".txt"
		rand_samplename = rand_path + filename[:-4] + ".txt"

		ga_words, ga_weights = np.loadtxt(ga_samplename, dtype = (str, float), delimiter='\t', unpack=True)
		rand_words, rand_weights = np.loadtxt(rand_samplename, dtype = (str, float), delimiter='\t', unpack=True)

		lens_ga_words[n] = len(ga_words)
		lens_rand_words[n] = len(rand_words)

		ga_nodups = list(set([float(i) for i in ga_weights]))
		rand_nodups = list(set([float(i) for i in rand_weights]))

		avgs_ga[n] = Average(ga_nodups)
		avgs_rand[n] = Average(rand_nodups)

		stdevs_ga[n] = statistics.stdev(ga_nodups)
		stdevs_rand[n] = statistics.stdev(rand_nodups)

		maxs_ga[n] = max(ga_nodups)
		maxs_rand[n] = max(rand_nodups)

		mins_ga[n] = min(ga_nodups)
		mins_rand[n] = min(rand_nodups)

	for n in xrange(N_SAMPLES):
		avg_ga = avg_ga + (avgs_ga[n]/N_SAMPLES)
		avg_rand = avg_rand + (avgs_rand[n]/N_SAMPLES)

		stdev_ga = stdev_ga + (stdevs_ga[n]/N_SAMPLES)
		stdev_rand = stdev_rand + (stdevs_rand[n]/N_SAMPLES)

		max_ga = max_ga + (maxs_ga[n]/N_SAMPLES)
		max_rand = max_rand + (maxs_rand[n]/N_SAMPLES)

		min_ga = min_ga + (mins_ga[n]/N_SAMPLES)
		min_rand = min_rand + (mins_rand[n]/N_SAMPLES)

		len_ga_words = len_ga_words + (lens_ga_words[n]/N_SAMPLES)
		len_rand_words = len_rand_words + (lens_rand_words[n]/N_SAMPLES)

	letter, alph, states = name(file[:-4])

	print letter+"("+alph+","+states+")", str(round(avg_rand, 2)) , str(round(stdev_rand*1.96, 2)), str(round(max_rand, 2)),  str(round((len_rand_words/120000),2)), str(round(avg_ga, 2)), str(round(stdev_ga*1.96, 2)), str(round(max_ga, 2)), str(round((len_ga_words/120000),2))

	initLists()
	initValues()

# Printing plots
print "Printing plots"

matplotlib.rcParams.update({'font.size': 12})


for sample in sample_list:

	ga_samplename = ga_path + sample[:-4] + ".txt"
	rand_samplename = rand_path + sample[:-4] + ".txt"

	ga_words, ga_weights = np.loadtxt(ga_samplename, dtype = (str, float), delimiter='\t', unpack=True)
	rand_words, rand_weights = np.loadtxt(rand_samplename, dtype = (str, float), delimiter='\t', unpack=True)
	ga_nodups = list(set([float(i) for i in ga_weights]))
	rand_nodups = list(set([float(i) for i in rand_weights]))

	color_rand = (0.8, 0.89, 1)
	color_ga = (1, 0.2, 0.8)

	y, x, _ = plt.hist([rand_nodups, ga_nodups], 50, label=['Random', 'Genetic'], color=[color_rand, color_ga])

	plt.axvline(max(rand_nodups), color=color_rand, linestyle='solid', label='Max Random')
	plt.axvline(max(ga_nodups), color=color_ga, linestyle='solid', label='Max Genetic')

	letter, alph, states = name(sample)

	plt.xlabel('Weights')
	plt.ylabel('Frequencies')
	plt.title("Input: "+letter+"("+alph+","+states+")")


	plt.grid(True)
	plt.legend(loc='upper left', shadow=True, ncol=1)
	print "Creating "+ "plots/newrandp/exp-comparison/"+letter+"-"+alph+"-"+states+"-"+sample[-5]+".pdf"
	plt.savefig("../plots/newrandp/exp-comparison/"+letter+"-"+alph+"-"+states+"-"+sample[-5]+".pdf")
	plt.close()