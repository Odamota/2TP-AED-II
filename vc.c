//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    VISÃO POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Desabilita (no MSVC++) warnings de funções não seguras (fopen, sscanf, etc...)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include "vc.h"


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUNÇÕES: ALOCAR E LIBERTAR UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Alocar memória para uma imagem
IVC *vc_image_new(int width, int height, int channels, int levels)
{
	IVC *image = (IVC *) malloc(sizeof(IVC));

	if(image == NULL) return NULL;
	if((levels <= 0) || (levels > 255)) return NULL;

	image->width = width;
	image->height = height;
	image->channels = channels;
	image->levels = levels;
	image->bytesperline = image->width * image->channels;
	image->data = (unsigned char *) malloc(image->width * image->height * image->channels * sizeof(char));

	if(image->data == NULL)
	{
		return vc_image_free(image);
	}

	return image;
}


// Libertar memória de uma imagem
IVC *vc_image_free(IVC *image)
{
	if(image != NULL)
	{
		if(image->data != NULL)
		{
			free(image->data);
			image->data = NULL;
		}

		free(image);
		image = NULL;
	}

	return image;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    FUNÇÕES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


char *netpbm_get_token(FILE *file, char *tok, int len)
{
	char *t;
	int c;
	
	for(;;)
	{
		while(isspace(c = getc(file)));
		if(c != '#') break;
		do c = getc(file);
		while((c != '\n') && (c != EOF));
		if(c == EOF) break;
	}
	
	t = tok;
	
	if(c != EOF)
	{
		do
		{
			*t++ = c;
			c = getc(file);
		} while((!isspace(c)) && (c != '#') && (c != EOF) && (t - tok < len - 1));
		
		if(c == '#') ungetc(c, file);
	}
	
	*t = 0;
	
	return tok;
}


long int unsigned_char_to_bit(unsigned char *datauchar, unsigned char *databit, int width, int height)
{
	int x, y;
	int countbits;
	long int pos, counttotalbytes;
	unsigned char *p = databit;

	*p = 0;
	countbits = 1;
	counttotalbytes = 0;

	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			pos = width * y + x;

			if(countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//*p |= (datauchar[pos] != 0) << (8 - countbits);
				
				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				*p |= (datauchar[pos] == 0) << (8 - countbits);

				countbits++;
			}
			if((countbits > 8) || (x == width - 1))
			{
				p++;
				*p = 0;
				countbits = 1;
				counttotalbytes++;
			}
		}
	}

	return counttotalbytes;
}


void bit_to_unsigned_char(unsigned char *databit, unsigned char *datauchar, int width, int height)
{
	int x, y;
	int countbits;
	long int pos;
	unsigned char *p = databit;

	countbits = 1;

	for(y=0; y<height; y++)
	{
		for(x=0; x<width; x++)
		{
			pos = width * y + x;

			if(countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//datauchar[pos] = (*p & (1 << (8 - countbits))) ? 1 : 0;

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				datauchar[pos] = (*p & (1 << (8 - countbits))) ? 0 : 1;
				
				countbits++;
			}
			if((countbits > 8) || (x == width - 1))
			{
				p++;
				countbits = 1;
			}
		}
	}
}


IVC *vc_read_image(char *filename)
{
	FILE *file = NULL;
	IVC *image = NULL;
	unsigned char *tmp;
	char tok[20];
	long int size, sizeofbinarydata;
	int width, height, channels;
	int levels = 255;
	int v;
	
	// Abre o ficheiro
	if((file = fopen(filename, "rb")) != NULL)
	{
		// Efectua a leitura do header
		netpbm_get_token(file, tok, sizeof(tok));

		if(strcmp(tok, "P4") == 0) { channels = 1; levels = 1; }	// Se PBM (Binary [0,1])
		else if(strcmp(tok, "P5") == 0) channels = 1;				// Se PGM (Gray [0,MAX(level,255)])
		else if(strcmp(tok, "P6") == 0) channels = 3;				// Se PPM (RGB [0,MAX(level,255)])
		else
		{
			#ifdef VC_DEBUG
			printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM, PGM or PPM file.\n\tBad magic number!\n");
			#endif

			fclose(file);
			return NULL;
		}
		
		if(levels == 1) // PBM
		{
			if(sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 || 
			   sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1)
			{
				#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM file.\n\tBad size!\n");
				#endif

				fclose(file);
				return NULL;
			}

			// Aloca memória para imagem
			image = vc_image_new(width, height, channels, levels);
			if(image == NULL) return NULL;

			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height;
			tmp = (unsigned char *) malloc(sizeofbinarydata);
			if(tmp == NULL) return 0;

			#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
			#endif

			if((v = fread(tmp, sizeof(unsigned char), sizeofbinarydata, file)) != sizeofbinarydata)
			{
				#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
				#endif

				vc_image_free(image);
				fclose(file);
				free(tmp);
				return NULL;
			}

			bit_to_unsigned_char(tmp, image->data, image->width, image->height);

			free(tmp);
		}
		else // PGM ou PPM
		{
			if(sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 || 
			   sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1 || 
			   sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &levels) != 1 || levels <= 0 || levels > 255)
			{
				#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PGM or PPM file.\n\tBad size!\n");
				#endif

				fclose(file);
				return NULL;
			}

			// Aloca memória para imagem
			image = vc_image_new(width, height, channels, levels);
			if(image == NULL) return NULL;

			#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
			#endif

			size = image->width * image->height * image->channels;

			if((v = fread(image->data, sizeof(unsigned char), size, file)) != size)
			{
				#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
				#endif

				vc_image_free(image);
				fclose(file);
				return NULL;
			}
		}
		
		fclose(file);
	}
	else
	{
		#ifdef VC_DEBUG
		printf("ERROR -> vc_read_image():\n\tFile not found.\n");
		#endif
	}
	
	return image;
}


