#    Universidade Federal do Amazonas - UFAM
#    Disciplina: Programação de Sistemas de Tempo Real - PTR
#    Author: Edson Nascimento Silva Neto - 21350694
#		 Evaldo Patrik dos Santos Cardoso - 21453640
#

all: Simulation

Simulation: matrix.c main2.c simulation.c monitors.c  #arquivos de  dependencia
	clear
	rm -rf Simulation
	rm -rf ${output}
	gcc -o Simulation matrix.c main2.c simulation.c monitors.c -lm -lpthread

exec: Simulation
	./Simulation ${output}

clean:
	rm -rf Simulation
	rm -rf simulation.h.gch
	rm -rf buffer.h.gch
	rm -rf *.ascii
	rm -rf ${output}
	clear
