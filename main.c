#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define Genislik_BOYUTU 1100
#define Yükseklik_BOYUTU 700
#define KARE_BOYUTU 200
#define MAX_ISIM_UZUNLUK 30
#define MAX_OYUNCU_KAYDI 50

typedef enum { EKRAN_ISIM_GIRISI, EKRAN_OYUN } OyunDurumu;
typedef struct {
    char isim[MAX_ISIM_UZUNLUK];
    int skor;
} OyuncuSkor;

int moveUp(int board[3][3]){
    int tempSatir = -1, tempSutun = -1, tempC;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (board[i][j] == 0){
                tempSatir = i;
                tempSutun = j;
            }
        }
    }
    if (tempSatir > 0){
        tempC = board[tempSatir-1][tempSutun];
        board[tempSatir-1][tempSutun] = 0;
        board[tempSatir][tempSutun] = tempC;
        return 1;
    }
    return 0;
}
int moveDown(int board[3][3]){
    int tempSatir = -1, tempSutun = -1, tempC;
    for (int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if (board[i][j] == 0){
                tempSatir = i;
                tempSutun = j;
            }
        }
    }
    if(tempSatir < 2){
        tempC = board[tempSatir+1][tempSutun];
        board[tempSatir+1][tempSutun] = 0;
        board[tempSatir][tempSutun] = tempC;
        return 1;
    }
    return 0;
}
int moveRight(int board[3][3]){
    int tempSatir = -1, tempSutun = -1, tempC;
    for (int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(board[i][j] == 0){
                tempSatir=i;
                tempSutun=j;
            }
        }
    }
    if(tempSutun < 2){
        tempC = board[tempSatir][tempSutun+1];
        board[tempSatir][tempSutun+1] = 0;
        board[tempSatir][tempSutun] = tempC;
        return 1;
    }
    return 0;
}
int moveLeft(int board[3][3]){
    int tempSatir = -1, tempSutun = -1, tempC;
    for (int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(board[i][j] == 0){
                tempSatir=i;
                tempSutun=j;
            }
        }
    }
    if (tempSutun > 0){
        tempC = board[tempSatir][tempSutun-1];
        board[tempSatir][tempSutun-1] = 0;
        board[tempSatir][tempSutun] = tempC;
        return 1;
    }
    return 0;
}

