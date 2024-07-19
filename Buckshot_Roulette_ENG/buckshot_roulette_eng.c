#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>

#define shotgun_bullets 3
#define max_item_r1 2
#define max_item_r2 3
#define max_item_r3 4
#define max_item 8
#define max_lives_r1 3
#define max_lives_r2 4
#define max_lives_r3 6
#define sleep 1000 // a one-second pause between each interaction


typedef struct item //typedef item
{
    char name[20]; //item name
}item;

typedef struct player //typedef player
{
    int lives; // player’s number of lives
    int counter; // player's number of items
    struct item item[max_item]; //array of items
}player;


int *bullets(int shotgun[], int *no_blank_rounds, int *no_live_rounds, int *no_bullets)
{
    *no_blank_rounds=0;
    *no_live_rounds=0;
    for(int i=0;i<*no_bullets;i++)
    {
        shotgun[i]=rand()%2; // a blank or a live round is randomly added 
        if(shotgun[i]==0) //if it is a blank round
        {
            (*no_blank_rounds)++; //increment the number of blank rounds
        }
        else //if it is a live round
        {
            (*no_live_rounds)++; //increment the number of live rounds
        }
    }
    return shotgun;
}


void show_bullets(int shotgun[], int *no_blank_rounds, int *no_live_rounds, int *no_bullets)
{
    FILE *f;
    f=fopen("bullets_order.txt","w+"); //this file is for showing the order and type of bullets in the shotgun (do not peek into the file while playing)
    if(!f)
    {
        perror("\nCould not open the file for writing!\n");
        exit(1);
    }
    for(int i=0;i<*no_bullets;i++)
    {
        if(shotgun[i]%2==0) //if it is a blank round
        {
            fprintf(f,"%d - blank\n",shotgun[i]); //print blank round
        }
        else //if it is a live round
        {
            fprintf(f,"%d - live\n",shotgun[i]); //print live round
        }
    }
    fclose(f);
}


int *load_shotgun(int shotgun[], int *no_blank_rounds, int *no_live_rounds, int *no_bullets)
{
    shotgun=bullets(shotgun,no_blank_rounds,no_live_rounds,no_bullets); //the shotgun is randomly loaded with bullets 
    while(((*no_bullets-abs(*no_blank_rounds-*no_live_rounds))+1)<=abs(*no_blank_rounds-*no_live_rounds)) //as long as there is no balance between blank and live rounds
    {
        shotgun=bullets(shotgun,no_blank_rounds,no_live_rounds,no_bullets); //shotgun bullets rebalance
    }
    show_bullets(shotgun,no_blank_rounds,no_live_rounds,no_bullets); //show the shotgun bullets in the "bullets_order" file
    return shotgun;
}


void show_items(player *p1, player *p2)
{
    FILE *f,*g;
    f=fopen("player1_items.txt","w+"); //this file is for showing the items of player 1
    if(!f)
    {
        perror("\nCould not open the file for writing!\n");
        exit(1);
    }
    for(int i=1;i<=p1->counter;i++)
    {
        fprintf(f,"%s\n",p1->item[i-1].name); //print item’s name 
    }
    fclose(f);
    g=fopen("player2_items.txt","w+"); //this file is for showing the items of player 2
    if(!g)
    {
        perror("\nCould not open the file for writing!\n");
        exit(1);
    }
    for(int i=1;i<=p2->counter;i++)
    {
        fprintf(f,"%s\n",p2->item[i-1].name); //print item’s name 
    }
    fclose(g);
}


char *check_item(int item, char *item_name)
{
    int current_item=item;
    switch (current_item) //check which item a player has chosen
    {
        case 0:
            strcpy(item_name,"handsaw");
            break;
        case 1:
            strcpy(item_name,"magnifying glass");
            break;
        case 2:
            strcpy(item_name,"beer");
            break;
        case 3:
            strcpy(item_name,"handcuffs");
            break;
        case 4:
            strcpy(item_name,"cigarette pack");
            break;
        case 5:
            strcpy(item_name,"inverter");
            break;
        case 6:
            strcpy(item_name,"burner phone");
            break;
        case 7:
            strcpy(item_name,"expired medicine");
            break;
        case 8:
            strcpy(item_name,"adrenaline");
            break;
        case 9:
            strcpy(item_name,"shotgun");
    }
    return item_name;
}


player *give_items_player(player *p, int *no_item)
{
    int new_item;
    for(int i=0;i<*no_item;i++)
    {
        if(p->counter==8) //if the player already has 8 items
        {
            break; //will not receive new ones
        }
        else //if the player has less than 8 items
        {
            new_item=rand()%9;
            p->counter++; //increment the player’s number of items 
            strcpy(p->item[p->counter-1].name,check_item(new_item,p->item[p->counter-1].name)); //we add in the array of items the item received randomly
            if(i>0)
            {
                while(strcmp(p->item[p->counter-1].name,p->item[p->counter-2].name)==0) //if you get the same item twice in a row
                {
                    new_item=rand()%9;
                    strcpy(p->item[p->counter-1].name,check_item(new_item,p->item[p->counter-1].name)); //will be changed with another item, to be fair
                }
            }   
        }
    }
    return p;
}


