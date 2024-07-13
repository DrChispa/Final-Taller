// Generala por: Emiliano Sandoval, Joaquin Rotondo ___ AUS2024 ___ Comision 2 ___ TP Final Taller de programacion I

#include <stdio.h> 
#include <stdlib.h> // system clear terminal
#include <time.h> 
#include <unistd.h> // sleep 

#ifdef _WIN32 // Si es windows CLEAR es cls, si es linux clear
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define DADOS 5 // Cantidad de dados
#define CARAS 6 // Catidad de caras en cada dado
#define RELANZADAS 3 // Numero máximo de relanzadas
#define CATEGORIAS 11 // Cantidad de categorias

#define TURNOS 11 // Cantidad máxima de turnos

#define VALOR_ESCALERA 35
#define VALOR_FULL 50
#define VALOR_POKER 75
#define VALOR_GENERALA 100
#define VALOR_DOBLE_GENERALA 120

#define DIFICULTADES 6 // Cantidad de dificultades
// Puntaje a alcanzar en cada dificultad (Arbitrario)
#define PUNTAJE_FACIL 100
#define PUNTAJE_MEDIA 200
#define PUNTAJE_DIFICIL 260
#define PUNTAJE_MUY_DIFICIL 310
#define PUNTAJE_EXPERTO 420
#define PUNTAJE_MAXIMO 450

// Las strings de las categorías
const char *CATEGORIAS_STRINGS[CATEGORIAS] = {
    "1", "2", "3", "4", "5", "6", 
    "Escalera", "Full", "Poker", "Generala", "Doble generala"
};

// Recibe un array, su tamaño y el valor que se le quiere poner a cada elemento
// Ej: -> (dados, DADOS, 0) -> {0,0,0,0,0}
// Ej: -> (puntos, CATEGORIAS, -1) -> {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}
void normalizarArray(int arr[], int size, int valor){
    int i;
    for(i=0;i<size;i++){
        arr[i]=valor;
    }
}

void limpiarConsola() {
    system(CLEAR);
}

// Hacer una tirada de los dados, solo reemplazar por un numero random del 1 al 6 los dados que son 0
void hacerTirada(int dados[]){
    int i;
    for(i = 0; i < DADOS ; i++){
        int numeroRandom = rand() % CARAS+1;
        if(!dados[i]) dados[i] = numeroRandom;
    }
}

// Imprime los dados
void mostrarDados(int dados[]){
    int i;
    printf("\n");

    for(i=0;i<DADOS;i++){
       printf("Dado %d: %d\n", i+1 ,dados[i]);
    }
}

int hacerCambio(int dados[]){
    int i, dadoACambiar, cantDadosACambiar;

    // Asegurarse de que el jugador elija un número válido de dados para cambiar
    do{
    printf("\nCuantos dados queres cambiar?(0-%d): ", DADOS);
    scanf("%d", &cantDadosACambiar);
    }while(cantDadosACambiar < 0 || cantDadosACambiar > DADOS);

    // Si el jugador quiere cambiar todos los dados
    if(cantDadosACambiar == DADOS){
        normalizarArray(dados, DADOS, 0);
        limpiarConsola();
    }
    else if(cantDadosACambiar!=0){
        for( i = 0 ; i < cantDadosACambiar ; i++ ){
            do{
                printf("\n[%d] Que dado deseas relanzar?: ",i+1);
                scanf("%d",&dadoACambiar); 
            }while(dadoACambiar<1 || dadoACambiar>DADOS || !dados[dadoACambiar-1]); // Si el jugador elije un numero que no esté entre [1, 5] o si elije un numero elegido anteriormente

            // Cambiar el dado seleccionado a 0 para que se vuelva a tirar
            dados[dadoACambiar - 1] = 0;
        }  
        limpiarConsola();
    }
    // Hacer una nueva tirada con los dados elegidos
    hacerTirada(dados);

    return cantDadosACambiar;
}

