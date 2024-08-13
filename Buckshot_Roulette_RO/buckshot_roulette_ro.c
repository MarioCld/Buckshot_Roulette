#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>

#define gloante_shotgun 3
#define max_item_r1 2
#define max_item_r2 3
#define max_item_r3 4
#define max_item 8
#define max_lives_r1 3
#define max_lives_r2 4
#define max_lives_r3 6
#define somn 1000 //pauza de o secunda intre fiecare interactiune


typedef struct item //typedef item
{
    char nume[20]; //numele item-ului
}item;

typedef struct jucator //typedef jucator
{
    int vieti; //numarul de vieti al jucatorului
    int contor; //numarul de iteme al jucatorului
    struct item item[max_item]; //tablou de iteme
}jucator;


int *gloante(int shotgun[], int *nr_gloante_oarbe, int *nr_gloante_adevarate, int *nr_gloante)
{
    *nr_gloante_oarbe=0;
    *nr_gloante_adevarate=0;
    for(int i=0;i<*nr_gloante;i++)
    {
        shotgun[i]=rand()%2; //se adauga aleatoriu glont orb sau glont adevarat
        if(shotgun[i]==0) //daca glontul e orb
        {
            (*nr_gloante_oarbe)++; //incrementam numarul de gloante oarbe
        }
        else //daca glontul e adevarat
        {
            (*nr_gloante_adevarate)++; //incrementam numarul de gloante adevarate
        }
    }
    return shotgun;
}


void afisare_gloante(int shotgun[], int *nr_gloante_oarbe, int *nr_gloante_adevarate, int *nr_gloante)
{
    FILE *f;
    f=fopen("ordine_gloante.txt","w+"); //acest fisier este pentru afisarea ordinii si tipului gloantelor din shotgun (nu trageti cu ochiul in fisier in timp ce jucati)
    if(!f)
    {
        perror("\nNu s-a putut deschide fisierul pentru scriere!\n");
        exit(1);
    }
    for(int i=0;i<*nr_gloante;i++)
    {
        if(shotgun[i]%2==0) //daca glontul e orb
        {
            fprintf(f,"%d - orb\n",shotgun[i]); //afiseaza glont orb
        }
        else //daca glontul e adevarat
        {
            fprintf(f,"%d - adevarat\n",shotgun[i]); //afiseaza glont adevarat
        }
    }
    fclose(f);
}


int *incarcare_shotgun(int shotgun[], int *nr_gloante_oarbe, int *nr_gloante_adevarate, int *nr_gloante)
{
    shotgun=gloante(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante); //incarca aleatoriu shotgun-ul cu gloante
    while(((*nr_gloante-abs(*nr_gloante_oarbe-*nr_gloante_adevarate))+1)<=abs(*nr_gloante_oarbe-*nr_gloante_adevarate)) //cat timp nu este un balans intre gloantele oarbe si adevarate
    {
        shotgun=gloante(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante); //se rebalanseaza gloantele din shotgun
    }
    afisare_gloante(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante); //afiseaza gloantele din shotgun in fisierul "ordine_gloante"
    return shotgun;
}


void afisare_iteme(jucator *j1, jucator *j2)
{
    FILE *f,*g;
    f=fopen("iteme_jucator1.txt","w+"); //acest fisier este pentru afisarea itemelor jucatorului 1
    if(!f)
    {
        perror("\nNu s-a putut deschide fisierul pentru scriere!\n");
        exit(1);
    }
    for(int i=1;i<=j1->contor;i++)
    {
        fprintf(f,"%s\n",j1->item[i-1].nume); //afiseaza numele item-ului
    }
    fclose(f);
    g=fopen("iteme_jucator2.txt","w+"); //acest fisier este pentru afisarea itemelor jucatorului 2
    if(!g)
    {
        perror("\nNu s-a putut deschide fisierul pentru scriere!\n");
        exit(1);
    }
    for(int i=1;i<=j2->contor;i++)
    {
        fprintf(f,"%s\n",j2->item[i-1].nume); //afiseaza numele item-ului
    }
    fclose(g);
}


char *verificare_item(int item, char *nume_item)
{
    int item_curent=item;
    switch (item_curent) //verifica ce item a ales un jucator
    {
        case 0:
            strcpy(nume_item,"cutit");
            break;
        case 1:
            strcpy(nume_item,"lupa");
            break;
        case 2:
            strcpy(nume_item,"bere");
            break;
        case 3:
            strcpy(nume_item,"catuse");
            break;
        case 4:
            strcpy(nume_item,"tigari");
            break;
        case 5:
            strcpy(nume_item,"invertor");
            break;
        case 6:
            strcpy(nume_item,"celular");
            break;
        case 7:
            strcpy(nume_item,"medicament expirat");
            break;
        case 8:
            strcpy(nume_item,"adrenalina");
            break;
        case 9:
            strcpy(nume_item,"shotgun");
    }
    return nume_item;
}


jucator *impartire_iteme_jucator(jucator *j, int *nr_item)
{
    int new_item;
    for(int i=0;i<*nr_item;i++)
    {
        if(j->contor==8) //daca jucatorul are deja 8 iteme
        {
            break; //nu va mai primi si altele
        }
        else //daca jucatorul are mai putin de 8 iteme
        {
            new_item=rand()%9;
            j->contor++; //incrementam numarul de iteme al jucatorului
            strcpy(j->item[j->contor-1].nume,verificare_item(new_item,j->item[j->contor-1].nume)); //adaugam in tabloul de iteme itemul primit aleatoriu
            if(i>0)
            {
                while(strcmp(j->item[j->contor-1].nume,j->item[j->contor-2].nume)==0) //daca primesti acelasi item de 2 ori la rand
                {
                    new_item=rand()%9;
                    strcpy(j->item[j->contor-1].nume,verificare_item(new_item,j->item[j->contor-1].nume)); //se va schimba cu alt item, ca sa fie cinstit
                }
            }   
        }
    }
    return j;
}


