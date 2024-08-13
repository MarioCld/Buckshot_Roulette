#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>

#define balles_fusil_de_chasse 3
#define max_objet_r1 2
#define max_objet_r2 3
#define max_objet_r3 4
#define max_objet 8
#define max_vies_r1 3
#define max_vies_r2 4
#define max_vies_r3 6
#define dormir 1000 //une pause d’une seconde entre chaque interaction


typedef struct objet //typedef objet
{
    char nom[20]; //le nom de l’objet
}objet;

typedef struct joueur //typedef joueur
{
    int vies; //le nombre de vies du joueur
    int compteur; //le nombre d’ojets du joueur 
    struct objet objet[max_objet]; //tableau d’objets
}joueur;


int *balles(int fusil_de_chasse[], int *no_balles_a_blanc, int *no_balles_reelles, int *no_balles)
{
    *no_balles_a_blanc=0;
    *no_balles_reelles=0;
    for(int i=0;i<*no_balles;i++)
    {
        fusil_de_chasse[i]=rand()%2; //on ajoute au hasard une balle a blanc ou une balle reelle
        if(fusil_de_chasse[i]==0) //si la balle est a blanc 
        {
            (*no_balles_a_blanc)++; //on augmente le nombre de balles a blanc 
        }
        else //si la balle est reelle
        {
            (*no_balles_reelles)++; //on augmente le nombre de balles reelles
        }
    }
    return fusil_de_chasse;
}


void afficher_balles(int fusil_de_chasse[], int *no_balles_a_blanc, int *no_balles_reelles, int *no_balles)
{
    FILE *f;
    f=fopen("ordre_balles.txt","w+"); //ce fichier sert a afficher l’ordre et le type des balles dans le fusil (ne pas regarder dans le fichier pendant le jeu)
    if(!f)
    {
        perror("\nImpossible d’ouvrir le fichier pour ecrir!\n");
        exit(1);
    }
    for(int i=0;i<*no_balles;i++)
    {
        if(fusil_de_chasse[i]%2==0) //si la balle est a blanc
        {
            fprintf(f,"%d - a blanc\n",fusil_de_chasse[i]); //on affiche une balle a blanc
        }
        else //si la balle est reelle
        {
            fprintf(f,"%d - reelle\n",fusil_de_chasse[i]); //on affiche une balle reelle
        }
    }
    fclose(f);
}


int *charger_fusil_de_chasse(int fusil_de_chasse[], int *no_balles_a_blanc, int *no_balles_reelles, int *no_balles)
{
    fusil_de_chasse=balles(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles); //charge au hasard le fusil de chasse avec des balles
    while(((*no_balles-abs(*no_balles_a_blanc-*no_balles_reelles))+1)<=abs(*no_balles_a_blanc-*no_balles_reelles)) //tant qu’il n’y a pas d’equilibre entre les balles a blanc et les balles reelles    {
    {
        fusil_de_chasse=balles(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles); //reequilibrage des balles de fusil de chasse
    }
    afficher_balles(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles); //on affiche les balles du fusil de chasse dans le fichier « ordre_balles »
    return fusil_de_chasse;
}


void afficher_objets(joueur *j1, joueur *j2)
{
    FILE *f,*g;
    f=fopen("objets_joueur1.txt","w+"); //ce fichier permet d'afficher les objets du joueur 1
    if(!f)
    {
        perror("\nImpossible d’ouvrir le fichier pour ecrir!\n");
        exit(1);
    }
    for(int i=1;i<=j1->compteur;i++)
    {
        fprintf(f,"%s\n",j1->objet[i-1].nom); //on affiche le nom de l’objet
    }
    fclose(f);
    g=fopen("objets_joueur2.txt","w+"); //ce fichier permet d’afficher les objets du joueur 2
    if(!g)
    {
        perror("\nImpossible d’ouvrir le fichier pour ecrir!\n");
        exit(1);
    }
    for(int i=1;i<=j2->compteur;i++)
    {
        fprintf(f,"%s\n",j2->objet[i-1].nom); //afficher le nom de l’objet
    }
    fclose(g);
}


char *verifier_objet(int objet, char *nom_de_l_objet)
{
    int objet_actuel=objet;
    switch (objet_actuel) //verifier l’article choisi par un joueur 
    {
        case 0:
            strcpy(nom_de_l_objet,"scie a main");
            break;
        case 1:
            strcpy(nom_de_l_objet,"loupe");
            break;
        case 2:
            strcpy(nom_de_l_objet,"bière");
            break;
        case 3:
            strcpy(nom_de_l_objet,"menottes");
            break;
        case 4:
            strcpy(nom_de_l_objet,"cigarettes");
            break;
        case 5:
            strcpy(nom_de_l_objet,"inverseur");
            break;
        case 6:
            strcpy(nom_de_l_objet,"telephone a brûleur");
            break;
        case 7:
            strcpy(nom_de_l_objet,"medicament perime");
            break;
        case 8:
            strcpy(nom_de_l_objet,"adrenaline");
            break;
        case 9:
            strcpy(nom_de_l_objet,"fusil de chasse");
    }
    return nom_de_l_objet;
}


joueur *repartir_des_objets_joueur(joueur *j, int *no_objet)
{
    int objet_neuf;
    for(int i=0;i<*no_objet;i++)
    {
        if(j->compteur==8) //si le joueur a deja 8 objets
        {
            break; //ne recevra plus rien
        }
        else //si le joueur a moins de 8 objets
        {
            objet_neuf=rand()%9;
            j->compteur++; //on augmente le nombre d'objets du joueur
            strcpy(j->objet[j->compteur-1].nom,verifier_objet(objet_neuf,j->objet[j->compteur-1].nom)); //on ajoute l’objet reçu au hasard au tableau des objets
            if(i>0)
            {
                while(strcmp(j->objet[j->compteur-1].nom,j->objet[j->compteur-2].nom)==0) //si vous recevez le même objet deux fois de suite
                {
                    objet_neuf=rand()%9;
                    strcpy(j->objet[j->compteur-1].nom,verifier_objet(objet_neuf,j->objet[j->compteur-1].nom)); //sera echange pour un autre objet, par souci d’equite
                }
            }   
        }
    }
    return j;
}


