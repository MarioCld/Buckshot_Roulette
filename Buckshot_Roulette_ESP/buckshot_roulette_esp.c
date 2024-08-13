#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string.h>

#define balas_escopeta 3
#define max_objeto_r1 2
#define max_objeto_r2 3
#define max_objeto_r3 4
#define max_objeto 8
#define max_vidas_r1 3
#define max_vidas_r2 4
#define max_vidas_r3 6
#define dormir 1000 //pausa de un segundo entre cada interaccion


typedef struct objeto //typedef objeto
{
    char nombre[20]; //el nombre del objeto 
}objeto;

typedef struct jugador //typedef jugador
{
    int vidas; //numero de vidas del jugador 
    int contador; //numero de objetos del jugador 
    struct objeto objeto[max_objeto]; //matriz de objetos
}jugador;


int *balas(int escopeta[], int *no_balas_de_salva, int *no_balas_de_verdad, int *no_balas)
{
    *no_balas_de_salva=0;
    *no_balas_de_verdad=0;
    for(int i=0;i<*no_balas;i++)
    {
        escopeta[i]=rand()%2; //anadir al azar una bala de salva o una bala de verdad
        if(escopeta[i]==0) //si la bala es de salva
        {
            (*no_balas_de_salva)++; //incrementar el numero de balas de salva
        }
        else //si la bala es de verdad
        {
            (*no_balas_de_verdad)++; //incrementar el numero de balas de verdad
        }
    }
    return escopeta;
}


void mostrar_balas(int escopeta[], int *no_balas_de_salva, int *no_balas_de_verdad, int *no_balas)
{
    FILE *f;
    f=fopen("el_orden_de_las_balas.txt","w+"); //este archivo es para mostrar el orden y el tipo de balas en la escopeta (no miren el archivo mientras juegan)
    if(!f)
    {
        perror("\nNo se ha podido abrir el archivo para escribir!\n");
        exit(1);
    }
    for(int i=0;i<*no_balas;i++)
    {
        if(escopeta[i]%2==0) //si la bala es de salva
        {
            fprintf(f,"%d - salva\n",escopeta[i]); //se muestra la bala de salva
        }
        else //si la bala es de verdad
        {
            fprintf(f,"%d - verdad\n",escopeta[i]); //se muestra la bala de verdad 
        }
    }
    fclose(f);
}


int *cargar_escopeta(int escopeta[], int *no_balas_de_salva, int *no_balas_de_verdad, int *no_balas)
{
    escopeta=balas(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas); //cargar al azar la escopeta con balas
    while(((*no_balas-abs(*no_balas_de_salva-*no_balas_de_verdad))+1)<=abs(*no_balas_de_salva-*no_balas_de_verdad)) //mientras no haya un equilibrio entre balas de salva y balas de verdad
    {
        escopeta=balas(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas); //reequilibrio de balas de escopeta
    }
    mostrar_balas(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas); //se muestran las balas de la escopeta en el archivo “el_orden_de_las_balas” 
    return escopeta;
}


void mostrar_objetos(jugador *j1, jugador *j2)
{
    FILE *f,*g;
    f=fopen("objetos_jugador1.txt","w+"); //este archivo sirve para mostrar los objetos del jugador 1 
    if(!f)
    {
        perror("\nNo se ha podido abrir el archivo para escribir!\n");
        exit(1);
    }
    for(int i=1;i<=j1->contador;i++)
    {
        fprintf(f,"%s\n",j1->objeto[i-1].nombre); //se muestra el nombre del objeto 
    }
    fclose(f);
    g=fopen("objetos_jugador2.txt","w+"); //este archivo sirve para mostrar los objetos del jugador 2 
    if(!g)
    {
        perror("\nNo se pudo abrir el archivo para escribir!\n");
        exit(1);
    }
    for(int i=1;i<=j2->contador;i++)
    {
        fprintf(f,"%s\n",j2->objeto[i-1].nombre); //se muestra el nombre del objeto 
    }
    fclose(g);
}


char *verificar_objeto(int objeto, char *nombre_objeto)
{
    int objeto_actual=objeto;
    switch (objeto_actual) //verificar que objeto eligio el jugador
    {
        case 0:
            strcpy(nombre_objeto,"serrucho");
            break;
        case 1:
            strcpy(nombre_objeto,"lupa");
            break;
        case 2:
            strcpy(nombre_objeto,"cerveza");
            break;
        case 3:
            strcpy(nombre_objeto,"esposas");
            break;
        case 4:
            strcpy(nombre_objeto,"cigarros");
            break;
        case 5:
            strcpy(nombre_objeto,"inversor");
            break;
        case 6:
            strcpy(nombre_objeto,"telefono desechable");
            break;
        case 7:
            strcpy(nombre_objeto,"medicamento caducado");
            break;
        case 8:
            strcpy(nombre_objeto,"adrenalina");
            break;
        case 9:
            strcpy(nombre_objeto,"escopeta");
    }
    return nombre_objeto;
}


jugador *repartir_objetos_jugador(jugador *j, int *no_objeto)
{
    int objeto_nuevo;
    for(int i=0;i<*no_objeto;i++)
    {
        if(j->contador==8) //si el jugador ya tiene 8 objetos 
        {
            break; //no recibira mas 
        }
        else //si el jugador tiene menos de 8 objetos
        {
            objeto_nuevo=rand()%9;
            j->contador++; //incrementar el numero de objetos del jugador
            strcpy(j->objeto[j->contador-1].nombre,verificar_objeto(objeto_nuevo,j->objeto[j->contador-1].nombre)); //anadir el objeto recibido al azar a la matriz de objetos
            if(i>0)
            {
                while(strcmp(j->objeto[j->contador-1].nombre,j->objeto[j->contador-2].nombre)==0) //si recibes el mismo objeto dos veces seguidas
                {
                    objeto_nuevo=rand()%9;
                    strcpy(j->objeto[j->contador-1].nombre,verificar_objeto(objeto_nuevo,j->objeto[j->contador-1].nombre)); //se cambiara por otro objeto, para que sea justo
                }
            }   
        }
    }
    return j;
}


