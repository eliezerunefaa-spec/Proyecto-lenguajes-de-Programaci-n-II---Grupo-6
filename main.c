#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RUTINAS     50          
#define MAX_EJERCICIO   50          
#define ARCHIVO_DATOS   "rutinas.dat" 

typedef struct {
    char dia[15];               
    char ejercicio[MAX_EJERCICIO]; 
    int  minutos;               
} Rutina;

Rutina rutinas[MAX_RUTINAS];    
int totalRutinas = 0;           

void mostrarMenu(void);
void registrarRutina(void);
void mostrarTodasLasRutinas(void);
void buscarRutinaPorDia(void);
void editarRutina(void);
void eliminarRutina(void);
void mostrarTotalMinutos(void);
void mostrarCreditos(void);
void cargarDatos(void);
void guardarDatos(void);
void limpiarBuffer(void);
void pausar(void);
int  diaValido(const char *dia);

int main(void) {
    int opcion;

    
    cargarDatos();

    
    do {
        mostrarMenu();

        
        printf("  Ingrese su opcion: ");
        if (scanf("%d", &opcion) != 1) {
            opcion = 0; 
        }
        limpiarBuffer();

        
        switch (opcion) {
            case 1: registrarRutina();        break;
            case 2: mostrarTodasLasRutinas(); break;
            case 3: buscarRutinaPorDia();     break;
            case 4: editarRutina();           break;
            case 5: eliminarRutina();         break;
            case 6: mostrarTotalMinutos();    break;
            case 7: mostrarCreditos();        break;
            case 8:
                printf("\n  Saliendo del sistema... Hasta pronto!\n\n");
                break;
            default:
                printf("\n  [!] Opcion invalida. Intente de nuevo.\n");
                pausar();
        }

    } while (opcion != 8);

    return 0;
}

void mostrarMenu(void) {
    
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("============================================================\n");
    printf("        REGISTRO DE RUTINAS DE ENTRENAMIENTO                \n");
    printf("============================================================\n");
    printf("  Rutinas registradas: %d / %d\n", totalRutinas, MAX_RUTINAS);
    printf("------------------------------------------------------------\n");
    printf("  1. Registrar rutina\n");
    printf("  2. Mostrar todas las rutinas\n");
    printf("  3. Buscar rutina por dia\n");
    printf("  4. Editar rutina\n");
    printf("  5. Eliminar rutina\n");
    printf("  6. Total de minutos entrenados\n");
    printf("  7. Creditos\n");
    printf("  8. Salir\n");
    printf("------------------------------------------------------------\n");
}

void registrarRutina(void) {
    printf("\n============================================================\n");
    printf("                  REGISTRAR RUTINA                         \n");
    printf("============================================================\n");

    
    if (totalRutinas >= MAX_RUTINAS) {
        printf("\n  [!] Limite maximo alcanzado (%d rutinas).\n", MAX_RUTINAS);
        pausar();
        return;
    }

    Rutina nueva; 

    
    do {
        printf("  Dia de la semana (Lunes/Martes/Miercoles/Jueves/Viernes): ");
        fgets(nueva.dia, sizeof(nueva.dia), stdin);
        nueva.dia[strcspn(nueva.dia, "\n")] = '\0'; 

        if (!diaValido(nueva.dia)) {
            printf("  [!] Dia invalido. Solo se permiten dias de Lunes a Viernes.\n");
        }
    } while (!diaValido(nueva.dia));

    
    printf("  Nombre del ejercicio: ");
    fgets(nueva.ejercicio, sizeof(nueva.ejercicio), stdin);
    nueva.ejercicio[strcspn(nueva.ejercicio, "\n")] = '\0';

    
    do {
        printf("  Duracion en minutos: ");
        if (scanf("%d", &nueva.minutos) != 1 || nueva.minutos <= 0) {
            printf("  [!] Ingrese un numero positivo de minutos.\n");
            limpiarBuffer();
            nueva.minutos = 0;
        } else {
            limpiarBuffer();
        }
    } while (nueva.minutos <= 0);

    
    rutinas[totalRutinas] = nueva;
    totalRutinas++;

    
    guardarDatos();

    printf("\n  [OK] Rutina registrada exitosamente.\n");
    pausar();
}

