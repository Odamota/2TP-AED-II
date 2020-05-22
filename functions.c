//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    ALGORITMOS E ESTRUTURAS DE DADOS
//
//				  [ ARTUR MENDES - Nº 14870 ]
//             [  MÁRIO VALE - mvale@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Desabilita warnings de funções não seguras (fopen, sscanf, etc...)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include "functions.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUNÇÕES: ACEDER E GUARDAR DADOS DAS CIDADES NO PROGRAMA
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// MENU PRINCIPAL
int menu() {
	
	printf("Escolha uma das seguintes opções:\n");
	printf("1 - Pesquisar Cidade \n ");
	printf("2 - Pesquisar Viagens por Código de Cidade \n");
	printf("3 - Cidades c/ + de 50k de Habitantes p/ cidades c/ menos de 30k habitantes\n ");
	printf("4 - Cidade com mais destinos\n ");
	printf("5 - Pesquisar viagens entre duas cidades\n ");
	printf("6 - Listar CidadesPT\n ");
    
	printf("7 - Listar Cidades Mundo\n ");
	scanf("%s", menuoption);
	
	return menuoption;



}


//Listagem de Cidades 
void ApresentaCidadesPT(CitiesPT* lst)
{
	CitiesPT * aux;
	printf("------------------------------\n");
	printf("CIDADES EM PORTUGAL:\n");
	printf("Código Cidade Origem\tNome\tCódigo Cidade Destino\tNome\n");


	for (aux = lst; aux; aux= aux->next)
	{
		printf("%d\t%c\t%d\t%c\n", aux->CodCityOrigem, aux->CityNameOrigem, aux->CodCityDestino, aux->CityNameDestino);
	}

	printf("-----------------------------------------")
}


void searchCitiesAvailable(CitiesPT* lst)
{
	printf("--------------------------------\n");
	printf("Introduza o código da cidade que procura:\n");

	int searchCode;
	scanf("%d", searchCode);

	printf("A procurar por %d...", searchCode);

	CitiesPT* aux;

	for (aux = lst; aux; aux = aux->next);
	{
		if (searchCode == aux->CodCityOrigem)
		{
			printf("Disponível viagem de %d para %c, código %d\n", aux->CityNameOrigem, aux->CityNameDestino, aux->CodCityDestino);
			
		}
	}

	printf("Terminou a procura de viagens da cidade com o código %d\n", searchCode);
}

void searchCity(CitiesPT* lst)
{
	printf("--------------------------------\n");
	printf("Introduza o nome da cidade que procura:\n");
	
	scanf("%s", SearchName);

	printf("Procurando %s na Lista de Cidades Portuguesas...\n");

	CitiesPT* aux; 

	int counter = 0;
	for (aux = lst; aux; aux = aux->next)
	{
		if (strcmp(SearchName, aux->CityNameOrigem) = 0)
		{
			counter++;
			printf("\nEncontrada Cidade: \n");
			printf("%d\t%c\t%d\t%c\n", aux->CodCityOrigem, aux->CityNameOrigem, aux->CodCityDestino, aux->CityNameDestino);
		}
	}

	printf("Terminou a procura por %c, com %d resultados", SearchName, counter);
	

}


CitiesPT* insereCidade(CitiesPT* lst, int codOrigin, char[64] OriginName, int codDestiny, char[64] DestinyName)
{
	CitiesPT* aux = (CitiesPT*)malloc(sizeof(CitiesPT));
	aux->CodCityOrigem = codOrigin;
	aux->CityNameOrigem = OriginName;
	aux->CodCityDestino = codDestiny;
	aux->CityNameDestino = DestinyName;
	aux->next = lst;

	//retorna lista já com a nova cidade;
	return aux;


}


//Acede ao ficheiro CidadesPT
//Guarda as cidades numa Lista Ligada
void setWork() 
{
	File* fh = fopen("cidadesPT.txt","r");

	if(!fh){
		printf("Ocorreu um erro ao abrir o ficheiro.\n", );
	}
	else
	{
		int linecounter = 0;
		citiesPT *CitiesList = NULL; 
		while(!feof(fh))
		{
			char CityInfo[128];
			char *aux;
			
			fgets(CityInfo, 128, fh);
			linecounter++;
			char *token;
			int  *CityCode = 0;
			char* cityname;
			int* destinyCode;
			char* destinyname;
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
						//
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
				//adiciona dados da à lista ligada citiesPT
				ListadeCidades = insereCidade( ListadeCidades, CityCode, cityname, destinyCode, destinyname );
				
			}
		} 

	}


}