void serrucho(int *cortado)
{
    printf("\nLa siguiente bala es el doble de fatal!\n");
    (*cortado)=1; //si el serrucho fue utilizado 
}


void lupa(int escopeta[])
{
    FILE *h;
    h=fopen("dinamica_de_juego.txt","w+"); //este archivo es para mostrar la bala vista con la lupa o detectada con el telefono desechable
    if(!h)
    {
        perror("\nNo se ha podido abrir el archivo para escribir!\n");
        exit(1);
    }
    if(escopeta[0]%2==0) //si la bala es de salva
    {
        fprintf(h,"La bala actual es de salva!\n"); //se muestra la bala de salva
    }
    else //si la bala es de verdad
    {
        fprintf(h,"La bala actual es de verdad!\n"); //se muestra la bala de verdad
    }
    fclose(h);
}


int *cerveza(int escopeta[], int *no_balas_de_salva, int *no_balas_de_verdad, int *no_balas)
{
    if(escopeta[0]%2==0) //si la bala es de salva
    {
        printf("\nLa bala expulsada era de salva!\n"); //se muestra la bala de salva 
        (*no_balas_de_salva)--; //disminuir el numero de balas de salva
    }
    else //si la bala es de verdad
    {
        printf("\nLa bala expulsada era de verdad!\n"); //print la bala de verdad 
        (*no_balas_de_verdad)--; //disminuir el numero de balas de verdad
    }
    for(int i=0;i<*no_balas-1;i++)
    {
        escopeta[i]=escopeta[i+1]; //mover el resto de las balas una posicion hacia atras
    }
    (*no_balas)--; //disminuir el numero de balas
    return escopeta;
}


void esposas(int *ventaja1, int *ventaja2, int *esposado1, int *esposado2)
{
    if(*ventaja1==1) //si es el turno del jugador 1
    {
        printf("\nEl jugador 2 es esposado por un turno!\n");
        (*esposado2)=1; //el jugador 2 es esposado 
    }
    else //si es el turno del jugador 2
    {
        printf("\nEl jugador 1 es esposado por un turno!\n");
        (*esposado1)=1; //el jugador 1 es esposado
    }
}


void cigarros(jugador *j, int *ronda)
{
    switch (*ronda) //segun la ronda de que se trate
    {
        case 1:
            if(j->vidas<max_vidas_r1) //si el jugador tiene menos de 3 vidas
            {
                j->vidas++; //el jugador recibe una vida
                printf("\nEl jugador recupero una vida!\n");
                printf("\nEl jugador tiene %d vidas!\n",j->vidas);
            }
            else
            {
                printf("\nEl jugador ya tiene el numero maximo de vidas! (%d vidas)\n",j->vidas);
            }
            break;
        case 2:
            if(j->vidas<max_vidas_r2) //si el jugador tiene menos de 4 vidas
            {
                j->vidas++; //el jugador recibe una vida
                printf("\nEl jugador recupero una vida!\n");
                printf("\nEl jugador tiene %d vidas!\n",j->vidas);

            }
            else
            {
                printf("\nEl jugador ya tiene el numero maximo de vidas! (%d vidas)\n",j->vidas);
            }
            break;
        case 3:
            if(j->vidas<max_vidas_r3) //si el jugador tiene menos de 6 vidas
            {
                j->vidas++; //el jugador recibe una vida 
                printf("\nEl jugador recupero una vida!\n");
                printf("\nEl jugador tiene %d vidas!\n",j->vidas);
            }
            else
            {
                printf("\nEl jugador ya tiene el numero maximo de vidas! (%d vidas)\n",j->vidas);
            }
            break;
    }
}


int *inversor(int escopeta[], int *no_balas_de_salva, int *no_balas_de_verdad)
{
    if(escopeta[0]%2==0) //si la bala actual es de salva
    {
        escopeta[0]=1; //se convierte en bala de verdad
        (*no_balas_de_salva)--; //disminuir el numero de balas de salva
        (*no_balas_de_verdad)++; //incrementar el numero de balas de verdad
    }
    else //si la bala actual es de verdad
    {
        escopeta[0]=0; //se convierte en bala de salva
        (*no_balas_de_verdad)--; //disminuir el numero de balas de verdad
        (*no_balas_de_salva)++; //incrementar el numero de balas de salva
    }
    printf("\nLa bala actual se ha invertido!\n");
    return escopeta;
}


void telefono_desechable(int escopeta[], int *no_balas_de_salva, int *no_balas_de_verdad, int *no_balas)
{
    FILE *h;
    h=fopen("dinamica_de_juego.txt","w+"); //este archivo es para mostrar la bala vista con la lupa o detectada con el telefono desechable
    if(!h)
    {
        perror("\nNo se ha podido abrir el archivo para escribir!\n");
        exit(1);
    }
    int bala=(rand()%(*no_balas))+1;
    for(int i=1;i<=*no_balas;i++)
    {
        if(bala==i) //si la bala de la escopeta coincide con la bala seleccionada al azar
        {
            if(escopeta[i-1]%2==0) //si la bala es de salva
            {
                fprintf(h,"La bala %d es de salva!\n",bala); //se muestra la bala de salva y donde se encuentra en la escopeta 
            }
            else //si la bala es de verdad
            {
                fprintf(h,"La bala %d es de verdad!\n",bala); //se muestra la bala de verdad y donde se encuentra en la escopeta
            }
        }
    }
    fclose(h);
}


