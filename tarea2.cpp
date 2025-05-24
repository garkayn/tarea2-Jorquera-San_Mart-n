using namespace std;
#include <iostream>
#include <string>
#include<fstream>

struct Room {
    string nombre;
    string informacion;
    Room* izq;
    Room* centro;
    Room* der;
    string tipoHabitacion;
    int numeroNodo;
    bool evento;
    bool combate;
};

class Mapa {
    private:
        Room* inicio;
        Room* actual;
        int numeroHabitaciones;
    public:
        void armarMapa(fstream &file){
            string usable;
            int n;
            string descartable;
            char c;
                
            getline(file, descartable); 
            getline(file, descartable); 

            file>>n;
            file.get(c);
            cout<<c;
            Room** habitaciones = new Room*[n];

            
            for(int i=0;i<n;i++){
                Room* nuevo = new Room();
                nuevo->izq = nullptr;
                nuevo->centro = nullptr;
                nuevo->der = nullptr;
                file >> nuevo->numeroNodo;
                file >> nuevo->tipoHabitacion;
                file.ignore(1000, '\n'); //pendiente revisar bien de por que funciona esta linea, y por que se va a la shit sin esto XD
                getline(file, nuevo->nombre);
                getline(file, nuevo->informacion);
                habitaciones[i] = nuevo;
            }
            //ahora procedemos a leer los enlaces
            getline(file, descartable);
            file>>n;
            file.ignore(1000, '\n');
            cout<<n<<endl; //agregar las habitaciones de sigilo y brutalidad como habitaciones extra para poder ramificar el arbol
            int j;
            int k;
            for(int i=0;i<n;i++){
                file >> j;
                file >> descartable;
                file >> k;
                file.ignore(1000, '\n');
                if(habitaciones[j]->izq == nullptr){
                    habitaciones[j]->izq = habitaciones[k];
                    cout<<"Camino izquierdo: "<<habitaciones[j]->izq->nombre<<endl;
                }
                else if(habitaciones[j]->centro == nullptr){
                    habitaciones[j]->centro = habitaciones[k];
                    cout<<"Camino centro: "<<habitaciones[j]->centro->nombre<<endl;
                }
                else if(habitaciones[j]->der == nullptr){
                    habitaciones[j]->der = habitaciones[k];
                     cout<<"Camino derecho: "<<habitaciones[j]->der->nombre<<endl;
                }
            }
            inicio = habitaciones[0];
            delete[] habitaciones;
            cout<<inicio->izq->centro->nombre;
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
    fstream file;
    file.open("data.map", ios::in);
    Mapa mapa;
    mapa.armarMapa(file);
}