void scie_a_main(int *coupe)
{
    printf("\nLa balle suivante est deux fois plus fatale!\n");
    (*coupe)=1; //si la scie a main a ete utilise 
}


void loupe(int fusil_de_chasse[])
{
    FILE *h;
    h=fopen("deroulement_de_jeu.txt","w+"); //ce fichier permet de montrer la balle vue a la loupe ou decouverte grâce a un telephone a brûleur
    if(!h)
    {
        perror("\nImpossible d’ouvrir le fichier pour ecrir!\n");
        exit(1);
    }
    if(fusil_de_chasse[0]%2==0) //si la balle est a blanc
    {
        fprintf(h,"La balle actuelle est a blanc!\n"); //on affiche une balle a blanc 
    }
    else //si la balle est reelle
    {
        fprintf(h,"La balle actuelle est reelle!\n"); //on affiche une balle reelle
    }
    fclose(h);
}


int *bière(int fusil_de_chasse[], int *no_balles_a_blanc, int *no_balles_reelles, int *no_balles)
{
    if(fusil_de_chasse[0]%2==0) //si la balle est a blanc
    {
        printf("\nLa balle ejectee etait a blanc!\n"); //on affiche la balle a blanc 
        (*no_balles_a_blanc)--; //on diminue le nombre de balles a blanc  
}
    else //si la balle est reelle
    {
        printf("\nLa balle ejectee etait reelle!\n"); //on affiche la balle reelle
        (*no_balles_reelles)--; //on diminue le nombre de balles reelles
    }
    for(int i=0;i<*no_balles-1;i++)
    {
        fusil_de_chasse[i]=fusil_de_chasse[i+1]; //on deplace le reste des balles d’une position vers l’arrière
    }
    (*no_balles)--; //on diminue le nombre de balles 
    return fusil_de_chasse;
}


void menottes(int *avantage1, int *avantage2, int *menotte1, int *menotte2)
{
    if(*avantage1==1) //si c’est le tour du joueur 1
    {
        printf("\nLe joueur 2 est menotte pendant un tour!\n");
        (*menotte2)=1; //le joueur 2  est menotte
    }
    else //si c’est le tour du joueur 2
    {
        printf("\nLe joueur 1 est menotte pendant un tour!\n");
        (*menotte1)=1; //le joueur 1  est menotte 
    }
}


void cigarettes(joueur *j, int *ronde)
{
    switch (*ronde) //en fonction de quelle ronde il s’agit 
    {
        case 1:
            if(j->vies<max_vies_r1) //si le joueur a moins de 3 vies
            {
                j->vies++; //le joueur obtient une vie
                printf("\nLe joueur a recupere une vie!\n");
                printf("\nLe joueur a %d vies!\n",j->vies);
            }
            else
            {
                printf("\nLe joueur a deja le nombre maximum de vies! (%d vies)\n",j->vies);
            }
            break;
        case 2:
            if(j->vies<max_vies_r2) //si le joueur a moins de 4 vies
            {
                j->vies++; //le joueur obtient une vie
                printf("\nLe joueur a recupere une vie!\n");
                printf("\nLe joueur a %d vies!\n",j->vies);

            }
            else
            {
                printf("\nLe joueur a deja le nombre maximum de vies! (%d vies)\n",j->vies);
            }
            break;
        case 3:
            if(j->vies<max_vies_r3) //si le joueur a moins de 6 vies
            {
                j->vies++; //le joueur a reçu une vie
                printf("\nLe joueur a recupere une vie!\n");
                printf("\nLe joueur a %d vies!\n",j->vies);
            }
            else
            {
                printf("\nLe joueur a deja le nombre maximum de vies! (%d vies)\n",j->vies);
            }
            break;
    }
}


int *inverseur(int fusil_de_chasse[], int *no_balles_a_blanc, int *no_balles_reelles)
{
    if(fusil_de_chasse[0]%2==0) //si la balle actuelle est a blanc
    {
        fusil_de_chasse[0]=1; //devient reelle
        (*no_balles_a_blanc)--; //on diminuele nombre de balles a blanc 
        (*no_balles_reelles)++; //on augmente le nombre de balles reelles
    }
    else //si la balle actuelle est reelle
    {
        fusil_de_chasse[0]=0; //devient a blanc
        (*no_balles_reelles)--; //on diminue le nombre de balles reelles 
        (*no_balles_a_blanc)++; //on augmente le nombre de balles a blanc

    }
    printf("\nLa balle actuelle a ete inversee!\n");
    return fusil_de_chasse;
}


void telephone_a_brûleur(int fusil_de_chasse[], int *no_balles_a_blanc, int *no_balles_reelles, int *no_balles)
{
    FILE *h;
    h=fopen("deroulement_de_jeu.txt","w+"); //ce fichier permet de montrer la balle vue a la loupe ou decouverte grâce a un telephone a brûleur
    if(!h)
    {
        perror("\nImpossible d’ouvrir le fichier pour ecrir!\n");
        exit(1);
    }
    int balle=(rand()%(*no_balles))+1;
    for(int i=1;i<=*no_balles;i++)
    {
        if(balle==i) //si la balle dans le fusil correspond a la balle choisie au hasard
        {
            if(fusil_de_chasse[i-1]%2==0) //si la balle est a blanc
            {
                fprintf(h,"La balle %d est a blanc!\n",balle); //on affiche la balle a blanc et où se trouve dans le fusil de chasse
            }
            else //si la balle est reelle
            {
                fprintf(h,"La balle %d est reelle!\n",balle); //on affiche la balle reelle et où se trouve dans le fusil de chasse
            }
        }
    }
    fclose(h);
}


