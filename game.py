import pprint
import math

width = 4
height = 4
players = ['x', 'o']

def print_board(board):
    for row in range(height):
        print(" ".join(board[row]))


def make_move(board, player_idx, column):
    player = players[player_idx]

    if board[0][column] != '-':
        # column full
        return False

    last_empty_row = 0
    while last_empty_row < height - 1 and board[last_empty_row+1][column] == '-':
        last_empty_row = last_empty_row + 1

    board[last_empty_row][column] = player
    return True


def check_winner_small(board):
    # horizontally
    for row in range(height):
        for col in range(width-2):
            if (
                board[row][col] != '-'
                and board[row][col] == board[row][col+1]
                and board[row][col] == board[row][col+2]
            ):
                return board[row][col]

    # vertically
    for row in range(height-2):
        for col in range(width):
            if (
                board[row][col] != '-'
                and board[row][col] == board[row+1][col]
                and board[row][col] == board[row+2][col]
            ):
                return board[row][col]

    # diagonally (primary)
    for row in range(height-2):
        for col in range(width-2):
            if (
                board[row][col] != '-'
                and board[row][col] == board[row+1][col+1]
                and board[row][col] == board[row+2][col+2]
            ):
                return board[row][col]

    # diagonally (secondary)
    for row in range(height-2):
        for col in range(2, width):
            if (
                board[row][col] != '-'
                and board[row][col] == board[row+1][col-1]
                and board[row][col] == board[row+2][col-2]
            ):
                return board[row][col]

    # if there are empty space no one won yet
    for col in range(width):
        if board[0][col] == '-':
            return '-'

    # if there aren't the game is over on a tie
    return 't'


def check_winner(board):
    # horizontally
    for row in range(height):
        for col in range(width-3):
            if (
                board[row][col] != '-'
                and board[row][col] == board[row][col+1]
                and board[row][col] == board[row][col+2]
                and board[row][col] == board[row][col+3]
            ):
                return board[row][col]

    # vertically
    for row in range(height-3):
        for col in range(width):
            if (
                board[row][col] != '-'
                and board[row][col] == board[row+1][col]
                and board[row][col] == board[row+2][col]
                and board[row][col] == board[row+3][col]
            ):
                return board[row][col]

    # diagonally (primary)
    for row in range(height-3):
        for col in range(width-3):
            if (
                board[row][col] != '-'
                and board[row][col] == board[row+1][col+1]
                and board[row][col] == board[row+2][col+2]
                and board[row][col] == board[row+3][col+3]
            ):
                return board[row][col]

    # diagonally (secondary)
    for row in range(height-3):
        for col in range(3, width):
            if (
                board[row][col] != '-'
                and board[row][col] == board[row+1][col-1]
                and board[row][col] == board[row+2][col-2]
                and board[row][col] == board[row+3][col-3]
            ):
                return board[row][col]

    # if there are empty space no one won yet
    for col in range(width):
        if board[0][col] == '-':
            return '-'

    # if there aren't the game is over on a tie
    return 't'

def get_possible_next_positions(board, player_idx):
    simulated_boards = []
    for col in range(width):
        if board[0][col] == '-':
            new_board = [[board[i][j] for j in range(width)] for i in range(height)]
            make_move(new_board, player_idx, col)
            simulated_boards.append((new_board, col))

    return simulated_boards

def minmax(board, player_idx, alpha, beta):
    winner = check_winner_small(board) 
    if winner != '-':
        return {
            "eval": {
                players[0]: 1,
                players[1]: -1,
                't': 0,
            }[winner],
            "move": -1
        }

    possible_boards = get_possible_next_positions(board, player_idx)

    if player_idx == 0:
        max_eval = -math.inf
        next_move = -1
        for next_board in possible_boards:
            if alpha < beta:
                simulation = minmax(next_board[0], (player_idx + 1) % 2, alpha, beta)
                if simulation["eval"] > max_eval:
                    max_eval = simulation["eval"]
                    next_move = next_board[1]
                alpha = max(alpha, simulation["eval"])
        return {
            "eval": max_eval,
            "move": next_move
        }
        
    if player_idx == 1:
        min_eval = math.inf
        next_move = -1
        for next_board in possible_boards:
            if alpha < beta:
                simulation = minmax(next_board[0], (player_idx + 1) % 2, alpha, beta)
                if simulation["eval"] < min_eval:
                    min_eval = simulation["eval"]
                    next_move = next_board[1]
                beta = min(beta, simulation["eval"])
        return {
            "eval": min_eval,
            "move": next_move
        }


def game():
    board = [['-' for _ in range(width)] for _ in range(height)]
    player_idx = 1
    while check_winner_small(board) == '-':
        if player_idx == 1:
            # computer turn
            simulation = minmax(board, player_idx, -math.inf, math.inf)
            make_move(board, player_idx, simulation["move"])

        else:
            print("Tabuleiro:")
            print_board(board)

            print("\nSua vez!", players[player_idx])
            print("Digita a coluna em que quer jogar: ")
            column = int(input())

            while column < 0 or column >= width:
                print("Coluna inválida. Digite um número entre 0 e {}: ".format(width-1))
                column = int(input())

            valid_move = make_move(board, player_idx, column)
            while not valid_move:
                print("Coluna cheia. Escolha outra: ")
                column = int(input())
                while column < 0 or column >= width:
                    print("Coluna inválida. Digite um número entre 0 e {}: ".format(width-1))
                    column = int(input())

                valid_move = make_move(board, player_idx, column)

        player_idx = (player_idx + 1) % 2

    print_board(board)
    print("Vencedor: ", check_winner_small(board))

game()