using namespace std;
#include <iostream>
#include <string>

struct Room {
    string nombre;
    string informacion;
    Room* caminoIzquierdo;
    Room* caminoCentral;
    Room* caminoDerecho;
    string tipo;
    int numeroNodo;
    float evento;
    float combate;
};

struct Evento {
    string nombre;
    float probabilidad;
    string informacion;
};

class Enemigo {
    private:
        int vida;
        int ataque;
        float precision;
        int recuperacion;
};

class Jugador {
    private :
        int vida;
        int ataque;
        float precision;
        int recuperacion;
        int cordura;
};