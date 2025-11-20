#include <stdio.h>
#include <string.h>
#include "types.h"
#include "encode.h"
// #include "common.h"

// TO check encode arguments: ./a.out -e beautiful.bmp secret.txt stego.bmp
Status check_encode_arguments(char *argv[])
{
    if (strstr(argv[2], ".bmp") == NULL)
    {
        printf("Error: Source file must be a .bmp file (beautiful.bmp)\n");
        return e_failure;
    }
    if (strstr(argv[3], ".txt") == NULL)
    {
        printf("Error: Secret file must be a .txt file (secret.txt)\n");
        return e_failure;
    }
    if (strstr(argv[4], ".bmp") == NULL)
    {
        printf("Error: Output stego file must be a .bmp file (stego.bmp)\n");
        return e_failure;
    }
    return e_success;
}


// To read and validate encode arguments
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if (strstr(argv[2], ".bmp") != NULL) // checks for beautiful.bmp
    {
        printf("beautiful.bmp is present\n");
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("beautiful.bmp not present\n");
        return e_failure;
    }

    if (strstr(argv[3], ".txt") != NULL) // checks for secret.txt
    {

        printf("secret.txt is present\n");
        encInfo->secret_fname = argv[3];
    }
    else
    {
        printf("secret.txt is not present\n");
        return e_failure;
    }

    if (strstr(argv[4], ".bmp") != NULL) // checks for stego.bmp
    {

        printf("stego.bmp is present\n");
        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }

    // to check if file has an extension 
    char *ext = strchr(encInfo->secret_fname, '.');// Try to find the '.' character in the secret file name.
    if (ext != NULL)
    {
        strcpy(encInfo->extn_secret_file, ext);// if found, copy starting from . into extn_secret_file
    }
    else
    {
        printf("Secret file has no extension\n"); // if no extension present
        return e_failure; // failure
    }

    return e_success; // success for all
}

Status do_encoding(EncodeInfo *encInfo)
{
    // open files
    if (open_encode_files(encInfo) == e_success) // pass address of structure :- encInfo
    {
        printf("All files are opened successfully for Encoding\n");
    }
    else
    {
        printf("Files are not opened\nError opening files\n");
        return e_failure; // failure
    }

    // Check capacity
    if (check_capacity(encInfo) == e_success)
    {
        printf("REQUIRED CAPACITY found SUCCESS\n");
    }
    else
    {
        printf("Failed to match capacity ;{\n");
        return e_failure; // failure
    }

    // Copy header (54 bytes)
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success) // fptr_stego_image and fptr_stego_image is the destination pointer
    {
        printf("STORED 54 bytes of BMP header SUCCESS\n");
    }
    else
    {
        printf("ERROR in storing 54 bytes ;{\n");
        return e_failure; // failure
    }

    printf("Enter your own Magic string\n");
    scanf("%s", encInfo->Magic_string);

    // Encode magic string size
    
    if (encode_magic_string_size(strlen(encInfo->Magic_string), encInfo) == e_success)
    {
        printf("Magic string size Encoded\n");
    }
    else
    {
        printf("Error encoding Magic string size\n");
        return e_failure; // failure
    }

    // Encode magic string
    if (encode_magic_string(encInfo->Magic_string, encInfo) == e_success)
    {
        printf("\nMAGIC STRING encoded SUCCESS\n");
    }
    else
    {
        printf("Failed to encode MAGIC STRING :{\n");
        return e_failure; // failure
    }

    // Encode secret extension size
    if (encode_secret_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
    {
        printf("Extension size encoded\n");
    }
    else
    {
        printf("Error encoding secret extension size\n");
        return e_failure; // failure
    }

    // Encode secret file extension
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
    {
        printf("\nEncoded Secret file extension successful\n");
    }
    else
    {
        printf("Failed to encode Secret file etension :{\n");
        return e_failure; // failure
    }

    // Encode secret file size
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
    {
        printf("Secret file size encoded\n");
    }

    // Encode secret data
    if (encode_secret_file_data(encInfo) == e_success)
    {
        printf("Secret data encoded successfully\n");
    }

    // Copy remaining image data
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
    {
        printf("Copied remaining files to Destination/stego.bmp\n");
    }
    else
    {
        printf("Failed to copy remaining data to Destination/stego:{\n");
        return e_failure; // failure
    }

    // close after opening the files
    fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_stego_image);

    return e_success; // success for all
}

// to open encode files i.e beautiful.bmp, secret.txt and stego.bmp
Status open_encode_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r"); // open beautiful.bmp

    if (encInfo->fptr_src_image == NULL)
    {
        printf("Source file not present\n");
        return e_failure; // failure
    }
    else
    {
        printf("Source file opened\n");
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r"); // open secret.txt

    if (encInfo->fptr_secret == NULL)
    {
        printf("Secret file not present\n");
        return e_failure; // failure
    }
    else
    {
        printf("Secret file opened\n");
    }
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w"); // open stego.bmp in write mode
    if (encInfo->fptr_stego_image == NULL)
    {
        printf("Error creating stego image\n");
        return e_failure; // failure
    }
    else
    {
        printf("Stego file opened\n");
    }

    return e_success; // success for all
}

