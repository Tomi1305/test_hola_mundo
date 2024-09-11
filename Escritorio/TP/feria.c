#include "feria.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define ARRIBA 'W'
#define DERECHA 'D'
#define ABAJO 'S'
#define IZQUIERDA 'A'
const int MAX_VIDAS = 3;
const int MAX_PUNTOS_ENERGIA = 100;
const char PERRY = 'P';
const char SOMBRERO = 'S';
const char GOLOSINA = 'G';
const char PHINEAS = 'H';
const char FERB = 'F';
const char CANDACE = 'C';
const char BOMBAS = 'B';
const int TOPE_FAMILIARES = 3;
const int MAX_GOLOSINAS = 5;
const int INICIO_GOLOSINAS = 3;
const int FIN_GOLOSINAS = 8;
const int MAX_SOMBREROS = 3;
const int MAX_FIL = 20;
const int MAX_COL = 20;
const int TOPE_BOMBAS = 10;
const int GANAR = 1;
const int PERDER = -1;
const int SIGUE_JUGANDO = 0;
const char VACIO = ' ';
const int INICIO_TABLERO = 0;
const int FIN_TABLERO = 19;
const char ROBOT = 'R';
const int BORRAR = -1;
const int DISTANCIA_FAMILIA = 1;
const int DISTANCIA_ROBOT = 2;
const int ENERGIA_PARA_DESACTIVAR_BOMBA = 10;
const int ENERGIA_PARA_DESTRUIR_ROBOTS = 5;
const int AUMENTO_DE_ENERGIA = 20;

/*
    Recibe dos posiciones(4 NUMEROS ENTEROS), en las cuales se realiza la suma de cada parte por separado encerrado en modulo y devuelve la suma de ambas partes.

*/
int distancia_manhattan(int fila_1, int col_1, int fila_2, int col_2){

    int distacia = abs(fila_1 - fila_2) + abs(col_1 - col_2);
    return distacia;
}
/* 
 *  Recibe un puntero a cordenada_t, una cantidad de robot que debe ser > 0, una fila y una columna que tambien debern ser >= 0
    Se les va sumando + 1 al PTR porque eso seria el indice de cada robot,se verifica si hay un espacio vacio donde no se encuentre algun robot. 
    devuelve false si hay un espacio vacio si no devuelve true.
 */