int vc_write_image(char *filename, IVC *image)
{
	FILE *file = NULL;
	unsigned char *tmp;
	long int totalbytes, sizeofbinarydata;
	
	if(image == NULL) return 0;

	if((file = fopen(filename, "wb")) != NULL)
	{
		if(image->levels == 1)
		{
			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height + 1;
			tmp = (unsigned char *) malloc(sizeofbinarydata);
			if(tmp == NULL) return 0;
			
			fprintf(file, "%s %d %d\n", "P4", image->width, image->height);
			
			totalbytes = unsigned_char_to_bit(image->data, tmp, image->width, image->height);
			printf("Total = %ld\n", totalbytes);
			if(fwrite(tmp, sizeof(unsigned char), totalbytes, file) != totalbytes)
			{
				#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
				#endif

				fclose(file);
				free(tmp);
				return 0;
			}

			free(tmp);
		}
		else
		{
			fprintf(file, "%s %d %d 255\n", (image->channels == 1) ? "P5" : "P6", image->width, image->height);
		
			if(fwrite(image->data, image->bytesperline, image->height, file) != image->height)
			{
				#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
				#endif

				fclose(file);
				return 0;
			}
		}
		
		fclose(file);

		return 1;
	}
	
	return 0;
}

/*
int vc_gray_negative(IVC *srcdst) {
	if (&srcdst.channels != 1 || &srcdst.data == NULL || &srcdst.width <= 0 || &srcdst.height <= 0) return -1;


	for (int i = 0; i < &srcdst.width * &srcdst.height; i++) {
		data[i] = 255 - data[i];
	}
	return 0;
}

int vc_rgb_negative(IVC *srcdst) {
	for (int i = 0; i < &srcdst.width * &srcdst.height; i++) {
		data[i] = 255 - data[i];
	}
	return 0;
}
*/
int vc_rgb_to_gray(IVC *src, IVC *dst) {

	unsigned char *datasrc = (unsigned char*) src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char*) dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x,y;
	long int pos_src, pos_dst;
	float rf, gf, bf;

	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height)) return 0;
	if ((src->channels != 3) || (dst->channels !=1)) return 0;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			rf = (float) datasrc[pos_src];
			gf = (float) datasrc[pos_src+1];
			bf = (float) datasrc[pos_src+2];

			datadst[pos_dst] = (unsigned char) ((rf * 0.299) + (gf * 0.587) + (bf * 0.114));
		}
	}
	return 1;
}

int vc_rgb_to_hsv(IVC *src, IVC *dst)
	{
		unsigned char *datasrc = (unsigned char *)src->data;
		int bytesPerLine_src = src->width * src->channels;
		int channels_src = src->channels;

		unsigned char *datadst = (unsigned char *)dst->data;
		int bytesPerLine_dst = dst->width * dst->channels;
		int channels_dst = dst->channels;

		int width = src->width;
		int height = src->height;
		int size;

		float r, g, b, rgb_max, rgb_min, value, saturation, hue;

		// Verificação de erros
		if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
			return 0;
		if ((src->width != dst->width) || (src->height != dst->height))
			return 0;
		if (src->channels != dst->channels)
			return 0;

		// Calculat tamanho do array
		size = width * height * channels_src;

		for (int i = 0; i < size; i += channels_src)
		{
			// Leitura de informação da imagem original
			r = (float)datasrc[i];
			g = (float)datasrc[i + 1];
			b = (float)datasrc[i + 2];

			// Calcular valores máximo e mínimo dos canais de cor R, G, B
			rgb_max = (r > g ? (r > b ? r : b) : (g > b ? g : b));
			rgb_min = (r < g ? (r < b ? r : b) : (g < b ? g : b));

			// Value toma valores entre [0, 255]
			value = rgb_max;
			if (value == 0.0f)
			{
				saturation = 0;
				hue = 0;
			}
			else
			{
				if (rgb_max == rgb_min) saturation = 0;
				else
					saturation = ((rgb_max - rgb_min) / value);
				if (rgb_max == r && g >= b) hue = (60.0 * (g - b) / (rgb_max - rgb_min));
				if (rgb_max == r && b > g) hue = (360.0 + 60.0 * (g - b) / (rgb_max - rgb_min));
				if (rgb_max == g) hue = (120.0 + 60.0 * (b - r) / (rgb_max - rgb_min));
				if (rgb_max == b) hue = (240.0 + 60.0 * (r - g) / (rgb_max - rgb_min));
			}

			// Enviar valores para o array da nova imagem
			datadst[i] = (unsigned char)(hue / 360.0 * 255.0);
			datadst[i + 1] = (unsigned char)(saturation * 255.0);
			datadst[i + 2] = (unsigned char)value;
		}
		// Return
		return 1;
	}

	int vc_hsv_segmentation(IVC *src, IVC *dst, int hmin, int hmax, int smin, int smax, int vmin, int vmax)
	{
		unsigned char *datasrc = (unsigned char *)src->data;
		int bytesPerLine_src = src->width * src->channels;
		int channels_src = src->channels;

		unsigned char *datadst = (unsigned char *)dst->data;
		int bytesPerLine_dst = dst->width * dst->channels;
		int channels_dst = dst->channels;

		int width = src->width;
		int height = src->height;
		int size;

		float value, saturation, hue;
		float valueMin, valueMax, saturationMin, saturationMax, hueMin, hueMax;

		
		// Verificação de erros
		if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
			return 0;
		if ((src->width != dst->width) || (src->height != dst->height))
			return 0;
		if (src->channels == dst->channels)
			return 0;

		hueMin = ((float)hmin) / 360.0 * 255.0;
		hueMax = ((float)hmax) / 360.0 * 255.0;
		saturationMin = ((float)smin)/ 100.0 * 255.0;
		saturationMax = ((float)smax)/100.0 * 255.0;
		valueMin = ((float)vmin)/100.0 * 255.0;
		valueMax = ((float)vmax)/100.0 * 255.0;

		// Calcular tamanho do array
		size = width * height * channels_src;

		for (int i = 0,c=0; i < size; i += channels_src,c++)
		{
			hue = (float)src->data[i];
			saturation = (float)src->data[i+1];
			value = (float)src->data[i+2];

			if (valueMin<=value && value<=valueMax && hueMin<=hue && hue<=hueMax && saturationMin<=saturation && saturation<=saturationMax)
			{
				datadst[c] = 255;
			}
			else
			{
				datadst[c] = 0;
			}		
		}
		return 1;
	}

