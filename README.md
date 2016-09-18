# advanced-cpp-socket

<h1>What is advanced-cpp-socket (ACPPS)</h1>
advanced-cpp-socket also called ACPPS is a improved version of [simple-cpp-socket](https://github.com/Enlumis/simple-cpp-socket) (SCCPS).</br>
ACPPS provide a modular system, allowing services creation and code modularty for a huge server development project</br>
<b>ACPPS does not need any dependance to compile</b>

<h1>How to compile the server</h1>
You must install Makefile to compile the projet
Use the command line to compile the server:
```
make all
```
To recompile the server, use the command line:
```
make re
```
To clean the sources, use the command line:
```
make clean
```
To hard clean the project, use the command line:
```
make fclean
```
<h1>How to compile a service</h1>
When you created your service, you can compile and make it compatible with the server core.
To compile your service use the command line below replacing name by your service directory:
```
make SERVICE=name
```
To clean only the service sources, use the command line:
```
make SERVICE=name sclean
```
