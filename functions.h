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
	char * CodCity;
	float population;
	char * cityName;
	struct Mytree *left;
	struct Mytree *right;
} CitiesTree;


typedef struct MyList{
	char *CodCityOrigem;
	char *CityNameOrigem;
	char *CodCityDestino;
	char *CityNameDestino;
	struct MyList* next;
} CitiesPT;


int menuoption;

//Funcao que acede ao ficheiro e guarda na lista
CitiesPT* setWork(CitiesPT * ListadeCidades);
CitiesTree * getCitiesWorld(CitiesTree * WorldCities);

//Insercao à cabeça na Lista
CitiesPT* insereCidade(CitiesPT* lst, char* codOrigin, char * OriginName , char * codDestiny, char * DestinyName);

void ApresentaCidadesPT(CitiesPT * lst);

void searchCity(CitiesPT* lst, char* Search);

void searchCitiesAvailable(CitiesPT * lst, char *CodeToSearch);

int menu();