//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    ALGORITMOS E ESTRUTURAS DE DADOS
//
//				  [ ARTUR MENDES - Nº 14870 ]
//             [  MÁRIO VALE - mvale@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


typedef struct Mytree{
	int CodCity;
	char[64] cityName;
	char[10] cityName;


	struct Mytree *left;
	struct Mytree *right;
} CitiesIberia;

typedef struct MyList{
	int CodCityOrigem;
	char[64] CityNameOrigem;
	int CodCityDestino;
	char[10] CityNameDestino;

	struct MyList* next;	
} CitiesPT;

CitiesPT ListadeCidades = NULL;

char[64] searchCity;

void setWork() {};

//Insercao à cabeça na Lista
CitiesPT* insereCidade(CitiesPT* lst, int codOrigin, char[64] OriginName, int codDestiny, char[64] DestinyName);

void menu();