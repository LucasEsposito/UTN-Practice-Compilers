#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

int buscar(char *id, struct ts_entry *ts, token *tok){
    int i;
    for (i=0;i<TAMDIC;i++){
        if (strcmp(ts[i].lexema,id)==0){
        *tok=ts[i].tok;
        return 1;
        }
        else if (ts[i].lexema[0]=='\0')
            break;
    }
    return 0;
}

void colocar(char *id, struct ts_entry *ts, token tok){
    int i;
    for (i=0;i<TAMDIC;i++)
    {
        if (ts[i].lexema[0]=='\0')
        {
            strcpy(ts[i].lexema,id);
            ts[i].tok=tok;
            break;
        }
    }
}
/*
 * struct ts_entry{
   char lexema[TAMLEX];
   token tok;
 */

int chequear(struct reg_expr* reg)
{
	if (!buscar(reg->nombre,tabla_simb,&reg->clase))
	{
		colocar(reg->nombre,tabla_simb,ID);
		return 1;
	}
	return 0;
}

void inicializarDiccionario(struct ts_entry *ts){
    int i;
    for (i=0;i<TAMDIC;i++) {
        ts[i].lexema[0]='\0';
        ts[i].tok=(token)0;
    }
}

void graficarDiccionario(struct ts_entry *ts){
    int i;
    printf("\n\nTABLA DE SIMBOLOS\n");
    for (i=0;i<TAMDIC;i++) {
        if (ts[i].lexema[0]=='\0')
            break;
        printf("%d %32s %s\n",i,ts[i].lexema,token_name(ts[i].tok));
        }
}

void agregarPalabrasReservadas(struct ts_entry *ts){
    colocar("inicio",ts,INICIO);
    colocar("fin",ts,FIN);
    colocar("leer",ts,LEER);
    colocar("escribir",ts,ESCRIBIR);
}
