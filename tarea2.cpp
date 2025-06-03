#include<iostream>
#include<string>
#include<fstream>
#include<random>

using namespace std;

// =============================================
// Sección 1: Funciones de Utilidad Aleatoria
// =============================================

/*****
* int aleatorioEntero
******
* Genera un número entero aleatorio entre un mínimo y un máximo (ambos incluidos).
******
* Input:
* int min : valor mínimo del rango
* int max : valor máximo del rango
******
* Returns:
* int , número entero aleatorio en el rango [min, max]
*****/
int aleatorioEntero(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}


/*****
* int enteroConProbabilidad
******
* Genera un número entero aleatorio entre min y max (ambos incluidos),
* utilizando una distribución basada en los pesos proporcionados.
******
* Input:
* int min : valor mínimo del rango
* int max : valor máximo del rango
* double* pesos : arreglo de pesos asociados a cada valor en el rango [min, max]
******
* Returns:
* int , número entero aleatorio en el rango [min, max], elegido según la distribución de pesos
*****/
int enteroConProbabilidad(int min, int max, double* pesos) {
    static random_device rd;
    static mt19937 gen(rd());

    int cantidad = max - min + 1;

    discrete_distribution<> dist(pesos, pesos + cantidad);

    int indice = dist(gen);

    return min + indice;
}

/*****
* bool aciertaAtaque
******
* Determina si un ataque acierta en función de una probabilidad dada.
******
* Input:
* double probabilidad : valor entre 0.0 y 1.0 que representa la probabilidad de acierto
******
* Returns:
* bool , true si el ataque acierta, false si falla
*****/
bool aciertaAtaque(double probabilidad) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.0, 1.0);

    return dist(gen) < probabilidad;
}

// =============================================
// Sección 2: Clase Personaje jugable
// =============================================

class Jugador {
    private :
        int vida;
        int ataque;
        double precision;
        int cordura;
        float sigilo;
        int recuperacion;

    public :
        Jugador(){vida = 60; ataque = 7; precision = 0.8; cordura = 6; sigilo = 0.2; recuperacion = 5;}
        // Métodos de modificación
        void cambioVida(int cambio){
            vida += cambio;
        }
        void cambioAtaque(int cambio){
            ataque += cambio;
        }
        void cambioPresicion(float cambio){
            precision += cambio;
        }
        void cambioCordura(int cambio){
            cordura += cambio;
        }
        void cambioSigilo(float cambio){
            sigilo += cambio;
        }
        void cambioRecuperacion(int cambio){
            recuperacion += cambio;
        }
        void matarJugador(){
            vida = 0;
        }

        // Métodos para retornar estado
        int mostrarVida(){
            return vida;
        }
        int mostrarAtaque(){
            return ataque;
        }
        double mostrarPrecision(){
            return precision;
        }
        int mostrarCordura(){
            return cordura;
        }
        float mostrarSigilo(){
            return sigilo;
        }
        int mostrarRecuperacion(){
            return recuperacion;
        }
        bool estaVivo(){
            return vida > 0;
        }
};

// =============================================
// Sección 3: TDA Cola para el combate
// =============================================
struct combatiente {
    string nombre;
    int damage;
    combatiente* next;
};

class colaCombate {
    private:
        combatiente* frente;
        combatiente* cola;
        int size;

    public:
        colaCombate() : frente(nullptr), cola(nullptr), size(0) {}

        ~colaCombate() {
            while (frente != nullptr) {
                pasarTurno();
            }
        }

        void agregarCombatiente(int dmg) {   
            combatiente* nuevo = new combatiente;
            nuevo->damage = dmg;
            nuevo->next = nullptr;       
            if (frente == nullptr) {    
                frente = nuevo;
                cola = frente;
            }
            else {
                cola->next = nuevo;
                cola = nuevo;
            }          
            size++;
            nuevo->next = nullptr;       
        }

        void pasarTurno() {
            if (frente != nullptr) {
                combatiente* temp = frente;
                frente = frente->next;
                delete temp;
                if (frente == nullptr) {
                    cola = nullptr;
                }
                size--;
            }
        }

        int frontDmg() {
            return frente->damage;
        }

        string frontName() {
            return frente->nombre;
        }

        bool isEmpty() {
            return frente == nullptr;
        }

        int getSize() {
            return size;
        }
};

