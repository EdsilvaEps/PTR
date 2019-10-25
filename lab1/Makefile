#    Universidade Federal do Amazonas - UFAM
#    Disciplina: Programação de Sistemas de Tempo Real - PTR
#    Author: Evaldo Patrik dos Santos Cardoso - 21453640
# 	 Edson Nascimento Silv Neto - 21350694

all: somasub

somasub: adicao.c subtracao.c main.c  #arquivos de  dependencia 
	clear
	rm -rf somasub
	gcc -o somasub adicao.c subtracao.c main.c

exec: somasub
	./somasub 3 5

clean: #remove arquivo somasub
	rm -rf somasub
	clear