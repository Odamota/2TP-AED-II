//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    ALGORITMOS E ESTRUTURAS DE DADOS
//
//				  [ ARTUR MENDES - N� 14870 ]
//             [  M�RIO VALE - mvale@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Desabilita (no MSVC++) warnings de fun��es n�o seguras (fopen, sscanf, etc...)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include "functions.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUN��ES: ACEDER E GUARDAR DADOS DAS CIDADES NO PROGRAMA
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            MENU PRINCIPAL
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void menu() {
	printf("Bem-Vindo ao Interrail Iberia!\n");
	printf("Escolha uma das seguintes op��es:\n");
	printf("1 - Pesquisar Cidade \n ");

	scanf("%s", searchCity);

	//percorre lista de cidades e apresenta

}

//Inser��o � cabe�a na Lista
CitiesPT* insereCidade(CitiesPT* lst, int codOrigin, char[64] OriginName, int codDestiny, char[64] DestinyName);
{
	CitiesPT* aux = (CitiesPT*)malloc(sizeof(CitiesPT));
	aux->CodCityOrigem = codOrigin;
	aux->CityNameOrigem = OriginName;
	aux->CodCityDestino = codDestiny;
	aux->CityNameDestino = DestinyName;
	aux->next = lst;

	//retorna lista j� com a nova cidade;
	return aux;


}

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

			}
		} 

	}


}