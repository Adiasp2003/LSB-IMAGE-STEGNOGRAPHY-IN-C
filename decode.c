#include <stdio.h>
#include <string.h>
#include "types.h"
#include "decode.h"
#include "encode.h"
// #include "common.h"

// TO check decode arguments: ./a.out -e stego.bmp
Status check_decode_arguments(char *argv[])
{
    if (strstr(argv[2], ".bmp") == NULL)
    {
        printf("Error: Source file must be a .bmp file (stego.bmp)\n");
        return e_failure;
    }
    return e_success;
}

// To read and validate decode arguments such as stego.bmp
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (strstr(argv[2], ".bmp") != NULL)
    {
        printf("stego.bmp present\n");
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        decInfo->stego_image_fname = "stego.bmp";
    }

    if (argv[3] != NULL)
    {
        decInfo->secret_fname = argv[3]; // if given filename.txt by user to store decoded secret data
    }
    else
    {
        decInfo->secret_fname = "decoded.txt"; // else create
    }

    return e_success;
}

// To decode all the encoded data from stego.bmp
Status do_decoding(DecodeInfo *decInfo)
{
    // check for open files
    if (open_decode_files(decInfo) == e_success) 
    {
        printf("File opened to decode\n");
    }
    else
    {
        printf("Files are not opened successfully\n");
        return e_failure;
    }

    fseek(decInfo->fptr_stego_image, 54, SEEK_CUR); // skip 54 bytes of BMP header

    // Decode magic string size
    if (decode_magic_string_size(decInfo) == e_success)
    {
        printf("Magic string size decoded successfully\n");
    }
    else
    {
        printf("Magic string size decoding failed\n");
        return e_failure;
    }

    // Decode magic string
    if (decode_magic_string(decInfo) == e_success)
    {
        printf("Magic string decoded successfully\n");
    }
    else
    {
        printf("Magic string decoding failed\n");
        return e_failure;
    }

    // Decode secret file extension size
    if (decode_secret_extn_size(decInfo) == e_success)
    {
        printf("Secret file extension size decoded\n");
    }

    // Decode secret file extension
    if (decode_secret_file_extn(decInfo) == e_success)
    {
        printf("Secret file extension decoded: %s\n", decInfo->extn_secret_file);
    }

    // Decode secret file size
    if (decode_secret_file_size(decInfo) == e_success)
    {
        printf("Secret file size decoded: %d bytes\n", decInfo->size_secret_file);
    }

    // Decode secret file data
    if (decode_secret_file_data(decInfo) == e_success)
    {
        printf("Secret file data decoded and written to %s\n", decInfo->secret_fname);
    }

    // Close all files after open
    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_secret);

    return e_success;
}

// To Open files i.e stego.bmp
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");

    if (decInfo->fptr_stego_image == NULL)
    {
        printf("Stego file is not present\n");
        return e_failure;
    }
    else
    {
        printf("Stego file is opened\n");
    }
    return e_success;
}

// Decode a single byte from 8 LSBs of 8 bytes
char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0; // initially data is zero
    for (int i = 0; i < 8; i++)
    {
        data = (data << 1) | (image_buffer[i] & 1);// formula to decode 8 bytes from lsb
    }
    return data; // then stored in data
}

// Decode integer (4 bytes) from LSBs // 32 bytes
int decode_int_from_lsb(char *image_buffer)
{
    int value = 0;// initially value is 0
    for (int i = 0; i < 32; i++)
    {
        value = (value << 1) | (image_buffer[i] & 1);// formula to decode 32 bytes from lsb
    }
    return value; // then stored in value
}

// To decode magic string 'SIZE' 
Status decode_magic_string_size(DecodeInfo *decInfo)
{
    char buffer[8]; //Temporary buffer to store 8 bytes read from stego image
    fread(buffer, 1, 8, decInfo->fptr_stego_image); // read from stego image
    decInfo->magic_size = decode_byte_from_lsb(buffer); // store it to magic_size and call 8 byte function
    printf("Magic string size: %d\n",decInfo->magic_size); // print MAGIC STRING SIZE
    return e_success;
}

// Decode magic string
Status decode_magic_string(DecodeInfo *decInfo)
{

    char magic_string[100];// declare magic string with size 100 to avoid stack smashing/segmentation faults
    char buffer[8]; // Temporary buffer to store 8 bytes read from stego image

    for (int i = 0; i < decInfo->magic_size; i++) //read until magic string size declared above
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image); // read 8 bytes from stego image
        magic_string[i] = decode_byte_from_lsb(buffer); // call 8 byte function and store in magic_string[i]
    }
    magic_string[decInfo->magic_size] = '\0'; // Declare null for EOF
    printf("Magic string: %s\n", magic_string); // Print the decoded magic string
    return e_success;
}

// Decode secret file extension size
Status decode_secret_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];// Temporary buffer to store 32 bytes read from stego image
    fread(buffer, 1, 32, decInfo->fptr_stego_image); // read 32 bytes from stego image
    decInfo->extn_size = decode_int_from_lsb(buffer); // call 32 byte function and store in extension size
    printf("Extension size: %d\n",decInfo->extn_size); // Prints EXTENSION size
    return e_success;
}

// Decode secret file extension
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];// Temporary buffer to store 8 bytes read from stego image
    for (int i = 0; i < decInfo->extn_size; i++) // run loop until EXTENSION SIZE times
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image); // read 8 bytes from stego image
        decInfo->extn_secret_file[i] = decode_byte_from_lsb(buffer); // call 8 byte function and store it to extension secret file
    }
    decInfo->extn_secret_file[decInfo->extn_size] = '\0'; //Declare null for EOF
    return e_success;
}
// Decode secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32]; // Temporary buffer to store 32 bytes read from stego image
    fread(buffer, 1, 32, decInfo->fptr_stego_image); // read 32 bytes from stego image
    decInfo->size_secret_file = decode_int_from_lsb(buffer); // call 32 byte functiona and store it in secret file size
    //printf("Decoded file size: %d\n",decInfo->size_secret_file); //prints the decoded file size
    return e_success;
}

// Decode secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w"); // open output file in "w" mode that creates new txt file or overwrites if exisiting

    if (decInfo->fptr_secret == NULL)
    {
        printf("Error opening output file\n");
        return e_failure; // failure is file not opened
    }

    char buffer[8];  // Temporary buffer to store 8 bytes read from stego image
    for (int i = 0; i < decInfo->size_secret_file; i++) // Run a loop until secret file size times
    {
        fread(buffer, 1, 8, decInfo->fptr_stego_image); // read 8 bytes from stego image
        char ch = decode_byte_from_lsb(buffer);// call 32 byte function and store in ch
        fputc(ch, decInfo->fptr_secret); // write decoded character to output secret file
    }

    return e_success; // success for all
}