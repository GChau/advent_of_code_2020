# Get the base Ubuntu image from Docker Hub
FROM ubuntu:focal-20201008

# Update apps on the base image and install Clang compiler
RUN apt-get -y update && apt-get -y install && apt-get -y install clang

# Copy the current folder which contains C++ source code to the Docker image under /usr/src
COPY /src /usr/src/

COPY /input /input

# Specify the working directory
WORKDIR /usr/src/

# Use Clang to compile the main.cpp source file
RUN clang++ -o main main.cpp -std=c++17

# Run the output program from the previous step
CMD ["./main", "/input/input.txt"]