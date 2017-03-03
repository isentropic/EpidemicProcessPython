
import glob
import os
import numpy as np
import random
import math
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm

fullpath = os.path.dirname(os.path.realpath(__file__))
nodes_file = open('degree_distr.txt', 'r')

lines = nodes_file.readlines()

nodes = np.array([0] * len(lines))
mu = np.array([1.0] * len(lines))
total_degree = np.array([1] * len(lines))
global_degree = np.array([1] * len(lines))
local_degree = np.array([1] * len(lines))

for i in range(len(lines)):
    thestring = lines[i].split()
    nodes[i] = int(thestring[0])
    mu[i] = float(thestring[1])
    total_degree[i] = int(thestring[2])
    global_degree[i] = int(thestring[3])
    local_degree[i] = int(thestring[4])
number_of_nodes = len(local_degree)
# hist1 = np.histogram(total_degree)

# plt.subplot(121)
plt.hist(total_degree, bins=20)

# plt.hist(local_degree, bins=20)

plt.title("Degree distribution, k_{av} = %f" % (
    sum(total_degree) / float(len(total_degree))))
plt.xlabel("$k$")
plt.ylabel("Frequency")

plt.savefig('%s/graphs/k_distr.png' % (fullpath), format='png', dpi=1000)

# plt.yscale('log')


#   ************************    nodes mu
plt.figure(5)
plt.title('Distribution of $p$')
plt.xlabel('$p$')
plt.ylabel("Frequency")
plt.hist(mu, normed=1)

plt.savefig('%s/graphs/mu_distr.png' % (fullpath), format='png', dpi=1000)

#  ************************      community mu


community_mu_file = open('community_distr.txt', 'r')
lines = community_mu_file.readlines()
mu_percommunity = np.array([0.0] * len(lines))
community_number = np.array([0.0] * len(lines))
for i in range(len(lines)):
    mu_percommunity[i] = float(lines[i].split()[0])
    community_number[i] = float(lines[i].split()[1])


plt.figure(2)
plt.bar(community_number, mu_percommunity, width=1, color='green')
plt.xlabel("Community Number")
plt.ylabel("$p$")
plt.title("$p = {k_{inter}}/{k_{total}}$ Distribution per community")
# plt.text(1,.28, r'Total number of Nodes $N = %i$'%(number_of_nodes))
plt.ylim(0, .3)
# plt.grid(True)

plt.savefig('%s/graphs/mu_per_community.png' %
            (fullpath), format='png', dpi=1000)

# plt.show()

#  ************************       community size distribtion

community_size_file = open('community_sizes.txt', 'r')
lines = community_size_file.readlines()
size_of_a_community = np.array([0] * len(lines))
community_number = np.array([0] * len(lines))
for i in range(len(lines)):
    size_of_a_community[i] = int(lines[i].split()[1])
    community_number[i] = int(lines[i].split()[0])


plt.figure(3)

plt.bar(community_number, size_of_a_community, color='blue')

plt.xlabel("Community Number")
plt.ylabel("Size")
plt.title("Community Sizes")
# plt.text(1,.28, r'Total number of Nodes $N = %i$'%(number_of_nodes))
# plt.ylim(0,.3)
plt.grid(True)
# plt.yscale('log')
# plt.xscale('log')

plt.savefig('%s/graphs/community_sizes.png' %
            (fullpath), format='png', dpi=1000)

plt.figure(4)


# plt.hist(size_of_a_community,bins=np.arange(min(size_of_a_community), max(size_of_a_community) + 2, 1))

hist, bin_edges = np.histogram(size_of_a_community, bins=np.arange(
    min(size_of_a_community), max(size_of_a_community) + 2, 1))

plt.scatter(bin_edges[:-1], hist, marker='+')

t = np.linspace(min(size_of_a_community) - 3,
                max(size_of_a_community) + 3, 1000)
N = len(nodes)
x_min = float(min(size_of_a_community))
x_max = float(max(size_of_a_community))

Beta = 3.0
plt.plot(t, N * (1 - Beta) / (x_max**(1 - Beta) - x_min**(1 - Beta)) /
         90.0 * 1.357 * np.power(t, -Beta), color='red')

plt.xlabel("Size")
plt.ylabel("Frequency")
plt.title("Community Sizes Distribution")
# plt.grid(True)


plt.yscale('log', nonposy='clip')
plt.xscale('log')
# plt.xlim(min(size_of_a_community)-5, max(size_of_a_community)+5)
if max(size_of_a_community) < 100:
    plt.xlim(min(size_of_a_community) - 5, 100)
else:
    plt.xlim(min(size_of_a_community) - 5, max(size_of_a_community) + 5)
# plt.xticks(np.arange(min(size_of_a_community)-5, max(size_of_a_community)+5+1, 10.0))
plt.savefig('%s/graphs/community_sizes_log.png' %
            (fullpath), format='png', dpi=1000)


# plt.show()