void handsaw(int *cut)
{
    printf("\nThe next bullet is twice as deadly!\n");
    (*cut)=1; //if the handsaw was used
}


void magnifying_glass(int shotgun[])
{
    FILE *h;
    h=fopen("game_flow.txt","w+"); //this file is for showing the bullet seen with a magnifying glass or discovered through the burner phone
    if(!h)
    {
        perror("\nCould not open the file for writing!\n");
        exit(1);
    }
    if(shotgun[0]%2==0) //if it is a blank round
    {
        fprintf(h,"Current bullet is blank!\n"); //print blank round
    }
    else //if it is a live round
    {
        fprintf(h,"Current bullet is live!\n"); //print live round
    }
    fclose(h);
}


int *beer(int shotgun[], int *no_blank_rounds, int *no_live_rounds, int *no_bullets)
{
    if(shotgun[0]%2==0) //if it is a blank round
    {
        printf("\nThe ejected bullet was blind!\n"); //print blank round
        (*no_blank_rounds)--; //decrement the number of blind rounds
    }
    else //if it is a live round
    {
        printf("\nThe ejected bullet was live!\n"); //print live round
        (*no_live_rounds)--; //decrement the number of live rounds
    }
    for(int i=0;i<*no_bullets-1;i++)
    {
        shotgun[i]=shotgun[i+1]; //we move the rest of the bullets by one position backwards
    }    
    (*no_bullets)--; //decrement the number of bullets
    return shotgun;
}


void handcuffs(int *advantage1, int *advantage2, int *tied1, int *tied2)
{
    if(*advantage1==1) //if it is the turn of player 1
    {
        printf("\nPlayer 2 is handcuffed for a turn!\n");
        (*tied2)=1; //player 2 is handcuffed
    }
    else //if it is the turn of player 2
    {
        printf("\nPlayer 1 is handcuffed for a turn!\n");
        (*tied1)=1; //player 1 is handcuffed
    }
}


void cigarette_pack(player *p, int *round)
{
    switch (*round) //depending on which round is
    {
        case 1:
            if(p->lives<max_lives_r1) //if the player has less than 3 lives
            {
                p->lives++; //the player gets a bonus life
                printf("\nThe player got a life back!\n");
                printf("\nThe player has %d lives!\n",p->lives);
            }
            else
            {
                printf("\nThe player already has the maximum number of lives! (%d lives)\n",p->lives);
            }
            break;
        case 2:
            if(p->lives<max_lives_r2) //if the player has less than 4 lives
            {
                p->lives++; //player gets a bonus life
                printf("\nThe player got a life back!\\n");
                printf("\nThe player has %d lives!\n",p->lives);

            }
            else
            {
                printf("\nThe player already has the maximum number of lives! (%d lives)\n",p->lives);
            }
            break;
        case 3:
            if(p->lives<max_lives_r3) //if the player has less than 6 lives
            {
                p->lives++; //player gets a bonus life
                printf("\nThe player got a life back!\n");
                printf("\nThe player has %d lives!\n",p->lives);
            }
            else
            {
                printf("\nThe player already has the maximum number of lives! (%d lives)\n",p->lives);
            }
            break;
    }
}


int *inverter(int shotgun[], int *no_blank_rounds, int *no_live_rounds)
{
    if(shotgun[0]%2==0) //if it is a blank round
    {
        shotgun[0]=1; //becomes live
        (*no_blank_rounds)--; //decrement the number of blank rounds 
        (*no_live_rounds)++; //increment the number of live rounds 
    }
    else //if it is a live round
    {
        shotgun[0]=0; //becomes blank
        (*no_live_rounds)--; //decrement the number of live rounds
        (*no_blank_rounds)++; //increment the number of blank rounds
    }
    printf("\nCurrent bullet has been reversed!\n");
    return shotgun;
}


void burner_phone(int shotgun[], int *no_blank_rounds, int *no_live_rounds, int *no_bullets)
{
    FILE *h;
    h=fopen("game_flow.txt","w+"); //this file is for showing the bullet seen with a magnifying glass or discovered through the burner phone
    if(!h)
    {
        perror("\nCould not open the file for writing!\n");
        exit(1);
    }
    int bullet=(rand()%(*no_bullets))+1;
    for(int i=1;i<=*no_bullets;i++)
    {
        if(bullet==i) //if the bullet in the shotgun matches the one selected randomly
        {
            if(shotgun[i-1]%2==0) //if it is a blank round
            {
                fprintf(h,"Bullet %d is blank!\n",bullet); //print blank round and whichever is in the shotgun 
            }
            else //if it is a live round
            {
                fprintf(h,"Bullet %d is live!\n",bullet); //print live round and whichever is in the shotgun 
            }
        }
    }
    fclose(h);
}


