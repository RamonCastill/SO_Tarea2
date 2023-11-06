#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <stdbool.h>
#include <string.h>


// Estructura para guardar datos de procesos 
typedef struct{
    double t_llegada;
    double duracion;
    double prioridad;
    int largo_cola;
    double R;
    double t_espera;
    double t_respuesta;
    double t_restante;
} Proceso;


//Declaracion de funciones
void PPP(Proceso procesos[1000],  double *t_procesador, double *t_durmiendo);
void SRJF(Proceso procesos[1000], double *t_procesador, double *t_durmiendo);
void HRRF(Proceso procesos[1000], double *t_procesador, double *t_durmiendo);
void print_times(Proceso procesos[1000], double t_procesador, double t_durmiendo);

int main(int argc, char *argv[]){

    // Lectura del archivo 
    FILE *file_in = fopen("procesos2.txt","r");

    if (file_in == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }

    // Creacion arrays de estructuras
    char buffer[1024];
    Proceso procesos[1000];
    int i_read = 0;

    // Almacenamiento de cada linea del archivo procesos.txt en la estructura procesos
    while(fgets(buffer,1024,file_in)){
        double t_llegada, duracion;
        double prioridad;
        if (sscanf(buffer, "%lf %lf %lf", &t_llegada, &duracion, &prioridad) == 3) {
            procesos[i_read].t_llegada = t_llegada;
            procesos[i_read].duracion = duracion;
            procesos[i_read].t_restante = duracion;
            procesos[i_read].prioridad = prioridad;
            procesos[i_read].largo_cola = 0;
            procesos[i_read].R = 0;
            procesos[i_read].t_espera = 0;
            procesos[i_read].t_respuesta = 0;
            i_read++;
        } else {
            printf("Error al leer la línea %d\n", i_read + 1);
        }
    }
    fclose(file_in);

    double t_procesador = 0, t_durmiendo = 0;

    //promp para elegir la funcion a usar
    printf("Ingresa: \n'ppp' --> Planofocacion por prioridad\n" );
    printf("'srjf' --> Shortest Remaining Time First\n" );
    printf("'hrrf' --> Highest Response Ratio First\n" );
    printf("aquí --> " );

    char entrada[256];

    fgets(entrada,256,stdin);
    // Se elimina el caracter de termino de lo ingresado por el usuario
    entrada[strcspn(entrada, "\n")] = '\0';

    // se ejecuta la funcion dependiendo de lo que el usuario ingrese
    if (strcmp(entrada, "ppp") == 0) {
        PPP(procesos, &t_procesador, &t_durmiendo);
    } else if (strcmp(entrada, "srjf") == 0) {
        SRJF(procesos, &t_procesador, &t_durmiendo);
    } else if (strcmp(entrada, "hrrf") == 0) {
        HRRF(procesos, &t_procesador, &t_durmiendo);
    } else {
        printf("Opción no válida: %s\n", entrada);
    }


    // Se imprimen los tiempos
    print_times(procesos, t_procesador, t_durmiendo);


    return 0;     
}