bool hay_robot(coordenada_t* coordenadas, int cantidad_robot, int fila, int col){
    bool hay_robot = false;
    for (int i = 0; i < cantidad_robot; ++i)
    {
        if (coordenadas->fil == fila && coordenadas->col == col)
        {
            hay_robot = true;
        }
        coordenadas ++;
    }
    return hay_robot;
}
/*
    La funcion distancia_manhattan se debe resperar sus parametros y pasarlos de forma correcta.
    Si Perry está camuflado y está a una distancia de 1 de cualquier familiar con inicial_nombre PHINEAS, CANDACE o FERB, y tiene vida > 0, su vida se reduce en 1 
    por cada familiar de ese tipo a distancia 1.

*/
void distancia_familia(juego_t* juego){
    for (int i = 0; i < juego->tope_familiares; ++i) {
        int distancia = distancia_manhattan(juego->perry.posicion.fil,juego->perry.posicion.col,juego->familiares[i].posicion.fil,juego->familiares[i].posicion.col);

        if (distancia <= DISTANCIA_FAMILIA && 
            (juego->familiares[i].inicial_nombre == PHINEAS || 
             juego->familiares[i].inicial_nombre == CANDACE || 
             juego->familiares[i].inicial_nombre == FERB) && 
            juego->perry.camuflado == false && 
            juego->perry.vida > 0) {
            
            juego->perry.vida--;
        }
    }
}
/*
    Recibe como parametro Juego de tipo juego_t.
    Se mueve PHINEAS por cada movimiento de perry, al llegar a su respectivo tope ya sea la columna inicial o columna final entonces cambia el sentido.

*/
void movimiento_de_phineas(juego_t* juego){
    for (int i = 0; i < juego->tope_familiares; ++i) {
        if (juego->familiares[i].inicial_nombre == PHINEAS) {
            if (juego->familiares[i].posicion.col == FIN_TABLERO) {
                juego->familiares[i].sentido = IZQUIERDA;
            } 
            else if (juego->familiares[i].posicion.col == INICIO_TABLERO) {
                juego->familiares[i].sentido = DERECHA;
            }

            if (juego->familiares[i].sentido == DERECHA) {
                juego->familiares[i].posicion.col += 1;
            } 
            else if (juego->familiares[i].sentido == IZQUIERDA) {
                juego->familiares[i].posicion.col -= 1;
            }
        }    
    }
}
/*
    Recibe como parametro Juego de tipo juego_t.
    Se mueve FERB por cada movimiento de perry, al llegar a su respectivo tope ya sea la columna inicial o columna final entonces cambia el sentido.

*/
void movimiento_de_ferb(juego_t* juego){
    for (int i = 0; i < juego->tope_familiares; ++i) {
        if (juego->familiares[i].inicial_nombre == FERB) {
            if (juego->familiares[i].posicion.col == FIN_TABLERO) {
                juego->familiares[i].sentido = IZQUIERDA;
            } 
            else if (juego->familiares[i].posicion.col == INICIO_TABLERO) {
                juego->familiares[i].sentido = DERECHA;
            }

            if (juego->familiares[i].sentido == DERECHA) {
                juego->familiares[i].posicion.col += 1;
            } 
            else if (juego->familiares[i].sentido == IZQUIERDA) {
                juego->familiares[i].posicion.col -= 1;
            }
        }
    }
}
/*
    Recibe como parametro Juego de tipo juego_t.
    Se mueve CANDACE por cada movimiento de perry, al llegar a su respectivo tope ya sea la fila inicial o fila final entonces cambia el sentido.

*/
void movimiento_de_candace(juego_t* juego){
    for (int i = 0; i < juego->tope_familiares; ++i) {
        if (juego->familiares[i].inicial_nombre == CANDACE) {
            if (juego->familiares[i].posicion.fil == FIN_TABLERO) {
                juego->familiares[i].sentido = ARRIBA;
            } 
            else if (juego->familiares[i].posicion.fil == INICIO_TABLERO) {
                juego->familiares[i].sentido = ABAJO;
            }

            if (juego->familiares[i].sentido == ABAJO) {
                juego->familiares[i].posicion.fil += 1;
            } 
            else if (juego->familiares[i].sentido == ARRIBA) {
                juego->familiares[i].posicion.fil -= 1;
            }
        }
    }

}

