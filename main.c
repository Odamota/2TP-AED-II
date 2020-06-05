//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    ALGORITMOS E ESTRUTURAS DE DADOS
//
//				  [ ARTUR MENDES - Nº 14870 ]
//             [  MÁRIO VALE - mvale@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include "functions.h"


int main() {
	setlocale(LC_ALL, "");
	printf("A importar o ficheiro de dados para execução...\n");

	CitiesPT *ListadeCidades = NULL;
	CitiesTree * WorldCities = NULL;
	ListadeCidades = setWork(ListadeCidades);

	WorldCities = getCitiesWorld(WorldCities);

 	char SearchName[64];
 	char *CodeToSearch;

	printf("\nBem-Vindo ao Interrail Iberia!\n");
	
	while(menuoption != 9)
	{
	   menuoption = menu();

	   switch (menuoption)
	   {
	   case 1:
		   	printf("--------------------------------\n");
		   	printf("Introduza o nome da cidade que procura:\n");
			//scanf("%s", SearchName);
			fflush(stdin);
			fgets(SearchName, sizeof(SearchName), stdin);
	       	
	       	searchCity(ListadeCidades, SearchName);
		   	//percorre lista de cidades e apresenta
		   break;
	   case 2:
		   //pesquisa de cidade por código
		   //searchCitiesAvailable(ListadeCidades, CodeToSearch);
		   break;
	   case 3:
		   //apresenta cidades c/ + de 50k habitantes com destinos com n_Habitantes>30 000
		   //
	   		//searchCitiesAvailable
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
		default:
		exit(1);
		break;
	   }
	}

}