B90807 David Atias
B94560 Gilbert Marquez
B80626 Kevin Arguedas

Servidor web concurrente que permite al usuario calcular las sumas de goldbach de los números ingresados.
Para compilar el programa solo se debe ingresar el comando make.
Al ejecutar el programa se puede enviar como primer argumento el puerto y como el segundo la cantidad de consumidores que se quiere, lo cual equivale a la cantidad de conexiones que acepta el programa al mismo tiempo. Si no se brindan argumentos se utiliza el puerto 8080 y se pueden manejar diez conexiónes al mismo tiempo. Luego de ejecutar el programa debe accesar la conexión a traves de un navegador donde se debe accesar a la página localhost:8080 en el caso donde no se envíen argumentos, en otro caso se debe sustituir el 8080 por el puerto ingrasado como primer argumento. Luego de esto se cargará un página que tiene una barra para hacer las consultas de goldbach, para esto se debe ingresar el número o números que desea consultar, si se quiere ingresar varios números estos deben estar separados por un coma luego se presiona el botón calculate y se cargará su consulta y aparecerá un página con el número solicitado y la cantidad de sumas.

Requerimientos:
  El servidor debe poder manejar varias peticiones al mismo tiempo (Debe tratar las conexiones de forma concurrente)
  Se debe responder con las sumas de goldbach que pida el usuario, la petición puede ser de una suma o de varias.
