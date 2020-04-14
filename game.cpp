#include <stdio.h>
#include <stdlib.h>

#define WIDTH 7
#define HEIGHT 6

// This struct is what is returned by the isBoardWinnable function. The expectedResult is 1 if
// we can win from this position, 0 if we can tie but not win, and -1 if we're sure to lose.
// The next move is the column we should play to get the best result for us.
typedef struct Tuple {
    int expectedResult;
    int nextMove;
} Tuple;

void printBoard(char** board) {
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++)
            printf("%c ", board[i][j]);
        printf("\n");
    }
}

int makeMove(char **board, char player, int column) {
    if (board[0][column] != '-') {
        // illegal move because column is already full
        return -1;
    }

    // the move will change the last empty position in that row
    int rowToChange;
    for (rowToChange=0; rowToChange<HEIGHT-1 && board[rowToChange+1][column]=='-'; rowToChange++) {}

    board[rowToChange][column] = player;

    return 1;
}

char checkWinnerSmall(char** board) {
    // vertically
    for (int i=0; i<HEIGHT-2; i++)
        for (int j=0; j<WIDTH; j++) {
            if (
                board[i][j] != '-'
                && board[i][j] == board[i+1][j]
                && board[i][j] == board[i+2][j]
            ) return board[i][j];
        }

    // horizontally
    for (int i=0; i<HEIGHT; i++)
        for (int j=0; j<WIDTH-2; j++) {
            if (
                board[i][j] != '-'
                && board[i][j] == board[i][j+1]
                && board[i][j] == board[i][j+2]
            ) return board[i][j];
        }

    // diagonally (primary)
    for (int i=0; i<HEIGHT-2; i++)
        for (int j=0; j<WIDTH-2; j++)
            if (
                board[i][j] != '-'
                && board[i][j] == board[i+1][j+1]
                && board[i][j] == board[i+2][j+2]
            ) return board[i][j];

    // diagonally (secondary)
    for (int i=0; i<HEIGHT-2; i++)
        for (int j=2; j<WIDTH; j++)
            if (
                board[i][j] != '-'
                && board[i][j] == board[i+1][j-1]
                && board[i][j] == board[i+2][j-2]
            ) return board[i][j];

    // If no one has won and all the columns are full we return 't', for tie,
    // otherwise we return '-', meaning there are still valid movements to be made.
    for (int j=0; j<WIDTH; j++)
        if (board[0][j] == '-') return '-';

    return 't';
}

char checkWinner(char** board) {
    // vertically
    for (int i=0; i<HEIGHT-3; i++)
        for (int j=0; j<WIDTH; j++) {
            if (
                board[i][j] != '-'
                && board[i][j] == board[i+1][j]
                && board[i][j] == board[i+2][j]
                && board[i][j] == board[i+3][j]
            ) return board[i][j];
        }

    // horizontally
    for (int i=0; i<HEIGHT; i++)
        for (int j=0; j<WIDTH-3; j++) {
            if (
                board[i][j] != '-'
                && board[i][j] == board[i][j+1]
                && board[i][j] == board[i][j+2]
                && board[i][j] == board[i][j+3]
            ) return board[i][j];
        }

    // diagonally (primary)
    for (int i=0; i<HEIGHT-3; i++)
        for (int j=0; j<WIDTH-3; j++)
            if (
                board[i][j] != '-'
                && board[i][j] == board[i+1][j+1]
                && board[i][j] == board[i+2][j+2]
                && board[i][j] == board[i+3][j+3]
            ) return board[i][j];

    // diagonally (secondary)
    for (int i=0; i<HEIGHT-3; i++)
        for (int j=3; j<WIDTH; j++)
            if (
                board[i][j] != '-'
                && board[i][j] == board[i+1][j-1]
                && board[i][j] == board[i+2][j-2]
                && board[i][j] == board[i+3][j-3]
            ) return board[i][j];

    // If no one has won and all the columns are full we return 't', for tie,
    // otherwise we return '-', meaning there are still valid movements to be made.
    for (int j=0; j<WIDTH; j++)
        if (board[0][j] == '-') return '-';

    return 't';
}