/*
    Recibe como parametro Juego de tipo juego_t.
    Desactiva las bombas en el tablero cuando perry se posicione arriba de las bombas estando camuflado,
    se le resta 10 puntos de energia y se elimina la bomba ya desactivada.
*/
void desactivar_bomba(juego_t* juego){
    for (int i = 0; i < juego->tope_bombas ; ++i)
    {
        if (juego->bombas[i].desactivada == false && (juego->bombas[i].posicion.fil == juego->perry.posicion.fil ) 
            && (juego->bombas[i].posicion.col == juego->perry.posicion.col) && (juego->perry.camuflado == false) )
        {
            if (juego->perry.energia >= ENERGIA_PARA_DESACTIVAR_BOMBA)
            {
                juego->perry.energia = juego->perry.energia - ENERGIA_PARA_DESACTIVAR_BOMBA;
                juego->bombas[i].desactivada = true;
                
            }
            juego->bombas[i].posicion.fil = BORRAR;
            juego->bombas[i].posicion.col = BORRAR;

        }
    }
/*


*/
}
/*
*   Se debe recibir el parametro de forma completa.
*   Se Verifica que al iniciar ya exista por lo menos un robot, para que no haya un error.
*   A traves de la distancia sea menor igual a 2. si perry este camuflado y que la vida sea mayor a 0, pierde una vida perry.
*   pero si perry no esta camuflado entonces hay dos casos, el primero si perry tiene 5 o mas energia, pierde 5 de energia y si tiene menos de 5 energia
*   pierde una vida.
*   Se borra el robot al finalizar alguno de los dos casos.
*/
void destruir_robot(juego_t* juego) {

    if (juego->cantidad_robots == 0) {
        printf("No hay robots para destruir.\n");
        return ;
    }
    
    for (int i = 0; i < juego->cantidad_robots; ++i)
    {
        int distancia = distancia_manhattan(juego->perry.posicion.fil, juego->perry.posicion.col,
                                        juego->robots[i].fil, juego->robots[i].col);
        if (distancia <= DISTANCIA_ROBOT) {
            if (juego->perry.camuflado == true && juego->perry.vida > 0) {
                juego->perry.vida -= 1;

            } 
            else if(juego->perry.camuflado == false) {
                if (juego->perry.energia >= ENERGIA_PARA_DESTRUIR_ROBOTS) {
                    juego->perry.energia = juego->perry.energia - ENERGIA_PARA_DESTRUIR_ROBOTS;

                } 
                else if (juego->perry.vida > 0 && juego->perry.energia < ENERGIA_PARA_DESTRUIR_ROBOTS) {
                    juego->perry.vida = juego->perry.vida - 1;
                    
                }
                for (int j = i; j < juego->cantidad_robots - 1; ++j) {
                    juego->robots[j] = juego->robots[j + 1];
                }
                juego->cantidad_robots--;
                juego->robots = realloc(juego->robots, sizeof(coordenada_t) * (size_t)(juego->cantidad_robots));
                --i;
            }
        printf("Robot destruido. Quedan %i robots.\n", juego->cantidad_robots);
        }
    }
}
/*
    Recibe como parametro Juego de tipo juego_t.
    Se les resta un segundo a las bombas por cada movimiento de perry.
    Si Explota una bomba disminuye una vida a perry y se borra del tablero.
*/
void tiempo_de_bomba(juego_t* juego){
    for (int i = 0; i < juego->tope_bombas ; ++i)
    {   
        if (juego->bombas[i].desactivada == false)
        {
            if (juego->bombas[i].timer > 0)
            {
                juego->bombas[i].timer = juego->bombas[i].timer -1;   
            }
        }
    }
    for (int i = 0; i < juego->tope_bombas; ++i)
    {
        if (juego->bombas[i].timer == 0)
        {   
            juego->bombas[i].desactivada = true;
           if (juego->perry.vida > 0)
           {
               juego->perry.vida --;
           }

        }
    }
}
/*
    Recibe como parametro Juego de tipo juego_t.
    Si perry se posiciona sobre un SOMBRERO entonces se le suma una vida.
    Si sucede lo anterior se elimina del tablero el sombrero.
*/

void aumento_de_vida(juego_t* juego) {
    for (int i = 0; i < juego->tope_herramientas; ++i) {
        if (juego->herramientas[i].tipo == SOMBRERO && 
            juego->herramientas[i].posicion.fil == juego->perry.posicion.fil && 
            juego->herramientas[i].posicion.col == juego->perry.posicion.col) {
            
            juego->perry.vida++;
            printf("La vida de Perry es %i\n", juego->perry.vida);
            juego->herramientas[i] = juego->herramientas[juego->tope_herramientas - 1];
            juego->tope_herramientas--;
            i--;
        }
    }
}
/*
    Recibe como parametro Juego de tipo juego_t.
    Si perry se posiciona sobre una GOLOSINA entonces se le suma 20 puntos de energia.
    Si sucede lo anterior se elimina del tablero la golosina.
*/

void aumento_de_energia(juego_t* juego) {
    for (int i = 0; i < juego->tope_herramientas; ++i) {
        if (juego->herramientas[i].tipo == GOLOSINA && 
            juego->herramientas[i].posicion.fil == juego->perry.posicion.fil && 
            juego->herramientas[i].posicion.col == juego->perry.posicion.col) {
            
            juego->perry.energia += AUMENTO_DE_ENERGIA;
            printf("La energia de Perry es %i\n", juego->perry.energia);
            juego->herramientas[i] = juego->herramientas[juego->tope_herramientas - 1];
            juego->tope_herramientas--;
            i--;
        }
    }
}

/*
 * Se verifica si hay un espacio vacio donde no se encuentre algun sombrero. devuelve false si hay un espacio vacio si no devuelve true.
 */
bool hay_sombrero(herramienta_t herramientas[MAX_HERRAMIENTAS], int fila, int col) { 

    bool hay_sombrero = false;
    for (int i = 0; i < MAX_SOMBREROS; ++i) {                                              
        if (herramientas[i].posicion.fil == fila && herramientas[i].posicion.col == col) {
            hay_sombrero = true;
        }
    }
    return hay_sombrero;
}

