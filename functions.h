//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    ALGORITMOS E ESTRUTURAS DE DADOS
//
//				  [ ARTUR MENDES - N� 14870 ]
//             [  M�RIO VALE - mvale@ipca.pt  ]
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

//Insercao � cabe�a na Lista
CitiesPT* insereCidade(CitiesPT* lst, int codOrigin, char[64] OriginName, int codDestiny, char[64] DestinyName);

void menu();