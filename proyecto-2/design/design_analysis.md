<!--- Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0 -->

### Descomposición y Mapeo de la parte distribuida

  La unidad de descomposición escogida fueron los mapas, donde a cada proceso se le da un conjunto de mapas de los cuales procesará todos sus dias. El tipo de mapeo utilizado es el mapeo cíclico.

### Descomposición y Mapeo de la parte concurrente

  La unidad de decomposición escogida fueron nuevamente los mapas. Cada hilo toma un mapa del conjunto de mapas del proceso a la vez. El mapeo escogido en este caso fue el mapeo dinámico, ya que tuvo mejor rendimiento en las mediciones realizadas anteriormente y es más acertado para este tipo de descomposición porque distribuye mejor el trabajo.

### Ventajas y desventajas del mapeo y descomposición

  La ventaja de este tipo de descomposición y mapeo para la parte distribuida es que hace que los procesos no necesiten comunicación, lo que evita overheads de paso de mensajes. La desventaja es que los procesos podrían tener cargas de trabajo muy diferentes, dependiendo de la entrada.

  La ventaja del tipo de descomposición escogido para la parte concurrente es que se adaptaba mejor a nuestro diseño serial que realizamos originalmente, ya que nuestro programa lee todos los mapas por adelantado antes de procesarlos, en lugar de trabajar mapa por mapa. La desventaja es que las cargas de trabajo pueden llegar a ser muy diferentes, dependiendo de la entrada, aumentando potencialmente el tiempo serial o de poca concurrencia. La ventaja de utilizar el mapeo dinámico es contrarrestar un poco el problema de la cargas de trabajo diferentes. La desventaja es que si la entrada forma cargas de trabajo iguales o muy similares para cada hilo, no vale la pena el tiempo adicional en tiempo de ejecución que implica realizar un mapeo dinámico.

### Manejo de entrada y salida de los procesos

  Dado que los procesos no necesitan comunicarse, no interfieren de ninguna manera en cuanto a salida, ya que cada proceso escribe en archivos que no son accedidos por el resto de procesos. En cuanto a la entrada, todos los procesos leen el mismo archivo, pero ya que es solo una lectura sin modificar de ninguna forma el archivo, no hay problema en que varios procesos lo lean simultáneamente.
 