void detectarOpciones(int dados[], int jugador[], int opciones[]) {
    int i, contador[CARAS];

    // Inicializar las variables para detectar Full
    int  dos = 0, tres = 0;

    // Inicializar contador con todos 0;
    normalizarArray(contador, CARAS, 0);

    // Inicializar todas las opciones a 0
    normalizarArray(opciones, CATEGORIAS, 0);

    // Contar las ocurrencias de cada valor de dado
    // Ej: si tirada = {3,3,4,4,4}, contador = {0,0,2,3,0,0}
    // Ej: si tirada = {1,2,3,4,5}, contador = {1,1,1,1,1,0}
    for (i = 0; i < DADOS; i++) {
        contador[dados[i]-1]++;
    }

    for (i = 0; i < CARAS; i++) {
        // Detectar categorías numéricas
        if (jugador[i] == -1) {
            opciones[i] = (i+1) * contador[i];  // Sumamos los valores de los dados
        }
         // Detectar Escalera (1-2-3-4-5 o 2-3-4-5-6)
        if (jugador[6] == -1 && ((contador[0] == 1 && contador[1] == 1 && contador[2] == 1 && contador[3] == 1 && contador[4] == 1) ||
                                (contador[1] == 1 && contador[2] == 1 && contador[3] == 1 && contador[4] == 1 && contador[5] == 1))) {
            opciones[6] = VALOR_ESCALERA; // Puntaje fijo para escalera
        }
    
        // Detectar Full (tres de un número y dos de otro)
        if (contador[i] == 2) dos = 1;
        if (contador[i] == 3) tres = 1;
        if (jugador[7] == -1 && tres && dos) {
            opciones[7] = VALOR_FULL; // Puntaje fijo para Full
        }
    
        // Detectar Poker (cuatro del mismo número)
        if (jugador[8] == -1 && contador[i] == 4) {
            opciones[8] = VALOR_POKER; // Puntaje fijo para Poker
        }
    
        // Detectar Generala (cinco del mismo número)
        if (jugador[9] == -1 && contador[i] == 5) {
            opciones[9] = VALOR_GENERALA; // Puntaje fijo para Generala
        }
        
        // Detectar Doble Generala (una segunda Generala)
        if (jugador[10] == -1 && jugador[9] == VALOR_GENERALA && contador[i] == 5) {
                opciones[10] = VALOR_DOBLE_GENERALA; // Puntaje fijo para Doble Generala
        }
    }
}

void sumarPuntos(int opciones[], int jugador[]){
    int decision;

    do{
        printf("\nElegi una opcion: ");
        scanf("%d", &decision);
    }while(decision <= 0 || decision > CATEGORIAS || jugador[decision-1] != -1); // Si ingresa un int fuera del rango [1 , 11] o si elige una categoría que ya tiene tachada o anotada

    // Anotarse en la lista de puntos el valor de opciones en el indice elegido, puede ser 0 o un numero mayor
    jugador[decision-1] = opciones[decision-1];
}

void darOpcionesYSumar(int dados[], int jugador[]) {
    int opciones[CATEGORIAS];

    // Detectar opciones dependiendo de los dados
    detectarOpciones(dados, jugador, opciones);

    printf("\nOpciones:\n");
    for (int i = 0; i < CATEGORIAS; i++) {
        if (opciones[i] != 0 && jugador[i] == -1) { // Si se pueden sumar puntos en cierta categoría y el jugador no cuenta con puntos en esa categoria 
            printf("%d) Anotarse: %d a %s\n", i + 1, opciones[i], CATEGORIAS_STRINGS[i]);
        } else if(jugador[i] == -1){ // Si el jugador no cuenta con puntos en esa categoria (pero tampoco puede anotarse puntos)
            printf("%d) Tacharse: %s\n", i + 1, CATEGORIAS_STRINGS[i]);
        }
    }

    sumarPuntos(opciones, jugador);
}

// Recibe un array de enteros y devuelve suma sus elementos mayores o iguales a 0
int puntuacionesTotales(int puntuaciones[]) {
    int total = 0;

    for (int i = 0; i < CATEGORIAS; i++) {
        if (puntuaciones[i] >= 0) total += puntuaciones[i]; // Asegurarse que solo sume si el valor no es -1
    }

    return total;
}