void print_times(Proceso tiempos[1000], double t_procesador, double t_durmiendo){
    // Declaracion de variables
    double t_espera_min = 1000, t_espera_max = 0, t_espera_promedio = 0;
    double duracion_min = 1000, duracion_max = 0, duracion_promedio = 0;
    double t_respuesta_min = 1000, t_respuesta_max = 0, t_respuesta_promedio = 0;
    double R_min = 1000, R_max = 0, R_promedio = 0;
    int l_cola_min = 1000, l_cola_max = 0,l_cola_promedio = 0;

    // Se recorre el array con estructuras de tipo tiempo para encontrar min y max
    for(int i = 0; i<1000; i++){
        t_espera_min = (tiempos[i].t_espera < t_espera_min) ? tiempos[i].t_espera : t_espera_min;
        t_espera_max = (tiempos[i].t_espera > t_espera_max) ? tiempos[i].t_espera : t_espera_max;
        duracion_min = (tiempos[i].duracion < duracion_min) ? tiempos[i].duracion : duracion_min;
        duracion_max = (tiempos[i].duracion > duracion_max) ? tiempos[i].duracion : duracion_max;
        t_respuesta_min = (tiempos[i].t_respuesta < t_respuesta_min) ? tiempos[i].t_respuesta : t_respuesta_min;
        t_respuesta_max = (tiempos[i].t_respuesta > t_respuesta_max) ? tiempos[i].t_respuesta : t_respuesta_max;
        R_min = (tiempos[i].R < R_min) ? tiempos[i].R : R_min;
        R_max = (tiempos[i].R > R_max) ? tiempos[i].R : R_max;
        l_cola_min = (tiempos[i].largo_cola < l_cola_min) ? tiempos[i].largo_cola : l_cola_min;
        l_cola_max = (tiempos[i].largo_cola > l_cola_max) ? tiempos[i].largo_cola : l_cola_max;

        // Se recorre el array con estructuras de tipo tiempo sumar todo
        t_espera_promedio += tiempos[i].t_espera;
        duracion_promedio += tiempos[i].duracion;
        t_respuesta_promedio += tiempos[i].t_respuesta;
        R_promedio += tiempos[i].R;
        l_cola_promedio += tiempos[i].largo_cola;
    }
    // Se hace el promedio
    t_espera_promedio /= 1000;
    duracion_promedio /= 1000;
    t_respuesta_promedio /= 1000;
    R_promedio /= 1000;
    l_cola_promedio /= 1000;

    // Mostrar por pantalla...
    printf("Mínimo t_espera: %lf\n", t_espera_min);
    printf("Máximo t_espera: %lf\n", t_espera_max);
    printf("Promedio t_espera: %lf\n", t_espera_promedio);

    printf("Mínima duración: %lf\n", duracion_min);
    printf("Máxima duración: %lf\n", duracion_max);
    printf("Promedio duración: %lf\n", duracion_promedio);

    printf("Mínima t_respuesta: %lf\n", t_respuesta_min);
    printf("Máxima t_respuesta: %lf\n", t_respuesta_max);
    printf("Promedio t_respuesta: %lf\n", t_respuesta_promedio);

    printf("Mínimo R: %lf\n", R_min);
    printf("Máximo R: %lf\n", R_max);
    printf("Promedio R: %lf\n", R_promedio);

    printf("Mínimo largo_cola: %i\n", l_cola_min);
    printf("Máximo largo_cola: %i\n", l_cola_max);
    printf("Promedio largo_cola: %i\n", l_cola_promedio);

    printf("Tiempo útil = %lfs, Utilización = %lf%\n", t_procesador, 100 - (t_durmiendo/t_procesador)*100);
}

void PPP(Proceso procesos[1000], double *t_procesador, double *t_durmiendo){

    double t_espera = 0, t_respuesta = 0, R = 0;
    int l = 0;
    int lc = 0;
    int i = 0, j = 0;
    while(i < 1000){

        if(*t_procesador < procesos[i].t_llegada){

            *t_durmiendo += procesos[i].t_llegada - *t_procesador;
            *t_procesador  = procesos[i].t_llegada;

        }
        if(i+j < 1000 && *t_procesador + procesos[i].t_restante > procesos[i+j].t_llegada){
            procesos[i].t_restante -= procesos[i+j].t_llegada - *t_procesador;
            t_espera = procesos[i+j].t_llegada - *t_procesador ;
            *t_procesador = procesos[i+j].t_llegada;
        }else{
            *t_procesador += procesos[i].t_restante;
            t_espera = procesos[i].t_restante;
            procesos[i].largo_cola = lc;
            procesos[i].t_restante -= procesos[i].t_restante;
            procesos[i].t_respuesta = *t_procesador - procesos[i].t_llegada;
            procesos[i].R = (procesos[i].t_espera+procesos[i].duracion)/(procesos[i].duracion); 
            i++;
        }

        lc = l;
        l = 0;
        while(i + l < 1000 && *t_procesador >= procesos[i + l].t_llegada){
            procesos[i+l].t_espera += t_espera;
            procesos[i+l].R = (procesos[i+l].t_espera+procesos[i+l].duracion)/(procesos[i+l].duracion); 
            if(procesos[i].prioridad > procesos[i + l].prioridad){
                Proceso a;
                a = procesos[i];
                procesos[i] = procesos[i + l];
                procesos[i + l] = a;

            }
            else if(procesos[i].prioridad == procesos[i + l].prioridad && procesos[i].t_llegada > procesos[i + l].t_llegada){
                Proceso a;
                a = procesos[i];
                procesos[i] = procesos[i + l];
                procesos[i + l] = a;    
            } 
            j++;
            l++;
        }



    }

}

