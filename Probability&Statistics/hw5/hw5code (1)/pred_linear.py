import numpy as np

def pred_linear(x_train, y_train, x_test):
    '''
    Fit a Gaussian model to training data, and predict y given x_test
    Inputs:
    x_train: length-N vector
    y_train: length-N vector
    x_test:  length-N vector

    Outputs:
    y_pred:  length-M vector
    mu:      length-2 mean vector
    sigma:   2*2 symmetric covariance matrix
    '''

    # Mean of training data
    # mu[0] = E[X]
    # mu[1] = E[Y]
    mu = [np.mean(x_train), np.mean(y_train)]

    # Covariance of training data
    # sigma[0,0] = Var[X]
    # sigma[1,1] = Var[Y]
    # sigma[0,1] = sigma[1,0] = Cov[X,Y]
    sigma = np.cov([x_train, y_train], bias=True)

    # Predict y_test as conditional mean given x_test, under Gaussian model
    # y_pred[i] = E[Y | X=x_test[i]]

    y_pred = mu[1] * np.ones(x_test.shape)
    # TODO:  Improve prediction formula to account for observations x_test
    return y_pred, mu, sigma
