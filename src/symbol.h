#ifndef SYMBOL_H_INCLUDED
#define SYMBOL_H_INCLUDED

#include "scanner.h"

struct reg_expr {
	token clase;
	char nombre[TAMLEX];
	int valor;
};


extern struct ts_entry tabla_simb[];

int buscar(char *id, struct ts_entry *ts, token *tok);
void colocar(char *id, struct ts_entry *ts, token tok);
int chequear(struct reg_expr* reg);
void inicializarDiccionario(struct ts_entry *ts);
void graficarDiccionario(struct ts_entry *ts);
void agregarPalabrasReservadas(struct ts_entry *ts);

#endif // SYMBOL_H_INCLUDED
