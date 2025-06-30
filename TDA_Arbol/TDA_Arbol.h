#ifndef TDA_ARBOL_H_INCLUDED
#define TDA_ARBOL_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define CLA_DUP 0
#define SIN_MEM 0
#define SIN_INICIALIZAR 0
#define ERROR_ARCH 0
#define TODO_BIEN 1

typedef struct sNodoArbol
{
    void *info;
    size_t tamInfo;
    struct sNodoArbol *izq,*der;
}tNodoArbol;

typedef tNodoArbol *tArbol;

void CrearArbolBin(tArbol *p);
void vaciarArbol(tArbol *p);
int InsertarArbolBin(tArbol *p, const void *d, size_t tam, int (*cmp)(const void*,const void *));
int InsertarRecArbolBin(tArbol *p, const void *d, size_t tam, int (*cmp)(const void*,const void*));
void RecorrerEnOrdenArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,size_t,void*));
void RecorrerEnOrdenInversoArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,size_t,void*));
void RecorrerPreOrdenArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,size_t,void*));
void RecorrerPosOrdenArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,size_t,void*));
void RecorrerEnOrdenSimpleArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,void*));
void RecorrerPreOrdenSimpleArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,void*));
void RecorrerPosOrdenSimpleArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,void*));
int EliminarRaizArbolBin(tArbol *p);
int EliminarElemArbolBin(tArbol *p, void *d, size_t tam, int (*cmp)(const void*,const void*));
int BuscarElemArbolBin(const tArbol *p, void *d, size_t tam, int (*cmp)(const void*,const void*));
int CargarArchivoBinOrdenadoAbiertoArbolBin(tArbol *p, FILE *pf, size_t tamInfo);
int CargarArchivoBinOrdenadoArbolBin(tArbol *p, const char *path, size_t tamInfo);
int CargarDesdeDatosOrdenadosArbolBin(tArbol *p, void *ds, size_t cantReg, size_t (*leer)(void **,void *,size_t,void *params), void *params);
int MayorElemNoClaveArbolBin(const tArbol *p, void *d, size_t tam, int (*cmp)(const void*,const void*));
int MenorElemNoClaveArbolBin(const tArbol *p, void *d, size_t tam, int (*cmp)(const void*,const void*));
int BuscarElemNoClaveArbolBin(const tArbol *p, void *d, size_t tam, int (*cmp)(const void*,const void*));

///Utils

size_t AlturaArbolBin(const tArbol *p);
size_t CantNodosArbolBin(const tArbol *p);
size_t CantNodosHastaNivelArbolBin(const tArbol *p, int n);
int MayorElemArbolBin(const tArbol *p, void *d, size_t tam);
int MenorElemArbolBin(const tArbol *p, void *d, size_t tam);
int EsCompletoArbolBin(const tArbol *p);
int EsBalanceadoArbolBin(const tArbol *p);
int EsAVLArbolBin(const tArbol *p);
int EsCompleto2ArbolBin(const tArbol *p);
int EsBalanceado2ArbolBin(const tArbol *p);
int EsAVL2ArbolBin(const tArbol *p);

#endif // TDA_ARBOL_H_INCLUDED
