//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    ALGORITMOS E ESTRUTURAS DE DADOS
//
//				  [ ARTUR MENDES - N� 14870 ]
//             [  M�RIO VALE - mvale@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Desabilita warnings de fun��es n�o seguras (fopen, sscanf, etc...)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include "functions.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUN��ES: ACEDER E GUARDAR DADOS DAS CIDADES NO PROGRAMA
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// MENU PRINCIPAL
int menu() {
	
	printf("Escolha uma das seguintes op��es:\n");
	printf("1 - Pesquisar Cidade \n ");
	printf("2 - Pesquisar Viagens por C�digo de Cidade \n");
	printf("3 - Cidades c/ + de 50k de Habitantes p/ cidades c/ menos de 30k habitantes\n ");
	printf("4 - Cidade com mais destinos\n ");
	printf("5 - Pesquisar viagens entre duas cidades\n ");
	printf("6 - Listar CidadesPT\n ");
    
	printf("7 - Listar Cidades Mundo\n ");
	scanf("%d", &menuoption);
	
	return menuoption;



}


//Listagem de Cidades 

void ApresentaCidadesPT(CitiesPT* lst)
{
	CitiesPT * aux;
	printf("------------------------------\n");
	printf("CIDADES EM PORTUGAL:\n");
	printf("C�digo Cidade Origem\tNome\tC�digo Cidade Destino\tNome\n");


	for (aux = lst; aux; aux= aux->next)
	{
		printf("%s\t%s\t%s\t%s\n", aux->CodCityOrigem, aux->CityNameOrigem, aux->CodCityDestino, aux->CityNameDestino);
	}

	printf("-----------------------------------------");
}



/*
void searchCitiesAvailable(CitiesPT* lst)
{
	printf("--------------------------------\n");
	printf("Introduza o c�digo da cidade que procura:\n");

	int searchCode;
	scanf("%d", searchCode);

	printf("A procurar por %d...", searchCode);

	CitiesPT* aux;

	for (aux = lst; aux; aux = aux->next);
	{
		if (searchCode == aux->CodCityOrigem)
		{
			printf("Dispon�vel viagem de %d para %s, c�digo %d\n", aux->CityNameOrigem, aux->CityNameDestino, aux->CodCityDestino);
			
		}
	}

	printf("Terminou a procura de viagens da cidade com o c�digo %d\n", searchCode);
}
*/


/*
void searchCity(CitiesPT* lst)
{
	printf("--------------------------------\n");
	printf("Introduza o nome da cidade que procura:\n");
	
	scanf("%s", SearchName);

	printf("Procurando %s na Lista de Cidades Portuguesas...\n", SearchName);

	CitiesPT* aux; 

	int counter = 0;
	for (aux = lst; aux; aux = aux->next)
	{
		if (strcmp(SearchName, aux->CityNameOrigem) = 0)
		{
			counter++;
			printf("\nEncontrada Cidade: \n");
			printf("%d\t%s\t%d\t%s\n", aux->CodCityOrigem, aux->CityNameOrigem, aux->CodCityDestino, aux->CityNameDestino);
		}
	}

	printf("Terminou a procura por %s, com %d resultados", SearchName, counter);
	

}

*/



CitiesPT* insereCidade(CitiesPT* lst, int codOrigin, char * OriginName, char * codDestiny, char * DestinyName)
{
	CitiesPT* aux = (CitiesPT*)malloc(sizeof(CitiesPT));
	strcpy( aux->CodCityOrigem, codOrigin );
	strcpy( aux->CityNameDestino ,DestinyName );
	strcpy(aux->CityNameOrigem = OriginName);
	
	//aux->CodCityOrigem = codOrigin;
	aux->CityNameOrigem = OriginName;
//	aux->CityNameDestino = DestinyName;
	//aux->CityNameOrigem = OriginName;
	aux->CodCityDestino = codDestiny;
	//aux->CityNameDestino = DestinyName;
	aux->next = lst;

	//retorna lista j� com a nova cidade;
	return aux;


}



//Acede ao ficheiro CidadesPT
//Guarda as cidades numa Lista Ligada
void setWork() 
{
	FILE * fh = fopen("cidadesPT.txt","r");

	if(!fh){
		printf("Ocorreu um erro ao abrir o ficheiro .\n" );
	}
	else
	{
		int linecounter = 0;
		CitiesPT *CitiesList = NULL; 
		while(!feof(fh))
		{
			char CityInfo[128];
			char *aux;
			
			fgets(CityInfo, 128, fh);
			linecounter++;
			char *token;
			char * CityCode;
			char * cityname;
			char *  destinyCode;
			char * destinyname;
			aux = CityInfo;

			int contavirgulas = 0;

			if(!feof(fh))
			{
			//le linha
				token =strtok(aux, ",");
				while(token != NULL)
				{
					contavirgulas++;
					switch(contavirgulas)
					{
						case 1:
							strcpy(CityCode, token);
							break;
						case 2:
							strcpy(cityname, token);
							break;
						case 3:
							 
							strcpy(destinyCode, token);
							break;
						case 4:
							strcpy(destinyname, token);
					}
					token = strtok(NULL, "");
				}
			}
			if (linecounter > 1)
			{
				//adiciona dados da � lista ligada citiesPT
				ListadeCidades = insereCidade( ListadeCidades, CityCode, cityname, destinyCode, destinyname );
				
			}
		} 

	}


}