void expired_medicine(player *p, int *round)
{
    int luck=rand()%6; //50/50 chance of it being good or bad
    if(luck%2==1) //if it is good
    {
        switch (*round) //depending on which round it is
        {
            case 1:
                if(p->lives<max_lives_r1-1) //if the player has less than 2 lives
                {
                    p->lives=p->lives+2; //gets 2 lives
                    printf("\nThe player recovered 2 lives!\n");
                    printf("\nThe player has %d lives!\n",p->lives);
                }
                break;
            case 2:
                if(p->lives<max_lives_r2-1) //if the player has less than 3 lives
                {
                    p->lives=p->lives+2; //gets 2 lives
                    printf("\nThe player recovered 2 lives!\n");
                    printf("\nThe player has %d lives!\n",p->lives);
                }
                break;
            case 3:
                if(p->lives<max_lives_r3-1) //if the player has less than 5 lives
                {
                    p->lives=p->lives+2; //gets 2 lives
                    printf("\nThe player recovered 2 lives!\n");
                    printf("\nThe player has %d lives!\n",p->lives);
                }
                break;
        }
    }
    else //if it is bad
    {
        p->lives--; //the player loses a life
        printf("\nThe player lost a life!\n");
        printf("\nThe player has %d lives!\n",p->lives);
    }
}


int adrenaline(player *p, player *pla, int *advantage1, int *advantage2)
{
    int choice; //choose item
    char chosen[20]; //item chosen
    int ok; //Check if the player has the item to be stolen
    printf("\nThe item you want to steal = ");
    scanf("%d",&choice); //choose which item you steal
    while(choice<0 || choice>8) //as long as you choose an invalid number    
    {
        printf("\nSteal a valid item!\n");
        printf("\nThe item you want to steal = ");
        scanf("%d",&choice); //choose which item you steal
    }
    if(choice>=0 && choice<=8) //if a valid item was chosen
    {
        ok=0; //suppose he does not have the chosen item
        if(choice==8) //if adrenaline was chosen
        {
            while(choice==8) //as long as adrenaline is chosen
            {
                printf("\nYou can't steal the adrenaline!\n");
                printf("\nThe item you want to steal = ");
                scanf("%d",&choice); //choose which item you steal
            }
        }
        strcpy(chosen,check_item(choice,chosen)); //the chosen item is checked
        while(ok==0) //as long as the opponent does not have the chosen item
        {
            for(int i=1;i<=pla->counter;i++)
            {
                if(strcmp(chosen,pla->item[i-1].name)!=0) //check if the player has the item
                {
                    continue; //if he does not have it, continue
                }
                else if(ok==0) //if he has it 
                {
                    ok=1; //is checked that he has it
                    for(int k=i-1;k<pla->counter-1;k++)
                    {
                        strcpy(pla->item[k].name,pla->item[k+1].name); //the item is removed from the list of items
                    }
                    pla->counter--; //decrement the number of items
                }
            }
            if(ok==0) //if he does not have it at all
            {
                printf("\nSteal a valid item!\n");
                printf("\nThe item you want to steal = ");
                scanf("%d",&choice); //choose which item to steal
                ok=0;  //suppose he does not have the chosen item
                strcpy(chosen,check_item(choice,chosen)); //the chosen item is checked
            }
        }
        if(*advantage1==1) //if player 1 stole an item
        {
            show_items(p,pla); //the items are shown again after being stolen from player 2
        }
        else //if player 2 stole an item
        {
            show_items(pla,p); //the items are shown again after being stolen from player 1
        }
        printf("\nThe player stole %s!\n",chosen);
    }
    return choice;
}


