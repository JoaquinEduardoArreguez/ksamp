# ksamp
Primer trabajo práctico para Sistemas Operativos 1.

**Se añadieron funcionalidades no requeridas:**

  * ksamp acepta las opciones -s y -l al mismo tiempo, ya que -l incluye a -s, se ejecuta -l.

  * getopt_long no permite dar más de 1 argumento a una opción

  **(por ejemplo -l 2 10 no está admitido)**

  por lo tanto el código está modificado de forma que pueda tomar los parámetros de forma correcta siempre que se los reciba entre "":

  **-l "2 10" funciona correctamente.**

  * No hay interferecia entre los comandos -l y -s siempre que se cumplan las condiciones requeridas por cada comando.
  ** ksamp no acepta entradas incorrectas, y se encarga de recordar la forma correcta de las mismas.**

**Para poder ejecutar el programa, es necesario compilarlo de antemano.
Requisitos e Instrucciones:**
  * tener "make" instalado.
  * Por terminal, dentro de la carpeta donde se encuentra el programa y su código fuente, ejecutar **make**, ésto generará un archivo "ksamp" ejecutable.
  * Para ejecutar "ksamp", por terminal y posicionado en la carpeta donde se encuentra su archivo ejecutable, escribir "./ksamp" con las opciones deseadas.

  
  **Las opciones posibles son:**

  **-s y -l "intervalo tiempo" en orden indistinto**
