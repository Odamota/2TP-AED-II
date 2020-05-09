//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    VISÃO POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#define VC_DEBUG
#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UM BLOB (OBJECTO)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct {
	int x, y, width, height;	// Caixa Delimitadora (Bounding Box)
	int area;					// Área
	int xc, yc;					// Centro-de-massa
	int perimeter;				// Perímetro
	int label;					// Etiqueta
} OVC;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


typedef struct {
	unsigned char *data;
	int width, height;
	int channels;			// Binário/Cinzentos=1; RGB=3  Cores base
	int levels;				// Binário=1; Cinzentos [1,255]; RGB [1,255] Força da cor
	int bytesperline;		// width * channels ???
} IVC;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                    PROTÓTIPOS DE FUNÇÕES
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// FUNÇÕES: ALOCAR E LIBERTAR UMA IMAGEM
IVC *vc_image_new(int width, int height, int channels, int levels);
IVC *vc_image_free(IVC *image);

// FUNÇÕES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
IVC *vc_read_image(char *filename);
int vc_write_image(char *filename, IVC *image);
int vc_open_image(char *filename);
// FUNÇÕES: MUDAR A NEGATIVO AS CORES DE UMA IMAGEM
// REMAKE
int vc_gray_negative(IVC *srcdst);
int vc_rgb_negative(IVC *srcdst);

int vc_rgb_to_gray(IVC *src, IVC *dst);
int vc_rgb_to_hsv(IVC* src, IVC *dst);

int vc_binary_to_gray(IVC *src, IVC *dst);
int vc_gray_to_rgb(IVC *src, IVC *dst);
int vc_gray_binary(IVC *src, IVC *dst, unsigned char threshold);
int vc_gray_binary_global_mean(IVC *src, IVC *dst);
int vc_gray_to_binary_midpoint(IVC *src, IVC *dst, int kernel);

int vc_binary_dilate(IVC *src, IVC *dst, int kernel);
int vc_binary_erode(IVC *src, IVC *dst, int kernel);

OVC* vc_binary_blob_labelling(IVC *src, IVC *dst, int *nlabels);
int vc_binary_blob_info(IVC *src, OVC *blobs, int nblobs);
int vc_draw_boundingbox(IVC *src, int x, int y, int width, int height, int cor);
int vc_draw_centerofgravity(IVC *src, int xc, int yc);

int vc_gray_histogram_show(IVC *src, IVC *dst);
int vc_gray_histogram_equalization(IVC *src, IVC *dst);

int vc_gray_edge_prewitt(IVC *src, IVC *dst, float th);
int vc_gray_edge_sobel(IVC *src, IVC *dst, float th);

int vc_gray_lowpass_mean_filter(IVC *src, IVC *dst, int kernelsize);
int vc_gray_lowpass_median_filter(IVC *src, IVC *dst, int kernelsize);
int vc_gray_lowpass_gaussian_filter(IVC *src, IVC *dst);


int vc_fill_edges(IVC *srcdst);
int vc_edges_classic(IVC *src, IVC *dst, int bright);
int vc_edges_der1(IVC *src, IVC *dst, int bright);
int vc_edges_der2(IVC *src, IVC *dst, int bright);
int vc_graph_edges(IVC *src, IVC *col, IVC *row);
int vc_segmentate_rect_over_mean(IVC *src, IVC *dst, IVC *col, IVC *row);
int vc_clean_points(IVC *src, IVC * dst);
int vc_count_points_column(IVC* src, int *qtPts);
OVC vc_blob_plate_selection(IVC *dst, OVC *blobs, int nlabels);
int vc_obj_to_image(OVC blob, IVC *src, IVC *dst);
int vc_blob_to_erase_selection(IVC *dst, OVC *blobs, int nlabels);
int vc_erase_blob(IVC *srcdst, OVC blob);
int vc_blob_find_digits(OVC *blobs, OVC *blobsPlate, int numBlobs, int amp);
int IsBetween(int min, int value, int max);
int vc_draw_digits(OVC *blobs, OVC plate, IVC *dst);


