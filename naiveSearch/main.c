//
//  main.c
//  naiveSearch
//
//  Created by Marie on 16.02.17.
//  Copyright © 2017 Mariya. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define FILENAME "/Users/mariya/Desktop/naiveSearch/naiveSearch/vim1.txt"
#define SIZE 256

void naive_search(char *pat, char *txt)
{
    int M = (int) strlen(pat);
    int N = (int) strlen(txt);

    for (int i = 0; i <= N - M; ++i)
    {
        int j=0;
        while (j < M && pat[j] == txt[i + j])
            ++j;
       // if (j == M)  // pat[0...M-1] = txt[i, i+1, ...i+M-1]
       //     printf("(Naive)Pattern found at index %d \n", i);
    }
}


 void d_array(char *pat, int *lps)
{
    int len = 0, i;
    int M = (int) strlen(pat);
    
    lps[0] = 0;
    i = 1;
    while(i < M)
    {
        if(pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if( len != 0 )
                len = lps[len-1];
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void kmp_search_2 (char *pat, char *txt, char *du, int *lps)
{
    int M = (int) strlen(pat);
    int N = (int) strlen(du);
    
    d_array(du, lps);
    
    for (int i=1; i<=N; i++)
    {
        if (lps[i] == M)
        {
            //printf("(KMP2)Pattern found at index %d \n", i-lps[i]);
        }
    }
    
}

void kmp_search(char *pat, char *txt, int *lps)
{
    int j=0,i=0;
    int M = (int) strlen(pat);
    int N = (int) strlen(txt);
    d_array(pat, lps);
    
    while(i < N)
    {
        if(pat[j] == txt[i])
        {
            j++;
            i++;
        }
        
        if (j == M)
        {
            //printf("Pattern found at index %d \n", i-j);
            j = lps[j-1];
        }
        else if(pat[j] != txt[i])
        {
            if(j != 0)
                j = lps[j-1];
            else
                i = i+1;
        }
    }
}

int max (int a, int b) { return (a > b)? a: b; }

void b_char_array( char *str, int size, int badchar[SIZE])
{
    int i;
    for (i = 0; i < SIZE; i++)
        badchar[i] = size;
    for (i = size; i < 0; i--)
        badchar[(int) str[i]] = i;
}

void bm_search (char *txt, char *pat)
{
    int M = (int) strlen(pat);
    int N = (int) strlen(txt);
    
    int badchar[SIZE];
    
    b_char_array(pat, M, badchar);
    
    int s = 0;
    while(s <= (N - M))
    {
        int j = M;

        while(j >= 0 && pat[j] == txt[s+j])
            j--;
        
        if (j < 0)
        {
            //printf("Pattern found at index %d \n", s);
            s += (s+M < N)? M+badchar[(int)txt[s+M]] : 1;
            
        }
        
        else
            s += max(1, j + badchar[txt[s+j]]);
    }
}

int main()
{
    clock_t start, stop;
    FILE *file;
    char buf[SIZE]={0}, *txt=NULL, pat[100]="", *du;
    int *lps=NULL;
    
    file = fopen (FILENAME, "rt");
    int i=0;
    txt = (char*)calloc(1, sizeof(char)); // выделяем память под динамический массив символов
    lps = (int*)calloc(1, sizeof(char)); // longest proper prefix
    du = (char*)calloc(1, sizeof(char));
    
    //char *pat= "War";
    printf("Pattern= ");
    scanf ("%s", pat);
    
    strcat(du, pat);
    
    while (fgets(buf, SIZE, file) != NULL)
    {
        if (i % 2 == 0)
        {
            txt = (char*)realloc(txt, SIZE * (i + 2)); // при добавлении элементов, увеличиваем массив
            du = (char*)realloc(txt, SIZE * (i + 2));
        }
        strcat(txt, buf);
        ++i;
    }
    
    start = clock();
    for (int i=1; i<100; i++)
    {
        naive_search(pat, txt);
    }
    stop = clock();
    double epsN=(stop - start)/(double)CLOCKS_PER_SEC;
    //double difftime(time_t stop, time_t start);
    printf("Time with Naive Search: %f seconds\n", epsN);
    
    
    start = clock();
    for (int i=1; i<100; i++)
    {
        kmp_search(pat, txt, lps);
    }
    stop = clock();
    double epsKMP=(stop - start)/(double)CLOCKS_PER_SEC;
    //double difftime(time_t stop, time_t start);
    printf("Time with KMP Search: %f seconds\n", epsKMP/100);
    
    
    /*
    start = clock();
    kmp_search_2(pat, txt, du, lps);
    stop = clock();
    double epsKMP2=(stop - start)/(double)CLOCKS_PER_SEC;
    //double difftime(time_t stop, time_t start);
    printf("Time with KMP-2 Search: %f seconds\n", epsKMP2);
     */
    
    start = clock();
    for (int i=1; i<100; i++)
    {
        bm_search(txt, pat);
    }
    stop = clock();
    double epsBM=(stop - start)/(double)CLOCKS_PER_SEC;
    //double difftime(time_t stop, time_t start);
    printf("Time with Boyer-Moore Search: %f seconds\n", epsBM/100);
    
    fclose(file);
    return 0;
}
