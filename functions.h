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

//Insercao � cabe�a na Lista
CitiesPT* insereCidade(CitiesPT* lst, char* codOrigin, char * OriginName , char * codDestiny, char * DestinyName);

void ApresentaCidadesPT(CitiesPT * lst);

void searchCity(CitiesPT* lst, char* Search);

void searchCitiesAvailable(CitiesPT * lst, char *CodeToSearch);

int menu();