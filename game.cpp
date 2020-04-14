#include <stdio.h>

#define WIDTH 7
#define HEIGHT 6

void printBoard(char board[HEIGHT][WIDTH]) {
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++)
            printf("%c ", board[i][j]);
        printf("\n");
    }
}

int makeMove(char board[HEIGHT][WIDTH], char player, int column) {
    if (board[0][column] != '-') {
        // illegal move because column is already full
        return -1;
    }

    // the move will change the last empty position in that row
    int rowToChange;
    for (rowToChange=0; board[rowToChange+1][column]=='-' && rowToChange<HEIGHT; rowToChange++) {}

    board[rowToChange][column] = player;
    return 1;
}

char checkWinner(char board[HEIGHT][WIDTH]) {
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

    return '-';
}


int main() {
    char board[HEIGHT][WIDTH];
    const char players[2] = {'x', 'o'};

    // initialize empty board
    for (int i=0; i<HEIGHT; i++)
        for (int j=0; j<WIDTH; j++)
            board[i][j] = '-';

    char currentPlayerIndex = 0;
    int chosenColumn;
    char winner = '-';
    while (winner == '-') {
        printBoard(board);

        char currentPlayer = players[currentPlayerIndex];
        printf("\nVez do jogador %c", currentPlayer);
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

        currentPlayerIndex = (currentPlayerIndex + 1) % 2;
        winner = checkWinner(board);
    }

    printBoard(board);
    printf("O vencedor eh o jogador %c!", winner);
}