void cutit(int *taiat)
{
    printf("\nUrmatorul glont este de 2 ori mai fatal!\n");
    (*taiat)=1; //daca s-a folosit cutitul
}


void lupa(int shotgun[])
{
    FILE *h;
    h=fopen("derulare_joc.txt","w+"); //acest fisier este pentru afisarea glontului vazut cu lupa sau aflat prin celular
    if(!h)
    {
        perror("\nNu s-a putut deschide fisierul pentru scriere!\n");
        exit(1);
    }
    if(shotgun[0]%2==0) //daca glontul e orb
    {
        fprintf(h,"Glontul curent este orb!\n"); //afiseaza glont orb
    }
    else //daca glontul e adevarat
    {
        fprintf(h,"Glontul curent este adevarat!\n"); //afisaza glont adevarat
    }
    fclose(h);
}


int *bere(int shotgun[], int *nr_gloante_oarbe, int *nr_gloante_adevarate, int *nr_gloante)
{
    if(shotgun[0]%2==0) //daca glontul e orb
    {
        printf("\nGlontul ejectat era orb!\n"); //afiseaza glontul orb
        (*nr_gloante_oarbe)--; //decrementam numarul de gloante oarbe
    }
    else //daca glontul e adevarat
    {
        printf("\nGlontul ejectat era adevarat!\n"); //afiseaza glontul adevarat
        (*nr_gloante_adevarate)--; //decrementam numarul de gloante adevarate
    }
    for(int i=0;i<*nr_gloante-1;i++)
    {
        shotgun[i]=shotgun[i+1]; //mutam restul gloantelor cu o pozitie inapoi
    }
    (*nr_gloante)--; //decrementam numarul de gloante
    return shotgun;
}


void catuse(int *avantaj1, int *avantaj2, int *legat1, int *legat2)
{
    if(*avantaj1==1) //daca e randul jucatorului 1
    {
        printf("\nJucatorul 2 este incatusat o tura!\n");
        (*legat2)=1; //se incatuseaza jucatorul 2
    }
    else //daca e randul jucatorului 2
    {
        printf("\nJucatorul 1 este incatusat o tura!\n");
        (*legat1)=1; //se incatuseaza jucatorul 1
    }
}


void tigari(jucator *j, int *runda)
{
    switch (*runda) //in functie de ce runda e
    {
        case 1:
            if(j->vieti<max_lives_r1) //daca jucatorul are mai putin de 3 vieti
            {
                j->vieti++; //jucatorul primeste o viata
                printf("\nJucatorul si-a recuperat o viata!\n");
                printf("\nJucatorul are %d vieti!\n",j->vieti);
            }
            else
            {
                printf("\nJucatorul are deja numarul maxim de vieti! (%d vieti)\n",j->vieti);
            }
            break;
        case 2:
            if(j->vieti<max_lives_r2) //daca jucatorul are mai putin de 4 vieti
            {
                j->vieti++; //jucatorul primeste o viata
                printf("\nJucatorul si-a recuperat o viata!\n");
                printf("\nJucatorul are %d vieti!\n",j->vieti);

            }
            else
            {
                printf("\nJucatorul are deja numarul maxim de vieti! (%d vieti)\n",j->vieti);
            }
            break;
        case 3:
            if(j->vieti<max_lives_r3) //daca jucatorul are mai putin de 6 vieti
            {
                j->vieti++; //jucatorul primeste o viata
                printf("\nJucatorul si-a recuperat o viata!\n");
                printf("\nJucatorul are %d vieti!\n",j->vieti);
            }
            else
            {
                printf("\nJucatorul are deja numarul maxim de vieti! (%d vieti)\n",j->vieti);
            }
            break;
    }
}


int *invertor(int shotgun[], int *nr_gloante_oarbe, int *nr_gloante_adevarate)
{
    if(shotgun[0]%2==0) //daca glontul curent este orb
    {
        shotgun[0]=1; //devine adevarat
        (*nr_gloante_oarbe)--; //decrementam numarul de gloante oarbe
        (*nr_gloante_adevarate)++; //incrementam numarul de gloante adevarate
    }
    else //daca glontul curent este adevarat
    {
        shotgun[0]=0; //devine orb
        (*nr_gloante_adevarate)--; //decrementam numarul de gloante adevarate
        (*nr_gloante_oarbe)++; //incrementam numarul de gloante oarbe
    }
    printf("\nGlontul curent a fost inversat!\n");
    return shotgun;
}


void celular(int shotgun[], int *nr_gloante_oarbe, int *nr_gloante_adevarate, int *nr_gloante)
{
    FILE *h;
    h=fopen("derulare_joc.txt","w+"); //acest fisier este pentru afisarea glontului vazut cu lupa sau aflat prin celular
    if(!h)
    {
        perror("\nNu s-a putut deschide fisierul pentru scriere!\n");
        exit(1);
    }
    int glont=(rand()%(*nr_gloante))+1;
    for(int i=1;i<=*nr_gloante;i++)
    {
        if(glont==i) //daca glontul din shotgun corespunde cu cel ales aleatoriu
        {
            if(shotgun[i-1]%2==0) //daca glontul e orb
            {
                fprintf(h,"Glontul %d este orb!\n",glont); //afiseaza glont orb si al catelea e in shotgun
            }
            else //daca glontul e adevarat
            {
                fprintf(h,"Glontul %d este adevarat!\n",glont); //afiseaza glont adevarat si al catelea e in shotgun
            }
        }
    }
    fclose(h);
}


