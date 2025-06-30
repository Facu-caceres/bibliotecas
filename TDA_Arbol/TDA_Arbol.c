#include "TDA_Arbol.h"

#define MINIMO(X,Y)((X)<(Y)?(X):(Y))

tNodoArbol **MayorNodoArbolBin(const tArbol *p);
tNodoArbol **MenorNodoArbolBin(const tArbol *p);
tNodoArbol **MayorRecNodoArbolBin(const tArbol *p);
tNodoArbol **MenorRecNodoArbolBin(const tArbol *p);
tNodoArbol **BuscarNodoArbolBin(const tArbol *p, const void *d, int (*cmp)(const void*,const void*));
tNodoArbol **BuscarRecNodoArbolBin(const tArbol *p, const void *d, int (*cmp)(const void*,const void *));

int EsCompletoHastaNivelArbolBin(const tArbol *p, int n);
int EsAVL2CalculoArbolBin(const tArbol *p);

const tArbol *MayorNodoNoClaveArbolBin(const tArbol *p, const tArbol *mayor, int (*cmp)(const void*,const void *));
const tArbol *MenorNodoNoClaveArbolBin(const tArbol *p, const tArbol *menor, int (*cmp)(const void*,const void *));
const tArbol *BuscarNodoNoClaveArbolBin(const tArbol *p, const void *d, int (*cmp)(const void*,const void*));

void CrearArbolBin(tArbol *p)
{
    *p = NULL;
}

/// Reservar Memoria Nodo
#define RESERVARMEMORIANODO( X , Y , Z , W ) (                  \
                ( (X) = (typeof( X ))malloc( Y ) ) == NULL ||   \
                ( (Z) = malloc( W ) ) == NULL ?                 \
                            free( X ), 0 : 1 )

int InsertarArbolBin(tArbol *p, const void *d, size_t tam, int (*cmp)(const void*,const void*))
{
    tNodoArbol *nue;
    int rc;

    while(*p)
    {
        if((rc = cmp(d, (*p)->info)) < 0)
            p = &(*p)->izq;
        else if(rc > 0)
            p = &(*p)->der;
        else
            return CLA_DUP;
    }
    if(!RESERVARMEMORIANODO(nue, sizeof(tNodoArbol), nue->info, tam))
        return SIN_MEM;
    nue->tamInfo = tam;
    memcpy(nue->info, d, tam);
    nue->der = nue->izq = NULL;
    *p = nue;
    return TODO_BIEN;
}

int InsertarRecArbolBin(tArbol *p, const void *d, size_t tam, int (*cmp)(const void*, const void*))
{
    tNodoArbol *nue;
    int rc;

    if(*p)
    {
        if((rc = cmp(d, (*p)->info)) < 0)
            return InsertarRecArbolBin(&(*p)->izq, d, tam, cmp);
        if(rc > 0)
            return InsertarRecArbolBin(&(*p)->der, d, tam, cmp);
        return CLA_DUP;
    }
    if(!RESERVARMEMORIANODO(nue, sizeof(tNodoArbol), nue->info, tam))
        return SIN_MEM;
    nue->tamInfo = tam;
    memcpy(nue->info, d, tam);
    nue->der = nue->izq = NULL;
    *p = nue;
    return TODO_BIEN;
}

void RecorrerEnOrdenRecArbolBin(const tArbol *p, size_t n, void *params, void (*accion)(void*,size_t,size_t,void*))
{
    if(!*p)
        return;
    RecorrerEnOrdenRecArbolBin(&(*p)->izq, n+1, params, accion);
    accion((*p)->info, (*p)->tamInfo, n, params);
    RecorrerEnOrdenRecArbolBin(&(*p)->der, n+1, params, accion);
}

void RecorrerEnOrdenArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,size_t,void*))
{
    RecorrerEnOrdenRecArbolBin(p, 0, params, accion);
}

void RecorrerEnOrdenInversoRecArbolBin(const tArbol *p, size_t n, void *params, void (*accion)(void *, size_t,size_t,void *))
{
    if(!*p)
        return;
    RecorrerEnOrdenInversoRecArbolBin(&(*p)->der, n+1, params, accion);
    accion((*p)->info, (*p)->tamInfo, n, params);
    RecorrerEnOrdenInversoRecArbolBin(&(*p)->izq, n+1, params, accion);
}

void RecorrerEnOrdenInversoArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,size_t,void*))
{
    RecorrerEnOrdenInversoRecArbolBin(p, 0, params, accion);
}

