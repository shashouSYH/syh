#include <stdio.h>
#include <stdlib.h>

typedef struct ArcNode{
    int adjvex;
    double weight;
    struct ArcNode *nextarc;
} ArcNode;

typedef struct VertexNode{
    char data;
    ArcNode *firstarc;
} VertexNode, AdjList[1000];

typedef struct{
    AdjList vertices;
    int vexnum;
    int arcnum;
} ALGraph;

int LocateVex(ALGraph G, char v){
    int i;
    for (i = 0; i < G.vexnum; i++){
        if (G.vertices[i].data == v){
            return i;
        }
    }
    return -1;
}

int InsertArc(ALGraph *G, int i, int j, double weight){
    ArcNode *p;
    p = (ArcNode *)malloc(sizeof(ArcNode));
    if (p == NULL){
        return 0;
    }

    p->adjvex = j;
    p->weight = weight;
    p->nextarc = G->vertices[i].firstarc;
    G->vertices[i].firstarc = p;
    return 1;
}

int CreateGraph(ALGraph *G){
    int i, k;
    char vi, vj;
    double weight;

    if (scanf("%d %d", &G->vexnum, &G->arcnum) != 2){
        return 0;
    }
    if (G->vexnum <= 0 || G->vexnum > 1000 || G->arcnum < 0){
        return 0;
    }

    for (i = 0; i < G->vexnum; i++){
        if (scanf(" %c", &G->vertices[i].data) != 1){
            return 0;
        }
        G->vertices[i].firstarc = NULL;
    }

    for (k = 0; k < G->arcnum; k++){
        int v1, v2;
        if (scanf(" %c %c %lf", &vi, &vj, &weight) != 3){
            return 0;
        }

        v1 = LocateVex(*G, vi);
        v2 = LocateVex(*G, vj);
        if (v1 == -1 || v2 == -1){
            return 0;
        }

        if (!InsertArc(G, v1, v2, weight) || !InsertArc(G, v2, v1, weight)){
            return 0;
        }
    }

    return 1;
}

int MinEdge(double lowcost[], int intree[], int n){
    int i, k;
    double min;
    k = -1;
    min = 100000;
    for (i = 0; i < n; i++){
        if (!intree[i] && lowcost[i] < min){
            min = lowcost[i];
            k = i;
        }
    }
    return k;
}

int Prim(ALGraph G, int v0){
    int i, j, k;
    int adjvex[1000];
    int intree[1000];
    double lowcost[1000];
    double total;
    ArcNode *p;

    for (i = 0; i < G.vexnum; i++){
        intree[i] = 0;
        adjvex[i] = -1;
        lowcost[i] = 100000;
    }

    intree[v0] = 1;
    lowcost[v0] = 0;
    total = 0;
    for (p = G.vertices[v0].firstarc; p != NULL; p = p->nextarc){
        lowcost[p->adjvex] = p->weight;
        adjvex[p->adjvex] = v0;
    }

    for (i = 1; i < G.vexnum; i++){
        k = MinEdge(lowcost, intree, G.vexnum);
        if (k == -1 || adjvex[k] == -1){
            printf("The graph is not connected.\n");
            return 0;
        }

        printf("%c %c %.1f\n",
               G.vertices[adjvex[k]].data,
               G.vertices[k].data,
               lowcost[k]);

        total += lowcost[k];
        intree[k] = 1;

        for (p = G.vertices[k].firstarc; p != NULL; p = p->nextarc){
            j = p->adjvex;
            if (!intree[j] && p->weight < lowcost[j]){
                lowcost[j] = p->weight;
                adjvex[j] = k;
            }
        }
    }
    printf("总费用: %.1f\n", total);
    return 1;
}

void DestroyGraph(ALGraph *G){
    int i;
    for (i = 0; i < G->vexnum; i++){
        ArcNode *p = G->vertices[i].firstarc;
        while (p != NULL){
            ArcNode *q = p;
            p = p->nextarc;
            free(q);
        }
        G->vertices[i].firstarc = NULL;
    }
}

int main(void){
    ALGraph G;
    FILE *fp;

    fp = freopen("input.txt", "r", stdin);
    if (fp == NULL){
        printf("无法打开 input.txt 文件。\n");
        return 1;
    }

    if (!CreateGraph(&G)){
        printf("输入错误。\n");
        return 1;
    }

    Prim(G, 0);
    DestroyGraph(&G);
    return 0;
}
