#         Grupo T001          #
#   57842 - Filipa Marques    #
#   84708 - David Coimbra     #

# -*- coding: utf-8 -*-
"""
Created on Mon Oct 16 20:31:54 2017

@author: mlopes
"""
import numpy as np


def Q2pol(Q, eta=5):
    return


class myRL:
    def __init__(self, nS, nA, gamma):
        self.nS = nS
        self.nA = nA
        self.gamma = gamma
        self.Q = np.zeros((nS, nA))

    def traces2Q(self, trace):
        # implementar esta funcao

        # o coimbra escreveu o cabecalho abaixo lol

        # trace e' uma matriz em que cada linha contém o
        # estado atual, a acao executada, o estado seguinte e a recompensa

        # [s, a, s', r]

        # Para calcular Q num certo estado, usa-se a seguinte formula:
        # Q[s, a] = Q[s, a] + alpha * (r + gamma * max(Q[s', a']) - Q[s, a])

        for i in range(215): # repete-se o calculo do Q ate estabilizar
            for line in trace:
                s = int(line[0])
                a = int(line[1])
                next_s = int(line[2])
                r = int(line[3])
                alpha = 0.01

                                                                        # max Q de todas as ações possíveis no estado seguinte
                self.Q[s, a] = self.Q[s, a] + alpha * (r + self.gamma * max(self.Q[next_s, :]) - self.Q[s, a])
        return self.Q
    