# 57842 - Filipa Marques   Grupo 01   84708 - David Coimbra

from copy import deepcopy
from random import randint
from search import *


# Classes

class Group:
    def __init__(self):
        self._positions = []
        self._len = 0

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
        if not self._len:
            self._len = len(self._positions)
        return self._len

    def is_playable(self):
        if not self._len:
            return self.get_len() > 1
        return self._len > 1


# ------------------------------------------- #
class Board:
    def __init__(self, user_board):
        self._board = user_board

        self._size = [len(user_board), len(user_board[0])]
        self._all_positions = []

    def get_positions(self):
        """ returns a list of all positions (tuples) in the board"""
        if not self._all_positions:
            i = 0
            while i < self._size[0]:
                j = 0
                while j < self._size[1]:
                    pos = make_pos(i, j)
                    self._all_positions.append(pos)
                    j += 1
                i += 1
        return self._all_positions

    def get_board(self):
        return self._board

    def get_num_lines(self):
        return self._size[0]

    def get_num_columns(self):
        return self._size[1]

    def get_color(self, position):
        """ returns de color of a giver position"""
        return self._board[pos_l(position)][pos_c(position)]

    def set_color(self, position, new_color):
        self._board[pos_l(position)][pos_c(position)] = new_color

    def remove_color(self, position):
        self.set_color(position, get_no_color())

    def same_color(self, pos1, pos2):
        """ returns true if the two given positions have the same color and false otherwise.
        returns false if any of the positions is empty """
        if (not self.is_empty_position(pos1)) and (not self.is_empty_position(pos2)):
            return self.get_color(pos1) == self.get_color(pos2)
        return False

    def is_empty_position(self, position):
        return self.get_color(position) == get_no_color()

    def adjacent_positions(self, pos):
        """ returns a list of the adjacent positions to the given position within this board """
        line = pos_l(pos)
        col = pos_c(pos)
        res = []
        if line <= self.get_num_lines() and col <= self.get_num_columns():
            if line - 1 >= 0:  # left
                res.append(make_pos(line - 1, col))
            if line + 1 < self.get_num_lines():  # right
                res.append(make_pos(line + 1, col))
            if col + 1 < self.get_num_columns():  # up
                res.append(make_pos(line, col + 1))
            if col - 1 >= 0:  # down
                res.append(make_pos(line, col - 1))
        return res

    def swap_positions(self, pos1, pos2):
        """swaps two positions in the same board"""
        color1 = self.get_color(pos1)
        color2 = self.get_color(pos2)

        self.set_color(pos1, color2)
        self.set_color(pos2, color1)

    def remove_group(self, group):
        """ removes a single group from the board, substituting all positions to the value 0 (empty position)"""
        positions = group.get_group()

        for elem in positions:
            self.remove_color(elem)

    def get_last_line(self):
        return self._board[-1]

    def erase_column(self, index):
        for line in self.get_board():
            line.pop(index)
            line.append(get_no_color())


def is_board(board):
    """ returns true if the given argument is a list of lists of integers (representing a same game board)"""
    board_columns = 0
    for line in board:
        if not isinstance(line, list):
            return False
        line_size = len(line)

        if board_columns == 0:
            board_columns = line_size
        elif board_columns != line_size:
            return False
    return True


# ------------------------------------------- #
class sg_state:
    """ """

    def __init__(self, board):
        self.board = board
        self._objboard = Board(board)
        self._all_groups = []

    def __lt__(self, other_state):
        """ Returns the best state between the present one and one other state
         which have the same heuristic result. """
        if randint(0, 1):
            return True
        return False

    def get_objboard(self):
        return self._objboard

    def get_all_groups(self):
        if not self._all_groups:
            self._all_groups = board_find_groups(self.board)
        return self._all_groups


# ------------------------------------------- #
class same_game(Problem):
    """Models a Same Game problem as a satisfaction problem.
    A solution cannot have pieces left on the board."""

    def __init__(self, board):
        """The constructor specifies the initial state as a board."""
        Problem.__init__(self, sg_state(board), )
        self._board = Board(board)

    def actions(self, state):
        """Return the list groups that can be removed in the given
        state."""
        groups = state.get_all_groups()
        groups = list(filter(lambda group: (len(group) > 1), groups))
        return groups

    def result(self, state, action):
        """Return the state that results from removing the given
        group from the board. The action must be one of
        self.actions(state)"""
        return sg_state(board_remove_group(state.get_objboard().get_board(), action))

    def goal_test(self, state):
        """Return True if the state is a goal. The state is
        a goal if the board is empty. A board is empty if
        the lower-left position is empty."""
        board_lines = self._board.get_num_lines()

        return state.get_objboard().is_empty_position(make_pos(board_lines - 1, 0)) # checks if the lowest left corner is empty

    def path_cost(self, c, state1, action, state2):
        """Return the cost of a solution path that arrives at state2 from
        state1 via action, assuming cost c to get up to state1. The path doesn't
        matter, so this method costs 1 for every step in the path."""
        return c + 1

    def h(self, node):
        """Needed for informed search."""
        groups = node.state.get_all_groups()
        return len(groups)

# ------------------------------------------- #
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
    return l, c


def pos_l(pos):
    return pos[0]


def pos_c(pos):
    return pos[1]


def upper_position(pos):
    if pos_l(pos) - 1 >= 0:
        return make_pos(pos_l(pos) - 1, pos_c(pos))
    return


# ------------------------------------------- #
# Generic Same Game functions #
def board_find_groups(user_board):
    """ Returns a list with all colour groups (lists of positions) withing a given board """
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
        all_groups.append(single_group.get_group())
    return all_groups


def board_find_groups_aux(board, pos, final_group, rejects, visited):
    """ Auxiliary recursive function for board_find_groups:
     returns a single colour group in the given board """
    visited[pos] = True
    final_group.add_position(pos)
    aux_pos = board.adjacent_positions(pos)
    for position in aux_pos:
        if visited[position]:
            continue
        if (final_group.is_in_group(position)) or (not board.same_color(pos, position)):
            rejects.append(position)
            continue
        if not (position in rejects):
            board_find_groups_aux(board, position, final_group, rejects, visited)
    return final_group


# ------------------------------------------- #
def board_remove_group(user_board, user_group):
    """ Returns a new board which results from removing the given group from the given board,
    including the vertical and horizontal realignment of all pieces in said board."""
    original_board = Board(user_board)
    egroup = Group()
    egroup.set_group(user_group)
    if not egroup.is_playable():
        return user_board
    game_board = deepcopy(original_board)
    game_board.remove_group(egroup)
    board_vertical_align(game_board, egroup)
    board_horizontal_align(game_board)
    return game_board.get_board()


def board_vertical_align(board, group):
    aux = group.get_group()
    aux.sort()
    for position in aux:
        vertical_align_aux(board, position)


def vertical_align_aux(board, pos):
    upper = upper_position(pos)
    if upper and not board.is_empty_position(upper):
        board.swap_positions(pos, upper)
        vertical_align_aux(board, upper)


def board_horizontal_align(board):
    last_line = board.get_last_line()
    to_erase = False
    for i in range(board.get_num_columns() - 1, -1, -1):  # starts on rightmost position
        if color(last_line[i]):
            to_erase = True
        elif to_erase:
            board.erase_column(i)
