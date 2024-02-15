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

// Minimum ve maksimum arasýnda rastgele bir sayý üretme fonksiyonu
int getRandom(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Tahtaya mayýn yerleþtirme fonksiyonu , %5 oranýnda 
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

        // Mayýnýn etrafýndaki güvenli olmayan alanlarý iþaretleme
        if (row > 0) board[row - 1][col] = 'x';
        if (row < size - 1) board[row + 1][col] = 'x';
        if (col > 0) board[row][col - 1] = 'x';
        if (col < size - 1) board[row][col + 1] = 'x';
    }
}

// Giriþ için 'G'  ve çýkýþ için 'C' yerleþtirme
void initializeDisplayBoardWithSymbols(int size) {  //sembolü ekran kartý ile baþlat 
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (i == playerPosition.x && j == playerPosition.y) {
                displayBoard[i][j] = 'G';  // Oyuncunun baþlangýç  pozisyonu
            } else if (i == 0 && j == 0) {
                displayBoard[i][j] = 'G';  // Baþlangýç sembolü
            } else if (i == exitPosition.x && j == exitPosition.y) {
                displayBoard[i][j] = 'C';  // Çýkýþ sembolü
            } else {
                displayBoard[i][j] = ' ';
            }
        }
    }
}

// Ekran oyun tahtasýný baþlatma iþlevi
void initializeDisplayBoard(int size) {
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            displayBoard[i][j] = ' ';
        }
    }
}

// Oyuncu tahtasýný ekrana yazdýrma 
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

// Bir pozisyonun matris  sýnýrlarý içinde olup olmadýðýný kontrol etme iþlevi
int isValidMove(int x, int y, int size) {
    return x >= 0 && x < size && y >= 0 && y < size;
}

// Oyuncuyu  hareket ettirme iþlevi
void movePlayer(int deltaX, int deltaY, int size) {
    int newX = playerPosition.x + deltaX;
    int newY = playerPosition.y + deltaY;
    char playAgain;
    
    if (isValidMove(newX, newY, size)) {
        if (board[newX][newY] == 'o') {
            printf("Mayina bastin oyun bitti.\n");
            exit(0);
        } else {
            displayBoard[playerPosition.x][playerPosition.y] = '.';  // gittiðimiz yolu '.' iþaretiyle iþaretleme 
            playerPosition.x = newX;
            playerPosition.y = newY;
            displayBoard[playerPosition.x][playerPosition.y] = 'A';  //yeni pozisyonu iþaretleme 
            printBoard(size);
        }
    }
}

//mayýna basmadan gidilen yolu iþaretleme iþlevi
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
                printf("  ");  // Diðer karakterler için boþ alan yazdýr böylece sadece güvenli yolu görücez
            }
        }
        printf("\n");
    }
}

// Konsol ekranýný temizleme iþlevi
void clearScreen() {
    system("cls");
    
}
// Kullanýcý giriþi alma iþlevi
char getUserInput() {
    char input;
    scanf(" %c", &input);
    return input;
}

// Oyun tahtasýna hafýza ayýrma fonksiyonu
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
Neden malloc fonksiyonu kullandým 
Oyun tahtasýnýn boyutu derleme zamanýnda bilinmemektedir; kullanýcýnýn giriþine baðlýdýr. 
Dinamik bellek ayýrma, programýn çalýþma süresi boyunca kullanýcý tarafýndan belirlenen farklý kart boyutlarýna uyum saðlamasýna olanak tanýr.
Dinamik ayýrma verimli bellek kullanýmý saðlar.

her satýrýn boyutu size*
*/

// Tahsis edilen belleði serbest býrakma( boþaltma )iþlevi
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

        // Board için bellek ayýrma
        allocateMemory(size);

        //  board'ý baþlatma (girilen bir boyut var bu boyut üzerine baþlatýlýyor
		int i, j;
        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                board[i][j] = ' ';
            }
        }

        // Oyuncuyu ayarlama  ve pozisyonlardan çýkma 
        playerPosition.x = 0;
        playerPosition.y = 0;
        exitPosition.x = size - 1;
        exitPosition.y = size - 1;

        // Mayýnlarý tahtaya yerleþtirme
        placeMines(size);

        // Giriþ ve çýkýþ noktalarýnýn mayýn olmadýðýndan emin olma iþlevi 
        board[playerPosition.x][playerPosition.y] = ' ';
        board[exitPosition.x][exitPosition.y] = ' ';

        // Boardý yani oyun tahtasýný sembollerle baþlatma giriþ için G ,çýkýþ için C vermiþtik.
        initializeDisplayBoardWithSymbols(size);

        // Oyun döngüsü
        int gameResult = 0;  //0: Devam, 1: Oyuncu kazandý, 2: Oyuncu mayýna bastý
        while(1) {
            printBoard(size);
            printf("Oyuncu pozisyonu: (%d, %d)\n", playerPosition.x, playerPosition.y);
            printf("Hareket etmek icin (w:yukari, a:sola, s:asagiya, d:saga) tuslarini kullanin.Cikmak icin 'q' tusuna basin: \n");

            char move = getUserInput();

            switch (move) {
                case 'q':
                    printf("Oynadiginiz icin tesekkurler.Cýkýs yapiliyor..\n");
                    freeMemory(size);
                    exit(0);
                case 'w':
                    movePlayer(-1, 0, size);  // yukarý taþý
                    break;
                case 's':
                    movePlayer(1, 0, size);  // aþaðý taþý 
                    break;
                case 'a':
                    movePlayer(0, -1, size);  // sola taþý 
                    break;
                case 'd':
                    movePlayer(0, 1, size);  // saða taþý 
                    break;
                default:
                    printf("Gecersiz hamle! Tasimak icin 'w', 's', 'a', 'd', cikmak icin 'q' tuslarini kullanin.\n");
            }

            // Oyuncunun çýkýþa ulaþýp ulaþmadýðýný veya mayýna basýp basmadýðýný kontrol etme iþlemi
			
			
            if (playerPosition.x == exitPosition.x && playerPosition.y == exitPosition.y) {
                printf("Tebrikler! Cikisa ulastiniz!\n");

                drawSafePath(size);
                gameResult = 1;  // oyuncu kazandý 
                break;
            } else if (board[playerPosition.x][playerPosition.y]== 'o') {
                printf("Mayina bastin! Tekrar oynamak ister misin? (y/n):");
                
                drawSafePath(size);
                gameResult = 2; 
				scanf(" %c", &playAgain); // oyuncu kaybetti
                break;
            }
        }

// Tekrar oynamak için sor

printf("Tekrar oynamak ister misin? (y/n): ");
scanf(" %c", &playAgain);
if (playAgain != 'y' && playAgain != 'n') {
  break;  // 'y' veya 'n' deðilse dýþ while döngüsünden çýkýn
  }
}

    freeMemory(MAX_SIZE);

    return 0;
}
