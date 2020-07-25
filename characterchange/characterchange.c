#include "charcaterchange.h"
#include "../Player/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cc_makeme(FieldInfo *f_info){
    char myname[100];
    //mynameに入力された名前を格納
    int cnum=10;
    //cnumにキャラ選択の番号が格納

    printf("Hello\n");
    printf("Welcome to snowball fight game!!!\n");
    //スタートのあいさつ

    printf("----------------------------------------------\n");
    printf("Input your name: ");
    // TODO: testing
    // scanf("%s", myname);
    strcpy(myname, "player");
    myname[99] = '\0';
    printf("Your name is %s\n",myname);
    printf("----------------------------------------------\n");
    //名前入力

    //キャラ選択
    printf("Which character do you choose?\n");
    printf("-------------------------\n");
    printf("0: DORAEMON\n");
    printf("1: LOTSO\n");
    printf("2: KATANA\n");
    printf("3: Rilakkuma\n");
    printf("-------------------------\n");

    //正しい数字が入るまで繰り返す入力を促す
    while(cnum<0 || cnum>3){
        printf("Input character number: ");
        // TODO: testing
        // scanf("%d",&cnum);
        cnum = 0;
        if(cnum>-1&&cnum<4){
            break;
        }
        printf("Error: Your number is wrong\n");
        printf("Please Input number 0~3\n");
    }
    //正しい値が入力されたら表示する
    printf("-------------------------\n");
    printf("Your character number is %d\n",cnum);

    //入力された数字に応じてキャラの確認
    if(cnum==0){
        printf("Your character is DORAEMON!\n");
    }else if(cnum==1){
        printf("Your character is LOTSO!\n");
    }else if(cnum==2){
        printf("Your character is KATANA!\n");
    }else{
        printf("Your character is Rilakkuma!\n");
    }
    printf("----------------------------------------------\n");

    f_info->me = (Player *)malloc(sizeof(Player));
    // player_init(f_info->me, rand() % FIELD_MAX_X, rand() % FIELD_MAX_Z, myname, cnum);
    player_init(f_info->me, 20, 50, "test player", rand() % 4);
}

void cc_window(){}