// To check if stego image has the capacity to hold secret data
Status check_capacity(EncodeInfo *encInfo)
{

    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image); // call get_size_of_bmp function and store

    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret); // call get file size function and store

    if (encInfo->image_capacity > (54 + (2 * 4 * 4 * 4 * 4 * (encInfo->size_secret_file)) * 8)) // check capacity formula
    {
        printf("Image has REQUIRED CAPACITY to store encode secret file\n");
    }
    else
    {
        printf("FAILURE :{\nImage does not have the required capacity to encode secret file data\n");
        return e_failure; // failure
    }
    return e_success; // success
}

uint get_image_size_for_bmp(FILE *fptr_image) // image size
{
    // width * height * pixels(3)(for RGB)
    uint width;
    uint height;

    // Move to width( offset to 18)
    fseek(fptr_image, 18, SEEK_SET); // same but different variable name.
    // For funtion call fptr_src_image name is used, but while receiving fptr_image is used.

    fread(&width, sizeof(int), 1, fptr_image);
    printf("Width = %u\n", width);

    // Move to height( offset to 22)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("Height = %u\n", height);

    return width * height * 3; // returns total size of the source image
}

uint get_file_size(FILE *fptr) // secret file size
{
    uint file_size;
    fseek(fptr, 0, SEEK_END); // same but used of different variable name
    file_size = ftell(fptr);  // to find the position
    rewind(fptr);
    return file_size; // stored in a new_variable as we cannot return fptr due to return type mismatch/ datatype and return type should match
}

// Copy First 54 bytes of BMP header
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char arr_header[54];                        // BMP header with 54bytes
    fseek(fptr_src_image, 0, SEEK_SET);         // to go towards beginning of source file // use rewind
    fread(arr_header, 54, 1, fptr_src_image);   // read 54 bytes from source file
    fwrite(arr_header, 54, 1, fptr_dest_image); // write  54 bytes to destination file
    return e_success;
}

// To encode 8 bytes to lsb
Status encode_byte_to_lsb(char data, FILE *src, FILE *dest)
{
    printf("%c", data);

    char buffer[8];
    fread(buffer, 8, 1, src);

    for (int i = 0; i < 8; i++)
    {
        buffer[i] = (buffer[i] & 0xFE) | ((data >> (7 - i)) & 1); // formula to encode 8 bytes to lsb
    }

    fwrite(buffer, 8, 1, dest);
    return e_success;
}

// To encode 32 bytes to lsb
Status encode_size_to_lsb(int data, FILE *src, FILE *dest) // size = 32
{
    printf("%d\n", data);
    char buffer[32];           // image_buffer of size 32
    fread(buffer, 32, 1, src); // read from file source -> beautiful.bmp
    for (int i = 0; i < 32; i++)
    {
        buffer[i] = (buffer[i] & 0xFE) | ((data >> (31 - i)) & 1); // formula to encode 32 bytes to lsb
    }

    fwrite(buffer, 32, 1, dest); // write to the destination -> stegobmp
    return e_success;            // success
}
// To Encode magic string size entered by user
Status encode_magic_string_size(int magic_size, EncodeInfo *encInfo)
{
    encode_byte_to_lsb(magic_size, encInfo->fptr_src_image, encInfo->fptr_stego_image);
    return e_success;
}

// To Encode magic string
Status encode_magic_string(const char *Magic_string, EncodeInfo *encInfo)
{
    for (int i = 0; i < strlen(Magic_string); i++) // run a loop until magic_string length
    {
        encode_byte_to_lsb(Magic_string[i], encInfo->fptr_src_image, encInfo->fptr_stego_image); // call byte to lsb i.e 8 byte encoding function
    }
    return e_success; // success
}

// To Encode secret file extension size -> where extension size for .txt is (4)
Status encode_secret_extn_size(int extn_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(extn_size, encInfo->fptr_src_image, encInfo->fptr_stego_image); // call size to lsb i.e 32 byte encoding function
    return e_success;                                                                  // success
}

// To Encode secret file extension -> (.txt)
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    for (int i = 0; i < strlen(file_extn); i++) // run a loop for .txt times -> 4 times
    {
        encode_byte_to_lsb(file_extn[i], encInfo->fptr_src_image, encInfo->fptr_stego_image); // call 32 byte encoding function
    }
    return e_success; // success
}

// To Encode secret file size (size of the file)
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(file_size, encInfo->fptr_src_image, encInfo->fptr_stego_image); // call 32 byte encode function

    return e_success; // success
}

// To Encode secret file data (the data present inside the secret file) -> My password is SECRET ;)
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo->fptr_secret); // moves/points to the beginning of the secret file data
    char ch;
    while ((ch = fgetc(encInfo->fptr_secret)) != EOF) // check character != End of file
    {
        encode_byte_to_lsb(ch, encInfo->fptr_src_image, encInfo->fptr_stego_image); // call 32 byte encode function
    }
    return e_success; // success
}

// To Copy remaining data from source to destination(stego.bmp)
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while ((ch = getc(fptr_src)) != EOF) // check character != End of file
    {
        putc(ch, fptr_dest); // feeds it into the destination file(stego.bmp)
    }
    return e_success; // success
}
