#         Grupo T001          #
#   57842 - Filipa Marques    #
#   84708 - David Coimbra     #

import numpy as np
from sklearn import datasets, tree, linear_model
from sklearn.kernel_ridge import KernelRidge
from sklearn.model_selection import cross_val_score
import timeit


def mytraining(X, Y):

    best_kernel_reg = KernelRidge()
    min_err = 2000

    for kernel in ['poly', 'linear', 'rbf', 'cosine']:
        reg = mytrainingaux(X, Y, kernel)

        err = -cross_val_score(reg, X, Y, cv=5, scoring='neg_mean_squared_error').mean()

        if (err < min_err):
            min_err = err
            best_kernel_reg = reg

    print("Best kernel: ", best_kernel_reg.get_params()['kernel'])
    print(min_err)

    return best_kernel_reg


def mytrainingaux(X, Y, par):
    reg = KernelRidge(kernel=par, gamma=0.08, alpha=0.001)

    reg = reg.fit(X, Y)

    return reg


def myprediction(X, reg):
    Ypred = reg.predict(X)

    return Ypred
