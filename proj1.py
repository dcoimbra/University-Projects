from search import *
from utils import *


# Classes

class Group:

    def __init__(self):
        self._positions = []

    def get_group(self):
        return self._positions

    def set_group(self, group):
        self._positions = group

    def add_position(self, pos):
        self._positions.append(pos)

    def remove_position(self, pos):
        self._positions.remove(pos)

    def is_in_group(self, pos):
        return pos in self._positions

    def get_len(self):
        return len(self._positions)


# ------------------------------- #
class Board:

    def __init__(self, user_board):
        self._all_positions = []
        self._board = []
        self._size = []

        if is_board(user_board):
            self._board = user_board

        self._size = [len(user_board), len(user_board[0])]

        i = 0
        while i < self._size[0]:
            j = 0
            while j < self._size[1]:
                pos = make_pos(i,j)
                self._all_positions.append(pos)
                j += 1
            i += 1

    def get_board(self):
        return self._board

    def get_size(self):
        return self._size

    def get_num_lines(self):
        return self._size[0]

    def get_num_columns(self):
        return self._size[1]

    def get_positions(self):
        return self._all_positions

    def get_color(self, position):
        return self._board[pos_l(position)][pos_c(position)]

    def set_color(self, position, color):
        self._board[pos_l(position)][pos_c(position)] = color

    def remove_color(self, position):
        self.set_color(position, get_no_color())

    def is_empty_position(self, position):
        return self.get_color(position) == get_no_color()

    def same_color(self, pos1, pos2):
        if (not self.is_empty_position(pos1)) and (not self.is_empty_position(pos2)):
            return self.get_color(pos1) == self.get_color(pos2)
        return False

    def is_empty(self):
        for position in self._all_positions:
            if not self.is_empty_position(position):
                return False
        return True

    def adjacent_pos(self, pos):
        """

        :param pos: an object of type position
        :return: a list of the adjacent positions to the given one within this board
        """
        line = pos_l(pos)
        col = pos_c(pos)
        res = []
        if line <= self.get_num_lines() and col <= self.get_num_columns():
            if line - 1 >= 0:
                res.append(make_pos(line - 1, col))

            if line + 1 < self.get_num_lines():
                res.append(make_pos(line + 1, col))

            if col - 1 >= 0:
                res.append(make_pos(line, col - 1))

            if col + 1 < self.get_num_columns():
                res.append(make_pos(line, col + 1))
        return res

    def remove_group(self, group):
        """ removes a single group from the board, substituting all positions to the value 0"""
        positions = group.get_group()

        for elem in positions:
            self.remove_color(elem)

    def swap_positions(self, pos1, pos2):
        """swaps two positions in the same board"""

        color1 = self.get_color(pos1)
        color2 = self.get_color(pos2)

        self.set_color(pos1, color2)
        self.set_color(pos2, color1)

    def upper_position(self, pos):
        if (pos_l(pos) - 1 >= 0):
            return make_pos(pos_l(pos) - 1, pos_c(pos))
        return

    def get_line(self, index):
        if (index >= 0 and index < self.get_num_lines()):
            return self._board[index]

    def get_last_line(self):
        return self._board[-1]



    def print_board(self):
        """
        sends to stdout a graphic representation of a board
        :return:
        """
        result = "| "
        for l in self._board:
            for i in l:
                if i == get_no_color():
                    i = "_"
                result += str(i)+" "
            print(result+"|")
            result = "| "
        return



def is_board(board):
    board_lines = 0
    board_columns = 0
    for line in board:
        if not isinstance(line, list):
            return False
        board_lines += 1
        line_size = len(line)

        if board_columns == 0:
            board_columns = line_size
        elif board_columns != line_size:
            return False
    return True


# --------------------------- #
class sg_state:
    """Needed for informed search."""

    def __lt__(self, other):
        """ """
        # TODO

# ---------------------------- #
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


# Generic Same Game functions #
def board_find_groups(user_board):
    """
    :param user_board: list of lists containing the colour of each position
    :return: a list with all colour groups (lists of positions) withing a given board
    """
    board = Board(user_board)
    all_positions = board.get_positions()
    all_groups = []
    visited = {}
    for position in all_positions:
        visited[position] = False
    for position in all_positions:
        if board.is_empty_position(position) or visited[position]:
            continue
        single_group = board_find_groups_aux(board, position, Group(), [], visited)
        if single_group.get_len() > 1:
            all_groups.append(single_group.get_group())
    return all_groups


def board_find_groups_aux(board, pos, final_group, rejects, visited):
    """
    Auxiliary recursive function for board_find_groups
    :param board: an object of class Board
    :param pos: a tuple of type position
    :param final_group: an object of type group (a list of positions)
    :param rejects: a list of all positions which do not have the same colour as the position in search
    :param visited:  a list of all searched positions
    :return: the complete group which the given position is a part of
    """
    visited[pos] = True
    final_group.add_position(pos)
    aux_pos = board.adjacent_pos(pos)
    for position in aux_pos:
        if visited[position]:
            continue
        if (final_group.is_in_group(position)) or (not board.same_color(pos, position)):
            rejects.append(position)
            continue
        if not (position in rejects):
            board_find_groups_aux(board, position, final_group, rejects, visited)
    return final_group


def board_remove_group(user_board, user_group):
    """ remove o grupo do tabuleiro fazendo a compactacao vertical e horizontal das pecas."""
    # TODO


def vertical_align_aux(board, pos):
    upper = board.upper_position(pos)
    if upper and not board.is_empty_position(upper):
        board.swap_positions(pos, upper)
        vertical_align_aux(board, upper)

def board_vertical_align(board, group):
    aux = group.get_group()
    for position in aux:
        vertical_align_aux(board, position)

def horizontal_align(board):

    last_line = board.get_last_line()
    index = -1

    for color in last_line:
        index += 1
        if no_color(color):
            erase_column(board, index)


def erase_column(board, index):
    for line in board.get_board():
        line.pop(index)
        line.append(get_no_color())