int vc_gray_to_rgb(IVC *src, IVC *dst) {
	unsigned char *datasrc = (unsigned char*) src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char*) dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x,y;
	long int pos_src, pos_dst;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			if (datasrc[pos_src] < 64) {
				datadst[pos_dst] = 0;
				datadst[pos_dst+1] = datasrc[pos_src] * 4;
				datadst[pos_dst+2] = 255;
			} else if (datasrc[pos_src] < 128) {
				datadst[pos_dst] = 0;
				datadst[pos_dst+1] = 255;
				datadst[pos_dst+2] = 255 - (datasrc[pos_src] - 64) * 4;
			} else if (datasrc[pos_src] < 192) {
				datadst[pos_dst] = (datasrc[pos_src]-128) * 4;
				datadst[pos_dst+1] = 255;
				datadst[pos_dst+2] = 0;
			} else {
				datadst[pos_dst] = 255;
				datadst[pos_dst+1] = 255 - (datasrc[pos_src] - 192) * 4;
				datadst[pos_dst+2] = 0;
			}
		}
	}

	return 1;

}

int vc_gray_binary(IVC *src, IVC *dst, unsigned char threshold) {
	unsigned char *datasrc = (unsigned char*) src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char*) dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x,y;
	long int pos_src, pos_dst;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			if (datasrc[pos_src] > threshold) {
				datadst[pos_dst] = 255;
			} else {
				datadst[pos_dst] = 0;
			}
		}
	}

	return 1;
}

int vc_gray_binary_global_mean(IVC *src, IVC *dst) {
	unsigned char *datasrc = (unsigned char*) src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char*) dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x,y;
	long int pos_src, pos_dst;
	int sum = 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;

			sum += datasrc[pos_dst];
		}
	}

	unsigned char threshold = sum/(width*height);
	printf("%d\n",threshold);
	vc_gray_binary(src, dst, threshold);
	return 1;
}

// Converter de Gray para Binário (threshold automático Midpoint)
int vc_gray_to_binary_midpoint(IVC *src, IVC *dst, int kernel)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, kx, ky;
	int offset = (kernel - 1) / 2; //(int) floor(((double) kernel) / 2.0);
	int max, min;
	long int pos, posk;
	unsigned char threshold;

	// Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height) || (src->channels != dst->channels)) return 0;
	if (channels != 1) return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			max = 0;
			min = 255;

			// NxM Vizinhos
			for (ky = -offset; ky <= offset; ky++)
			{
				for (kx = -offset; kx <= offset; kx++)
				{
					if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
					{
						posk = (y + ky) * bytesperline + (x + kx) * channels;

						if (datasrc[posk] > max) max = datasrc[posk];
						if (datasrc[posk] < min) min = datasrc[posk];
					}
				}
			}

			threshold = (unsigned char)((float)(max + min) / (float)2);

			if (datasrc[pos] > threshold) datadst[pos] = 255;
			else datadst[pos] = 0;
		}
	}

	return 1;
}

int vc_binary_dilate(IVC *src, IVC *dst, int kernel) {
	unsigned char *datasrc = (unsigned char*) src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char*) dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x,y;
	long int pos_src, pos_dst, pos_k;

	int offset = (kernel-1)/2;
	int kx, ky, primeiro_plano;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;
			primeiro_plano = 0;

			for (kx = -offset; kx <= offset; kx++) {
				for (ky = -offset; ky <= offset; ky++) {
					
					if ((x+kx >= 0) && (x+kx < width) && (y+ky >= 0) && (y+ky < height)) {
						
						pos_k = (y+ky)*bytesperline_src + (x+kx)*channels_src;

						primeiro_plano |= datasrc[pos_k];
					}
				}
			}

		datadst[pos_dst] = primeiro_plano;
		
		}
	}

	return 1;

}

int vc_binary_erode(IVC *src, IVC *dst, int kernel) {
	unsigned char *datasrc = (unsigned char*) src->data;
	int bytesperline_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char *datadst = (unsigned char*) dst->data;
	int bytesperline_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	int x,y;
	long int pos_src, pos_dst, pos_k;

	int offset = (kernel-1)/2;
	int kx, ky, primeiro_plano;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			pos_src = y * bytesperline_src + x * channels_src;
			pos_dst = y * bytesperline_dst + x * channels_dst;
			primeiro_plano = 255;

			for (kx = -offset; kx <= offset; kx++) {
				for (ky = -offset; ky <= offset; ky++) {
					
					if ((x+kx >= 0) && (x+kx < width) && (y+ky >= 0) && (y+ky < height)) {
						
						pos_k = (y+ky)*bytesperline_src + (x+kx)*channels_src;

						primeiro_plano &= datasrc[pos_k];
					}
				}
			}

		datadst[pos_dst] = primeiro_plano;
			
		}
	}

	return 1;

}

int vc_open_image(char *filename) {
	char command[256] = "xdg-open ";
 	strcat(command, filename);
 	system(command);
 	return 1;
}

