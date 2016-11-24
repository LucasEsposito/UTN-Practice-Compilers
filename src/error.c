#include <stdio.h>
#include <stdlib.h>
#include "error.h"

/* ERRORES LEXICOS */
int estado_de_error(token tok)
{
	return (tok==ERRORLEXICO || tok==ERRORASIG || tok==ERRORCTE)?1:0;
}



void error_lexico(token tok)
{
	if(estado_de_error(tok))
	{
		switch ((int)tok)
		{
			case ERRORLEXICO: error_comun(tok); break;
			case ERRORASIG: error_asignacion(tok); break;
			case ERRORCTE: error_cte(tok); break;
			default: break;
		}
		errores++;
	}
}


void error_comun(token tok)
{
	printf("Error lexico comun en la linea %d en el token %s.\n",lineas, token_name(tok));
}

void error_asignacion(token tok)
{
	printf("Error lexico de asignacion en la linea %d en el token %s.\n",lineas, token_name(tok));
}

void error_cte(token tok)
{
	printf("Error lexico: constante mal formada en la linea %d.\n",lineas);
}



/* ERRORES SINTACTICOS */
void error_sintactico(token tok)
{
    printf("Error sintactico en la linea %d. Token %s inesperado.\n",lineas, token_name(tok));
    errores++;
}