void imprimirPuntuaciones(int jugador1[], int jugador2[]) {
    int i;

    printf("\nCATEGORIAS      |  J1   |  J2\n");
    printf("-----------------------------\n");

    for ( i = 0; i < CATEGORIAS; i++) {
        printf("%-15s |  ", CATEGORIAS_STRINGS[i]);

        // Imprimir puntuaciones para el jugador 1
        if (jugador1[i] == -1) {
            printf("     |  ");  // Espacio vacío para -1
        } else if (jugador1[i] == 0) {
            printf("X    |  ");  // X para 0 puntos
        } else {
            printf("%-5d|  ", jugador1[i]);  // Puntuación
        }

        // Imprimir puntuaciones para el jugador 2
        if (jugador2[i] == -1) {
            printf("     \n");  // Espacio vacío para -1
        } else if (jugador2[i] == 0) {
            printf("X    \n");  // X para 0 puntos
        } else {
            printf("%-5d\n", jugador2[i]);  // Puntuación
        }
    }

    printf("-----------------------------\n");
    // Imprimir puntuaciones totales
    printf("Total           |  %-3d  |  %-5d\n", puntuacionesTotales(jugador1), puntuacionesTotales(jugador2));
}

void turno(int jugador[], int turnoActual, int jugadorActual, int jugador1[], int jugador2[], double tiempoAcumulado){
    int i = 0, dadosACambiar = 1;

    // Inicializar dados
    int dados[DADOS];
    normalizarArray(dados, DADOS, 0);

    // Primer tirada
    hacerTirada(dados);
    limpiarConsola();

    while(i < RELANZADAS && dadosACambiar ){ // Si todavia no se relanzó todas las veces posibles y ademas hay dados para cambiar
        printf("Jugador: %d    |   Turno: %d   |   Tirada: %d   |   Tiempo Acumulado: %.0f segundos\n", jugadorActual, turnoActual, i+1, tiempoAcumulado); // Imprimir info del juego
        mostrarDados(dados); 
        dadosACambiar = hacerCambio(dados); // Invocar la funcion para cambiar los dados y actualizar la variable de dados a cambiar para salir del loop si es necesario
        // Si el jugador elije cambiar 0 dados, terminar con el loop
        i++;
    }

    limpiarConsola();
    imprimirPuntuaciones(jugador1, jugador2);

    printf("\n\nJugador: %d   |   Turno: %d\n", jugadorActual, turnoActual); // Imprimir info del juego
    printf("Tus dados son\n"); 
    mostrarDados(dados); // Mostrar los 5 dados elegidos
    darOpcionesYSumar(dados, jugador); // Imprimir las opciones posibles y sumarse o tacharse puntos
}


void evaluarResultadoIndividual(int jugador, int dificultad) {
    limpiarConsola();
    if (jugador >= dificultad) {
        printf("\n C: :) Felicitaciones!!! c: :D\n\n Ganaste con %d puntos\n", jugador);
    } else {
        printf("\n D: :c Perdiste :( T_T :C.\n\n Tu puntaje es %d y necesitabas %d para ganar\n", jugador, dificultad);
    } 
}

void evaluarResultadoMultijugador(int puntosJugador1, int puntosJugador2) {
    limpiarConsola();
    if (puntosJugador1 > puntosJugador2) {
        printf(" C: :) Felicitaciones Jugador 1 !!! c: :D\n\n Ganaste con %d puntos\n\n\n", puntosJugador1);
    } else if (puntosJugador1 < puntosJugador2) {
        printf("\n C: :) Felicitaciones Jugador 2 !!! c: :D\n\n Ganaste con %d puntos\n\n", puntosJugador2);
    } else {
        printf("\n :| Empataron con %d puntos! :S \n\n", puntosJugador1);
    } 
}

