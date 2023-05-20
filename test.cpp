#define   _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef int KeyType;        //���������

typedef struct {               //����洢�ṹRcdType
    KeyType key;
} RcdType;

typedef struct Node {      //����ڵ�
    RcdType r;
    struct Node* next;
} Node;

typedef struct {                //�����ϣ��
    Node** rcd;
    int size;
    int count;
    int (*hash)(KeyType key, int);
} HashTable;

bool InitHash(HashTable& H, int size, int (*hash)(KeyType, int)) {                                      //��ϣ���ʼ��
    int i;
    if (NULL == (H.rcd = (Node**)malloc(sizeof(Node*) * size))) return false;
    for (i = 0; i < size; i++) H.rcd[i] = NULL;
    H.size = size;
    H.hash = hash;
    H.count = 0;
    return true;
}

int hash(int key, int hashsize) {                                                                                         //��ϣ��������
    return key % hashsize;
}

Node* SearchHash(HashTable& H, int key) {                                                                    //��ϣ��Ĳ���
    int p = H.hash(key, H.size);
    Node* np;
    for (np = H.rcd[p]; np != NULL; np = np->next) {
        if (np->r.key == key)
            return np;
    }
    return NULL;
}

bool InsertHash(HashTable& H, RcdType e) {                                                                    //��ϣ��Ԫ�ز���
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

float TestInsertConflict(HashTable& H, int* keys, int n) {                                                    //�����ͻ����
    int i;
    float avglen;
    RcdType e;

    for (i = 0; i < n; i++) {
        e.key = keys[i];
        InsertHash(H, e);
    }

    avglen = (float)H.count / H.size;                                                                                    //ƽ�����ҳ��ȼ���

    printf("ƽ�����ҳ���Ϊ��%f\n", avglen/2+1);

    return avglen;
}

void FreeHash(HashTable& H) {                                                                                        //�ͷŹ�ϣ��
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
    printf("�������ϣ���С��");
    scanf("%d", &size);
    if (size <= 0) {
        printf("��ϣ������Ҫ���ڵ���1�����������룺");
        scanf("%d", &size);
    }
    printf("\n");
    printf("�ؼ���key�ֱ�Ϊ��3, 4, 5, 6, 7, 8, 9, 10, 11\n");

    InitHash(H, size, hash);

    for (int i = 0; i < n; i++) {
        RcdType e;
        e.key = keys[i];
        InsertHash(H, e);
    }

again:
    printf("��ѡ����еĲ���:\n1�����ҵ���Ԫ��\n2������ƽ�����ҳ��ȺͿռ临�Ӷ�\n3����������size\n4����������\n");
    printf("�����룺");
    scanf("%d", &choice);
    printf("\n");

    if (choice == 1) {
        printf("������Ҫ���ҵĹؼ��֣�");
        scanf("%d", &k);
        Node* np = SearchHash(H, k);
        if (np == NULL) {
            printf("δ�ҵ��ؼ���Ϊ %d ��Ԫ��\n", k);
        }
        else {
            printf("�ҵ��˹ؼ���Ϊ %d ��Ԫ��\n", np->r.key);
        }
        goto again;
    }

    if (choice == 2) {
        for (int i = 0; i < n; i++) {
            Node* np = SearchHash(H, keys[i]);
            if (np == NULL) {
                printf("δ�ҵ��ؼ���Ϊ %d ��Ԫ��\n", keys[i]);
            }
            else {
                printf("�ҵ��˹ؼ���Ϊ %d ��Ԫ��\n", np->r.key);
            }
        }
        avglen = TestInsertConflict(H, keys, n);
        printf("���캯���Ŀռ临�Ӷ�Ϊ��%d bytes\n", sizeof(Node*) * size);
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
        printf("�����������������");
        goto again;
    }
end:
    FreeHash(H);
    system("pause");
    return 0;
}

