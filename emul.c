#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int pc = 0; //Счётчик
int c = 0; //Флаг переполнения
int A = 0; //Регистр А
int B = 0; //Регистр В
int out = 0; //Вывод

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

//Вывод значений на каждом ходу
void new_print(int A, int B, int c, int out, int pc, char* command){
    printf("A: %s\tB: %s\tc: %d\tPC: %s\tКоманда: %s\tOUT: %s\n", Bin(A, 4), Bin(B, 4), c, Bin(pc, 4), command, Bin(out, 4));
}

//сложение двух чисел
int addition(int A, int Im){
    int res = A + convert(Im);
    if (res > 15){
	c = 1;
	return res - 16;
    }
    else
	c = 0;
	return res;
}

//выполнение команды
void com(char code[]){
    char cd[8];
    memset(cd, 0, sizeof(cd));
    char buf[5];
    memset(buf, 0, sizeof(buf));
    memcpy(cd, code + pc * 8, 8);
    memcpy(buf, cd + 4, 4);
    if (strncmp(cd, "0000", 4) == 0){
	char* endptr;
	int num = strtol(cd + 4, &endptr, 10); 
	char command[10] = "ADD A,";
	strcat(command, buf);
	new_print(A, B, c, out, pc, command);
	A = addition(A, num);
	pc++;
    }

    else if (strncmp(cd, "0101", 4)  == 0){
	char* endptr;
        int num = strtol(cd + 4, &endptr, 10);
        char command[10] = "ADD B,";
	strcat(command, buf);
        new_print(A, B, c, out, pc, command);
        B = addition(B, num);
        pc++;
    }

    else if (strncmp(cd, "0011", 4) == 0){
        char* endptr;
        int num = strtol(cd + 4, &endptr, 10);
        char command[10] = "MOV A,";
	strcat(command, buf);
        new_print(A, B, c, out, pc, command); 
	A = num;
	pc++;
	c = 0;
    }

    else if (strncmp(cd, "0111", 4)  == 0){
        char* endptr;
        int num = strtol(cd + 4, &endptr, 10);
        char command[10] = "MOV B,";
	strcat(command, buf);
        new_print(A, B, c, out, pc, command); 
        B = num;
        pc++;
	c = 0;
    }
	
    else if (strncmp(cd, "0001", 4)  == 0){
        new_print(A, B, c, out, pc, "MOV A,B");
        A = B;
        pc++;
        c = 0;
    }

    else if (strncmp(cd, "0100", 4)  == 0){
        new_print(A, B, c, out, pc, "MOV B,A");
        B = A;
        pc++;
	c = 0;
    }

    else if (strncmp(cd, "0010", 4)  == 0){
        new_print(A, B, c, out, pc, "IN A");
        printf("Введите значение регистра А: ");
        scanf("%d", &A);
        A = convert(A);
        pc++;
	c = 0;
    }

    else if (strncmp(cd, "0110", 4)  == 0){
        new_print(A, B, c, out, pc, "IN B");
        printf("Введите значение регистра B: ");
        scanf("%d", &B);
        B = convert(B);
        pc++;
	c = 0;
    }

    else if (strncmp(cd, "1001", 4)  == 0){
        new_print(A, B, c, out, pc, "OUT B");
        out = B;
        pc++;
	c = 0;
    }

    else if (strncmp(cd, "1011", 4)  == 0){
        char* endptr;
        int num = strtol(cd + 4, &endptr, 10);
        char command[10] = "OUT ";
	strcat(command, buf);
        new_print(A, B, c, out, pc, command);
        out = num;
        pc++;
	c = 0;
    }

    else if (strncmp(cd, "1111", 4)  == 0){
        char* endptr;
        int num = strtol(cd + 4, &endptr, 10);
        char command[10] = "JMP ";
	strcat(command, buf);
        new_print(A, B, c, out, pc, command);
        pc = convert(num);
	c = 0;
    }

    else if (strncmp(cd, "1110", 4)  == 0){
        char* endptr;
        int num = strtol(cd + 4, &endptr, 10);
        char command[10] = "JNC ";
	strcat(command, buf);
        new_print(A, B, c, out, pc, command);
        if (c == 0)
	   pc = convert(num);
	else{
	   pc++;
	   c = 0;
	}
    }

    else{
	new_print(A, B, c, out, pc, "NOP");
	pc++;
	c = 0;
    }
}

//автоматический режим
void auto_mode(char code[], int herz){
    while(1){
	if (pc > 15)
	    break;
	sleep(1/herz);
	com(code);
    }
}

//ручной режим
void manual_mode(char code[]){
    char m[1];
    while(1){
	if (pc > 15)
	    break;
	scanf("%c", m);
	if (strcmp(m, "p") == 0){
	    printf("Пауза...для продолжения нажмите Enter\n");
	    scanf("%c", m);
	    continue;
	}
	com(code);
    }
}

int main(int argc, char argv[]){
    //считывание программы с бинарника
    FILE* fd = fopen("./prog.bin", "rb");
    if (fd ==0){
	perror("Ошибка чтения файла программы");
	exit(1);
    }

    char code[128];
    int r = 0;
    int c;
    while((c = getc(fd)) != EOF){
        strcat(code, Bin(c, 8));
    }

    fclose(fd);
    
    //Выбор режима
    printf("Выберите режим запуска:\n1 - ручной\n2 - автоматический\nВвод: ");
    scanf("%d", &r);
    if (r == 1)
        manual_mode(code);
    else if (r == 2){
        printf("Выберите частоту 1 ГЦ или 10 Гц\nВвод: ");
        int herz = 0;
        scanf("%d", &herz);
	auto_mode(code, herz);
    }
    
    return 0;
}