void medicament_perime(joueur *j, int *ronde)
{
    int bonne_chance =rand()%6; //une chance sur deux qu’il y ait bon ou mauvais
    if(bonne_chance%2==1) //s’il est bon
    {
        switch (*ronde) //en fonction de quelle ronde il s’agit 
        {
            case 1:
                if(j->vies<max_vies_r1-1) //si le joueur a moins de 2 vies
                {
                    j->vies=j->vies+2; //il reçoit 2 vies
                    printf("\nLe joueur a recupere 2 vies!\n");
                    printf("\nLe joueur a %d vies!\n",j->vies);
                }
                break;
            case 2:
                if(j->vies<max_vies_r2-1) //si le joueur a moins de 3 vies
                {
                    j->vies=j->vies+2; //il reçoit 2 vies
                    printf("\nLe joueur a recupere 2 vies!\n");
                    printf("\nLe joueur a %d vies!\n",j->vies);
                }
                break;
            case 3:
                if(j->vies<max_vies_r3-1) //si le joueur a moins de 5 vies
                {
                    j->vies=j->vies+2; //il reçoit 2 vies
                    printf("\nLe joueur a recupere 2 vies!\n");
                    printf("\nLe joueur a %d vies!\n",j->vies);
                }
                break;
        }
    }
    else //s’il est mauvais
    {
        j->vies--; //le joueur perd une vie
        printf("\nLe joueur a perdu une vie!\n");
        printf("\nLe joueur a %d vies!\n",j->vies);
    }
}


int adrenaline(joueur *j, joueur *jou, int *avantage1, int *avantage2)
{
    int choix; //choix de l'objet
    char choisi[20]; //objet choisi
    int ok; //verifier si le joueur a l’objet qui doit être vole
    printf("\nL'objet que tu souhaites voler = ");
    scanf("%d",&choix); //choisis ce que tu voles
    while(choix<0 || choix>8) //si tu choisis un numero non valide
    {
        printf("\nVoler un objet valide!\n");
        printf("\nL’objet que tu souhaites voler = ");
        scanf("%d",&choix); //choisis ce que tu voles
    }
    if(choix>=0 && choix<=8) //si un objet valide a ete choisi
    {
        ok=0; //on suppose qu’il n’a pas l’objet choisi
        if(choix==8) //si l’adrenaline a ete choisie
        {
            while(choix==8) //tant que l’adrenaline est choisie
            {
                printf("\nTu ne peux pas voler l'adrenaline!\n");
                printf("\nL’objet que tu souhaites voler = ");
                scanf("%d",&choix); //choisis ce que tu voles
            }
        }
        strcpy(choisi,verifier_objet(choix,choisi)); //l’objet choisi est verifie
        while(ok==0) //tant que l’adversaire n’a pas l’objet choisi 
        {
            for(int i=1;i<=jou->compteur;i++)
            {
                if(strcmp(choisi,jou->objet[i-1].nom)!=0) //verifier si le joueur a l’objet 
                {
                    continue; //s’il ne l’a pas, continuez
                }
                else if(ok==0) //s’il l’a
                {
                    ok=1; //il est verifie qu’il l’a
                    for(int k=i-1;k<jou->compteur-1;k++)
                    {
                        strcpy(jou->objet[k].nom,jou->objet[k+1].nom); //l’objet de la liste d’objets est supprime
                    }
                    jou->compteur--; //on diminue le nombre des objets 
                }
            }
            if(ok==0) //s’il ne l’a pas du tout
            {
                printf("\nVoler un objet valide!\n");
                printf("\nL’objet que tu souhaites voler = ");
                scanf("%d",&choix); //choisis ce que tu voles
                ok=0;  //on suppose qu’il n’a pas l’objet choisi
                strcpy(choisi,verifier_objet(choix,choisi)); //l’objet choisi est verifie
            }
        }
        if(*avantage1==1) //si le joueur 1 a vole un objet
        {
            afficher_objets(j,jou); //les objets sont montes après avoir ete vole au joueur 2
        }
        else //si le joueur 1 a vole un objet 2 
        {
            afficher_objets(jou,j); //les objets sont montes après avoir ete vole au joueur 1
        }
        printf("\nLe joueur a vole %s!\n",choisi);
    }
    return choix;
}


