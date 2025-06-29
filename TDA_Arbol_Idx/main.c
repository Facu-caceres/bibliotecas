#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TDA_Arbol_Idx.h"

// Archivos a utilizar
#define ARCH_ALUMNOS "alumnos.dat"
#define ARCH_INDICE  "alumnos.idx"

// Estructura del registro de datos (alumno)
typedef struct
{
    long dni;
    char apynom[50];
    char estado; // 'A' = Activo, 'B' = Baja
} t_alu;

// Funciones callback para el TDA de índice
long obtener_dni(const void *alu) { return ((t_alu *)alu)->dni; }
int es_alumno_activo(const void *alu) { return ((t_alu *)alu)->estado == 'A'; }
void marcar_alumno_baja(void *alu) { ((t_alu *)alu)->estado = 'B'; }
int cmp_claves_dni(const void* idx1, const void* idx2) { return ((t_idx_alu*)idx1)->dni - ((t_idx_alu*)idx2)->dni; }

// Función para mostrar un alumno (usada en el listado)
void mostrar_alumno_desde_archivo(long nro_reg)
{
    FILE* pf = fopen(ARCH_ALUMNOS, "rb");
    if(!pf) return;

    t_alu alu;
    fseek(pf, nro_reg * sizeof(t_alu), SEEK_SET);
    fread(&alu, sizeof(t_alu), 1, pf);
    printf("DNI: %ld | Nombre: %s | Estado: %c\n", alu.dni, alu.apynom, alu.estado);

    fclose(pf);
}

// Acción para el recorrido en orden del índice
void listar_en_orden(void* info, size_t tam, size_t n, void* params)
{
    t_idx_alu* reg_idx = (t_idx_alu*)info;
    mostrar_alumno_desde_archivo(reg_idx->nro_reg);
}

void dar_alta(tArbolIdx *pIndice);
void dar_baja(tArbolIdx *pIndice);
void crear_archivo_inicial();

int main()
{
    tArbolIdx indice;
    crearIndice(&indice);

    // Opcional: Crear un archivo de datos de prueba si no existe
    FILE *f = fopen(ARCH_ALUMNOS, "rb");
    if (!f) {
        crear_archivo_inicial();
    } else {
        fclose(f);
    }

    // Cargar índice desde archivo, si existe. Si no, generarlo.
    if (!cargarIndiceDeArchivo(&indice, ARCH_INDICE, sizeof(t_idx_alu)))
    {
        printf("No se pudo cargar el indice '%s'. Generandolo desde '%s'...\n", ARCH_INDICE, ARCH_ALUMNOS);
        generarIndice(&indice, ARCH_ALUMNOS, sizeof(t_alu), obtener_dni, es_alumno_activo);
    } else {
        printf("Indice '%s' cargado en memoria.\n", ARCH_INDICE);
    }

    // Menú de opciones
    int opt;
    do
    {
        printf("\n--- MENU ---\n");
        printf("1. Dar de Alta un Alumno\n");
        printf("2. Dar de Baja un Alumno\n");
        printf("3. Listar Alumnos por DNI (en orden)\n");
        printf("0. Salir y Guardar Indice\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opt);

        switch (opt)
        {
        case 1:
            dar_alta(&indice);
            break;
        case 2:
            dar_baja(&indice);
            break;
        case 3:
            printf("\n--- Listado de Alumnos Activos (ordenado por DNI) ---\n");
            recorrerIndice(&indice, listar_en_orden, NULL);
            break;
        }
    } while (opt != 0);


    // Guardar el índice en disco antes de salir
    printf("Guardando indice en '%s'...\n", ARCH_INDICE);
    guardarIndiceEnArchivo(&indice, ARCH_INDICE);

    printf("Programa finalizado.\n");

    return 0;
}

void dar_alta(tArbolIdx *pIndice)
{
    t_alu nuevo_alu;
    printf("\n--- Alta de Alumno ---\n");
    printf("Ingrese DNI: ");
    scanf("%ld", &nuevo_alu.dni);
    printf("Ingrese Apellido y Nombre: ");
    fflush(stdin); // Limpiar buffer
    gets(nuevo_alu.apynom);
    nuevo_alu.estado = 'A';

    if (altaIndice(pIndice, &nuevo_alu, sizeof(t_alu), ARCH_ALUMNOS, obtener_dni, cmp_claves_dni))
    {
        printf("Alta exitosa!\n");
    }
    else
    {
        printf("Error: El DNI %ld ya existe.\n", nuevo_alu.dni);
    }
}

void dar_baja(tArbolIdx *pIndice)
{
    t_alu alu_a_bajar;
    printf("\n--- Baja de Alumno ---\n");
    printf("Ingrese DNI del alumno a dar de baja: ");
    scanf("%ld", &alu_a_bajar.dni);

    if (bajaIndice(pIndice, &alu_a_bajar, sizeof(t_alu), ARCH_ALUMNOS, obtener_dni, marcar_alumno_baja, cmp_claves_dni))
    {
        printf("Baja exitosa! El alumno con DNI %ld fue marcado como inactivo.\n", alu_a_bajar.dni);
    }
    else
    {
        printf("Error: No se encontro un alumno activo con el DNI %ld.\n", alu_a_bajar.dni);
    }
}

void crear_archivo_inicial()
{
    FILE* pf = fopen(ARCH_ALUMNOS, "wb");
    if(!pf) {
        printf("No se pudo crear el archivo de alumnos.\n");
        return;
    }
    t_alu alumnos[] = {
        {30123456, "Perez, Juan", 'A'},
        {32987654, "Gonzalez, Maria", 'A'},
        {28111222, "Lopez, Carlos", 'B'}, // Dado de baja
        {35000111, "Martinez, Ana", 'A'}
    };
    fwrite(alumnos, sizeof(alumnos), 1, pf);
    fclose(pf);
    printf("Archivo '%s' de prueba creado.\n", ARCH_ALUMNOS);
}
