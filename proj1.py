from search import *
from utils import *


############# Classes #################

class sg_state:


    def __lt__(self, other):
        """Needed for informed search."""


class same_game(Problem):
    """Models a Same Game problem as a satisfaction problem.
    A solution cannot have pieces left on the board."""

    def __init__(self, board):

    def actions(self, state):

    def result(self, state, action):

    def goal_test(self, state):

    def path_cost(self, c, state1, action, state2):

    def h(self, node):
        """Needed for informed search."""


############# TAIs ###############

# TAI color
# sem cor = 0
# com cor > 0
def get_no_color():
    return 0


def no_color(c):
    return c == 0


def color(c):
    return c > 0


# TAI pos
# Tuplo (l, c)
def make_pos(l, c):
    return (l, c)


def pos_l(pos):
    return pos[0]


def pos_c(pos):
    return pos[1]


# TAI group
# Lista: elementos do tipo pos
def group(position_list):


# TAI board
# Lista: linhas do tabuleiro
def board():


def board_find_groups(board):
    """devolve uma lista com os grupos de pecas que se podem encontrar no tabuleiro"""


def board_remove_group(board, group):
    """ remove o grupo do tabuleiro fazendo a compactacao vertical e horizontal das pecas."""