void balle_fusil_de_chasse(joueur *j, joueur *jou, int *coupe, int *sauve, int fusil_de_chasse[],int *no_balles_a_blanc, int *no_balles_reelles, int *no_balles, int *ronde, int *no_objet)
{
    if(*no_balles!=0) //si le fusil de chasse n’est pas vide
    {
        printf("\nQui tires-tu ?\n");
        printf("\nMoi (codee 0)\n");
        printf("\nAdversaire (codee 1)\n");
        int decision; //qui sera tire
        printf("\nCode = ");
        scanf("%d",&decision); //choisis qui se fait tirer dessus
        while(decision!=0 && decision!=1) //si tu choisis un numero non valide
        {
            printf("\nChoisir un code valide!\n");
            printf("\nCode = ");
            scanf("%d",&decision); //choisis qui se fait tirer dessus       
        }
        if(decision==0) //si le joueur se tire dessus
        {
            printf("\nLe joueur se tire dessus!\n");
            Sleep(dormir);
            if(fusil_de_chasse[0]==1) //si la balle est reelle 
            {
                if(*coupe==1) //si la scie a main a ete utilisee
                {
                    printf("\nLe joueur a perdu 2 vies!\n");
                    j->vies=j->vies-2; //on diminue le nombre de vies par 2
                    (*no_balles_reelles)--; //on diminue le nombre de balles reelles
                    Sleep(dormir);
                    printf("\nLe joueur a %d vies!\n",j->vies);
                }
                else //si non
                {
                    printf("\nLe joueur a perdu 1 vie!\n");
                    j->vies=j->vies-1; //on diminue le nombre de vies par 1
                    (*no_balles_reelles)--; //on diminue le nombre de balles reelles
                    Sleep(dormir);
                    printf("\nLe joueur a %d vies!\n",j->vies);
                }
                for(int i=0;i<*no_balles-1;i++)
                {
                    fusil_de_chasse[i]=fusil_de_chasse[i+1]; //on deplace le reste des balles d'une position vers l’arrière             
                }
                (*no_balles)--; //on diminue le nombre de balles 
            }
            else
            {
                printf("\nLe joueur est sauve...momentanement!\n");
                (*sauve)++; //c’est encore le tour du même joueur
                (*no_balles_a_blanc)--; //on diminue le nombre de balles a blanc
                for(int i=0;i<*no_balles-1;i++)
                {
                    fusil_de_chasse[i]=fusil_de_chasse[i+1]; //on deplace le reste des balles d'une position vers l’arrière     
                }
                (*no_balles)--; //on diminue le nombre de balles 
                Sleep(dormir);
                printf("\nLe joueur a %d vies!\n",j->vies);
            }
        }
        else //si le joueur tire sur l’adversaire
        {
            printf("\nLe joueur tire sur l’adversaire!\n");
            Sleep(dormir);
            if(fusil_de_chasse[0]==1) //si la balle est reelle 
            {
                if(*coupe==1) //si la scie a main a ete utilisee
                {
                    printf("\nL’adversaire a perdu 2 vies!\n");
                    j->vies=j->vies-2; //on diminue le nombre de vies par 2
                    (*no_balles_reelles)--; //on diminue le nombre de balles reelles
                    Sleep(dormir);
                    printf("\nL’adversaire a %d vies!\n",j->vies);
                }
                else //si non
                {
                    printf("\nL’adversaire a perdu 1 vie!\n");
                    j->vies=j->vies-1; //on diminue le nombre de vies par 1
                    (*no_balles_reelles)--; //on diminue le nombre de balles reelles
                    Sleep(dormir);
                    printf("\nL’adversaire a %d vies!\n",j->vies);
                }
                for(int i=0;i<*no_balles-1;i++)
                {
                    fusil_de_chasse[i]=fusil_de_chasse[i+1]; //on deplace le reste des balles d'une position vers l’arrière     
                }
                (*no_balles)--; //on diminue le nombre de balles 
            }
            else
            {
                printf("\nL’adversaire est sauve...momentanement!\n");
                (*sauve)++; //c’est encore le tour du même joueur
                (*no_balles_a_blanc)--; //on diminue le nombre de balles a blanc
                for(int i=0;i<*no_balles-1;i++)
                {
                    fusil_de_chasse[i]=fusil_de_chasse[i+1]; //on deplace le reste des balles d'une position vers l’arrière     
                }
                (*no_balles)--; //on diminue le nombre de balles 
                Sleep(dormir);
                printf("\nL’adversaire a %d vies!\n",j->vies);
            }
        }
        afficher_balles(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles); //on affiche les balles du fusil de chasse dans le fichier « ordre_balles »
    }
    else
    {
        switch (*ronde) //en fonction de quelle ronde il s’agit 
        {
            case 1:
                *no_balles=(rand()%balles_fusil_de_chasse)+2; //nombre de balles entre 2 et 4 pour le fusil de chasse
                break;
            case 2:
                *no_balles=(rand()%balles_fusil_de_chasse)+4; //nombre de balles entre 4 et 6 pour le fusil de chasse
                break;
            case 3:
                *no_balles=(rand()%balles_fusil_de_chasse)+6; //nombre de balles entre 6 et 8 pour le fusil de chasse
                break;
        }
        printf("\nLe fusil de chasse est recharge... \n");
        fusil_de_chasse=charger_fusil_de_chasse(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles); //le fusil de chasse est charge
        Sleep(dormir);
        printf("\nNombre de balles  = %d\n",*no_balles); //on affiche le nombre de balles dans le fusil de chasse
        printf("Nombre de balles a blanc= %d\n",*no_balles_a_blanc); //on affiche le nombre de balles reelles dans le fusil de chasse
        printf("Nombre de balles reelles= %d\n",*no_balles_reelles); //on affiche le nombre de balles a blanc dans le fusil de chasse
        printf("\nDes objets sont repartis...\n");
        j=repartir_des_objets_joueur(j,no_objet); //repartir des objets a joueur 1
        jou=repartir_des_objets_joueur(jou,no_objet); //repartir des objets a joueur  2
        afficher_objets(j,jou); //on affiche les objets de les 2 joueurs
        Sleep(dormir);
    }
}


void choix(int choisis, int *coupe, int *sauve, int fusil_de_chasse[],int *no_balles_a_blanc, int *no_balles_reelles, int *no_balles, joueur *j, objet *it, int *ronde, joueur *jou, int *avantage1, int *avantage2, int *menotte1, int *menotte2, int *no_objet)
{
    switch (choisis) //en fonction de l’objet choisi   
    {
        case 0:
            scie_a_main(coupe);
            Sleep(dormir);
            break;
        case 1:
            loupe(fusil_de_chasse);
            Sleep(dormir);
            break;
        case 2:
            fusil_de_chasse=bière(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles);
            afficher_balles(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles); //on affiche les balles du fusil de chasse dans le fichier « ordre_balles »
            Sleep(dormir);
            break;
        case 3:
            menottes(avantage1,avantage2,menotte1,menotte2);
            Sleep(dormir);
            break;
        case 4:
            cigarettes(j,ronde);
            Sleep(dormir);
            break;
        case 5:
            fusil_de_chasse=inverseur(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles);
            afficher_balles(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles); //on affiche les balles du fusil de chasse dans le fichier « ordre_balles »
            Sleep(dormir);
            break;
        case 6:
            telephone_a_brûleur(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles);
            Sleep(dormir);
            break;
        case 7:
            medicament_perime(j,ronde);
            Sleep(dormir);
            break;
        case 8:
            choisis=adrenaline(j,jou,avantage1,avantage2);
            Sleep(dormir);
            choix(choisis,coupe,sauve,fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles,j,it,ronde,jou,avantage1,avantage2,menotte1,menotte2,no_objet);
            break;
        case 9:
            balle_fusil_de_chasse(j,jou,coupe,sauve,fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles,ronde,no_objet);
            Sleep(dormir);
            break;
    }
}