void medicament_expirat(jucator *j, int *runda)
{
    int noroc=rand()%6; //sanse de 50/50 sa fie bine sau rau
    if(noroc%2==1) //daca e bine
    {
        switch (*runda) //in functie de ce runda e
        {
            case 1:
                if(j->vieti<max_lives_r1-1) //daca jucatorul are mai putin de 2 vieti
                {
                    j->vieti=j->vieti+2; //primeste 2 vieti
                    printf("\nJucatorul si-a recuperat 2 vieti!\n");
                    printf("\nJucatorul are %d vieti!\n",j->vieti);
                }
                break;
            case 2:
                if(j->vieti<max_lives_r2-1) //daca jucatorul are mai putin de 3 vieti
                {
                    j->vieti=j->vieti+2; //primeste 2 vieti
                    printf("\nJucatorul si-a recuperat 2 vieti!\n");
                    printf("\nJucatorul are %d vieti!\n",j->vieti);
                }
                break;
            case 3:
                if(j->vieti<max_lives_r3-1) //daca jucatorul are mai putin de 5 vieti
                {
                    j->vieti=j->vieti+2; //primeste 2 vieti
                    printf("\nJucatorul si-a recuperat 2 vieti!\n");
                    printf("\nJucatorul are %d vieti!\n",j->vieti);
                }
                break;
        }
    }
    else //daca e rau
    {
        j->vieti--; //jucatorul pierde o viata
        printf("\nJucatorul a pierdut o viata!\n");
        printf("\nJucatorul are %d vieti!\n",j->vieti);
    }
}


int adrenalina(jucator *j, jucator *juc, int *avantaj1, int *avantaj2)
{
    int alegere; //alegere item
    char ales[20]; //item ales
    int ok; //verifica daca jucatorul are item-ul care vrea sa fie furat 
    printf("\nItem-ul pe care vrei sa-l furi = ");
    scanf("%d",&alegere); //alege ce item furi
    while(alegere<0 || alegere>8) //cat timp alegi un numar invalid
    {
        printf("\nFura un item valid!\n");
        printf("\nItem-ul pe care vrei sa-l furi = ");
        scanf("%d",&alegere); //alege ce item furi
    }
    if(alegere>=0 && alegere<=8) //daca s-a ales un item valid
    {
        ok=0; //presupunem ca nu are item-ul ales
        if(alegere==8) //daca s-a ales adrenalina
        {
            while(alegere==8) //cat timp este aleasa adrenalina
            {
                printf("\nNu poti fura adrenalina!\n");
                printf("\nItem-ul pe care vrei sa-l furi = ");
                scanf("%d",&alegere); //alege ce item furi
            }
        }
        strcpy(ales,verificare_item(alegere,ales)); //se verifica item-ul ales
        while(ok==0) //cat timp adversarul nu are item-ul ales 
        {
            for(int i=1;i<=juc->contor;i++)
            {
                if(strcmp(ales,juc->item[i-1].nume)!=0) //verifica daca jucatorul are item-ul
                {
                    continue; //daca nu il are, se continua
                }
                else if(ok==0) //daca il are 
                {
                    ok=1; //e verificat ca il are
                    for(int k=i-1;k<juc->contor-1;k++)
                    {
                        strcpy(juc->item[k].nume,juc->item[k+1].nume); //se sterge item-ul din lista de iteme
                    }
                    juc->contor--; //decrementam numarul de iteme
                }
            }
            if(ok==0) //daca nu il are deloc
            {
                printf("\nFura un item valid!\n");
                printf("\nItem-ul pe care vrei sa-l furi = ");
                scanf("%d",&alegere); //alege ce item furi
                ok=0;  //presupunem ca nu are item-ul ales
                strcpy(ales,verificare_item(alegere,ales)); //se verifica item-ul ales
            }
        }
        if(*avantaj1==1) //daca jucatorul 1 a furat un item
        {
            afisare_iteme(j,juc); //se reafiseaza itemele dupa ce s-a furat de la jucatorul 2
        }
        else //daca jucatorul 2 a furat un item
        {
            afisare_iteme(juc,j); //se reafiseaza itemele dupa ce s-a furat de la jucatorul 1
        }
        printf("\nJucatorul a furat %s!\n",ales);
    }
    return alegere;
}


