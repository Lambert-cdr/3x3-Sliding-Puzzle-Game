#include <raylib.h>
#include <stdio.h>
#include <string.h>

#define Genislik_BOYUTU 1150
#define Yükseklik_BOYUTU 700
#define KARE_BOYUTU 170
#define MAX_ISIM_UZUNLUK 30
#define MAX_OYUNCU_KAYDI 50
#define RESIM_SAYISI 3

Font oyunFontu;
Texture2D puzzleResimleri[RESIM_SAYISI];
const char* resimIsimleri[] = { "hacker", "surprise", "kedi" }; // Ekranda görünecek isimler
const char* dosyaYollari[] = { "manzara.png", "kedi.png", "uzay.png" };

typedef enum { EKRAN_ISIM_GIRISI, EKRAN_OYUN } OyunDurumu;
typedef enum { TEMA_SAYILAR, TEMA_RESIM } OyunTemasi;

typedef struct {
    char isim[MAX_ISIM_UZUNLUK];
    int skor;
} OyuncuSkor;

OyunTemasi seciliTema = TEMA_SAYILAR;
int seciliResimIndex = 0;

// --- Hareket Fonksiyonları ---
int moveUp(int board[3][3]){
    int tr = -1, tc = -1;
    for (int i=0; i<3; i++) for (int j=0; j<3; j++) if (board[i][j] == 0) { tr=i; tc=j; }
    if (tr > 0){ board[tr][tc] = board[tr-1][tc]; board[tr-1][tc] = 0; return 1; }
    return 0;
}
int moveDown(int board[3][3]){
    int tr = -1, tc = -1;
    for (int i=0; i<3; i++) for (int j=0; j<3; j++) if (board[i][j] == 0) { tr=i; tc=j; }
    if (tr < 2){ board[tr][tc] = board[tr+1][tc]; board[tr+1][tc] = 0; return 1; }
    return 0;
}
int moveRight(int board[3][3]){
    int tr = -1, tc = -1;
    for (int i=0; i<3; i++) for (int j=0; j<3; j++) if (board[i][j] == 0) { tr=i; tc=j; }
    if (tc < 2){ board[tr][tc] = board[tr][tc+1]; board[tr][tc+1] = 0; return 1; }
    return 0;
}
int moveLeft(int board[3][3]){
    int tr = -1, tc = -1;
    for (int i=0; i<3; i++) for (int j=0; j<3; j++) if (board[i][j] == 0) { tr=i; tc=j; }
    if (tc > 0){ board[tr][tc] = board[tr][tc-1]; board[tr][tc-1] = 0; return 1; }
    return 0;
}

void tahtayiKar(int board[3][3], int hamle) {
    for(int i=0; i<hamle; i++) {
        int r = GetRandomValue(0,3);
        if(r==0) moveUp(board); else if(r==1) moveDown(board);
        else if(r==2) moveLeft(board); else moveRight(board);
    }
}

void drawBoard(int board[3][3], OyunTemasi tema, Texture2D aktifResim) {
    float ox = 50, oy = 50;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            int val = board[i][j];
            float px = j * KARE_BOYUTU + ox, py = i * KARE_BOYUTU + oy;
            Rectangle tileRec = { px + 5, py + 5, KARE_BOYUTU - 10, KARE_BOYUTU - 10 };
            if (val != 0) {
                if (tema == TEMA_RESIM) {
                    int r = (val-1)/3, c = (val-1)%3;
                    Rectangle src = { c*170.0f, r*170.0f, 170.0f, 170.0f };
                    DrawTextureRec(aktifResim, src, (Vector2){px, py}, WHITE);
                    DrawRectangleLinesEx(tileRec, 1, DARKGRAY);
                } else {
                    DrawRectangleRounded(tileRec, 0.15f, 32, BLUE);
                    Vector2 tSize = MeasureTextEx(oyunFontu, TextFormat("%d", val), 60, 2);
                    DrawTextEx(oyunFontu, TextFormat("%d", val), (Vector2){px+(170-tSize.x)/2, py+(170-tSize.y)/2}, 60, 2, RAYWHITE);
                }
            } else DrawRectangleRounded(tileRec, 0.15f, 32, LIGHTGRAY);
        }
    }
}