void medicamento_caducado(jugador *j, int *ronda)
{
    int suerte=rand()%6; //50/50 de posibilidades de acertar o fallar
    if(suerte%2==1) //si ha acertado
    {
        switch (*ronda) //segun la ronda de que se trate
        {
            case 1:
                if(j->vidas<max_vidas_r1-1) //si el jugador tiene menos de 2 vidas
                {
                    j->vidas=j->vidas+2; //recibe 2 vidas
                    printf("\nEl jugador recupero 2 vidas!\n");
                    printf("\nEl jugador tiene %d vidas!\n",j->vidas);
                }
                break;
            case 2:
                if(j->vidas<max_vidas_r2-1) //si el jugador tiene menos de 3 vidas

                {
                    j->vidas=j->vidas+2; //recibe 2 vidas
                    printf("\nEl jugador recupero 2 vidas!\n");
                    printf("\nEl jugador tiene %d vidas!\n",j->vidas);
                }
                break;
            case 3:
                if(j->vidas<max_vidas_r3-1) //si el jugador tiene menos de 5 vidas
                {
                    j->vidas=j->vidas+2; //recibe 2 vidas
                    printf("\nEl jugador recupero 2 vidas!\n");
                    printf("\nEl jugador tiene %d vidas!\n",j->vidas);
                }
                break;
        }
    }
    else //si ha fallado 
    {
        j->vidas--; //el jugador pierde una vida
        printf("\nEl jugador perdio una vida!\n");
        printf("\nEl jugador tiene %d vieti!\n",j->vidas);
    }
}


int adrenalina(jugador *j, jugador *jug, int *ventaja1, int *ventaja2)
{
    int eleccion; //eleccion del objeto
    char elegido[20]; //objeto elegido
    int ok; //verificar si el jugador tiene el objeto que quiere robar 
    printf("\nEl objeto que quieres robar = ");
    scanf("%d",&eleccion); //elige que objeto vas a robar
    while(eleccion<0 || eleccion>8) //si es elegido un objeto no valido
    {
        printf("\nRobar un objeto valido!\n");
        printf("\nEl objeto que quieres robar = ");
        scanf("%d",&eleccion); //elige que objeto vas a robar
    }
    if(eleccion>=0 && eleccion<=8) //si es elegido un objeto valido
    {
        ok=0; //supongamos que no tiene el objeto elegido
        if(eleccion==8) //si es adrenalina
        {
            while(eleccion==8) //cuanto tiempo se elige la adrenalina
            {
                printf("\nNo puedes robar la adrenalina!\n");
                printf("\nEl objeto que quieres robar = ");
                scanf("%d",&eleccion); //elige que objeto vas a robar
            }
        }
        strcpy(elegido,verificar_objeto(eleccion,elegido)); //el objeto elegido es verificado 
        while(ok==0) //mientras el oponente no tenga el objeto elegido
        {
            for(int i=1;i<=jug->contador;i++)
            {
                if(strcmp(elegido,jug->objeto[i-1].nombre)!=0) //verificar si el jugador tiene el objeto
                {
                    continue; //si no lo tiene, continue
                }
                else if(ok==0) //si lo tiene 
                {
                    ok=1; //se verifica que lo tiene
                    for(int k=i-1;k<jug->contador-1;k++)
                    {
                        strcpy(jug->objeto[k].nombre,jug->objeto[k+1].nombre); //se quita el objeto de la lista de objetos
                    }
                    jug->contador--; //disminuir el numero de objetos
                }
            }
            if(ok==0) //si no lo tiene en absoluto
            {
                printf("\nRobar un objeto valido!\n");
                printf("\nEl objeto que quieres robar = ");
                scanf("%d",&eleccion); //elige que objeto vas a robar
                ok=0;  //supongamos que no tiene el objeto elegido
                strcpy(elegido,verificar_objeto(eleccion,elegido)); //el objeto elegido es verificado
            }
        }
        if(*ventaja1==1) //si el jugador 1 robo un objeto
        {
            mostrar_objetos(j,jug); //los objetos se muestran de nuevo tras ser robados al jugador 2
        }
        else //si el jugador 2 robo un objeto
        {
            mostrar_objetos(jug,j); //los objetos se muestran de nuevo tras ser robados al jugador 1
        }
        printf("\nEl jugador robo %s!\n",elegido);
    }
    return eleccion;
}


