import glob
import os
import matplotlib.pyplot as plt
import matplotlib.colors
import numpy as np

matplotlib.rcParams['pdf.fonttype'] = 42
matplotlib.rcParams['ps.fonttype'] = 42
plt.rc('legend',fontsize='small') # using a named size

path_res = "../res/ga/paren/exp-paren/"

path = "../tests/paren/"
full_path_file_list = glob.glob(os.path.join(os.getcwd(), path, "*.txt"))
file_list = [elem.split("/")[-1] for elem in full_path_file_list]

text_file="paren.txt"
filename_diff1 = path_res + text_file[:-4] + "-" + "diff1" + ".txt"
filename_diff2 = path_res + text_file[:-4] + "-" + "diff2" + ".txt"


diff1_words, diff1_weights = np.loadtxt(filename_diff1, dtype = (str, float), delimiter='\t', unpack=True)
diff2_words, diff2_weights = np.loadtxt(filename_diff2, dtype = (str, float), delimiter='\t', unpack=True)
diff1_nodups = list(set([float(i) for i in diff1_weights]))
diff2_nodups = list(set([float(i) for i in diff2_weights]))


color_large = (0.71, 0.92, 0.67)
color_small = (0.49, 0.02, 0.32)
epsilon = 'r'
plt.hist([diff1_nodups, diff2_nodups], 50, label=[r'$\mathcal{A}_{R}\ominus \mathcal{A}_E$', r'$\mathcal{A}_{E}\ominus \mathcal{A}_R$'], color=[color_small, color_large])

plt.xlabel('Weights')
plt.ylabel('Frequencies')
plt.title('Observed-weights distribution of 'r'$\mathcal{A}_{R}\ominus \mathcal{A}_E$'' and 'r'$\mathcal{A}_{E}\ominus \mathcal{A}_R$')

plt.vlines(max(diff2_nodups), 0, 450, colors=color_large, linestyles='solid', label='Max. 'r'$\mathcal{A}_{E}\ominus \mathcal{A}_R$' )
plt.vlines(max(diff1_nodups), 0, 450, colors=color_small, linestyles='solid', label='Max. 'r'$\mathcal{A}_{R}\ominus \mathcal{A}_E$' )
#plt.vlines(0.013, 0, 450, colors=epsilon, linestyles='solid', label='error 'r'$\varepsilon$', linewidth=2 )

# Adjust as desired these two lines as desired:
plt.xlim(-0.01, 0.016)
plt.ylim(0, 450)

plt.grid(True)
plt.legend(loc='upper left', shadow=True, ncol=1)
print "Creating "+ "../plots/paren/exp-paren/"+text_file[:-4] +".pdf"
plt.savefig("../plots/paren/exp-paren/"+text_file[:-4] +".pdf")
plt.close()