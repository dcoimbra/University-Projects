import numpy as np
from sklearn import neighbors, datasets, tree, linear_model

from sklearn.externals import joblib
import timeit

from sklearn.model_selection import cross_val_score

def features(X):
    
    F = np.zeros((len(X),5))
    for x in range(0,len(X)):
        F[x, 0] = len(X[x]) #tamanho do vetor
        F[x, 1] = hasEvenVowels(X[x]) #numero de vogais
        F[x, 2] = X[x][0] #primeira letra
        F[x, 3] = X[x][-1] #ultima letra
        F[x, 4] =

    return F     

def mytraining(f,Y):
    
   
    return clf
    
def mytrainingaux(f,Y,par):
    
    return clf

def myprediction(f, clf):
    Ypred = clf.predict(f)

    return Ypred


def countVowels(word):

    count = 0
    vowels = set("aáâeéèêiíìioóòôuúù")

    for letter in word:
        if letter in vowels:
            count += 1

    return count


def hasEvenVowels(word):

    vowelCount = countVowels(word)

    return (vowelCount % 2 == 0)