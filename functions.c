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
#define MALLOC(t) (t*)malloc(sizeof(t))

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <locale.h>
#include <malloc.h>
#include "functions.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUNÇÕES: ACEDER E GUARDAR DADOS DAS CIDADES NO PROGRAMA
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// MENU PRINCIPAL
int menu() {
	setlocale(LC_ALL, "");
	printf("Escolha uma das seguintes opcoes:\n");
	printf("1 - Pesquisar Cidade \n ");
	printf("2 - Pesquisar Viagens por Código de Cidade \n");
	printf("3 - Cidades c/ + de 50k de Habitantes p/ cidades c/ menos de 30k habitantes\n ");
	printf("4 - Cidade com mais destinos\n ");
	printf("5 - Pesquisar viagens entre duas cidades\n ");
	printf("6 - Listar CidadesPT\n ");
    
	printf("7 - Listar Cidades Mundo\n ");
	printf("Opção: ");

	scanf("%d", &menuoption);
	
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
		printf("%s\t%s\t%s\t%s\n", aux->CodCityOrigem, aux->CityNameOrigem, aux->CodCityDestino, aux->CityNameDestino);
	}

	printf("-----------------------------------------");
}



/*\
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
			printf("Disponível viagem de %d para %s, código %d\n", aux->CityNameOrigem, aux->CityNameDestino, aux->CodCityDestino);
			
		}
	}

	printf("Terminou a procura de viagens da cidade com o código %d\n", searchCode);
}
*/



void searchCity(CitiesPT* lst, char * Search)
{
	printf("Procurando %s na Lista de Cidades Portuguesas...\n", Search);

	CitiesPT* aux; 
	int counter = 0;
	for (aux = lst; aux; aux = aux->next)
	{
		if (strcmp(Search, aux->CityNameOrigem) == 0)
		{
			counter++;
			printf("\nEncontrada Cidade: \n");
			printf("%s\t%s\t%s\t%s\n", aux->CodCityOrigem, aux->CityNameOrigem, aux->CodCityDestino, aux->CityNameDestino);
		}
	}

	printf("Terminou a procura por %s, com %d resultados", Search, counter);
}



CitiesPT* insereCidade(CitiesPT* lst, char * codOrigin, char * OriginName, char * codDestiny, char * DestinyName)
{

	printf("\nInserindo %s !!!!\n", OriginName );

	CitiesPT* aux = (CitiesPT*)malloc(sizeof(CitiesPT));
	aux->CodCityOrigem = strdup(codOrigin);
	aux->CityNameDestino = strdup(DestinyName);
	aux->CityNameOrigem = strdup(OriginName);
	aux->CodCityDestino = strdup(codDestiny);
	aux->next = lst;
	//retorna lista já com a nova cidade;
	return aux;
}



//Acede ao ficheiro CidadesPT
//Guarda as cidades numa Lista Ligada
CitiesPT * setWork(CitiesPT * ListadeCidades) 
{

	printf("\nLeitura do Ficheiro de Cidades PT\n");
	FILE * fh = fopen("cidadesPT.txt","r");

	if(!fh){
		printf("Ocorreu um erro ao abrir o ficheiro .\n" );
	}
	else
	{
		printf("Lendo o ficheiro...\n");
		int linecounter = 0;
		while(!feof(fh))
		{
			char CityInfo[100];
			
			fgets(CityInfo, 100, fh);

			//printf("%s\n", CityInfo);
			linecounter++;
			
			char *CityCode, *CityName, *DestinyCode, *DestinyName, *aux, *token;
			aux = CityInfo;
			int contavirgulas = 0;
			if(!feof(fh))
			{
			//le linha
				token =strtok(aux, ",");
				while(token != NULL && contavirgulas <=4)
				{
					//printf("%s", token);
					contavirgulas++;
					//printf("\nEncontrou a virgula nr %d", contavirgulas);
					switch(contavirgulas)
					{
						case 1:
							//printf("\nTOKEN: %s\n", token);
							CityCode = strdup(token);
							//printf("\nGuarda o codigo:%s \n", CityCode);
							break;
						case 2:
					//	printf("\nTOKEN: %s\n", token);
						//	printf("\nPalavra: %s",token);
							CityName = strdup(token);
							break;
						case 3:
						//	printf("\nTOKEN: %s\n", token);
							DestinyCode = strdup(token);
							break;
						case 4:
						//	printf("\nTOKEN: %s\n", token);
							DestinyName = strdup(token);
							break;
						default:
							break;
					}
					token = strtok(NULL, ",");
				}
				
				//printf("Insere a cidade!: %s\n",CityName );
				//printf("Código de Origem: %s\n", CityCode);
				//printf("Código de Destino: %s\n", DestinyCode);
				//printf("Nome de Destino: %s\n", DestinyName);
				
			}
			ListadeCidades = insereCidade( ListadeCidades ,CityCode, CityName, DestinyCode, DestinyName );
			
		} 

return ListadeCidades;
	}


}


/* Acede ao ficheiro de WorldCities.csv */
CitiesTree * getCitiesWorld(CitiesTree * WorldCities)
{
printf("\nLeitura do Ficheiro de Cidades no Mundo:\n");
	FILE * fh = fopen("worldcities.csv","r");

	if(!fh){
		printf("Ocorreu um erro ao abrir o ficheiro .\n" );
	}
	else
	{
		printf("Lendo o ficheiro...\n");
		int linecounter = 0;
		while(!feof(fh))
		{
			char Linha[128];
			
			fgets(Linha, 128, fh);

			//printf("%s\n", CityInfo);
			linecounter++;
			
			char *CodCidade, *NomeCidade, *aux, *token;
			float pop;
			aux = Linha;
			int contavirgulas = 0;
			if(!feof(fh))
			{
			//le linha
				token =strtok(aux, ",");
				while(token != NULL && contavirgulas <=4)
				{
					printf("Encontrou %s\n", token);
					contavirgulas++;
					//printf("\nEncontrou a virgula nr %d", contavirgulas);
					switch(contavirgulas)
					{
						case 2:
							//CityCode_ASCII
							//CodCidade = strdup(token);
							//printf("\nGuarda o codigo:%s \n", CityCode);
							break;
						case 10:
					//	printf("\nTOKEN: %s\n", token);
						//	printf("\nPalavra: %s",token);
							//NomeCidade = strdup(token);
							break;
						case 11:
						//	printf("\nTOKEN: %s\n", token);
							//pop = atof(token);
							//DestinyCode = strdup(token);
							break;
						case 4:
						//	printf("\nTOKEN: %s\n", token);
							//DestinyName = strdup(token);
							break;
						default:
							break;
					}
					token = strtok(NULL, ",");
				}
				
				//printf("Insere a cidade!: %s\n",CityName );
				//printf("Código de Origem: %s\n", CityCode);
				//printf("Código de Destino: %s\n", DestinyCode);
				//printf("Nome de Destino: %s\n", DestinyName);
				
			}
			
			//WorldCities = insereCidade( ListadeCidades ,CityCode, CityName, DestinyCode, DestinyName );
			
		} 

return WorldCities;
	}
}