/*************************************
 * Lab 5 Exercise 2
 * Name:		  Jerrell Ezralemuel
 * Student No:    A0181002B
 * Lab Group:     08
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common/USFAT.h"

#define DE_NAME_SIZE 11

uint32_t min(uint32_t a, uint32_t b) {
    return a < b ? a : b;
}

void print_as_text(uint8_t *data, int size) {
    for (int i = 0; i < size; i++) {
        printf("%c", data[i]);
    }
}

void print_file(FAT_RUNTIME* rt, uint16_t start_sector, uint32_t file_size) {
    uint8_t buf[S_SECTOR_SIZE];
    uint32_t rem_char = file_size;
    for (uint16_t current_sector = start_sector; 
            current_sector != FE_BAD && current_sector != FE_END; 
            current_sector = rt->fat[current_sector]) {
        read_data_sector(rt->media_f, current_sector, buf);
        print_as_text(buf, min(rem_char, S_SECTOR_SIZE));
        rem_char -= min(rem_char, S_SECTOR_SIZE);
    }
}

bool read_file(FAT_RUNTIME* rt, char filename[]) {
    char sfn[DE_NAME_SIZE];
    to_sfn(filename, sfn);

    FAT_DE *directory = rt->dir_buffer;
    for (int i = 0; i < DE_NUMBER_IN_SECTOR; i++) {
        if (DE_IS_FREE(directory[i]))
            continue;
        if (memcmp(directory[i].name, sfn, DE_NAME_SIZE) != 0)
            continue;

        print_file(rt, directory[i].start_sector, directory[i].file_size);
        return true;
    }
    return false;
}

int main(int argc, char** argv)
{
    FAT_RUNTIME fat_rt;
    char filename[13];

    if (argc < 2 ){
        printf("Usage: %s <image_file_name>\n", argv[0]);
        return -1;
    }

    //Load USFAT-based file system from a "disk media"
    if (init_runtime_from_media(&fat_rt, argv[1]) == -1){
        printf("Unable to initialize USFAT from \"%s\".\n",argv[1]);
        return 1;
    }

    //Set current directory to root
    set_directory_to(&fat_rt, FATFS_ROOT_SECTOR);

    print_directory(fat_rt.dir_buffer);
    printf("Read File (\"DONE\" to quit) > ");
    while(scanf("%s", filename) != 0){

        if(!strcmp(filename, "DONE")) 
            break;

        if(!read_file( &fat_rt, filename)){
            printf("\"%s\" not found!\n", filename);
        } else {
            printf("\n");   //additional newline for readability
        }

        print_directory(fat_rt.dir_buffer);
        printf("Read File (\"DONE\" to quit) > ");
    }

    return 0;
}
