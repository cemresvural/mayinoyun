#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int MAX_SIZE = 100;

char** board;
char** displayBoard;


typedef struct {
    int x, y;
} Point;

Point playerPosition, exitPosition;

const char MINES_HIDDEN = '*';

// Minimum ve maksimum aras�nda rastgele bir say� �retme fonksiyonu
int getRandom(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Tahtaya may�n yerle�tirme fonksiyonu , %5 oran�nda 
void placeMines(int size) {
    int minesToPlace = size * size * 0.05;
    int i;
    for (i = 0; i < minesToPlace; i++) {
        int row, col;
        do {
            row = getRandom(0, size - 1);
            col = getRandom(0, size - 1);
        } while (board[row][col] != ' ' || (row == playerPosition.x && col == playerPosition.y) || (row == exitPosition.x && col == exitPosition.y));

        board[row][col] = 'o';

        // May�n�n etraf�ndaki g�venli olmayan alanlar� i�aretleme
        if (row > 0) board[row - 1][col] = 'x';
        if (row < size - 1) board[row + 1][col] = 'x';
        if (col > 0) board[row][col - 1] = 'x';
        if (col < size - 1) board[row][col + 1] = 'x';
    }
}

// Giri� i�in 'G'  ve ��k�� i�in 'C' yerle�tirme
void initializeDisplayBoardWithSymbols(int size) {  //sembol� ekran kart� ile ba�lat 
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (i == playerPosition.x && j == playerPosition.y) {
                displayBoard[i][j] = 'G';  // Oyuncunun ba�lang��  pozisyonu
            } else if (i == 0 && j == 0) {
                displayBoard[i][j] = 'G';  // Ba�lang�� sembol�
            } else if (i == exitPosition.x && j == exitPosition.y) {
                displayBoard[i][j] = 'C';  // ��k�� sembol�
            } else {
                displayBoard[i][j] = ' ';
            }
        }
    }
}

// Ekran oyun tahtas�n� ba�latma i�levi
void initializeDisplayBoard(int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            displayBoard[i][j] = ' ';
        }
    }
}

// Oyuncu tahtas�n� ekrana yazd�rma 
void printBoard(int size) {
    system("cls");  
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            printf("%c ", displayBoard[i][j]);
        }
        printf("\n");
    }
}

// Bir pozisyonun matris  s�n�rlar� i�inde olup olmad���n� kontrol etme i�levi
int isValidMove(int x, int y, int size) {
    return x >= 0 && x < size && y >= 0 && y < size;
}

// Oyuncuyu  hareket ettirme i�levi
void movePlayer(int deltaX, int deltaY, int size) {
    int newX = playerPosition.x + deltaX;
    int newY = playerPosition.y + deltaY;
    char playAgain;
    
    if (isValidMove(newX, newY, size)) {
        if (board[newX][newY] == 'o') {
            printf("Mayina bastin oyun bitti.\n");
            exit(0);
        } else {
            displayBoard[playerPosition.x][playerPosition.y] = '.';  // gitti�imiz yolu '.' i�aretiyle i�aretleme 
            playerPosition.x = newX;
            playerPosition.y = newY;
            displayBoard[playerPosition.x][playerPosition.y] = 'A';  //yeni pozisyonu i�aretleme 
            printBoard(size);
        }
    }
}

//may�na basmadan gidilen yolu i�aretleme i�levi
void drawSafePath(int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (displayBoard[i][j] == 'A') {
                printf("A ");
            } else if (displayBoard[i][j] == 'C') {
                printf("C ");
            } else if (displayBoard[i][j] == 'G') {
                printf("G ");
            } else if (displayBoard[i][j] == '.') {
                printf(". ");
            } else {
                printf("  ");  // Di�er karakterler i�in bo� alan yazd�r b�ylece sadece g�venli yolu g�r�cez
            }
        }
        printf("\n");
    }
}

// Konsol ekran�n� temizleme i�levi
void clearScreen() {
    system("cls");
    
}
// Kullan�c� giri�i alma i�levi
char getUserInput() {
    char input;
    scanf(" %c", &input);
    return input;
}

