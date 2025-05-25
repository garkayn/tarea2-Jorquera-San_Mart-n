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
            int n;
            string descartable;
                
            getline(file, descartable); 
            getline(file, descartable); 

            file>>n;

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
            file.ignore(1000, '\n'); //agregar las habitaciones de sigilo y brutalidad como habitaciones extra para poder ramificar el arbol
            int j;
            int k;
            for(int i=0;i<n;i++){
                file >> j;
                file >> descartable;
                file >> k;
                file.ignore(1000, '\n');
                if(habitaciones[j]->izq == nullptr){
                    habitaciones[j]->izq = habitaciones[k];
                }
                else if(habitaciones[j]->centro == nullptr){
                    habitaciones[j]->centro = habitaciones[k];
                }
                else if(habitaciones[j]->der == nullptr){
                    habitaciones[j]->der = habitaciones[k];
                }
            }
            inicio = habitaciones[0];
            actual = inicio;
            delete[] habitaciones;
        }
        
        void moverIzquierda(){
            actual = actual->izq;
            cout<<actual->nombre<<endl;
        }
        void moverCentro(){
            actual = actual->centro;
            cout<<actual->nombre<<endl;
        }
        void moverDerecha(){
            actual = actual->der;
            cout<<actual->nombre<<endl;
        }

        void procesarNodo(){
            if(actual->tipoHabitacion == "(COMBATE)"){
                cout<<"comboijueperra"<<endl;
            }
            if(actual->tipoHabitacion == "(COMBATE FINAL)"){
                cout<<"SE FINO EL COMPARE"<<endl;
            }
            if(actual->tipoHabitacion == "(FINAL)"){
                cout<<"wea triste :("<<endl;
            }
            if(actual->tipoHabitacion == "(MINIJUEGO)"){
                cout<<"corre perra correeeee"<<endl;
            }
            if(actual->tipoHabitacion == "(EVENTO)"){
                cout<<"Nicki Nicole siono"<<endl;
            }
        }
};

struct Combatiente { //Investigar herencia en clases
    int damage;
    Combatiente* next;
};

class ColaTurnos {
    private:
        Combatiente* frente;
        Combatiente* cola;
        int size;
    public:
        ColaTurnos() : frente(nullptr), cola(nullptr), size(0) {}

        ~ColaTurnos() {
            while (frente != nullptr) {
                pasarTurno();
            }
        }

        void agregarCombatiente(int dmg) {   
            Combatiente* nuevo = new Combatiente{dmg, nullptr};       
            if (frente == nullptr) {    
                frente = nuevo;
                cola = frente;
            }
            else {
                cola->next = nuevo;
                cola = nuevo;
            }          
            size++;
            nuevo->next = nullptr; // Asegura que el nuevo nodo apunte a nullptr          
        }

        void pasarTurno() {
            if (frente != nullptr) {
                Combatiente* temp = frente;
                frente = frente->next;
                delete temp;
                if (frente == nullptr) {
                    cola = nullptr;           // Si la cola quedó vacía
                }
                size--;
            }
        }

        bool isEmpty() const {
            return frente == nullptr;
        }

        int getSize() const {
            return size;
        }
};

struct Evento {
    string nombre;
    float probabilidad;
    string informacion;
    int cantidadOpciones;
    string* opciones;
    string* infoOpciones;
    //arreglo de strings con las opciones
};

class Enemigo {
    private:
        string nombre;
        int vida;
        int ataque;
        float precision;
        int probabilidad;
};

class Jefe {
    private:
        string nombre;
        int vida;
        int ataque;
        float precision;
};

class Jugador {
    private :
        int vida;
        int ataque;
        float precision;
        int recuperacion;
        int cordura;
};

Evento* leerEventos(fstream &file, int &cant_eventos){
    string descartable;
    getline(file, descartable);
    file>>cant_eventos;
    cout<<cant_eventos<<endl;
    file.ignore(1000, '\n');
    Evento* eventos = new Evento[cant_eventos];
    for(int i=0; i<cant_eventos;i++){
        getline(file, eventos[i].nombre);
        getline(file, descartable);
        file>>eventos[i].probabilidad;
        file.ignore(1000, '\n');
        getline(file, eventos[i].informacion);
        getline(file, descartable);
        file>>eventos[i].cantidadOpciones;
        file.ignore(1000, '\n');
        eventos[i].opciones = new string[eventos[i].cantidadOpciones];
        eventos[i].infoOpciones = new string[eventos[i].cantidadOpciones];
        for (int j = 0; j < eventos[i].cantidadOpciones; j++) {
            getline(file, eventos[i].opciones[j]);
            getline(file, eventos[i].infoOpciones[j]);
        }
    }

    return eventos;
}

int main(){
    fstream file;
    file.open("data.map", ios::in);
    Mapa mapa;
    mapa.armarMapa(file);
    int cant_eventos;
    Evento* eventos = leerEventos(file, cant_eventos);
    cout<<eventos[0].opciones[0]<<endl<<eventos[0].infoOpciones[0];
}