// --- Skor Sistemleri ---
int oyunuKazandiMi(int b[3][3]) {
    int win[3][3] = {{1,2,3},{4,5,6},{7,8,0}};
    for(int i=0; i<3; i++) for(int j=0; j<3; j++) if(b[i][j] != win[i][j]) return 0;
    return 1;
}

void skoruKaydet(const char* isim, int skor) {
    OyuncuSkor s[MAX_OYUNCU_KAYDI]; int n = 0, found = 0;
    FILE *f = fopen("skorlar.txt", "r");
    if(f) {
        while(fscanf(f, "%s %d", s[n].isim, &s[n].skor) != EOF && n < MAX_OYUNCU_KAYDI) {
            if(strcmp(s[n].isim, isim) == 0) { found = 1; if(skor < s[n].skor) s[n].skor = skor; }
            n++;
        }
        fclose(f);
    }
    if(!found && n < MAX_OYUNCU_KAYDI) { strcpy(s[n].isim, isim); s[n].skor = skor; n++; }
    f = fopen("skorlar.txt", "w");
    if(f) { for(int i=0; i<n; i++) fprintf(f, "%s %d\n", s[i].isim, s[i].skor); fclose(f); }
}

int top5Al(OyuncuSkor t5[]) {
    OyuncuSkor all[MAX_OYUNCU_KAYDI]; int n = 0;
    FILE *f = fopen("skorlar.txt", "r");
    if(!f) return 0;
    while(fscanf(f, "%s %d", all[n].isim, &all[n].skor) != EOF && n < MAX_OYUNCU_KAYDI) n++;
    fclose(f);
    for(int i=0; i<n-1; i++) for(int j=0; j<n-i-1; j++)
        if(all[j].skor > all[j+1].skor) { OyuncuSkor tmp = all[j]; all[j] = all[j+1]; all[j+1] = tmp; }
    int count = (n < 5) ? n : 5;
    for(int i=0; i<count; i++) t5[i] = all[i];
    return count;
}

