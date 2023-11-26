# Usar una imagen base de Ubuntu
FROM ubuntu:latest

# Actualizar el sistema e instalar g++
RUN apt-get update && \
    apt-get install -y g++ 

# Establecer el directorio de trabajo
WORKDIR /app

# Copiar los archivos del programa al contenedor
COPY . /app

# Compilar el programa
RUN g++ -std=c++11 -o disk_scheduling diskV2.cpp

# Comando por defecto para ejecutar el programa
CMD ["./disk_scheduling"]