// Oyun tahtas�na haf�za ay�rma fonksiyonu
void allocateMemory(int size) {
    board = (char**)malloc(size * sizeof(char*));
    displayBoard = (char**)malloc(size * sizeof(char*));
    int i;
    for (i = 0; i < size; i++) {
        board[i] = (char*)malloc(size * sizeof(char));
        displayBoard[i] = (char*)malloc(size * sizeof(char));
    }
}
/*
Neden malloc fonksiyonu kulland�m 
Oyun tahtas�n�n boyutu derleme zaman�nda bilinmemektedir; kullan�c�n�n giri�ine ba�l�d�r. 
Dinamik bellek ay�rma, program�n �al��ma s�resi boyunca kullan�c� taraf�ndan belirlenen farkl� kart boyutlar�na uyum sa�lamas�na olanak tan�r.
Dinamik ay�rma verimli bellek kullan�m� sa�lar.

her sat�r�n boyutu size*
*/

// Tahsis edilen belle�i serbest b�rakma( bo�altma )i�levi
void freeMemory(int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(board[i]);
        free(displayBoard[i]);
    }
    free(board);
    free(displayBoard);
}

int main() {
    srand(time(NULL));

    char playAgain;

    while (1) {  
        int size;
        do {
        	printf("MAYIN TARLASINA HOSGELDINIZ\n");
            printf("Matris boyutunu giriniz:(En fazla girebileceginiz boyut %d'dur.): ", MAX_SIZE);
            scanf("%d", &size);
        } while (size <= 0 || size > MAX_SIZE);

        // Board i�in bellek ay�rma
        allocateMemory(size);

        //  board'� ba�latma (girilen bir boyut var bu boyut �zerine ba�lat�l�yor
		int i, j;
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                board[i][j] = ' ';
            }
        }

        // Oyuncuyu ayarlama  ve pozisyonlardan ��kma 
        playerPosition.x = 0;
        playerPosition.y = 0;
        exitPosition.x = size - 1;
        exitPosition.y = size - 1;

        // May�nlar� tahtaya yerle�tirme
        placeMines(size);

        // Giri� ve ��k�� noktalar�n�n may�n olmad���ndan emin olma i�levi 
        board[playerPosition.x][playerPosition.y] = ' ';
        board[exitPosition.x][exitPosition.y] = ' ';

        // Board� yani oyun tahtas�n� sembollerle ba�latma giri� i�in G ,��k�� i�in C vermi�tik.
        initializeDisplayBoardWithSymbols(size);

        // Oyun d�ng�s�
        int gameResult = 0;  //0: Devam, 1: Oyuncu kazand�, 2: Oyuncu may�na bast�
        while(1) {
            printBoard(size);
            printf("Oyuncu pozisyonu: (%d, %d)\n", playerPosition.x, playerPosition.y);
            printf("Hareket etmek icin (w:yukari, a:sola, s:asagiya, d:saga) tuslarini kullanin.Cikmak icin 'q' tusuna basin: \n");

            char move = getUserInput();

            switch (move) {
                case 'q':
                    printf("Oynadiginiz icin tesekkurler.C�k�s yapiliyor..\n");
                    freeMemory(size);
                    exit(0);
                case 'w':
                    movePlayer(-1, 0, size);  // yukar� ta��
                    break;
                case 's':
                    movePlayer(1, 0, size);  // a�a�� ta�� 
                    break;
                case 'a':
                    movePlayer(0, -1, size);  // sola ta�� 
                    break;
                case 'd':
                    movePlayer(0, 1, size);  // sa�a ta�� 
                    break;
                default:
                    printf("Gecersiz hamle! Tasimak icin 'w', 's', 'a', 'd', cikmak icin 'q' tuslarini kullanin.\n");
            }

            // Oyuncunun ��k��a ula��p ula�mad���n� veya may�na bas�p basmad���n� kontrol etme i�lemi
			
			
            if (playerPosition.x == exitPosition.x && playerPosition.y == exitPosition.y) {
                printf("Tebrikler! Cikisa ulastiniz!\n");

                drawSafePath(size);
                gameResult = 1;  // oyuncu kazand� 
                break;
            } else if (board[playerPosition.x][playerPosition.y]== 'o') {
                printf("Mayina bastin! Tekrar oynamak ister misin? (y/n):");
                
                drawSafePath(size);
                gameResult = 2; 
				scanf(" %c", &playAgain); // oyuncu kaybetti
                break;
            }
        }

// Tekrar oynamak i�in sor

printf("Tekrar oynamak ister misin? (y/n): ");
scanf(" %c", &playAgain);
if (playAgain != 'y' && playAgain != 'n') {
  break;  // 'y' veya 'n' de�ilse d�� while d�ng�s�nden ��k�n
  }
}

    freeMemory(MAX_SIZE);

    return 0;
}
