
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    char *stego_image_fname; // name of stego image
    FILE *fptr_stego_image;// stego file pointer

    char *secret_fname; // name of secret file
    FILE *fptr_secret;// secret file pointer

    int extn_size; // to store extension size
    int magic_size;// to store magic string size
    char extn_secret_file[10]; // to store secret message

    //char Magic_string1[100];

    int size_secret_file; // to store secret file size
} DecodeInfo;

/*To check decode arguments*/
Status check_decode_arguments(char *argv[]);

/* TO check validation for decode files*/
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* To call all decode functions inside do_decode*/
Status do_decoding(DecodeInfo *decInfo);

/*To open files for decoding*/
Status open_decode_files(DecodeInfo *decInfo);

/* To decode magic string*/
Status decode_magic_string_size(DecodeInfo *decInfo);


/* To decode magic string*/
Status decode_magic_string(DecodeInfo *decInfo);

/*TO decode secret file extenion size*/
Status decode_secret_extn_size(DecodeInfo *decInfo);

/*To decode secret file extension*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* To decode secret file size*/
Status decode_secret_file_size(DecodeInfo *decInfo);

/* To decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);
/* 8 byte function*/
char decode_byte_from_lsb(char *image_buffer);
/* 32 byte function*/
int decode_int_from_lsb(char *image_buffer);

#endif
