#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

typedef enum { INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
      PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA,
      MULTIPLICACION, DIVISION, COMENTARIO, FDT,ERRORLEXICO,
      ERRORASIG, ERRORCTE
}token;

#define TAMLEX 32+1
#define TAMDIC 32+1

struct ts_entry{
   char lexema[TAMLEX];
   token tok; /* tok=0, 1, 2, 3 Palabra Reservada, tok=ID=4 Identificador */
};

extern FILE* stream;
token scanner();
extern int errores;
extern token tokenActual;
extern char * yytext;
extern int lineas;
extern char *tok_names[]; //={"INICIO","FIN","LEER","ESCRIBIR","ID","CONSTANTE","PARENIZQUIERDO","PARENDERECHO","PUNTOYCOMA","COMA","ASIGNACION","SUMA","RESTA","MULTIPLICACION","DIVISION","COMENTARIO","FDT","ERRORLEXICO","ERRORASIG","ERRORCTE"};
char* token_name(token tok);
token prox_token(void);
void match(token tok);

#endif // SCANNER_H_INCLUDED