// Etiquetagem de blobs
// src		: Imagem binária de entrada
// dst		: Imagem grayscale (irá conter as etiquetas)
// nlabels	: Endereço de memória de uma variável, onde será armazenado o número de etiquetas encontradas.
// OVC*		: Retorna um array de estruturas de blobs (objectos), com respectivas etiquetas. É necessário libertar posteriormente esta memória.
OVC* vc_binary_blob_labelling(IVC *src, IVC *dst, int *nlabels)
{
	unsigned char *datasrc = (unsigned char *)src->data;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, a, b;
	long int i, size;
	long int posX, posA, posB, posC, posD;
	int labeltable[256] = { 0 };
	int labelarea[256] = { 0 };
	int label = 1; // Etiqueta inicial.
	int num, tmplabel;
	OVC *blobs; // Apontador para array de blobs (objectos) que será retornado desta função.

	// Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height) || (src->channels != dst->channels)) return NULL;
	if (channels != 1) return NULL;

	// Copia dados da imagem binária para imagem grayscale
	memcpy(datadst, datasrc, bytesperline * height);

	// Todos os pixéis de plano de fundo devem obrigatóriamente ter valor 0
	// Todos os pixéis de primeiro plano devem obrigatóriamente ter valor 255
	// Serão atribuídas etiquetas no intervalo [1,254]
	// Este algoritmo está assim limitado a 255 labels
	for (i = 0, size = bytesperline * height; i<size; i++)
	{
		if (datadst[i] != 0) datadst[i] = 255;
	}

	// Limpa os rebordos da imagem binária
	for (y = 0; y<height; y++)
	{
		datadst[y * bytesperline + 0 * channels] = 0;
		datadst[y * bytesperline + (width - 1) * channels] = 0;
	}
	for (x = 0; x<width; x++)
	{
		datadst[0 * bytesperline + x * channels] = 0;
		datadst[(height - 1) * bytesperline + x * channels] = 0;
	}

	// Efectua a etiquetagem
	for (y = 1; y<height - 1; y++)
	{
		for (x = 1; x<width - 1; x++)
		{
			// Kernel:
			// A B C
			// D X

			posA = (y - 1) * bytesperline + (x - 1) * channels; // A
			posB = (y - 1) * bytesperline + x * channels; // B
			posC = (y - 1) * bytesperline + (x + 1) * channels; // C
			posD = y * bytesperline + (x - 1) * channels; // D
			posX = y * bytesperline + x * channels; // X

			// Se o pixel foi marcado
			if (datadst[posX] != 0)
			{
				if ((datadst[posA] == 0) && (datadst[posB] == 0) && (datadst[posC] == 0) && (datadst[posD] == 0))
				{
					datadst[posX] = label;
					labeltable[label] = label;
					label++;
				}
				else
				{
					num = 255;

					// Se A está marcado
					if (datadst[posA] != 0) num = labeltable[datadst[posA]];
					// Se B está marcado, e é menor que a etiqueta "num"
					if ((datadst[posB] != 0) && (labeltable[datadst[posB]] < num)) num = labeltable[datadst[posB]];
					// Se C está marcado, e é menor que a etiqueta "num"
					if ((datadst[posC] != 0) && (labeltable[datadst[posC]] < num)) num = labeltable[datadst[posC]];
					// Se D está marcado, e é menor que a etiqueta "num"
					if ((datadst[posD] != 0) && (labeltable[datadst[posD]] < num)) num = labeltable[datadst[posD]];

					// Atribui a etiqueta ao pixel
					datadst[posX] = num;
					labeltable[num] = num;

					// Actualiza a tabela de etiquetas
					if (datadst[posA] != 0)
					{
						if (labeltable[datadst[posA]] != num)
						{
							for (tmplabel = labeltable[datadst[posA]], a = 1; a<label; a++)
							{
								if (labeltable[a] == tmplabel)
								{
									labeltable[a] = num;
								}
							}
						}
					}
					if (datadst[posB] != 0)
					{
						if (labeltable[datadst[posB]] != num)
						{
							for (tmplabel = labeltable[datadst[posB]], a = 1; a<label; a++)
							{
								if (labeltable[a] == tmplabel)
								{
									labeltable[a] = num;
								}
							}
						}
					}
					if (datadst[posC] != 0)
					{
						if (labeltable[datadst[posC]] != num)
						{
							for (tmplabel = labeltable[datadst[posC]], a = 1; a<label; a++)
							{
								if (labeltable[a] == tmplabel)
								{
									labeltable[a] = num;
								}
							}
						}
					}
					if (datadst[posD] != 0)
					{
						if (labeltable[datadst[posD]] != num)
						{
							for (tmplabel = labeltable[datadst[posC]], a = 1; a<label; a++)
							{
								if (labeltable[a] == tmplabel)
								{
									labeltable[a] = num;
								}
							}
						}
					}
				}
			}
		}
	}

	// Volta a etiquetar a imagem
	for (y = 1; y<height - 1; y++)
	{
		for (x = 1; x<width - 1; x++)
		{
			posX = y * bytesperline + x * channels; // X

			if (datadst[posX] != 0)
			{
				datadst[posX] = labeltable[datadst[posX]];
			}
		}
	}

	//printf("\nMax Label = %d\n", label);

	// Contagem do número de blobs
	// Passo 1: Eliminar, da tabela, etiquetas repetidas
	for (a = 1; a<label - 1; a++)
	{
		for (b = a + 1; b<label; b++)
		{
			if (labeltable[a] == labeltable[b]) labeltable[b] = 0;
		}
	}
	// Passo 2: Conta etiquetas e organiza a tabela de etiquetas, para que não hajam valores vazios (zero) entre etiquetas
	*nlabels = 0;
	for (a = 1; a<label; a++)
	{
		if (labeltable[a] != 0)
		{
			labeltable[*nlabels] = labeltable[a]; // Organiza tabela de etiquetas
			(*nlabels)++; // Conta etiquetas
		}
	}

	// Se não há blobs
	if (*nlabels == 0) return NULL;

	// Cria lista de blobs (objectos) e preenche a etiqueta
	blobs = (OVC *)calloc((*nlabels), sizeof(OVC));
	if (blobs != NULL)
	{
		for (a = 0; a<(*nlabels); a++) blobs[a].label = labeltable[a];
	}
	else return NULL;

	return blobs;
}


