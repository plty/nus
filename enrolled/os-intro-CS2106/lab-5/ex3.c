/*************************************
 * Lab 5 Exercise 3
 * Name:        Jerrell Ezralemuel
 * Student No:  A0181002B
 * Lab Group:   08 
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common/USFAT.h"

#define DE_NAME_SIZE 11
//Import a file content into USFAT based filesystem

int min(int a, int b) {
    return a < b ? a : b;
}

int import_file(FAT_RUNTIME* rt, char* filename, uint16_t try_sector) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        return -1;
    off_t file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    // check is name unique
    char sfn[DE_NAME_SIZE];
    to_sfn(filename, sfn);
    FAT_DE *directory = rt->dir_buffer;
    for (int i = 0; i < DE_NUMBER_IN_SECTOR; i++) {
        if (DE_IS_FREE(directory[i]))
            continue;
        if (memcmp(directory[i].name, sfn, DE_NAME_SIZE) == 0)
            return -1;
    }

    // check is free space exist
    int n = FATFS_FAT_ENTRY_NUMBER;
    bool free_space_exist = false;
    for (int i = 0; i < n; i++)
        free_space_exist |= rt->fat[i];

    if (!free_space_exist)
        return -1;

    // Find empty DE
    int id = 0;
    while (!DE_IS_FREE(directory[id]))
        id++;
    FAT_DE *de = &directory[id];

    memcpy(de->name, sfn, DE_NAME_SIZE);
    de->attribute = DE_ATTR_READ;
    de->file_size = (int) file_size;

    int remaining_size = (int) file_size;

    // Find free space
    int start_sector = try_sector;
    while(rt->fat[start_sector % n] != FE_FREE) 
        start_sector++;
    start_sector %= n;

    de->start_sector = start_sector;

    uint8_t buf[S_SECTOR_SIZE];
    rt->fat[start_sector] = FE_END;
    read(fd, buf, min(remaining_size, S_SECTOR_SIZE));
    remaining_size -= min(remaining_size, S_SECTOR_SIZE);
    write_data_sector(rt->media_f, start_sector, buf);


    int prev = start_sector;
    for (int k = 0, i = (start_sector + 1) % n; k < n && remaining_size > 0; k++, i = (i + 1) % n) {
        if (rt->fat[i] != FE_FREE) {
            continue;
        }
        rt->fat[prev] = i;

        int write_size = min(remaining_size, S_SECTOR_SIZE);
        read(fd, buf, write_size);
        remaining_size -= write_size;
        write_data_sector(rt->media_f, i, buf);
        rt->fat[i] = FE_END;

        prev = i;
    }

    int written_size = file_size - remaining_size;
    de->file_size = written_size;

    write_fat(rt->media_f, rt->fat);
    write_data_sector(rt->media_f, FATFS_ROOT_SECTOR, (uint8_t*) rt->dir_buffer);
    return written_size;
}


int main(int argc, char** argv) {
    FAT_RUNTIME fat_rt;
    char filename[13];
    int result;

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
    printf("Import File (\"DONE\" to quit) > ");
    while(scanf("%s", filename) != 0){

        if(!strcmp(filename, "DONE")) break;

        int start_sector;

        printf("Start sector (in Hex) > 0x");
        scanf("%x", &start_sector);

        printf("Import \"%s\" to [0x%04x] Data Sector...", filename, start_sector);

        if((result = import_file(&fat_rt, filename, start_sector)) == -1){
            printf("FAILED!\n");
        } else {
            printf("Written %d bytes.\n", result);
        } 

        print_directory(fat_rt.dir_buffer);
        printf("\nImport File (\"DONE\" to quit) > ");
    }

    return 0;
}