void selectionner_objet(int fusil_de_chasse[], joueur *j, objet *it, int *no_balles_a_blanc, int *no_balles_reelles, int *no_balles, int *ronde, joueur *jou, int *avantage1, int *avantage2, int *menotte1, int *menotte2, int *no_objet)
{
    if(*no_balles==0) //si le fusil de chasse est vide
    {
        switch (*ronde) //en fonction de quelle ronde il s’agit 
        {
            case 1:
                *no_balles=(rand()%balles_fusil_de_chasse)+2; //nombre de balles entre 2 et 4 pour le fusil de chasse
                break;
            case 2:
                *no_balles=(rand()%balles_fusil_de_chasse)+4; //nombre de balles entre 4 et 6 pour le fusil de chasse
                break;
            case 3:
                *no_balles=(rand()%balles_fusil_de_chasse)+6; //nombre de balles entre 6 et 8 pour le fusil de chasse
                break;
        }
        printf("\nLe fusil de chasse est recharge... \n");
        fusil_de_chasse=charger_fusil_de_chasse(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles); //le fusil de chasse est charge
        Sleep(dormir);
        printf("\nNombre de balles  = %d\n",*no_balles); //on affiche le nombre de balles dans le fusil de chasse
        printf("Nombre de balles a blanc= %d\n",*no_balles_a_blanc); //on affiche le nombre de balles reelles dans le fusil de chasse
        printf("Nombre de balles reelles= %d\n",*no_balles_reelles); //on affiche le nombre de balles a blanc dans le fusil de chasse
        printf("\nSe impart objets...\n");
        j=repartir_des_objets_joueur(j,no_objet); //attribue au hasard des objets au joueur 1
        jou=repartir_des_objets_joueur(jou,no_objet); //attribue au hasard des objets au joueur 2
        afficher_objets(j,jou); //on affiche les objets de les 2 joueurs
        Sleep(dormir);
    }
    printf("\nSelectionner objet:\n");
    int choisis; //choix objet
    char choisi[20]; //objet choisi
    int ok; //verifier si le joueur a l’objet qu’il veut utiliser
    int coupe=0; //si le joueur
    int sauve=0; //si le joueur s’est tire dessus et s’est sauve
    printf("\nObjet = ");
    scanf("%d",&choisis); //choisis l’objet que tu souhaites utiliser
    while(choisis<0 || choisis>9) //si tu choisis un numero non valide 
    {
        printf("\nChoisir un objet valide!\n");
        printf("\nObjet = ");
        scanf("%d",&choisis); //choisis l’objet que tu souhaites utiliser    
    }
    if(choisis!=9) //si le fusil de chasse n’a pas ete choisi
    {
        while(choisis>=0 && choisis<=8) //si un objet valide a ete choisi
        {
            ok=0; //on suppose qu’il n’a pas l’objet choisi 
            strcpy(choisi,verifier_objet(choisis,choisi)); //l’objet choisi est verifie
            while(ok==0) //tant que le joueur n’a pas l’objet choisi
            {
                for(int i=1;i<=j->compteur;i++)
                {
                    if(strcmp(choisi,j->objet[i-1].nom)!=0) //si le joueur n’a pas l’objet choisi
                    {
                        continue; //s’il ne l’a pas, continuez
                    }
                    else //s’il l’a
                    {
                        ok=1; //il est verifie qu’il l’a
                    }
                }
                if(ok==0) //s’il ne l’a pas du tout
                {
                    printf("\nChoisir un objet valide!\n");
                    printf("\nObjet = ");
                    scanf("%d",&choisis); //choisis l’objet que tu utilise
                    ok=0; //on suppose qu’il n’a pas l’objet choisi
                    strcpy(choisi,verifier_objet(choisis,choisi)); //l’objet choisi est verifie
                }
            }
            printf("\nLe joueur a choisi %s!\n",choisi);
            Sleep(dormir);
            for(int i=1;i<=j->compteur;i++)
            {
                if(strcmp(choisi,j->objet[i-1].nom)!=0) //si l’objet choisi n’est pas trouve
                {
                    continue; //on continue
                }
                else //s’il est trouve
                {
                    for(int k=i-1;k<j->compteur-1;k++)
                    {
                        strcpy(j->objet[k].nom,j->objet[k+1].nom); //l’objet est supprime de la liste des objets
                    }
                    j->compteur--; //on diminue le nombre d’objets
                    break;
                }
            }
            if(*avantage1==1) //si le joueur 1 a utilise un objet
            {
                afficher_objets(j,jou); //les objets sont montes au nouveau après que le joueur 2 a utilise un objet
            }
            else //si le joueur 2 a utilise un objet
            {
                afficher_objets(jou,j); // les objets sont montes au nouveau après que le joueur 1 a utilise un objet
            }
            choix(choisis,&coupe,&sauve,fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles,j,it,ronde,jou,avantage1,avantage2,menotte1,menotte2,no_objet); //l’objet choisi est utilise
            printf("\nSelectionner objet:\n");
            printf("\nObjet = ");
            scanf("%d",&choisis); //choisis l’objet que tu souhaites utiliser
            while(choisis<0 || choisis>9) //si tu choisis un numero non valide 
            {
                printf("\nChoisir un objet valide!\n");
                printf("\nObjet = ");
                scanf("%d",&choisis); //choisis l’objet que tu utilise
            }
        }
    }
    strcpy(choisi,verifier_objet(choisis,choisi)); //le fusil de chasse est choisi
    printf("\nLe joueur a choisi %s!\n",choisi);
    Sleep(dormir);
    choix(choisis,&coupe,&sauve,fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles,j,it,ronde,jou,avantage1,avantage2,menotte1,menotte2,no_objet); //le fusil de chasse est choisi
    if(*no_balles==0) //si le fusil de chasse est vide
    {
        switch (*ronde) //en fonction de quelle ronde il s’agit 
        {
            case 1:
                *no_balles=(rand()%balles_fusil_de_chasse)+2; //nombre de balles entre 2 et 4 pour le fusil de chasse
                break;
            case 2:
                *no_balles=(rand()%balles_fusil_de_chasse)+4; //nombre de balles entre 4 et 6 pour le fusil de chasse
                break;
            case 3:
                *no_balles=(rand()%balles_fusil_de_chasse)+6; //nombre de balles entre 6 et 8 pour le fusil de chasse
                break;
        }
        printf("\nLe fusil de chasse est recharge... \n");
        fusil_de_chasse=charger_fusil_de_chasse(fusil_de_chasse,no_balles_a_blanc,no_balles_reelles,no_balles); //le fusil de chasse est charge
        Sleep(dormir);
        printf("\nNombre de balles  = %d\n",*no_balles); //on affiche le nombre de balles dans le fusil de chasse
        printf("Nombre de balles a blanc= %d\n",*no_balles_a_blanc); //on affiche le nombre de balles reelles dans le fusil de chasse
        printf("Nombre de balles reelles= %d\n",*no_balles_reelles); //on affiche le nombre de balles a blanc dans le fusil de chasse
        printf("\nSe impart objets...\n");
        j=repartir_des_objets_joueur(j,no_objet); //repartir des objets a joueur  1
        jou=repartir_des_objets_joueur(jou,no_objet); //repartir des objets a joueur  2
        afficher_objets(j,jou); //on affiche les objets de les 2 joueurs
        Sleep(dormir);
    }
    if(sauve==1) //si le joueur se tire une balle a blanc
    {
        selectionner_objet(fusil_de_chasse,j,it,no_balles_a_blanc,no_balles_reelles,no_balles,ronde,jou,avantage1,avantage2,menotte1,menotte2,no_objet); //le joueur choisit a nouveau un objet
    }
}


