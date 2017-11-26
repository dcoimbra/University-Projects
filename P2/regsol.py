#         Grupo T001          #
#   57842 - Filipa Marques    #
#   84708 - David Coimbra     #

import numpy as np
from sklearn import datasets, tree, linear_model
from sklearn.kernel_ridge import KernelRidge
from sklearn.model_selection import cross_val_score
import timeit


def mytraining(X, Y):

    reg = KernelRidge(kernel='rbf', gamma=0.08, alpha=0.001)
    reg = reg.fit(X, Y)
    return reg


def mytrainingaux(X, Y, par):
    reg.fit(X, Y)

    return reg


def myprediction(X, reg):
    Ypred = reg.predict(X)

    return Ypred
