/*
NAME :- Aditya K
ID :- 25021_260
PROJECT NAME:- LSB Image Stegnography
DESCRIPTION:-
LSB image steganography is a simple way to hide secret data
inside an image by changing the last bit of each pixel’s color
value. These tiny changes are invisible to the human eye, so
the image looks the same even though it contains hidden information.
During decoding, those bits are read back to recover the secret message or file.
It works best with uncompressed formats like BMP or PNG, making it an easy and effective
method for basic data hiding.
APPLICATIONS:-
* Secure communication
* Digital watermarking
* Data integrity verification
* Covert data storage
* Intelligence and forensics
* Educational and research purposes
*/

#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    if (argc >= 3) // check for user entered arguments
    {
        if (check_operation_type(argv) == e_encode)
        {
            

            EncodeInfo encInfo;
            if (check_encode_arguments(argv) == e_success)
            {
                printf("You have choosen encoding\n");
                if (read_and_validate_encode_args(argv, &encInfo) == e_success)
                {
                    printf("Read and validate is successful\n");

                    if (do_encoding(&encInfo) == e_success)
                    {
                        printf("Encoding Successful\n");
                    }
                    else
                    {
                        printf("Encoding Unsuccessful\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Read and validate is unsuccessfull\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Arguments do not match.\nEnter the correct arguments to decode {./a.out -e beautiful.bmp secret.txt stego.bmp}\n");
                return e_failure;
            }
        }

        else if (check_operation_type(argv) == e_decode)
        {

            DecodeInfo decInfo;
            if (check_decode_arguments(argv) == e_success)
            {
                printf("You have choosen decoding\n");
            if (read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                
                printf("Read and validate is successful\n");
                if (do_decoding(&decInfo) == e_success)
                {
                    printf("Decoding Successful\n");
                }
                else
                {
                    printf("Decoding Unsuccessful\n");
                    return e_failure;
                }
            }
        }
        else
        {
            printf("Arguments do not match.\nEnter the correct arguments to decode {./a.out -d stego.bmp}\n");
            return e_failure;
        }
        }
        else
        {
            printf("It is unsupported\n");
            return e_failure;
        }
        return e_success;
    }
    else // argc invalid
    {
        printf("Enter the correct arguments to validate as you have only entered %d arguments\n", argc);
        return e_failure;
    }
}

// To check either encode or decode
OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0) // for encoding
    {
        return e_encode;
    }

    else if (strcmp(argv[1], "-d") == 0) // for decoding
    {
        return e_decode;
    }

    else // else unsupported
    {
        return e_unsupported;
    }
}