/*
 * Se verifica si hay un espacio vacio donde no se encuentre alguna golosina. devuelve false si hay un espacio vacio si no devuelve true.
 */
bool hay_golosinas(herramienta_t herramientas[MAX_HERRAMIENTAS], int fila, int col) { 
    bool hay_golosina = false;
    for (int i = 0; i < MAX_GOLOSINAS; ++i) {
        if (herramientas[i].posicion.fil == fila && herramientas[i].posicion.col == col) {
            hay_golosina = true;
        }
    }
    return hay_golosina;
}
/*
 * Se verifica si hay un espacio vacio donde no se encuentre alguna bomba. devuelve false si hay un espacio vacio si no devuelve true.
 */
bool hay_bomba(bomba_t bombas[MAX_BOMBAS], int fila, int col) { 
    bool hay_bomba = false;
    for (int i = 0; i < MAX_BOMBAS; ++i) {
        if (bombas[i].posicion.fil == fila && bombas[i].posicion.col == col) {
            hay_bomba = true;
        }
    }
    return hay_bomba;
}
/*
 * Se verifica si hay un espacio vacio donde no se encuentre algun familiar. devuelve false si hay un espacio vacio si no devuelve true.
 */

bool hay_familiar(familiar_t familiares[MAX_FAMILIARES], int fila, int col) {
    bool hay_familiar = false;
    for (int i = 0; i < MAX_FAMILIARES; ++i) {
        if (familiares[i].posicion.fil == fila && familiares[i].posicion.col == col) {
            hay_familiar = true;
        }
    }
    return hay_familiar;

}
/*
 * Se verifica si hay un espacio vacio donde no se encuentre perry. devuelve false si hay un espacio vacio si no devuelve true.
 */
int hay_perry(juego_t* juego, int fila, int col){
    return (juego->perry.posicion.fil == fila && juego->perry.posicion.col == col);
}
/*
 * Inicializa a perry con un maximo de vidas, maximo de energia, empieza no camuflado y le se asigna una posicion random.
 */
void inicializar_perry(juego_t* juego){
    juego->perry.vida = MAX_VIDAS;
    printf("La vida de Perry es %i\n", juego->perry.vida);
    juego->perry.energia = MAX_PUNTOS_ENERGIA;
    printf("La energia de Perry es %i\n", juego->perry.energia);
    juego->perry.camuflado = false;
    printf("Perry no está camuflado.\n");
    juego->perry.posicion.fil = rand() % MAX_FIL;
    juego->perry.posicion.col = rand() % MAX_COL;

}
/*
 * Se inicializa las bombas de forma completa, empiezan desactivadas pero con un timer random entre 50 y 300 y se le asignan una posicion random.
    Ademas tiene como condiciones en el bucle do while que no pisen a otras bombas o a perry.
 */
void inicializar_bombas(juego_t* juego){
    juego->tope_bombas = TOPE_BOMBAS;
   for (int i = 0; i < TOPE_BOMBAS; ++i) {
        int fila_bomba, col_bomba;
        do {
            fila_bomba = rand() % MAX_FIL;
            col_bomba = rand() % MAX_COL;
        } while ((hay_bomba(juego->bombas, fila_bomba, col_bomba)) || (hay_perry(juego,fila_bomba,col_bomba)));

        juego->bombas[i].posicion.fil = fila_bomba;
        juego->bombas[i].posicion.col = col_bomba;
        juego->bombas[i].timer = 50 + rand() % 251;
        juego->bombas[i].desactivada = false;
    }
    
}

/*
 * Se inicializa los sombreros de forma completa,se le asignan una posicion random y se le agrega el tipo de herramienta.
    Ademas tiene como condiciones en el bucle do while que no pisen a las bombas,perry y a otros sombreros.
 */
