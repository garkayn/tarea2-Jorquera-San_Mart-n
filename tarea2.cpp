using namespace std;
#include <iostream>
#include <string>
#include<fstream>
#include<limits>

struct Room {
    string nombre;
    string informacion;
    Room* caminoIzquierdo;
    Room* caminoCentral;
    Room* caminoDerecho;
    string tipoHabitacion;
    int numeroNodo;
    float evento;
    float combate;
};


class Mapa {
    private:
        Room* inicio;
    public:
        void armarMapa(fstream& file){
            string usable;
            int n;
            string descartable;
            char c;
                
            getline(file, descartable); 
            getline(file, descartable); 

            file>>n;
            file.get(c);
            cout<<c;
            Room* habitaciones = new Room[n];

            
            for(int i=0;i<n;i++){
                file >> habitaciones[i].numeroNodo;
                file >> habitaciones[i].tipoHabitacion;
                file.ignore(1000, '\n'); //pendiente revisar bien de por que funciona esta linea, y por que se va a la shit sin esto XD
                getline(file, habitaciones[i].nombre);
                getline(file, habitaciones[i].informacion);

                cout<<habitaciones[i].numeroNodo<<endl;
                cout<<habitaciones[i].tipoHabitacion<<endl;
                cout<<habitaciones[i].nombre<<endl;
                cout<<habitaciones[i].informacion<<endl;
                cout<<endl;
            }

        }
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

int main(){
    Mapa mapa;
    fstream file;
    file.open("data.map", ios::in);
    mapa.armarMapa(file);
    
}