void glont_shotgun(jucator *j, jucator *juc, int *taiat, int *scapat, int shotgun[],int *nr_gloante_oarbe, int *nr_gloante_adevarate, int *nr_gloante, int *runda, int *nr_item)
{
    if(*nr_gloante!=0) //daca shotgun-ul nu e gol
    {
        printf("\nPe cine impusti?\n");
        printf("\nEu (cod 0)\n");
        printf("\nAdversar (cod 1)\n");
        int decizie; //cine va fi impuscat
        printf("\nCod = ");
        scanf("%d",&decizie); //alege cine va fi impuscat
        while(decizie!=0 && decizie!=1) //cat timp alegi un numar invalid
        {
            printf("\nAlege un cod valid!\n");
            printf("\nCod = ");
            scanf("%d",&decizie); //alege cine va fi impuscat
        }
        if(decizie==0) //daca jucatorul se impusca pe el insusi
        {
            printf("\nJucatorul se impusca pe el insusi!\n");
            Sleep(somn);
            if(shotgun[0]==1) //daca glontul e adevarat 
            {
                if(*taiat==1) //daca a fost folosit cutitul
                {
                    printf("\nJucatorul a pierdut 2 vieti!\n");
                    j->vieti=j->vieti-2; //decrementam numarul de vieti cu 2
                    (*nr_gloante_adevarate)--; //decrementam numarul de gloante adevarate
                    Sleep(somn);
                    printf("\nJucatorul are %d vieti!\n",j->vieti);
                }
                else //daca nu
                {
                    printf("\nJucatorul a pierdut o viata!\n");
                    j->vieti=j->vieti-1; //decrementam numarul de vieti cu 1
                    (*nr_gloante_adevarate)--; //decrementam numarul de gloante adevarate
                    Sleep(somn);
                    printf("\nJucatorul are %d vieti!\n",j->vieti);
                }
                for(int i=0;i<*nr_gloante-1;i++)
                {
                    shotgun[i]=shotgun[i+1]; //mutam restul gloantelor cu o pozitie inapoi
                }
                (*nr_gloante)--; //decrementam numarul de gloante
            }
            else
            {
                printf("\nJucatorul a scapat...momentan!\n");
                (*scapat)++; //ramane randul aceluiasi jucator
                (*nr_gloante_oarbe)--; //decrementam numarul de gloante oarbe
                for(int i=0;i<*nr_gloante-1;i++)
                {
                    shotgun[i]=shotgun[i+1]; //mutam restul gloantelor cu o pozitie inapoi
                }
                (*nr_gloante)--; //decrementam numarul de gloante
                Sleep(somn);
                printf("\nJucatorul are %d vieti!\n",j->vieti);
            }
        }
        else //daca jucatorul se impusca adversarul
        {
            printf("\nJucatorul impusca adversarul!\n");
            Sleep(somn);
            if(shotgun[0]==1) //daca glontul e adevarat 
            {
                if(*taiat==1) //daca a fost folosit cutitul
                {
                    printf("\nAdversarul a pierdut 2 vieti!\n");
                    j->vieti=j->vieti-2; //decrementam numarul de vieti cu 2
                    (*nr_gloante_adevarate)--; //decrementam numarul de gloante adevarate
                    Sleep(somn);
                    printf("\nAdversarul are %d vieti!\n",j->vieti);
                }
                else //daca nu
                {
                    printf("\nAdversarul a pierdut o viata!\n");
                    j->vieti=j->vieti-1; //decrementam numarul de vieti cu 1
                    (*nr_gloante_adevarate)--; //decrementam numarul de gloante adevarate
                    Sleep(somn);
                    printf("\nAdversarul are %d vieti!\n",j->vieti);
                }
                for(int i=0;i<*nr_gloante-1;i++)
                {
                    shotgun[i]=shotgun[i+1]; //mutam restul gloantelor cu o pozitie inapoi
                }
                (*nr_gloante)--; //decrementam numarul de gloante
            }
            else
            {
                printf("\nAdversarul a scapat...momentan!\n");
                (*scapat)++; //ramane randul aceluiasi jucator
                (*nr_gloante_oarbe)--; //decrementam numarul de gloante oarbe
                for(int i=0;i<*nr_gloante-1;i++)
                {
                    shotgun[i]=shotgun[i+1]; //mutam restul gloantelor cu o pozitie inapoi
                }
                (*nr_gloante)--; //decrementam numarul de gloante
                Sleep(somn);
                printf("\nAdversarul are %d vieti!\n",j->vieti);
            }
        }
        afisare_gloante(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante); //afiseaza gloantele din shotgun in fisierul "ordine_gloante"
    }
    else
    {
        switch (*runda) //in functie de ce runda e
        {
            case 1:
                *nr_gloante=(rand()%gloante_shotgun)+2; //numar de gloante intre 2 si 4 pentru shotgun
                break;
            case 2:
                *nr_gloante=(rand()%gloante_shotgun)+4; //numar de gloante intre 4 si 6 pentru shotgun
                break;
            case 3:
                *nr_gloante=(rand()%gloante_shotgun)+6; //numar de gloante intre 6 si 8 pentru shotgun
                break;
        }
        printf("\nSe reincarca shotgun-ul...\n");
        shotgun=incarcare_shotgun(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante); //se incarca shotgun-ul
        Sleep(somn);
        printf("\nNumarul de gloante = %d\n",*nr_gloante); //afiseaza numarul de gloante din shotgun
        printf("Numarul de gloante oarbe = %d\n",*nr_gloante_oarbe); //afiseaza numarul de gloante adevarate din shotgun
        printf("Numarul de gloante adevarate = %d\n",*nr_gloante_adevarate); //afiseaza numarul de gloante oarbe din shotgun
        printf("\nSe impart iteme...\n");
        j=impartire_iteme_jucator(j,nr_item); //imparte iteme la jucatorul 1
        juc=impartire_iteme_jucator(juc,nr_item); //imparte iteme la jucatorul 2
        afisare_iteme(j,juc); //afiseaza itemele celor 2 jucatori
        Sleep(somn);
    }
}


void alegere(int aleg, int *taiat, int *scapat, int shotgun[],int *nr_gloante_oarbe, int *nr_gloante_adevarate, int *nr_gloante, jucator *j, item *it, int *runda, jucator *juc, int *avantaj1, int *avantaj2, int *legat1, int *legat2, int *nr_item)
{
    switch (aleg) //in functie de item-ul ales
    {
        case 0:
            cutit(taiat);
            Sleep(somn);
            break;
        case 1:
            lupa(shotgun);
            Sleep(somn);
            break;
        case 2:
            shotgun=bere(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante);
            afisare_gloante(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante); //afiseaza gloantele din shotgun in fisierul "ordine_gloante"
            Sleep(somn);
            break;
        case 3:
            catuse(avantaj1,avantaj2,legat1,legat2);
            Sleep(somn);
            break;
        case 4:
            tigari(j,runda);
            Sleep(somn);
            break;
        case 5:
            shotgun=invertor(shotgun,nr_gloante_oarbe,nr_gloante_adevarate);
            afisare_gloante(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante); //afiseaza gloantele din shotgun in fisierul "ordine_gloante"
            Sleep(somn);
            break;
        case 6:
            celular(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante);
            Sleep(somn);
            break;
        case 7:
            medicament_expirat(j,runda);
            Sleep(somn);
            break;
        case 8:
            aleg=adrenalina(j,juc,avantaj1,avantaj2);
            Sleep(somn);
            alegere(aleg,taiat,scapat,shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante,j,it,runda,juc,avantaj1,avantaj2,legat1,legat2,nr_item);
            break;
        case 9:
            glont_shotgun(j,juc,taiat,scapat,shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante,runda,nr_item);
            Sleep(somn);
            break;
    }
}


