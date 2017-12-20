'''
 UCI CS 177: Old Faithful Geyser Data Statistics
 This is DEMONSTRATION code:
 - It gives an example of how to load the geyser data, plot the data,
   and compute simple statistics of this data.
 - You may (but do not have to) reuse parts of this code in your solutions. 
 - It is NOT a template for the individual questions you must answer,
   for that see the main homework pdf.


 Description of data:
   Waiting time between eruptions and the duration of the eruption
   for the Old Faithful geyser in Yellowstone National Park, Wyoming, USA.

 eruptions (numeric)  Eruption time in minutes
 waiting   (numeric)  Waiting time to next eruption in minutes
 
 References:
 - Hardle, W. (1991) Smoothing Techniques with Implementation in S.
   New York: Springer.
 - Azzalini, A. and Bowman, A. W. (1990). A look at some data on the
   Old Faithful geyser. Applied Statistics 39, 357-365.
'''

# Import numpy and make floating point division the default for Python 2.x
from __future__ import division 
import numpy as np
import matplotlib.pyplot as plt

# Load data 
S = np.load('eruptions.npy')  # vector of observed eruption times
T = np.load('waiting.npy')    # vector of observed waiting times
n = S.shape[0]                # number of observations

# Plot data
plt.plot(S, T, 'ok')
plt.xlabel('Eruption Time (minutes)')
plt.ylabel('Waiting Time to Next Eruption (minutes)')
plt.show()

# Compute mean under empirical distribution
meanS = np.sum(S)/n
meanT = np.sum(T)/n

# Thresholds used to define X,Y variables in parts (c,d)
threshX = 3.5
threshY = 70