int vc_binary_blob_info(IVC *src, OVC *blobs, int nblobs)
{
	unsigned char *data = (unsigned char *)src->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, i;
	long int pos;
	int xmin, ymin, xmax, ymax;
	long int sumx, sumy;

	// Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if (channels != 1) return 0;

	// Conta área de cada blob
	for (i = 0; i<nblobs; i++)
	{
		xmin = width - 1;
		ymin = height - 1;
		xmax = 0;
		ymax = 0;

		sumx = 0;
		sumy = 0;

		blobs[i].area = 0;

		for (y = 1; y<height - 1; y++)
		{
			for (x = 1; x<width - 1; x++)
			{
				pos = y * bytesperline + x * channels;

				if (data[pos] == blobs[i].label)
				{
					// Área
					blobs[i].area++;

					// Centro de Gravidade
					sumx += x;
					sumy += y;

					// Bounding Box
					if (xmin > x) xmin = x;
					if (ymin > y) ymin = y;
					if (xmax < x) xmax = x;
					if (ymax < y) ymax = y;

					// Perímetro
					// Se pelo menos um dos quatro vizinhos não pertence ao mesmo label, então é um pixel de contorno
					if ((data[pos - 1] != blobs[i].label) || (data[pos + 1] != blobs[i].label) || (data[pos - bytesperline] != blobs[i].label) || (data[pos + bytesperline] != blobs[i].label))
					{
						blobs[i].perimeter++;
					}
				}
			}
		}

		// Bounding Box
		blobs[i].x = xmin;
		blobs[i].y = ymin;
		blobs[i].width = (xmax - xmin) + 1;
		blobs[i].height = (ymax - ymin) + 1;

		// Centro de Gravidade
		//blobs[i].xc = (xmax - xmin) / 2;
		//blobs[i].yc = (ymax - ymin) / 2;
		blobs[i].xc = sumx / MAX(blobs[i].area, 1);
		blobs[i].yc = sumy / MAX(blobs[i].area, 1);
	}

	return 1;
}


int vc_draw_boundingbox(IVC *src, int x, int y, int width, int height, int cor) {

	int pos, pos2;
	//Horizontal
	for (int i = 0; i < width; i++)
	{
		pos = y * src->bytesperline + (x+i) * src->channels;
		src -> data[pos] = cor;
		src -> data[pos + src->bytesperline] = cor;
		src -> data[pos - src->bytesperline] = cor;

		pos2 = (y+height) * src->bytesperline + (x+i) * src->channels;
		src -> data[pos2] = cor;
		src -> data[pos2 + src->bytesperline] = cor;
		src -> data[pos2 - src->bytesperline] = cor;
	}

	//Vertical
	for (int i = 0; i < height; i++)
	{
		pos = (y+i) * src->bytesperline + x * src->channels;
		src -> data[pos] = cor;
		src -> data[pos + src->channels] = cor;
		src -> data[pos - src->channels] = cor;

		pos2 = (y+i) * src->bytesperline + (x+width) * src->channels;
		src -> data[pos2] = cor;
		src -> data[pos2 + src->channels] = cor;
		src -> data[pos2 - src->channels] = cor;
	}

	return 1;
}

int vc_draw_centerofgravity(IVC *src, int xc, int yc) {
	
	return 1;
}


int vc_gray_histogram_show(IVC *src, IVC *dst) {
	int brilho[256] = {0};
	int n = src -> width * src -> height;
	for (int i = 0; i < n; i++) {
		brilho[src -> data[i]] += 1; 
	}

	float pdf[256];
	float pdfmax = 0;
	for (int i = 0; i < 256; i++) {
		pdf[i] = (float)brilho[i] / (float)n;
		if (pdf[i] > pdfmax) pdfmax = pdf[i];
	}

	if (pdfmax == 0) return 2;

	int pos;
	for (int i = 0; i < 256; i++) {
		float limit = (256 * (float) pdf[i])/ pdfmax;
		for (int j = 0; j < (int)limit; j++) {
			pos = (255-j) * dst->bytesperline + i * dst->channels;
			dst -> data[pos] = 255;
		}
	}	

	return 1;
}

int vc_rgb_histogram_show(IVC *src, IVC *dst) {
	int brilho[256] = {0};
	int n = src -> width * src -> height;
	for (int i = 0; i < n; i++) {
		brilho[src -> data[i]] += 1; 
	}

	float pdf[256];
	float pdfmax = 0;
	for (int i = 0; i < 256; i++) {
		pdf[i] = (float)brilho[i] / (float)n;
		if (pdf[i] > pdfmax) pdfmax = pdf[i];
	}

	if (pdfmax == 0) return 2;

	int pos;
	for (int i = 0; i < 256; i++) {
		float limit = (256 * (float) pdf[i])/ pdfmax;
		for (int j = 0; j < (int)limit; j++) {
			pos = (255-j) * dst->bytesperline + i * dst->channels;
			dst -> data[pos] = 255;
		}
	}	

	return 1;
}

int vc_gray_histogram_equalization(IVC *src, IVC *dst) {
	int brilho[256] = {0};
	int n = src -> width * src -> height;
	for (int i = 0; i < n; i++) {
		brilho[src -> data[i]] += 1; 
	}

	float pdf[256];
	float pdfmax = 0;
	for (int i = 0; i < 256; i++) {
		pdf[i] = (float)brilho[i] / (float)n;
		if (pdf[i] > pdfmax) pdfmax = pdf[i];
	}

	float cdf[256] = {0};
	cdf[0] = pdf[0];
	float aux = cdf[0];
	for (int i = 1; i < 256; i++) {
		cdf[i] = aux + pdf[i];
		aux = cdf[i];
	}

	int i = 0;
	while (cdf[i] == 0) i++;

	if (cdf[i] == 0) {
		return 2;
	}
	float cdfmin = cdf[i];

	for (int i = 0; i < n; i++) {
		dst -> data[i] = (cdf[src -> data[i]] - cdfmin)*(dst -> levels-1)/(1 - cdfmin);
		//dst -> data[i] = cdf[src -> data[i]]*((dst -> levels-1)-1);
	}

	return 1;
}

