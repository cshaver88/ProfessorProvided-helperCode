main()
{
    char temp;
    int i = 0;
    char Strings[NUM][LEN];


    printf("Please enter %d strings, one per line:\n", NUM);
    for(i; i<25; i++){
        fgets(&Strings[i][0], LEN,stdin);
    }


    i=0;
    puts("\nHere are the strings in the order you entered:");
    for (i; i<25; i++){
        printf("%s\n",Strings[i]);
    }

    mergesort(Strings,NUM);

    i=0;
    puts("\nHere are the strings in alphabetical order");
    for (i; i<25; i++){
        printf("%s\n",Strings[i]);
    }
}

int mergesort(char list[NUM][LEN], int length) // First part
{
    mergesort_r(0, length, list);
    return 0;
}

int mergesort_r(int left, int right, char list[NUM][LEN]) // Overloaded portion
{
    if (right - left <= 1)
    {
        return 0;
    }


    int left_start  = left;
    int left_end    = (left+right)/2;
    int right_start = left_end;
    int right_end   = right;


    mergesort_r( left_start, left_end, list);

    mergesort_r( right_start, right_end, list);


    merge(list, left_start, left_end, right_start, right_end);
}

int merge(char list[NUM][LEN], int left_start, int left_end, int right_start, int      right_end)
{

    int left_length = left_end - left_start;
    int right_length = right_end - right_start;


    char *left_half[left_length];
    char *right_half[right_length];

    int r = 0;
    int l = 0;
    int i = 0;


    for (i = left_start; i < left_end; i++, l++)
    {
        strcpy(left_half[l], list[i]);
    }


    for (i = right_start; i < right_end; i++, r++)
    {
        strcpy(right_half[r], list[i]);
    }


    for ( i = left_start, r = 0, l = 0; l < left_length && r < right_length; i++)
    {
        if ( strcmp(left_half[l], right_half[r])<0 ) 
        { strcpy(list[i], left_half[l++]); }
        else { strcpy(list[i], right_half[r++]); }
    }


    for ( ; l < left_length; i++, l++) { strcpy(list[i], left_half[l]); }
    for ( ; r < right_length; i++, r++) { strcpy(list[i], right_half[r]); }
    return 0;
}
