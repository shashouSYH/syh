#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

typedef char KeyType[64];

typedef struct{
    int count[2];
} Datatype;

typedef struct ElemType{
    Datatype data;
    KeyType key;
    struct ElemType *next;
} Elemtype;

typedef struct{
    Elemtype *elem[43];
    int len;
} HashTable;

const char *KeyWords[32] ={
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof",
    "static", "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};

int Hash(const char *key)
{
    int len;
    len = (int)strlen(key);
    if (len == 0){
        return 0;
    }
    return ((unsigned char)key[0] * 100 + (unsigned char)key[len - 1]) % 41;
}

void InitHashTable(HashTable *HT)
{
    int i;
    for (i = 0; i < 43; i++){
        HT->elem[i] = NULL;
    }
    HT->len = 0;
}

Elemtype *SearchHash(HashTable *HT, const char *key)
{
    int h;
    Elemtype *p;
    h = Hash(key);
    p = HT->elem[h];
    while (p != NULL){
        if (strcmp(p->key, key) == 0){
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int InsertHash(HashTable *HT, const char *key)
{
    int h;
    Elemtype *p;
    if (SearchHash(HT, key) != NULL){
        return 1;
    }
    p = (Elemtype *)malloc(sizeof(Elemtype));
    if (p == NULL){
        return 0;
    }
    h = Hash(key);
    strcpy(p->key, key);
    p->data.count[0] = 0;
    p->data.count[1] = 0;
    p->next = HT->elem[h];
    HT->elem[h] = p;
    HT->len++;
    return 1;
}

int CreateKeyWordTable(HashTable *HT)
{
    int i;
    int pos;
    InitHashTable(HT);
    for (i = 0; i < 32; i++){
        pos = InsertHash(HT, KeyWords[i]);
        if (pos == 0){
            return 0;
        }
    }
    return 1;
}

void CountWord(HashTable *HT, const char *word, int fileNo)
{
    Elemtype *p;
    p = SearchHash(HT, word);
    if (p != NULL){
        p->data.count[fileNo]++;
    }
}

int CountFile(HashTable *HT, const char *filename, int fileNo)
{
    FILE *fp;
    int ch;
    int next;
    int len;
    char word[64];

    fp = fopen(filename, "r");
    if (fp == NULL){
        return 0;
    }

    while ((ch = fgetc(fp)) != EOF){
        if (isalpha(ch) || ch == '_'){
            len = 0;
            do{
                if (len < 64 - 1){
                    word[len++] = (char)ch;
                }
                ch = fgetc(fp);
            } while (ch != EOF && (isalnum(ch) || ch == '_'));
            word[len] = '\0';
            CountWord(HT, word, fileNo);
            if (ch != EOF){
                ungetc(ch, fp);
            }
        } else if (ch == '/'){
            next = fgetc(fp);
            if (next == '/'){
                while ((ch = fgetc(fp)) != EOF && ch != '\n'){
                }
            } else if (next == '*'){
                while ((ch = fgetc(fp)) != EOF){
                    if (ch == '*'){
                        next = fgetc(fp);
                        if (next == '/'){
                            break;
                        }
                        if (next != EOF){
                            ungetc(next, fp);
                        }
                    }
                }
            } else if (next != EOF){
                ungetc(next, fp);
            }
        } else if (ch == '"'){
            while ((ch = fgetc(fp)) != EOF){
                if (ch == '\\'){
                    fgetc(fp);
                } else if (ch == '"'){
                    break;
                }
            }
        } else if (ch == '\''){
            while ((ch = fgetc(fp)) != EOF){
                if (ch == '\\'){
                    fgetc(fp);
                } else if (ch == '\''){
                    break;
                }
            }
        }
    }

    fclose(fp);
    return 1;
}

void PrintTable(HashTable HT)
{
    int i;
    Elemtype *p;
    printf("\n%-8s%-16s%-12s%-12s\n", "Address", "Keyword", "Program1", "Program2");
    for (i = 0; i < 43; i++){
        p = HT.elem[i];
        while (p != NULL){
            printf("%-8d%-16s%-12d%-12d\n", i, p->key,
                   p->data.count[0], p->data.count[1]);
            p = p->next;
        }
    }
}

void Calculate(HashTable HT)
{
    int i;
    double dot;
    double len1;
    double len2;
    double diff;
    double dist2;
    double s;
    double d;
    Elemtype *p;

    dot = 0.0;
    len1 = 0.0;
    len2 = 0.0;
    dist2 = 0.0;
    for (i = 0; i < 43; i++){
        p = HT.elem[i];
        while (p != NULL){
            dot += p->data.count[0] * p->data.count[1];
            len1 += p->data.count[0] * p->data.count[0];
            len2 += p->data.count[1] * p->data.count[1];
            diff = p->data.count[0] - p->data.count[1];
            dist2 += diff * diff;
            p = p->next;
        }
    }

    if (len1 == 0.0 && len2 == 0.0){
        s = 1.0;
    } else if (len1 == 0.0 || len2 == 0.0){
        s = 0.0;
    } else{
        s = dot / (sqrt(len1) * sqrt(len2));
    }
    d = sqrt(dist2);

    printf("\nS = %.6f\n", s);
    printf("D = %.6f\n", d);
}

void DestroyHashTable(HashTable *HT)
{
    int i;
    Elemtype *p;
    Elemtype *q;
    for (i = 0; i < 43; i++){
        p = HT->elem[i];
        while (p != NULL){
            q = p->next;
            free(p);
            p = q;
        }
        HT->elem[i] = NULL;
    }
    HT->len = 0;
}

void ReadFileName(char *name, const char *prompt)
{
    size_t len;
    printf("%s", prompt);
    fgets(name, 260, stdin);
    len = strlen(name);
    if (len > 0 && name[len - 1] == '\n'){
        name[len - 1] = '\0';
    }
}

int main(int argc, char *argv[])
{
    HashTable HT;
    char file1[260];
    char file2[260];

    if (!CreateKeyWordTable(&HT)){
        printf("Failed to create hash table.\n");
        return 1;
    }

    if (argc >= 3){
        strcpy(file1, argv[1]);
        strcpy(file2, argv[2]);
    } else{
        ReadFileName(file1, "输入第一个C程序文件名: ");
        ReadFileName(file2, "输入第二个C程序文件名: ");
    }

    if (!CountFile(&HT, file1, 0)){
        printf("Cannot open %s\n", file1);
        DestroyHashTable(&HT);
        return 1;
    }
    if (!CountFile(&HT, file2, 1)){
        printf("Cannot open %s\n", file2);
        DestroyHashTable(&HT);
        return 1;
    }

    PrintTable(HT);
    Calculate(HT);
    DestroyHashTable(&HT);
    return 0;
}
