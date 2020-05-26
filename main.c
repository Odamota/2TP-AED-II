#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "functions.h"


int main() {
	printf("A importar o ficheiro de dados para execução...\n");

	setWork();
	printf("Bem-Vindo ao Interrail Iberia!\n");
	
	while(menuoption != 9)
	{
	   menuoption = menu();

	   switch (menuoption)
	   {
	   case 1:
		   //searchCity(ListadeCidades);
		   //percorre lista de cidades e apresenta
		   break;
	   case 2:
		   //pesquisa de cidade por código
		   //searchCitiesAvailable(ListadeCidades);
		   break;
	   case 3:
		   //apresenta cidades c/ + de 50k habitantes com destinos com n_Habitantes>30 000
		   //
		   //
		   break;
	   case 4:
		   //Calcular cidade c/ maior numero de destinos
		   //caso exista mais que uma, listar todas
		   break;
	   case 5: 
		// recebe dois identificadores de cidades
		   // origem + Destino
		   //procura um item que tenha esses dois códigos e apresenta-os
		   //(Tentar aceder ao caminho mais curto)
		   break;
	   case 6:
		   ApresentaCidadesPT(ListadeCidades);
		   menu();
		   break;
	   case 7:
		   //Lista de cidades da Iberia
		   break;

	   }
	}

}