int vc_gray_edge_prewitt(IVC *src, IVC *dst, float th) {
	
	int mx, my, magn, pos;
	my = 0; mx = 0;
	for (int x = 1; x < src->width-1; x++) {
		for (int y = 1; y < src->height-1; y++) {
			pos = y * src->bytesperline + x * src->channels;

			mx = 0; my = 0;
			for (int i = -1; i < 2; i+=2) {
				for (int j = -1; j < 2; j++) {
					my += i * (src->data[pos + j * src->channels + i * src->bytesperline]);
					mx += i * (src->data[pos + j * src->bytesperline + i * src->channels]);
				}
			}

			my /= 3;
			mx /= 3;

			magn = ((mx*mx)^2 + (my*my)^2)^(1/2);

			dst->data[pos] = magn;

			
		}
	}

	int hist[256] = {0};
	for (int y = 0; y < dst->height; y++) {
		for (int x = 0; x < dst->width; x++){
			hist[dst->data[y * dst->bytesperline + x * dst->channels]]++;
		}
	}

	int histmax = 0;
	int size = dst->width * dst->height;

	int i;
	for (i = 0; i <= 255; i++) {
		histmax += hist[i];

		if (histmax >= (((float)size)*th)) break;
	}
	int histthreshold = i;

	for (int y = 0; y < dst->height; y++) {
		for (int x = 0; x < dst->width; x++){
			pos = y * dst->bytesperline + x * dst->channels;
			if (dst->data[pos] >= histthreshold) { dst->data[pos] = 255;} else dst->data[pos] = 0;
		}
	}

	return 1;
}


int vc_gray_edge_sobel(IVC *src, IVC *dst, float th) {
	
	int mx, my, magn, pos, c;
	my = 0; mx = 0;
	for (int x = 1; x < src->width-1; x++) {
		for (int y = 1; y < src->height-1; y++) {
			pos = y * src->bytesperline + x * src->channels;

			mx = 0; my = 0;
			for (int i = -1; i < 2; i+=2) {
				for (int j = -1; j < 2; j++) {
					c = i;
					if (j == 0) c *= 2;
					my += c * (src->data[pos + j * src->channels + i * src->bytesperline]);
					mx += c * (src->data[pos + j * src->bytesperline + i * src->channels]);
				}
			}

			my /= 3;
			mx /= 3;

			magn = ((mx*mx)^2 + (my*my)^2)^(1/2);

			dst->data[pos] = magn;

			
		}
	}

	int hist[256] = {0};
	for (int y = 0; y < dst->height; y++) {
		for (int x = 0; x < dst->width; x++){
			hist[dst->data[y * dst->bytesperline + x * dst->channels]]++;
		}
	}

	int histmax = 0;
	int size = dst->width * dst->height;

	int i;
	for (i = 0; i <= 255; i++) {
		histmax += hist[i];

		if (histmax >= (((float)size)*th)) break;
	}
	int histthreshold = i;

	for (int y = 0; y < dst->height; y++) {
		for (int x = 0; x < dst->width; x++){
			pos = y * dst->bytesperline + x * dst->channels;
			if (dst->data[pos] >= histthreshold) { dst->data[pos] = 255;} else dst->data[pos] = 0;
		}
	}

	return 1;
}

int vc_edges_classic(IVC *src, IVC *dst, int bright) {
	int pos;
	int diff;

	for (int x = 0; x < src->width-1; x++) {
		for (int y = 0; y < src->height-1; y++) {
			pos = y * src->bytesperline + x * src->channels;
	
			if (dst->data[pos] != 255) {
				//diff = 2*src->data[pos] - src->data[pos-1] + src->data[pos+1]; //2da der
				diff = src->data[pos+1] - src->data[pos]; //clasico
				//diff = src->data[pos+1] - src->data[pos-1]; //1ra der
				if (diff < 0) diff *= -1;	
				if (diff > bright) dst->data[pos] = 255;

			}

			if (dst->data[pos] != 255) {
				//diff = 2*src->data[pos] - src->data[pos - src->bytesperline] + src->data[pos + src->bytesperline]; //2da der
				//diff = src->data[pos + src->bytesperline] - src->data[pos - src->bytesperline]; //1ra derivada
				diff = src->data[pos + src->bytesperline] - src->data[pos]; //clasico
				if (diff < 0) diff *= -1;	
				if (diff > bright) dst->data[pos] = 255;
			}
			
		}
	}

	return 1;
}

int vc_graph_edges(IVC *src, IVC *col, IVC *row) {

	int *brow = malloc(sizeof(int)*src->height);
	int *bcol = malloc(sizeof(int)*src->width);
	int pos, rmax, cmax;
	float limit;
	rmax = 0;
	cmax = 0;

	//Initialize array
	for (int i = 0; i < src->height; i++){
		brow[i] = 0;
	}
	for (int i = 0; i < src->width; i++){
		bcol[i] = 0;
	}

	//Filling arrays
	for (int x = 0; x < src->width; x++) {
		for (int y = 0; y < src->height; y++) {
			pos = y * src->bytesperline + x * src->channels;
			if (src->data[pos] == 255) bcol[x] += 1;
		}
		if (bcol[x] > cmax) cmax = bcol[x];
	}

	for (int y = 0; y < src->height; y++) {
		for (int x = 0; x < src->width; x++) {
			pos = y * src->bytesperline + x * src->channels;
			if (src->data[pos] == 255) brow[y] += 1;
		}
		if (brow[y] > rmax) rmax = brow[y];
	}

	//Creating tables
	for (int x = 0; x < src->width; x++) {
		limit = (256 * (float) bcol[x])/ cmax;
		for (int y = 0; y < (int)limit; y++) {
			pos = (255-y) * col->bytesperline + x * col->channels;
			col -> data[pos] = 255;
		}
	}

	for (int x = 0; x < src->height; x++) {
		limit = (256 * (float) brow[x])/ rmax;
		for (int y = 0; y < (int)limit; y++) {
			pos = (255-y) * row->bytesperline + x * row->channels;
			row -> data[pos] = 255;
		}
	}

	return 1;
}

int vc_gray_lowpass_mean_filter(IVC *src, IVC *dst, int kernelsize) {

	int pos, posx, sum, cnt;
	for (int x = 0; x < src->width; x++) {
		for (int y = 0; y < src->height; y++) {
			pos = x * src->channels + y * src->bytesperline;

			sum = 0; cnt = 0;
			for (int i = (kernelsize-1)/2; i < kernelsize; i++) {
				for (int j = (kernelsize-1)/2; j < kernelsize; j++) {
					posx = pos + i * src->channels + j * src->bytesperline;
					if (posx >= 0) {
						sum += src->data[posx];
						cnt++;
					}
				}
			}

			dst->data[pos] = sum/cnt;
		}
	}

	return 1;
}