// =============================================
// Sección 4: Enemigos
// =============================================

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
        double precision;
        double probabilidad;
    public:
        Enemigo(){nombre = ""; vida = 0; ataque = 0; precision = 0; probabilidad = 0;}

        // Métodos de asignación
        void recibirNombre(string asignacion){
            nombre = asignacion;
        }
        void recibirVida(int asignacion){
            vida = asignacion;
        }
        void recibirPrecision(float asignacion){
            precision = asignacion;
        }
        void recibirProbabilidad(double asignacion){
            probabilidad = asignacion;
        }
        void recibirAtaque(int asignacion){
            ataque = asignacion;
        }

        // Métodos de modificación
        void perderVida(int dmg){
            vida -= dmg;
        }
        void perderPrecision(double asignacion){
            precision -= asignacion;
        }

        // Métodos para retornar estado
        string mostrarNombre(){
            return nombre;
        }
        int mostrarVida(){
            return vida;
        }
        int mostrarAtaque(){
            return ataque;
        }
        double mostrarPrecision(){
            return precision;
        }
        double mostrarProbabilidad(){
            return probabilidad;
        }     
        bool estaVivo(){
            return vida > 0;
        }
    };

/*****
* statsEnemigos* leerEnemigos
******
* Lee información de enemigos desde un archivo y la almacena en un arreglo dinámico de structs statsEnemigos.
******
* Input:
* fstream &file : referencia al archivo desde el cual se leen los datos
* int &cant_enemigos : referencia a una variable donde se almacenará la cantidad de enemigos leídos
******
* Returns:
* statsEnemigos* , puntero al arreglo dinámico de structs con los datos de los enemigos
*****/
statsEnemigos* leerEnemigos(fstream &file, int &cant_enemigos){
    string descartable;

    getline(file, descartable);
    file>>cant_enemigos;
    file.ignore(1000, '\n');

    statsEnemigos* enemigos = new statsEnemigos[cant_enemigos];

    for(int i=0; i < cant_enemigos; i++){
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

/*****
* Enemigo* procesarEnemigos
******
* Crea un arreglo dinámico de objetos Enemigo a partir de un arreglo de structs statsEnemigos,
* copiando sus atributos correspondientes a cada objeto Enemigo.
******
* Input:
* statsEnemigos* array : arreglo de structs con los datos de cada enemigo
* int cantidad : cantidad de enemigos a procesar
******
* Returns:
* Enemigo* , puntero al arreglo dinámico de enemigos creado
*****/
Enemigo* procesarEnemigos(statsEnemigos* array, int cantidad){
    Enemigo* enemigos = new Enemigo[cantidad];

    for(int i = 0; i < cantidad; i++){
        enemigos[i].recibirNombre(array[i].nombre);
        enemigos[i].recibirVida(array[i].vida);
        enemigos[i].recibirAtaque(array[i].ataque);
        enemigos[i].recibirPrecision(array[i].precision);
        enemigos[i].recibirProbabilidad(array[i].probabilidad);   
    }

    return enemigos;
}

// =============================================
// Sección 5: Combate para enemigos
// =============================================

/*****
* void seleccionarEnemigos
******
* Selecciona una cantidad específica de enemigos de un arreglo, basándose en sus probabilidades individuales,
* y los almacena en otro arreglo para ser usados como combatientes.
******
* Input:
* Enemigo* enemigos : Arreglo de enemigos disponibles para selección
* int enemigosTotales : Cantidad total de enemigos en el arreglo
* Enemigo* combatientes : Arreglo donde se almacenarán los enemigos seleccionados
* int cantidadEnemigosCombate : Cantidad de enemigos en el combate
******
* Returns:
* void , Los enemigos seleccionados se guardan en el arreglo 'combatientes' y se imprime el nombre de cada uno
*****/
void seleccionarEnemigos(Enemigo* enemigos, int enemigosTotales, Enemigo* combatientes, int cantidadEnemigosCombate) {
    double* pesos = new double[enemigosTotales];
    for (int i = 0; i < enemigosTotales; i++) {
        pesos[i] = enemigos[i].mostrarProbabilidad();
    }
    for (int j = 0; j < cantidadEnemigosCombate; j++) {
        int seleccionadorEnemigo = enteroConProbabilidad(0, enemigosTotales - 1, pesos);
        combatientes[j] = enemigos[seleccionadorEnemigo];
    }
    delete[] pesos;
}

/*****
* void entradaSigilosaOBruto
******
* Permite al jugador elegir entre un enfoque sigiloso o directo (bruto) al iniciar un combate,
* aplicando diferentes efectos a los enemigos y al jugador según la elección.
******
* Input:
* Jugador &jugador : Referencia al objeto jugador que realiza la acción
* Enemigo* combatientes : Arreglo de enemigos que participarán en el combate
* int cantidadEnemigosCombate : Cantidad de enemigos en el combate
******
* Returns:
* void , Modifica el estado de los enemigos y del jugador según la estrategia elegida
*****/
void entradaSigilosaOBruto(Jugador &jugador, Enemigo* combatientes, int cantidadEnemigosCombate) {
    string respuesta;
    while (true) {
        cout << "Quieres entrar con sigilo? (s/n)" << endl;
        cin >> respuesta;

        if (respuesta == "S" || respuesta == "s") {
            cout << "Decides entrar de manera sigilosa y alcanzas a atacar a varios enemigos sin ser detectado." << endl;
            
            for (int j = 0; j < cantidadEnemigosCombate; j++) {
                combatientes[j].perderVida(jugador.mostrarAtaque() * (jugador.mostrarSigilo() + 1));
            }

            jugador.cambioCordura(2);
            break;

        } else if (respuesta == "N" || respuesta == "n"){
            cout << "Decides atacar de forma directa, causando terror en tus enemigos. La precision de los enemigos a disminuido." << endl;
            
            for (int j = 0; j < cantidadEnemigosCombate; j++) {
                combatientes[j].perderPrecision(0.1);
            }
            
            jugador.cambioCordura(-2);
            break;

        } else {
            cout << "Opción no válida. Usa s/n." << endl;
        }
    }
}

/*****
* void mostrarEstadoCombatientes
******
* Muestra el estado actual del combate contra un grupo de enemigos.
******
* Input:
* Enemigo* combatientes : Arreglo de combatientes participantes en el combate
* int cantidadEnemigosCombate : Cantidad total de enemigos en el combate
******
* Returns:
* void , Imprime el estado de la vida y el nombre de los combatientes
*****/
void mostrarEstadoCombatientes(Jugador &jugador, Enemigo* combatientes, int cantidadEnemigosCombate) {
    cout << "Batman";
    for (int i = 0; i < cantidadEnemigosCombate; i++) {
        cout << " | " << combatientes[i].mostrarNombre();
    }
    cout << endl;
    if (jugador.estaVivo()) {
        cout << jugador.mostrarVida();       
    } else {
        cout << "X";
    }

    for (int i = 0; i < cantidadEnemigosCombate; i++) {
        if (combatientes[i].estaVivo()) {
            cout <<  " | " << combatientes[i].mostrarVida();
        } else {
            cout << " | " << "X";
        }
    }
    cout << endl;
}

/*****
* void turnoJugador
******
* Gestiona el turno de ataque del jugador durante el combate, verificando si acierta,
* aplicando el daño al enemigo objetivo, y actualizando la cola de turnos.
* También maneja el cambio de objetivo si el enemigo actual es derrotado.
******
* Input:
* Jugador &jugador : Referencia al objeto jugador que realiza el ataque
* Enemigo* combatientes : Arreglo de enemigos en combate
* int &cont_objetivo : Referencia al índice del enemigo objetivo actual (se incrementa si muere)
* colaCombate &turnos : Referencia a la cola que gestiona el orden de turnos
******
* Returns:
* void , Modifica el estado del combate y muestra resultados en consola
*****/
void turnoJugador(Jugador &jugador, Enemigo* combatientes, int &cont_objetivo, colaCombate &turnos) {
    if (aciertaAtaque(jugador.mostrarPrecision())) {
        turnos.agregarCombatiente(jugador.mostrarAtaque());
        combatientes[cont_objetivo].perderVida(turnos.frontDmg());

        cout << "Atacas a " << combatientes[cont_objetivo].mostrarNombre() << " y le haces " << jugador.mostrarAtaque() << " de daño. "<< endl;

        turnos.pasarTurno();

        if (!combatientes[cont_objetivo].estaVivo()) {
            cont_objetivo++;
        }
    } else {
        cout << "Has fallado!" << endl;
    }
}

/*****
* void turnosEnemigos
******
* Gestiona completamente los turnos de los enemigos en combate, incluyendo:
* 1. Determinación de ataques exitosos/fallidos
* 2. Actualización de la cola de turnos
* 3. Aplicación de daño al jugador
* 4. Visualización del estado del combate
******
* Input:
* Jugador &jugador : Referencia al objeto jugador (recibe daño)
* Enemigo* combatientes : Arreglo de enemigos en combate
* int cantidadEnemigosCombate : Total de enemigos participantes
* colaCombate &turnos : Referencia a la cola de gestión de turnos
******
* Returns:
* void , Actualiza el estado del combate y muestra información en consola
*****/
void turnosEnemigos(Jugador &jugador, Enemigo* combatientes, int cantidadEnemigosCombate, colaCombate &turnos) {
    // Se agregan los enemigos a la cola
    for (int i = 0; i < cantidadEnemigosCombate; i++) {
        if (combatientes[i].estaVivo()) {
            if (aciertaAtaque(combatientes[i].mostrarPrecision())) {
                turnos.agregarCombatiente(combatientes[i].mostrarAtaque());
            } else {
                cout << endl;
                cout << combatientes[i].mostrarNombre() << " ha fallado!" << endl;
                cout << endl;
                mostrarEstadoCombatientes(jugador, combatientes, cantidadEnemigosCombate);
            }
        }
    }
    // Se ejecutan ataques de los enemigos
    for (int i = 0; i < cantidadEnemigosCombate; i++) {
        if (combatientes[i].estaVivo() && !turnos.isEmpty()) {

            cout << endl;
            
            jugador.cambioVida(-turnos.frontDmg());
            cout << combatientes[i].mostrarNombre() << " ataca y te hace " << turnos.frontDmg() << " de daño."<< endl;
            turnos.pasarTurno();

            cout << endl;

            mostrarEstadoCombatientes(jugador, combatientes, cantidadEnemigosCombate);

            if (!jugador.estaVivo()) break;
        }
    }
}

/*****
* void combate
******
* Gestiona el flujo completo de un combate, incluyendo:
* 1. Selección aleatoria de enemigos
* 2. Inicio del combate con entrada sigilosa o bruta
* 3. Turnos alternados entre jugador y enemigos
* 4. Visualización constante del estado del combate
* 5. Liberación de recursos al finalizar
******
* Input:
* Jugador &jugador : Referencia al objeto jugador que participa en el combate
* Enemigo* enemigos : Arreglo de todos los enemigos disponibles
* int enemigosTotales : Cantidad total de enemigos en el arreglo
******
* Returns:
* void , Modifica el estado del jugador y enemigos durante el combate
*****/
void combate(Jugador &jugador, Enemigo* enemigos, int enemigosTotales) {
    int cantidadEnemigosCombate = aleatorioEntero(1, 3);
    Enemigo* combatientes = new Enemigo[cantidadEnemigosCombate];
    colaCombate turnos;
    int cont_objetivo = 0;

    seleccionarEnemigos(enemigos, enemigosTotales, combatientes, cantidadEnemigosCombate);

    cout << endl;
    cout << "=== COMBATE ===" << endl;
    cout << endl;

    mostrarEstadoCombatientes(jugador, combatientes, cantidadEnemigosCombate);
    
    cout << endl;

    entradaSigilosaOBruto(jugador, combatientes, cantidadEnemigosCombate);

    cout << endl;

    mostrarEstadoCombatientes(jugador, combatientes, cantidadEnemigosCombate);

    while (jugador.estaVivo() && cont_objetivo != cantidadEnemigosCombate) {
        cout << endl;

        turnoJugador(jugador, combatientes, cont_objetivo, turnos);

        cout << endl;

        mostrarEstadoCombatientes(jugador, combatientes, cantidadEnemigosCombate);

        turnosEnemigos(jugador, combatientes, cantidadEnemigosCombate, turnos);
    }

    delete[] combatientes;
    turnos.~colaCombate();
}

// =============================================
// Sección 6: Jefes
// =============================================

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
        Jefe(){nombre = ""; vida = 0; ataque = 0; precision = 0;}
        
        // Métodos de asignación
        void recibirNombre(string asignacion){
            nombre = asignacion;
        }
        void recibirVida(int asignacion){
            vida = asignacion;
        }
        void recibirPrecision(float asignacion){
            precision = asignacion;
        }
        void recibirAtaque(int asignacion){
            ataque = asignacion;
        }

        // Métodos de modificación
        void perderVida(int dmg){
            vida -= dmg;
        }

        // Métodos para retornar estado
        string mostrarNombre(){
            return nombre;
        }
        int mostrarVida(){
            return vida;
        }
        int mostrarAtaque(){
            return ataque;
        }
        float mostrarPrecision(){
            return precision;
        }
        bool estaVivo(){
            return vida>0;
        }
};

/*****
* statsJefes* leerJefes
******
* Lee los datos de los jefes desde un archivo, almacenándolos en un arreglo dinámico.
******
* Input:
* fstream &file : Referencia al flujo de archivo ya abierto para lectura
* int &cant_jefes : Referencia a variable donde se almacenará la cantidad de jefes leídos
******
* Returns:
* statsJefes* , Puntero a arreglo dinámico con los datos de todos los jefes leídos
*****/
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

/*****
* Jefe* procesarJefes
******
* Convierte un arreglo de structs statsJefes a un arreglo de objetos Jefe,
* asignando los valores correspondientes a cada atributo del jefe.
******
* Input:
* statsJefes* array : Arreglo de structs con datos base de los jefes
* int cantidad : Cantidad de jefes en el arreglo
******
* Returns:
* Jefe* , Puntero a nuevo arreglo de objetos Jefe completamente inicializados
*****/
Jefe* procesarJefes(statsJefes* array, int cantidad){
    Jefe* jefes = new Jefe[cantidad];
    for(int i = 0; i<cantidad ; i++){
        jefes[i].recibirNombre(array[i].nombre);
        jefes[i].recibirVida(array[i].vida);
        jefes[i].recibirAtaque(array[i].ataque);
        jefes[i].recibirPrecision(array[i].precision);
    }
    // borrar array dinamico
    return jefes;
}

// =============================================
// Sección 7: Combate para jefes
// =============================================

/*****
* void mostrarEstadoJefes
******
* Muestra el estado actual del combate contra un jefe específico.
******
* Input:
* Jugador &jugador : Referencia al objeto jugador (para verificar estado y vida)
* Jefe* jefes : Arreglo de jefes en combate
* int posJefe : Índice del jefe actual en el arreglo
******
* Returns:
* void , Imprime el estado del combate
*****/
void mostrarEstadoJefes (Jugador &jugador, Jefe* jefes, int posJefe) {
    cout << "Batman" << " | " << jefes[posJefe].mostrarNombre();

    cout << endl;

    if (jugador.estaVivo()) {
    cout << jugador.mostrarVida();       
    } else {
        cout << "X";
    }

    if (jefes[posJefe].estaVivo()) {
        cout <<  " | " << jefes[posJefe].mostrarVida();
    } else {
        cout << " | " << "X";
    }
    
    cout << endl;
}

/*****
* void turnoJugadorJefes
******
* Gestiona el turno de ataque del jugador contra un jefe en combate:
* 1. Verifica si el ataque es exitoso según la precisión del jugador
* 2. Registra el daño en la cola de turnos si acierta
* 3. Aplica el daño al jefe correspondiente
* 4. Proporciona retroalimentación visual del resultado
******
* Input:
* Jugador &jugador : Referencia al objeto jugador que realiza el ataque
* Jefe* jefes : Arreglo de jefes en combate
* int posJefe : Índice del jefe objetivo en el arreglo
* colaCombate &turnos : Referencia a la cola de gestión de turnos
******
* Returns:
* void , Modifica el estado del jefe en función de las estadisticas del jugador y actualiza la cola de turnos
*****/
void turnoJugadorJefes(Jugador &jugador, Jefe* jefes, int posJefe, colaCombate &turnos) {
    if (aciertaAtaque(jugador.mostrarPrecision())) {
        turnos.agregarCombatiente(jugador.mostrarAtaque());
        jefes[posJefe].perderVida(turnos.frontDmg());

        cout << "Atacas a " << jefes[posJefe].mostrarNombre() << " y le haces " << jugador.mostrarAtaque() << " de daño. "<< endl;

        turnos.pasarTurno();
    } else {
        cout << "Has fallado!" << endl;
    }
}

/*****
* void turnoJefe
******
* Gestiona el turno completo de un jefe en combate:
* 1. Intenta atacar al jugador (puede fallar según su precisión)
* 2. Si acierta, registra el daño en la cola de turnos
* 3. Aplica el daño al jugador si corresponde
* 4. Muestra el estado actualizado del combate después de cada acción
******
* Input:
* Jugador &jugador : Referencia al objeto jugador que recibe el daño
* Jefe* jefes : Arreglo de jefes en combate
* int posJefe : Índice del jefe actual en el arreglo
* colaCombate &turnos : Referencia a la cola que gestiona el orden de turnos
******
* Returns:
* void , Modifica el estado del jugador y actualiza la información del combate
*****/
void turnoJefe(Jugador &jugador, Jefe* jefes, int posJefe, colaCombate &turnos) {
    // Se agrega el jefe a la cola
    if (jefes[posJefe].estaVivo()) {
        if (aciertaAtaque(jefes[posJefe].mostrarPrecision())) {
            turnos.agregarCombatiente(jefes[posJefe].mostrarAtaque());
        } else {
            cout << endl;
            cout << jefes[posJefe].mostrarNombre() << " ha fallado!" << endl;
            cout << endl;
            mostrarEstadoJefes(jugador, jefes, posJefe);
        }
    }
    // Se ejecutan ataques del jefe
    if (jefes[posJefe].estaVivo() && !turnos.isEmpty()) {

        cout << endl;
        
        jugador.cambioVida(-turnos.frontDmg());
        cout << jefes[posJefe].mostrarNombre() << " ataca y te hace " << turnos.frontDmg() << " de daño."<< endl;
        turnos.pasarTurno();

        cout << endl;

        mostrarEstadoJefes(jugador, jefes, posJefe);
    }
}

void combateJefe(Jugador &jugador, Jefe* jefes, int jefesTotales, string nombre){
    int posJefe = 0;
    colaCombate turnos;

    // Determinar posición del jefe en el arreglo
    for(int i=0; i<jefesTotales; i++){
        if(jefes[i].mostrarNombre() == nombre){
            posJefe = i;
            break;
        }
    }

    cout << endl;
    cout << "=== COMBATE ===" << endl;
    cout << endl;

    mostrarEstadoJefes(jugador, jefes, posJefe);

    while(jugador.estaVivo() && jefes[posJefe].estaVivo()){
    
        cout << endl;
    
        turnoJugadorJefes(jugador, jefes, posJefe, turnos);

        cout << endl;

        mostrarEstadoJefes(jugador, jefes, posJefe);

        turnoJefe(jugador, jefes, posJefe, turnos);
    }
    
}

// =============================================
// Sección 8: Mejoras de atributos
// =============================================

struct statsMejoras{
    string* info;
    int vida;
    int ataque;
    float precision;
    float sigilo;
    int recuperacion;
};

/*****
* statsMejoras leerMejoras
******
* Lee y procesa las mejoras de atributos de mejoras desde un archivo.
******
* Input:
* fstream &file : Referencia al archivo ya abierto para lectura
* int &cant_mejoras : Referencia donde se almacena la cantidad de mejoras leídas
******
* Returns:
* statsMejoras , struct con:
* - Campos numéricos para cada atributo mejorable
* - Arreglo dinámico de strings con descripciones completas
*****/
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

/*****
* void panelMejoras
******
* Muestra un menú de mejoras post-combate y aplica la selección del jugador.
* Primero recupera vida base y luego permite elegir una mejora permanente.
******
* Input:
* Jugador &jugador : Referencia al jugador que recibirá las mejoras
* statsMejoras mejoras : Estructura con valores de mejoras disponibles
* int mejorasTotales : Cantidad total de mejoras (no utilizado actualmente)
******
* Returns:
* void , Modifica los atributos del jugador según la elección
*****/
void panelMejoras(Jugador &jugador, statsMejoras mejoras, int mejorasTotales){
    cout << endl;
    cout << "Has sobrevivido el combate!" << endl;
    cout << endl;
    cout << "Recuperas "<< jugador.mostrarRecuperacion() << " de vida tras el combate." << endl;

    jugador.cambioVida(jugador.mostrarRecuperacion());

    cout << endl;
    cout << "Debes decidir:" << endl;
    string eleccion;
    cout << "1. +" << mejoras.vida<<" de vida."<<endl;
    cout<<"2. +"<<mejoras.ataque<<" de ataque."<<endl;
    cout<<"3. +"<<mejoras.precision<<" de precision."<<endl;
    cout<<"4. +"<<mejoras.sigilo<<" de sigilo."<<endl;
    cout<<"5. +"<<mejoras.recuperacion<<" de recuperacion."<<endl;
    while (true) {
        cin>>eleccion;
        if (eleccion == "1"){
            jugador.cambioVida(mejoras.vida);
            cout<<"Vida actual: "<<jugador.mostrarVida()<<endl;
            break;
        } else if (eleccion == "2") {
            jugador.cambioAtaque(mejoras.ataque);
            cout<<"Ataque actual: "<<jugador.mostrarAtaque()<<endl; 
            break;     
        } else if (eleccion == "3") {
            jugador.cambioPresicion(mejoras.precision);
            cout<<"Precision actual: "<<jugador.mostrarPrecision()<<endl;
            break;
        } else if (eleccion == "4") {
            jugador.cambioSigilo(mejoras.sigilo);
            cout<<"Sigilo actual: "<<jugador.mostrarSigilo()<<endl;
            break;
        } else if (eleccion == "5") {
            jugador.cambioRecuperacion(mejoras.recuperacion);
            cout<<"Ahora recuperas "<<jugador.mostrarRecuperacion()<<" puntos de vida despues de un combate."<<endl;
            break;  
        } else {
            cout << "Opción no válida. Ingrese de nuevo." << endl;
        }
    }
}

// =============================================
// Sección 9: Eventos
// =============================================

struct Evento {
    string nombre;
    double probabilidad;
    string informacion;
    int cantidadOpciones;
    string* opciones;
    string* infoOpciones;
    string* consecuencias;
};

/*****
* Evento* leerEventos
******
* Lee y carga eventos aleatorios desde archivo, incluyendo sus opciones y consecuencias.
* Asigna memoria dinámica para los arreglos de opciones y sus propiedades.
******
* Input:
* fstream &file : Archivo ya abierto en modo lectura
* int &cant_eventos : Variable donde se almacenará la cantidad de eventos leídos
******
* Returns:
* Evento* , Arreglo dinámico de eventos completamente inicializados
*****/
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

/*****
* void panelEventos
******
* Selecciona y ejecuta un evento aleatorio basado en probabilidades.
******
* Input:
* Jugador &jugador : Referencia al jugador afectado por el evento
* Jefe* jefes : Arreglo de jefes que pueden ser afectados
* Evento* eventos : Arreglo de eventos posibles
* int cantidadEventos : Total de eventos disponibles
******
* Returns:
* void , Modifica estados del juego según el evento ocurrido
*****/
void panelEventos(Jugador &jugador, Jefe* jefes, Evento* eventos, int cantidadEventos){
    double* pesos = new double[cantidadEventos];
    for(int i = 0; i < cantidadEventos; i++){
        pesos[i] = eventos[i].probabilidad;
    }
    int evento = enteroConProbabilidad(0, cantidadEventos - 1, pesos);
    
    cout << eventos[evento].informacion << endl;
    for(int j=0; j<eventos[evento].cantidadOpciones; j++){
        cout << eventos[evento].opciones[j] << endl;
    }
    
    int opcion;
    string opcionElegida;
    cout << endl;
    cout << "Que deseas hacer?" << endl;
    cin >> opcionElegida;
    cout << endl;
    while (true) {
        if (opcionElegida=="1") {
            opcion=0;
            break;
        } else if (opcionElegida=="2") {
            opcion=1;
            break;
        } else if (opcionElegida=="3") {
            opcion=2;
            break;
        }  else {
            cout << "Opción no válida. Ingrese de nuevo." << endl;
        }
    }
    cout << eventos[evento].infoOpciones[opcion] << endl;
    cout << eventos[evento].consecuencias[opcion] << endl;
    
    // CocaCola helada
    if (evento == 0 && opcion == 0) {
        jugador.matarJugador();
    } 
    
    // Paquete de Lucius Fox
    if (evento == 1 && opcion == 0) {
        jugador.cambioAtaque(10);
    } if (evento == 1 && opcion == 1) {
        jugador.cambioVida(5);
    } if (evento == 1 && opcion == 3) {
        jugador.cambioPresicion(0.3);
    } 
    
    // Destello verde
    if (evento == 2 && opcion == 0) {
        jugador.cambioSigilo(0.2);
    } if (evento == 2 && opcion == 1) {
        jefes[4].recibirAtaque(5);
    }

}

// =============================================
// Sección 10: Habitaciones y Mapa
// =============================================

struct Room {
    string nombre;
    string informacion;
    string mensajeFinal;
    Room* izq;
    Room* centro;
    Room* der;
    string tipoHabitacion;
    int numeroNodo;
    string nombreJefe;
};

class Mapa {
    private:
        Room* inicio;
        Room* actual;
        int numeroHabitaciones;
        Evento* eventos;
        int cantidadEventos;

    public:
        Mapa(){inicio = nullptr; actual = nullptr; numeroHabitaciones = 0; eventos = nullptr;}
        
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
                file.ignore(1000, '\n'); 
                getline(file, nuevo->nombre);
                getline(file, nuevo->informacion);
                if(nuevo->tipoHabitacion == "(COMBATE_FINAL)"){
                    getline(file, nuevo->nombreJefe);
                    getline(file, nuevo->mensajeFinal);
                }
                habitaciones[i] = nuevo;
            }
            //ahora procedemos a leer los enlaces
            getline(file, descartable);
            file>>n;
            file.ignore(1000, '\n');
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

        void recibirEventos(Evento* arrayEventos, int n){
            eventos = arrayEventos;
            cantidadEventos = n;
        }

        void procesarNodo(Jugador &jugador, Enemigo* enemigos, int enemigosTotales, statsMejoras mejoras, int mejorasTotales, Jefe* jefes, int jefesTotales, bool &juegoActivo){
            if(actual->tipoHabitacion == "(COMBATE)"){
                cout << "=== " << actual->nombre << " ===" << endl;
                cout << endl;
                cout << actual->informacion<<endl;

                combate(jugador,enemigos,enemigosTotales);

                if(jugador.estaVivo()){
                    panelMejoras(jugador, mejoras, mejorasTotales);
                }
            }
            
            if(actual->tipoHabitacion == "(COMBATE_FINAL)"){
                cout<<"=== "<<actual->nombre<<" ==="<<endl;
                cout << endl;
                cout<<actual->informacion<<endl;

                combateJefe(jugador,jefes,jefesTotales,actual->nombreJefe);

                if(jugador.estaVivo() && actual->nombre != "Sala de rehabilitacion" && actual->nombre != "Area de cirugia"){ // es decir, que el jugador salio vivo del combate
                    juegoActivo = false;
                }
                if (jugador.estaVivo()) {
                    cout << endl;
                    cout << actual->mensajeFinal << endl;
                    cout << endl;
                }
            }

            if(actual->tipoHabitacion == "(FINAL)"){
                if(actual->nombre == "Oficina del alcaide" && jugador.mostrarCordura()<5){
                    cout << endl;
                    cout << "Una sombra... una aguja... una risa lejana. Parpadeas. Estas de vuelta. Algo no encaja. ¿Siempre estuviste aqui?" << endl;
                    actual = inicio;
                }
                else if(actual->nombre == "Archivo del fundador" && jugador.mostrarCordura()>=5){
                    cout << endl;
                    cout << "Las estanterias se extienden en laberintos imposibles, iluminadas por el resplandor verde de multiples monitores. En el centro, una consola con enigmas sin resolver, esperando la siguiente jugada. No es solo una batalla física; es un duelo de mentes, donde cada movimiento debe ser tan calculado como un acertijo. La mente mas peligrosa de Arkham te desafia a demostrar que puedes seguir su juego... y ganarle." << endl;
                    
                    combateJefe(jugador,jefes,jefesTotales,"El Acertijo");

                    if(jugador.estaVivo()){
                        cout << endl;
                        cout << "Las pantallas que antes destellaban enigmas ahora muestran un brillo estable y sereno. La precision del orden se quiebra, y con ella, la fria prision mental se desmorona. Has descifrado el ultimo secreto, desarmado la trampa perfecta. El silencio inunda la sala, pero no es vacio, es triunfo. El Acertijo queda vencido, su mente finalmente doblegada por la tuya. En este santuario del conocimiento, Gotham encuentra una esperanza renovada. El murcielago ha demostrado que la mente puede ser su arma mas poderosa." << endl;
                        
                        juegoActivo = false;
                    }
                }
                else{
                    cout << "=== "<<actual->nombre << " ===" << endl;
                    cout << endl;
                    cout << actual->informacion << endl;

                    jugador.matarJugador();
                }
            }

            if(actual->tipoHabitacion == "(EVENTO)"){
                cout << "=== "<<actual->nombre << " ===" << endl;
                cout << endl;
                cout << actual->informacion << endl;

                panelEventos(jugador, jefes, eventos, cantidadEventos);
            }

            // Movimiento entre habitaciones
            if (jugador.estaVivo() && (actual->centro != nullptr || actual->der != nullptr)) {
                string eleccionJugador;

                cout << endl;
                cout << "A donde quieres ir?" << endl;

                if (actual->izq != nullptr) {
                    cout << "1. " << actual->izq->nombre << endl;
                } if (actual->centro != nullptr) {
                    cout << "2. " << actual->centro->nombre << endl;         
                } if (actual->der !=nullptr){
                    cout << "3. " << actual->der->nombre << endl;
                }

                while (true) {
                    cin>>eleccionJugador;

                    if (eleccionJugador == "1") {
                        actual = actual->izq;
                        break;
                    } else if (eleccionJugador == "2") {
                        actual = actual->centro;  
                        break;             
                    } else if (eleccionJugador == "3"){
                        actual = actual->der;
                        break;                  
                    }  else {
                        cout << "Opción no válida. Ingrese de nuevo." << endl;
                    }
                }
 
            } 
            else {
                actual = actual->izq;
            }
        }
};