void selecteaza_item(int shotgun[], jucator *j, item *it, int *nr_gloante_oarbe, int *nr_gloante_adevarate, int *nr_gloante, int *runda, jucator *juc, int *avantaj1, int *avantaj2, int *legat1, int *legat2, int *nr_item)
{
    if(*nr_gloante==0) //daca shotgun-ul e gol
    {
        switch (*runda) //in functie de ce runda e
        {
            case 1:
                *nr_gloante=(rand()%gloante_shotgun)+2; //numar de gloante intre 2 si 4 pentru shotgun
                break;
            case 2:
                *nr_gloante=(rand()%gloante_shotgun)+4; //numar de gloante intre 4 si 6 pentru shotgun
                break;
            case 3:
                *nr_gloante=(rand()%gloante_shotgun)+6; //numar de gloante intre 6 si 8 pentru shotgun
                break;
        }
        printf("\nSe reincarca shotgun-ul...\n");
        shotgun=incarcare_shotgun(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante); //se incarca shotgun-ul
        Sleep(somn);
        printf("\nNumarul de gloante = %d\n",*nr_gloante); //afiseaza numarul de gloante din shotgun
        printf("Numarul de gloante oarbe = %d\n",*nr_gloante_oarbe); //afiseaza numarul de gloante adevarate din shotgun
        printf("Numarul de gloante adevarate = %d\n",*nr_gloante_adevarate); //afiseaza numarul de gloante oarbe din shotgun
        printf("\nSe impart iteme...\n");
        j=impartire_iteme_jucator(j,nr_item); //imparte iteme aleatoriu la jucatorul 1
        juc=impartire_iteme_jucator(juc,nr_item); //imparte iteme aleatoriu la jucatorul 2
        afisare_iteme(j,juc); //afiseaza itemele celor 2 jucatori
        Sleep(somn);
    }
    printf("\nSelecteaza item:\n");
    int aleg; //alegere item
    char ales[20]; //item ales
    int ok; //verifica daca jucatorul are item-ul pe care vrea sa-l foloseasca
    int taiat=0; //daca jucatorul
    int scapat=0; //daca jucatorul s-a impuscat singur si a scapat
    printf("\nItem = ");
    scanf("%d",&aleg); //alege ce item folosesti
    while(aleg<0 || aleg>9) //cat timp alegi un numar invalid
    {
        printf("\nAlege un item valid!\n");
        printf("\nItem = ");
        scanf("%d",&aleg); //alege ce item folosesti
    }
    if(aleg!=9) //daca nu s-a ales shotgun-ul
    {
        while(aleg>=0 && aleg<=8) //daca s-a ales un item valid
        {
            ok=0; //presupunem ca nu are item-ul ales
            strcpy(ales,verificare_item(aleg,ales)); //se verifica item-ul ales
            while(ok==0) //cat timp jucatorul nu are item-ul ales 
            {
                for(int i=1;i<=j->contor;i++)
                {
                    if(strcmp(ales,j->item[i-1].nume)!=0) //daca jucatorul nu are item-ul ales
                    {
                        continue; //daca nu il are, se continua
                    }
                    else //daca il are
                    {
                        ok=1; //e verificat ca il are
                    }
                }
                if(ok==0) //daca nu il are deloc
                {
                    printf("\nAlege un item valid!\n");
                    printf("\nItem = ");
                    scanf("%d",&aleg); //alege ce item folosesti
                    ok=0;  //presupunem ca nu are item-ul ales
                    strcpy(ales,verificare_item(aleg,ales)); //se verifica item-ul ales
                }
            }
            printf("\nJucatorul a ales %s!\n",ales);
            Sleep(somn);
            for(int i=1;i<=j->contor;i++)
            {
                if(strcmp(ales,j->item[i-1].nume)!=0) //daca item-ul ales nu este gasit
                {
                    continue; //se continua
                }
                else //daca e gasit
                {
                    for(int k=i-1;k<j->contor-1;k++)
                    {
                        strcpy(j->item[k].nume,j->item[k+1].nume); //se sterge item-ul din lista de iteme
                    }
                    j->contor--; //decrementam numarul de iteme
                    break;
                }
            }
            if(*avantaj1==1) //daca jucatorul 1 a folosit un item
            {
                afisare_iteme(j,juc); //se reafiseaza itemele dupa ce jucatorul 2 a folosit un item
            }
            else //daca jucatorul 2 a folosit un item
            {
                afisare_iteme(juc,j); //se reafiseaza itemele dupa ce jucatorul 1 a folosit un item
            }
            alegere(aleg,&taiat,&scapat,shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante,j,it,runda,juc,avantaj1,avantaj2,legat1,legat2,nr_item); //se foloseste item-ul ales
            printf("\nSelecteaza item:\n");
            printf("\nItem = ");
            scanf("%d",&aleg); //alege ce item folosesti
            while(aleg<0 || aleg>9) //cat timp alegi un numar invalid
            {
                printf("\nAlege un item valid!\n");
                printf("\nItem = ");
                scanf("%d",&aleg); //alege ce item folosesti
            }
        }
    }
    strcpy(ales,verificare_item(aleg,ales)); //se alege shotgun-ul
    printf("\nJucatorul a ales %s!\n",ales);
    Sleep(somn);
    alegere(aleg,&taiat,&scapat,shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante,j,it,runda,juc,avantaj1,avantaj2,legat1,legat2,nr_item); //se foloseste shotgun-ul
    if(*nr_gloante==0) //daca shotgun-ul e gol
    {
        switch (*runda) //in functie de ce runda e
        {
            case 1:
                *nr_gloante=(rand()%gloante_shotgun)+2; //numar de gloante intre 2 si 4 pentru shotgun
                break;
            case 2:
                *nr_gloante=(rand()%gloante_shotgun)+4; //numar de gloante intre 4 si 6 pentru shotgun
                break;
            case 3:
                *nr_gloante=(rand()%gloante_shotgun)+6; //numar de gloante intre 6 si 8 pentru shotgun
                break;
        }
        printf("\nSe reincarca shotgun-ul...\n");
        shotgun=incarcare_shotgun(shotgun,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante); //se incarca shotgun-ul
        Sleep(somn);
        printf("\nNumarul de gloante = %d\n",*nr_gloante); //afiseaza numarul de gloante din shotgun
        printf("Numarul de gloante oarbe = %d\n",*nr_gloante_oarbe); //afiseaza numarul de gloante adevarate din shotgun
        printf("Numarul de gloante adevarate = %d\n",*nr_gloante_adevarate); //afiseaza numarul de gloante oarbe din shotgun
        printf("\nSe impart iteme...\n");
        j=impartire_iteme_jucator(j,nr_item); //imparte iteme la jucatorul 1
        juc=impartire_iteme_jucator(juc,nr_item); //imparte iteme la jucatorul 2
        afisare_iteme(j,juc); //afiseaza itemele celor 2 jucatori
        Sleep(somn);
    }
    if(scapat==1) //daca jucatorul se impusca singur cu un glont orb
    {
        selecteaza_item(shotgun,j,it,nr_gloante_oarbe,nr_gloante_adevarate,nr_gloante,runda,juc,avantaj1,avantaj2,legat1,legat2,nr_item); //jucatorul selecteaza din nou item
    }
}


