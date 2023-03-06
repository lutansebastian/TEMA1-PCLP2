#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "structs.h"

#define MAX_COMMAND_LEN            256
#define MAX_LINE_SIZE              256

int compute_arr_size(void *arr, int nr_elem) {
    int i;
    int arr_size = 0;

    for (i = 0; i < nr_elem; i++) {
        head *header = (head *)arr;
        int curr_elem_size = sizeof(head) + header->len;
        arr_size += curr_elem_size;
        arr = (uint8_t *)arr + curr_elem_size;
    }
    
    return arr_size;
}

data_structure *prepare_data_structure(int insert_tip, int value1, int value2,
                                       char *dedicator, char *dedicated) {
    data_structure *data = malloc(sizeof(data_structure));
    
    data->header = malloc(sizeof(head));
    
    data->header->type = insert_tip;
    
    int data_len = 0;
    
    data_len += strlen(dedicator) + 1; /*lungime sting + \0*/
    data_len += strlen(dedicated) + 1; /*lungime sting + \0*/
    
    if (insert_tip == 1) {
        data_len += sizeof(int8_t) + sizeof(int8_t);
    } else if (insert_tip == 2) {
        data_len += sizeof(int16_t) + sizeof(int32_t);
    } else if (insert_tip == 3) {
        data_len += sizeof(int32_t) + sizeof(int32_t);
    }
    
    data->header->len = data_len;
    
    data->data = malloc(data_len * sizeof(uint8_t));
    uint8_t *data_ptr = (uint8_t *)(data->data);
    
    memcpy(data_ptr, dedicator, strlen(dedicator));
    data_ptr += strlen(dedicator);
    *data_ptr = '\0';
    data_ptr++;
    
    if (insert_tip == 1) {
        *(int8_t *)data_ptr = (int8_t)value1;
        data_ptr++;
        *(int8_t *)data_ptr = (int8_t)value2;
        data_ptr++;
    } else if (insert_tip == 2) {
        *(int16_t *)data_ptr = (int16_t)value1;
        data_ptr += sizeof(int16_t);
        *(int32_t *)data_ptr = (int32_t)value2;
        data_ptr += sizeof(int32_t);
    } else if (insert_tip == 3) {
        *(int32_t *)data_ptr = (int32_t)value1;
        data_ptr += sizeof(int32_t);
        *(int32_t *)data_ptr = (int32_t)value2;
        data_ptr += sizeof(int32_t);
    }
    
    memcpy(data_ptr, dedicated, strlen(dedicated));
    data_ptr += strlen(dedicated);
    *data_ptr = '\0';
    
    return data;
}

 int add_last(void **arr, int *len, data_structure *data)
 {
    int index_to_copy = 0;
    if (*len == 0) {
        *arr = malloc(sizeof(head) + data->header->len);
        if (*arr == NULL) {
            return -1;
        }
    } else {
        index_to_copy = compute_arr_size(*arr, *len);
        *arr = realloc(*arr, index_to_copy + sizeof(head) + data->header->len);
        if (*arr == NULL) {
            return -1;
        }
    }
     
    memcpy((uint8_t *)*arr + index_to_copy, data->header, sizeof(head));
    memcpy((uint8_t *)*arr + index_to_copy + sizeof(head), data->data, 
    data->header->len);
     
    (*len)++;

    return 0;
 }

 int add_at(void **arr, int *len, data_structure *data, int index)
 {
    if (index < 0) {
        return -1;
    }

    if (index >= *len) {
        add_last(arr, len, data);
        return 0;
     }
     
    *arr = realloc(*arr, compute_arr_size(*arr, *len) + sizeof(head) + 
    data->header->len);
    
    if (*arr == NULL) {
        return -1;
    }
     
    void *arr_ptr = *arr;
    int i;
    for (i = 0; i < index; i++) {
        head *header = (head *)arr_ptr;
        arr_ptr = (uint8_t *)arr_ptr + sizeof(head);
        arr_ptr += header->len;
     }
     
    int index_to_move = (int)(arr_ptr - *arr);
    int size_to_move = compute_arr_size(*arr, *len) - index_to_move;
    memmove(arr_ptr + sizeof(head) + data->header->len, arr_ptr, size_to_move);
    memcpy(arr_ptr, data->header, sizeof(head));
    arr_ptr += sizeof(head);
    memcpy(arr_ptr, data->data, data->header->len);
     
    (*len)++;
     
    return 0;
 }

