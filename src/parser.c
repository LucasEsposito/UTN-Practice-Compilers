#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

FILE* stream, *output;
int errores=0;

/// GRAMATICA SINTACTICA

void objetivo(){
    // <objetivo> -> <programa> FDT #terminar
    programa();
    match(FDT);
    terminar();
}
void programa(){
    // <programa> -> #comenzar INICIO <listaSentencias> FIN
    comenzar();
    match(INICIO);
    lista_sentencias();
    match(FIN);
}

void lista_sentencias(){
    // <lista_sentencias> -> <sentencia> {<sentencia>}
    sentencia();
    while (1) {
    switch (prox_token()) {
        case ID:case LEER:case ESCRIBIR:
        sentencia();
        break;default:return;
        }
    }
}

void sentencia() {
    // <sentencia> -> <identificador> ASIGNACION <expresion> #asignar PUNTOYCOMA | LEER PARENIZQUIERDO <lista_identificadores> PARENDERECHO PUNTOYCOMA | ESCRIBIR PARENIZQUIERDO <lista_expresiones> PARENDERECHO PUNTOYCOMA
	token tok = prox_token();
	struct reg_expr z,var;
	switch (tok) {
		case ID: /* <sentencia> -> <identificador> ASIGNACION <expresion> #asignar PUNTOYCOMA */
			strcpy(var.nombre,yytext);
			identificador(&var);
			match(ASIGNACION);
			expresion(&z);
			match(PUNTOYCOMA);
			asignar(&var,&z);
		break;
		case LEER: /* <sentencia> -> LEER PARENIZQUIERDO <lista_identificadores> PARENDERECHO PUNTOYCOMA */
			match(LEER);
			match(PARENIZQUIERDO);
			lista_identificadores();
			match(PARENDERECHO);
			match(PUNTOYCOMA);
		break;
		case ESCRIBIR: /* <sentencia> -> ESCRIBIR PARENIZQUIERDO <listaExpresiones> PARENDERECHO PUNTOYCOMA */
			match(ESCRIBIR);
			match(PARENIZQUIERDO);
			lista_expresiones();
			match(PARENDERECHO);
			match(PUNTOYCOMA);
		break;
		default:error_sintactico(tok); break;
	}
}

void lista_identificadores(){
    // <lista_identificadores> -> <identificador> #leer_id {COMA <identificador> #leer_id}
    struct reg_expr z;
    identificador(&z);
    leer_id(&z);
    while (prox_token()==COMA){
        match(COMA);
        identificador(&z);
        leer_id(&z);
    }
}

void identificador(struct reg_expr *preg){
    // <identificador> -> ID #procesar_id
    match(ID);
    (*preg)=procesar_id();
}

void lista_expresiones(){
    // <lista_expresiones> -> <expresion> #escribir_exp {COMA <expresion> #escribir_exp}
    struct reg_expr z;
    expresion(&z);
    escribir_exp(&z);
    while (prox_token()==COMA){
        match(COMA);
        expresion(&z);
        escribir_exp(&z);
    }
}
void expresion (struct reg_expr *preg) {
    // <expresion> -> <termino> {<operador_aditivo> <termino> #gen_infijo}
    token t;
    struct reg_expr a,b;
    struct reg_op op;
    termino(&a);
    for (t = prox_token(); t == SUMA || t == RESTA; t = prox_token()) {
        operador_aditivo(&op); termino(&b);
    a=gen_infijo(&a,&op,&b);
    }
    strcpy(preg->nombre,a.nombre);
}

void termino(struct reg_expr *preg){
    // <termino> -> <primaria> {<operador_multiplicativo> <primaria> #gen_infijo}
    token t;
    struct reg_expr a,b;
    struct reg_op op;
    primaria(&a);
    for (t = prox_token(); t == MULTIPLICACION || t == DIVISION; t = prox_token()) {
        operador_multiplicativo(&op);
        primaria(&b);
    a=gen_infijo(&a,&op,&b);
    }
    strcpy(preg->nombre,a.nombre);
}

void primaria(struct reg_expr *preg){
    // <primaria> -> <identificado> | CONSTANTE #procesar_cte | PARENIZQUIERDO <expresion> PARENDERECHO
    token tok = prox_token();
    switch(tok){
        case ID: identificador(preg); break;
        case CONSTANTE: match(CONSTANTE); (*preg)=procesar_cte();  break;
        case PARENIZQUIERDO:
            match(PARENIZQUIERDO);
            expresion(preg);
            match(PARENDERECHO);
            break;
        default: error_sintactico(tok); break;
    }
}

void operador_aditivo(struct reg_op *preg){
    // <operador_aditivo> -> SUMA #procesar_op | RESTA #procesar_op
    token tok;
    tok=prox_token();
    switch(tok){
        case SUMA: match(SUMA); (*preg)=procesar_op(); break;
        case RESTA: match(RESTA); (*preg)=procesar_op(); break;
        default: error_sintactico(tok); break;
    }
}

void operador_multiplicativo(struct reg_op *preg){
    // <operador_multiplicativo> -> MULTIPLICACION #procesar_op | DIVISION #procesar_op
    token tok;
    tok=prox_token();
    switch(tok){
        case MULTIPLICACION: match(MULTIPLICACION); (*preg)=procesar_op(); break;
        case DIVISION: match(DIVISION); (*preg)=procesar_op(); break;
        default: error_sintactico(tok); break;
    }
}
