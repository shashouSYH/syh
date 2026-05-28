#include <stdio.h>
#include <string.h>

int index_str(const char *text, const char *pattern){
    int n = strlen(text);
    int m = strlen(pattern);
    if (m == 0) return 0;
    for (int i = 0; i <= n-m; i++){
        int j = 0;
        while (j < m && text[i+j] == pattern[j]){
            j++;
        }
        if (j == m) return i;
    }
    return -1;
}

int main(){
    char filename[200];
    char keyword[1000];

    fgets(filename, sizeof(filename), stdin);
    fgets(keyword, sizeof(keyword), stdin);
    
    filename[strcspn(filename, "\r\n")] = '\0';
    keyword[strcspn(keyword, "\r\n")] = '\0';

    FILE *fp = fopen(filename, "r");
    if (!fp){
        printf("No match found\n");
        return 0;
    }

    char line[100000];
    int num = 0;
    int found = 0;

    while (fgets(line, sizeof(line), fp)){
        num++;
        line[strcspn(line, "\r\n")] = '\0';
        if (index_str(line, keyword) != -1){
            printf("%d %s\n", num, line);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) printf("No match found\n");
    return 0;
}