void shotgun_bullet(player *p, player *pla, int *cut, int *safe, int shotgun[],int *no_blank_rounds, int *no_live_rounds, int *no_bullets, int *round, int *no_item)
{
    if(*no_bullets!=0) //if the shotgun is not empty
    {
        printf("\nWho are you shooting?\n");
        printf("\nMe (code 0)\n");
        printf("\nOpponent (code 1)\n");
        int decision; //who is to be shot
        printf("\nCode = ");
        scanf("%d",&decision); //choose who gets shot
        while(decision!=0 && decision!=1) //as long as you choose an invalid number
        {
            printf("\nChoose a valid code!\n");
            printf("\nCode = ");
            scanf("%d",&decision); //choose who is to be shot
        }
        if(decision==0) //if the player shoots himself
        {
            printf("\nThe player shoots himself!\n");
            Sleep(sleep);
            if(shotgun[0]==1) //if it is a live round 
            {
                if(*cut==1) //if the handsaw was used
                {
                    printf("\nThe player lost 2 lives!\n");
                    p->lives=p->lives-2; //decrement the number of lives by 2 
                    (*no_live_rounds)--; //decrement the number of live rounds 
                    Sleep(sleep);
                    printf("\nThe player has %d lives!\n",p->lives);
                }
                else //if not
                {
                    printf("\nThe player lost a life!\n");
                    p->lives=p->lives-1; //decrement the number of lives by 1 
                    (*no_live_rounds)--; //decrement the number of live rounds 
                    Sleep(sleep);
                    printf("\nThe player has %d lives!\n",p->lives);
                }
                for(int i=0;i<*no_bullets-1;i++)
                {
                    shotgun[i]=shotgun[i+1]; //move the rest of the bullets one place backwards
                }
                (*no_bullets)--; //decrement the number of bullets 
            }
            else
            {
                printf("\nThe player is safe...for now!\n");
                (*safe)++; // it’s the same player’s turn
                (*no_blank_rounds)--; //decrease the number of blank rounds 
                for(int i=0;i<*no_bullets-1;i++)
                {
                    shotgun[i]=shotgun[i+1]; //move the rest of the bullets one place backwards
                }
                (*no_bullets)--; //decrease the number of bullets 
                Sleep(sleep);
                printf("\nThe player has %d lives!\n",p->lives);
            }
        }
        else //if the player shoots the opponent
        {
            printf("\nThe player shoots the opponent!\n");
            Sleep(sleep);
            if(shotgun[0]==1) //if it is a live round 
            {
                if(*cut==1) //if the handsaw was used
                {
                    printf("\nThe opponent lost 2 lives!\n");
                    p->lives=p->lives-2; //decrease the number of lives by 2
                    (*no_live_rounds)--; //decrease the number of live rounds 
                    Sleep(sleep);
                    printf("\nThe opponent has %d lives!\n",p->lives);
                }
                else //if not
                {
                    printf("\nThe opponent lost a life!\n");
                    p->lives=p->lives-1; //decrease the number of lives by 1 
                    (*no_live_rounds)--; //decrease the number of live rounds 
                    Sleep(sleep);
                    printf("\nThe opponent has %d lives!\n",p->lives);
                }
                for(int i=0;i<*no_bullets-1;i++)
                {
                    shotgun[i]=shotgun[i+1]; //move the rest of the bullets one place backwards
                }
                (*no_bullets)--; //decrease the number of bullets 
            }
            else
            {
                printf("\nThe opponent is safe...for now!\n");
                (*safe)++; //it’s the same player’s turn
                (*no_blank_rounds)--; //decrease the number of blank rounds 
                for(int i=0;i<*no_bullets-1;i++)
                {
                    shotgun[i]=shotgun[i+1]; //move the rest of the bullets one place backwards
                }
                (*no_bullets)--; //decrease the number of bullets
                Sleep(sleep);
                printf("\nThe opponent has %d lives!\n",p->lives);
            }
        }
        show_bullets(shotgun,no_blank_rounds,no_live_rounds,no_bullets); //shows the shotgun bullets in the "order_shots" file
    }
    else
    {
        switch (*round) //depending on which round it is
        {
            case 1:
                *no_bullets=(rand()%shotgun_bullets)+2; //number of bullets between 2 and 4 for the shotgun 
                break;
            case 2:
                *no_bullets=(rand()%shotgun_bullets)+4; //number of bullets between 4 and 6 for the shotgun
                break;
            case 3:
                *no_bullets=(rand()%shotgun_bullets)+6; //number of bullets between 6 and 8 for the shotgun
                break;
        }
        printf("\nThe shotgun is reloaded...\n");
        shotgun=load_shotgun(shotgun,no_blank_rounds,no_live_rounds,no_bullets); //the shotgun is loaded
        Sleep(sleep);
        printf("\nNumber of bullets = %d\n",*no_bullets); //print the number of bullets in the shotgun
        printf("Number of blank rounds = %d\n",*no_blank_rounds); //print the number of live rounds in the shotgun 
        printf("Number of live rounds = %d\n",*no_live_rounds); //print the number of blank rounds in the shotgun
        printf("\nItems are given...\n");
        p=give_items_player(p,no_item); //give items to player 1
        pla=give_items_player(pla,no_item); //give items to player 2
        show_items(p,pla); //print items of the 2 players
        Sleep(sleep);
    }
}


void choice(int choose, int *cut, int *safe, int shotgun[],int *no_blank_rounds, int *no_live_rounds, int *no_bullets, player *p, item *it, int *round, player *pla, int *advantage1, int *advantage2, int *tied1, int *tied2, int *no_item)
{
    switch (choose) //depending on the chosen item
    {
        case 0:
            handsaw(cut);
            Sleep(sleep);
            break;
        case 1:
            magnifying_glass(shotgun);
            Sleep(sleep);
            break;
        case 2:
            shotgun=beer(shotgun,no_blank_rounds,no_live_rounds,no_bullets);
            show_bullets(shotgun,no_blank_rounds,no_live_rounds,no_bullets); //print the shotgun bullets in the "bullets_order" file
            Sleep(sleep);
            break;
        case 3:
            handcuffs(advantage1,advantage2,tied1,tied2);
            Sleep(sleep);
            break;
        case 4:
            cigarette_pack(p,round);
            Sleep(sleep);
            break;
        case 5:
            shotgun=inverter(shotgun,no_blank_rounds,no_live_rounds);
            show_bullets(shotgun,no_blank_rounds,no_live_rounds,no_bullets); //print the shotgun bullets in the "bullets_order" file
            Sleep(sleep);
            break;
        case 6:
            burner_phone(shotgun,no_blank_rounds,no_live_rounds,no_bullets);
            Sleep(sleep);
            break;
        case 7:
            expired_medicine(p,round);
            Sleep(sleep);
            break;
        case 8:
            choose=adrenaline(p,pla,advantage1,advantage2);
            Sleep(sleep);
            choice(choose,cut,safe,shotgun,no_blank_rounds,no_live_rounds,no_bullets,p,it,round,pla,advantage1,advantage2,tied1,tied2,no_item);
            break;
        case 9:
            shotgun_bullet(p,pla,cut,safe,shotgun,no_blank_rounds,no_live_rounds,no_bullets,round,no_item);
            Sleep(sleep);
            break;
    }
}