int main() {
    InitWindow(Genislik_BOYUTU, Yükseklik_BOYUTU, "Puzzle Pro - Coklu Resim");
    SetTargetFPS(60);

    oyunFontu = LoadFontEx("arial.ttf", 128, NULL, 0);
    SetTextureFilter(oyunFontu.texture, TEXTURE_FILTER_BILINEAR);

    for (int i=0; i<RESIM_SAYISI; i++) {
        Image img = LoadImage(dosyaYollari[i]);
        if (img.data) { ImageResize(&img, 510, 510); puzzleResimleri[i] = LoadTextureFromImage(img); UnloadImage(img); }
    }

    int sayac = 0, oyunBitti = 0, harfSayisi = 0;
    char oyuncuIsmi[MAX_ISIM_UZUNLUK] = "\0";
    OyunDurumu durum = EKRAN_ISIM_GIRISI;
    OyuncuSkor t5[5]; int t5n = top5Al(t5);
    int board[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    tahtayiKar(board, 200);

    while (!WindowShouldClose()) {
        float spc = 2.0f;
        if (durum == EKRAN_ISIM_GIRISI) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 125 && harfSayisi < MAX_ISIM_UZUNLUK-1) {
                    oyuncuIsmi[harfSayisi++] = (char)key; oyuncuIsmi[harfSayisi] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && harfSayisi > 0) oyuncuIsmi[--harfSayisi] = '\0';
            if (IsKeyPressed(KEY_LEFT_ALT)) seciliTema = (seciliTema == TEMA_SAYILAR) ? TEMA_RESIM : TEMA_SAYILAR;
            if (seciliTema == TEMA_RESIM) {
                if (IsKeyPressed(KEY_RIGHT)) seciliResimIndex = (seciliResimIndex + 1) % RESIM_SAYISI;
                if (IsKeyPressed(KEY_LEFT)) seciliResimIndex = (seciliResimIndex - 1 + RESIM_SAYISI) % RESIM_SAYISI;
            }
            if (IsKeyPressed(KEY_ENTER) && harfSayisi > 0) durum = EKRAN_OYUN;
        } else {
            if (IsKeyPressed(KEY_M)) { durum = EKRAN_ISIM_GIRISI; sayac = 0; oyunBitti = 0; tahtayiKar(board, 200); }
            if (!oyunBitti) {
                int moved = 0;
                if (IsKeyPressed(KEY_W)) moved = moveUp(board);
                else if (IsKeyPressed(KEY_S)) moved = moveDown(board);
                else if (IsKeyPressed(KEY_A)) moved = moveLeft(board);
                else if (IsKeyPressed(KEY_D)) moved = moveRight(board);
                if (moved) sayac++;
                if (oyunuKazandiMi(board)) { oyunBitti = 1; skoruKaydet(oyuncuIsmi, sayac); t5n = top5Al(t5); }
            }
            if (IsKeyPressed(KEY_SPACE)) { tahtayiKar(board, 100); sayac = 0; oyunBitti = 0; }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (durum == EKRAN_ISIM_GIRISI) {
            DrawTextEx(oyunFontu, "PUZZLE MASTER", (Vector2){415, 120}, 45, spc, DARKGRAY);
            DrawTextEx(oyunFontu, "Isminizi girin:", (Vector2){490, 200}, 20, spc, GRAY);
            DrawRectangle(375, 240, 400, 60, WHITE); DrawRectangleLines(375, 240, 400, 60, BLUE);
            DrawTextEx(oyunFontu, oyuncuIsmi, (Vector2){395, 255}, 30, spc, MAROON);
            
            DrawTextEx(oyunFontu, "[ALT] TEMA DEGISTIR", (Vector2){450, 340}, 20, spc, DARKGRAY);
            const char* m = (seciliTema == TEMA_SAYILAR) ? "MOD: SAYILAR" : TextFormat("MOD: RESIM (%s)", resimIsimleri[seciliResimIndex]);
            DrawTextEx(oyunFontu, m, (Vector2){415, 375}, 28, spc, (seciliTema == TEMA_SAYILAR) ? BLUE : GREEN);
            if(seciliTema == TEMA_RESIM) DrawTextEx(oyunFontu, "< SECMEK ICIN OK TUSLARI >", (Vector2){430, 415}, 18, spc, GRAY);
            
            if (((int)(GetTime() * 2) % 2) == 0) {
                Vector2 sz = MeasureTextEx(oyunFontu, oyuncuIsmi, 30, spc);
                DrawTextEx(oyunFontu, "_", (Vector2){395 + sz.x + 2, 255}, 30, spc, MAROON);
            }
        } else {
            drawBoard(board, seciliTema, puzzleResimleri[seciliResimIndex]);
            DrawTextEx(oyunFontu, TextFormat("Hamle: %d", sayac), (Vector2){650, 180}, 40, spc, BLACK);
            DrawTextEx(oyunFontu, TextFormat("Oyuncu: %s", oyuncuIsmi), (Vector2){650, 230}, 25, spc, DARKBLUE);
            DrawTextEx(oyunFontu, "[SPACE] Karistir  [M] Menu  [TAB] Skorlar", (Vector2){650, 300}, 18, spc, GRAY);
            if (IsKeyDown(KEY_TAB)) {
                DrawRectangle(0, 0, 1150, 700, (Color){0,0,0,220});
                DrawTextEx(oyunFontu, "LIDERLIK TABLOSU", (Vector2){400, 150}, 40, spc, GOLD);
                for(int i=0; i<t5n; i++) DrawTextEx(oyunFontu, TextFormat("%d. %s - %d", i+1, t5[i].isim, t5[i].skor), (Vector2){430, 230+i*55}, 30, spc, WHITE);
            }
            if (oyunBitti) DrawTextEx(oyunFontu, "TEBRIKLER!", (Vector2){400, 620}, 50, spc, GREEN);
        }
        EndDrawing();
    }
    for(int i=0; i<RESIM_SAYISI; i++) UnloadTexture(puzzleResimleri[i]);
    UnloadFont(oyunFontu); CloseWindow();
    return 0;
}