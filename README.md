# so_project_2

Tarea 2 de la asignatura Sistemas Operativos 2024-2.

Desarrollo multihebra, mecanismos de sincronización y funcionamiento de memoria virtual.

## Parte 1: Simulador de cola circular. Monitor como solución a problema de Productores y Consumidores.

Para compilar el simulador de cola circular, usar el siguiente comando:

```
gcc monitor.c -o monitor
```

Para ejecutar el código, se debe ejecutar una línea de comando con la siguiente estructura:

```
./monitor -p <productores> -c <consumidores> -s <tamaño_inicial_cola> -t <tiempo>
```

El código no tiene validación de parámetros de entrada, por lo que para su correcto funcionamiento se debe respetar la estructura mencionada.

Un ejemplo para entrada es el siguiente:

```
./monitor -p 10 -c 5 -s 50 -t 1
```

## Parte 2: Simulador de memoria virtual con paginación y algoritmos de reemplazos de páginas.

Para compilar el simulador de memoria virtual, usar el siguiente comando:

```
gcc mvirtual.c -o mvirtual
```

Para ejecutar el código, se debe ejecutar una línea de comando con la siguiente estructura:

```
./mvirtual -m <marcos_iniciales> -a <algoritmo_reemplazo> -f <file_referencias>
```

El código no tiene validación de parámetros de entrada, por lo que para su correcto funcionamiento se debe respetar la estructura mencionada.

Un ejemplo para entrada es el siguiente:

```
./mvirtual -m 3 -a FIFO -f referencias.txt
```

