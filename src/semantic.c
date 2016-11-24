#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

int temporales=0;


/* RUTINAS SEMANTICAS */

void generar(char *op, char *a, char *b, char *c){
    fprintf(output,"%s %s %s %s\n",op,a,b,c);
    /* Para debug */ //printf("%s %s %s %s\n",op,a,b,c);
}

void terminar(){
    generar("TERMINAR","","","");
    }

void comenzar(){
    generar("COMENZAR","","","");
}

void asignar(struct reg_expr *vder, struct reg_expr *vizq){
    generar("ASIGNACION",vizq->nombre,vder->nombre,"");
}

void leer_id(struct reg_expr *pin){
    generar("LEER",pin->nombre,"","");
}
void escribir_exp(struct reg_expr *pout){
    generar("ESCRIBIR",pout->nombre,"","");
}




/* FUNCIONES QUE RETORNAN ESTRUCTURAS */

struct reg_expr gen_infijo(struct reg_expr *pei, struct reg_op *op, struct reg_expr *ped){
    struct reg_expr temporal;
    char temp[32];
    sprintf(temp,"%s%d","temporal",++temporales);
    strcpy(temporal.nombre,temp);
    generar("DECLARAR",temp,"INTEGER","");
    generar(token_name(op->cod_oper),pei->nombre,ped->nombre,temp);
    return temporal;
}


struct reg_expr procesar_id()
{
    struct reg_expr id;
    strcpy(id.nombre,yytext);
    if (chequear(&id))
    {
        generar("DECLARAR",id.nombre,"INTEGER","");
    }
    return id;
}

struct reg_expr procesar_cte(){
    struct reg_expr cons;
    strcpy(cons.nombre,yytext);
    return cons;
}

struct reg_op procesar_op(){
    struct reg_op op;
    op.cod_oper=tokenActual;
    return op;
}