void RecorrerPreOrdenRecArbolBin(const tArbol *p, size_t n, void *params, void (*accion)(void*,size_t,size_t,void*))
{
   if(!*p)
        return;
   accion((*p)->info, (*p)->tamInfo, n, params);
   RecorrerPreOrdenRecArbolBin(&(*p)->izq, n+1, params, accion);
   RecorrerPreOrdenRecArbolBin(&(*p)->der, n+1, params, accion);
}

void RecorrerPreOrdenArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,size_t,void*))
{
    RecorrerPreOrdenRecArbolBin(p, 0, params, accion);
}

void RecorrerPosOrdenRecArbolBin(const tArbol *p, size_t n, void *params, void (*accion)(void*,size_t,size_t,void*))
{
    if(!*p)
        return;
    RecorrerPosOrdenRecArbolBin(&(*p)->izq, n+1, params, accion);
    RecorrerPosOrdenRecArbolBin(&(*p)->der, n+1, params, accion);
    accion((*p)->info, (*p)->tamInfo, n, params);
}

void RecorrerPosOrdenArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,size_t,void*))
{
    RecorrerPosOrdenRecArbolBin(p, 0, params, accion);
}

void RecorrerEnOrdenSimpleArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,void*))
{
    if(!*p)
        return;
    RecorrerEnOrdenSimpleArbolBin(&(*p)->izq, params, accion);
    accion((*p)->info,(*p)->tamInfo, params);
    RecorrerEnOrdenSimpleArbolBin(&(*p)->der, params, accion);
}

void RecorrerPreOrdenSimpleArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,void*))
{
    if(!*p)
        return;
    accion((*p)->info, (*p)->tamInfo, params);
    RecorrerPreOrdenSimpleArbolBin(&(*p)->izq, params, accion);
    RecorrerPreOrdenSimpleArbolBin(&(*p)->der, params, accion);
}

void RecorrerPosOrdenSimpleArbolBin(const tArbol *p, void *params, void (*accion)(void*,size_t,void*))
{
    if(!*p)
        return;
    RecorrerPosOrdenSimpleArbolBin(&(*p)->izq, params, accion);
    RecorrerPosOrdenSimpleArbolBin(&(*p)->der, params, accion);
    accion((*p)->info, (*p)->tamInfo, params);
}

int EliminarRaizArbolBin(tArbol *p)
{
    tNodoArbol **remp, *elim;
    if(!*p)
        return 0; ///ARBOL_VACIO
    free((*p)->info);
    if(!(*p)->izq && !(*p)->der)
    {
        free(*p);
        *p = NULL;
        return 1; /// OK
    }
    remp = AlturaArbolBin(&(*p)->izq)>AlturaArbolBin(&(*p)->der) ? MayorNodoArbolBin(&(*p)->izq) : MenorNodoArbolBin(&(*p)->der);
    elim = *remp;
    (*p)->info = elim->info;
    (*p)->tamInfo = elim->tamInfo;
    *remp = elim->izq ? elim->izq : elim->der;
    free(elim);
    return 1; /// OK
}

int EliminarElemArbolBin(tArbol *p, void *d, size_t tam, int (*cmp)(const void*,const void*))
{
    if(!(p = BuscarNodoArbolBin(p, d, cmp)))
        return 0; ///NO_EXISTE
    memcpy(d, (*p)->info, MINIMO(tam, (*p)->tamInfo));
    return EliminarRaizArbolBin(p);
}

int BuscarElemArbolBin(const tArbol *p, void *d, size_t tam, int (*cmp)(const void*,const void*))
{
    if(!(p = BuscarNodoArbolBin(p, d, cmp)))
        return 0; ///NO_EXISTE
    memcpy(d, (*p)->info, MINIMO(tam, (*p)->tamInfo));
    return 1; ///OK
}

size_t LeerDesdeArchivoBin(void **d, void *pf, size_t pos, void *params)
{
    size_t tam = *((int*)params);
    *d = malloc(tam);
    if(!*d)
        return 0;
    fseek((FILE*)pf,pos*tam,SEEK_SET);
    return fread(*d, tam, 1, (FILE*)pf);
}

