#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    int weight;
    int parent, lchild, rchild;
    char ch;
    char code[100];
} HTNode;

typedef struct{
    HTNode *tree;
    int n;
} HuffData;

typedef struct{
    char elem[100];
    int top;
} SqStack;

void InitStack(SqStack *S){
    S->top = 0;
}

void Push(SqStack *S, char ch){
    S->elem[S->top++] = ch;
}

void Pop(SqStack *S, char *ch){
    if (S->top > 0){
        *ch = S->elem[--S->top];
    }
}

void Select(HTNode *HT, int end, int *s1, int *s2){
    int min1 = 0, min2 = 0;
    int m1 = 0, m2 = 0;
    int i;

    for (i = 1; i <= end; i++){
        if (HT[i].parent == 0){
            if (min1 == 0){
                min1 = i;
                m1 = HT[i].weight;
            } else if (min2 == 0){
                min2 = i;
                m2 = HT[i].weight;
                break;
            }
        }
    }

    if (m1 > m2){
        int tempWeight = m1; m1 = m2; m2 = tempWeight;
        int tempIndex = min1; min1 = min2; min2 = tempIndex;
    }

    for (i = i + 1; i <= end; i++){
        if (HT[i].parent == 0){
            if (HT[i].weight < m1){
                m2 = m1;
                min2 = min1;
                m1 = HT[i].weight;
                min1 = i;
            } else if (HT[i].weight < m2){
                m2 = HT[i].weight;
                min2 = i;
            }
        }
    }
    *s1 = min1;
    *s2 = min2;
}

void CreateHuffmanTree(HuffData *data, int *w, char *c){
    int n = data->n;
    if (n <= 1) return;
    int m = n * 2 - 1;
    
    data->tree = (HTNode *)malloc(sizeof(HTNode) * (m + 1));
    
    for (int i = 1; i <= m; i++){
        data->tree[i].weight = (i <= n) ? w[i - 1] : 0;
        data->tree[i].ch = (i <= n) ? c[i - 1] : '\0';
        data->tree[i].lchild = data->tree[i].rchild = data->tree[i].parent = 0;
        data->tree[i].code[0] = '\0';
    }
    
    for (int i = n + 1; i <= m; i++){
        int s1, s2;
        Select(data->tree, i - 1, &s1, &s2);
        data->tree[i].lchild = s1;
        data->tree[i].rchild = s2;
        data->tree[i].weight = data->tree[s1].weight + data->tree[s2].weight;
        data->tree[s1].parent = data->tree[s2].parent = i;
    }
}

void Coding(HTNode *HT, int root, SqStack *S){
    if (root != 0){
        if (HT[root].lchild == 0){
            Push(S, '\0');
            strcpy(HT[root].code, S->elem);
            char ch;
            Pop(S, &ch);
        } else{
            Push(S, '0');
            Coding(HT, HT[root].lchild, S);
            char ch;
            Pop(S, &ch);
            
            Push(S, '1');
            Coding(HT, HT[root].rchild, S);
            Pop(S, &ch);
        }
    }
}

void CreateHuffmanCoding(HuffData *data){
    SqStack S;
    InitStack(&S);
    Coding(data->tree, 2 * data->n - 1, &S);
}

void Initialization(HuffData *data){
    FILE *fin = fopen("init_input.txt", "r");
    if (!fin){
        printf("未找到 init_input.txt.\n");
        return;
    }
    
    fscanf(fin, "%d", &data->n);
    int *w = (int *)malloc(sizeof(int) * (data->n));
    char *c = (char *)malloc(sizeof(char) * (data->n));
    char str[20];
    int weight;
    
    for (int i = 0; i < data->n; i++){
        fscanf(fin, "%s %d", str, &weight);
        if (strcmp(str, "SPACE") == 0){
            c[i] = ' ';
        } else{
            c[i] = str[0];
        }
        w[i] = weight;
    }
    fclose(fin);

    CreateHuffmanTree(data, w, c);
    CreateHuffmanCoding(data);

    FILE *fout = fopen("hfmTree.txt", "wb");
    int m = 2 * data->n - 1;
    fwrite(&data->n, sizeof(int), 1, fout);
    fwrite(data->tree + 1, sizeof(HTNode), m, fout);
    fclose(fout);

    FILE *outText = fopen("ToBeTran.txt", "w");
    fprintf(outText, "THIS PROGRAM IS MY FAVORITE");
    fclose(outText);

    free(w);
    free(c);
    printf("初始化完成\n");
}

