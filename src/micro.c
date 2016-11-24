/*
 ============================================================================
 Name        : micro.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"



int main(int argc, char *argv[]) {
    /* Para debug */ // argc=3;argv[1]="prueba.txt";argv[2]="salida.txt";

    if( argc == 3 ) printf("Los argumentos son: %s %s\n\n", argv[1],argv[2]);
    else if( argc > 3 ) { printf("ERROR: Demasiados argumentos\n\n"); return 1;}
    else {printf("ERROR: Se esperan dos archivo de texto como argumento.\n\n"); return 2;}

    stream = fopen(argv[1],"r");
    output = fopen(argv[2],"w");

    inicializarDiccionario(tabla_simb);
    agregarPalabrasReservadas(tabla_simb);

    objetivo();

    fclose(stream);
    fclose(output);

    printf("\nFinalizo el archivo con %d errores.\n",errores);

    /* Para debug */ //graficarDiccionario(tabla_simb);
    return 0;
}