Tuple isBoardWinnable(char** board, char player, char enemy, int origin) {
    static int count = 0;
    //printf("\n\n%d\n", count);
    int current = count;
    count = count + 1;

    //printf("base board\n");
    //printBoard(board);

    // This function receives a board state, and recursively simulates movements to check if
    // there's a sure way to win from the received board. If there is, this returns 1, otherwise
    // returns -1 if we're sure to lose and 0 if we can tie.

    Tuple returnTuple;
    returnTuple.nextMove = -1;

    // The first thing we check is whether we've already won, lost or tied:
    char winner = checkWinner(board);
    if (winner == player) {
        returnTuple.expectedResult = 1;
        //printf("%d returning win to %d\n", current, origin);
        return returnTuple;
    }
    if (winner == 't') {
        returnTuple.expectedResult = 0;
        //printf("%d returning tie to %d\n", current, origin);
        return returnTuple;
    }
    if (winner != '-') {
        returnTuple.expectedResult = -1;
        //printf("%d returning loss to %d\n", current, origin);
        return returnTuple;
    }

    // We'll create one board to simulate each possible movement
    // Since there's one possible movement per column, we create WIDTH simulated boards
    char*** simulatedBoards;
    simulatedBoards = (char***)malloc(WIDTH*sizeof(char**));
    for (int k=0; k<WIDTH; k++) {
        simulatedBoards[k] = (char**)malloc(HEIGHT*sizeof(char*));
        for (int i=0; i<HEIGHT; i++)
            simulatedBoards[k][i] = (char*)malloc(WIDTH*sizeof(char));
    }

    // And we'll initialize them with the current board state
    for (int k=0; k<WIDTH; k++)
        for (int i=0; i<HEIGHT; i++)
            for (int j=0; j<WIDTH; j++)
                simulatedBoards[k][i][j] = board[i][j];

    // This is an array in the format [1, 1, -1, 1, ...], where a 1 in a given index means that
    // a movement in that column is a valid one. If the column is full, we'll have a -1 in that column's
    // index.
    int validMovements[WIDTH];
    for (int k=0; k<WIDTH; k++)
        validMovements[k] = makeMove(simulatedBoards[k], player, k);

    for (int k=0; k<WIDTH; k++) {
        if (validMovements[k] > 0) {
            //printf("simulated board %d\n", k);
            //printBoard(simulatedBoards[k]);
        }
    }

    // If we win in any of the simulated boards, the answer is yes
    for (int k=0; k<WIDTH; k++)
        // We check if the movement is valid to prevent unnecessary calculations
        if (validMovements[k]>0 && checkWinner(simulatedBoards[k]) == player) {
            returnTuple.expectedResult = 1;
            returnTuple.nextMove = k;
            //printf("\n%d returning win %d %d to %d\n", current, returnTuple.expectedResult, returnTuple.nextMove, origin);
            return returnTuple;
        }

    // Otherwise we need to simulate all possible responses to each movement we can make,
    // and choose the one that gives us the best result, assuming the other player will always
    // choose the best move for them.

    int bestPossibleResult = -2; // We start assuming we'll lose until we find a better possibility
    for (int k=0; k<WIDTH; k++) {
        char*** simulatedResponses;
        simulatedResponses = (char***)malloc(WIDTH*sizeof(char**));
        for (int r=0; r<WIDTH; r++) {
            simulatedResponses[r] = (char**)malloc(HEIGHT*sizeof(char*));
            for (int i=0; i<HEIGHT; i++)
                simulatedResponses[r][i] = (char*)malloc(WIDTH*sizeof(char));
        }

        // Simulating the possible responses is similar to simulating the possible movements.
        for (int r=0; r<WIDTH; r++)
            for (int i=0; i<HEIGHT; i++)
                for (int j=0; j<WIDTH; j++)
                    simulatedResponses[r][i][j] = simulatedBoards[k][i][j];

        int validResponses[WIDTH];
        for (int r=0; r<WIDTH; r++) {
            if (validMovements[k] < 0) {
                validResponses[r] = -1; // If the original movement was invalid so is any response
            } else {
                validResponses[r] = makeMove(simulatedResponses[r], enemy, r);
            }
        }

        for (int r=0; r<WIDTH; r++) {
            if (validResponses[r] > 0) {
                //printf("simulated response %d (current %d)\n", r, current);
                //printBoard(simulatedResponses[r]);
            }
        }

        // The enemy will choose what's worst for us, so here we need get the minimum simulated value
        int worstPossibleResult = 2; // We need to start with an invalid value that is bigger than all the options
        Tuple isWinnable;
        for (int r=0; r<WIDTH; r++)
            if (validResponses[r] > 0) {
                //printf("\n calling for response %d", r);
                isWinnable = isBoardWinnable(simulatedResponses[r], player, enemy, current);
                if (isWinnable.expectedResult < worstPossibleResult) worstPossibleResult = isWinnable.expectedResult;
            }

        // If the worst possible result for this move is 1, we don't need to keep on searching,
        // because we've already found a move that is sure to make us win
        if (worstPossibleResult == 1) {
            returnTuple.expectedResult = 1;
            returnTuple.nextMove = k;
            //printf("\n%d returning rec1 %d %d to %d\n", current, returnTuple.expectedResult, returnTuple.nextMove, origin);
            return returnTuple;
        }

        // worstPossibleResult still being 2 means all moves were invalid
        if (worstPossibleResult < 2 && worstPossibleResult > bestPossibleResult) {
            bestPossibleResult = worstPossibleResult;
            returnTuple.expectedResult = bestPossibleResult;
            returnTuple.nextMove = k;
        }
    }

    // If my move was the last possible one and I didn't win, return a tie with that movement
    if (returnTuple.nextMove == -1) {
        int lastMove;
        for (lastMove = 0; lastMove<WIDTH+1 && validMovements[lastMove]<0; lastMove++) {}
        returnTuple.expectedResult = 0;
        returnTuple.nextMove = lastMove;
    }
    //printf("\n%d returning rec2 %d %d to %d\n", current, returnTuple.expectedResult, returnTuple.nextMove, origin);
    return returnTuple;
}