void Encoding(HuffData *data){
    if (!data->tree){
        printf("请先初始化\n");
        return;
    }
    
    FILE *fin = fopen("ToBeTran.txt", "r");
    FILE *fout = fopen("CodeFile.txt", "w");
    if (!fin || !fout){
        printf("打开文件失败\n");
        return;
    }

    int ch;
    while ((ch = fgetc(fin)) != EOF){
        int idx = -1;
        for (int i = 1; i <= data->n; i++){
            if (data->tree[i].ch == (char)ch && data->tree[i].lchild == 0 && data->tree[i].rchild == 0){
                idx = i;
                break;
            }
        }
        if (idx != -1){
            fprintf(fout, "%s", data->tree[idx].code);
        }
    }
    
    fclose(fin);
    fclose(fout);
    printf("编码成功 (CodeFile.txt)\n");
}

void Decoding(HuffData *data){
    if (!data->tree){
        printf("请先初始化\n");
        return;
    }
    
    FILE *fin = fopen("CodeFile.txt", "r");
    FILE *fout = fopen("TextFile.txt", "w");
    if (!fin || !fout){
        printf("打开文件失败\n");
        return;
    }
    
    int root = 2 * data->n - 1;
    int curr = root;
    int bit;
    
    while ((bit = fgetc(fin)) != EOF){
        if (bit == '0'){
            curr = data->tree[curr].lchild;
        } else if (bit == '1'){
            curr = data->tree[curr].rchild;
        }
        
        if (data->tree[curr].lchild == 0 && data->tree[curr].rchild == 0){
            fputc(data->tree[curr].ch, fout);
            curr = root;
        }
    }
    
    fclose(fin);
    fclose(fout);
    printf("解码完成 (TextFile.txt)\n");
}

void PrintCodeFile(){
    FILE *fin = fopen("CodeFile.txt", "r");
    FILE *fout = fopen("CodePrint.txt", "w");
    if (!fin || !fout){
        printf("Error opening files. Have you encoded the text?\n");
        return;
    }
    
    int ch;
    int count = 0;
    printf("CodeFile.txt Content:\n");
    while ((ch = fgetc(fin)) != EOF){
        putchar(ch);
        fputc(ch, fout);
        count++;
        if (count % 50 == 0){
            putchar('\n');
            fputc('\n', fout);
        }
    }
    if (count % 50 != 0){
        putchar('\n');
        fputc('\n', fout);
    }
    
    fclose(fin);
    fclose(fout);
    printf("打印编码完成 (CodePrint.txt)\n");
}

void PrintNode(HTNode *HT, int root, int depth, FILE *fout){
    if (root == 0) return;
    PrintNode(HT, HT[root].rchild, depth + 1, fout);
    
    for (int i = 0; i < depth; i++){
        printf("    ");
        fprintf(fout, "    ");
    }
    
    if (HT[root].lchild == 0 && HT[root].rchild == 0){
        printf("%d('%c')\n", HT[root].weight, HT[root].ch);
        fprintf(fout, "%d('%c')\n", HT[root].weight, HT[root].ch);
    } else{
        printf("%d\n", HT[root].weight);
        fprintf(fout, "%d\n", HT[root].weight);
    }
    
    PrintNode(HT, HT[root].lchild, depth + 1, fout);
}

void TreePrinting(HuffData *data){
    if (!data->tree){
        printf("请先初始化\n");
        return;
    }
    
    FILE *fout = fopen("TreePrint.txt", "w");
    PrintNode(data->tree, 2 * data->n - 1, 0, fout);
    fclose(fout);
    printf("HuffmanTree打印完成 (TreePrint.txt)\n");
}

int main(){
    HuffData data;
    data.tree = NULL;
    data.n = 0;
    
    char choice[10];

    do{
        printf(" I. Initialization\n");
        printf(" E. Encoding\n");
        printf(" D. Decoding\n");
        printf(" P. Print CodeFile\n");
        printf(" T. Tree Printing\n");
        printf(" Q. Quit\n\n");
        printf("输入操作指令: ");
        scanf("%s", choice);

        switch (choice[0]){
            case 'I': case 'i': Initialization(&data); break;
            case 'E': case 'e': Encoding(&data); break;
            case 'D': case 'd': Decoding(&data); break;
            case 'P': case 'p': PrintCodeFile(); break;
            case 'T': case 't': TreePrinting(&data); break;
            case 'Q': case 'q': break;
            default: printf("没有对应指令\n"); break;
        }
    } while (choice[0] != 'Q' && choice[0] != 'q');

    if (data.tree){
        free(data.tree);
    }
    
    return 0;
}