void tahtayiKar(int board[3][3],int hamleSayisi)
{
    int i;
    for(i=0;i<hamleSayisi;i++)
    {
        int rastgeleHamle = GetRandomValue(0,3);
        if(rastgeleHamle == 0){
            moveUp(board);}
          else if(rastgeleHamle == 1){
            moveDown(board);}
          else if (rastgeleHamle == 2){
            moveLeft(board);}
          else if (rastgeleHamle == 3){
            moveRight(board);}

    }
}
void drawBoard(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int value = board[i][j];

 
            float posX = j * KARE_BOYUTU;//kareX
            float posY = i * KARE_BOYUTU;//kareY

            Rectangle tileRec = { posX + 10, posY + 10, KARE_BOYUTU - 20, KARE_BOYUTU - 20 };

            if (value != 0) {

                DrawRectangleRounded(tileRec, 0.15f, 16, (Color){ 41, 128, 185, 255 });//kareler

                // Sayı çizimi
                const char* text = TextFormat("%d", value);
                int fontSize = 60;
                int textWidth = MeasureText(text, fontSize);

                int textX = posX + (KARE_BOYUTU / 2) - (textWidth / 2);
                int textY = posY + (KARE_BOYUTU / 2) - (fontSize / 2);

                DrawText(text, textX, textY, fontSize, RAYWHITE);
            } else {
                DrawRectangleRounded(tileRec, 0.15f, 16, (Color){ 220, 224, 230, 255 });//arkaplan
            }
        }
    }
}
int oyunuKazandiMi(int board[3][3]) {
    return (board[0][0] == 1 && board[0][1] == 2 && board[0][2] == 3 &&
            board[1][0] == 4 && board[1][1] == 5 && board[1][2] == 6 &&
            board[2][0] == 7 && board[2][1] == 8 && board[2][2] == 0);
}
void skoruKaydet(const char* isim, int yeniSkor) {
    FILE *dosya = fopen("skorlar.txt", "r");
    OyuncuSkor skorlar[MAX_OYUNCU_KAYDI];
    int kayitSayisi = 0;
    int oyuncuBulundu = 0;

    if (dosya != NULL) {
        while (fscanf(dosya, "%s %d", skorlar[kayitSayisi].isim, &skorlar[kayitSayisi].skor) != EOF) {
            if (strcmp(skorlar[kayitSayisi].isim, isim) == 0) {
                oyuncuBulundu = 1;
                if (yeniSkor < skorlar[kayitSayisi].skor) {
                    skorlar[kayitSayisi].skor = yeniSkor;
                }
            }
            kayitSayisi++;
            if(kayitSayisi >= MAX_OYUNCU_KAYDI) break;
        }
        fclose(dosya);
    }
    if (!oyuncuBulundu && kayitSayisi < MAX_OYUNCU_KAYDI) {
        strcpy(skorlar[kayitSayisi].isim, isim);
        skorlar[kayitSayisi].skor = yeniSkor;
        kayitSayisi++;
    }
    dosya = fopen("skorlar.txt", "w");
    if (dosya != NULL) {
        for (int i = 0; i < kayitSayisi; i++) {
            fprintf(dosya, "%s %d\n", skorlar[i].isim, skorlar[i].skor);
        }
        fclose(dosya);
    }
}
int main() {
    int sayac=0;
    int oyunBitti = 0;
    InitWindow(Genislik_BOYUTU,Yükseklik_BOYUTU, "Puzzle Oyunu");
    SetTargetFPS(60); 

    char oyuncuIsmi[MAX_ISIM_UZUNLUK] = "\0";
    int harfSayisi = 0;
    OyunDurumu mevcutDurum = EKRAN_ISIM_GIRISI;

    int board[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    tahtayiKar(board,200);
    while (!WindowShouldClose()) {
        if (mevcutDurum == EKRAN_ISIM_GIRISI) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (harfSayisi < MAX_ISIM_UZUNLUK)) {
                    oyuncuIsmi[harfSayisi] = (char)key;
                    oyuncuIsmi[harfSayisi + 1] = '\0';
                    harfSayisi++;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (harfSayisi > 0) {
                    harfSayisi--;
                    oyuncuIsmi[harfSayisi] = '\0';
                }
            }
            if (IsKeyPressed(KEY_ENTER) && harfSayisi > 0) {
                mevcutDurum = EKRAN_OYUN;
            }

        } else if (mevcutDurum == EKRAN_OYUN) {
            if (IsKeyPressed(KEY_TAB)) {
                mevcutDurum = EKRAN_ISIM_GIRISI;}
        if(!oyunBitti){
            if (IsKeyPressed(KEY_W)) {
                if(moveUp(board)==1)
                    sayac++;
            } else if (IsKeyPressed(KEY_S)) {
                if(moveDown(board)==1)
                    sayac++;
            } else if (IsKeyPressed(KEY_A)) {
                if(moveLeft(board))
                    sayac++;
            } else if (IsKeyPressed(KEY_D)) {
                if(moveRight(board))
                    sayac++;
            }
            if (oyunuKazandiMi(board)) {
                oyunBitti = 1; // Bayrağı kaldır ki sürekli dosyaya yazmasın
                skoruKaydet(oyuncuIsmi, sayac);
            }
        }
    }
    if (IsKeyPressed(KEY_SPACE) && mevcutDurum == EKRAN_OYUN) {
            tahtayiKar(board, 100);
            sayac = 0;
            oyunBitti = 0;
    }

        BeginDrawing();
        ClearBackground((Color){ 236, 240, 241, 255 });
        if (mevcutDurum == EKRAN_ISIM_GIRISI) {
            // İsim Giriş Ekranı
            DrawText("HOS GELDINIZ!", 350, 200, 40, DARKGRAY);
            DrawText("Lütfen isminizi yazin ve ENTER'a basin:", 280, 280, 20, GRAY);
            DrawRectangle(300, 330, 400, 50, WHITE);
            DrawRectangleLines(300, 330, 400, 50, BLUE);
            DrawText(oyuncuIsmi, 320, 345, 25, MAROON);

            if (((int)(GetTime() * 2) % 2) == 0) {
                DrawText("_", 325 + MeasureText(oyuncuIsmi, 25), 345, 25, MAROON);
            }

        } else if (mevcutDurum == EKRAN_OYUN) {
        drawBoard(board);
        DrawText(TextFormat("Oyuncu: %s", oyuncuIsmi), 650, 80, 25, DARKBLUE);
        DrawText(TextFormat("Hamle Sayisi: %d",sayac), 20, 650, 25, BLACK);
        DrawText("Tekrar Oyna(BAS SPACE)",650,20,25,BLACK);
        DrawText("ISIM EKRANINA DON(BAS TAB)",650,50,25,BLACK);

        if(oyunBitti) {
            DrawText("KAZANDINIZ", 300, 620, 50, GREEN);
            DrawText("Skor Kaydedildi!", 350, 675, 20, DARKGREEN); // Kısa bir bilgi mesajı eklendi
        }
    }
        EndDrawing();
    }
    CloseWindow();

    return 0;
}