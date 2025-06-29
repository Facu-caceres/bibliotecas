#include "TDA_Arbol_Idx.h"

// Función de comparación interna para el árbol, basada en el DNI.
int cmp_indices(const void *idx1, const void *idx2)
{
    return ((t_idx_alu *)idx1)->dni - ((t_idx_alu *)idx2)->dni;
}

// Wrapper para la acción de guardar en el archivo de índice.
void guardar_reg_idx(void *info, size_t tam, size_t n, void *params)
{
    fwrite(info, tam, 1, (FILE *)params);
}


void crearIndice(tArbolIdx *pidx)
{
    CrearArbolBin(pidx);
}

int generarIndice(tArbolIdx *pidx, const char *path_datos, size_t tam_reg_dato,
                  long (*obtener_clave)(const void *), int (*es_activo)(const void *))
{
    FILE *pf_datos = fopen(path_datos, "rb");
    if (!pf_datos)
    {
        return ERROR_ARCH;
    }

    void *reg_dato = malloc(tam_reg_dato);
    if (!reg_dato)
    {
        fclose(pf_datos);
        return SIN_MEM;
    }

    t_idx_alu reg_idx;
    int nro_reg = 0;
    int res = TODO_BIEN;

    fread(reg_dato, tam_reg_dato, 1, pf_datos);
    while (!feof(pf_datos))
    {
        if (es_activo(reg_dato))
        {
            reg_idx.dni = obtener_clave(reg_dato);
            reg_idx.nro_reg = nro_reg;
            res = InsertarArbolBin(pidx, &reg_idx, sizeof(t_idx_alu), cmp_indices);
            if (res != TODO_BIEN)
            {
                break; // Salir si hay un error (ej. clave duplicada)
            }
        }
        nro_reg++;
        fread(reg_dato, tam_reg_dato, 1, pf_datos);
    }

    free(reg_dato);
    fclose(pf_datos);
    return res;
}

int guardarIndiceEnArchivo(const tArbolIdx *pidx, const char *path_idx)
{
    FILE *pf_idx = fopen(path_idx, "wb");
    if (!pf_idx)
    {
        return ERROR_ARCH;
    }

    RecorrerEnOrdenArbolBin(pidx, pf_idx, guardar_reg_idx);

    fclose(pf_idx);
    return TODO_BIEN;
}

int cargarIndiceDeArchivo(tArbolIdx *pidx, const char *path_idx, size_t tam_reg_idx)
{
    return CargarArchivoBinOrdenadoArbolBin(pidx, path_idx, tam_reg_idx);
}

int altaIndice(tArbolIdx *pidx, const void *dato, size_t tam_dato, const char *path_datos,
               long (*obtener_clave)(const void *), int (*cmp_claves)(const void *, const void *))
{
    FILE *pf_datos = fopen(path_datos, "ab"); // Abrir para añadir al final
    if (!pf_datos)
    {
        return ERROR_ARCH;
    }

    // Calcular el número de registro
    fseek(pf_datos, 0, SEEK_END);
    long nro_reg = ftell(pf_datos) / tam_dato;

    // Crear el registro de índice
    t_idx_alu reg_idx;
    reg_idx.dni = obtener_clave(dato);
    reg_idx.nro_reg = nro_reg;

    // Insertar en el árbol
    int res = InsertarArbolBin(pidx, &reg_idx, sizeof(t_idx_alu), cmp_claves);

    if (res == TODO_BIEN)
    {
        // Si se insertó en el árbol, escribir el dato en el archivo
        fwrite(dato, tam_dato, 1, pf_datos);
    }

    fclose(pf_datos);
    return res;
}

int bajaIndice(tArbolIdx *pidx, void *dato, size_t tam_dato, const char *path_datos,
                long (*obtener_clave)(const void *), void (*marcar_baja)(void *), int (*cmp_claves)(const void *, const void *))
{
    t_idx_alu reg_idx;
    reg_idx.dni = obtener_clave(dato);

    // Eliminar del árbol/índice
    if (EliminarElemArbolBin(pidx, &reg_idx, sizeof(t_idx_alu), cmp_claves))
    {
        // Si se eliminó, marcar como baja en el archivo de datos
        FILE *pf_datos = fopen(path_datos, "r+b");
        if (!pf_datos)
        {
            // Opcional: Manejar error, quizás re-insertando en el árbol.
            return ERROR_ARCH;
        }

        fseek(pf_datos, reg_idx.nro_reg * tam_dato, SEEK_SET);
        fread(dato, tam_dato, 1, pf_datos);
        marcar_baja(dato);
        fseek(pf_datos, -((long)tam_dato), SEEK_CUR);
        fwrite(dato, tam_dato, 1, pf_datos);

        fclose(pf_datos);
        return TODO_BIEN;
    }

    return 0; // No se encontró el elemento
}

int buscarEnIndice(const tArbolIdx *pidx, void *clave_a_buscar, t_idx_alu *reg_idx_encontrado, int (*cmp_claves)(const void *, const void *))
{
    t_idx_alu aux;
    aux.dni = *(long*)clave_a_buscar; // Asumimos que clave_a_buscar es un puntero a un long (DNI)

    if (BuscarElemArbolBin(pidx, &aux, sizeof(t_idx_alu), cmp_claves)) {
        *reg_idx_encontrado = aux;
        return TODO_BIEN;
    }
    return 0; // No encontrado
}

void recorrerIndice(const tArbolIdx *pidx, void (*accion)(void *, size_t, size_t, void *), void *params)
{
    RecorrerEnOrdenArbolBin(pidx, params, accion);
}
