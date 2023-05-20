#define   _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef int KeyType;        //定义空类型

typedef struct {               //定义存储结构RcdType
    KeyType key;
} RcdType;

typedef struct Node {      //定义节点
    RcdType r;
    struct Node* next;
} Node;

typedef struct {                //定义哈希表
    Node** rcd;
    int size;
    int count;
    int (*hash)(KeyType key, int);
} HashTable;

bool InitHash(HashTable& H, int size, int (*hash)(KeyType, int)) {                                      //哈希表初始化
    int i;
    if (NULL == (H.rcd = (Node**)malloc(sizeof(Node*) * size))) return false;
    for (i = 0; i < size; i++) H.rcd[i] = NULL;
    H.size = size;
    H.hash = hash;
    H.count = 0;
    return true;
}

int hash(int key, int hashsize) {                                                                                         //哈希函数定义
    return key % hashsize;
}

Node* SearchHash(HashTable& H, int key) {                                                                    //哈希表的查找
    int p = H.hash(key, H.size);
    Node* np;
    for (np = H.rcd[p]; np != NULL; np = np->next) {
        if (np->r.key == key)
            return np;
    }
    return NULL;
}

bool InsertHash(HashTable& H, RcdType e) {                                                                    //哈希表元素插入
    int p;
    Node* np;
    if ((np = SearchHash(H, e.key)) == NULL) {
        p = H.hash(e.key, H.size);
        np = (Node*)malloc(sizeof(Node));
        if (NULL == np)
            return false;
        np->r = e;
        np->next = H.rcd[p];
        H.rcd[p] = np;
        H.count++;
        return true;
    }
    else {
        return false;
    }
}

float TestInsertConflict(HashTable& H, int* keys, int n) {                                                    //插入冲突测试
    int i;
    float avglen;
    RcdType e;

    for (i = 0; i < n; i++) {
        e.key = keys[i];
        InsertHash(H, e);
    }

    avglen = (float)H.count / H.size;                                                                                    //平均查找长度计算

    printf("平均查找长度为：%f\n", avglen/2+1);

    return avglen;
}

void FreeHash(HashTable& H) {                                                                                        //释放哈希表
    int i;
    Node* np, * q;
    for (i = 0; i < H.size; i++) {
        np = H.rcd[i];
        while (np != NULL) {
            q = np->next;
            free(np);
            np = q;
        }
    }
    free(H.rcd);
    H.rcd = NULL;
    H.size = 0;
    H.count = 0;
}

int main() {
    int size;
    int choice;
    HashTable H;
    int k;
    int keys[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    int n = sizeof(keys) / sizeof(int);
    float avglen;

begin:
    printf("请输入哈希表大小：");
    scanf("%d", &size);
    if (size <= 0) {
        printf("哈希表长度需要大于等于1，请重新输入：");
        scanf("%d", &size);
    }
    printf("\n");
    printf("关键词key分别为：3, 4, 5, 6, 7, 8, 9, 10, 11\n");

    InitHash(H, size, hash);

    for (int i = 0; i < n; i++) {
        RcdType e;
        e.key = keys[i];
        InsertHash(H, e);
    }

again:
    printf("请选择进行的操作:\n1：查找单个元素\n2：计算平均查找长度和空间复杂度\n3：重新输入size\n4：结束程序\n");
    printf("请输入：");
    scanf("%d", &choice);
    printf("\n");

    if (choice == 1) {
        printf("请输入要查找的关键字：");
        scanf("%d", &k);
        Node* np = SearchHash(H, k);
        if (np == NULL) {
            printf("未找到关键字为 %d 的元素\n", k);
        }
        else {
            printf("找到了关键字为 %d 的元素\n", np->r.key);
        }
        goto again;
    }

    if (choice == 2) {
        for (int i = 0; i < n; i++) {
            Node* np = SearchHash(H, keys[i]);
            if (np == NULL) {
                printf("未找到关键字为 %d 的元素\n", keys[i]);
            }
            else {
                printf("找到了关键字为 %d 的元素\n", np->r.key);
            }
        }
        avglen = TestInsertConflict(H, keys, n);
        printf("构造函数的空间复杂度为：%d bytes\n", sizeof(Node*) * size);
        printf("\n");
        goto again;
    }

    if (choice == 3) {
        goto begin;
    }

    if (choice == 4) {
        printf("\n");
        goto end;
    }

    else {
        printf("输入错误，请重新输入");
        goto again;
    }
end:
    FreeHash(H);
    system("pause");
    return 0;
}

