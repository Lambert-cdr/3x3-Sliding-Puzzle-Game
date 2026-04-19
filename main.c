#include <raylib.h>
#include <stdio.h>

#define EKRAN_BOYUTU 600
#define KARE_BOYUTU (EKRAN_BOYUTU / 3)

void moveUp(int board[3][3]){
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
    }
}

void moveDown(int board[3][3]){
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
    }
}

void moveRight(int board[3][3]){
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
    }
}

void moveLeft(int board[3][3]){
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
    }
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

int main() {

    InitWindow(EKRAN_BOYUTU, EKRAN_BOYUTU, "Puzzle Oyunu");
    SetTargetFPS(60); 

    int board[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    tahtayiKar(board,200);
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_W)) {
            moveUp(board);
        } else if (IsKeyPressed(KEY_S)) {
            moveDown(board);
        } else if (IsKeyPressed(KEY_A)) {
            moveLeft(board);
        } else if (IsKeyPressed(KEY_D)) {
            moveRight(board);
        }

        BeginDrawing();


        ClearBackground((Color){ 236, 240, 241, 255 });

        drawBoard(board);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}