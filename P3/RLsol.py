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

        new_Q = np.zeros((self.nS, self.nA))

        iteracoes = 0
        while True: # repete-se o calculo do Q ate convergir
            for line in trace:
                s = int(line[0])
                a = int(line[1])
                next_s = int(line[2])
                r = float(line[3])
                alpha = 0.1

                                                                        # max Q de todas as ações possíveis no estado seguinte
                new_Q[s, a] = new_Q[s, a] + alpha * (r + self.gamma * max(new_Q[next_s, :]) - new_Q[s, a])

            diff = np.linalg.norm(self.Q-new_Q)
            self.Q = np.copy(new_Q)
            iteracoes += 1

            if diff < 0.001:
                print("Iteracoes: ", iteracoes)
                break

        return self.Q
