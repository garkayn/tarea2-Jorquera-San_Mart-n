Matías Jorquera Martínez 202310127-3
Juan Carlos San Martín García 202473047-9

Primero, se explicará el formato del archivo .map, el cual también fue modificado.
En nuestro código, ocupamos lineas como getline(file, descartable);  o file>>descartable; las cuales sirven para cuando leamos lineas como INICIO ARCHIVO o títulos como HABITACIONES, que no sirven para la asignación de variables, pero son importantes para ordenar a la vista el archivo .map.

Se comienza las habitaciones, que cambian el formato según el tipo de habitación que sea:

Para (COMBATE), (EVENTO) y (FINAL):
	-Número del nodo de la habitación
	-Tipo de la habitación
	-Nombre de la habitación
	-Descripción de la habitación / Descripción del final

Para (COMBATE_FINAL):
	-Número del nodo de la habitación
	-Tipo de la habitación
	-Nombre de la habitación
	-Descripción de la habitación
	-Nombre del jefe a enfrentar
	-Descripción de final (solo aparece si se logra derrotar al jefe)

Decidimos agregar combates finales contra los jefes, los cuales cuenten como un final para la historia, y otro tipo de nodo final, que es un final únicamente narrativo que sucede luego de tomar ciertos caminos.

Se continúa con los arcos, que representan que nodo se conecta con cual que tienen el siguiente formato:

padre -> hijo

Luego, los eventos que comienza con:
	-Cantidad total de eventos

Y luego cada evento tiene el formato:
	-Nombre evento
	-Probabilidad (descartable)
	-Probabilidad de aparecer (esta si asigna una variable)
	-Descripción del evento
	-Cantidad de opciones (descartable)
	-Cantidad de opciones del evento
	-Número de la opción
	-Descripción de la opción
	-Consecuencia de la opción

En cuanto a los enemigos, se lee primero la cantidad de enemigos normales, y por cada enemigo se tiene el siguiente formato:
	-Nombre del enemigo
	-Enumera los atributos y sus valores

Para los jefes se mantiene el mismo formato de leer primero la cantidad de jefes y luego:
	-Nombre de cada jefe
	-Enumera los atributos y sus valores

Como los jefes tienen asignada su habitación donde aparecer, no es necesario crear el campo de probabilidad.

Finalmente, vienen las mejoras de combate, se lee el número total de mejoras, y tiene el siguiente formato:
	-Atributo a mejorar cantidad en cuanto mejora



Como primera consideración, hay ciertos nodos que les dimos un proceso especial, por ejemplo, en las habitaciones que aparece el jefe de Harley Quinn, a pesar de ser un nodo de combate final, no termina el juego, porque luego viene otro combate de jefe.

Como segunda consideración, ocurre algo parecido en ambos finales del acertijo, donde en uno si ocurre un combate si llegas con cierta cantidad de cordura a dicho nodo.
Esto lo quisimos agregar para aumentar la narrativa y que cada camino tenga su distinción, aun que estos nodos no cumplen la condición de robusto a los cambios, no ocurre nada si se modifica el .map (siempre y cuando se respete el formato), ya que el que está preparado para ese nodo en especial es el mismo código.

Como tercera consideración, no se menciona que el jugador deba venir en el .map, por lo que también decidimos agregarle stats como sigilo y cordura, el primero funciona como un multiplicador al entrar con sigilo, y el segundo, desbloquea ciertos finales al llegar a nodos del nombre (FINAL), y cambia según entres con, o sin sigilo a una habitación.

Explicación mecánica especial:

Si llegas a cierto nodo final, con cierto nivel de cordura (no spoiler), ocurre un suceso donde Batman es envenenado por el espantapájaros, y este entra en un sueño, donde cree que ocurre todo de nuevo, en realidad, el pobre murciélago solo se devolvió al nodo raíz, que no es el padre inmediato de donde está colocado ese final.