void evaluarDesempenio(int puntosJugador, int numeroJugador){
    int veredicto = 0;

    if (puntosJugador < PUNTAJE_FACIL) veredicto = 1 ;
    else if (puntosJugador < PUNTAJE_MEDIA && puntosJugador >= PUNTAJE_FACIL) veredicto = 2 ;
    else if (puntosJugador < PUNTAJE_DIFICIL && puntosJugador >= PUNTAJE_MEDIA) veredicto = 3 ;
    else if (puntosJugador < PUNTAJE_MUY_DIFICIL && puntosJugador >= PUNTAJE_DIFICIL) veredicto = 4 ;
    else if (puntosJugador < PUNTAJE_EXPERTO && puntosJugador >= PUNTAJE_MUY_DIFICIL) veredicto = 5 ;
    else if (puntosJugador >= PUNTAJE_EXPERTO) veredicto = 6 ;

    switch (veredicto)
    {
    case 1:
        printf("Con %d puntos, el jugador %d tuvo una partida pobre\n", puntosJugador, numeroJugador);
        break;
    case 2:
        printf("Con %d puntos, el jugador %d tuvo una partida normal\n", puntosJugador, numeroJugador);
        break;
    case 3:
        printf("Con %d puntos, el jugador %d tuvo una partida buena\n", puntosJugador, numeroJugador);
        break;
    case 4:
        printf("Con %d puntos, el jugador %d tuvo una partida excelente\n", puntosJugador, numeroJugador);
        break;
    case 5:
        printf("Con %d puntos, el jugador %d tuvo una partida sobresaliente\n", puntosJugador, numeroJugador);
        break;
    case 6:
        printf("Con %d puntos, el jugador %d tuvo una partida increible\n", puntosJugador, numeroJugador);
        break;
    default:
        break;
    }
}

int elegirDificultad() {
    int dificultad;
    int puntajes[DIFICULTADES] = {PUNTAJE_FACIL, PUNTAJE_MEDIA, PUNTAJE_DIFICIL, PUNTAJE_MUY_DIFICIL, PUNTAJE_EXPERTO, PUNTAJE_MAXIMO};

    printf("Elegi la dificultad [Puntuacion a vencer]\n\n1) Facil [%d]\n2) Media [%d]\n3) Dificil [%d]\n4) Muy dificil [%d]\n5) Experto [%d]\n6) Imposible [%d]\n",PUNTAJE_FACIL, PUNTAJE_MEDIA, PUNTAJE_DIFICIL, PUNTAJE_MUY_DIFICIL, PUNTAJE_EXPERTO, PUNTAJE_MAXIMO);

    do {
        printf("\nIntroduci un numero entre 1 y 6: ");
        scanf("%d", &dificultad);
    } while (dificultad < 1 || dificultad > 6);

    return puntajes[dificultad - 1];
}

void unJugador(){
    limpiarConsola();
    // Inicializar los puntos del jugador 1
    int jugador1[CATEGORIAS];
    normalizarArray(jugador1, CATEGORIAS, -1);

    // Inicializar los puntos del jugador 2 (en este caso a 0 para que cuente como que no existe pero y no se queje la funcion de mostrar puntuaciones);
    int jugador2[CATEGORIAS];
    normalizarArray(jugador2, CATEGORIAS, 0);
    
    // Iniciar partida
    int i, turnoActual;

    // Elegir dificultad
    int dificultad=elegirDificultad();

    // Inicializar reloj
    clock_t inicioJugador, finJugador;
    double tiempoJugador = 0;

    for (i = 0; i < TURNOS ; i++){
        turnoActual= i + 1; // Calcular turno
        // Medir tiempo del jugador
        inicioJugador = clock();
        turno(jugador1, turnoActual, 1, jugador1, jugador2, tiempoJugador); // En este caso, el tercer arg de la funcion turno siempre va a ser 1
        finJugador = clock();
        tiempoJugador += (double)(finJugador - inicioJugador) / CLOCKS_PER_SEC; // Calcular tiempo del turno
    }

    evaluarResultadoIndividual(puntuacionesTotales(jugador1), dificultad);
    printf("\n Tiempo total de la partida: %.0f segundos\n", tiempoJugador);
    imprimirPuntuaciones(jugador1, jugador2);
}