int CargarDesdeDatosOrdenadosRecArbolBin(tArbol *p, void *ds, size_t (*leer)(void **,void*,size_t,void*params),int li, int ls, void *params)
{
    int m = (li+ls)/2, r;
    if(li>ls)
        return TODO_BIEN;
    (*p) = (tNodoArbol*) malloc(sizeof(tNodoArbol));
    if(!*p || !((*p)->tamInfo = leer(&(*p)->info, ds, m, params)))
    {
        free(*p);
        return SIN_MEM;
    }
    (*p)->izq = (*p)->der = NULL;
    if((r = CargarDesdeDatosOrdenadosRecArbolBin(&(*p)->izq, ds, leer, li, m-1, params)) != TODO_BIEN)
        return r;
    return CargarDesdeDatosOrdenadosRecArbolBin(&(*p)->der, ds, leer, m+1, ls, params);
}

int CargarArchivoBinOrdenadoAbiertoArbolBin(tArbol *p, FILE *pf, size_t tamInfo)
{
    int cantReg;
    if(*p || !pf)
        return 0;
    fseek(pf, 0L, SEEK_END);
    cantReg = ftell(pf)/tamInfo;
    return CargarDesdeDatosOrdenadosRecArbolBin(p, pf, LeerDesdeArchivoBin, 0, cantReg-1, &tamInfo);
}

int CargarArchivoBinOrdenadoArbolBin(tArbol *p, const char *path, size_t tamInfo)
{
    int cantReg, r;
    FILE *pf;
    if(*p)
        return SIN_INICIALIZAR;
    if(!(pf = fopen(path, "rb")))
        return ERROR_ARCH;
    fseek(pf, 0L, SEEK_END);
    cantReg = ftell(pf)/tamInfo;
    r = CargarDesdeDatosOrdenadosRecArbolBin(p, pf, LeerDesdeArchivoBin, 0, cantReg-1, &tamInfo);
    fclose(pf);
    return r;
}

int CargarDesdeDatosOrdenadosArbolBin(tArbol *p, void *ds, size_t cantReg, size_t (*leer)(void**,void*,size_t,void *params), void *params)
{
    if(*p || !ds)
        return 0;
    return CargarDesdeDatosOrdenadosRecArbolBin(p, ds, leer, 0, cantReg-1, params);
}

tNodoArbol **BuscarNodoArbolBin(const tArbol *p, const void *d, int (*cmp)(const void*, const void *))
{
    int rc;
    while(*p && (rc = cmp(d, (*p)->info)))
    {
        if(rc<0)
            p = &(*p)->izq;
        else
            p = &(*p)->der;
    }
    if(!*p)
        return NULL;
    return (tNodoArbol **)p;
}

tNodoArbol ** BuscarRecNodoArbolBin(const tArbol * p, const void * d, int (*cmp)(const void*, const void*))
{
    int rc;
    if(!*p)
        return NULL;
    if(*p && (rc=cmp(d, (*p)->info)))
    {
        if(rc<0)
            return BuscarRecNodoArbolBin(&(*p)->izq, d, cmp);
        return BuscarRecNodoArbolBin(&(*p)->der, d, cmp);
    }
    return (tNodoArbol **) p;
}

tNodoArbol ** MayorNodoArbolBin(const tArbol * p)
{
    if(!*p)
        return NULL;
    while((*p)->der)
        p = &(*p)->der;
    return (tNodoArbol **) p;
}

tNodoArbol ** MenorNodoArbolBin(const tArbol * p)
{
    if(!*p)
        return NULL;
    while((*p)->izq)
        p = &(*p)->izq;
    return (tNodoArbol **) p;
}

tNodoArbol ** MayorRecNodoArbolBin(const tArbol * p)
{
    if(!*p)
        return NULL;
    if(!(*p)->der)
        return (tNodoArbol **) p;
    return MayorRecNodoArbolBin(&(*p)->der);
}

tNodoArbol ** MenorRecNodoArbolBin(const tArbol * p)
{
    if(!*p)
        return NULL;
    if(!(*p)->izq)
        return (tNodoArbol **) p;
    return MenorRecNodoArbolBin(&(*p)->izq);
}

int MayorElemNoClaveArbolBin(const tArbol *p, void *d, size_t tam, int (*cmp)(const void *, const void *))
{
    const tArbol * mayor = p;
    if (!*p)
        return 0;
    mayor = MayorNodoNoClaveArbolBin(&(*p)->izq, mayor, cmp);
    mayor = MayorNodoNoClaveArbolBin(&(*p)->der, mayor, cmp);
    memcpy(d, (*mayor)->info, MINIMO(tam, (*mayor)->tamInfo));
    return 1;
}