void bala_escopeta(jugador *j, jugador *jug, int *cortado, int *a_salvo, int escopeta[],int *no_balas_de_salva, int *no_balas_de_verdad, int *no_balas, int *ronda, int *no_objeto)
{
    if(*no_balas!=0) //si la escopeta no esta vacia
    {
        printf("\nA quien disparas?\n");
        printf("\nYo (codigo 0)\n");
        printf("\nOponente (codigo 1)\n");
        int decision; //a quien se le disparara 
        printf("\nCodigo = ");
        scanf("%d",&decision); //elige a quien disparar
        while(decision!=0 && decision!=1) //si elige un numero no valido
        {
            printf("\nElige un codigo valido!\n");
            printf("\nCodigo = ");
            scanf("%d",&decision); //elige a quien disparar
        }
        if(decision==0) //si el jugador se dispara a si mismo
        {
            printf("\nEl jugador se dispara a si!\n");
            Sleep(dormir);
            if(escopeta[0]==1) //si la bala es de verdad 
            {
                if(*cortado==1) //si el serrucho fue utilizado 
                {
                    printf("\nEl jugador perdio 2 vidas!\n");
                    j->vidas=j->vidas-2; //disminuir el numero de vidas por 2
                    (*no_balas_de_verdad)--; //disminuir el numero de balas de verdad
                    Sleep(dormir);
                    printf("\nEl jugador tiene %d vidas!\n",j->vidas);
                }
                else //si no
                {
                    printf("\nEl jugador perdio una vida!\n");
                    j->vidas=j->vidas-1; //disminuir el numero de vidas por 1
                    (*no_balas_de_verdad)--; //disminuir el numero de balas de verdad
                    Sleep(dormir);
                    printf("\nEl jugador tiene %d vidas!\n",j->vidas);
                }
                for(int i=0;i<*no_balas-1;i++)
                {
                    escopeta[i]=escopeta[i+1]; //mover el resto de las balas una posicion hacia atras
                }
                (*no_balas)--; //disminuir el numero de balas 
            }
            else
            {
                printf("\nEl jugador esta a salvo...por ahora!n");
                (*a_salvo)++; //sigue siendo el turno del mismo jugador
                (*no_balas_de_salva)--; //disminuir el numero de balas de salva
                for(int i=0;i<*no_balas-1;i++)
                {
                    escopeta[i]=escopeta[i+1]; //mover el resto de las balas una posicion hacia atras
                }
                (*no_balas)--; //disminuir el numero de balas 
                Sleep(dormir);
                printf("\nEl jugador tiene %d vidas!\n",j->vidas);
            }
        }
        else //si el jugador le dispara al oponente
        {
            printf("\nEl jugador le dispara al oponente!\n");
            Sleep(dormir);
            if(escopeta[0]==1) //si la bala es de verdad 
            {
                if(*cortado==1) //si el serrucho fue utilizado
                {
                    printf("\nEl oponente perdio 2!\n");
                    j->vidas=j->vidas-2; //disminuir el numero de vidas por 2
                    (*no_balas_de_verdad)--; //disminuir el numero de balas de verdad
                    Sleep(dormir);
                    printf("\nEl oponente tiene %d vidas!\n",j->vidas);
                }
                else //si no
                {
                    printf("\nEl oponente perdio una vida!\n");
                    j->vidas=j->vidas-1; //disminuir el numero de vidas por 1
                    (*no_balas_de_verdad)--; //disminuir el numero de balas de verdad
                    Sleep(dormir);
                    printf("\nEl oponente tiene %d vidas!\n",j->vidas);
                }
                for(int i=0;i<*no_balas-1;i++)
                {
                    escopeta[i]=escopeta[i+1]; //el resto de las balas se mueven una posicion hacia atras
                }
                (*no_balas)--; //disminuir el numero de balas
            }
            else
            {
                printf("\nEl oponente esta a salvo...por ahora!\n");
                (*a_salvo)++; //sigue siendo el turno del mismo jugador 
                (*no_balas_de_salva)--; //disminuir el numero de balas de salva
                for(int i=0;i<*no_balas-1;i++)
                {
                    escopeta[i]=escopeta[i+1]; //el resto de las balas se mueven una posicion hacia atras
                }
                (*no_balas)--; //disminuir el numero de balas
                Sleep(dormir);
                printf("\nEl oponente tiene %d vidas!\n",j->vidas);
            }
            mostrar_balas(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas); //se muestran las balas de la escopeta en el archivo “el_orden_de_las_balas”
        }
    }
    else
    {
        switch (*ronda) //segun la ronda de que se trate
        {
            case 1:
                *no_balas=(rand()%balas_escopeta)+2; //numero de balas entre 2 y 4 para la escopeta
                break;
            case 2:
                *no_balas=(rand()%balas_escopeta)+4; //numero de balas entre 4 y 6 para la escopeta
                break;
            case 3:
                *no_balas=(rand()%balas_escopeta)+6; //numero de balas entre 6 y 8 para la escopeta
                break;
        }
        printf("\nRecargar la escopeta...\n");
        escopeta=cargar_escopeta(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas); //se recarga la escopeta
        Sleep(dormir);
        printf("\nNumero de balas = %d\n",*no_balas); //se muestra el numero de balas en la escopeta
        printf("Numero de balas de salva = %d\n",*no_balas_de_salva); //se muestra el numero de balas de verdad en la escopeta 
        printf("Numero de balas de verdad = %d\n",*no_balas_de_verdad); //se muestra el numero de balas de salva en la escopeta
        printf("\nSe reparten objetos...n");
        j=repartir_objetos_jugador(j,no_objeto); //repartir objetos al jugador 1
        jug=repartir_objetos_jugador(jug,no_objeto); //repartir objetos al jugador 2
        mostrar_objetos(j,jug); //se muestran los objetos de ambos jugadores
        Sleep(dormir);
    }
}


void eleccion(int elijo, int *cortado, int *a_salvo, int escopeta[], int *no_balas_de_salva, int *no_balas_de_verdad, int *no_balas, jugador *j, objeto *it, int *ronda, jugador *jug, int *ventaja1, int *ventaja2, int *esposado1, int *esposado2, int *no_objeto)
{
    switch (elijo) //segun el objeto elegido
    {
        case 0:
            serrucho(cortado);
            Sleep(dormir);
            break;
        case 1:
            lupa(escopeta);
            Sleep(dormir);
            break;
        case 2:
            escopeta=cerveza(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas);
            mostrar_balas(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas); //se muestran las balas de la escopeta en el archivo “el_orden_de_las_balas”
            Sleep(dormir);
            break;
        case 3:
            esposas(ventaja1,ventaja2,esposado1,esposado2);
            Sleep(dormir);
            break;
        case 4:
            cigarros(j,ronda);
            Sleep(dormir);
            break;
        case 5:
            escopeta=inversor(escopeta,no_balas_de_salva,no_balas_de_verdad);
            mostrar_balas(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas); //se muestran las balas de la escopeta en el archivo “el_orden_de_las_balas”
            Sleep(dormir);
            break;
        case 6:
            telefono_desechable(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas);
            Sleep(dormir);
            break;
        case 7:
            medicamento_caducado(j,ronda);
            Sleep(dormir);
            break;
        case 8:
            elijo=adrenalina(j,jug,ventaja1,ventaja2);
            Sleep(dormir);
            eleccion(elijo,cortado,a_salvo,escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas,j,it,ronda,jug,ventaja1,ventaja2,esposado1,esposado2,no_objeto);
            break;
        case 9:
            bala_escopeta(j,jug,cortado,a_salvo,escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas,ronda,no_objeto);
            Sleep(dormir);
            break;
    }
}


