#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"
#include "error.h"
#include "semantic.h"

int lineas=1;
char * yytext;
static char buffer[TAMLEX];
struct ts_entry tabla_simb[TAMDIC];
FILE* stream;

char* tok_names[]={"INICIO","FIN","LEER","ESCRIBIR","ID","CONSTANTE","PARENIZQUIERDO","PARENDERECHO","PUNTOYCOMA","COMA","ASIGNACION","SUMA","RESTA","MULTIPLICACION","DIVISION","COMENTARIO","FDT","ERRORLEXICO","ERRORASIG","ERRORCTE"};

int pedirAScanner=1;
token tokenActual;

char* token_name(token tok)
{
	return tok_names[tok];
}

token prox_token(){ // proximo token UTIL
    if (pedirAScanner==1) {
        do {
        tokenActual=scanner();
        } while (tokenActual==COMENTARIO);
        pedirAScanner=0;
        }
    return tokenActual;
}
void match(token tok_esperado)
{
	token tok_real = prox_token();
    if (tok_real!=tok_esperado)
    {
    	error_sintactico(tok_real);
    }
    pedirAScanner=1;
}


enum {letra, digito, signoMas, signoMenos, parenIzq, parenDer, coma, puntoYComa, dosPuntos, igual, fdt, espacio, otro, signoPor, signoDiv, saltoLinea};

int tipoDeCaracter(char c){
    if (( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ))   {return letra;}
    else if ( c >= '0' && c <= '9' )                            {return digito;}
    else if (c == '+')                                          {return signoMas;}
    else if (c == '-')                                          {return signoMenos;}
    else if (c == '(')                                          {return parenIzq;}
    else if (c == ')')                                          {return parenDer;}
    else if (c == ',')                                          {return coma;}
    else if (c == ';')                                          {return puntoYComa;}
    else if (c == ':')                                          {return dosPuntos;}
    else if (c == '=')                                          {return igual;}
    else if (c == EOF)                                          {return fdt;}
    else if (c == ' ' || c == '	')                       		{return espacio;} //el primero es un espacio y el segundo una tabulacion!
    else if (c == '*')                                          {return signoPor;}
    else if (c == '/')                                          {return signoDiv;}
    else if (c =='\n')                                          {return saltoLinea;}
    else                                                        {return otro;}
}

token tipoDeToken(int estado){
    switch (estado){
        case 2: return ID;
        case 4: return CONSTANTE;
        case 5: return SUMA;
        case 6: return RESTA;
        case 7: return PARENIZQUIERDO;
        case 8: return PARENDERECHO;
        case 9: return COMA;
        case 10: return PUNTOYCOMA;
        case 12: return ASIGNACION;
        case 13: return FDT;
        case 14: return MULTIPLICACION;
        case 20: return DIVISION;
        case 16: return ERRORLEXICO;
        case 17: return ERRORASIG;
        case 19: return ERRORCTE;
        case 22: return COMENTARIO;
        default: return (token)-1;
    }
}

int estadoAceptor(int estado){
    return (estado==2 || (estado>=4 && estado<=10) || (estado>=12 && estado<=14) || (estado>=16 && estado<=17) || (estado>=19 && estado<=20) || estado==22);
}

int estadoCentinela(int estado){
    return (estado==2 || estado==4 || estado==19 || estado==20 || estado==22);
}

int caracterIgnorado(char caracter){
    return (caracter==' ' || caracter=='\n');
}

void iniciarBuffer(char* b){
    b[0]='\0';
}

void insertarEnBuffer(char* b,int* s,char c){
    b[(*s)++]=c;
    b[*s]='\0';
}

static int tabla[23][16]={
        //      Let Dig  +   -   (   )   ,   ;   :   =  fdt esp otr  *   /  /n
        /*0- */{ 1,  3,  5,  6,  7,  8,  9, 10 , 11, 17, 13, 0, 17, 14, 15,  0}, // Inicial
        /*1  */{ 1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2}, // Reconociendo identificador
        /*2+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Identificador reconocido (UNGETC)
        /*3  */{18,  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4}, // Reconociendo constante
        /*4+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Constante reconocida (UNGETC)
        /*5+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Op. Aditivo "+" reconocido
        /*6+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Op. Aditivo "-" reconocido
        /*7+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Caracter de puntuacion "(" reconocido
        /*8+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Caracter de puntuacion ")" reconocido
        /*9+ */{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Caracter de puntuacion ", " reconocido
        /*10+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Caracter de puntuacion ";" reconocido
        /*11 */{17, 17, 17, 17, 17, 17, 17, 17, 17, 12, 17, 17, 17, 17, 17, 17}, // Reconociendo asignacion
        /*12+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Asignacion reconocida
        /*13+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // FDT reconocido
        /*14+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Op. Multiplicativo "*" reconocido
        /*15 */{20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21, 20}, // Reconociendo Op.Multiplicativo "/" o Comentario
        /*16+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Error comun
        /*17+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Error de asignacion
        /*18 */{18, 18, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19}, // Constante erronea
        /*19+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Error de constante (UNGETC)
        /*20+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Op. Multiplicativo "/" reconocido (UNGETC)
        /*21 */{21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 21, 21, 21, 21, 22}, // Reconociendo Comentario
        /*22+*/{99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99}, // Comentario Completo (UNGETC)
    };



token scanner(){
    char c;
    int es, estado=0, buffer_size=0;
    token tok;
    iniciarBuffer(buffer);
    yytext=buffer;
    while(!estadoAceptor(estado)){
        c = (char)fgetc(stream);
        es=tipoDeCaracter(c);
        estado=tabla[estado][es];
        if (estadoCentinela(estado))
            ungetc(c,stream);
        else
            if (!caracterIgnorado(c))
                insertarEnBuffer(buffer,&buffer_size,c);
            else if (es==saltoLinea) lineas++;
    }

    tok=tipoDeToken(estado);

    if (tok==ID)
        buscar(buffer,tabla_simb,&tok);

    /* Para debug */ //printf("Token %s  Lexema %s fila %d \n",token_name(tok),buffer,lineas);

    return tok;
}

