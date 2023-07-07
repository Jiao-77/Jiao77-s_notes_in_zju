#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void swap(int* a, int* b);
void Multisort(int arr[], int l, int r);
void Doublesort(int arr[], int l, int r);
void Threesort(int arr[], int l, int r);
void merge(int arr[], int left, int mid, int right);
void MergeSort(int arr[], int left, int right);
void printArray(int* arr, int n);
void test1();
void test2();

// ��ӡ����
void printArray(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// ���Ժ���
void test1() {
    srand(time(NULL));
    const int n = 100000000;
    int* arr = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000000;
    }

    printf("ԭʼ���飺");
    printArray(arr, 20);

    clock_t start = clock();
    int option;
    printf("Ҫ����ķ�ʽ��\n");
    printf("1.��·�Ͽ���\n");
    printf("2.˫·�Ͽ���\n");
    printf("3.��·�Ͽ���\n");
    printf("4.�鲢����\n");
    scanf("%d", &option);
    switch (option) {
        case 1:Multisort(arr, 0, n - 1); break;
        case 2:Doublesort(arr, 0, n - 1); break;
        case 3:Threesort(arr, 0, n - 1); break;
        case 4:MergeSort(arr, 0, n - 1); break;
        default:printf("��Ч�Ĳ���");
    }
    clock_t end = clock();
    printf("����������ʱ��%lf��\n", (double)(end - start) / CLOCKS_PER_SEC);

    printf("���������飺");
    printArray(arr, 20);
    free(arr);
}

void test2() {
    srand(time(NULL));
    const int n = 100000000;
    printf("�����ĸ����򷽷�����%d������\n",n);
    int* arr1 = (int*)malloc(sizeof(int) * n);
    int* arr2 = (int*)malloc(sizeof(int) * n);
    int* arr3 = (int*)malloc(sizeof(int) * n);
    int* arr4 = (int*)malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        arr1[i] = arr2[i] = arr3[i] = arr4[i] = rand() % n;
    }

    // printf("ԭʼ���飺");
    // printArray(arr1, 20);

    clock_t start = clock();
    Multisort(arr1, 0, n - 1);
    clock_t end = clock();
    printf("����ѭ��������ʱ��%lf��\n", (double)(end - start) / CLOCKS_PER_SEC);
    //printf("���������飺");
    //printArray(arr1, 500);
    free(arr1);

    start = clock();
    Doublesort(arr2, 0, n - 1);
    end = clock();
    printf("˫��ѭ��������ʱ��%lf��\n", (double)(end - start) / CLOCKS_PER_SEC);
    //printf("���������飺");
    //printArray(arr2, 500);
    free(arr2);

    start = clock();
    Threesort(arr3, 0, n - 1);
    end = clock();
    printf("��·ѭ��������ʱ��%lf��\n", (double)(end - start) / CLOCKS_PER_SEC);
    //printf("���������飺");
    //printArray(arr3, 500);
    free(arr3);

    start = clock();
    MergeSort(arr4, 0, n - 1);
    end = clock();
    printf("�鲢������ʱ��%lf��\n", (double)(end - start) / CLOCKS_PER_SEC);
    //printf("���������飺");
    //printArray(arr4, 500);
    free(arr4);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// �ϲ�������������
void merge(int arr[], int left, int mid, int right) {
    int len = right - left + 1;
    int* temp = (int*)malloc(sizeof(int) * len);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= right) {
        temp[k++] = arr[j++];
    }
    for (int p = 0; p < len; p++) {
        arr[left + p] = temp[p];
    }
    free(temp);
}

// �鲢����
void MergeSort(int arr[], int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    MergeSort(arr, left, mid);
    MergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void Multisort(int arr[], int l, int r) {
    if (l < r) {
        int pivot = arr[r];
        int i = l - 1;
        for (int j = l; j <= r - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i + 1], &arr[r]);
        int p = i + 1;
        Multisort(arr, l, p - 1);
        Multisort(arr, p + 1, r);
    }
}

void Doublesort(int arr[], int l, int r) {
    if (l < r) {
        int pivot = arr[l];
        int i = l, j = r;
        while (i < j) {
            while (i < j && arr[j] >= pivot) j--;
            if (i < j) arr[i++] = arr[j];
            while (i < j && arr[i] < pivot) i++;
            if (i < j) arr[j--] = arr[i];
        }
        arr[i] = pivot;
        Doublesort(arr, l, i - 1);
        Doublesort(arr, i + 1, r);
    }
}

void Threesort(int arr[], int l, int r) {
    if (l < r) {
        int pivot = arr[l];
        int lt = l, i = l + 1, gt = r;
        while (i <= gt) {
            if (arr[i] < pivot) {
                swap(&arr[i], &arr[lt]);
                lt++;
                i++;
            } else if (arr[i] > pivot) {
                swap(&arr[i], &arr[gt]);
                gt--;
            } else {
                i++;
            }
        }
        Threesort(arr, l, lt - 1);
        Threesort(arr, gt + 1, r);
    }
}

int main() {
    // test1();
    test2();
    return 0;
}