// =============================================
// Sección 11: Main
// =============================================

int main(){
    // Se inician variables
    Jugador Batman;
    int cant_enemigos;
    int cant_jefes;
    int cant_mejoras;
    int cant_eventos;
    Mapa mapa;
    bool juegoActivo;
    
    // Se abre el archivo data.map
    fstream file;
    file.open("data.map", ios::in);
    
    // Se arma el mapa y se crea el arreglo de eventos para crear sus habitaciones respectivas
    mapa.armarMapa(file);
    Evento* eventos = leerEventos(file, cant_eventos);
    mapa.recibirEventos(eventos, cant_eventos);

    // Se leen los datos de enemigos y jefes
    statsEnemigos* arrayStatsEnemigos = leerEnemigos(file, cant_enemigos);  
    statsJefes* arrayStatsJefes = leerJefes(file, cant_jefes);

    // Creación de los arreglos de enemigos y jefes
    Enemigo* enemigos = procesarEnemigos(arrayStatsEnemigos, cant_enemigos);
    Jefe* jefes = procesarJefes(arrayStatsJefes, cant_jefes);

    // Lectura de las mejoras disponibles para el jugador
    statsMejoras mejoras = leerMejoras(file, cant_mejoras);
    
    cout << "Es una noche oscura y lluviosa en el asilo de Arkham. Los mequetrefes han tomado control del recinto y han capturado al comisionado Gordon en el centro médico." << endl;
    cout << "Eres Batman, el mejor detective del mundo, y debes llegar al fondo de todo esto." << endl;
    cout << endl;

    // Ejecutamos el juego
    juegoActivo = true;
    while(Batman.estaVivo()){
        mapa.procesarNodo(Batman, enemigos, cant_enemigos, mejoras, cant_mejoras, jefes, cant_jefes, juegoActivo);
        
        // Muerte de Batman
        if (!Batman.estaVivo()) {
            cout << endl;
            cout << "Has muerto en combate. Gotham ya no tiene salvacion." << endl;
            break;
        }

        // El jugador terminó exitosamente el juego
        if(!juegoActivo){
            cout << endl;
            cout << "Gracias por prestar tus servicios. Gotham te va a recordar." << endl;
            break;
        }
    }

    file.close();
}