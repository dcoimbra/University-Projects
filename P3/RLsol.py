#         Grupo T001          #
#   57842 - Filipa Marques    #
#   84708 - David Coimbra     #

# -*- coding: utf-8 -*-
"""
Created on Mon Oct 16 20:31:54 2017

@author: mlopes
"""
import numpy as np


def Q2pol(Q, eta=0):
    """
    Q2pol(Q, eta=0): returns the best action to take given the Q value for each state
    Q: quality matrix (Q value for each state and action)
    eta: percentage of exploration. Default value is 0, making this a deterministic policy
    """
    prob = ((100-eta)/100)
    pol = np.zeros((len(Q), len(Q[0])))
    i = 0
    for i in range(len(Q)):
        pol[i, Q[i].argmax()] = 1*prob

    return pol


class myRL:
    def __init__(self, nS, nA, gamma):
        self.nS = nS
        self.nA = nA
        self.gamma = gamma
        self.Q = np.zeros((nS, nA))

    def traces2Q(self, trace):

        # trace e' uma matriz em que cada linha contem o
        # estado atual, a acao executada, o estado seguinte e a recompensa
        # [s,                a,                  s',                 r]

        # Para calcular Q num certo estado, usa-se a seguinte formula:
        # Q[s, a] = Q[s, a] + alpha * (r + gamma * max(Q[s', a']) - Q[s, a])

        new_Q = np.zeros((self.nS, self.nA))

        while True: # repete-se o calculo do Q ate convergir
            for line in trace:
                s = int(line[0])
                a = int(line[1])
                next_s = int(line[2])
                r = float(line[3])
                alpha = 0.1

                                                                    # max Q de todas as acoes possíveis no estado seguinte
                new_Q[s, a] = new_Q[s, a] + alpha * (r + self.gamma * max(new_Q[next_s, :]) - new_Q[s, a])

            diff = np.sum(np.fabs(self.Q - new_Q)) # np.linalg.norm(self.Q - new_Q)
            self.Q = np.copy(new_Q)

            if diff < 0.0001:
                break

        return self.Q
