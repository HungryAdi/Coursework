'''
    UCI CS177: Automobile horsepower prediction using Gaussian regression
    This is DEMONSTRATION code:
    - It gives an example of how to load the automobile data,
      and compute and plot Gaussian statistics of this data.
    - You may (but do not have to) reuse parts of this code in your solutions.
    - It is NOT a template for the individual questions you must answer,
      for that see the main homework pdf.

    VARIABLES IN 'auto-mpg.mat'
    weight_train: vector of training data for weight
    weight_test:  vector of test data for weight
    horsepower_train: vector of training data for horsepower
    horsepower_test : vector of test data for horsepower
    mpg_train: vector of training data for miles per gallon (mpg)
    mpg_test : vector of test data for miles per gallon (mpg)

'''

from __future__ import division
from plot_contour import plot_contour
from pred_linear import pred_linear
from scipy.io import loadmat
import numpy as np
import matplotlib.pyplot as plt

def load_data():
    data = loadmat('auto-mpg.mat')
    for k, v in data.items():
        data[k] = np.squeeze(data[k])
    return data

# Load data
data = load_data()
weight_train = data['weight_train']
weight_test = data['weight_test']
horsepower_train = data['horsepower_train']
horsepower_test = data['horsepower_test']
mpg_train = data['mpg_train']
mpg_test = data['mpg_test']

Ntrain = len(horsepower_train)
Ntest = len(horsepower_test)

# Possible experiments: Uncomment line corresponding to part being worked on
useWeight = True  # (b) X=weight
# useWeight = False; # (c,d) X=mpg

# Define selected training and test data
if useWeight:
    Xtrain = weight_train
    Xtest  = weight_test
    Xlabel = 'Weight'
    Ytrain = horsepower_train
    Ytest  = horsepower_test
else:
    Xtrain = mpg_train
    Xtest  = mpg_test
    Xlabel = 'MPG'
    Ytrain = horsepower_train
    Ytest  = horsepower_test


# Predict Ytest given Xtest, and model learned from (Ytrain, Xtrain)
# TODO: Extend pred_linear.py to compute correct Ypred values
Ypred, mu, sigma = pred_linear(Xtrain, Ytrain, Xtest)

# Plot model learned from training data
plt.figure(1)
plt.plot(Xtrain, Ytrain, '.k')
plot_contour(Xtrain, Ytrain, mu, sigma, 100)
plt.xlabel(Xlabel)
plt.ylabel('Horsepower')
plt.title('Training Data')
plt.axis([1000, 6000, 25, 250])

# Compute error on test data
squared_error = np.sqrt((1/Ntest) * np.sum((Ypred - Ytest)**2))
print 'Average test error: %.4f\n' % squared_error

# Plot Ypred versus Ytest
plt.figure(2)
h1, = plt.plot(Xtest, Ytest, '.k')
h2, = plt.plot(Xtest, Ypred, '*r')
plt.xlabel(Xlabel)
plt.ylabel('Horsepower')
plt.legend([h1, h2], ['Ground truth test data', 'Predicted test data'])
plt.title('Test Data')

# Threshold to use for part (d)
thresh = 20

plt.show()
