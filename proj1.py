from copy import deepcopy
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

    def is_playable(self):
        return len(self._positions) > 1

# ------------------------------- #
class Board:
    def __init__(self, user_board):
        if is_board(user_board):
            self._board = user_board

        self._size = [len(user_board), len(user_board[0])]
        self._all_positions = []

        i = 0
        while i < self._size[0]:
            j = 0
            while j < self._size[1]:
                pos = make_pos(i, j)
                self._all_positions.append(pos)
                j += 1
            i += 1

    def get_board(self):
        return self._board

    def get_num_lines(self):
        return self._size[0]

    def get_num_columns(self):
        return self._size[1]

    def get_positions(self):
        return self._all_positions

    def get_color(self, position):
        return self._board[pos_l(position)][pos_c(position)]

    def set_color(self, position, new_color):
        self._board[pos_l(position)][pos_c(position)] = new_color

    def remove_color(self, position):
        self.set_color(position, get_no_color())

    def same_color(self, pos1, pos2):
        if (not self.is_empty_position(pos1)) and (not self.is_empty_position(pos2)):
            return self.get_color(pos1) == self.get_color(pos2)
        return False

    def is_empty_position(self, position):
        return self.get_color(position) == get_no_color()

    def adjacent_positions(self, pos):
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
            if col + 1 < self.get_num_columns():
                res.append(make_pos(line, col + 1))
            if col - 1 >= 0:
                res.append(make_pos(line, col - 1))
        return res

    def under_position(self, pos):
        if pos_l(pos) + 1 < self.get_num_lines():
            return make_pos(pos_l(pos) + 1, pos_c(pos))
        return

    def swap_positions(self, pos1, pos2):
        """swaps two positions in the same board"""
        color1 = self.get_color(pos1)
        color2 = self.get_color(pos2)

        self.set_color(pos1, color2)
        self.set_color(pos2, color1)

    def is_empty(self):
        for position in self._all_positions:
            if not self.is_empty_position(position):
                return False
        return True

    def remove_group(self, group):
        """ removes a single group from the board, substituting all positions to the value 0"""
        positions = group.get_group()

        for elem in positions:
            self.remove_color(elem)

    def check_victory(self):
        """determines if the player has won with the current board"""
        groups = board_find_groups(self.get_board())

        if self.is_empty():
            return "You won"

        else:
            if len(groups) == 0:
                return "You lost"
            else:
                return "You can still play"

    def get_line(self, index):
        if 0 <= index < self.get_num_lines():
            return self._board[index]

    def get_last_line(self):
        return self._board[-1]

    def erase_column(self, index):
        for line in self.get_board():
            line.pop(index)
            line.append(get_no_color())

    def is_equal(self, board):
        return self._board == board.get_board()

    def empty_board(self):

        board_lines = self.get_num_lines()
        board_columns = self.get_num_columns()

        empty = [[get_no_color() for column in range(board_columns)] for line in range(board_lines)]

        return empty


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
    """ """

    def __init__(self, board):
        self.board = board
        self._objboard = Board(board)

    def __lt__(self, other_state):
        """Needed for informed search."""
        # TODO

    def get_board(self):
        return self._objboard


# ---------------------------- #
class same_game(Problem):
    """Models a Same Game problem as a satisfaction problem.
    A solution cannot have pieces left on the board."""

    def __init__(self, board):
        """The constructor specifies the initial state as a board."""
        Problem.__init__(self, sg_state(board),)
        self._board = Board(board)

    def actions(self, state):
        """Return the list groups that can be removed in the given
        state."""
        groups = board_find_groups(state.get_board().get_board())
        groups = list(filter(lambda group: (len(group) > 1), groups))

        return groups

    def result(self, state, action):
        """Return the state that results from removing the given
        group from the board. The action must be one of
        self.actions(state)"""
        return sg_state(board_remove_group(state.get_board().get_board(), action))

    def goal_test(self, state):
        """Return True if the state is a goal. The state is
        a goal if the board is empty. A board is empty if
        the lower-left position is empty."""
        board_lines = self._board.get_num_lines()

        return state.get_board().is_empty_position(make_pos(board_lines - 1, 0))

    def path_cost(self, c, state1, action, state2):
        """Return the cost of a solution path that arrives at state2 from
        state1 via action, assuming cost c to get up to state1. The path doesn't
        matter, so this method costs 1 for every step in the path."""
        return c + 1

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


def upper_position(pos):
    if pos_l(pos) - 1 >= 0:
        return make_pos(pos_l(pos) - 1, pos_c(pos))
    return


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
        single_group.get_group().sort()
        all_groups.append(single_group.get_group());
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


def board_remove_group(user_board, user_group):
    """ remove o grupo do tabuleiro fazendo a compactacao vertical e horizontal das pecas."""
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
    for i in range(board.get_num_columns() - 1, -1, -1):
        if color(last_line[i]):
            to_erase = True
        elif to_erase:
            board.erase_column(i)