void seleccionar_objeto(int escopeta[], jugador *j, objeto *it, int *no_balas_de_salva, int *no_balas_de_verdad, int *no_balas, int *ronda, jugador *jug, int *ventaja1, int *ventaja2, int *esposado1, int *esposado2, int *no_objeto)
{
    if(*no_balas==0) //si la escopeta esta vacia
    {
        switch (*ronda) //segun la ronda de que se trate
        {
            case 1:
                *no_balas=(rand()%balas_escopeta)+2; //numero de balas entre 2 y 4 para la escopeta
                break;
            case 2:
                *no_balas=(rand()%balas_escopeta)+4; //numero de balas entre 4 y 6 para la escopeta
                break;
            case 3:
                *no_balas=(rand()%balas_escopeta)+6; //numero de balas entre 6 y 8 para la escopeta
                break;
        }
        printf("\nRecargar la escopeta...\n");
        escopeta=cargar_escopeta(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas); //se recarga la escopeta
        Sleep(dormir);
        printf("\nNumero de balas = %d\n",*no_balas); //se muestra el numero de balas de la escopeta
        printf("Numero de balas de salva = %d\n",*no_balas_de_salva); //se muestra el numero de balas de la escopeta
        printf("Numero de balas de verdad = %d\n",*no_balas_de_verdad); //se muestra el numero de balas de la escopeta
        printf("\nSe reparten objetos...\n");
        j=repartir_objetos_jugador(j,no_objeto); //repartir objetos al jugador 1
        jug=repartir_objetos_jugador(jug,no_objeto); //repartir objetos al jugador 2
        mostrar_objetos(j,jug); //se muestran los objetos de ambos jugadores
        Sleep(dormir);
    }
    printf("\nSeleccionar objeto:\n");
    int elijo; //eleccion del objeto
    char elegido[20]; //el objeto elegido
    int ok; //verificar si el jugador tiene el objeto que quiere utilizar
    int cortado=0; //si el jugador
    int a_salvo=0; //si el jugador se disparo a si mismo y esta a salvo
    printf("\nObjeto = ");
    scanf("%d",&elijo); //elije que objeto vas a utilizar
    while(elijo<0 || elijo>9) //si elige un numero no valido
    {
        printf("\nElije un objeto valido!\n");
        printf("\nObjeto = ");
        scanf("%d",&elijo); //elije que objeto vas a utilizar
    }
    if(elijo!=9) //a no ser que la escopeta fuera elegida
    {
        while(elijo>=0 && elijo<=8) //si elige un objeto valido 
        {
            ok=0; //supongamos que no tiene el objeto elegido
            strcpy(elegido,verificar_objeto(elijo,elegido)); //se verifica el objeto elegido 
            while(ok==0) //mientras el jugador no tenga el objeto elegido
            {
                for(int i=1;i<=j->contador;i++)
                {
                    if(strcmp(elegido,j->objeto[i-1].nombre)!=0) //si el jugador no tiene el objeto elegido
                    {
                        continue; //si no lo tiene, continue
                    }
                    else //si lo tiene
                    {
                        ok=1; //se verifica que lo tiene
                    }
                }
                if(ok==0) //si no lo tiene en absoluto
                {
                    printf("\nElije un objeto valido!\n");
                    printf("\nObjeto = ");
                    scanf("%d",&elijo); //elije que objeto vas a utilizar 
                    ok=0;  //supongamos que no tiene el objeto elegido
                    strcpy(elegido,verificar_objeto(elijo,elegido)); //se verifica el objeto elegido
                }
            }
            printf("\nEl jugador eligio %s!\n",elegido);
            Sleep(dormir);
            for(int i=1;i<=j->contador;i++)
            {
                if(strcmp(elegido,j->objeto[i-1].nombre)!=0) //si no se encuentra el objeto elegido
                {
                    continue; //continua
                }
                else //si se encuentra
                {
                    for(int k=i-1;k<j->contador-1;k++)
                    {
                        strcpy(j->objeto[k].nombre,j->objeto[k+1].nombre); //se quita el objeto de la lista de objetos
                    }
                    j->contador--; //disminuir el numero de objetos
                    break;
                }
            }
            if(*ventaja1==1) //si el jugador 1 ha utilizado un objeto
            {
                mostrar_objetos(j,jug); //se muestran los objetos de nuevo despues de que el jugador 2 haya utilizado un objeto
            }
            else //si el jugador 2 ha utilizado un objeto
            {
                mostrar_objetos(jug,j); //se muestran los objetos de nuevo despues de que el jugador 1 haya utilizado un objeto
            }
            }
            eleccion(elijo,&cortado,&a_salvo,escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas,j,it,ronda,jug,ventaja1,ventaja2,esposado1,esposado2,no_objeto); //se utiliza el objeto elegido
            printf("\nSeleccionar objeto:\n");
            printf("\nObjeto = ");
            scanf("%d",&elijo); //elije que objeto vas a utilizar
            while(elijo<0 || elijo>9) //si eliges un numero no valido
            {
                printf("\nElige un objeto valido!\n");
                printf("\nObjeto = ");
                scanf("%d",&elijo); //elije que objeto vas a utilizar
            }
        }
    strcpy(elegido,verificar_objeto(elijo,elegido)); //se elige la escopeta
    printf("\nEl jugador eligio %s!\n",elegido);
    Sleep(dormir);
    eleccion(elijo,&cortado,&a_salvo,escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas,j,it,ronda,jug,ventaja1,ventaja2,esposado1,esposado2,no_objeto); //se utiliza la escopeta
    if(*no_balas==0) //si la escopeta esta vacia
    {
        switch (*ronda) //segun la ronda de que se trate
        {
            case 1:
                *no_balas=(rand()%balas_escopeta)+2; //numero de balas entre 2 y 4 para la escopeta                
                break;
            case 2:
                *no_balas=(rand()%balas_escopeta)+4; //numero de balas entre 4 y 6 para la escopeta
                break;
            case 3:
                *no_balas=(rand()%balas_escopeta)+6; //numero de balas entre 6 y 8 para la escopeta
                break;
        }
        printf("\nRecargar la escopeta...\n");
        escopeta=cargar_escopeta(escopeta,no_balas_de_salva,no_balas_de_verdad,no_balas); //se recarga la escopeta
        Sleep(dormir);
        printf("\nNumero de balas = %d\n",*no_balas); //se muestra el numero de balas en la escopeta
        printf("Numero de balas de salva = %d\n",*no_balas_de_salva); //se muestra el numero de balas de verdad en la escopeta
        printf("Numero de balas de verdad = %d\n",*no_balas_de_verdad); //se muestra el numero de balas de salva en la escopeta
        printf("\nSe reparten objetos...\n");
        j=repartir_objetos_jugador(j,no_objeto); //repartir objetos al jugador 1
        jug=repartir_objetos_jugador(jug,no_objeto); //repartir objetos al jugador 2
        mostrar_objetos(j,jug); //se muestran los objetos de ambos jugadores
        Sleep(dormir);
    }
    if(a_salvo==1) //si el jugador se dispara a si mismo con una bala de salva
    {
        seleccionar_objeto(escopeta,j,it,no_balas_de_salva,no_balas_de_verdad,no_balas,ronda,jug,ventaja1,ventaja2,esposado1,esposado2,no_objeto); //el jugador vuelve a elegir un objeto
    }
}