void select_item(int shotgun[], player *p, item *it, int *no_blank_rounds, int *no_live_rounds, int *no_bullets, int *round, player *pla, int *advantage1, int *advantage2, int *tied1, int *tied2, int *no_item)
{
    if(*no_bullets==0) //if the shotgun is empty
    {
        switch (*round) //depending on which round it is
        {
            case 1:
                *no_bullets=(rand()%shotgun_bullets)+2; //number of bullets between 2 and 4 for the shotgun
                break;
            case 2:
                *no_bullets=(rand()%shotgun_bullets)+4; //number of bullets between 4 and 6 for the shotgun
                break;
            case 3:
                *no_bullets=(rand()%shotgun_bullets)+6; //number of bullets between 6 and 8 for the shotgun
                break;
        }
        printf("\nThe shotgun is reloaded...\n");
        shotgun=load_shotgun(shotgun,no_blank_rounds,no_live_rounds,no_bullets); //the shotgun is loaded
        Sleep(sleep);
        printf("\nNumber of bullets = %d\n",*no_bullets); //print the number of bullets in the shotgun 
        printf("Number of blank rounds = %d\n",*no_blank_rounds); //print the number of live rounds in the shotgun
        printf("Number of live rounds = %d\n",*no_live_rounds); // print the number of blank rounds in the shotgun
        printf("\nItems are given...n");
        p=give_items_player(p,no_item); //give items randomly to player 1
        pla=give_items_player(pla,no_item); //give items randomly to player 2
        show_items(p,pla); //print items of the 2 players
        Sleep(sleep);
    }
    printf("\nSelect item:\n");
    int choose; //item choice
    char chosen[20]; //item chosen
    int ok; //check if the player has the item he wants to use
    int cut=0; //if the player
    int safe=0; //if the player shot himself and was safe
    printf("\nItem = ");
    scanf("%d",&choose); //choose which item you use
    while(choose<0 || choose>9) //as long as you choose an invalid number
    {
        printf("\nChoose a valid item!\n");
        printf("\nItem = ");
        scanf("%d",&choose); //choose the item you use
    }
    if(choose!=9) //unless the shotgun was chosen 
    {
        while(choose>=0 && choose<=8) //if a valid item was chosen
        {
            ok=0; //suppose he does not have the chosen item 
            strcpy(chosen,check_item(choose,chosen)); //check the chosen item
            while(ok==0) //as long as the player does not have the chosen item 
            {
                for(int i=1;i<=p->counter;i++)
                {
                    if(strcmp(chosen,p->item[i-1].name)!=0) //if the player does not have the chosen item
                    {
                        continue; //if he does not have it, continue
                    }
                    else //if he has it
                    {
                        ok=1; //to check that he has it
                    }
                }
                if(ok==0) //if he does not have it at all
                {
                    printf("\nChoose a valid item!\n");
                    printf("\nItem = ");
                    scanf("%d",&choose); //choose what item you use
                    ok=0;  //suppose he does not have the chosen item 
                    strcpy(chosen,check_item(choose,chosen)); //the chosen item is checked 
                }
            }
            printf("\nThe player chose %s!\n",chosen);
            Sleep(sleep);
            for(int i=1;i<=p->counter;i++)
            {
                if(strcmp(chosen,p->item[i-1].name)!=0) //if the chosen item is not found
                {
                    continue; //it contiunes
                }
                else //if it is found
                {
                    for(int k=i-1;k<p->counter-1;k++)
                    {
                        strcpy(p->item[k].name,p->item[k+1].name); //the item is removed from the list of items
                    }
                    p->counter--; //decrease the number of items 
                    break;
                }
            }
            if(*advantage1==1) //if player 1 used an item
            {
                show_items(p,pla); //the items are shown again after player 2 has used an item
            }
            else //if player 2 used an item
            {
                show_items(pla,p); //the items are shown again after player 1 has used an item
            }
            choice(choose,&cut,&safe,shotgun,no_blank_rounds,no_live_rounds,no_bullets,p,it,round,pla,advantage1,advantage2,tied1,tied2,no_item); //the chosen item is used
            printf("\nSelect item:\n");
            printf("\nItem = ");
            scanf("%d",&choose); //choose the item you use
            while(choose<0 || choose>9) //as long as you choose an invalid number
            {
                printf("\nChoose a valid item!\n");
                printf("\nItem = ");
                scanf("%d",&choose); //choose the item you use
            }
        }
    }
    strcpy(chosen,check_item(choose,chosen)); //the shotgun is used
    printf("\nThe player chose %s!\n",chosen);
    Sleep(sleep);
    choice(choose,&cut,&safe,shotgun,no_blank_rounds,no_live_rounds,no_bullets,p,it,round,pla,advantage1,advantage2,tied1,tied2,no_item); //the shotgun is used
    if(*no_bullets==0) //if the shotgun is empty
    {
        switch (*round) //depending on which round it is
        {
            case 1:
                *no_bullets=(rand()%shotgun_bullets)+2; //number of bullets between 2 and 4 for the shotgun
                break;
            case 2:
                *no_bullets=(rand()%shotgun_bullets)+4; //number of bullets between 4 and 6 for the shotgun
                break;
            case 3:
                *no_bullets=(rand()%shotgun_bullets)+6; //number of bullets between 6 and 8 for the shotgun
                break;
        }
        printf("\nThe shotgun is reloaded...\n");
        shotgun=load_shotgun(shotgun,no_blank_rounds,no_live_rounds,no_bullets); //the shutgun is loaded
        Sleep(sleep);
        printf("\nNumber of bullets = %d\n",*no_bullets); //print the number of bullets in the shotgun 
        printf("Number of blank rounds = %d\n",*no_blank_rounds); //print the number of live rounds in the shotgun 
        printf("Number of live rounds = %d\n",*no_live_rounds); //print the number of blank rounds in the shotgun 
        printf("\nItems are given...\n");
        p=give_items_player(p,no_item); //give items to player 1
        pla=give_items_player(pla,no_item); //give items to player 2
        show_items(p,pla); //print items of the 2 players
        Sleep(sleep);
    }
    if(safe==1) // if the player shoots himself with a blank round
    {
        select_item(shotgun,p,it,no_blank_rounds,no_live_rounds,no_bullets,round,pla,advantage1,advantage2,tied1,tied2,no_item); //the player chooses an item again
    }
}


