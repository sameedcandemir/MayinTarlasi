//PROJE SAH�B� --> SAMED CANDEM�R 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 100       // Maksimum tahta boyutu
#define MINE_PERCENTAGE 5   // May�n y�zdesi
#define SAFE_PATH '.'        // G�venli yol sembol�
#define EXPLORED_CELL '*'    // Ge�ilen h�cre sembol�
#define PLAYER '@'          // Oyuncu karakteri
#define MINE 'o'            // May�n karakteri
#define MINE_AREA 'x'       // May�n etki alan� sembol�
#define START 'G'           // Ba�lang�� noktas� karakteri
#define EXIT 'C'            // ��k�� noktas� karakteri
#define WARNING '!'         // Uyar� sembol�
#define RESTART 'R'         // Yeniden ba�latma karakteri

char board[MAX_SIZE][MAX_SIZE];
char exploredCells[MAX_SIZE][MAX_SIZE];
int revealMines = 0; 

int n;         
int playerX, playerY;

void initializeBoard() {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == 0 && j == 0) {
                board[i][j] = START;
            } else if (i == n - 1 && j == n - 1) {
                board[i][j] = EXIT;
            } else {
                board[i][j] = SAFE_PATH;
            }
            exploredCells[i][j] = 0;
        }
    }
}

void placeMines() {
    int numMines = (n * n * MINE_PERCENTAGE) / 100;
    int i;
    for (i = 0; i < numMines; i++) {
        int row, col;
        do {
            row = rand() % n;
            col = rand() % n;
        } while (board[row][col] == MINE || (row == 0 && col == 0) || (row == n - 1 && col == n - 1));
        board[row][col] = MINE;
        

        int areaRow, areaCol;

        // �st Kolon
        areaRow = row - 1;
        areaCol = col;
        if (areaRow >= 0 && board[areaRow][areaCol] != MINE) {
            board[areaRow][areaCol] = MINE_AREA;
        }
		
        // Alt Kolon
        areaRow = row + 1;
        areaCol = col;
        if (areaRow < n && board[areaRow][areaCol] != MINE) {
            board[areaRow][areaCol] = MINE_AREA;
        }

        // Sol Kolon
        areaRow = row;
        areaCol = col - 1;
        if (areaCol >= 0 && board[areaRow][areaCol] != MINE) {
            board[areaRow][areaCol] = MINE_AREA;
        }

        // Sa� Kolon
        areaRow = row;
        areaCol = col + 1;
        if (areaCol < n && board[areaRow][areaCol] != MINE) {
            board[areaRow][areaCol] = MINE_AREA;
        }
    }
}
//Oyun alan�n� ekrana bast�rma. May�n ve G�venli b�lge i�lemleri

void printBoard() {
    printf("\n");
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == playerX && j == playerY) {
                printf("%c ", PLAYER);
            } else if (exploredCells[i][j]) {
                printf("%c ", EXPLORED_CELL);
            } else if (revealMines && (board[i][j] == MINE || board[i][j] == MINE_AREA)) {
                printf("%c ", board[i][j]); // May�nlar� g�ster
            } else {
                // Ba�lang�� (G) noktas�n� ve ��k�� (C) noktas�n� ekle
                if (board[i][j] == START || board[i][j] == EXIT) {
                    printf("%c ", board[i][j]);
                } else {
                    printf("%c ", SAFE_PATH);//Oyun sonland�ktan sonra kullan�c�ya g�venli b�lgeleri g�stermek i�in
                    //oyun sonunda kullan�c� g�venli b�lgeleri y�ld�z (*) ile g�recek
                }
            }
        }
        printf("\n");
    }
}

void movePlayer(char direction) {
    int newX = playerX, newY = playerY;

    switch (direction) {
        case 'w':
            newX = playerX - 1;
            break;
        case 's':
            newX = playerX + 1;
            break;
        case 'a':
            newY = playerY - 1;
            break;
        case 'd':
            newY = playerY + 1;
            break;
            
        case RESTART:
            initializeBoard();
            placeMines();
            playerX = playerY = 0;
            printf("\nOyun tahtasi yeniden konumlandirildi!\n");
            return;
        default:
            printf("Gecersiz hareket. Tekrar deneyin.\n");
            return;
    }

    if (newX >= 0 && newX < n && newY >= 0 && newY < n) {
        if (board[newX][newY] == MINE) {
            printf("\nMayina bastiniz! Oyuncu yanar. Oyun sona erdi.\n");
            exit(0);
        } else if (board[newX][newY] == MINE_AREA) {
            
            printf("\nMayinin etki alanina girdiniz! Uyari: %c G noktasina donuluyor.\n", WARNING); 
            
            playerX = playerY = 0;
        } else {
            playerX = newX;
            playerY = newY;
        }
        exploredCells[playerX][playerY] = 1;
    } else {
        printf("\nTahta sinirlarini asamazsiniz! Uyari: %c G noktasina donuluyor.\n", WARNING);
        playerX = playerY = 0;
    }
}
 
int main() {
	system("COLOR a");
    srand(time(NULL));
	 
    printf("Tahta boyutunu girin (1-%d): ", MAX_SIZE);
    scanf("%d", &n);

    if (n < 1 || n > MAX_SIZE) {
        printf("Gecersiz tahta boyutu. Program sonlandiriliyor.\n");
        return 1;
    }
    

    initializeBoard();
    placeMines();

    playerX = playerY = 0;

    clock_t start_time = clock();

    while (1) {
        printBoard();

        printf("Yon tuslariyla hareket edin. (w: yukari, s: asagi, a: sola, d: saga, \n R: Yeniden baslat)\n");//Y�n tu�lar� 
        char move;
        scanf(" %c", &move);

        movePlayer(move);

        if (playerX == n - 1 && playerY == n - 1) {
            clock_t end_time = clock(); 
            double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;//Oyun s�resi i�in saya�

            printf("\nTebrikler! Cikisa ulastiniz.\n");
            printf("Mayin sayisi: %d\n", (n * n * MINE_PERCENTAGE) / 100);
            printf("Gecen sure: %.2f saniye\n", elapsed_time);

        
            revealMines = 1;
            printBoard();
            
            break;
        }
    }

    return 0;
}