int main()
{
    int ronda=0; //ronda
    int resultado1=0; //resultado jugador 1
    int resultado2=0; //resultado jugador 2
    while(ronda<3) //mientras no se hayan jugado 3 rondas
    {
        ronda++; //ronda actual
        int no_objeto; //el numero de objetos que recibiran los jugadores
        int no_balas; //numero de balas en la escopeta (al azar)
        int no_balas_de_salva=0; //numero de balas de salva en la escopeta
        int no_balas_de_verdad=0; //numero de balas de verdad en la escopeta
        printf("\nRONDA %d\n",ronda);
        jugador *j1=(jugador*)malloc(sizeof(jugador)); //asignar memoria al apuntador jugador1
        if(!j1)
        {
            perror("\nNo se ha podido asignar memoria!\n");
            exit(2);
        }
        jugador *j2=(jugador*)malloc(sizeof(jugador)); //asignar memoria al apuntador jugador2
        if(!j2)
        {
            perror("\nNo se ha podido asignar memoria!\n");
            exit(2);
        }
        objeto *it=(objeto*)malloc(max_objeto*sizeof(objeto)); //asignar memoria al apuntador objeto 
        if(!it)
        {
            perror("\nNo se ha podido asignar memoria!\n");
            exit(2);
        }
        srand(time(0));
        switch (ronda) //si es la primera ronda
        {
            case 1:
                j1->vidas=max_vidas_r1; //3 vidas
                j1->contador=0;
                j2->vidas=max_vidas_r1; //3 vidas
                j2->contador=0;
                no_objeto=max_objeto_r1; //los jugadores recibiran 2 objetos
                no_balas=(rand()%balas_escopeta)+2; //numero de balas entre 2 y 4 para la escopeta
                break;
            case 2:
                j1->vidas=max_vidas_r2; //4 vidas
                j1->contador=0;
                j2->vidas=max_vidas_r2; //4 vidas
                j2->contador=0;
                no_objeto=max_objeto_r2; //los jugadores recibiran 3 objetos
                no_balas=(rand()%balas_escopeta)+4; //numero de balas entre 4 y 6 para la escopeta
                break;
            case 3:
                j1->vidas=max_vidas_r3; //6 vidas
                j1->contador=0;
                j2->vidas=max_vidas_r3; //6 vidas
                j2->contador=0;
                no_objeto=max_objeto_r3; //los jugadores recibiran 4 objetos
                no_balas=(rand()%balas_escopeta)+6; //numero de balas entre 6 y 8 para la escopeta
                break;
        }
        Sleep(dormir);
        int *escopeta=(int*)malloc(no_balas*sizeof(int)); //asignar memoria al apuntador escopeta
        if(!escopeta)
        {
            perror("\nNo se ha podido asignar memoria!\n");
            exit(2);
        }
        escopeta=cargar_escopeta(escopeta,&no_balas_de_salva,&no_balas_de_verdad,&no_balas); //se recarga la escopeta
        printf("\nNumero de balas = %d\n",no_balas); //se muestra el numero de balas en la escopeta
        printf("Numero de balas de salva = %d\n",no_balas_de_salva); //se muestra el numero de balas de verdad en la escopeta
        printf("Numero de balas de verdad = %d\n",no_balas_de_verdad); //se muestra el numero de balas de salva en la escopeta
        Sleep(dormir);
        printf("\nSe reparten objetos...\n");
        j1=repartir_objetos_jugador(j1,&no_objeto); //repartir objetos al jugador 1
        j2=repartir_objetos_jugador(j2,&no_objeto); //repartir objetos al jugador 2
        mostrar_objetos(j1,j2); //se muestran los objetos de ambos jugadores
        Sleep(dormir);
        int jug1=rand()%2;
        int jug2=rand()%2;
        int decision=rand()%2;
        int ventaja1=0; //si es el turno del jugador 1
        int ventaja2=0; //si es el turno del jugador 2
        int atado1=0; //si el jugador 1 estara esposado
        int atado2=0; //si el jugador 2 estara esposado
        printf("\nSe decide quien empieza...\n");
        while(jug1==jug2) //se decide al azar quien empieza
        {
            jug1=rand()%2;
            jug2=rand()%2;
        }
        Sleep(dormir);
        if(decision==jug1)
        {
            ventaja1=1; //turno del jugador 1
        }
        else
        {
            ventaja2=1; //turno del jugador 2
        }
        if(ventaja1==1) //empieza el jugador 1
        {
            printf("\nEmpieza el jugador 1\n");
            while(j1->vidas>0 && j2->vidas>0) //mientras ningun jugador este muerto
            {
                if(atado1==0) //si el jugador 1 no ha sido esposado
                {
                    ventaja1=1; //es el turno del jugador 1
                    ventaja2=0; //el turno del jugador 2 ha pasado
                    if(j1->vidas>0) //si el jugador 1 no esta muerto
                    {
                        printf("\nEl jugador 1 elige un objeto!\n");
                        seleccionar_objeto(escopeta,j1,it,&no_balas_de_salva,&no_balas_de_verdad,&no_balas,&ronda,j2,&ventaja1,&ventaja2,&atado1,&atado2,&no_objeto); //el jugador 1 elige un objeto
                    }
                }
                atado1=0; //en caso de que haya estado esposado durante un turno, el efecto pasa
                if(no_balas==0 && (j1->vidas!=0 && j2->vidas!=0)) //si la escopeta esta vacia y no hay ningun jugador muerto
                {
                    switch (ronda) //segun la ronda de que se trate
                    {
                        case 1:
                            no_balas=(rand()%balas_escopeta)+2; //numero de balas entre 2 y 4 para la escopeta
                            break;
                        case 2:
                            no_balas=(rand()%balas_escopeta)+4; //numero de balas entre 4 y 6 para la escopeta
                            break;
                        case 3:
                            no_balas=(rand()%balas_escopeta)+6; //numero de balas entre 6 y 8 para la escopeta
                            break;
                    }
                    printf("\nRecargar la escopeta...\n");
                    escopeta=cargar_escopeta(escopeta,&no_balas_de_salva,&no_balas_de_verdad,&no_balas); //se recarga la escopeta
                    Sleep(dormir);
                    printf("\nNumero de balas = %d\n",no_balas); //se muestra el numero de balas en la escopeta
                    printf("Numero de balas de salva = %d\n",no_balas_de_salva); //se muestra el numero de balas de verdad en la escopeta
                    printf("Numero de balas de verdad = %d\n",no_balas_de_verdad); //se muestra el numero de balas de salva en la escopeta
                    printf("\nSe reparten objetos...\n");
                    j1=repartir_objetos_jugador(j1,&no_objeto); //repartir objetos al jugador 1 al azar
                    j2= repartir_objetos_jugador (j2,&no_objeto); //repartir objetos al jugador 2 al azar
                    mostrar_objetos(j1,j2); //se muestran los objetos de ambos jugadores
                    Sleep(dormir);
                }
                if(atado2==0) //si el jugador 2 no ha sido esposado
                {
                    ventaja1=0; //el turno del jugador 1 ha pasado 
                    ventaja2=1; //es el turno del jugador 2
                    if(j2->vidas>0) //si el jugador 2 no esta muerto
                    {
                        printf("\nEl judagor 2 elige un objeto!\n");
                        seleccionar_objeto(escopeta,j2,it,&no_balas_de_salva,&no_balas_de_verdad,&no_balas,&ronda,j1,&ventaja1,&ventaja2,&atado1,&atado2,&no_objeto); //el jugador 2 elige un objeto
                    }
                }
                atado2=0; //en caso de que haya estado esposado durante un turno, el efecto pasa
                if(no_balas==0 && (j1->vidas!=0 && j2->vidas!=0)) //si la escopeta esta vacia y no hay ningun jugador muerto
                {
                    switch (ronda) //segun la ronda de que se trate
                    {
                        case 1:
                            no_balas=(rand()%balas_escopeta)+2; //numero de balas entre 2 y 4 para la escopeta
                            break;
                        case 2:
                            no_balas=(rand()%balas_escopeta)+4; //numero de balas entre 4 y 6 para la escopeta
                            break;
                        case 3:
                            no_balas=(rand()%balas_escopeta)+6; //numero de balas entre 6 y 8 para la escopeta
                            break;
                    }
                    printf("\nRecargar la escopeta...\n");
                    escopeta=cargar_escopeta(escopeta,&no_balas_de_salva,&no_balas_de_verdad,&no_balas); //se recarga la escopeta
                    Sleep(dormir);
                    printf("\nNumero de balas = %d\n",no_balas); //se muestra el numero de balas en la escopeta
                    printf("Numero de balas de salva = %d\n",no_balas_de_salva); //se muestra el numero de balas de verdad en la escopeta
                    printf("Numero de balas de verdad = %d\n",no_balas_de_verdad); //se muestra el numero de balas de salva en la escopeta
                    printf("\nSe reparten objetos...\n");
                    j1=repartir_objetos_jugador(j1,&no_objeto); //repartir objetos al jugador 1
                    j2= repartir_objetos_jugador(j2,&no_objeto); //repartir objetos al jugador 2
                    mostrar_objetos(j1,j2); //se muestran los objetos de ambos jugadores
                    Sleep(dormir);
                }
            }
        }
        else //empieza el jugador 2
        {
            printf("\nEmpieza el jugador 2\n");
            Sleep(dormir);
            while(j1->vidas>0 && j2->vidas>0) //mientras ningun jugador este muerto
            {
                if(atado2==0) //si el jugador 2 no ha sido esposado
                {
                    ventaja1=0; //el turno del jugador 1 ha pasado
                    ventaja2=1; //es el turno del jugador 2
                    if(j2->vidas>0) //si el jugador 2 no esta muerto
                    {
                    printf("\nEl jugador 2 elige un objeto!\n");
                    seleccionar_objeto(escopeta,j2,it,&no_balas_de_salva,&no_balas_de_verdad,&no_balas,&ronda,j1,&ventaja1,&ventaja2,&atado1,&atado2,&no_objeto); //el judagor 2 elige un objeto
                    }
                }
                atado2=0; //en caso de que haya estado esposado durante un turno, el efecto pasa
                if(no_balas==0 && (j1->vidas!=0 && j2->vidas!=0)) //si la escopeta esta vacia y no hay ningun jugador muerto
                {
                    switch (ronda) //segun la ronda de que se trate
                    {
                        case 1:
                            no_balas=(rand()%balas_escopeta)+2; //numero de balas entre 2 y 4 para la escopeta
                            break;
                        case 2:
                            no_balas=(rand()%balas_escopeta)+4; //numero de balas entre 4 y 6 para la escopeta
                            break;
                        case 3:
                            no_balas=(rand()%balas_escopeta)+6; //numero de balas entre 6 y 8 para la escopeta
                            break;
                    }
                    printf("\nRecargar la escopeta...\n");
                    escopeta=cargar_escopeta(escopeta,&no_balas_de_salva,&no_balas_de_verdad,&no_balas); //se recarga la escopeta
                    Sleep(dormir);
                    printf("\nNumero de balas = %d\n",no_balas); //se muestra el numero de balas en la escopeta
                    printf("Numero de balas de salva = %d\n",no_balas_de_salva); //se muestra el numero de balas de verdad en la escopeta
                    printf("Numero de balas de verdad = %d\n",no_balas_de_verdad); //se muestra el numero de balas de salva en la escopeta
                    printf("\nSe reparten objetos...\n");
                    j1=repartir_objetos_jugador(j1,&no_objeto); //repartir objetos al jugador 1
                    j2=repartir_objetos_jugador(j2,&no_objeto); //repartir objetos al jugador 2
                    mostrar_objetos(j1,j2); //se muestran los objetos de ambos jugadores
                    Sleep(dormir);
                }
                if(atado1==0) //si el jugador 1 no ha sido esposado
                {
                    ventaja1=1; //es el turno del jugador 1
                    ventaja2=0; //el turno del jugador 2 ha pasado
                    if(j1->vidas>0) //si el jugador 1 no esta muerto
                    {
                        printf("\nEl jugador 1 elige un objeto!\n");
                        seleccionar_objeto(escopeta,j1,it,&no_balas_de_salva,&no_balas_de_verdad,&no_balas,&ronda,j2,&ventaja1,&ventaja2,&atado1,&atado2,&no_objeto); //el jugador 1 elige un objeto
                    }
                }
                atado1=0; //en caso de que haya estado esposado durante un turno, el efecto pasa
                if(no_balas==0 && (j1->vidas!=0 && j2->vidas!=0)) //si la escopeta esta vacia y no hay ningun jugador muerto
                {
                    switch (ronda) //segun la ronda de que se trate
                    {
                        case 1:
                            no_balas=(rand()%balas_escopeta)+2; //numero de balas entre 2 y 4 para la escopeta
                            break;
                        case 2:
                            no_balas=(rand()%balas_escopeta)+4; //numero de balas entre 4 y 6 para la escopeta
                            break;
                        case 3:
                            no_balas=(rand()%balas_escopeta)+6; //numero de balas entre 6 y 8 para la escopeta
                            break;
                    }
                    printf("\nRecargar la escopeta...\n");
                    escopeta=cargar_escopeta(escopeta,&no_balas_de_salva,&no_balas_de_verdad,&no_balas); //se recarga la escopeta
                    Sleep(dormir);
                    printf("\nNumero de balas = %d\n",no_balas); //se muestra el numero de balas en la escopeta
                    printf("Numero de balas de salva = %d\n",no_balas_de_salva); //se muestra el numero de balas de verdad en la escopeta
                    printf("Numero de balas de verdad = %d\n",no_balas_de_verdad); //se muestra el numero de balas de salva en la escopeta
                    printf("\nSe reparten objetos...\n");
                    j1=repartir_objetos_jugador(j1,&no_objeto); //repartir objetos al jugador 1
                    j2=repartir_objetos_jugador(j2,&no_objeto); //repartir objetos al jugador 2
                    mostrar_objetos(j1,j2); //se muestran los objetos de ambos jugadores
                    Sleep(3000);
                }
            }
        }   
        if(j1->vidas==0) //si el jugador 1 se queda sin vidas 
        {
            printf("\nEl jugador 1 ha muerto!\n");
            Sleep(dormir);
            printf("\nEl jugador 2 ha ganado la ronda!\n");
            resultado2++; //un punto para el jugador 2
        }
        else //si el jugador 2 se queda sin vidas
        {
            printf("\nEl jugador 2 ha muerto!\n");
            Sleep(dormir);
            printf("\nEl jugador 1 ha ganado la ronda!\n");
            resultado1++; //un punto para el jugador 1
        }
        printf("\nRONDA %d ha terminado!\n",ronda);
        Sleep(dormir);
        free(j1); //se libera la memoria del apuntador del jugador1
        free(j2); //se libera la memoria del apuntador del jugador2
        free(it); //se libera la memoria del apuntador del objeto
        free(escopeta); //se libera la memoria del apuntador de la escopeta 
    }
    printf("\nResultado final: %d - %d",resultado1,resultado2); //resultado final
    Sleep(dormir);
    if(resultado1>resultado2) //si el jugador 1 ha ganado varias rondas
    {
        printf("\nGano el jugador 1!\n");
    }
    else //si el jugador 1 ha ganado varias rondas
    {
        printf("\nGano el jugador 2!\n");
    }
    Sleep(dormir);
    return 0;
}