int main()
{
    int ronde=0; //ronde
    int score1=0; //score joueur 1
    int score2=0; //score joueur 2
    while(ronde<3) //tant que 3 tours n’ont pas ete joues
    {
        ronde++; //ronde actuelle
        int no_objet; //le nombre d’objets que les joueurs recevront
        int no_balles; //nombre de balles dans le fusil (au hasard)
        int no_balles_a_blanc=0; //nombre de balles a blanc dans le fusil de chasse
        int no_balles_reelles=0; //nombre de balles reelles dans le fusil de chasse
        printf("\nRONDE %d\n",ronde);
        joueur *j1=(joueur*)malloc(sizeof(joueur)); //on alloue de la memoire pour le pointeur joueur1
        if(!j1)
        {
            perror("\nLa memoire n’a pas pu être allouee!\n");
            exit(2);
        }
        joueur *j2=(joueur*)malloc(sizeof(joueur)); //on alloue de la memoire pour le pointeur joueur2
        if(!j2)
        {
            perror("\nLa memoire n'a pas pu être allouee!\n");
            exit(2);
        }
        objet *it=(objet*)malloc(max_objet*sizeof(objet)); //on alloue de la memoire pour le pointeur objet
        if(!it)
        {
            perror("\nLa memoire n'a pas pu être allouee!\n");
            exit(2);
        }
        srand(time(0));
        switch (ronde) //s’il s’agit de la première ronde
        {
            case 1:
                j1->vies=max_vies_r1; //3 vies
                j1->compteur=0;
                j2->vies=max_vies_r1; //3 vies
                j2->compteur=0;
                no_objet=max_objet_r1; //les joueurs recevront 2 objets chacun
                no_balles=(rand()%balles_fusil_de_chasse)+2; //nombre de balles entre 2 et 4 pour le fusil de chasse
                break;
            case 2:
                j1->vies=max_vies_r2; //4 vies
                j1->compteur=0;
                j2->vies=max_vies_r2; //4 vies
                j2->compteur=0;
                no_objet=max_objet_r2; //les joueurs recevront 3 objets chacun
                no_balles=(rand()%balles_fusil_de_chasse)+4; //nombre de balles entre 4 et 6 pour le fusil de chasse
                break;
            case 3:
                j1->vies=max_vies_r3; //6 vies
                j1->compteur=0;
                j2->vies=max_vies_r3; //6 vies
                j2->compteur=0;
                no_objet=max_objet_r3; //les joueurs recevront 4 objets chacun
                no_balles=(rand()%balles_fusil_de_chasse)+6; //nombre de balles entre 6 et 8 pour le fusil de chasse
                break;
        }
        Sleep(dormir);
        int *fusil_de_chasse=(int*)malloc(no_balles*sizeof(int)); //on alloue de la memoire pour le pointeur fusil_de_chasse
        if(!fusil_de_chasse)
        {
            perror("\nLa memoire n'a pas pu être allouee!\n");
            exit(2);
        }
        fusil_de_chasse=charger_fusil_de_chasse(fusil_de_chasse,&no_balles_a_blanc,&no_balles_reelles,&no_balles); //le fusil de chasse est charge
        printf("\nNombre de balles  = %d\n",no_balles); //on affiche le nombre de balles dans le fusil de chasse
        printf("Nombre de balles a blanc= %d\n",no_balles_a_blanc); //on affiche le nombre de balles reelles dans le fusil de chasse
        printf("Nombre de balles reelles= %d\n",no_balles_reelles); //on affiche le nombre de balles a blanc dans le fusil de chasse
        Sleep(dormir);
        printf("\nDes objets sont reparti...\n");
        j1=repartir_des_objets_joueur(j1,&no_objet); //repartir des objets a joueur  1
        j2=repartir_des_objets_joueur(j2,&no_objet); //repartir des objets a joueur  2
        afficher_objets(j1,j2); //on affiche les objets de les 2 joueurs
        Sleep(dormir);
        int jou1=rand()%2;
        int jou2=rand()%2;
        int decision=rand()%2;
        int avantage1=0; //si c’est le tour du joueur 1
        int avantage2=0; //si c’est le tour du joueur 2
        int menotte1=0; //si le joueur 1 sera menotte
        int menotte2=0; //si le joueur2 sera menotte
        printf("\nOn decide qui commence...\n");
        while(jou1==jou2) //il est decide au hasard qui commence
        {
            jou1=rand()%2;
            jou2=rand()%2;
        }
        Sleep(dormir);
        if(decision==jou1)
        {
            avantage1=1; //le tour du joueur 1
        }
        else
        {
            avantage2=1; //le tour du joueur 2
        }
        if(avantage1==1) //le joueur 1 demarre
        {
            printf("\nLe joueur 1 demarre\n");
            while(j1->vies>0 && j2->vies>0) //tant qu’aucun joueur n’est mort
            {
                if(menotte1==0) //si le joueur 1 n’a pas ete menotte
                {
                    avantage1=1; //c’est le tour du joueur 1
                    avantage2=0; //le tour du joueur 2 a passe
                    if(j1->vies>0) //si le joueur 1 n’est pas mort
                    {
                        printf("\nLe joueur 1 choisit un objet!\n");
                        selectionner_objet(fusil_de_chasse,j1,it,&no_balles_a_blanc,&no_balles_reelles,&no_balles,&ronde,j2,&avantage1,&avantage2,&menotte1,&menotte2,&no_objet); //le joueur 1 a choisi un objet
                    }
                }
                menotte1=0; //dans le cas où il a ete menotte pendant un tour, l’effet s'estompe
                if(no_balles==0 && (j1->vies!=0 && j2->vies!=0)) //si le fusil de chasse est vide et aucun joueur n’est mort
                {
                    switch (ronde) //en fonction de quelle ronde il s’agit 
                    {
                        case 1:
                            no_balles=(rand()%balles_fusil_de_chasse)+2; //nombre de balles entre 2 et 4 pour le fusil de chasse
                            break;
                        case 2:
                            no_balles=(rand()%balles_fusil_de_chasse)+4; //nombre de balles entre 4 et 6 pour le fusil de chasse
                            break;
                        case 3:
                            no_balles=(rand()%balles_fusil_de_chasse)+6; //nombre de balles entre 6 et 8 pour le fusil de chasse
                            break;
                    }
                    printf("\nLe fusil de chasse est recharge... \n");
                    fusil_de_chasse=charger_fusil_de_chasse(fusil_de_chasse,&no_balles_a_blanc,&no_balles_reelles,&no_balles); //on recharge le fusil de chasse
                    Sleep(dormir);
                    printf("\nNombre de balles  = %d\n",no_balles); //on affiche le nombre de balles dans le fusil de chasse
                    printf("Nombre de balles a blanc= %d\n",no_balles_a_blanc); //on affiche le nombre de balles reelles dans le fusil de chasse
                    printf("Nombre de balles reelles= %d\n",no_balles_reelles); //on affiche le nombre de balles a blanc dans le fusil de chasse
                    printf("\nDes objets sont reparti...\n");
                    j1=repartir_des_objets_joueur(j1,&no_objet); //repartir au hasard les objets au joueur 1
                    j2=repartir_des_objets_joueur(j2,&no_objet); //repartir au hasard les objets au joueur 2
                    afficher_objets(j1,j2); //on affiche les objets de les 2 joueurs
                    Sleep(dormir);
                }
                if(menotte2==0) //si le joueur 2 n’a pas ete menotte
                {
                    avantage1=0; //le tour du joueur 1 a passe
                    avantage2=1; //c’est le tour du joueur 2
                    if(j2->vies>0) //si le joueur 2 n’est pas mort
                    {
                        printf("\nLe joueur 2 choisit un objet!\n");
                        selectionner_objet(fusil_de_chasse,j2,it,&no_balles_a_blanc,&no_balles_reelles,&no_balles,&ronde,j1,&avantage1,&avantage2,&menotte1,&menotte2,&no_objet); //le joueur 2 a choisi un objet
                    }
                }
                menotte2=0; //dans le cas où il a ete menotte pendant un tour, l’effet s'estompe
                if(no_balles==0 && (j1->vies!=0 && j2->vies!=0)) //si le fusil de chasse est vide et aucun joueur n’est mort
                {
                    switch (ronde) //en fonction de quelle ronde il s’agit 
                    {
                        case 1:
                            no_balles=(rand()%balles_fusil_de_chasse)+2; //nombre de balles entre 2 et 4 pour le fusil de chasse
                            break;
                        case 2:
                            no_balles=(rand()%balles_fusil_de_chasse)+4; //nombre de balles entre 4 et 6 pour le fusil de chasse
                            break;
                        case 3:
                            no_balles=(rand()%balles_fusil_de_chasse)+6; //nombre de balles entre 6 et 8 pour le fusil de chasse
                            break;
                    }
                    printf("\nLe fusil de chasse est recharge... \n");
                    fusil_de_chasse=charger_fusil_de_chasse(fusil_de_chasse,&no_balles_a_blanc,&no_balles_reelles,&no_balles); //le fusil de chasse est recharge  
                    Sleep(dormir);
                    printf("\nNombre de balles  = %d\n",no_balles); //on affiche le nombre de balles dans le fusil de chasse
                    printf("Nombre de balles a blanc= %d\n",no_balles_a_blanc); //on affiche le nombre de balles reelles dans le fusil de chasse
                    printf("Nombre de balles reelles= %d\n",no_balles_reelles); //on affiche le nombre de balles a blanc dans le fusil de chasse
                    printf("\nDes objets sont reparti...\n");
                    j1=repartir_des_objets_joueur(j1,&no_objet); //repartir des objets a joueur  1
                    j2=repartir_des_objets_joueur(j2,&no_objet); //repartir des objets a joueur  2
                    afficher_objets(j1,j2); //on affiche les objets de les 2 joueurs
                    Sleep(dormir);
                }
            }
        }
        else //le joueur 2 demarre
        {
            printf("\nLe joueur 2 demarre\n");
            Sleep(dormir);
            while(j1->vies>0 && j2->vies>0) //tant qu’aucun joueur n’est mort
            {
                if(menotte2==0) //si le joueur 2 n’a pas ete menotte
                {
                    avantage1=0; //le tour du joueur 1 a passe
                    avantage2=1; //c’est le tour du joueur 2
                    if(j2->vies>0) //si le joueur 2 n’est pas mort
                    {
                    printf("\nLe joueur 2 choisit un objet!\n");
                    selectionner_objet(fusil_de_chasse,j2,it,&no_balles_a_blanc,&no_balles_reelles,&no_balles,&ronde,j1,&avantage1,&avantage2,&menotte1,&menotte2,&no_objet); //le joueur 2 a choisi un objet
                    }
                }
                menotte2=0; //dans le cas où il a ete menotte pendant un tour, l’effet s'estompe 
                if(no_balles==0 && (j1->vies!=0 && j2->vies!=0)) //si le fusil de chasse est vide et aucun joueur n’est mort
                {
                    switch (ronde) //en fonction de quelle ronde il s’agit 
                    {
                        case 1:
                            no_balles=(rand()%balles_fusil_de_chasse)+2; //nombre de balles entre 2 et 4 pour le fusil de chasse
                            break;
                        case 2:
                            no_balles=(rand()%balles_fusil_de_chasse)+4; //nombre de balles entre 4 et 6 pour le fusil de chasse
                            break;
                        case 3:
                            no_balles=(rand()%balles_fusil_de_chasse)+6; //nombre de balles entre 6 et 8 pour le fusil de chasse
                            break;
                    }
                    printf("\nLe fusil de chasse est recharge... \n");
                    fusil_de_chasse=charger_fusil_de_chasse(fusil_de_chasse,&no_balles_a_blanc,&no_balles_reelles,&no_balles); //le fusil de chasse est recharge  
                    Sleep(dormir);
                    printf("\nNombre de balles  = %d\n",no_balles); //on affiche le nombre de balles dans le fusil de chasse
                    printf("Nombre de balles a blanc= %d\n",no_balles_a_blanc); //on affiche le nombre de balles reelles dans le fusil de chasse
                    printf("Nombre de balles reelles= %d\n",no_balles_reelles); //on affiche le nombre de balles a blanc dans le fusil de chasse
                    printf("\nDes objets sont reparti...\n");
                    j1=repartir_des_objets_joueur(j1,&no_objet); //repartir des objets a joueur  1
                    j2=repartir_des_objets_joueur(j2,&no_objet); //repartir des objets a joueur  2
                    afficher_objets(j1,j2); //on affiche les objets de les 2 joueurs
                    Sleep(dormir);
                }
                if(menotte1==0) //si le joueur 1 n’a pas ete menotte
                {
                    avantage1=1; //c’est le tour du joueur 1
                    avantage2=0; //le tour du joueur 1 a passe 2
                    if(j1->vies>0) //si le joueur 1 n’est pas mort
                    {
                        printf("\nLe joueur 1 choisit un objet!\n");
                        selectionner_objet(fusil_de_chasse,j1,it,&no_balles_a_blanc,&no_balles_reelles,&no_balles,&ronde,j2,&avantage1,&avantage2,&menotte1,&menotte2,&no_objet); //le joueur 1 a choisi un objet
                    }
                }
                menotte1=0; //dans le cas où il a ete menotte pendant un tour, l’effet s'estompe 
                if(no_balles==0 && (j1->vies!=0 && j2->vies!=0)) //si le fusil de chasse est vide et aucun joueur n’est mort
                {
                    switch (ronde) //en fonction de quelle ronde il s’agit 
                    {
                        case 1:
                            no_balles=(rand()%balles_fusil_de_chasse)+2; //nombre de balles entre 2 et 4 pour le fusil de chasse
                            break;
                        case 2:
                            no_balles=(rand()%balles_fusil_de_chasse)+4; //nombre de balles entre 4 et 6 pour le fusil de chasse
                            break;
                        case 3:
                            no_balles=(rand()%balles_fusil_de_chasse)+6; //nombre de balles entre 6 et 8 pour le fusil de chasse
                            break;
                    }
                    printf("\nLe fusil de chasse est recharge... \n");
                    fusil_de_chasse=charger_fusil_de_chasse(fusil_de_chasse,&no_balles_a_blanc,&no_balles_reelles,&no_balles); //le fusil de chasse est recharge  
                    Sleep(dormir);
                    printf("\nNombre de balles  = %d\n",no_balles); //on affiche le nombre de balles dans le fusil de chasse
                    printf("Nombre de balles a blanc= %d\n",no_balles_a_blanc); //on affiche le nombre de balles reelles dans le fusil de chasse
                    printf("Nombre de balles reelles= %d\n",no_balles_reelles); //on affiche le nombre de balles a blanc dans le fusil de chasse
                    printf("\nDes objets sont reparti...\n");
                    j1=repartir_des_objets_joueur(j1,&no_objet); //repartir des objets a joueur  1
                    j2=repartir_des_objets_joueur(j2,&no_objet); //repartir des objets a joueur  2
                    afficher_objets(j1,j2); //on affiche les objets de les 2 joueurs
                    Sleep(dormir);
                }
            }
        }   
        if(j1->vies==0) //si le joueur 1 n’a plus de vies
        {
            printf("\nLe joueur 1 est mort!\n");
            Sleep(dormir);
            printf("\nLe joueur 2 a gagne la ronde!\n");
            score2++; //un point pour le joueur 2
        }
        else //si le joueur 2 n’a plus de vies
        {
            printf("\nLe joueur 2 est mort!\n");
            Sleep(dormir);
            printf("\nLe joueur 1 a gagne la ronde!\n");
            score1++; //un point pour le joueur 1
        }
        printf("\nRONDE %d est fini!\n",ronde);
        Sleep(dormir);
        free(j1); //la memoire du pointeur joueur1 est liberee
        free(j2); //la memoire du pointeur joueur2 est liberee
        free(it); //la memoire du pointeur objet est liberee
        free(fusil_de_chasse); //la memoire du pointeur fusil de chasse est liberee
    }
    printf("\nScore final: %d - %d",score1,score2); //score final
    Sleep(dormir);
    if(score1>score2) //si le joueur 1 a gagne plusieurs rondes
    {
        printf("\nLe joueur 1 a gagne!\n");
    }
    else //si le joueur 2 a gagne plusieurs rondes
    {
        printf("\nLe joueur 2 a gagne!\n");
    }
    Sleep(dormir);
    return 0;
}
