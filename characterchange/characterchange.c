#include "characterchange.h"
#include <stdio.h>


void cc_makeme(&fieldinfo){
    char myname[100];
    //mynameに入力された名前を格納
    int cnum=10;
    //cnumにキャラ選択の番号が格納

    printf("Hello");
    printf("\nWelcome to snowball fight game!!!");
    //スタートのあいさつ

    printf("\n----------------------------------------------");
    printf("\nInput your name:");
    scanf("%s", myname);
    printf("\nYour name is %s",myname);
    printf("\n----------------------------------------------");
    //名前入力
    
    
    //キャラ選択
    printf("\nWhich character do you choose?");
    printf("\n-------------------------");
    printf("\nDORAEMON:0");
    printf("\nLOTSO:1");
    printf("\nKATANA:2");
    printf("\nRilakkuma:3");
    printf("\n-------------------------");

    //正しい数字が入るまで繰り返す入力を促す
    while(cnum<0 || cnum>3){
        printf("\nInput character number:");
        scanf("%d",&cnum);
        if(cnum>-1&&cnum<4){
            break;
        }
        printf("\nError Your number is wroing");
        printf("\nPlease Input number 0~3")    
    }
    //正しい値が入力されたら表示する
    printf("\n-------------------------");
    printf("\nYour character number is %d",cnum);
    
    if(cnum==0){
        printf("\nYour character is DORAEMON! ");
        
    }else if(cnum==1){
        printf("\nYour character is LOTSO! "); 
        
        
    }else if(cnum==2){
        printf("\nYour character is KATANA! "); 
        
        
    }else{
        printf("\nYour character is Rilakkuma! "); 
        
    }
    //入力された数字に応じてキャラの確認
    
    printf("\n----------------------------------------------");


    

    Player *me = (Player *)malloc(sizeof(Player));
    //me->id=;
    //me ->location
    //me->hp =100.0;
       //hp初期値
    //me->name =myname;
    //me->sleep_bullet=;
    //me->sleep_wall;
    //me->c =cnum;

    
    

    fieldinfo->me = me;


    
    
}

void cc_window(){}







