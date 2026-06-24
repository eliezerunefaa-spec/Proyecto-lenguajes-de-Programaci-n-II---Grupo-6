# 1. IMAGEN BASE: Descargamos un entorno Linux que ya incluye el compilador de C/C++
FROM gcc:latest

# 2. DIRECTORIO DE TRABAJO: Creamos una carpeta llamada /app dentro del contenedor
WORKDIR /app

# 3. COPIA DE ARCHIVOS: Copiamos nuestro main.cpp (y todo lo demás) a la carpeta /app del contenedor
COPY . .

# 4. COMPILACIÓN: Compilamos los archivos .cpp y generamos un ejecutable llamado "programa_grupo"
RUN g++ -o programa_grupo *.cpp