int vc_gray_lowpass_median_filter(IVC *src, IVC *dst, int kernelsize) {
	
	int pos, posx, cnt, aux;
	int arr[256] = {-1};
	for (int x = 0; x < src->width; x++) {
		for (int y = 0; y < src->height; y++) {
			pos = x * src->channels + y * src->bytesperline;

			cnt = 0;
			for (int i = (kernelsize-1)/2; i < kernelsize; i++) {
				for (int j = (kernelsize-1)/2; j < kernelsize; j++) {
					posx = pos + i * src->channels + j * src->bytesperline;
					if (posx >= 0) {
						arr[cnt] = src->data[posx];
						cnt++;
					}
				}
			}

			for (int i = 0; i < cnt; i++) {
				for (int j = 0; j < cnt; j++) {
					if (arr[j] > arr[j + 1]) {
						aux = arr[j+1];
						arr[j+1] = arr[j];
						arr[j] = aux;
					}
				}
			}

			dst->data[pos] = arr[(cnt-1)/2];
			
		}
	}

	return 1;
}

// Conta nº de pontos brancos numa imagem binária
int vc_count_points_column(IVC* src, int *qtPts) {

	long int pos;

	for (int x = 0; x < src->width; x++) {
		for (int y = src->height -1; y > 0 ; y--) {
			pos = y * src->bytesperline + x * src->channels;
			if (src->data[pos] == 255) qtPts[x]++;
			else continue;
		}		
	}

	return 1;
}

int vc_segmentate_rect_over_mean(IVC *src, IVC *dst, IVC *col, IVC *row) {

	//Mean (for columns and lines is equal)
	int pos;
	int sum = 0;
	int med;
	for (int i = 0; i < col->width; i++) {
		for (int j = 0; j < 256; j++) {
			pos = i * col->channels + (255-j) * col->bytesperline;
			if (col->data[pos] == 0) {
				sum += j;
				break;
			}
		}
	}
	med = sum / col->width;
	printf("Media  %d\n", med);

	//Array of points
	int *colArr = (int*) calloc(col->width, sizeof(int));
	int *rowArr = (int*) calloc(row->width, sizeof(int));

	vc_count_points_column(col, colArr);
	vc_count_points_column(row, rowArr);


	int height, width, relation, auxPos;
	relation = 5;
	//Search for first row
	for (int i = 0; i < row->width; i++) {
		//Check points
		if (rowArr[i] > med * 2) {
			//Search for second row
			for (int j = i+1; j < row->width; j++) {
				//Check points
				if (rowArr[i] + 6 >= rowArr[j] && rowArr[i] - 6 <= rowArr[j]) {
					height = j - i;

					//Search for first col
					for (int m = 0; m < col->width; m++) {
						printf("CU %d\n", m);
						//Check points
						if (colArr[m] > med * 2) {
							//Search second col
							auxPos = m + (relation * height);
							for (int n = 0; n < 5; n++) {
								if (auxPos + n >= col->width || auxPos - n < 0 || auxPos - n <= m) break;
								printf("CD %d\n", auxPos + n);
								//Check points (symetry)
								if (colArr[m] + 6 >= colArr[auxPos + n] && colArr[m] - 6 <= colArr[auxPos + n]){

									width = auxPos + n - m;
									printf("Width: %d, Height: %d, Posx: %d, Posy: %d\n", width, height, i, m);
									vc_draw_boundingbox(dst, i, m, width, height, 255);
									break;
								} else if (colArr[m] + 6 >= colArr[auxPos - n] && colArr[m] - 6 <= colArr[auxPos - n]) {
									width = auxPos - n - m;

									printf("Width: %d, Height: %d, Posx: %d, Posy: %d\n", width, height, i, m);
									vc_draw_boundingbox(dst, i, m, width, height, 255);
									break;
								}
							}
						}
					}

				}
			}
		}
	}

	
	//Rect Search Algorithm

/*
	//Search first row
	for (int i = 0; i < row->width; i++) {

		//Check first row white points
		cnt1 = 255;
		while (cnt1 > (med + (med/2) - 1) && row->data[i * row->channels + (255-cnt1) * row->bytesperline] == 0) {
			cnt1--;
		}

		//Limit search to higher points
		if (cnt1 > (med + (med/2))) {
			//Search second row
			for (int j = i; j < row->width; j++) {

				//Check second row white points
				cnt2 = 255;
				while (cnt2 > (med + (med/2) - 1) && row->data[j * row->channels + (255-cnt2) * row->bytesperline] == 0) {
					cnt2--;
				}

				//Find rigth size row
				if ((cnt1 + 6 >= cnt2) && (cnt1 - 6 <= cnt2)) {
					height = j - i;


					//Search first col
					for (int m = 0; m < col->width; m++) {
						
						//Check first col white points
						cnt3 = 255;
						while (cnt3 > (med + (med/2) - 1) && col->data[m * col->channels + (255-cnt3) * col->bytesperline] == 0) {
							cnt3--;
						}

						//Limit search to higher points 
						if (cnt3 > (med + (med/2))) {
							//Search second col
							for (int n = m + (relation * height) - 5; n < row->width && n < m + (relation * height) + 5; m++) {
								
								//Check second col white points
								cnt4 = 255;
								while (cnt4 > (med + (med/2) - 1) && col->data[n * col->channels + (255-cnt4) * col->bytesperline] == 0) {
									cnt4--;
								}

								//Limit search to higher points
								if ((cnt3 + 6 >= cnt4) && (cnt3 - 6 <= cnt4)) {
									width = n - m;

									vc_draw_boundingbox(dst, i * dst->channels, m * dst->bytesperline, width, height);
								}
							}
						}
					}
				}
			}
		}
	}

*/
	return 1;
}

