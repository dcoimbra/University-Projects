from search import *
from utils import *


############# Classes #################

class sg_state:
    """Needed for informed search."""

    def __lt__(self, other):
        """ """
        # TODO



class same_game(Problem):
    """Models a Same Game problem as a satisfaction problem.
    A solution cannot have pieces left on the board."""

    def __init__(self, board):
        """ """
        # TODO

    def actions(self, state):
        """ """
        # TODO

    def result(self, state, action):
        """ """
        # TODO

    def goal_test(self, state):
        """ """
        # TODO

    def path_cost(self, c, state1, action, state2):
        """ """
        # TODO

    def h(self, node):
        """Needed for informed search."""
        # TODO


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
    """ """
    # TODO


# TAI board
# Lista: linhas do tabuleiro
def board():
    """ """
    #  TODO

def is_board(board):
    num_l = 0;
    num_c = 0;

    for l in board:
        if not isinstance(l, list):
            return False;
        num_l += 1;

        cols = len(l);
        if num_c == 0:
            num_c = cols;
        elif num_c != cols:
            return False;
    return True;

def board_find_groups(board):
    """devolve uma lista com os grupos de pecas que se podem encontrar no tabuleiro"""
    # TODO


def board_remove_group(board, group):
    """ remove o grupo do tabuleiro fazendo a compactacao vertical e horizontal das pecas."""
    # TODO