void inicializar_sombreros(juego_t* juego){ 
    juego->tope_herramientas = MAX_SOMBREROS;
    for (int j = 0; j < juego->tope_herramientas; ++j) {
        int fila_sombrero, col_sombrero;
        do {
            fila_sombrero = rand() % MAX_FIL;
            col_sombrero = rand() % MAX_COL;
        } while ((hay_sombrero(juego->herramientas, fila_sombrero, col_sombrero)) || 
            (hay_perry(juego,fila_sombrero,col_sombrero)) || (hay_bomba(juego->bombas, fila_sombrero, col_sombrero)));

        juego->herramientas[j].posicion.fil = fila_sombrero;
        juego->herramientas[j].posicion.col = col_sombrero;
        juego->herramientas[j].tipo = SOMBRERO;
    }
}
/*
 * Se inicializa las golosinas de forma completa,se le asignan una posicion random y se le agrega el tipo de herramienta.
    Ademas tiene como condiciones en el bucle do while que no pisen a las bombas,perry, sombreros y a otras golosinas.
 */
void inicializar_golosinas(juego_t* juego){
    juego->tope_herramientas = FIN_GOLOSINAS;
    for (int k = INICIO_GOLOSINAS; k < juego->tope_herramientas; ++k) {
        int fila_golosina, col_golosina;
        do {
            fila_golosina = rand() % MAX_FIL;
            col_golosina = rand() % MAX_COL;
        } while ((hay_golosinas(juego->herramientas, fila_golosina, col_golosina)) || (hay_perry(juego,fila_golosina,col_golosina)) 
            || (hay_bomba(juego->bombas, fila_golosina, col_golosina))  || (hay_sombrero(juego->herramientas, fila_golosina, col_golosina)));
        juego->herramientas[k].posicion.fil = fila_golosina;
        juego->herramientas[k].posicion.col = col_golosina;
        juego->herramientas[k].tipo = GOLOSINA;
    }
}
/*
 * Se inicializa las golosinas de forma completa,se le asignan una posicion random a cada familiar .
    Ademas tiene como condiciones en el bucle do while que no pisen a las bombas,perry, sombreros, golosinas y que no pise a ningun familiar.
 */
void incializar_familiares(juego_t* juego){
    juego->tope_familiares = TOPE_FAMILIARES;
    for (int i = 0; i < TOPE_FAMILIARES; ++i) {
        int fila_familia, col_familia;
        switch (i) {
            case 0:
                juego->familiares[i].inicial_nombre = PHINEAS;
                juego->familiares[i].sentido = DERECHA;
                break;
            case 1:
                juego->familiares[i].inicial_nombre = FERB;
                juego->familiares[i].sentido = IZQUIERDA;
                break;
            case 2:
                juego->familiares[i].inicial_nombre = CANDACE;
                juego->familiares[i].sentido = ARRIBA;
                break;
        }
        do {
            fila_familia = rand() % MAX_FIL;
            col_familia = rand() % MAX_COL;
        } while ((hay_familiar(juego->familiares, fila_familia, col_familia)) || (hay_golosinas(juego->herramientas, fila_familia, col_familia)) 
            || (hay_sombrero(juego->herramientas, fila_familia, col_familia))|| (hay_perry(juego,fila_familia,col_familia)) 
            || (hay_bomba(juego->bombas, fila_familia, col_familia)));
            juego->familiares[i].posicion.fil = fila_familia;
            juego->familiares[i].posicion.col = col_familia;
    }

}
/*
 * Se inicializa los robots de forma completa,se le asignan una posicion random a cada robot .
    Ademas tiene como condiciones en el bucle do while que no pisen a las bombas,perry, sombreros, golosinas, familiares y tampoco a otro robot.
    Se verifica que el vector iniciado por malloc no sea nulo, si es nulo retorna. Lo mismo se verifica cuando se utiliza realloc para aumentar 
    el tamaño del vector, si es nulo, retorna.
 */
