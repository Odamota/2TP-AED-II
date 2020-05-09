#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "vc.h"

/*int main()
{
	
	IVC *image = vc_read_image("Images/FLIR/flir-01.pgm");
	IVC *image3 = vc_image_new( image->width, image->width, image->channels, image->levels);
	IVC *image3 = vc_image_new( image->width, image->width, image->channels, image->levels);

	int nblobs;
	vc_gray_binary(image,image3,127);
	OVC *blobs; 
	blobs = vc_binary_blob_labelling(image3, image3, &nblobs);

	if (blobs != NULL) {
		vc_binary_blob_info(image3, blobs, nblobs);


		printf("%s: %d","Number of segmented objects: ", nblobs);

		for (int i = 0; i < nblobs; i++)
		{
			vc_draw_boundingbox(image3, blobs[i].x, blobs[i].y, blobs[i].width, blobs[i].height);
			vc_draw_centerofgravity(image3, blobs[i].xc, blobs[i].yc);

			if (blobs[i].area > 700) {
				printf("\n-> Label %d:\n",blobs[i].label);
				printf("	Area=%-5d Perimetro=%-5d x=%-5d y=%-5d w=%-5d h=%-5d xc=%-5d yc=%-5d", blobs[i].area, blobs[i].perimeter, blobs[i].x, blobs[i].y, blobs[i].width, blobs[i].height, blobs[i].xc, blobs[i].yc);
			}
		}
	}


	char *filename = "Images/Generated/flir-01_binary.pbm";
	char *filename1 = "Images/Generated/blob.pbm";
	vc_write_image(filename, image3);
	vc_write_image(filename1, image3);

	vc_image_free(image);
	vc_image_free(image3);
	vc_image_free(image3);
 	
 	vc_open_image(filename);
 	vc_open_image(filename1);

	return 0;
}

int main()
{
	
	IVC *image = vc_read_image("Images/barbara2.pgm");
	IVC *image3 = vc_image_new( image->height, image->height, image->channels, image->levels);
	//IVC *green = vc_image_new( 256, 256, image->channels, image->levels);
	//IVC *blue = vc_image_new( 256, 256, image->channels, image->levels);

	vc_gray_histogram_equalization(image, image3);

	char *filename = "Images/Generated/barbara2_equal.pgm";
	//char *filename1 = "Images/Generated/boats_histogram.pbm";
	//char *filename2 = "Images/Generated/boats_histogram.pbm";
	vc_write_image(filename, image3);

	vc_image_free(image);
	vc_image_free(image3);
 	
 	vc_open_image(filename);
 	vc_open_image("Images/barbara2.pgm");

	return 0;
}
*/

int main()
{
	IVC *image = vc_read_image("TP1/Car3.ppm");
	IVC *image0 = vc_read_image("TP1/Car3.ppm");
	IVC *image1 = vc_image_new(image->width, image->height, 1, 255);
	IVC *image2 = vc_image_new(image->width, image->height, 1, 1);
	IVC *image3 = vc_image_new(image->width, image->height, 1, 1);
	IVC *image4 = vc_image_new(image->width, image->height, 1, 255);
	IVC *image5 = vc_image_new(image->width, image->height, 1, 1);

	vc_rgb_to_gray(image, image1);
	vc_edges_classic(image1, image2, 55);
	vc_edges_classic(image1, image3, 55);
	vc_fill_edges(image3);
	vc_clean_points(image3, image5);

	int labels, labelsPlate, labelsPlate2;
	OVC *blobs, *blobsPlate, *blobsPlate2, *digits, plate;

	blobs = vc_binary_blob_labelling(image5, image4, &labels);
	vc_binary_blob_info(image4, blobs, labels);
	printf("Number of labels: %d\n", labels);

	plate = vc_blob_plate_selection(image0, blobs, labels);

	IVC *image6 = vc_image_new(plate.width, plate.height, 1, 1);
	IVC *image7 = vc_image_new(image6->width, image6->height, 1, 255);
	IVC *image8 = vc_image_new(image6->width, image6->height, 1, 1);
	IVC *image9 = vc_image_new(image6->width, image6->height, 1, 255);
	vc_obj_to_image(plate, image2, image6);

	blobsPlate = vc_binary_blob_labelling(image6, image7, &labelsPlate);
	vc_binary_blob_info(image7, blobsPlate, labelsPlate);
	printf("Number of labelsPlate: %d\n", labelsPlate);

	vc_blob_to_erase_selection(image7, blobsPlate, labelsPlate);

	vc_gray_binary(image7, image8, 0);
	vc_fill_edges(image8);

	blobsPlate2 = vc_binary_blob_labelling(image8, image9, &labelsPlate2);
	vc_binary_blob_info(image9, blobsPlate2, labelsPlate2);
	printf("Number of labels after filled: %d\n", labelsPlate2);

	digits = (OVC*) calloc(6, sizeof(OVC));
	vc_blob_find_digits(blobsPlate2, digits, labelsPlate2, 18);
	vc_draw_digits(digits, plate, image0);

	vc_write_image("Images/Generated/Car3_Segmented_Plate.ppm", image0);
	vc_write_image("Images/Generated/Car3_Edges_Classic.pbm", image2);
	vc_write_image("Images/Generated/Car3_Blobs.pgm", image4);
	vc_write_image("Images/Generated/Car3_Plate_Edges.pbm", image6);
	vc_write_image("Images/Generated/Car3_Plate_Blobs.pgm", image8);

	vc_image_free(image);
	vc_image_free(image0);

	vc_image_free(image1);
	vc_image_free(image2);
	vc_image_free(image3);
	vc_image_free(image4);
	vc_image_free(image5);
	vc_image_free(image6);
	vc_image_free(image7);
	vc_image_free(image8);
	vc_image_free(image9);
	
	vc_open_image("Images/Generated/Car3_Edges_Classic.pbm");
	vc_open_image("Images/Generated/Car3_Segmented_Plate.ppm");
	vc_open_image("Images/Generated/Car3_Blobs.pgm");
	vc_open_image("Images/Generated/Car3_Plate_Edges.pbm");
	vc_open_image("Images/Generated/Car3_Plate_Blobs.pgm");
	
	return 0;
}

/*
int main()
{
	IVC *image = vc_read_image("Images/Classic/barbara-sp.pgm");
	IVC *image1 = vc_image_new(image->width, image->height, 1, 255);

	vc_gray_lowpass_median_filter(image, image1, 5);

	vc_write_image("Images/Generated/barbara-sp_blur.pgm", image1);

	vc_image_free(image);
	vc_image_free(image1);

	vc_open_image("Images/Classic/barbara-sp.pgm");
	vc_open_image("Images/Generated/barbara-sp_blur.pgm");


	return 0;
}
*/

/*
    Modelo americano - dimensões de 300 mm x 150 mm - usado na maioria dos países da América;
    Modelo europeu - dimensões de 520 mm x 110/120 mm - usado na maior parte da Europa e nos países ou territórios que estiveram sob domínio europeu;
    Modelo oceânico - dimensões de 372 mm x 135 mm - usado na maior parte dos países da Oceânia e do Pacífico.
*/