const tArbol * MayorNodoNoClaveArbolBin(const tArbol *p, const tArbol *mayor, int (*cmp)(const void *, const void *))
{
    if (!*p)
        return mayor;
    if (cmp((*p)->info, (*mayor)->info) > 0)
        mayor = p;
    mayor = MayorNodoNoClaveArbolBin(&(*p)->izq, mayor, cmp);
    mayor = MayorNodoNoClaveArbolBin(&(*p)->der, mayor, cmp);
    return mayor;
}

int MenorElemNoClaveArbolBin(const tArbol *p, void *d, size_t tam, int (*cmp)(const void *, const void *))
{
    const tArbol * menor = p;
    if (!*p)
        return 0;
    menor = MenorNodoNoClaveArbolBin(&(*p)->izq, menor, cmp);
    menor = MenorNodoNoClaveArbolBin(&(*p)->der, menor, cmp);
    memcpy(d, (*menor)->info, MINIMO(tam, (*menor)->tamInfo));
    return 1;
}

const tArbol * MenorNodoNoClaveArbolBin(const tArbol *p, const tArbol *menor, int (*cmp)(const void *, const void *))
{
    if (!*p)
        return menor;
    if (cmp((*p)->info, (*menor)->info) < 0)
        menor = p;
    menor = MenorNodoNoClaveArbolBin(&(*p)->izq, menor, cmp);
    menor = MenorNodoNoClaveArbolBin(&(*p)->der, menor, cmp);
    return menor;
}

int BuscarElemNoClaveArbolBin(const tArbol *p, void *d, size_t tam, int (*cmp)(const void*, const void*))
{
    const tArbol *busq = NULL;
    if(!*p)
        return 0;
    if(cmp((*p)->info, d) == 0)
        busq = p;
    else if((busq = BuscarNodoNoClaveArbolBin(&(*p)->izq, d, cmp)) == NULL)
        busq = BuscarNodoNoClaveArbolBin(&(*p)->der, d, cmp);
    if(busq == NULL)
        return 0;
    memcpy(d, (*busq)->info, MINIMO(tam, (*busq)->tamInfo));
    return 1;
}

const tArbol * BuscarNodoNoClaveArbolBin(const tArbol *p, const void *d, int (*cmp)(const void *, const void *))
{
    const tArbol * busq = NULL;
    if (!*p)
        return NULL;
    if (cmp((*p)->info, d) < 0)
        return p;
    if ((busq = BuscarNodoNoClaveArbolBin(&(*p)->izq, busq, cmp)))
        return busq;
    return BuscarNodoNoClaveArbolBin(&(*p)->der, busq, cmp);
}

size_t AlturaArbolBin(const tArbol *p)
{
    int hi, hd;
    if (!*p)
        return 0;
    hi = AlturaArbolBin(&(*p)->izq);
    hd = AlturaArbolBin(&(*p)->der);
    return (hi > hd ? hi : hd) + 1;
}

size_t CantNodosArbolBin(const tArbol *p)
{
    if (!*p)
        return 0;
    return CantNodosArbolBin(&(*p)->izq) + CantNodosArbolBin(&(*p)->der) + 1;
}

int MayorElemArbolBin(const tArbol *p, void *d, size_t tam)
{
    if (!(p = MayorNodoArbolBin(p)))
        return 0;
    memcpy(d, (*p)->info, MINIMO(tam, (*p)->tamInfo));
    return 1;
}

int MenorElemArbolBin(const tArbol *p, void *d, size_t tam)
{
    if (!(p = MenorNodoArbolBin(p)))
        return 0;
    memcpy(d, (*p)->info, MINIMO(tam, (*p)->tamInfo));
    return 1;
}

size_t CantNodosHastaNivelArbolBin(const tArbol *p, int n)
{
    if(!*p)
        return 0;
    if(n==0)
        return 1;
    return CantNodosHastaNivelArbolBin(&(*p)->izq, n-1) + CantNodosHastaNivelArbolBin(&(*p)->der, n-1) + 1;
}

int EsCompletoHastaNivelArbolBin(const tArbol *p, int n)
{
    if(!*p)
        return n<0;
    if(n==0)
        return 1;
    return EsCompletoHastaNivelArbolBin(&(*p)->izq,n-1) && EsCompletoHastaNivelArbolBin(&(*p)->der,n-1);
}

int EsCompletoArbolBin(const tArbol *p)
{
    return EsCompletoHastaNivelArbolBin(p, AlturaArbolBin(p)-1);
}