void find(void *data_block, int len, int index)
{
    int i;
    int value1 = 0;
    int value2 = 0;

    if ((index > len) || (index < 0)) {
        return;
    }
    
    for (i = 0; i < len; i++) {
        head *header = (head *)data_block;
        if (i == index) {
            printf("Tipul %d\n", header->type);
        }
        data_block = (uint8_t *)data_block + sizeof(head);
       
        char *dedicator = (char *)data_block;
        data_block = (uint8_t *)data_block + strlen(dedicator) + 1;
        
        if (header->type == 1) {
            value1 = (int)*((int8_t *)data_block);
            data_block = (uint8_t *)data_block + 1;
            value2 = (int)*((int8_t *)data_block);
            data_block = (uint8_t *)data_block + 1;
        } else if (header->type == 2) {
            value1 = (int)*((int16_t *)data_block);
            data_block = (int16_t *)data_block + 1;
            value2 = (int)*((int32_t *)data_block);
            data_block = (int32_t *)data_block + 1;
        } else if (header->type == 3) {
            value1 = (int)*((int32_t *)data_block);
            data_block = (int32_t *)data_block + 1;
            value2 = (int)*((int32_t *)data_block);
            data_block = (int32_t *)data_block + 1;
        }
        
        char *dedicated = (char *)data_block;
        data_block = (uint8_t *)data_block + strlen(dedicated) + 1;
       
        if (i == index) {
            printf("%s pentru %s\n", dedicator, dedicated);
            printf("%d\n%d\n\n", value1, value2);
        }
    }
}

 int delete_at(void **arr, int *len, int index)
 {
    if (*len == 0) {
        return -1;
    }
     
    if ((index < 0) || (index > *len)) {
        return -1;
    }
     
    if (*len == 1) {
     	free(*arr);
        (*len)--;
        return 0;
    }
     
    void *arr_ptr = *arr;
    int i;
    for (i = 0; i < index; i++) {
        head *header = (head *)arr_ptr;
        arr_ptr = (uint8_t *)arr_ptr + sizeof(head);
        arr_ptr += header->len;
    }
     
    head *index_header = (head*)arr_ptr;
     
    int index_size = sizeof(head) + index_header->len;
    int arr_size = compute_arr_size(*arr, *len);
    int index_to_move = (int)(arr_ptr - *arr) + index_size;
    int size_to_move = arr_size - index_to_move;
     
    if (size_to_move) {
        memmove(arr_ptr, (uint8_t *)arr_ptr + index_size, size_to_move);
    }
     
    *arr = realloc(*arr, arr_size - index_size);
    if (*arr == NULL) {
        return -1;
    }
     
    (*len)--;

    return 0;
}

void print(void *arr, int len) {
    int i;
    int value1 = 0;
    int value2 = 0;
     
    for (i = 0; i < len; i++) {
        head *header = (head *)arr;
        printf("Tipul %d\n", header->type);
        arr = (uint8_t *)arr + sizeof(head);
        
        char *dedicator = (char *)arr;
        arr = (uint8_t *)arr + strlen(dedicator) + 1;
         
        if (header->type == 1) {
            value1 = (int)*((int8_t *)arr);
            arr = (uint8_t *)arr + 1;
            value2 = (int)*((int8_t *)arr);
            arr = (uint8_t *)arr + 1;
        } else if (header->type == 2) {
            value1 = (int)*((int16_t *)arr);
            arr = (int16_t *)arr + 1;
            value2 = (int)*((int32_t *)arr);
            arr = (int32_t *)arr + 1;
        } else if (header->type == 3) {
            value1 = (int)*((int32_t *)arr);
            arr = (int32_t *)arr + 1;
            value2 = (int)*((int32_t *)arr);
            arr = (int32_t *)arr + 1;
        }
         
        char *dedicated = (char *)arr;
        arr = (uint8_t *)arr + strlen(dedicated) + 1;
        
        printf("%s pentru %s\n", dedicator, dedicated);
         
        printf("%d\n%d\n\n", value1, value2);
    }
}

int main() {
    void *arr = NULL;
    int len = 0;

    char *line = malloc(MAX_LINE_SIZE * sizeof(char));

    while (1) {
        fgets(line, MAX_LINE_SIZE, stdin);
        line[strlen(line) - 1] = '\0';

        char *p = strtok(line, " ");

        if (strcmp(p, "exit") == 0) {
            break;
        }

        else if (strcmp(p, "print") == 0) {
            print(arr, len);
        }

        else if (strncmp(p, "insert_at", strlen("insert_at")) == 0) {
            int insert_tip, value1, value2, index;
            
            char *dedicator = malloc(MAX_COMMAND_LEN * sizeof(char));
            char *dedicated = malloc(MAX_COMMAND_LEN * sizeof(char));
            
            p = strtok(NULL, " ");
            index = atoi(p);
            
            p = strtok(NULL, " ");
            insert_tip = atoi(p);

            p = strtok(NULL, " ");
            strcpy(dedicator, p);

            p = strtok(NULL, " ");
            value1 = atoi(p);

            p = strtok(NULL, " ");
            value2 = atoi(p);

            p = strtok(NULL, " ");
            strcpy(dedicated, p);

            data_structure *data = prepare_data_structure(insert_tip, value1, 
            value2, dedicator, dedicated);
            add_at(&arr, &len, data, index);
                        
            free(dedicated);
            free(dedicator);
            free(data->data);
            free(data->header);
            free(data);
            
        }
        
        else if (strncmp(p, "insert", strlen("insert")) == 0) {
            int insert_tip, value1, value2;

            char *dedicator = malloc(MAX_COMMAND_LEN * sizeof(char));
            char *dedicated = malloc(MAX_COMMAND_LEN * sizeof(char));
            
            p = strtok(NULL, " ");
            insert_tip = atoi(p);

            p = strtok(NULL, " ");
            strcpy(dedicator, p);

            p = strtok(NULL, " ");
            value1 = atoi(p);

            p = strtok(NULL, " ");
            value2 = atoi(p);

            p = strtok(NULL, " ");
            strcpy(dedicated, p);

            data_structure *data = prepare_data_structure(insert_tip, value1, 
            value2, dedicator, dedicated);
            add_last(&arr, &len, data);
                        
            free(dedicated);
            free(dedicator);
            free(data->data);
            free(data->header);
            free(data);
        }

        else if (strcmp(p, "find") == 0){
            int find_tip;
            
            p = strtok(NULL, " ");
            find_tip = atoi(p);
            find(arr, len, find_tip);
        }

        else if (strcmp(p, "delete_at") == 0){
            p = strtok(NULL, " ");
            int delete_at_tip = atoi(p);
            delete_at(&arr, &len, delete_at_tip);
        }

    }

    free(line);
    free(arr);

    return 0;
}
