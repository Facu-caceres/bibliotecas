#ifndef TDA_ARBOL_IDX_H_INCLUDED
#define TDA_ARBOL_IDX_H_INCLUDED

#include <stdio.h>
#include "TDA_Arbol.h" // Se incluye la biblioteca base del árbol

// Estructura para cada registro del índice.
// Contiene el DNI (clave) y el número de registro en el archivo de datos.
typedef struct
{
    long dni;
    int nro_reg;
} t_idx_alu;

// El TDA del índice es en esencia un puntero al árbol.
// Usamos un typedef para mayor claridad.
typedef tArbol tArbolIdx;

// Prototipos de las funciones del TDA de Índice

void crearIndice(tArbolIdx *pidx);

int generarIndice(tArbolIdx *pidx, const char *path_datos, size_t tam_reg_dato,
                  long (*obtener_clave)(const void *), int (*es_activo)(const void *));

int guardarIndiceEnArchivo(const tArbolIdx *pidx, const char *path_idx);

int cargarIndiceDeArchivo(tArbolIdx *pidx, const char *path_idx, size_t tam_reg_idx);

int altaIndice(tArbolIdx *pidx, const void *dato, size_t tam_dato, const char *path_datos,
               long (*obtener_clave)(const void *), int (*cmp_claves)(const void *, const void *));

int bajaIndice(tArbolIdx *pidx, void *dato, size_t tam_dato, const char *path_datos,
                long (*obtener_clave)(const void *), void (*marcar_baja)(void *), int (*cmp_claves)(const void *, const void *));

int buscarEnIndice(const tArbolIdx *pidx, void *clave, t_idx_alu *reg_idx, int (*cmp_claves)(const void *, const void *));

void recorrerIndice(const tArbolIdx *pidx, void (*accion)(void *, size_t, size_t, void *), void *params);



#endif // TDA_ARBOL_IDX_H_INCLUDED
