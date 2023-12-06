#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;
int main(int argc, char *argv[])
{
    BYTE buffer[512];

    //ensure only one argument
    if (argc != 2)
    {
        printf("./recover memory_card");
        return 1;

    }
    //remember memory name
    char *raw_file = argv[1];
    //open the memory card
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", raw_file);
        return 1;
    }
    //initializing variables
    int i = 0;
    int test = 0;
    char *Filename;
    Filename = malloc(8);
    FILE *img;
    //
    while (fread(&buffer, 1, 512, file) == 512)
    {

        //start of jpeg img
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            //first jpeg img
            if (i == 0)
            {
                sprintf(Filename, "%03i.jpg", i);
                img = fopen(Filename, "w");
                test = 1;
                fwrite(&buffer, 1, 512, img);
                i++;

            }
            //middle jpeg img
            else
            {
                //closing the jpeg img
                fclose(img);

                sprintf(Filename, "%03i.jpg", i);
                //opening the new jpeg image
                img = fopen(Filename, "w");
                fwrite(&buffer, 1, 512, img);
                test = 1;
                i++;




            }

        }
        else
        {
            //rest of jpeg img
            if (test == 1)
            {
                fwrite(&buffer, 1, 512, img);


            }
        }

    }
    //close image
    fclose(img);
    //free memory for img name
    free(Filename);
    //close memory
    fclose(file);


}