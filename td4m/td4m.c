#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int c = 0;
int z = 0;
int input = 0;
int output = 0;
int pc = 0;
int x = 0;
int y = 0;
char rom[256][9];
char ram[256][5];

//функция перевода из двоичной системы в десятичную
int convert(int a){
    int dec = 0;
    int base = 1;
    while (a > 0){
	int n = a % 10;
	a /= 10;
	dec += n * base;
	base *= 2;
    }
    return dec;
}

//функция перевода из десятичной в двоичную
char* Bin(int a, int byte){
    char* binary = malloc(byte);
    memset(binary, 0, byte);
    int mas;
    char m[2];
    while (a > 0){
        mas = a % 2; 
        a /= 2;
        sprintf(m, "%d", mas);
        strcat(binary, m);
        byte--;
    }
    
    //дополнение нулями до заданной битности
    if (byte > 0){
        char res[byte];
        memset(res, '0', byte);
        res[byte] = '\0';
        strcat(binary, res);
    }
    
    int length = strlen(binary);
    int temp = 0;
    int i, j;
    //разворот строки
    for (i = 0, j = length - 1; i < j; i++, j--){
        temp = binary[i];
        binary[i] = binary[j];
        binary[j] = temp;
    }
    
    return binary;
}

void compilation(FILE* fd){
    char buf[15];
    char res[9];
    char Im[5];
    int index = 0;
    while (fgets(buf, sizeof(buf), fd) != NULL){
        buf[strcspn(buf, "\r\n")] = 0;
        if ((strncmp(buf, "ADD A,", 6)) == 0){
            if (strlen(buf) == 10){
                strcpy(Im, buf+6);
                strcpy(res, "0000");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
                
            else if (strlen(buf) == 7){
                strcpy(rom[index++],"10000000");
                rom[index][8] = '\0';
            }
                
            else if (strlen(buf) == 12){
                strcpy(Im, buf+8);
                strcpy(res, "0001");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
        }
            
        else if (strncmp(buf, "MOV A,", 6) == 0){
            if (strlen(buf) == 7){
                strcpy(rom[index++],"00010000");
            rom[index][8] = '\0';
            }
            
            else if (strlen(buf) == 10){
                strcpy(Im, buf+6);
                strcpy(res, "0011");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
        }
            
        else if ((strncmp(buf, "IN A", 4)) == 0){
            if (strlen(buf) == 4){
                strcpy(rom[index++],"00100000");
                rom[index][8] = '\0';
            }
            
            else if (strlen(buf) == 9){
                strncpy(Im, buf+5, 4);
                strcpy(res, "0010");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
        }   
        
        else if (strncmp(buf, "MOV B,", 6) == 0){
            if (strlen(buf) == 7){
                strcpy(rom[index++],"01000000");
            rom[index][8] = '\0';
            }
            
            else if (strlen(buf) == 10){
                strcpy(Im, buf+6);
                strcpy(res, "0111");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
        }
        
        if ((strncmp(buf, "ADD B,", 6)) == 0){
            if (strlen(buf) == 10){
                strcpy(Im, buf+6);
                strcpy(res, "0101");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
                
            else if (strlen(buf) == 12){
                strcpy(Im, buf+8);
                strcpy(res, "0100");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
        }
        
        else if ((strncmp(buf, "IN B", 4)) == 0){
            if (strlen(buf) == 4){
                strcpy(rom[index++],"01100000");
                rom[index][8] = '\0';
            }
            
            else if (strlen(buf) == 9){
                strncpy(Im, buf+5, 4);
                strcpy(res, "0110");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
        }
        
        else if (strcmp(buf, "NEG A") == 0){
            strcpy(rom[index++],"10000001");
            rom[index][8] = '\0';
        }
        
        else if (strcmp(buf, "NOT A") == 0){
            strcpy(rom[index++],"10000010");
            rom[index][8] = '\0';
        }
        
        else if (strcmp(buf, "OR A,B") == 0){
            strcpy(rom[index++],"10000011");
            rom[index][8] = '\0';
        }
        
        else if (strcmp(buf, "AND A,B") == 0){
            strcpy(rom[index++],"10000100");
            rom[index][8] = '\0';
        }
        
        else if (strcmp(buf, "XOR A,B") == 0){
            strcpy(rom[index++],"10000101");
            rom[index][8] = '\0';
        }
        
        else if (strcmp(buf, "SUB A,B") == 0){
            strcpy(rom[index++],"10000110");
            rom[index][8] = '\0';
        }
        
        else if (strcmp(buf, "OUT A") == 0){
            strcpy(rom[index++],"10000111");
            rom[index][8] = '\0';
        }
        
        else if (strcmp(buf, "LD A") == 0){
            strcpy(rom[index++],"10001000");
            rom[index][8] = '\0';
        }
        
        else if (strcmp(buf, "ST A") == 0){
            strcpy(rom[index++],"10001001");
            rom[index][8] = '\0';
        }
        
        else if (strcmp(buf, "LD B") == 0){
            strcpy(rom[index++],"10001010");
            rom[index][8] = '\0';
        }
        
        else if (strcmp(buf, "ST B") == 0){
            strcpy(rom[index++],"10001011");
            rom[index][8] = '\0';
        }
        
        else if (strncmp(buf, "MOV X,", 6) == 0){
            if (strlen(buf) == 7){
                strcpy(rom[index++],"10001100");
            rom[index][8] = '\0';
            }
            
            else if (strlen(buf) == 10){
                strcpy(Im, buf+6);
                strcpy(res, "1100");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
        }
        
        else if (strncmp(buf, "MOV Y,", 6) == 0){
            if (strlen(buf) == 7){
                strcpy(rom[index++],"10001101");
            rom[index][8] = '\0';
            }
            
            else if (strlen(buf) == 10){
                strcpy(Im, buf+6);
                strcpy(res, "1101");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
        }
        
        else if (strcmp(buf, "INC XY") == 0){
            strcpy(rom[index++],"10001110");
        }
        
        else if (strncmp(buf, "JMP", 3) == 0){
            if (strlen(buf) == 6){
                strcpy(rom[index++],"10001111");
                rom[index][8] = '\0';
            }
            
            else if (strlen(buf) == 8){
                strcpy(Im, buf+4);
                strcpy(res, "1111");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
        }
        
        else if (strncmp(buf, "OUT B", 5) == 0){
            if (strlen(buf) == 5){
                strcpy(rom[index++],"10010000");
                rom[index][8] = '\0';
            }
            
             else if (strlen(buf) == 10){
                strcpy(Im, buf+6);
                strcpy(res, "1001");
                strcat(res, Im);
                strcpy(rom[index++], res);
                rom[index][8] = '\0';
            }
        }
        
        else if (strncmp(buf, "JZ", 2) == 0){
            strncpy(Im, buf+3, 4);
            strcpy(res, "1010");
            strcat(res, Im);
            strcpy(rom[index++], res);
            rom[index][8] = '\0';
            
        }
        
        else if (strncmp(buf, "JNC", 3) == 0){
            strncpy(Im, buf+4, 4);
            strcpy(res, "1110");
            strcat(res, Im);
            strcpy(rom[index++], res);
            rom[index][8] = '\0';
        }  
        
        else if (strncmp(buf, "OUT", 3) == 0){
            if (strlen(buf) == 8){
                strncpy(Im, buf+4, 4);
              strcpy(res, "1011");
              strcat(res, Im);
              strcpy(rom[index++], res);
              rom[index][8] = '\0';
            }
        }
    }
}

int main(int argc, char* argv[]){
    char code_file[256], ram_file[256], rom_file[256];
    strcpy(code_file, argv[1]);
    strcpy(ram_file, argv[2]);
    if (argc == 4){
        strcpy(ram_file, argv[3]);
    }

    else if (argc < 3 || argc > 4){
        perror("Указано неверное количество аргументов");
        exit(1);
    }
    
    FILE* fd = fopen(code_file, "r");
    int j = 0;
    compilation(fd);
    fclose(fd);
    for (int i = 0; i < 256; i++)
        printf("%d: %s\n", i, rom[i]);
    return 0;
}
