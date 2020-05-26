#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void my_encrypt( char const * const in_file, char const * const out_file, char const * const key ){

    size_t const key_len = strlen(key);
    if (key_len == 0) {
        fprintf(stderr, "length of key must be greater than 0\n");
        exit( EXIT_FAILURE );
    }

    FILE * in_fh = fopen( in_file, "rb");
    if (in_fh == NULL) {
        fprintf(stderr, "input file %s can not be opened\n", in_file);
        exit( EXIT_FAILURE );
    }

    FILE * out_fh = fopen( out_file, "wb");
    if (out_fh == NULL) {
        fprintf(stderr, "output file %s can not be opened\n", out_file);
        exit( EXIT_FAILURE );
    }


    size_t key_index = 0;
    int cur_int;
    char cur_char, encrypt_char;

    while ( EOF != ( cur_int = fgetc(in_fh) )) {
        cur_char = cur_int;
        encrypt_char = cur_char ^ key[key_index++];
        int rc = fputc(encrypt_char, out_fh);
        if ( rc == EOF ) {
            fprintf(stderr, "output fail\n");
            exit( EXIT_FAILURE );
        }

        if(key_index == key_len){
            key_index = 0;
        }
    }

    fclose( in_fh );
    fclose( out_fh );

}

void usage(){
    fprintf(stderr, "usage: encrypt -i input_file -o output_file\n");
}

int main(int argc, char *argv[])
{
    char  in_file[100], out_file[100];
    in_file[0]='\0';
    out_file[0]='\0';

    char * key, * key1, *key2;
    size_t key_len;

    int opt;

    while ((opt = getopt(argc, argv, "o:i:")) != -1){
        switch(opt){
                break;
            case 'i':
                strcpy( in_file, optarg );
                break;
            case 'o':
                strcpy( out_file, optarg );
                break;
            default:
                fprintf(stderr, "unrecognized opt (%c)\n", opt);
                usage();
                exit(EXIT_FAILURE);

        }

    }

    if (in_file[0] == '\0') {
        fprintf(stderr, "miss i\n");
        usage();
        exit(EXIT_FAILURE);
    }

    if (out_file[0] == '\0') {
        fprintf(stderr, "miss o\n");
        usage();
        exit(EXIT_FAILURE);
    }

    char prompt[1000];
    sprintf( prompt, "Please enter password:");
    key = getpass( prompt );
    key_len = strlen(key);

    if (key_len == 0) {
        fprintf(stderr, "password length must be greater than 0\n");
        exit(EXIT_FAILURE);
    }


    key1 = malloc(sizeof(char) * (strlen(key) + 10 ));
    strcpy( key1, key );

    sprintf( prompt, "Please enter password again:");
    key = getpass( prompt );
    key2 = malloc(sizeof(char) * (strlen(key) + 10 ));
    strcpy( key2, key );

    if ( strcmp(key1,key2) != 0 ) {
        fprintf(stderr, "password must be the same\n");
        exit(EXIT_FAILURE);
    }

    my_encrypt( in_file, out_file, key );

    free(key1);
    free(key2);

    return 0;
}