void mostrarTodasLasRutinas(void) {
    printf("\n============================================================\n");
    printf("               TODAS LAS RUTINAS                           \n");
    printf("============================================================\n");

    if (totalRutinas == 0) {
        printf("  No hay rutinas registradas todavia.\n");
        pausar();
        return;
    }

    
    printf("  %-4s %-12s %-30s %-8s\n", "No.", "Dia", "Ejercicio", "Minutos");
    printf("  -----------------------------------------------------------\n");

    
    for (int i = 0; i < totalRutinas; i++) {
        printf("  %-4d %-12s %-30s %-8d\n",
               i + 1,
               rutinas[i].dia,
               rutinas[i].ejercicio,
               rutinas[i].minutos);
    }

    printf("  -----------------------------------------------------------\n");
    printf("  Total de rutinas: %d\n", totalRutinas);
    pausar();
}

void buscarRutinaPorDia(void) {
    char diaBuscado[15];
    int encontradas = 0;

    printf("\n============================================================\n");
    printf("               BUSCAR RUTINA POR DIA                       \n");
    printf("============================================================\n");

    printf("  Dia a buscar (Lunes/Martes/Miercoles/Jueves/Viernes): ");
    fgets(diaBuscado, sizeof(diaBuscado), stdin);
    diaBuscado[strcspn(diaBuscado, "\n")] = '\0';

    printf("\n  Rutinas para el dia: %s\n", diaBuscado);
    printf("  -----------------------------------------------------------\n");

    
    for (int i = 0; i < totalRutinas; i++) {
        #ifdef _WIN32
            int igual = (_stricmp(rutinas[i].dia, diaBuscado) == 0);
        #else
            int igual = (strcasecmp(rutinas[i].dia, diaBuscado) == 0);
        #endif

        if (igual) {
            printf("  [%d] Ejercicio: %-30s | %d minutos\n",
                   i + 1, rutinas[i].ejercicio, rutinas[i].minutos);
            encontradas++;
        }
    }

    if (encontradas == 0) {
        printf("  No se encontraron rutinas para ese dia.\n");
    } else {
        printf("  -----------------------------------------------------------\n");
        printf("  Total encontradas: %d\n", encontradas);
    }

    pausar();
}

void editarRutina(void) {
    int numero;

    printf("\n============================================================\n");
    printf("                  EDITAR RUTINA                            \n");
    printf("============================================================\n");

    if (totalRutinas == 0) {
        printf("  No hay rutinas para editar.\n");
        pausar();
        return;
    }

    
    mostrarTodasLasRutinas();

    printf("  Numero de rutina a editar (1 - %d): ", totalRutinas);
    if (scanf("%d", &numero) != 1) {
        limpiarBuffer();
        printf("  [!] Entrada invalida.\n");
        pausar();
        return;
    }
    limpiarBuffer();

    
    if (numero < 1 || numero > totalRutinas) {
        printf("  [!] Numero fuera de rango.\n");
        pausar();
        return;
    }

    int idx = numero - 1; 

    printf("\n  Editando: %s - %s (%d min)\n",
           rutinas[idx].dia, rutinas[idx].ejercicio, rutinas[idx].minutos);
    printf("  (Presione Enter para conservar el valor actual)\n\n");

    
    char temp[50];
    printf("  Nuevo dia [%s]: ", rutinas[idx].dia);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0 && diaValido(temp)) {
        strcpy(rutinas[idx].dia, temp);
    } else if (strlen(temp) > 0) {
        printf("  [!] Dia invalido, se conserva el anterior.\n");
    }

    
    printf("  Nuevo ejercicio [%s]: ", rutinas[idx].ejercicio);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        strcpy(rutinas[idx].ejercicio, temp);
    }

    
    printf("  Nuevos minutos [%d]: ", rutinas[idx].minutos);
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    if (strlen(temp) > 0) {
        int nuevosMin = atoi(temp);
        if (nuevosMin > 0) {
            rutinas[idx].minutos = nuevosMin;
        } else {
            printf("  [!] Valor invalido, se conservan los minutos anteriores.\n");
        }
    }

    
    guardarDatos();
    printf("\n  [OK] Rutina actualizada correctamente.\n");
    pausar();
}

