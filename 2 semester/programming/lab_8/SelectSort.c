#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void FillInc(long long int a[], int n){
    for (int i=0;i<n;i++){
        a[i]=i+1;}}

void FillDec(long long int a[], int n){
    int k = n;
    for (int i=0;i<n;i++){
        a[i]=k;
        k--;}}

void FillRand(long long int a[], int n){
    srand(time(NULL));
    for (int i=0;i<n;i++){
        a[i]=rand()%100;}}

int SelectSort(long long int a[], int n){
    int M=0,C=0;
    for (int i=0;i<n-1;i++){
        int k = i;
        for (int j=i+1;j<n;j++){
            C++;
            if (a[k] > a[j]){
                k=j;}}
        M=M+3;
        int p = a[i];
        a[i]=a[k];
        a[k]=p;}
    return M+C;}

int SelectSort2(long long int a[], int n){
    int M=0,C=0;
    for (int i=0;i<n-1;i++){
        int k = i;
        for (int j=i+1;j<n;j++){
            C++;
            if (a[k] > a[j]){
                k=j;}}
        if (i!=k){
            M=M+3;
            int p = a[i];
            a[i]=a[k];
            a[k]=p;}}
    return M+C;}

void PrintMas(long long int a[], int n){
    for (int i=0;i<n;i++){
        printf("%lld ",a[i]);
    }printf("\n");}

