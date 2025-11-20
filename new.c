/*
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"

int main(int argc, int *argv[])
{
    if (argc == 4)
    {
        if (strcmp(argv[1], "-e") == 0) // to check encoding operation
        {
            printf("You have choosen encoding\n");
            EncodeInfo encInfo;
            if (strstr(argv[2], ".bmp") != NULL) // to find beautiful.bmp file
            {
                printf("Beautiful.bmp PRESENT\n");
                encInfo->src_image_fname = argv[2];
            }
            else
            {
                printf("beautiful.bmp not present\n"); // beautiful.bmp not found
                return e_failure;
            }

            if (strstr(argv[3], ".txt") != NULL) // to find secret.txt file
            {
                {
                    printf("secret.txt PRESENT\n");
                    encInfo->secret_fname = argv[3];
                }

                else
                {
                    printf("secret.txt is not present\n");
                    return e_failure;
                }

                if (strstr(argv[4], ".bmp") != NULL) // to find stego.bmp file
                {
                    {
                        printf("stego.bmp is present\n");
                        encInfo->stego_image_fname = argv[4];
                    }
                    else
                    {
                        encInfo->stego_image_fname = "stego.bmp";
                    }

                    printf("READ and VALIDATION for all done SUCCESSFULLY\n");

                    printf("Now encoding is being checked\n");

                    // Opening all files i.e source,secret,stego

                    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");

                    if (encInfo->fptr_src_image == NULL)
                    {
                        printf("Source file is not present\n");
                        return e_failure;
                    }
                    else
                    {
                        printf("Source file is opened\n");
                    }

                    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");

                    if (encInfo->secret_fname == NULL)
                    {
                        printf("Secret file is not present\n");
                        return e_failure;
                    }
                    else
                    {
                        printf("Secret file is opened\n");
                    }
                    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "r");
                    printf("Stego file is opened\n");

                    printf("All files are OPENED SUCCESSFULLY\n");

                    Status status = encode(&encInfo);

                    // close all files after opened
                    fclose(encInfo.fptr_src_image);
                    fclose(encInfo.fptr_secret);
                    fclose(encInfo.fptr_stego_image);

                    if (status == e_success) // check status for encoding
                    {
                        printf("ENCODING SUCCESS\n");
                    }

                    else // else print failure
                    {
                        printf("ENCODING FAILED :{\n");
                    }
                    return e_success;
                }




                
                else if (strcmp(argv[1], "-d") == 0) // to check decoding operation
                {
                    printf("You have choosen decoding\n");
                }

                else // UNSUPPORTED operation
                {
                    printf("It is neither encoding or decoding\n Unsupported\n");
                    return e_failure;
                }
                return e_success;
            }
            else // argc invalid
            {
                printf("Enter the correct arguments to validate argc =4\n");
                return e_failure;
            }
        }
    }
}
    */