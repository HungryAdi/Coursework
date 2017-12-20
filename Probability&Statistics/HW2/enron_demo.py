'''
    UCI CS177: Naive Bayes Spam Classification
    This is DEMONSTRATION code:
    It gives an example of how to load the Enron email training & test data,
    and computing simple statistics of this data.
    You may (but do not have to) reuse parts of this code in your solutions.
    It is NOT a template for the individual questions you must answer,
    for that see the main homework pdf.
'''

# Import numpy and make floating point division the default for Python 2.x
import numpy as np
from scipy.io import loadmat

def load_data():
    data = loadmat('enron.mat')
    trainFeat = np.array(data['trainFeat'], dtype=bool)
    trainLabels = np.squeeze(data['trainLabels'])
    testFeat = np.array(data['testFeat'], dtype=bool)
    testLabels = np.squeeze(data['testLabels'])
    vocab = np.squeeze(data['vocab'])
    vocab = [vocab[i][0].encode('ascii', 'ignore') for i in xrange(len(vocab))]
    data = dict(trainFeat=trainFeat, trainLabels=trainLabels,
                testFeat=testFeat, testLabels=testLabels, vocab=vocab)
    return data

# Load data
data = load_data()
trainFeat = data['trainFeat']
trainLabels = data['trainLabels']
testFeat = data['testFeat']
testLabels = data['testLabels']
vocab = data['vocab']
W = len(vocab)
'''
    Data description:
    - trainFeat: (Dtrain, W) logical 2d-array of word appearance for training documents.
    - trainLabels: (Dtrain,) 1d-array of {0,1} training labels where 0=ham, 1=spam.
    - testFeat: (Dtest, W) logical 2d-array of word appearance for test documents.
    - testLabels:  (Dtest,) 1d-array of {0,1} test labels where 0=ham, 1=spam.
    - vocab: (W,) 1d-array where vocab[i] is the English characters for word i.
'''

# Different possible vocabularies to use in classification, uncomment chosen line
# vocabInds =  179  # Part (c): "money"
# vocabInds =  859  # Part (d): "thanks"
# vocabInds = 2211  # Part (e): "possibilities"
# vocabInds = [179, 859, 2211]  # Part (f): "money", "thanks", & "possibilities"
vocabInds = np.arange(W)  # Part (g): full vocabularly of all W words

# Separate "ham" and "spam" classes, subsample selected vocabulary words
trainHam  = trainFeat[trainLabels == 0][:, vocabInds]
trainSpam = trainFeat[trainLabels == 1][:, vocabInds]

# Number of training examples of each class
numHam = len(trainHam)
numSpam = len(trainSpam)

# Count number of times each word occurs in each class
countsHam = np.sum(trainHam, axis=0)
# P(X_ij=1 | Y_i=H) can be computed from countsHam and numHam
countsSpam = np.sum(trainSpam, axis=0)
# P(X_ij=1 | Y_i=S) can be computed from countsSpam and numSpam

# Display words that are common in one class, but rare in the other
ind = np.argsort(countsHam-countsSpam)
print 'Words common in Ham but not Spam:'
for i in xrange(-1, -100, -1):
    print vocab[ind[i]],
print
print 'Words common in Spam but not Ham:'
for i in xrange(100):
    print vocab[ind[i]],
