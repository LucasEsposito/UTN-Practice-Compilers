#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include "scanner.h"

int estado_de_error(token tok);
void error_lexico(token tok);
void error_comun(token tok);
void error_asignacion(token tok);
void error_cte(token tok);
void error_sintactico(token tok);

#endif // ERROR_H_INCLUDED