int main()
{
    int runda=0; //runda
    int scor1=0; //scor jucator 1
    int scor2=0; //scor jucator 2
    while(runda<3) //cat timp nu s-au jucat 3 runde
    {
        runda++; //runda curenta
        int nr_item; //numarul de iteme pe care jucatorii il vor primi
        int nr_gloante; //numarul de gloante din shotgun (aleatoriu)
        int nr_gloante_oarbe=0; //numarul de gloante oarbe din shotgun
        int nr_gloante_adevarate=0; //numarul de gloante adevarate din shotgun
        printf("\nRUNDA %d\n",runda);
        jucator *j1=(jucator*)malloc(sizeof(jucator)); //se aloca memorie pentru pointerul jucator1
        if(!j1)
        {
            perror("\nNu s-a putut aloca memorie!\n");
            exit(2);
        }
        jucator *j2=(jucator*)malloc(sizeof(jucator)); //se aloca memorie pentru pointerul jucator2
        if(!j2)
        {
            perror("\nNu s-a putut aloca memorie!\n");
            exit(2);
        }
        item *it=(item*)malloc(max_item*sizeof(item)); //se aloca memorie pentru pointerul item
        if(!it)
        {
            perror("\nNu s-a putut aloca memorie!\n");
            exit(2);
        }
        srand(time(0));
        switch (runda) //daca e prima runda
        {
            case 1:
                j1->vieti=max_lives_r1; //3 vieti
                j1->contor=0;
                j2->vieti=max_lives_r1; //3 vieti
                j2->contor=0;
                nr_item=max_item_r1; //jucatorii vor primi cate 2 iteme
                nr_gloante=(rand()%gloante_shotgun)+2; //numar de gloante intre 2 si 4 pentru shotgun
                break;
            case 2:
                j1->vieti=max_lives_r2; //4 vieti
                j1->contor=0;
                j2->vieti=max_lives_r2; //4 vieti
                j2->contor=0;
                nr_item=max_item_r2; //jucatorii vor primi cate 3 iteme
                nr_gloante=(rand()%gloante_shotgun)+4; //numar de gloante intre 4 si 6 pentru shotgun
                break;
            case 3:
                j1->vieti=max_lives_r3; //6 vieti
                j1->contor=0;
                j2->vieti=max_lives_r3; //6 vieti
                j2->contor=0;
                nr_item=max_item_r3; //jucatorii vor primi cate 4 iteme
                nr_gloante=(rand()%gloante_shotgun)+6; //numar de gloante intre 6 si 8 pentru shotgun
                break;
        }
        Sleep(somn);
        int *shotgun=(int*)malloc(nr_gloante*sizeof(int)); //se aloca memorie pentru pointerul shotgun
        if(!shotgun)
        {
            perror("\nNu s-a putut aloca memorie!\n");
            exit(2);
        }
        shotgun=incarcare_shotgun(shotgun,&nr_gloante_oarbe,&nr_gloante_adevarate,&nr_gloante); //se incarca shotgun-ul
        printf("\nNumarul de gloante = %d\n",nr_gloante); //afiseaza numarul de gloante din shotgun
        printf("Numarul de gloante oarbe = %d\n",nr_gloante_oarbe); //afiseaza numarul de gloante adevarate din shotgun
        printf("Numarul de gloante adevarate = %d\n",nr_gloante_adevarate); //afiseaza numarul de gloante oarbe din shotgun
        Sleep(somn);
        printf("\nSe impart iteme...\n");
        j1=impartire_iteme_jucator(j1,&nr_item); //imparte iteme la jucatorul 1
        j2=impartire_iteme_jucator(j2,&nr_item); //imparte iteme la jucatorul 2
        afisare_iteme(j1,j2); //afiseaza itemele celor 2 jucatori
        Sleep(somn);
        int juc1=rand()%2;
        int juc2=rand()%2;
        int decizie=rand()%2;
        int avantaj1=0; //daca va fi randul jucatorului 1
        int avantaj2=0; //daca va fi randul jucatorului 2
        int legat1=0; //daca jucatorul 1 va fi legat cu catuse
        int legat2=0; //daca jucatorul 2 va fi legat cu catuse
        printf("\nSe decide cine incepe...\n");
        while(juc1==juc2) //se decide aleatoriu cine incepe
        {
            juc1=rand()%2;
            juc2=rand()%2;
        }
        Sleep(somn);
        if(decizie==juc1)
        {
            avantaj1=1; //randul jucatorului 1
        }
        else
        {
            avantaj2=1; //randul jucatorului 2
        }
        if(avantaj1==1) //incepe jucatorul 1
        {
            printf("\nIncepe jucatorul 1\n");
            while(j1->vieti>0 && j2->vieti>0) //cat timp niciun jucator nu e mort
            {
                if(legat1==0) //daca jucatorul 1 nu a fost incatusat
                {
                    avantaj1=1; //este randul jucatorului 1
                    avantaj2=0; //a trecut randul jucatorului 2
                    if(j1->vieti>0) //daca jucatorul 1 nu e mort
                    {
                        printf("\nJucatorul 1 alege item!\n");
                        selecteaza_item(shotgun,j1,it,&nr_gloante_oarbe,&nr_gloante_adevarate,&nr_gloante,&runda,j2,&avantaj1,&avantaj2,&legat1,&legat2,&nr_item); //jucatorul 1 selecteaza item
                    }
                }
                legat1=0; //in caz ca a fost incatusat o tura, efectul dispare
                if(nr_gloante==0 && (j1->vieti!=0 && j2->vieti!=0)) //daca shotgun-ul e gol si niciun jucator nu e mort
                {
                    switch (runda) //in functie de ce runda e
                    {
                        case 1:
                            nr_gloante=(rand()%gloante_shotgun)+2; //numar de gloante intre 2 si 4 pentru shotgun
                            break;
                        case 2:
                            nr_gloante=(rand()%gloante_shotgun)+4; //numar de gloante intre 4 si 6 pentru shotgun
                            break;
                        case 3:
                            nr_gloante=(rand()%gloante_shotgun)+6; //numar de gloante intre 6 si 8 pentru shotgun
                            break;
                    }
                    printf("\nSe reincarca shotgun-ul...\n");
                    shotgun=incarcare_shotgun(shotgun,&nr_gloante_oarbe,&nr_gloante_adevarate,&nr_gloante); //reincarcam shotgun-ul
                    Sleep(somn);
                    printf("\nNumarul de gloante = %d\n",nr_gloante); //afiseaza numarul de gloante din shotgun
                    printf("Numarul de gloante oarbe = %d\n",nr_gloante_oarbe); //afiseaza numarul de gloante adevarate din shotgun
                    printf("Numarul de gloante adevarate = %d\n",nr_gloante_adevarate); //afiseaza numarul de gloante oarbe din shotgun
                    printf("\nSe impart iteme...\n");
                    j1=impartire_iteme_jucator(j1,&nr_item); //imparte iteme aleatoriu la jucatorul 1
                    j2=impartire_iteme_jucator(j2,&nr_item); //imparte iteme aleatoriu la jucatorul 2
                    afisare_iteme(j1,j2); //afiseaza itemele celor 2 jucatori
                    Sleep(somn);
                }
                if(legat2==0) //daca jucatorul 2 nu a fost incatusat
                {
                    avantaj1=0; //a trecut randul jucatorului 1
                    avantaj2=1; //este randul jucatorului 2
                    if(j2->vieti>0) //daca jucatorul 2 nu e mort
                    {
                        printf("\nJucatorul 2 alege item!\n");
                        selecteaza_item(shotgun,j2,it,&nr_gloante_oarbe,&nr_gloante_adevarate,&nr_gloante,&runda,j1,&avantaj1,&avantaj2,&legat1,&legat2,&nr_item); //jucatorul 2 selecteaza item
                    }
                }
                legat2=0; //in caz ca a fost incatusat o tura, efectul dispare
                if(nr_gloante==0 && (j1->vieti!=0 && j2->vieti!=0)) //daca shotgun-ul e gol si niciun jucator nu e mort
                {
                    switch (runda) //in functie de ce runda e
                    {
                        case 1:
                            nr_gloante=(rand()%gloante_shotgun)+2; //numar de gloante intre 2 si 4 pentru shotgun
                            break;
                        case 2:
                            nr_gloante=(rand()%gloante_shotgun)+4; //numar de gloante intre 4 si 6 pentru shotgun
                            break;
                        case 3:
                            nr_gloante=(rand()%gloante_shotgun)+6; //numar de gloante intre 6 si 8 pentru shotgun
                            break;
                    }
                    printf("\nSe reincarca shotgun-ul...\n");
                    shotgun=incarcare_shotgun(shotgun,&nr_gloante_oarbe,&nr_gloante_adevarate,&nr_gloante); //reincarcam shotgun-ul
                    Sleep(somn);
                    printf("\nNumarul de gloante = %d\n",nr_gloante); //afiseaza numarul de gloante din shotgun
                    printf("Numarul de gloante oarbe = %d\n",nr_gloante_oarbe); //afiseaza numarul de gloante adevarate din shotgun
                    printf("Numarul de gloante adevarate = %d\n",nr_gloante_adevarate); //afiseaza numarul de gloante oarbe din shotgun
                    printf("\nSe impart iteme...\n");
                    j1=impartire_iteme_jucator(j1,&nr_item); //imparte iteme la jucatorul 1
                    j2=impartire_iteme_jucator(j2,&nr_item); //imparte iteme la jucatorul 2
                    afisare_iteme(j1,j2); //afiseaza itemele celor 2 jucatori
                    Sleep(somn);
                }
            }
        }
        else //incepe jucatorul 2
        {
            printf("\nIncepe jucatorul 2\n");
            Sleep(somn);
            while(j1->vieti>0 && j2->vieti>0) //cat timp niciun jucator nu e mort
            {
                if(legat2==0) //daca jucatorul 2 nu a fost incatusat
                {
                    avantaj1=0; //a trecut randul jucatorului 1
                    avantaj2=1; //este randul jucatorului 2
                    if(j2->vieti>0) //daca jucatorul 2 nu e mort
                    {
                    printf("\nJucatorul 2 alege item!\n");
                    selecteaza_item(shotgun,j2,it,&nr_gloante_oarbe,&nr_gloante_adevarate,&nr_gloante,&runda,j1,&avantaj1,&avantaj2,&legat1,&legat2,&nr_item); //jucatorul 2 selecteaza item
                    }
                }
                legat2=0; //in caz ca a fost incatusat o tura, efectul dispare
                if(nr_gloante==0 && (j1->vieti!=0 && j2->vieti!=0)) //daca shotgun-ul e gol si niciun jucator nu e mort
                {
                    switch (runda) //in functie de ce runda e
                    {
                        case 1:
                            nr_gloante=(rand()%gloante_shotgun)+2; //numar de gloante intre 2 si 4 pentru shotgun
                            break;
                        case 2:
                            nr_gloante=(rand()%gloante_shotgun)+4; //numar de gloante intre 4 si 6 pentru shotgun
                            break;
                        case 3:
                            nr_gloante=(rand()%gloante_shotgun)+6; //numar de gloante intre 6 si 8 pentru shotgun
                            break;
                    }
                    printf("\nSe reincarca shotgun-ul...\n");
                    shotgun=incarcare_shotgun(shotgun,&nr_gloante_oarbe,&nr_gloante_adevarate,&nr_gloante); //reincarcam shotgun-ul
                    Sleep(somn);
                    printf("\nNumarul de gloante = %d\n",nr_gloante); //afiseaza numarul de gloante din shotgun
                    printf("Numarul de gloante oarbe = %d\n",nr_gloante_oarbe); //afiseaza numarul de gloante adevarate din shotgun
                    printf("Numarul de gloante adevarate = %d\n",nr_gloante_adevarate); //afiseaza numarul de gloante oarbe din shotgun
                    printf("\nSe impart iteme...\n");
                    j1=impartire_iteme_jucator(j1,&nr_item); //imparte iteme la jucatorul 1
                    j2=impartire_iteme_jucator(j2,&nr_item); //imparte iteme la jucatorul 2
                    afisare_iteme(j1,j2); //afiseaza itemele celor 2 jucatori
                    Sleep(somn);
                }
                if(legat1==0) //daca jucatorul 1 nu a fost incatusat
                {
                    avantaj1=1; //este randul jucatorului 1
                    avantaj2=0; //a trecut randul jucatorului 2
                    if(j1->vieti>0) //daca jucatorul 1 nu e mort
                    {
                        printf("\nJucatorul 1 alege item!\n");
                        selecteaza_item(shotgun,j1,it,&nr_gloante_oarbe,&nr_gloante_adevarate,&nr_gloante,&runda,j2,&avantaj1,&avantaj2,&legat1,&legat2,&nr_item); //jucatorul 1 selecteaza item
                    }
                }
                legat1=0; //in caz ca a fost incatusat o tura, efectul dispare
                if(nr_gloante==0 && (j1->vieti!=0 && j2->vieti!=0)) //daca shotgun-ul e gol si niciun jucator nu e mort
                {
                    switch (runda) //in functie de ce runda e
                    {
                        case 1:
                            nr_gloante=(rand()%gloante_shotgun)+2; //numar de gloante intre 2 si 4 pentru shotgun
                            break;
                        case 2:
                            nr_gloante=(rand()%gloante_shotgun)+4; //numar de gloante intre 4 si 6 pentru shotgun
                            break;
                        case 3:
                            nr_gloante=(rand()%gloante_shotgun)+6; //numar de gloante intre 6 si 8 pentru shotgun
                            break;
                    }
                    printf("\nSe reincarca shotgun-ul...\n");
                    shotgun=incarcare_shotgun(shotgun,&nr_gloante_oarbe,&nr_gloante_adevarate,&nr_gloante); //reincarcam shotgun-ul
                    Sleep(somn);
                    printf("\nNumarul de gloante = %d\n",nr_gloante); //afiseaza numarul de gloante din shotgun
                    printf("Numarul de gloante oarbe = %d\n",nr_gloante_oarbe); //afiseaza numarul de gloante adevarate din shotgun
                    printf("Numarul de gloante adevarate = %d\n",nr_gloante_adevarate); //afiseaza numarul de gloante oarbe din shotgun
                    printf("\nSe impart iteme...\n");
                    j1=impartire_iteme_jucator(j1,&nr_item); //imparte iteme la jucatorul 1
                    j2=impartire_iteme_jucator(j2,&nr_item); //imparte iteme la jucatorul 2
                    afisare_iteme(j1,j2); //afiseaza itemele celor 2 jucatori
                    Sleep(3000);
                }
            }
        }   
        if(j1->vieti==0) //daca jucatorul 1 nu mai are nicio viata
        {
            printf("\nJucatorul 1 a murit!\n");
            Sleep(somn);
            printf("\nJucatorul 2 a castigat runda!\n");
            scor2++; //un punct pentru jucator 2
        }
        else //daca jucatorul 2 nu mai are nicio viata
        {
            printf("\nJucatorul 2 a murit!\n");
            Sleep(somn);
            printf("\nJucatorul 1 a castigat runda!\n");
            scor1++; //un punct pentru jucatorul 1
        }
        printf("\nRUNDA %d s-a incheiat!\n",runda);
        Sleep(somn);
        free(j1); //se elibereaza memoria pointerului jucator1
        free(j2); //se elibereaza memoria pointerului jucator2
        free(it); //se elibereaza memoria pointerului item
        free(shotgun); //se elibereaza memoria pointerului shotgun
    }
    printf("\nScor final: %d - %d",scor1,scor2); //scor final
    Sleep(somn);
    if(scor1>scor2) //daca jucatorul 1 a castigat mai multe runde
    {
        printf("\nJucatorul 1 a castigat!\n");
    }
    else //daca jucatorul 2 a castigat mai multe runde
    {
        printf("\nJucatorul 2 a castigat!\n");
    }
    Sleep(somn);
    return 0;
}
