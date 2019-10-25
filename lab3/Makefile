#    Universidade Federal do Amazonas - UFAM
#    Disciplina: Programação de Sistemas de Tempo Real - PTR
#    Author: Edson Nascimento Silva Neto - 21350694
#		 Evaldo Patrik dos Santos Cardoso - 21453640
#

all: Simulation

Simulation: matrix.c main.c simulation.c  #arquivos de  dependencia
	clear
	rm -rf Simulation
	gcc -o Simulation matrix.c main.c simulation.c -lm

exec: Simulation
	./Simulation

clean: #remove arquivo somasub
	rm -rf Lab2
	clear
