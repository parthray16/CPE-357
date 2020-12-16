#include "HuffTree.h"
#include "safe_mem_functions.h"


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
    return nodeArr;
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

void writeHeader(HuffNode *root, int outfile, unsigned char num){
    HuffNode **nodeArr = NULL;
    uint32_t count = 0; 
    nodeArr = safe_malloc((num + 1) * sizeof(HuffNode *));
    fillArr(nodeArr, root);
    qsort(nodeArr, num + 1, sizeof(HuffNode *), charcmp);
    if(write(outfile, &num, sizeof(char)) == -1){
        perror("write");
        exit(EXIT_FAILURE);
    }
    while(num > 0){
        if (write(outfile, &(nodeArr[num]->chr), sizeof(char)) == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
        count = htonl(nodeArr[num--]->freq);
        if (write(outfile, &count, sizeof(uint32_t)) == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    if ((char)num == 0){
        if (write(outfile, &(nodeArr[num]->chr), sizeof(char)) == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
        count = htonl(nodeArr[num--]->freq);
        if (write(outfile, &count, sizeof(uint32_t)) == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    free(nodeArr);
    return;
}

int charcmp(const void *a, const void *b){
    const HuffNode *aa = *(HuffNode **)a;
    const HuffNode *bb = *(HuffNode **)b;
    return (unsigned char)bb->chr - (unsigned char)aa->chr;
}

static int location = 0;
void fillArr(HuffNode **Arr, HuffNode *root){
    if (root == NULL){
        return;
    }
    if (root->left == NULL && root->right == NULL){
        Arr[location++] = root;
        return;
    }
    fillArr(Arr, root->left);
    fillArr(Arr, root->right);
    return;
}

void writeCodes(HuffNode *root, int infile, int outfile){
    char *codeArr[NUMCHAR];
    char code[NUMCHAR];
    int i = 0;
    int j = 0;
    unsigned char num = 0;
    unsigned char byte = 0;
    char *end;
    int r_status = 0;
    unsigned char c = 0;
    char *buff = NULL;
    buff = safe_calloc(9, sizeof(char));
    while (i < NUMCHAR){
        codeArr[i++] = NULL;
    }
    i = 0;
    strcpy(code, "");
    setCodes(root, codeArr, code, &num);
    writeHeader(root, outfile, num - 1);
    if (num - 1 != 0){
        r_status = read(infile, &c, sizeof(char));
        if(r_status == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
        while (1){
            if (r_status != 0){
                while(i < 8){
                    if (codeArr[c][j] == '\0'){
                        j = 0;
                        r_status = read(infile, &c, sizeof(char));
                        if(r_status == -1){
                            perror("read");
                            exit(EXIT_FAILURE);
                        }
                        if(r_status == 0){
                            break;
                        }
                    }
                    buff[i++] = codeArr[c][j++];  
                }
                while (i < 8){
                    buff[i++] = '0';
                }
                byte = strtol(buff, &end, 2);
                if (byte != 0 || r_status != 0){
                    if (write(outfile, &byte, sizeof(char)) == -1){
                        perror("write");
                        exit(EXIT_FAILURE);
                    }
                }    
                i = 0;
                free(buff);
            }
            if (r_status == 0){
                break;
            }
            buff = safe_calloc(9, sizeof(char));
        }
    }    
    i = 0;
    while(i < NUMCHAR){
        if (codeArr[i] != NULL){
            free(codeArr[i]);
        }
        i++;
    }
    return;
}

void setCodes(HuffNode *root, char **codeArr, char *code, unsigned char *num){
    char *storeCode = NULL;
    char saveCode[NUMCHAR];
    if (root == NULL){
        return;
    }
    if (root->left == NULL && root->right == NULL){
        storeCode = (char *)safe_malloc((strlen(code) + 1) * sizeof(char));
        strcpy(storeCode, code);
        codeArr[(unsigned char)root->chr] = storeCode;
        (*num)++;
        return;
    }
    strcpy(saveCode, code);
    setCodes(root->left, codeArr, strcat(code, "0"), num);
    setCodes(root->right, codeArr, strcat(saveCode, "1"), num);
    return;
}

void decode(HuffNode *root, int totalc, int infile, int outfile){
    unsigned char mask = 0x80;
    int r_status = 0;
    unsigned char c = 0;
    HuffNode *curr = root;
    char bit = '0';

    if (totalc > 0){
        r_status = read(infile, &c, sizeof(char));
        if(r_status == -1){
            perror("read");
            exit(EXIT_FAILURE);
        }
        while (1){
            if (curr->left != NULL && curr->right != NULL){
                if (c & mask){
                    bit = '1';
                }
                else{
                    bit = '0';
                }
                if (bit == '0'){
                    curr = curr->left;
                }
                else{
                    curr = curr->right;
                }
                mask >>= 1;
                if(mask == 0){
                    r_status = read(infile, &c, sizeof(char));
                    if(r_status == -1){
                        perror("read");
                        exit(EXIT_FAILURE);
                    }
                    mask = 0x80;
                }
            }
            else if (curr->left == NULL && curr->right == NULL){
                if (write(outfile, &(curr->chr), sizeof(char)) == -1){
                        perror("write");
                        exit(EXIT_FAILURE);
                }
                totalc--;
                curr = root;
            }
            if (totalc <= 0){
                break;
            }
        }
    }
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