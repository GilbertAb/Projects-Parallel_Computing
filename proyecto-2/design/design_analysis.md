<!--- Copyright 2021 Rostipollos. Universidad de Costa Rica. CC BY 4.0 -->

### Descomposición y Mapeo de la parte distribuida

  La unidad de descomposición escogida fueron los mapas, donde a cada proceso se le da un mapa a la vez y procesará todos sus dias. El tipo de mapeo utilizado es el mapeo dinámico debido a su utilidad en este tipo de descomposición.

### Descomposición y Mapeo de la parte concurrente

  La unidad de decomposición escogida fueron las filas del mapa que se está procesando. Cada hilo toma un conjunto de filas distribuidas de forma cíclica. El mapeo escogido en este caso fue el mapeo cíclico porque todas las filas son del mismo tamaño por lo que no hubo una necesidad de utilizar mapeo dinámico

### Ventajas y desventajas del mapeo y descomposición

  La ventaja de este tipo de descomposición y mapeo para la parte distribuida es que presenta buenos resultados cuando se le dan trabajos de tamaño muy distinto entre sí. La desventaja es que los procesos podrían tener cargas de trabajo muy similares por lo que el aumento en el intercambio de mensajes crearía una demora en comparación con el mapeo estático.

  La ventaja del tipo de descomposición escogido para la parte concurrente es que el procesamiento de cada día en el mapa se volverá más rápido. La desventaja es que hay una creación y destrucción de hilos constante lo cual puede causar un aumento en el tiempo. La ventaja de utilizar el mapeo estático es que se van a distribuir cargas de trabajo muy similares entre sí haciendo que sea benéfica la pequeña cantidad de comunicación existente. No se consideran desventajas para el tipo de mapeo ya que es el mejor para el caso donde las cargas de trabajo son muy similares.

### Manejo de entrada y salida de los procesos

  Dado que los procesos no necesitan comunicarse, no interfieren de ninguna manera en cuanto a salida, ya que cada proceso escribe en archivos que no son accedidos por el resto de procesos. En cuanto a la entrada, todos los procesos leen el mismo archivo, pero ya que es solo una lectura sin modificar de ninguna forma el archivo, no hay problema en que varios procesos lo lean simultáneamente.
 
