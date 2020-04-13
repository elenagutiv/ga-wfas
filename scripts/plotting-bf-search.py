import glob
import os
import matplotlib.pyplot as plt
import matplotlib.colors
import numpy as np
import statistics

matplotlib.rcParams['pdf.fonttype'] = 42
matplotlib.rcParams['ps.fonttype'] = 42
plt.rc('legend',fontsize='small') # using a named size

N_SAMPLES = 10

def geq_float(a,b):
	return int(a * 10**13) >= int(b * 10**13)

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


ga_path = "../res/ga/newrandp/exp-bf-search/"
bf_path = "../res/bf-search/newrandp/"

full_path_sample_list = glob.glob(os.path.join(os.getcwd(), ga_path, "*.txt"))
sample_list = [elem.split("/")[-1] for elem in full_path_sample_list]

file_path = "../tests/newrandp/Random-test/"

full_path_file_list = glob.glob(os.path.join(os.getcwd(), file_path, "*.txt"))
file_list = [elem.split("/")[-1] for elem in full_path_file_list]

#Printing table

print "Printing table"
print "name-file	Max. found by GA 	Maximum	n-best"

for file in file_list:

	avg = 0.0

	filename_bf = file[:-4] + "-" + str(1) + ".txt"
	bf_samplename = bf_path + filename_bf[:-4] + ".txt"
	bf_weights = np.loadtxt(bf_samplename, dtype = float, delimiter='\t',usecols=(0,), unpack=True)
	max_bf = bf_weights[-1]

	for n in xrange(N_SAMPLES):

		filename = file[:-4] + "-" + str(n+1) + ".txt"

		ga_samplename = ga_path + filename[:-4] + ".txt"

		ga_times, ga_weights = np.loadtxt(ga_samplename, dtype = (float, float), delimiter='\t', usecols=(0,1), unpack=True)

		maximum = ga_weights[-1]
		avg = avg + (maximum/N_SAMPLES);

	letter, alph, states = name(file[:-4])
	print letter+"("+alph+","+states+")", avg, max_bf, map(lambda x: geq_float(avg,x), bf_weights[::-1]).index(1)+1

# Printing plots
print "Printing plots"

matplotlib.rcParams.update({'font.size': 12})


for sample in sample_list:

	ga_samplename = ga_path + sample[:-4] + ".txt"

	bf_prefix = sample.split("-")[:-1] 
	bf_prefix_def =  "-".join(bf_prefix) + "-1"
	bf_samplename = bf_path + bf_prefix_def + ".txt"

	print bf_samplename

	ga_weights, ga_times = np.loadtxt(ga_samplename, dtype = (float, float), delimiter='\t', usecols=(0,1), unpack=True)
	bf_weights= np.loadtxt(bf_samplename, dtype = float, delimiter='\t',usecols=(0,), unpack=True)

	color_rand = (0.8, 0.89, 1)
	color_ga = (1, 0.2, 0.8)

	plt.plot(ga_weights, ga_times, 'b', label='GA')
	plt.axhspan(bf_weights[-5], bf_weights[-1], alpha=0.5, color='red', label='5-best words')

	# plt.axvline(max(rand_nodups), color=color_rand, linestyle='solid', label='Max Random')
	# plt.axvline(max(ga_nodups), color=color_ga, linestyle='solid', label='Max Genetic')

	letter, alph, states = name(sample)

	plt.xlabel('Time')
	plt.ylabel('Max. found')
	plt.title("Input: "+letter+"("+alph+","+states+")")


	plt.grid(True)
	plt.legend(loc='lower right', shadow=True, ncol=1)
	print "Creating "+ "plots/newrandp/exp-comparison/"+letter+"-"+alph+"-"+states+"-"+sample[-5]+".pdf"
	plt.savefig("../plots/newrandp/exp-comparison/"+letter+"-"+alph+"-"+states+"-"+sample[-5]+".pdf")
	plt.close()