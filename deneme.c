#include <stdio.h>
#include <stdlib.h>

void boardYazdir(int board[3][3]){
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (board[i][j]==0){
                    printf("#");
                }else{
                    printf("%d",board[i][j]);
            }       
            printf(" ");
        }
        printf("\n");
    }
}

void moveUp(int board[3][3]){                                                    //0 1 2
                                                                           
    int tempSatir;                                                         //[0] //1 2 3
    int tempSutun;                                                         //[1] //4 5 6
    int tempC;                                                             //[2] //7 8 0
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (board[i][j]==0){
                tempSatir = i;
                tempSutun = j;
            }
        }
    }    
    if (tempSatir == 0){
        printf("\nGecersiz Hareket!\n");
    }else{
        tempC = board[tempSatir-1][tempSutun];
        board[tempSatir-1][tempSutun] = 0;
        board[tempSatir][tempSutun] = tempC;
    }    
}
void moveDown(int board[3][3]){
    int tempSatir;
    int tempSutun;
    int tempC;
    for (int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if (board[i][j]==0){
                tempSatir = i;
                tempSutun = j;
            }
            
        }
    }
    if(tempSatir==2){
        printf("\nGecersiz Hareket!\n");
    }else{
        tempC = board[tempSatir+1][tempSutun];
        board[tempSatir+1][tempSutun] = 0;
        board[tempSatir][tempSutun] = tempC;
    }
    
    
}

void moveRight(int board[3][3]){
    int tempSatir;
    int tempSutun;
    int tempC;

    for (int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(board[i][j]==0){
                tempSatir=i;
                tempSutun=j;
            }
        }
    }
    if(tempSutun == 2){
        printf("\nGecersiz Hareket!\n");
    }else{
        tempC = board[tempSatir][tempSutun+1];
        board[tempSatir][tempSutun+1] = 0;
        board[tempSatir][tempSutun] = tempC;
    }
    
}

void moveLeft(int board[3][3]){
    int tempSatir;
    int tempSutun;
    int tempC;

    for (int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(board[i][j]==0){
                tempSatir=i;
                tempSutun=j;
            }
        }
    }

    if (tempSutun == 0){
        printf("\nGecersiz Hareket!\n");
    }else{
        tempC = board[tempSatir][tempSutun-1];
        board[tempSatir][tempSutun-1] = 0;
        board[tempSatir][tempSutun] = tempC;
    }
    


}




int main(){
    int board[3][3] = {{1,2,3},{4,5,6},{7,8,0}};


    printf("-----Puzzle oyununa HOŞGELDİNİZ!-----\n");
    printf("\nOynanış:\n");
    printf("\nw tuşu boş kareyi üstteki kare ile yer değiştirmenizi sağlar.");
    printf("\na tuşu boş kareyi soldaki kare ile yer değiştirmenizi sağlar.");
    printf("\ns tuşu boş kareyi altındaki kare ile yer değiştirmenizi sağlar.");
    printf("\nd tuşu boş kareyi sağındaki kare ile yer değiştirmenizi sağlar.\n\n");


    boardYazdir(board);

    int game=1;
    while (game)
    {
        char yon;
        printf("\nyon seciniz(w,a,s,d): ");
        scanf("%c",&yon);
        if (yon != 'w' && yon != 'a' && yon != 's' && yon != 'd')
        {
            printf("Lutfen gecerli bir tusa basınız!");
            continue;
        }else if (yon == 'w'){
            moveUp(board);
            boardYazdir(board);            
        }else if (yon == 's'){
            moveDown(board);
            boardYazdir(board);
        }else if(yon == 'd'){
            moveRight(board);
            boardYazdir(board);
        }else if(yon == 'a'){
            moveLeft(board);
            boardYazdir(board);
        }
        
        
    }
    
    



    




    return 0;
}