int vc_clean_points(IVC *src, IVC * dst) {
	
	int pos;
	int found;

	for (int i = 1; i < src->width-1; i++) {
		for (int j = 1; j < src->height-1; j++) {
			
			found = 0;
			pos = i * src->channels + j * src->bytesperline;
			for (int x = -1; x < 2; x++) {
				for (int y = -1; y < 2; y++) {
					if (x == 0 && y == 0) continue;
					
					if (src->data[pos + x * src->channels + y * src->bytesperline] == 255) {
						found = 1;
						break;
					}
				}
				if (!found) {
					dst->data[pos] = 0;
				} else {
					dst->data[pos] = src->data[pos];
				}
			}

		}
	}

	return 1;
}

int vc_gray_lowpass_gaussian_filter(IVC *src, IVC *dst) {
	unsigned char *datasrc = (unsigned char *)src->data;
	unsigned char *datadst = (unsigned char *)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, i, j;
	long int pos, posX;
	// Condisdere-se uma função Gaussina com desvio padrão (STD) de 1 e média 0
	// Então, as porbabilidades para -2*STD, -STD, 0, STD, 2STD sáo:
	float gaussarray[5] = { (float) 0.061, (float) 0.242, (float) 0.383, (float) 0.242,(float) 0.061 };
	float gausssum;

	// Verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL)) return 0;
	if ((src->width != dst->width) || (src->height != dst->height) || (src->channels != dst->channels)) return 0;
	if (channels != 1) return 0;

	for (y = 5; y < height; y++) {
		for (x = 5; x < width; x++) {
			gausssum = 0.0;

			for (j = 0; j < 5; j++) {
				for (i = 0; i < 5; i++) {
					
					pos = (y - j) * bytesperline + (x - i) * channels;
					//sif (y-j < 0 || x -i < 0) continue;

					gausssum += ((float)datasrc[pos]) * gaussarray[i] * gaussarray[j];
				}
			}
			posX = y * bytesperline + x * channels;

			datadst[posX] = (unsigned char)gausssum;
		}
	}
	return 1;
}

int vc_fill_edges(IVC *srcdst) {

	int pos1, pos2;
	for (int x = 1; x < srcdst->width-1; x++) {
		for (int y = 1; y < srcdst->height-1; y++) {

			pos1 = x * srcdst->channels + y * srcdst->bytesperline;
			pos2 = (srcdst->width - x) * srcdst->channels + (srcdst->height - y) * srcdst->bytesperline;

			//Top to Bottom
			if (srcdst->data[pos1 - srcdst->channels] == 255 && srcdst->data[pos1 - srcdst->bytesperline]) {
				srcdst->data[pos1] = 255;
			}

			//Bottom to Top
			if (srcdst->data[pos2 + srcdst->channels] == 255 && srcdst->data[pos2 + srcdst->bytesperline]) {
				srcdst->data[pos2] = 255;
			}

		}
	}

	return 1;
}

OVC vc_blob_plate_selection(IVC *dst, OVC *blobs, int nlabels) {

	float relation, amp;
	int areaMin;
	amp = 1.5;
	areaMin = (dst->width * dst->height) * 0.02;

	for (int i = 0; i < nlabels; i++) {
		
		relation = (float) blobs[i].width / (float) blobs[i].height;
		if (relation >= 5 - amp && relation <= 5 + amp && blobs[i].area > areaMin) {
			vc_draw_boundingbox(dst, blobs[i].x, blobs[i].y, blobs[i].width, blobs[i].height, 255);
			return blobs[i];
		}
	}

	OVC nullBlob;
	return nullBlob;
}

int vc_obj_to_image(OVC blob, IVC *src, IVC *dst) {

	int pos, posSrc;

	for (int i = 0; i < dst->width; i++) {
		for (int j = 0; j < dst->height; j++) {
			pos = i * dst->channels + j * dst->bytesperline;
			posSrc = (blob.x + i) * src->channels + (blob.y + j) * src->bytesperline;

			dst->data[pos] = src->data[posSrc];
		}
	}

	return 1;
}

int vc_blob_to_erase_selection(IVC *dst, OVC *blobs, int nlabels) {

	int areaLim, heightLim, widthLim;
	areaLim = (dst->width * dst->height) * 0.03;
	heightLim = dst->height * 0.8;
	widthLim = dst->width * 0.2;

	for (int i = 0; i < nlabels; i++) {
		if (blobs[i].area > areaLim || blobs[i].height > heightLim || blobs[i].width > widthLim) {
			vc_erase_blob(dst, blobs[i]);
		}
	}

	return 1;
}

int vc_erase_blob(IVC *srcdst, OVC blob) {

	int pos;
	for (int i = blob.x; i < blob.width + blob.x; i++) {
		for (int j = blob.y; j < blob.height + blob.y; j++) {
			pos = i * srcdst->channels + j * srcdst->bytesperline;

			if (srcdst->data[pos] == blob.label) srcdst->data[pos] = 0;
		}
	}

	return 1;
}

int vc_blob_find_digits(OVC *blobs, OVC *blobsPlate, int numBlobs, int amp) {

	int maxArea = 0;
	int indice = 0;

	// Encontrar altura máxima
	for (int i = 0; i < numBlobs; i++) {
		if (blobs[i].area > maxArea) maxArea = blobs[i].area;
		printf("AREA n %d:%d\n", i, blobs[i].area);
	}

	// Encontrar blobs com altura máxima, ou dentro da amplitude estabelecida
	for (int i = 0; i < numBlobs; i++) {
		if (IsBetween(maxArea - amp*amp, blobs[i].area, maxArea)) {
			printf("FOUND %d\n", indice);
			blobsPlate[indice++] = blobs[i];
		}
	}

	return 1;
}

int IsBetween(int min, int value, int max) {
	if (value <= max && value >= min) return 1;
	return 0;
}

int vc_draw_digits(OVC *blobs, OVC plate, IVC *dst) {

	for (int i = 0; i < 6; i++) {
		vc_draw_boundingbox(dst, blobs[i].x + plate.x, blobs[i].y + plate.y, blobs[i].width, blobs[i].height, 100);
	}

	return 1;
}