int main()
{
    int round=0; //round
    int score1=0; //score player 1
    int score2=0; //score player 2
    while(round<3) //as long as 3 rounds have not been played
    {
        round++; //current round
        int no_item; //the number of items that players will receive
        int no_bullets; //the number of bullets in the shotgun (random)
        int no_blank_rounds=0; //the number of blank rounds in the shotgun 
        int no_live_rounds=0; //the number of live rounds in the shotgun
        printf("\nROUND %d\n",round);
        player *p1=(player*)malloc(sizeof(player)); //it is allocated memory for the player1 pointer
        if(!p1)
        {
            perror("\nMemory could not be allocated!\n");
            exit(2);
        }
        player *p2=(player*)malloc(sizeof(player)); //it is allocated memory for the player2 pointer
        if(!p2)
        {
            perror("\nMemory could not be allocated!\n");
            exit(2);
        }
        item *it=(item*)malloc(max_item*sizeof(item)); //it is allocated memory for the item pointer
        if(!it)
        {
            perror("\nMemory could not be allocated!\n");
            exit(2);
        }
        srand(time(0));
        switch (round) //if it is the first round
        {
            case 1:
                p1->lives=max_lives_r1; //3 lives
                p1->counter=0;
                p2->lives=max_lives_r1; //3 lives
                p2->counter=0;
                no_item=max_item_r1; //players will receive 2 items
                no_bullets=(rand()%shotgun_bullets)+2; //number of bullets between 2 and 4 for the shotgun
                break;
            case 2:
                p1->lives=max_lives_r2; //4 lives
                p1->counter=0;
                p2->lives=max_lives_r2; //4 lives
                p2->counter=0;
                no_item=max_item_r2; //players will receive 3 items 
                no_bullets=(rand()%shotgun_bullets)+4; //number of bullets between 4 and 6 for the shotgun
                break;
            case 3:
                p1->lives=max_lives_r3; //6 lives
                p1->counter=0;
                p2->lives=max_lives_r3; //6 lives
                p2->counter=0;
                no_item=max_item_r3; //players will receive 4 items
                no_bullets=(rand()%shotgun_bullets)+6; //number of bullets between 6 and 8 for the shotgun
                break;
        }
        Sleep(sleep);
        int *shotgun=(int*)malloc(no_bullets*sizeof(int)); //it is allocated memory for the shotgun pointer
        if(!shotgun)
        {
            perror("\nMemory could not be allocated!\n");
            exit(2);
        }
        shotgun=load_shotgun(shotgun,&no_blank_rounds,&no_live_rounds,&no_bullets); //the shotgun is loaded
        printf("\nNumber of bullets = %d\n",no_bullets); //print the number of bullets in the shotgun 
        printf("Number of blank rounds = %d\n",no_blank_rounds); //print the number of live rounds in the shotgun 
        printf("Number of live rounds = %d\n",no_live_rounds); //print the number of blank rounds in the shotgun 
        Sleep(sleep);
        printf("\nItems are given...\n");
        p1=give_items_player(p1,&no_item); //give items to player 1
        p2=give_items_player(p2,&no_item); //give items to player 2 
        show_items(p1,p2); //print items of the 2 players
        Sleep(sleep);
        int pla1=rand()%2;
        int pla2=rand()%2;
        int decision=rand()%2;
        int advantage1=0; //if it’s the turn of player 1
        int advantage2=0; // if it’s the turn of player 2
        int tied1=0; //if player 1 will be handcuffed
        int tied2=0; //if player 2 will be handcuffed 
        printf("\nIt is decided who starts...\n");
        while(pla1==pla2) //it is decided randomly who starts
        {
            pla1=rand()%2;
            pla2=rand()%2;
        }
        Sleep(sleep);
        if(decision==pla1)
        {
            advantage1=1; //player 1’s turn 
        }
        else
        {
            advantage2=1; //player 2’s turn 
        }
        if(advantage1==1) //player 1 starts
        {
            printf("\nPlayer 1 starts\n");
            while(p1->lives>0 && p2->lives>0) //as long as no player is dead
            {
                if(tied1==0) //if player 1 has not been handcuffed 
                {
                    advantage1=1; //it is player 1’ turn
                    advantage2=0; //player 2’s turn passed
                    if(p1->lives>0) //if player 1 is not dead
                    {
                        printf("\nJ Player 1 chooses an item!\n");
                        select_item(shotgun,p1,it,&no_blank_rounds,&no_live_rounds,&no_bullets,&round,p2,&advantage1,&advantage2,&tied1,&tied2,&no_item); //player 1 chooses an item
                    }
                }
                tied1=0; //if he has been handcuffed for a turn, the effect wears off
                if(no_bullets==0 && (p1->lives!=0 && p2->lives!=0)) //if the shotgun is empty and no player is dead
                {
                    switch (round) //depending on which round it is
                    {
                        case 1:
                            no_bullets=(rand()%shotgun_bullets)+2; //number of bullets between 2 and 4 for the shotgun
                            break;
                        case 2:
                            no_bullets=(rand()%shotgun_bullets)+4; //number of bullets between 4 and 6 for the shotgun
                            break;
                        case 3:
                            no_bullets=(rand()%shotgun_bullets)+6; //number of bullets between 6 and 8 for the shotgun 
                            break;
                    }
                    printf("\nThe shoygun is reloaded...\n");
                    shotgun=load_shotgun(shotgun,&no_blank_rounds,&no_live_rounds,&no_bullets); //the shotgun is reloaded
                    Sleep(sleep);
                    printf("\nNumber of bullets = %d\n",no_bullets); //print the number of bullets in the shotgun
                    printf("Number of blank rounds = %d\n",no_blank_rounds); //print the number of live rounds in the  shotgun
                    printf("Number of live rounds = %d\n",no_live_rounds); //print the number of blank rounds in the shotgun 
                    printf("\nItems are given...\n");
                    p1=give_items_player(p1,&no_item); //give items randomly to player 1
                    p2=give_items_player(p2,&no_item); //give items randomly to player 2
                    show_items(p1,p2); //print items of the 2 players 
                    Sleep(sleep);
                }
                if(tied2==0) //if player 2 has not been handcuffed 
                {
                    advantage1=0; //player 1’ turn passed
                    advantage2=1; //it is player 2’ turn
                    if(p2->lives>0) //if player 2 is not dead
                    {
                        printf("\nJPlayer 2 chooses an item!\n");
                        select_item(shotgun,p2,it,&no_blank_rounds,&no_live_rounds,&no_bullets,&round,p1,&advantage1,&advantage2,&tied1,&tied2,&no_item); //player 2 chooses an item
                    }
                }
                tied2=0; //if he has been handcuffed for a turn, the effect wears off
                if(no_bullets==0 && (p1->lives!=0 && p2->lives!=0)) //if the shotgun is empty and no player is dead
                {
                    switch (round) //depending on which round it is
                    {
                        case 1:
                            no_bullets=(rand()%shotgun_bullets)+2; //number of bullets between 2 and 4 for the shotgun
                            break;
                        case 2:
                            no_bullets=(rand()%shotgun_bullets)+4; //number of bullets between 4 and 6 for the shotgun
                            break;
                        case 3:
                            no_bullets=(rand()%shotgun_bullets)+6; //number of bullets between 6 and 8 for the shotgun 
                            break;
                    }
                    printf("\nThe shotgun is reloaded...\n");
                    shotgun=load_shotgun(shotgun,&no_blank_rounds,&no_live_rounds,&no_bullets); //the shotgun is reloaded
                    Sleep(sleep);
                    printf("\nNumber of bullets = %d\n",no_bullets); //print the number of bullets in the shotgun
                    printf("Number of blank rounds = %d\n",no_blank_rounds); //print the number of live rounds in the shotgun
                    printf("Number of live rounds = %d\n",no_live_rounds); //print the number of blank rounds in the shotgun 
                    printf("\nItems are given...\n");
                    p1=give_items_player(p1,&no_item); //give items to player 1
                    p2=give_items_player(p2,&no_item); //give items to player 2
                    show_items(p1,p2); //print items of the 2 players 
                    Sleep(sleep);
                }
            }
        }
        else //player 2 starts
        {
            printf("\nPlayer 2 starts\n");
            Sleep(sleep);
            while(p1->lives>0 && p2->lives>0) //as long as no player is dead
            {
                if(tied2==0) //if player 2 has not been handcuffed 
                {
                    advantage1=0; //player 1’s turn passed 
                    advantage2=1; //it is player 2’s turn 
                    if(p2->lives>0) //if player 2 is not dead
                    {
                    printf("\nPlayer 2 chooses an item!\n");
                    select_item(shotgun,p2,it,&no_blank_rounds,&no_live_rounds,&no_bullets,&round,p1,&advantage1,&advantage2,&tied1,&tied2,&no_item); //player 2 chooses an item
                    }
                }
                tied2=0; //if he has been handcuffed for a turn, the effect wears off
                if(no_bullets==0 && (p1->lives!=0 && p2->lives!=0)) //if the shotgun is empty and no player is dead
                {
                    switch (round) //depending on which round it is 
                    {
                        case 1:
                            no_bullets=(rand()%shotgun_bullets)+2; //number of bullets between 2 and 4 for the shotgun 
                            break;
                        case 2:
                            no_bullets=(rand()%shotgun_bullets)+4; //number of bullets between 4 and 6 for the shotgun 
                            break;
                        case 3:
                            no_bullets=(rand()%shotgun_bullets)+6; //number of bullets between 6 and 8 for the shotgun
                            break;
                    }
                    printf("\nThe shotgun is reloaded...\n");
                    shotgun=load_shotgun(shotgun,&no_blank_rounds,&no_live_rounds,&no_bullets); //the shutgun is reloaded
                    Sleep(sleep);
                    printf("\nNumber of bullets = %d\n",no_bullets); //print the number of bullets in the shotgun
                    printf("Number of blank rounds = %d\n",no_blank_rounds); //print the number of live rounds in the shotgun 
                    printf("Number of live rounds = %d\n",no_live_rounds); //print the number of blank rounds in the shotgun
                    printf("\nItems are given...\n");
                    p1=give_items_player(p1,&no_item); //give items to player 1
                    p2=give_items_player(p2,&no_item); //give items to player 2
                    show_items(p1,p2); //print items of the 2 players 

                    Sleep(sleep);
                }
                if(tied1==0) //if player 1 has not been handcuffed 
                {
                    advantage1=1; //it is player 1’s turn
                    advantage2=0; //player 2’ turn passed
                    if(p1->lives>0) //if player 1 is not dead
                    {
                        printf("\nPlayer 1 chooses an item!\n");
                        select_item(shotgun,p1,it,&no_blank_rounds,&no_live_rounds,&no_bullets,&round,p2,&advantage1,&advantage2,&tied1,&tied2,&no_item); //player 1 chooses an item
                    }
                }
                tied1=0; //if he has been handcuffed for a turn, the effect wears off
                if(no_bullets==0 && (p1->lives!=0 && p2->lives!=0)) //if the shotgun is empty and no player is dead
                {
                    switch (round) //depending on which round it is
                    {
                        case 1:
                            no_bullets=(rand()%shotgun_bullets)+2; //number of bullets between 2 and 4 for the shotgun
                            break;
                        case 2:
                            no_bullets=(rand()%shotgun_bullets)+4; //number of bullets between 4 and 6 for the shotgun
                            break;
                        case 3:
                            no_bullets=(rand()%shotgun_bullets)+6; //number of bullets between 6 and 8 for the shotgun
                            break;
                    }
                    printf("\nThe shotgun is reloaded...\n");
                    shotgun=load_shotgun(shotgun,&no_blank_rounds,&no_live_rounds,&no_bullets); //the shotgun is reloaded
                    Sleep(sleep);
                    printf("\nNumber of bullets = %d\n",no_bullets); //print the number of bullets in the shotgun
                    printf("Number of blank rounds = %d\n",no_blank_rounds); //print the number of live rounds in the shotgun
                    printf("Number of live rounds = %d\n",no_live_rounds); //print the number of blank rounds in the shotgun
                    printf("\nItems are given...\n");
                    p1=give_items_player(p1,&no_item); //give items to player 1
                    p2=give_items_player(p2,&no_item); //give items to player 2
                    show_items(p1,p2); //print items of the 2 players 
                    Sleep(3000);
                }
            }
        }   
        if(p1->lives==0) //if player 1 has no life left
        {
            printf("\nPlayer 1 is dead!\n");
            Sleep(sleep);
            printf("\nPlayer 2 won the round!\n");
            score2++; //one point for player 2
        }
        else //if player 2 has no life left
        {
            printf("\nPlayer 2 is dead!\n");
            Sleep(sleep);
            printf("\nPlayer 1 won the round!\n");
            score1++; //one point for player 1
        }
        printf("\nROUND %d is over!\n",round);
        Sleep(sleep);
        free(p1); //player 1 pointer memory is cleared
        free(p2); //player 2 pointer memory is cleared 
        free(it); //item pointer memory is cleared
        free(shotgun); //shotgun pointer memory is cleared
    }
    printf("\nFinal score: %d - %d",score1,score2); //final score
    Sleep(sleep);
    if(score1>score2) //if player 1 has won more rounds
    {
        printf("\nPlayer 1 won!\n");
    }
    else //if player 2 has won more rounds
    {
        printf("\nPlayer 2 won!\n");
    }
    Sleep(sleep);
    return 0;
}
