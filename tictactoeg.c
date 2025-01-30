#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <MMsystem.h>

#define MAX_N 10  
int N;
char board[MAX_N][MAX_N];  

void initializeBoard();
void drawBoard();
int checkWin();
int minimax(int depth, int isMaximizing);
int findBestMove();
void playSound(int frequency);
int isMovesLeft();

void playSound(int frequency) {
    Beep(frequency, 300);
}

void initializeBoard() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            board[i][j] = ' ';
}

void drawBoard() {
    system("cls");
    printf("\n\n\t \033[1;36mTic Tac Toe (%d x %d)\033[0m \n\n", N, N);
    printf("\033[1;33mPlayer1 (X) - Player2/Computer (O)\033[0m \n\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            char symbol = board[i][j];
            if (symbol == 'X')
                printf("  \033[1;31m%c\033[0m  ", symbol); '
            else if (symbol == 'O')
                printf("  \033[1;34m%c\033[0m  ", symbol);  
            else
                printf("     ");

            if (j < N - 1) printf("\033[1;32m|\033[0m"); 
        }
        printf("\n");
        if (i < N - 1) {
            for (int j = 0; j < N; j++)
                printf("\033[1;32m-----\033[0m "); 
            printf("\n");
        }
    }
}

int checkWin() {
    for (int i = 0; i < N; i++) {
        int rowWin = 1, colWin = 1;
        for (int j = 1; j < N; j++) {
            if (board[i][j] != board[i][0] || board[i][j] == ' ') rowWin = 0;
            if (board[j][i] != board[0][i] || board[j][i] == ' ') colWin = 0;
        }
        if (rowWin && board[i][0] != ' ') return (board[i][0] == 'X') ? 10 : -10;
        if (colWin && board[0][i] != ' ') return (board[0][i] == 'X') ? 10 : -10;
    }

    int diag1 = 1, diag2 = 1;
    for (int i = 1; i < N; i++) {
        if (board[i][i] != board[0][0] || board[i][i] == ' ') diag1 = 0;
        if (board[i][N - i - 1] != board[0][N - 1] || board[i][N - i - 1] == ' ') diag2 = 0;
    }
    if (diag1 && board[0][0] != ' ') return (board[0][0] == 'X') ? 10 : -10;
    if (diag2 && board[0][N - 1] != ' ') return (board[0][N - 1] == 'X') ? 10 : -10;

    return isMovesLeft() ? -1 : 0;
}

int isMovesLeft() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (board[i][j] == ' ')
                return 1;
    return 0;
}

int minimax(int depth, int isMaximizing) {
    int score = checkWin();
    if (score == 10) return 10 - depth;
    if (score == -10) return -10 + depth;
    if (score == 0) return 0;

    int bestScore = isMaximizing ? -1000 : 1000;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = isMaximizing ? 'O' : 'X';
                int currentScore = minimax(depth + 1, !isMaximizing);
                board[i][j] = ' ';
                bestScore = isMaximizing ? (currentScore > bestScore ? currentScore : bestScore)
                                         : (currentScore < bestScore ? currentScore : bestScore);
            }
        }
    }
    return bestScore;
}

int findBestMove() {
    int bestMoveX = -1, bestMoveY = -1;
    int bestValue = 1000;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = 'O';
                int moveValue = minimax(0, 1);
                board[i][j] = ' ';
                if (moveValue < bestValue) {
                    bestMoveX = i;
                    bestMoveY = j;
                    bestValue = moveValue;
                }
            }
        }
    }
    return bestMoveX * N + bestMoveY;
}

int main() {
    system("color 1A");
    int player = 1, i, choice;
    char mark;
    int mode, row, col;

    printf("Enter board size (N x N): ");
    scanf("%d", &N);
    if (N < 3 || N > MAX_N) {
        printf("Invalid size! Choose between 3 and %d.\n", MAX_N);
        return 1;
    }

    initializeBoard();
    
    printf("Choose Mode: 1. Player vs Player  2. Player vs Computer\n");
    scanf("%d", &mode);

    do {
        drawBoard();
        if (mode == 1 || player == 1) {
            printf("Player %d, enter row and column (1-%d): ", player, N);
            fflush(stdin);
            scanf("%d %d", &row, &col);
            row--; col--;
            mark = (player == 1) ? 'X' : 'O';
        } else {
            int move = findBestMove();
            row = move / N;
            col = move % N;
            mark = 'O';
        }
        if (row >= 0 && row < N && col >= 0 && col < N && board[row][col] == ' ') {
            board[row][col] = mark;
            playSound(900);
            i = checkWin();
            if (i == -1) player = 3 - player;
        } else if (mode == 1 || player == 1) {
            printf("Invalid move! Try again.\n");
            getchar();
        }
    } while (i == -1);

    drawBoard();
    if (i == 10) printf("\033[1;32m==> Player 1 (X) won!\033[0m\n");
    else if (i == -10) printf("\033[1;31m==> Player 2 (O) won!\033[0m\n");
    else printf("\033[1;33m==> Game draw!\033[0m\n");
    
    playSound(1000000000);
    printf("\nPress Enter to exit...");
    getchar();
    getchar();
    return 0;
}

