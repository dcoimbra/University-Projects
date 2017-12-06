#         Grupo T001          #
#   57842 - Filipa Marques    #
#   84708 - David Coimbra     #

import numpy as np
from sklearn import datasets, tree, linear_model
from sklearn.kernel_ridge import KernelRidge
from sklearn.svm import SVR
from sklearn.model_selection import cross_val_score, GridSearchCV
import timeit
from time import time


def mytraining(X, Y):

    Y = Y.ravel()

    reg = KernelRidge().fit(X, Y)

    param_grid = [
        {
            'alpha': [0.1, 0.01, 0.001, 0.0001], 'gamma': [0.1, 0.01, 0.001], 'kernel': ['rbf', 'poly']
        }
    ]

    '''
    reg = SVR().fit(X, Y)

    param_grid = [
         {
            'C': [100, 1000, 10000, 100000], 'gamma': [0.1, 0.01], 'kernel': ['rbf'],
         }
    ]
    '''

    search = GridSearchCV(reg, param_grid, scoring='neg_mean_squared_error', cv=5)


    fit_time_start = time()

    search.fit(X, Y)

    fit_time = time() - fit_time_start

    print("Fit time:", fit_time)

    reg = search.best_estimator_

    print(reg)

    return reg

def mytrainingaux(X, Y, par):

    return reg


def myprediction(X, reg):

    predict_time_start = time()

    Ypred = reg.predict(X)

    predict_time = time() - predict_time_start

    print("Predict time:", predict_time)

    return Ypred
