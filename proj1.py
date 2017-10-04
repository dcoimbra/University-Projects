from search import *
from utils import *


# Classes

class group:
    positions = []

    def __init__(self, positions):
        self.positions = positions

    def add_position(self, pos):
        self.positions.append(pos)

    def remove_position(self, pos):
        self.positions.remove(pos)

    def get_positions(self):
        return self.positions


class board:
    lines = []

    def __init__(self, lines):
        if is_board(lines):
            self.lines = lines

    def get_lines(self):
        return self.lines


class sg_state:
    """Needed for informed search."""

    def __lt__(self, other):
        """ """
        # TODO


class same_game(Problem):
    """Models a Same Game problem as a satisfaction problem.
    A solution cannot have pieces left on the board."""

    board = []

    def __init__(self, board):
        """ """
        self.board = board

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


# TAIs #


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


# TAI board
# Lista: linhas do tabuleiro

def is_board(board):
    num_l = 0
    num_c = 0

    for l in board:

        if not isinstance(l, list):
            return False

        num_l += 1
        cols = len(l)

        if num_c == 0:
            num_c = cols

        elif num_c != cols:
            return False

    return True


def make_group(positions):
    return group(positions)


def make_board(lines):
    return board(lines)


def board_find_groups(board):
    """devolve uma lista com os grupos de pecas que se podem encontrar no tabuleiro"""
    # TODO


def board_remove_group(board, group):
    """ remove o grupo do tabuleiro fazendo a compactacao vertical e horizontal das pecas."""
    # TODO
