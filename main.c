#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define rutinas_limite     50          
#define ejercicio_limite   50          
#define archivo_datos   "rutinas.dat" 

typedef struct {
    char dia[15];               
    char ejercicio[ejercicio_limite]; 
    int  minutos;               
} rutina;

rutina lista_rutinas[rutinas_limite];    
int total_rutinas = 0;           

void mostrar_menu(void);
void registrar_rutina(void);
void mostrar_todas_las_rutinas(void);
void buscar_rutina_por_dia(void);
void editar_rutina(void);
void eliminar_rutina(void);
void mostrar_total_minutos(void);
void mostrar_creditos(void);
void cargar_datos(void);
void guardar_datos(void);
void limpiar_buffer(void);
void pausar(void);
int  dia_valido(const char *dia);

int main(void) {
    int opcion;

    cargar_datos();

    do {
        mostrar_menu();

        printf("  Ingrese su opcion: ");
        if (scanf("%d", &opcion) != 1) {
            opcion = 0; 
        }
        limpiar_buffer();

        switch (opcion) {
            case 1: registrar_rutina();        break;
            case 2: mostrar_todas_las_rutinas(); break;
            case 3: buscar_rutina_por_dia();     break;
            case 4: editar_rutina();           break;
            case 5: eliminar_rutina();         break;
            case 6: mostrar_total_minutos();    break;
            case 7: mostrar_creditos();        break;
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

void mostrar_menu(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("============================================================\n");
    printf("        REGISTRO DE RUTINAS DE ENTRENAMIENTO                \n");
    printf("============================================================\n");
    printf("  Rutinas registradas: %d / %d\n", total_rutinas, rutinas_limite);
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

void registrar_rutina(void) {
    printf("\n============================================================\n");
    printf("                  REGISTRAR RUTINA                         \n");
    printf("============================================================\n");

    if (total_rutinas >= rutinas_limite) {
        printf("\n  [!] Limite maximo alcanzado (%d rutinas).\n", rutinas_limite);
        pausar();
        return;
    }

    rutina nueva; 

    do {
        printf("  Dia de la semana (Lunes/Martes/Miercoles/Jueves/Viernes): ");
        fgets(nueva.dia, sizeof(nueva.dia), stdin);
        nueva.dia[strcspn(nueva.dia, "\n")] = '\0'; 

        if (!dia_valido(nueva.dia)) {
            printf("  [!] Dia invalido. Solo se permiten dias de Lunes a Viernes.\n");
        }
    } while (!dia_valido(nueva.dia));

    printf("  Nombre del ejercicio: ");
    fgets(nueva.ejercicio, sizeof(nueva.ejercicio), stdin);
    nueva.ejercicio[strcspn(nueva.ejercicio, "\n")] = '\0';

    do {
        printf("  Duracion en minutos: ");
        if (scanf("%d", &nueva.minutos) != 1 || nueva.minutos <= 0) {
            printf("  [!] Ingrese un numero positivo de minutos.\n");
            limpiar_buffer();
            nueva.minutos = 0;
        } else {
            limpiar_buffer();
        }
    } while (nueva.minutos <= 0);

    lista_rutinas[total_rutinas] = nueva;
    total_rutinas++;

    guardar_datos();

    printf("\n  [OK] Rutina registrada exitosamente.\n");
    pausar();
}

void mostrar_todas_las_rutinas(void) {
    printf("\n============================================================\n");
    printf("               TODAS LAS RUTINAS                           \n");
    printf("============================================================\n");

    if (total_rutinas == 0) {
        printf("  No hay rutinas registradas todavia.\n");
        pausar();
        return;
    }

    printf("  %-4s %-12s %-30s %-8s\n", "No.", "Dia", "Ejercicio", "Minutos");
    printf("  -----------------------------------------------------------\n");

    for (int i = 0; i < total_rutinas; i++) {
        printf("  %-4d %-12s %-30s %-8d\n",
               i + 1,
               lista_rutinas[i].dia,
               lista_rutinas[i].ejercicio,
               lista_rutinas[i].minutos);
    }

    printf("  -----------------------------------------------------------\n");
    printf("  Total de rutinas: %d\n", total_rutinas);
    pausar();
}

void buscar_rutina_por_dia(void) {
    char dia_buscado[15];
    int encontradas = 0;

    printf("\n============================================================\n");
    printf("               BUSCAR RUTINA POR DIA                       \n");
    printf("============================================================\n");

    printf("  Dia a buscar (Lunes/Martes/Miercoles/Jueves/Viernes): ");
    fgets(dia_buscado, sizeof(dia_buscado), stdin);
    dia_buscado[strcspn(dia_buscado, "\n")] = '\0';

    printf("\n  Rutinas para el dia: %s\n", dia_buscado);
    printf("  -----------------------------------------------------------\n");

    for (int i = 0; i < total_rutinas; i++) {
        #ifdef _WIN32
            int igual = (_stricmp(lista_rutinas[i].dia, dia_buscado) == 0);
        #else
            int igual = (strcasecmp(lista_rutinas[i].dia, dia_buscado) == 0);
        #endif

        if (igual) {
            printf("  [%d] Ejercicio: %-30s | %d minutos\n",
                   i + 1, lista_rutinas[i].ejercicio, lista_rutinas[i].minutos);
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

void editar_rutina(void) {
    int numero;

    printf("\n============================================================\n");
    printf("                  EDITAR RUTINA                            \n");
    printf("============================================================\n");

    if (total_rutinas == 0) {
        printf("  No hay rutinas para editar.\n");
        pausar();
        return;
    }

    mostrar_todas_las_rutinas();

    printf("  Numero de rutina a editar (1 - %d): ", total_rutinas);
    if (scanf("%d", &numero) != 1) {
        limpiar_buffer();
        printf("  [!] Entrada invalida.\n");
        pausar();
        return;
    }
    limpiar_buffer();

    if (numero < 1 || numero > total_rutinas) {
        printf("  [!] Numero fuera de rango.\n");
        pausar();
        return;
    }

    int idx = numero - 1; 

    printf("\n  Editando: %s - %s (%d min)\n",
           lista_rutinas[idx].dia, lista_rutinas[idx].ejercicio, lista_rutinas[idx].minutos);
    printf("  (Presione Enter para conservar el valor actual)\n\n");

    char temporal[50];
    printf("  Nuevo dia [%s]: ", lista_rutinas[idx].dia);
    fgets(temporal, sizeof(temporal), stdin);
    temporal[strcspn(temporal, "\n")] = '\0';
    if (strlen(temporal) > 0 && dia_valido(temporal)) {
        strcpy(lista_rutinas[idx].dia, temporal);
    } else if (strlen(temporal) > 0) {
        printf("  [!] Dia invalido, se conserva el anterior.\n");
    }

    printf("  Nuevo ejercicio [%s]: ", lista_rutinas[idx].ejercicio);
    fgets(temporal, sizeof(temporal), stdin);
    temporal[strcspn(temporal, "\n")] = '\0';
    if (strlen(temporal) > 0) {
        strcpy(lista_rutinas[idx].ejercicio, temporal);
    }

    printf("  Nuevos minutos [%d]: ", lista_rutinas[idx].minutos);
    fgets(temporal, sizeof(temporal), stdin);
    temporal[strcspn(temporal, "\n")] = '\0';
    if (strlen(temporal) > 0) {
        int nuevos_minutos = atoi(temporal);
        if (nuevos_minutos > 0) {
            lista_rutinas[idx].minutos = nuevos_minutos;
        } else {
            printf("  [!] Valor invalido, se conservan los minutos anteriores.\n");
        }
    }

    guardar_datos();
    printf("\n  [OK] Rutina actualizada correctamente.\n");
    pausar();
}

void eliminar_rutina(void) {
    int numero;

    printf("\n============================================================\n");
    printf("                 ELIMINAR RUTINA                           \n");
    printf("============================================================\n");

    if (total_rutinas == 0) {
        printf("  No hay rutinas para eliminar.\n");
        pausar();
        return;
    }

    mostrar_todas_las_rutinas();

    printf("  Numero de rutina a eliminar (1 - %d): ", total_rutinas);
    if (scanf("%d", &numero) != 1) {
        limpiar_buffer();
        printf("  [!] Entrada invalida.\n");
        pausar();
        return;
    }
    limpiar_buffer();

    if (numero < 1 || numero > total_rutinas) {
        printf("  [!] Numero fuera de rango.\n");
        pausar();
        return;
    }

    int idx = numero - 1;

    char confirmacion;
    printf("  Confirma eliminar '%s - %s'? (s/n): ",
           lista_rutinas[idx].dia, lista_rutinas[idx].ejercicio);
    confirmacion = getchar();
    limpiar_buffer();

    if (confirmacion != 's' && confirmacion != 'S') {
        printf("  Eliminacion cancelada.\n");
        pausar();
        return;
    }

    for (int i = idx; i < total_rutinas - 1; i++) {
        lista_rutinas[i] = lista_rutinas[i + 1];
    }
    total_rutinas--;

    guardar_datos();
    printf("\n  [OK] Rutina eliminada correctamente.\n");
    pausar();
}

void mostrar_total_minutos(void) {
    int total_minutos = 0;

    printf("\n============================================================\n");
    printf("              TOTAL DE MINUTOS ENTRENADOS                  \n");
    printf("============================================================\n");

    if (total_rutinas == 0) {
        printf("  No hay rutinas registradas.\n");
        pausar();
        return;
    }

    for (int i = 0; i < total_rutinas; i++) {
        total_minutos += lista_rutinas[i].minutos;
    }

    int horas   = total_minutos / 60;
    int minutos_restantes = total_minutos % 60;

    printf("  Rutinas registradas: %d\n", total_rutinas);
    printf("  Total de minutos:    %d minutos\n", total_minutos);
    printf("  Equivale a:          %d hora(s) y %d minuto(s)\n", horas, minutos_restantes);

    printf("\n  ");
    if (total_minutos < 60) {
        printf("Buen comienzo! Sigue adelante.\n");
    } else if (total_minutos < 300) {
        printf("Muy bien! Vas por buen camino.\n");
    } else {
        printf("Excelente constancia! Eres una maquina.\n");
    }

    pausar();
}

void mostrar_creditos(void) {
    printf("\n============================================================\n");
    printf("                      CREDITOS                             \n");
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

void cargar_datos(void) {
    FILE *archivo = fopen(archivo_datos, "rb");

    if (archivo == NULL) {
        total_rutinas = 0;
        return;
    }

    fread(&total_rutinas, sizeof(int), 1, archivo);

    if (total_rutinas > rutinas_limite) {
        total_rutinas = rutinas_limite;
    }

    fread(lista_rutinas, sizeof(rutina), total_rutinas, archivo);

    fclose(archivo);
}

void guardar_datos(void) {
    FILE *archivo = fopen(archivo_datos, "wb");

    if (archivo == NULL) {
        printf("  [ERROR] No se pudo abrir el archivo para guardar.\n");
        return;
    }

    fwrite(&total_rutinas, sizeof(int), 1, archivo);
    fwrite(lista_rutinas, sizeof(rutina), total_rutinas, archivo);

    fclose(archivo);
}

int dia_valido(const char *dia) {
    const char *dias_permitidos[] = {
        "Lunes", "Martes", "Miercoles", "Jueves", "Viernes",
        "lunes", "martes", "miercoles", "jueves", "viernes",
        "LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES"
    };
    int cantidad_dias = 15;

    for (int i = 0; i < cantidad_dias; i++) {
        if (strcmp(dia, dias_permitidos[i]) == 0) {
            return 1; 
        }
    }
    return 0; 
}

void limpiar_buffer(void) {
    int caracter;
    while ((caracter = getchar()) != '\n' && caracter != EOF);
}

void pausar(void) {
    printf("\n  Presione Enter para continuar...");
    getchar();
}