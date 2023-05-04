#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void create_pgm(char * file_name, int ** nuance, int width, int height){
    FILE *pgm = fopen(file_name, "w");
    if(pgm == NULL)
    {
        printf("Unable to create pgm file.\n");
        exit(EXIT_FAILURE);
    }
    char title[2] = "P5"; 

    fputs(title,pgm);
    fputs(title,pgm);
    fclose(pgm);


}
void create_ppm(char * file_name, int * R, int * G, int * B){
    FILE *ppm = fopen(file_name, "w");
    if(ppm == NULL)
    {
        printf("Unable to create ppm file.\n");
        exit(EXIT_FAILURE);
    }
    char *title = "P6"; 
   
    fputs(title,ppm);
    fclose(ppm);
}
int main(int argc, char **argv)
{
    /*
    if (argc != 2) {

          #  Si y'a pas au moins un argument en ligne de commandes, on
          #  boude.

        fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);

        return EXIT_FAILURE;
    }
    */
    int * nuance = malloc(sizeof(int) * 2);
    nuance[0] = 1;
    nuance[2] = 4;
    create_pgm("test.pgm",nuance,2,1);
    /* On se congratule. */
    return EXIT_SUCCESS;
}