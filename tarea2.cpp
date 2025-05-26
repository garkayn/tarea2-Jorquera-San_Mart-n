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

struct Combatiente {
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
                    cola = nullptr; // Si la cola quedó vacía
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
    string* consecuencias;
};

Evento* leerEventos(fstream &file, int &cant_eventos){
    string descartable;
    getline(file, descartable);
    file>>cant_eventos;
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
        eventos[i].consecuencias = new string[eventos[i].cantidadOpciones];
        for (int j = 0; j < eventos[i].cantidadOpciones; j++) {
            getline(file, eventos[i].opciones[j]);
            getline(file, eventos[i].infoOpciones[j]);
            getline(file, eventos[i].consecuencias[j]);
        }
    }
    return eventos;
}

class Jugador {
    private :
        int vida;
        int ataque;
        float precision;
        int cordura;
        float sigilo;
        int recuperacion;
    public :
        Jugador(){vida = 60; ataque = 5; precision = 0.8; cordura = 100; sigilo = 0.2;}
        bool estasVivo(){
            if(vida>0) return true;
            return false;
        }
        void cambioVida(int cambio){
            vida += cambio;
        }
        void cambioAtaque(int cambio){
            ataque += cambio;
        }
        void cambioPresicion(int cambio){
            precision += cambio;
        }
        void cambioCordura(int cambio){
            cordura += cambio;
        }
        void cambioSigilo(int cambio){
            sigilo += cambio;
        }
        int mostrarAtaque(){
            return ataque;
        }
        int mostrarPrecision(){
            return precision;
        }
        int mostrarCordura(){
            return sigilo;
        }
        float mostrarSigilo(){
            return sigilo;
        }
        void recibirGolpe(int damage){
            vida -= damage;
        }
};



struct statsEnemigos{
    string nombre;
    int vida;
    int ataque;
    float precision;
    float probabilidad;
};

class Enemigo {
    private:
        string nombre;
        int vida;
        int ataque;
        float precision;
        float probabilidad;
    public:
        Enemigo(statsEnemigos stats){nombre = stats.nombre; vida = stats.vida; ataque = stats.ataque; precision = stats.precision; probabilidad = stats.probabilidad;}
        string mostrarNombre(){
            return nombre;
        }
        int mostrarAtaque(){
            return ataque;
        }
        int mostrarPrecision(){
            return precision;
        }
        int mostrarProbabilidad(){
            return probabilidad;
        }
};

statsEnemigos* leerEnemigos(fstream &file, int &cant_enemigos){
    string descartable;
    getline(file, descartable);
    file>>cant_enemigos;
    file.ignore(1000, '\n');
    statsEnemigos* enemigos = new statsEnemigos[cant_enemigos];
    for(int i=0;i<cant_enemigos;i++){
        getline(file, enemigos[i].nombre);
        file>>descartable;
        file>>enemigos[i].vida;
        file>>descartable;
        file>>enemigos[i].ataque;
        file>>descartable;
        file>>enemigos[i].precision;
        file>>descartable;
        file>>enemigos[i].probabilidad;
        file.ignore(1000, '\n');
    }
    return enemigos;
}

struct statsJefes{
    string nombre;
    int vida;
    int ataque;
    float precision;
};

class Jefe {
    private:
        string nombre;
        int vida;
        int ataque;
        float precision;
    public:
        Jefe(statsJefes stats){nombre = stats.nombre; vida = stats.vida; ataque = stats.ataque; precision = stats.precision;}
        string mostrarNombre(){
            return nombre;
        }
        int mostrarAtaque(){
            return ataque;
        }
        int mostrarPrecision(){
            return precision;
        }
};

statsJefes* leerJefes(fstream &file, int &cant_jefes){
    string descartable;
    getline(file, descartable);
    file>>cant_jefes;
    file.ignore(1000, '\n');
    statsJefes* jefes = new statsJefes[cant_jefes];
    for(int i=0;i<cant_jefes;i++){
        getline(file, jefes[i].nombre);
        file>>descartable;
        file>>jefes[i].vida;
        file>>descartable;
        file>>jefes[i].ataque;
        file>>descartable;
        file>>jefes[i].precision;
        file.ignore(1000, '\n');        
    }
    return jefes;
}

struct statsMejoras{
    string* info;
    int vida;
    int ataque;
    float precision;
    float sigilo;
    int recuperacion;
};

statsMejoras leerMejoras(fstream &file, int &cant_mejoras){
    string descartable;
    statsMejoras mejoras;
    getline(file,descartable);
    file>>cant_mejoras;
    mejoras.info = new string[cant_mejoras];
    for(int i=0;i<cant_mejoras;i++){
        string mejora_s;
        string informacion = "";
        string concatenar;
        file>>concatenar;
        informacion += concatenar;
        informacion += " ";
        file>>mejora_s;
        informacion+=mejora_s;   
        if(concatenar == "Vida"){
            int mejora = stoi(mejora_s);
            mejoras.vida = mejora;    
        }
        else if(concatenar == "Ataque"){
            int mejora = stoi(mejora_s);
            mejoras.ataque = mejora;    
        }
        else if(concatenar == "Precision") {
            float mejora_f = stof(mejora_s);
            mejoras.precision = mejora_f;
        }   
        else if(concatenar == "Sigilo") {
            float mejora_f = stof(mejora_s);
            mejoras.sigilo = mejora_f;
        }       
        else{
            int mejora = stoi(mejora_s);
            mejoras.recuperacion = mejora;
        }        
        mejoras.info[i] = informacion;
    }

    return mejoras;
}

int main(){
    fstream file;
    file.open("data.map", ios::in);
    Mapa mapa;
    mapa.armarMapa(file);
    int cant_eventos;
    int cant_enemigos;
    int cant_jefes;
    int cant_mejoras;
    Evento* eventos = leerEventos(file, cant_eventos);
    statsEnemigos* arrayStatsEnemigos = leerEnemigos(file, cant_enemigos);
    statsJefes* arrayStatsJefes = leerJefes(file, cant_jefes);
    statsMejoras mejoras = leerMejoras(file, cant_mejoras);
}