import numpy as np
from numpy import exp, max, min, sqrt
from numpy.linalg import det, inv
import matplotlib.pyplot as plt

def plot_contour(x_train, y_train, mu, sigma, n_steps):
# plot_contour    Plot elliptical contours of estimated bivariate normal density

    x_steps = ((max(x_train) - min(x_train))/n_steps)
    y_steps = ((max(y_train) - min(y_train))/n_steps)
    x = np.arange(min(x_train)-x_steps*100, max(x_train)+x_steps*100, x_steps)
    y = np.arange(min(y_train)-y_steps*100, max(y_train)+y_steps*100, y_steps)

    [X, Y] = np.meshgrid(x, y)
    inv_sigma = inv(sigma)
    Z = 1/np.sqrt(2*np.pi)/sqrt(det(sigma)) * exp(-1/2 *
        ((X -mu[0])**2*inv_sigma[0,0] +
        (Y - mu[1])**2*inv_sigma[1,1] +
        (X-mu[0])*(Y-mu[1])*2*inv_sigma[0,1]))

    plt.contour(X, Y, Z, 10)