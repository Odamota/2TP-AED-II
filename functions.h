//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    ALGORITMOS E ESTRUTURAS DE DADOS
//
//				  [ ARTUR MENDES - Nº 14870 ]
//             [  MÁRIO VALE - mvale@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



/*
typedef struct Mytree{
	int CodCity;
	char cityName[64];
	char cityName[10];


	struct Mytree *left;
	struct Mytree *right;
} CitiesTree;
*/

typedef struct MyList{
	char CodCityOrigem[64];
	char CityNameOrigem[64];
	char CodCityDestino[64];
	char CityNameDestino[64];

	struct MyList* next;
		
} CitiesPT;



CitiesPT * ListadeCidades;

char SearchName[64];


int menuoption;

void setWork();

//Insercao à cabeça na Lista
CitiesPT* insereCidade(CitiesPT* lst, char* codOrigin, char * OriginName , char * codDestiny, char * DestinyName);

void ApresentaCidadesPT(CitiesPT * lst);

void searchCity(CitiesPT* lst);

void searchCitiesAvailable(CitiesPT * lst);

int menu();