void inicializar_robots(juego_t* juego) {
    if (juego->cantidad_robots == 0) {
        juego->robots = malloc(sizeof(coordenada_t));
        if (juego->robots == NULL) {
            return;
        }
    } else {
        coordenada_t* nuevos_robots = realloc(juego->robots, sizeof(coordenada_t) * (size_t)(juego->cantidad_robots + 1));
        if (nuevos_robots == NULL) {
            return;
        }
        juego->robots = nuevos_robots;
    }
    juego->cantidad_robots++;

    int fila_robot, col_robot;
    do {
        fila_robot = rand() % MAX_FIL;
        col_robot = rand() % MAX_COL;
    } while (hay_bomba(juego->bombas, fila_robot, col_robot) || hay_perry(juego, fila_robot, col_robot) || hay_robot(juego->robots, juego->cantidad_robots, fila_robot, col_robot) || 
        hay_golosinas(juego->herramientas, fila_robot, col_robot) || hay_sombrero(juego->herramientas, fila_robot, col_robot) || hay_familiar(juego->familiares, fila_robot, col_robot));

    juego->robots[juego->cantidad_robots - 1].fil = fila_robot;
    juego->robots[juego->cantidad_robots - 1].col = col_robot;
}
/*
 * Inicializa la feria vacia.
 */
void inicializar_feria(char feria[MAX_FIL][MAX_COL]){
    
    for (int i = 0; i < MAX_FIL; ++i) {
        for (int j = 0; j < MAX_COL; ++j) {
            feria[i][j] = VACIO;
        }
    }
}
/*
 *  Se recibe la feria vacia.
    agarra las posiciones generadas anteriormente de perry y las guarda en la feria.
 */
void ingresar_a_perry(juego_t juego, char feria[MAX_FIL][MAX_COL]){
    feria[juego.perry.posicion.fil][juego.perry.posicion.col] = PERRY;

}
/*
 *  Se recibe la feria con perry.
     agarra las posiciones generadas anteriormente de cada una de las bombas y las guarda en la feria.
 */
void ingresar_las_bombas(juego_t juego, char feria[MAX_FIL][MAX_COL]) {
    for (int i = 0; i < juego.tope_bombas; ++i) {
        feria[juego.bombas[i].posicion.fil][juego.bombas[i].posicion.col] = BOMBAS;
    }
}
/*
 *  Se recibe la feria con perry y todas las bombas.
    agarra las posiciones generadas anteriormente de cada uno de los sombreros y las guarda en la feria.
 */

void ingresar_los_sombreros(juego_t juego, char feria[MAX_FIL][MAX_COL]){
    for (int j = 0; j < MAX_SOMBREROS; ++j) {
        feria[juego.herramientas[j].posicion.fil][juego.herramientas[j].posicion.col] = SOMBRERO;
        
    }

}
/*
 *  Se recibe la feria con perry, las bombas y los sombreros. 
    agarra las posiciones generadas anteriormente de cada una de las golosinas y las guarda en la feria.
 */
void ingresar_las_golosinas(juego_t juego, char feria[MAX_FIL][MAX_COL]){
    for (int k = INICIO_GOLOSINAS; k < FIN_GOLOSINAS; ++k) {
        feria[juego.herramientas[k].posicion.fil][juego.herramientas[k].posicion.col] = GOLOSINA;
        
    } 
}
/*
 *   Se recibe la feria con perry, las bombas, los sombreros y las golosinas. 
    agarra las posiciones generadas anteriormente de cada uno de los familiares y las guarda en la feria.
 */
void ingresar_los_familiares(juego_t juego, char feria[MAX_FIL][MAX_COL]){
    for (int l = 0; l < TOPE_FAMILIARES ; ++l)
    {
        if (l == 0)
        {
            feria[juego.familiares[l].posicion.fil][juego.familiares[l].posicion.col] = PHINEAS;
            
        }
        if (l == 1)
        {
            feria[juego.familiares[l].posicion.fil][juego.familiares[l].posicion.col] = FERB;
            
        }
        if (l == 2)
        {
            feria[juego.familiares[l].posicion.fil][juego.familiares[l].posicion.col] = CANDACE;
            
        }

    }

}
/*
 *  Se recibe la feria con perry, las bombas, los sombreros, las golosinas y los familiares ya inicializados.
    Se debe igresar un ptr a cordenada_t que hace referencia al indice del vector de robots, por eso aumenta 1 por cada robot que se pone.
    agarra las posiciones generadas anteriormente de cada robot y las guarda en la feria.
 */
void ingresar_robot(coordenada_t* ptr, int cantidad_robot,char feria[MAX_FIL][MAX_COL]){

    for (int i = 0; i < cantidad_robot; ++i)
    {
        feria[ptr->fil][ptr->col] = ROBOT;
        ptr ++;
    }

}
void inicializar_juego(juego_t* juego) {
    inicializar_perry(juego);
    inicializar_bombas(juego);
    inicializar_sombreros(juego);
    inicializar_golosinas(juego);
    incializar_familiares(juego);
    juego->cantidad_robots = 0;
    juego->movimientos = 0;
}