int EsBalanceadoArbolBin(const tArbol *p)
{
    return EsCompletoHastaNivelArbolBin(p,AlturaArbolBin(p)-2);
}

int EsAVLArbolBin(const tArbol *p)
{
    int hi,hd;
    if(!*p)
        return 1;
    hi = AlturaArbolBin(&(*p)->izq);
    hd = AlturaArbolBin(&(*p)->der);
    if(abs(hi-hd)>1)
        return 0;
    return EsAVLArbolBin(&(*p)->izq) && EsAVLArbolBin(&(*p)->der);
}

int EsCompleto2ArbolBin(const tArbol *p)
{
    int h = AlturaArbolBin(p);
    return CantNodosArbolBin(p) == pow(2,h)-1;
}

int EsBalanceado2ArbolBin(const tArbol *p)
{
    int h = AlturaArbolBin(p);
    return CantNodosHastaNivelArbolBin(p,h-2) == pow(2,h-1)-1;
}

int EsAVL2CalculoArbolBin(const tArbol *p)
{
    int hi,hd;
    if(!*p)
        return 0;
    hi = EsAVL2CalculoArbolBin(&(*p)->izq);
    hd = EsAVL2CalculoArbolBin(&(*p)->der);
    if(hi<0 || hd<0 || abs(hi-hd)>1)
        return -1;
    return (hi>hd ? hi : hd) + 1;
}

int EsAVL2ArbolBin(const tArbol *p)
{
    return EsAVL2CalculoArbolBin(p)>=0;
}

void EliminarElemNoClaveArbolBin(tArbol *p, void *d, int (*cmp)(const void *, const void *))
{
    if (!*p)
        return;
    EliminarElemNoClaveArbolBin(&(*p)->izq, d, cmp);
    EliminarElemNoClaveArbolBin(&(*p)->der, d, cmp);
    if (!cmp((*p)->info, d))
        EliminarRaizArbolBin(p);
}

void vaciarArbol(tArbol *p)
{
    if(!*p)
        return;

    vaciarArbol(&(*p)->izq);
    vaciarArbol(&(*p)->der);
    free((*p)->info);
    free(*p);
    *p = NULL;
}

/////Inserta en el arbol, pero sin reservar memoria par ael dato,
/////referencia directo el parametro
//int insertarArbolBinBusqDirecto(tArbolBinBusq *p,
//                                const void *d,
//                                unsigned tam,
//                                int (*cmp)(const void *, const void *))
//{
//    tNodoArbol *nue;
//    int         rc;
//
//    while(*p)
//    {
//        if((rc = cmp(d, (*p)->info)) < 0)
//            p = &(*p)->izq;
//        else if (rc > 0)
//            p = &(*p)->der;
//        else
//            return CLA_DUP;
//    }
//    nue = (tNodoArbol*) malloc(sizeof(tNodoArbol));
//    if(!nue)
//        return SIN_MEM;
//    nue->tamInfo = tam;
//    nue->info = (void*) d;
//    nue->der = nue->izq = NULL;
//    *p = nue;
//    return TODO_BIEN;
//}
//
//int cargarDesdeArchivoPreOrdenArbol(tArbolBinBusq * p,
//                                    const char * path,
//                                    unsigned tamInfo,
//                                    int (*cmp)(const void *, const void *))
//{
//    int cantReg,
//        r;
//    FILE * pf;
//    if(*p)
//        return SIN_INICIALIZAR;
//    if(!(pf= fopen(path, "rb")))
//        return ERROR_ARCH;
//    fseek(pf, 0L, SEEK_END);
//    cantReg = ftell(pf)/tamInfo;
//    r = cargarDesdeDatosPreOrdenArbol(p, pf, cantReg, leerDesdeArchivoBin, cmp, tamInfo);
//    fclose(pf);
//    return r;
//}
//
//int cargarDesdeDatosPreOrdenArbol(tArbolBinBusq *p,
//                        void *ds, unsigned cantReg,
//                        int (*leer)(void*, void *, unsigned, size_t tam),
//                        int (*cmp)(const void *, const void *),
//                        size_t tam)
//{
//    int ri;
//    unsigned i;
//    void * dato = malloc(tam);
//    if (!dato)
//        return SIN_MEM;
//
//    for(i=0; i<cantReg; i++)
//    {
//        leer(dato, ds, i, tam);
//        if ((ri=insertarArbolBinBusqDirecto(p, dato, tam, cmp))!=TODO_BIEN)
//        {
//            return ri;
//        }
//    }
//    return TODO_BIEN;
//}
