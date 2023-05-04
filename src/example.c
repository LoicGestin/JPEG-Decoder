#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void decode_entete(){
    FILE* file = fopen("../images/invader.jpeg", "r");

   
    fseek(file, 0, SEEK_END);
    unsigned long fileLen=ftell(file);
    char* file_data;
    rewind(file);

    file_data=malloc((fileLen)*sizeof(char));
    if (file_data == NULL){
        printf("Memory error"); exit (2);
    }
    int num_read=0;
    char s;
    int cpt =1;
    while ((num_read = fread(&s, 1, 1, file))) {

        if(s == -1){
            if(num_read = fread(&s, 1, 1, file)){
                if( s == -40){
                    printf("[SOI] marker found\n");
                }
                if( s == -32){
                    printf("[APP0] ");
                    int length = 0;
                    if(num_read = fread(&s, 1, 1, file)){
                        length = s;
                    }
                    if(num_read = fread(&s, 1, 1, file)){
                        length += s;
                    }
                    printf("length (%d) \n", s);
                    printf("JFIF application")
                }

                if( s == -37){
                    printf("[DQT] ");
                }
               
            }
        }

        
        /*
        if(cpt % 16 == 0){
            printf("%02x\n", s & 0xff);
        }
        else if(cpt % 8 == 0){
            printf("%02x  ", s & 0xff);
        }
        else{
             printf("%02x ", s & 0xff);
        }
        strncat(file_data,&s,1);
        cpt += 1;
        */
    }
    fclose(file);
}


void diag(){
    int mat [4][4]={0,1,5,6,2,4,7,13,3,8,12,17,9,11,18,24};
    int row = 4;
    int col = 4;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            printf("%d ", mat[i][j]);
        }
    }



}


int main(int argc, char **argv)
{
    if (argc != 2) {
    	/* 
            Si y'a pas au moins un argument en ligne de commandes, on
    	    boude. 
        */
    	fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
      
    	return EXIT_FAILURE;
    }
    decode_entete();
    diag();
    /* On se congratule. */
    return EXIT_SUCCESS;
}
