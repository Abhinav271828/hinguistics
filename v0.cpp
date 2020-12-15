//
//  main.cpp
//  HE
//
//  Created by Abhinav Menon on 01/11/20.
//  Copyright © 2020 Abhinav Menon. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#define WM 100

/* Tree[0][0] = the
          [1] = quick
          [2] = brown
          [3] = fox
 Tree[1][0] = NP*/

struct Leaf
{
public:
    char word[WM] = {' '};
    char cat = ' ';
    int direct_f = 0;               //immediate child nodes
    int total_f = 0;            //total words in sentence accounted for
    Leaf* F[10] = {NULL};
};

int wparse(Leaf L[])
{
    int c = 0, w = 0, l = 0; char sent[100 * WM];
    printf("Enter the sentence:\n");
    while (sent[c-1] != '\n')
    {
        scanf("%c",&sent[c++]);                //1-D character array containing sentence
    }

    for (int i = 0; i < c-1; i++)
    {
        if (sent[i] == ' ')
        {
            L[w++].word[l] = '\n';                   //terminating each word
            l = 0;
        }
        else
        {
            L[w].word[l++] = sent[i];
        }
        L[i].total_f = 1;                   //parsing bottom-up
        L[i].direct_f = 0;
    }

    L[w].word[l] = '\n';
    
    printf("Enter the word categories separated by spaces:\n");
    for (int i = 0; i <= w; i++)
    {
        scanf(" %c",&L[i].cat);
    }
    
    return w;                           //returns number of words - 1;
}

void p_sent(Leaf a[], int k)
{
    for (int i = 0; i < k; i++)
    {
        printf("%c %d %d ",a[i].cat,a[i].direct_f,a[i].total_f);
    }
}

int collapse(Leaf a[], Leaf b[], int len_a)
{
    int j = 0, k = 0;
    for (int i = 0; i < len_a; i++, j++)
    {
        if ((a[i].cat == 'A') || (a[i].cat == 'J'))     //collapses (A+)(J+)*N in a[] to N in b[] e.g. "the quick brown fox"
        {
            b[j].cat = 'N';
            b[j].F[0] = &a[i];
            b[j].total_f += a[i].total_f;
            k = 1;
            for (k = 1; a[i+k].cat == 'J'; k++)
            {
                b[j].F[k] = &a[k+i];
                b[j].total_f += a[k+i].total_f;
            }
            b[j].F[k] = &a[k+i];
            b[j].total_f += a[i+k].total_f;
            i = k+i;
            b[j].direct_f = k+1;
        }
        else if (a[i].cat == 'V')
        {
            b[j].cat = 'V';
            b[j].F[0] = &a[i];
            b[j].total_f += a[i].total_f;
            if (a[i+1].cat == 'V')                      //collapses V(+V) in a[] to V in b[] e.g. "was going"
            {
                for (k = 1; a[k+i].cat == 'V'; k++)
                {
                    b[j].F[k] = &a[k+i];
                    b[j].total_f += a[k+i].total_f;
                }
                i = k+i-1;
                b[j].direct_f = k;
            }
            else if (a[i+1].cat == 'P')                      //collapses V(+P) in a[] to V in b[] e.g. "jump over"
            {
                for (k = 1; a[k+i].cat == 'P'; k++)
                {
                    b[j].F[k] = &a[k+i];
                    b[j].total_f += a[k+i].total_f;
                }
                i = k+i-1;
                b[j].direct_f = k;
            }
            else if (a[i+1].cat == 'N')                 //collapses V(+N) in a[] to V in b[]
            {
                b[j].F[1] = &a[i+1];
                b[j].total_f += a[++i].total_f;
                b[j].direct_f = 2;
            }
            else if (a[i+1].cat == 'L')                //negation: collapses L+V in a[] to V in b[] e.g. "never does"
            {
                b[j].F[1] = &a[i+1];
                b[j].total_f += 1;
                b[j].direct_f = 2;
                if (a[i+2].cat == 'V')
                {
                    b[j].F[2] = &a[i+2];
                    b[j].total_f += a[i+2].total_f;
                    b[j].direct_f = 3;
                    i += 2;
                }
                else
                {
                    i += 1;
                }
            }
        }
        else if ((a[i].cat == 'N') && (a[i+1].cat == 'V') && (len_a == 2))
        {
            b[j].cat = 'S';
            b[j].F[0] = &a[i];
            b[j].F[1] = &a[i+1];
            b[j].total_f = a[i].total_f + a[i+1].total_f;
            b[j++].direct_f = 2;
            break;
        }
        else
        {
            b[j] = a[i];
            b[j].F[0] = &a[i];
            b[j].total_f = a[i].total_f;
            b[j].direct_f = 1;
        }
    }
    
    return j;    //no. of nodes/elements/leaves in b[]
}

/*void p_Tree(Leaf S[], Leaf L[],int k)
{
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < L[i].direct_f - 1; j++)
        {
            for (int l = 0; l < j; l++)
            {
                
            }
        }
        printf("-");

        for (int l = 0; l < strlen(S[L[i].total_f-1].word)-1; l++)
            printf(" ");
    }
    printf("\n");
    
    for (int i = 0; i < k; i++)
    {
        printf("|");
        for (int j = 0; j < L[i].total_f; j++)
        {
            for (int l = 0; l < strlen(S[j].word)-1; l++)
                printf(" ");
            if (j < L[i].total_f-1) printf(" ");
        }
    }
    printf("\n");
    
    for (int i = 0; i < k; i++)
    {
        printf("%c",L[i].cat);
        for (int j = 0; j < L[i].total_f; j++)
        {
            for (int l = 0; l < strlen(S[j].word)-1; l++)
                printf(" ");
            if (j < L[i].total_f-1) printf(" ");
        }
    }
    printf("\n");
    
    for (int i = 0; i < k; i++)
    {
        printf("|");
        for (int j = 0; j < L[i].total_f; j++)
        {
            for (int l = 0; l < strlen(S[j].word)-1; l++)
                printf(" ");
            if (j < L[i].total_f-1) printf(" ");
        }
    }
    printf("\n");
    
}*/

int main()
{
    Leaf Tree[10][100];
    int n_w = wparse(Tree[0]) + 1;
    int level = 0, total = n_w, temp = 0;
    
    while (1)
    {
        temp = n_w;
        n_w = collapse(Tree[level],Tree[level+1],n_w);      //no. of things in Tree[level+1]
        p_sent(Tree[level+1],n_w);
        printf(" %d \n",n_w);
        
        //p_Tree(Tree[0],Tree[level+1],n_w);
        if ((Tree[level+1][0].total_f == total) || (n_w == temp))
            break;
        else
            level++;
    }
    
    return 0;
}


/*
 NP -> A + J + N
 VP -> V + P
 N -> man
 N -> flower
 */
