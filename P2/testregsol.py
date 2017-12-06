import numpy as np
from sklearn import datasets, tree, linear_model
from sklearn.kernel_ridge import KernelRidge
from sklearn.model_selection import cross_val_score
from sklearn.externals import joblib
from time import time
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap

import regsol

tres = [.3, 800]

filename_test = "test1.svg"

for ii, test in enumerate(["regress.npy", "regress2.npy"]):
    print("Testing " + test)

    X, Y, Xp, Yp = np.load(test)

    fit_time_start = time()

    reg = regsol.mytraining(X, Y)

    fit_time = time() - fit_time_start
    print("Fit time:", fit_time)


    predict_time_start = time()

    Ypred = regsol.myprediction(Xp, reg)

    predict_time = time() - predict_time_start
    print("Predict time:", predict_time)

    cross_val_time_start = time()

    val = -cross_val_score(reg, X, Y, cv=5, scoring='neg_mean_squared_error').mean()

    cross_val_time = time() - cross_val_time_start
    print("Time to calculate mean squared error:", cross_val_time)

    print("Total time:", fit_time + predict_time + cross_val_time)

    if  val < tres[ii]:
        print("Erro dentro dos limites de tolerância. OK Score:", val, "tres: ", tres[ii])
    else:
        print("Erro acima dos limites de tolerância. FAILED Score:", val, "tres: ", tres[ii])
    plt.figure()
    plt.plot(Xp, Yp, 'g.', label='datatesting')
    plt.plot(X, Y, 'k+', label='datatrain')
    plt.plot(Xp, Ypred, 'm', label='linregres1')
    plt.legend(loc=1)

    plt.savefig(filename_test)

    filename_test = "test2.svg"


plt.show()
