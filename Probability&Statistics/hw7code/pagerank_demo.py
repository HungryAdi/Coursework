'''
UCI CS177: Markov chains and Pagerank
    This is DEMONSTRATION code:
    - It gives an example of how to construct a state transition matrix
      for the web link data, and use it to predict "random surfer" behavior.
    - You may (but do not have to) reuse parts of this code in your solutions.
    - It is NOT a template for the individual questions you must answer,
      for that see the main homework pdf.
'''

from __future__ import division
import numpy as np
from scipy.io import loadmat

def load_data():
    # Data collected in 2002 by Prof. Jon Kleinberg, Cornell University
    # Cleaned version courtesy of ECEN5322, University of Colorado
    #  L[i,j]   = 1 if there is a directed link from website i to website j
    #  L[i,j]   = 0 if there is no directed link from website i to website j
    #  names[i] = URL of website i
    L = loadmat('large_network')['L']
    tmp = loadmat('large_network')['names']
    names = [None for _ in xrange(len(tmp))]
    for i in xrange(len(names)):
        names[i] = tmp[i][0][0].encode('ascii', 'ignore')
    return L, names

L, names = load_data()
m = np.shape(L)[0]  # number of websites (nodes)

# Define local random-walk state transition matrix T
T = np.array(L, np.float)
for i in xrange(m):
    s = np.sum(T[i])
    if s > 0:
        T[i] /= s
    else:
        T[i, i] = 1

# Find state distribution after one step of random walk from uniform init.
p0 = np.ones(m) / m
p1 = np.dot(p0, T)

# Display highest ranked webpages after only one step of random walk
rank_inds = np.argsort(p1)[::-1]
rank_value = p1[rank_inds]
print 'pagerank\tin\tout\turl:'
for i in xrange(25):
    cur_ind = rank_inds[i]
    links_in  = np.sum(L[:, cur_ind])
    links_out = np.sum(L[cur_ind, :])
    print '%.5f\t\t%d\t%d\t%s' % (rank_value[i], links_in, links_out, names[rank_inds[i]])