void SRJF(Proceso procesos[1000], double *t_procesador, double *t_durmiendo){

    double t_espera = 0, t_respuesta = 0, R = 0;
    int l = 0;
    int lc = 0;
    int i = 0, j = 0;
    while(i < 1000){

        if(*t_procesador < procesos[i].t_llegada){

            *t_durmiendo += procesos[i].t_llegada - *t_procesador;
            *t_procesador  = procesos[i].t_llegada;

        }
        if(i+j < 1000 && *t_procesador + procesos[i].t_restante > procesos[i+j].t_llegada){
            procesos[i].t_restante -= procesos[i+j].t_llegada - *t_procesador;
            t_espera = procesos[i+j].t_llegada - *t_procesador ;
            *t_procesador = procesos[i+j].t_llegada;
        }else{
            *t_procesador += procesos[i].t_restante;
            t_espera = procesos[i].t_restante;
            procesos[i].largo_cola = lc;
            procesos[i].t_restante -= procesos[i].t_restante;
            procesos[i].t_respuesta = *t_procesador - procesos[i].t_llegada;
            procesos[i].R = (procesos[i].t_espera+procesos[i].duracion)/(procesos[i].duracion); 
            i++;
        }

        lc = l;
        l = 0;
        while(i + l < 1000 && *t_procesador >= procesos[i + l].t_llegada){
            procesos[i+l].t_espera += t_espera;
            procesos[i+l].R = (procesos[i+l].t_espera+procesos[i+l].duracion)/(procesos[i+l].duracion); 
            if(procesos[i].t_restante > procesos[i + l].t_restante){
                Proceso a;
                a = procesos[i];
                procesos[i] = procesos[i + l];
                procesos[i + l] = a;

            }
            else if(procesos[i].t_restante == procesos[i + l].t_restante && procesos[i].t_llegada > procesos[i + l].t_llegada){
                Proceso a;
                a = procesos[i];
                procesos[i] = procesos[i + l];
                procesos[i + l] = a;    
            } 
            j++;
            l++;
        }



    }
    

}

void HRRF(Proceso procesos[1000], double *t_procesador, double *t_durmiendo){

    double t_espera = 0, t_respuesta = 0, R = 0;
    int l = 0;
    int lc = 0;
    int i = 0, j = 0;
    while(i < 1000){

        if(*t_procesador < procesos[i].t_llegada){

            *t_durmiendo += procesos[i].t_llegada - *t_procesador;
            *t_procesador  = procesos[i].t_llegada;

        }
        if(i+j < 1000 && *t_procesador + procesos[i].t_restante > procesos[i+j].t_llegada){
            procesos[i].t_restante -= procesos[i+j].t_llegada - *t_procesador;
            t_espera = procesos[i+j].t_llegada - *t_procesador ;
            *t_procesador = procesos[i+j].t_llegada;
        }else{
            *t_procesador += procesos[i].t_restante;
            t_espera = procesos[i].t_restante;
            procesos[i].largo_cola = lc;
            procesos[i].t_restante -= procesos[i].t_restante;
            procesos[i].t_respuesta = *t_procesador - procesos[i].t_llegada;
            procesos[i].R = (procesos[i].t_espera+procesos[i].duracion)/(procesos[i].duracion); 
            i++;
        }

        lc = l;
        l = 0;
        while(i + l < 1000 && *t_procesador >= procesos[i + l].t_llegada){
            procesos[i+l].t_espera += t_espera;
            procesos[i+l].R = (procesos[i+l].t_espera+procesos[i+l].duracion)/(procesos[i+l].duracion); 
            if(procesos[i].R < procesos[i + l].R){
                Proceso a;
                a = procesos[i];
                procesos[i] = procesos[i + l];
                procesos[i + l] = a;

            }
            else if(procesos[i].R == procesos[i + l].R && procesos[i].t_llegada > procesos[i + l].t_llegada){
                Proceso a;
                a = procesos[i];
                procesos[i] = procesos[i + l];
                procesos[i + l] = a;    
            } 
            j++;
            l++;
        }



    }

}   

