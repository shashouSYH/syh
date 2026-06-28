#include <stdio.h>
#include <string.h>

void getNext(const char *pattern, int next[]){
    int m = strlen(pattern);
    int j = 0;
    int k = -1;
    next[0] = -1;

    while (j < m-1){
        if (k==-1 || pattern[j]==pattern[k]){
            j++;
            k++;
            next[j] = k;
        } else{
            k = next[k];
        }
    }
}

int index_str(const char *text, const char *pattern){
    int n = strlen(text);
    int m = strlen(pattern);
    if (m==0) return 0;
    int next[1000];
    getNext(pattern, next);

    int i = 0, j = 0;
    while (i<n && j<m){
        if (j==-1 || text[i]==pattern[j]){
            i++;
            j++;
        } else {
            j = next[j];
        }
    }

    if (j==m) return i-j;
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

    char line[1000000];
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