void dosJugadores() {
    // Inicializar los puntos de los jugadores
    int jugador1[CATEGORIAS];
    normalizarArray(jugador1,CATEGORIAS,-1);
    int jugador2[CATEGORIAS];
    normalizarArray(jugador2,CATEGORIAS,-1);

    // Inicializar tiempos de los jugadores
    double tiempoJugador1 = 0, tiempoJugador2 = 0;

    // Inicializar reloj
    clock_t inicioJugador1, finJugador1;
    clock_t inicioJugador2, finJugador2;

    for (int i = 0; i < TURNOS; i++) {
        int turnoActual = i + 1;

        // Medir tiempo del jugador 1
        inicioJugador1 = clock(); // Tomar tick del reloj
        turno(jugador1, turnoActual, 1, jugador1, jugador2, tiempoJugador1);
        finJugador1 = clock(); // Tomar tick del reloj
        tiempoJugador1 += (double)(finJugador1 - inicioJugador1) / CLOCKS_PER_SEC; // Calcular el tiempo del turno

        // Medir tiempo del jugador 2
        inicioJugador2 = clock(); // Tomar tick del reloj
        turno(jugador2, turnoActual, 2, jugador1, jugador2, tiempoJugador2);
        finJugador2 = clock(); // Tomar tick del reloj
        tiempoJugador2 += (double)(finJugador2 - inicioJugador2) / CLOCKS_PER_SEC; // Calcular el tiempo del turno
    }

    // Calcular las puntuaciones totales
    int totalJugador1 = puntuacionesTotales(jugador1); 
    int totalJugador2 = puntuacionesTotales(jugador2);

    evaluarResultadoMultijugador(totalJugador1, totalJugador2); // Mostrar quien gano

    double totalPartida = tiempoJugador1 + tiempoJugador2; // Calcular el tiempo total de la partida
    
    // Mostrar datos del tiempo
    printf(" Tiempo total de la partida: %.0f segundos\n", totalPartida);
    printf(" Tiempo de juego del jugador 1: %.0f segundos\n", tiempoJugador1);
    printf(" Tiempo de juego del jugador 2: %.0f segundos\n\n", tiempoJugador2);

    evaluarDesempenio(totalJugador1, 1); // Mostrar desempeño de los jugadores
    evaluarDesempenio(totalJugador2, 2); // Mostrar desempeño de los jugadores

    imprimirPuntuaciones(jugador1, jugador2); // Mostrar la tabla de puntuaciones final
}

void dibujarDado() {
    printf("\n");
    printf("       ________\n");
    printf("      / *  *  /|\n");
    printf("     / *  *  / |\n");
    printf("    |-------|* |\n");
    printf("    |       |  |\n");
    printf("    |   *   |* /\n");
    printf("    |       | /\n");
    printf("    |_______|/\n");
}

void iniciarJuego(){
    limpiarConsola();

    // Elegir modo de juego
    int modoDeJuego;

    printf("\n Bienvenidos/as al juego de la Generala!!");
    dibujarDado();
    printf("\n Elegir el modo de juego\n \n 1) Individual \n 2) Dos Jugadores\n");

    do {
        printf("\n > ");
        scanf("%d",&modoDeJuego);
    }while(modoDeJuego<1 || modoDeJuego>2);

    if (modoDeJuego==1){
        unJugador();
    } else{
        dosJugadores();
    }

    //Elegir si jugar o no de nuevo
    int jugarDeNuevo;

    printf("\nJugar de nuevo?\n\n1) Si\n2) No\n");    
    do {
        printf("\n > ");
        scanf("%d",&jugarDeNuevo);
    }while(jugarDeNuevo<1 || jugarDeNuevo>2);

    limpiarConsola();
    if (jugarDeNuevo==1){
        iniciarJuego(); // Llamada recursiva para jugar de nuevo
    } else{
        printf("\n Gracias por jugar! Ponganos 10\n\n Generala desarrollada por\n\n > Emiliano Sandoval\n > Joaquin Rotondo\n\n");
    }
}

int main(){
    srand(time(NULL)); // Seed para la funcion random

    iniciarJuego();

    return 0;
}