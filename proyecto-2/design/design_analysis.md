<!--- Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0 -->

### Descomposición y Mapeo de la parte distribuida

  La unidad de descomposición escogida fueron los mapas, donde a cada proceso se le da un mapa a la vez y procesará todos sus dias. El tipo de mapeo utilizado es el mapeo dinámico debido a que las cargas de trabajo pueden ser muy heterogéneas, para las cuales un mapeo estático no sería eficiente.

### Descomposición y Mapeo de la parte concurrente

  La unidad de decomposición escogida fueron las filas del mapa que se está procesando. Cada hilo toma un bloque de las filas del mapa. El mapeo escogido en este caso fue el mapeo por bloque porque todas las filas son del mismo tamaño por lo que hace que las cargas de trabajo sean muy similares, por no decir iguales, haciendo innecesario o incluso ineficiente un mapeo dinámico.

### Ventajas y desventajas del mapeo y descomposición

  La ventaja de este tipo de descomposición y mapeo para la parte distribuida es que presenta buenos resultados para la gran mayoría de entradas, las cuales pueden ser muy heterogéneas. La desventaja es que los procesos podrían en algunos casos tener cargas de trabajo muy similares por lo que el aumento en el intercambio de mensajes crearía una demora en comparación con el mapeo estático, en especial, cuando el trabajo a realizar es muy poco, haciendo que sea más relevante el tiempo perdido en mensajes de red.

  La ventaja del tipo de descomposición escogido para la parte concurrente es que el procesamiento de cada día en el mapa es más rápido, por lo que, a diferencia de descomponer por números, el rendimiento no se ve afectado si los mapas son muy heterogéneos o si la entrada es muy pequeña. La desventaja es que dependiendo de la cantidad de filas que tenga la matriz, podrían quedarse hilos inactivos por mucho tiempo, reduciendo el tiempo concurrente. Para una matriz de 1 fila sería totalmente serial.

### Separación de asuntos

  Se tienen dos tipos de procesos: 

  El proceso 0 se encarga de llevar un control de cuantos mapas se han procesado y asigna a cada proceso trabajador el indice del mapa con el que va a trabajar. Los demás procesos se reportan a él en busca de más trabajo y en caso de haber terminado con todo el trabajo que había que realizar, este proceso se encargará de avisar al resto de procesos que ya se realizó todo el trabajo, finalizando su ejecución.

  El resto de procesos son procesos trabajadores, que se reportan al proceso 0 en busca de trabajo y procesan el mapa que les fue asignado. Una vez terminado avisan al proceso 0 y obtendrán más trabajo o un aviso de que no hay más trabajo por hacer, tras el cual terminan su ejecución.

### Manejo de entrada y salida de los procesos

  Los procesos no interfieren de ninguna manera en cuanto a salida, ya que cada proceso escribe en archivos que no son accedidos por el resto de procesos. En cuanto a la entrada, todos los procesos leen el mismo archivo, pero ya que es solo una lectura sin modificar de ninguna forma el archivo, no hay problema en que varios procesos lo lean simultáneamente.
 