int enemyMove(char** board, char player, char enemy) {
    return isBoardWinnable(board, player, enemy, -1).nextMove;
}

int main() {
    char** board;
    const char players[2] = {'x', 'o'};

    // initialize empty board
    board = (char**)malloc(HEIGHT*sizeof(char*));
    for (int i=0; i<HEIGHT; i++)
        board[i] = (char*)malloc(WIDTH*sizeof(char));

    for (int i=0; i<HEIGHT; i++)
        for (int j=0; j<WIDTH; j++)
            board[i][j] = '-';

    char currentPlayerIndex = 1;
    int chosenColumn;
    char winner = '-';
    while (winner == '-') {
        char currentPlayer = players[currentPlayerIndex];
        char enemy = players[(currentPlayerIndex+1)%2];

        if (currentPlayerIndex == 1) {
            // non-human player's turn
            makeMove(board, currentPlayer, enemyMove(board, currentPlayer, enemy));
        } else {
            printf("\nTabuleiro:\n");
            printBoard(board);

            printf("\nSua vez!");
            printf("\nDigite a coluna em que voce quer jogar: ");
            scanf(" %d", &chosenColumn);
            while (chosenColumn < 0 || chosenColumn >= WIDTH) {
                printf("Coluna invalida. Digite um numero entre 0 e %d: ", HEIGHT-1);
                scanf(" %d", &chosenColumn);
            }

            int isMoveValid = makeMove(board, currentPlayer, chosenColumn);
            while (isMoveValid < 0) {
                printf("Coluna cheia. Escolha outra: ");
                scanf(" %d", &chosenColumn);
                while (chosenColumn < 0 || chosenColumn >= WIDTH) {
                    printf("Coluna invalida. Digite um numero entre 0 e %d: ", HEIGHT-1);
                    scanf(" %d", &chosenColumn);
                }

                isMoveValid = makeMove(board, currentPlayer, chosenColumn);
            }
        }

        currentPlayerIndex = (currentPlayerIndex + 1) % 2;
        winner = checkWinner(board);
    }

    printBoard(board);
    printf("O vencedor eh o jogador %c!", winner);
}