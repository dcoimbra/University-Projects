#         Grupo T001          #
#   57842 - Filipa Marques    #
#   84708 - David Coimbra     #


import numpy as np
from sklearn import neighbors, datasets, tree, linear_model

from sklearn.externals import joblib
import timeit

from sklearn.model_selection import cross_val_score
from sklearn.metrics import confusion_matrix

def features(X):
    
    F = np.zeros((len(X), 5))
    for x in range(0,len(X)):
        F[x, 0] = len(X[x])  # tamanho da palavra
        F[x, 1] = hasEvenVowels(X[x])  # se numero de vogais e' par
        F[x, 2] = (len(X[x]) % 2 == 0)  # tamanho da palavra par
        F[x, 3] = has_letter_a(X[x])  # se tem a letra a
        F[x, 4] = hasAccent(X[x])  # se tem acentos

    return F     



def mytraining(f,Y):

    clf = tree.DecisionTreeClassifier(min_samples_split=2) #linear_model.LogisticRegression()
    clf = clf.fit(f, Y)
    mytrainingaux(f, Y, clf)
    return clf


def mytrainingaux(f, Y, clf):

    print(" 5-fold cross validation score (scoring='f1'):\n", cross_val_score(clf, f, Y, scoring='f1', cv=5))
    clf = clf.fit(f, Y)
    Ypred = clf.predict(f)
    print("confusion matrix: \n", confusion_matrix(Y, Ypred, labels=[1, 0]))

    return clf


def myprediction(f, clf):

    Ypred = clf.predict(f)

    return Ypred


def countVowels(word):

    count = 0
    vowels = set("aáâãeéèêiíìioóòôõuúù")

    for letter in word:
        if letter in vowels:
            count += 1
    return count


def hasEvenVowels(word):

    vowelCount = countVowels(word)
    return (vowelCount % 2 == 0)


def hasNumber(word):

    numbers = set('0123456789')

    for letter in word:
        if letter in numbers:
            return True
    return False

# nao esta' a ser usada
def lastLetterisVowel(word):

    vowels = set("aáâãeéèêiíìioóòôõuúù")
    return word[-1] in vowels


def hasAccent(word):

    accents = set("áâãéèêiíìióòôõúùç")

    for letter in word:
        if letter in accents:
            return True
    return False


def has_letter_a(word):
    for letter in word:
        if letter == 'a':
            return True
    return False
