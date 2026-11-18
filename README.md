# Aventura ramificada en C++ — Batman: Arkham

Aventura de consola con exploración de habitaciones conectadas por un mapa, eventos aleatorios y combates por turnos. Las decisiones y el estado del personaje pueden llevar a rutas y finales diferentes.

## Requisitos y ejecución

Se requiere un compilador C++11 o posterior.

```bash
g++ -std=c++17 -O2 tarea2.cpp -o aventura
./aventura
```

En Windows, ejecuta `aventura.exe`. El programa carga `data.map` desde el directorio actual; puedes revisar `ejemplo.map` como segundo ejemplo del formato.

## Formato del mapa

El archivo describe habitaciones, conexiones dirigidas (`padre -> hijo`), eventos y sus opciones, enemigos, jefes y mejoras. Los tipos de sala incluyen combate, evento, final y combate final. Si editas el mapa, conserva el orden y los campos del formato que consume el lector.

La implementación incluye mecánicas propias para sigilo, cordura y encuentros con jefes. Es un trabajo universitario colaborativo; autores y créditos originales se indican en el informe `T2.pdf`.