void eliminarRutina(void) {
    int numero;

    printf("\n============================================================\n");
    printf("                 ELIMINAR RUTINA                           \n");
    printf("============================================================\n");

    if (totalRutinas == 0) {
        printf("  No hay rutinas para eliminar.\n");
        pausar();
        return;
    }

    mostrarTodasLasRutinas();

    printf("  Numero de rutina a eliminar (1 - %d): ", totalRutinas);
    if (scanf("%d", &numero) != 1) {
        limpiarBuffer();
        printf("  [!] Entrada invalida.\n");
        pausar();
        return;
    }
    limpiarBuffer();

    if (numero < 1 || numero > totalRutinas) {
        printf("  [!] Numero fuera de rango.\n");
        pausar();
        return;
    }

    int idx = numero - 1;

    
    char confirmacion;
    printf("  Confirma eliminar '%s - %s'? (s/n): ",
           rutinas[idx].dia, rutinas[idx].ejercicio);
    confirmacion = getchar();
    limpiarBuffer();

    if (confirmacion != 's' && confirmacion != 'S') {
        printf("  Eliminacion cancelada.\n");
        pausar();
        return;
    }

    
    for (int i = idx; i < totalRutinas - 1; i++) {
        rutinas[i] = rutinas[i + 1];
    }
    totalRutinas--;

    guardarDatos();
    printf("\n  [OK] Rutina eliminada correctamente.\n");
    pausar();
}

void mostrarTotalMinutos(void) {
    int totalMin = 0;

    printf("\n============================================================\n");
    printf("              TOTAL DE MINUTOS ENTRENADOS                  \n");
    printf("============================================================\n");

    if (totalRutinas == 0) {
        printf("  No hay rutinas registradas.\n");
        pausar();
        return;
    }

    
    for (int i = 0; i < totalRutinas; i++) {
        totalMin += rutinas[i].minutos;
    }

    
    int horas   = totalMin / 60;
    int minRest = totalMin % 60;

    printf("  Rutinas registradas: %d\n", totalRutinas);
    printf("  Total de minutos:    %d minutos\n", totalMin);
    printf("  Equivale a:          %d hora(s) y %d minuto(s)\n", horas, minRest);

    
    printf("\n  ");
    if (totalMin < 60) {
        printf("Buen comienzo! Sigue adelante.\n");
    } else if (totalMin < 300) {
        printf("Muy bien! Vas por buen camino.\n");
    } else {
        printf("Excelente constancia! Eres una maquina.\n");
    }

    pausar();
}

void mostrarCreditos(void) {
    printf("\n============================================================\n");
    printf("                      CREDITOS                             \n");
    printf("============================================================\n");
    printf("\n");
    printf("  PROYECTO: Registro de Rutinas de Entrenamiento\n");
    printf("  ASIGNATURA: Lenguaje II\n");
    printf("  CARRERA: Ingenieria en Sistemas\n");
    printf("  UNIVERSIDAD: UNEFA - Nucleo Maracay\n");
    printf("\n");
    printf("  Hecho con amor, mal internet y sin luz.\n");
    printf("\n");
    printf("  Este programa fue desarrollado a base de:\n");
    printf("    - Cafe a las 2am\n");
    printf("    - Stack Overflow (con fe)\n");
    printf("    - Un compilador que a veces funciona\n");
    printf("    - Mucha voluntad y poca RAM\n");
    printf("\n");
    printf("  Si este programa compila, fue un milagro.\n");
    printf("  Si ademas funciona... dos milagros.\n");
    printf("\n");
    printf("  Lenguaje C: porque los punteros no son suficientemente\n");
    printf("  confusos por si solos.\n");
    printf("\n");
    printf("============================================================\n");
    pausar();
}

void cargarDatos(void) {
    FILE *archivo = fopen(ARCHIVO_DATOS, "rb");

    if (archivo == NULL) {
        
        totalRutinas = 0;
        return;
    }

    
    fread(&totalRutinas, sizeof(int), 1, archivo);

    
    if (totalRutinas > MAX_RUTINAS) {
        totalRutinas = MAX_RUTINAS;
    }

    fread(rutinas, sizeof(Rutina), totalRutinas, archivo);

    fclose(archivo);
}

void guardarDatos(void) {
    FILE *archivo = fopen(ARCHIVO_DATOS, "wb");

    if (archivo == NULL) {
        printf("  [ERROR] No se pudo abrir el archivo para guardar.\n");
        return;
    }

    
    fwrite(&totalRutinas, sizeof(int), 1, archivo);
    fwrite(rutinas, sizeof(Rutina), totalRutinas, archivo);

    fclose(archivo);
}

int diaValido(const char *dia) {
    
    const char *diasPermitidos[] = {
        "Lunes", "Martes", "Miercoles", "Jueves", "Viernes",
        "lunes", "martes", "miercoles", "jueves", "viernes",
        "LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES"
    };
    int cantDias = 15;

    for (int i = 0; i < cantDias; i++) {
        if (strcmp(dia, diasPermitidos[i]) == 0) {
            return 1; 
        }
    }
    return 0; 
}

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar(void) {
    printf("\n  Presione Enter para continuar...");
    getchar();
}