void realizar_jugada(juego_t* juego, char accion) {
    
    switch (accion) {
        case ARRIBA:
            if (juego->perry.posicion.fil > INICIO_TABLERO) {
                juego->perry.posicion.fil--;
            }
            aumento_de_vida(juego);
            aumento_de_energia(juego);
            desactivar_bomba(juego);
            tiempo_de_bomba(juego);
            movimiento_de_ferb(juego);
            movimiento_de_candace(juego);
            movimiento_de_phineas(juego);
            distancia_familia(juego);
            juego->movimientos ++;
            if (juego->movimientos % 10 == 0)
            {
                inicializar_robots(juego);
            }
            
            destruir_robot(juego);

            break;
        case ABAJO:
            if (juego->perry.posicion.fil < FIN_TABLERO) {
                juego->perry.posicion.fil++;
            }
            aumento_de_vida(juego);
            aumento_de_energia(juego);
            desactivar_bomba(juego);
            tiempo_de_bomba(juego);
            movimiento_de_ferb(juego);
            movimiento_de_candace(juego);
            movimiento_de_phineas(juego);
            distancia_familia(juego);
            juego->movimientos ++;
            if (juego->movimientos % 10 == 0)
            {
                inicializar_robots(juego);
            }
            
            destruir_robot(juego);
            break;
        case DERECHA:
            if (juego->perry.posicion.col < FIN_TABLERO) {
                juego->perry.posicion.col++;
            }
            aumento_de_vida(juego);
            aumento_de_energia(juego);
            desactivar_bomba(juego);
            tiempo_de_bomba(juego);
            movimiento_de_ferb(juego);
            movimiento_de_candace(juego);
            movimiento_de_phineas(juego);
            distancia_familia(juego);
            juego->movimientos ++;
            if (juego->movimientos % 10 == 0)
            {
                inicializar_robots(juego);

            }
            destruir_robot(juego);

            break;
        case IZQUIERDA:
            if (juego->perry.posicion.col > INICIO_TABLERO) {
                juego->perry.posicion.col--;
            }
            aumento_de_vida(juego);
            aumento_de_energia(juego);
            desactivar_bomba(juego);
            tiempo_de_bomba(juego);
            movimiento_de_ferb(juego);
            movimiento_de_candace(juego);
            movimiento_de_phineas(juego);
            distancia_familia(juego);
            juego->movimientos ++;
            if (juego->movimientos % 10 == 0)
            {
                inicializar_robots(juego);
            }
            
            destruir_robot(juego);

            break;
        case 'Q':
            juego->perry.camuflado = !juego->perry.camuflado;
            if (juego->perry.camuflado) {
                printf("Perry está ahora camuflado.\n");
            } else {
                printf("Perry no está camuflado.\n");
            }
            break;
    }
}

void imprimir_terreno(juego_t juego) {
    char feria[MAX_FIL][MAX_COL];

    inicializar_feria(feria);
    ingresar_a_perry(juego, feria);
    ingresar_las_bombas(juego, feria);
    ingresar_robot(juego.robots, juego.cantidad_robots, feria);
    ingresar_los_sombreros(juego, feria);
    ingresar_las_golosinas(juego, feria);
    ingresar_los_familiares(juego, feria);


    for (int i = INICIO_TABLERO; i < MAX_FIL; ++i) {
        for (int j = INICIO_TABLERO; j < MAX_COL; ++j) {
            printf(" [%c] ", feria[i][j]);
        }
        printf("\n");
    }
}

int estado_juego(juego_t juego) {
    int contador_de_bombas_desactivadas = 0;
    for (int i = 0; i < TOPE_BOMBAS; ++i) {
        if (juego.bombas[i].desactivada == true) {
            contador_de_bombas_desactivadas++;
        }
    }

    if (contador_de_bombas_desactivadas == TOPE_BOMBAS) {
        return GANAR;

    }

    if (juego.perry.vida == 0) {
        return PERDER;
        
    }

    return SIGUE_JUGANDO;
}

