#include "HuffTree.h"
#include "safe_mem_functions.h"
#include <stdio.h>

HuffNode *createHuffNode(char chr, int freq){
    HuffNode *huffnode = NULL;
    huffnode = (HuffNode *)safe_malloc(sizeof(HuffNode));
    huffnode->chr = chr;
    huffnode->freq = freq;
    huffnode->left = NULL;
    huffnode->right = NULL;
    huffnode->isLeaf = 1;
    return huffnode;
}

HuffNode **createHuffNodeArray(){
    HuffNode **nodeArr = NULL;
    nodeArr = (HuffNode **)safe_calloc(NUMCHAR,sizeof(HuffNode *));
    return nodeArr;
}

void put(HuffNode **nodeArr, char c, int *numitems){
    HuffNode *huffnode = NULL;
    if (nodeArr[(unsigned char)c]){
        nodeArr[(unsigned char)c]->freq++;
        return;
    }
    huffnode = createHuffNode(c, 1);
    nodeArr[(unsigned char)c] = huffnode;
    (*numitems)++;
    return;
}

HuffNode **cleanNodeArr(HuffNode **nodeArr, int numitems){
    int i = 0;
    int j = 0;
    for (i = 0; i < NUMCHAR; i++){
        if (nodeArr[i] != NULL){
            nodeArr[j++] = nodeArr[i];
        }
    }
    nodeArr = (HuffNode **)safe_realloc(nodeArr, numitems*sizeof(HuffNode *));
    return nodeArr;
}


HuffNode **buildHuffTree(HuffNode **nodeArr, int numitems){
    HuffNode *newHuffNode = NULL;
    int size = numitems;
    while (size > 1){
        qsort(nodeArr, size, sizeof(HuffNode *), compare);
        newHuffNode = createHuffNode(-1,((nodeArr[size - 1]->freq)+
                                        (nodeArr[size - 2]->freq)));
        newHuffNode->left = nodeArr[size - 1];
        newHuffNode->right = nodeArr[size - 2];
        newHuffNode->isLeaf = 0;
        nodeArr[size - 2] = newHuffNode;
        nodeArr = (HuffNode **)safe_realloc(nodeArr,
                                            (size - 1) * sizeof(HuffNode *));
        size--;
    }
    return nodeArr; //pointer to pointer to root of tree
}

int compare(const void *a, const void *b){
    const HuffNode *aa = *(HuffNode **)a;
    const HuffNode *bb = *(HuffNode **)b;
    if (bb->freq == aa->freq){
        if (bb->isLeaf && aa->isLeaf){
            return (unsigned char)bb->chr - (unsigned char)aa->chr;
        }
        else if(bb->isLeaf){
            return (unsigned char)bb->chr - (aa->chr);
        }
        return (bb->chr) - (unsigned char)aa->chr;
    }
    return (bb->freq) - (aa->freq);
}

void printCodes(HuffNode *root){
    char *codeArr[NUMCHAR];
    char code[NUMCHAR];
    int i = 0;
    while (i < NUMCHAR){
        codeArr[i++] = NULL;
    }
    strcpy(code, "");
    setCodes(root, codeArr, code);
    i = 0;
    while(i < NUMCHAR){
        if (codeArr[i] != NULL){
            printf("0x%-.2x: %s\n", i, codeArr[i]);
            free(codeArr[i]);
        }
        i++;
    }
    return;
}

void setCodes(HuffNode *root, char **codeArr, char *code){
    char *storeCode = NULL;
    char saveCode[NUMCHAR];
    if (root == NULL){
        return;
    }
    if (root->left == NULL && root->right == NULL){
        storeCode = (char *)safe_malloc((strlen(code) + 1) * sizeof(char));
        strcpy(storeCode, code);
        codeArr[(unsigned char)root->chr] = storeCode;
        return;
    }
    strcpy(saveCode, code);
    setCodes(root->left, codeArr, strcat(code, "0"));
    setCodes(root->right, codeArr, strcat(saveCode, "1"));
    return;
}

void freeNodeArr(HuffNode **nodeArr){
    freeTree(nodeArr[0]);
    free(nodeArr);
    return;
}

void freeTree(HuffNode *root){
    if (root == NULL){
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
    return;
}