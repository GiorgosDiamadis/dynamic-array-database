#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Array
{
    int size;
    int threshHold;
    int capacity;
    int isSorted;
    int nextEmpty;
    struct Student **students;
} ArrayList;

typedef struct Student
{
    long unsigned int AEM;
    short unsigned int courses;
    char name[64];

} student;

void initializeArray(ArrayList **arrayList, char *argv[]);
void initializeStudent(student *student);
void printUppercase(char name[]); /*Print name with all letters uppercased*/
int indexOf(ArrayList *arrayList, long unsigned int AEM);

int add(long unsigned int AEM, short unsigned int courses, char name[64], ArrayList **arrayList);
void addProcess(ArrayList **arrayList);

void printProcess(ArrayList *arrayList, char *separator);

student *find(long unsigned int AEM, ArrayList *arrayList);
student *binaryFind(long unsigned int AEM, int low, int high, ArrayList *arrayList); /*Binary search*/
void findProcess(ArrayList *arrayList);

int mod(ArrayList *arrayList, long unsigned int AEM, short unsigned int courses);
void modProcess(ArrayList *arrayList);

int rmv(ArrayList **arrayList, long unsigned int AEM);
void removeProcess(ArrayList **arrayList);

void clear(ArrayList **arrayList);

void sort(ArrayList *arrayList);

int main(int argc, char *argv[])
{

    ArrayList *arrayList = NULL;
    char input;
    char *separator = "\n#\n";

    initializeArray(&arrayList, argv);

    while (scanf("%c", &input) && input != 'q')
    {
        if (input == 'a')
        {
            addProcess(&arrayList);
        }
        else if (input == 'p')
        {
            printProcess(arrayList, separator);
        }
        else if (input == 'f')
        {
            findProcess(arrayList);
        }
        else if (input == 'm')
        {
            modProcess(arrayList);
        }
        else if (input == 'r')
        {
            removeProcess(&arrayList);
        }
        else if (input == 'c')
        {
            clear(&arrayList);
        }
        else if (input == 's')
        {
            sort(arrayList);
        }
    }

    clear(&arrayList);
    free(arrayList);

    return 0;
}

void sort(ArrayList *arrayList)
{
    for (int i = 1; i < arrayList->capacity; i++)
    {
        if (arrayList->students[i] != NULL)
        {
            int j = i - 1;

            student *st = arrayList->students[i];

            while (j > -1)
            {
                if (arrayList->students[j] != NULL)
                {
                    if (arrayList->students[j]->AEM > st->AEM)
                    {
                        arrayList->students[j + 1] = arrayList->students[j];
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    arrayList->students[j + 1] = arrayList->students[j];
                }
                j--;
            }
            arrayList->students[j + 1] = st;
        }
    }

    printf("\nS-OK\n");
    arrayList->isSorted = 1;
}

void clear(ArrayList **arrayList)
{
    for (int i = 0; i < (*arrayList)->capacity; i++)
    {
        if ((*arrayList)->students[i] != NULL)
        {
            /*Free allocated memory from students*/
            free((*arrayList)->students[i]);
            (*arrayList)->students[i] = NULL;
        }
    }

    /*Free allocated memory from array slots*/
    if ((*arrayList)->capacity != 0)
        free((*arrayList)->students);

    (*arrayList)->capacity = 0;
    (*arrayList)->size = 0;
    (*arrayList)->nextEmpty = 0;

    printf("\nC-OK\n");
}

void removeProcess(ArrayList **arrayList)
{
    long unsigned int AEM;

    scanf("%ld", &AEM);
    if (rmv(arrayList, AEM))
    {
        printf("\nR-OK %ld, %d %d\n", AEM, (*arrayList)->size, (*arrayList)->capacity);
    }
    else
    {
        printf("\nR-NOK %ld, %d %d\n", AEM, (*arrayList)->size, (*arrayList)->capacity);
    }
}

int rmv(ArrayList **arrayList, long unsigned int AEM)
{
    int index = indexOf((*arrayList), AEM);
    if (index == -1)
    {
        return 0;
    }

    free((*arrayList)->students[index]);  /*Free memory allocated from student*/
    (*arrayList)->students[index] = NULL; /*The pointer that pointed to the student now points to Null*/
    (*arrayList)->size--;
    (*arrayList)->nextEmpty = index;

    int emptySlots = (*arrayList)->capacity - (*arrayList)->size;

    if (emptySlots == (*arrayList)->capacity)
    {
        (*arrayList)->nextEmpty = 0;
        (*arrayList)->capacity = 0;
        (*arrayList)->size = 0;
        return 1;
    }

    if (emptySlots > (*arrayList)->threshHold) /*Are there more than K empty slots*/
    {
        int j = 0;
        int newCapacity = (*arrayList)->capacity - (*arrayList)->threshHold; /*Decrease by K*/

        /*Copy last K elements to the K first empty slots*/
        for (int i = newCapacity; i < (*arrayList)->capacity; i++)
        {
            if ((*arrayList)->students[i] != NULL)
            {
                student *st = (*arrayList)->students[i];
                while ((*arrayList)->students[j] && j < newCapacity)
                {
                    j++;
                }
                (*arrayList)->students[j] = st;
            }
        }
        student **temp = (student **)realloc((*arrayList)->students, newCapacity * sizeof(student *));

        if (temp == NULL) /*realloc might return null*/
        {
            return -1;
        }
        else
        {
            (*arrayList)->students = temp;
        }

        (*arrayList)->capacity = newCapacity;

        for (int i = 0; i < (*arrayList)->capacity; i++)
        {
            if ((*arrayList)->students[i] == NULL)
            {
                (*arrayList)->nextEmpty = i;
                break;
            }
        }
    }

    return 1;
}

int indexOf(ArrayList *arrayList, long unsigned int AEM)
{
    for (int i = 0; i < arrayList->capacity; i++)
    {
        if (arrayList->students[i] != NULL)
        {
            if (arrayList->students[i]->AEM == AEM)
            {
                return i;
            }
        }
    }
    return -1;
}
void modProcess(ArrayList *arrayList)
{
    long unsigned int AEM;
    short unsigned int courses;

    scanf("%ld %hd", &AEM, &courses);
    if (mod(arrayList, AEM, courses))
    {
        printf("\nM-OK %ld\n", AEM);
    }
    else
    {
        printf("\nM-NOK %ld\n", AEM);
    }
}
int mod(ArrayList *arrayList, long unsigned int AEM, short unsigned int courses)
{
    student *st = find(AEM, arrayList);

    if (st != NULL)
    {
        st->courses = courses;
        return 1;
    }
    else
    {
        return 0;
    }
}
void findProcess(ArrayList *arrayList)
{
    long unsigned int AEM;
    scanf("%ld", &AEM);
    student *st;

    if (!arrayList->isSorted)
    {
        st = find(AEM, arrayList);
    }
    else
    {
        st = binaryFind(AEM, 0, arrayList->size - 1, arrayList);
    }

    if (st != NULL)
    {
        printf("\nF-OK ");

        printUppercase(st->name);

        printf(" %hd\n", st->courses);
    }
    else
    {
        printf("\nF-NOK %ld\n", AEM);
    }
}

student *find(long unsigned int AEM, ArrayList *arrayList)
{
    if (arrayList->size == 0)
    {
        return NULL;
    }

    if (!arrayList->isSorted) /*Linear search*/
    {
        for (int i = 0; i < arrayList->capacity; i++)
        {
            if (arrayList->students[i] == NULL)
                continue;

            if (arrayList->students[i]->AEM == AEM)
            {
                return arrayList->students[i];
            }
        }
    }

    return NULL;
}

student *binaryFind(long unsigned int AEM, int low, int high, ArrayList *arrayList)
{
    if (arrayList->size == 0)
    {
        return NULL;
    }
    int mid = (low + high) / 2;

    student *s = arrayList->students[mid];
    int i = 0;

    if (s == NULL)
    {
        while (s == NULL)
        {
            s = arrayList->students[mid - i];

            if (s != NULL)
            {
                break;
            }

            s = arrayList->students[mid + i];

            if (s != NULL)
            {
                break;
            }
            i++;
        }
    }

    if (high >= low)
    {

        if (s->AEM == AEM)
        {
            return s;
        }

        if (s->AEM < AEM)
        {
            return binaryFind(AEM, mid + 1, high, arrayList);
        }
        else if (s->AEM > AEM)
        {
            return binaryFind(AEM, low, mid - 1, arrayList);
        }
    }
    return NULL;
}
void printProcess(ArrayList *arrayList, char *separator)
{
    printf("%s", separator);

    if (arrayList == NULL)
    {
        return;
    }

    if (arrayList->students == NULL)
    {
        return;
    }

    for (int i = 0; i < arrayList->capacity; i++)
    {
        if (arrayList->students[i] != NULL)
        {
            printf("%ld ", arrayList->students[i]->AEM);

            printUppercase(arrayList->students[i]->name);

            printf(" %hd\n", arrayList->students[i]->courses);
        }
        // else
        // {
        //     printf("---\n");
        // }
    }
}

void addProcess(ArrayList **arrayList)
{
    long unsigned int AEM;
    short unsigned int courses;
    char name[64];

    scanf("%ld %s %hd", &AEM, name, &courses);
    name[strlen(name)] = '\0';

    if (add(AEM, courses, name, arrayList) == 1) /*If add succeeds*/
    {
        printf("\nA-OK %ld, %d %d\n", AEM, (*arrayList)->size, (*arrayList)->capacity);
    }
    else
    {
        printf("\nA-NOK %ld, %d %d\n", AEM, (*arrayList)->size, (*arrayList)->capacity);
    }

    (*arrayList)->isSorted = 0;
}

int add(long unsigned int AEM, short unsigned int courses, char name[64], ArrayList **arrayList)
{
    int nextEmpty = (*arrayList)->nextEmpty;
    //Check if this student already exists
    if (find(AEM, *arrayList) != NULL)
    {

        return -1;
    }

    //Check if more space is needed
    if ((*arrayList)->size == (*arrayList)->capacity)
    {
        student **temp;
        int newCapacity = (*arrayList)->capacity + (*arrayList)->threshHold;

        if ((*arrayList)->capacity == 0)
        {
            temp = (student **)malloc(newCapacity * sizeof(student *));
        }
        else
        {
            temp = (student **)realloc((*arrayList)->students, newCapacity * sizeof(student *));
        }

        if (temp == NULL) /*realloc might return null*/
        {
            return -1;
        }
        else
        {
            (*arrayList)->students = temp;
            for (int i = (*arrayList)->capacity; i < newCapacity; i++)
            {
                (*arrayList)->students[i] = NULL;
            }
        }

        (*arrayList)->capacity = newCapacity;
    }

    /*Allocate memory for new student*/
    (*arrayList)->students[nextEmpty] = (student *)malloc(sizeof(student));

    if ((*arrayList)->students[nextEmpty] == NULL)
    {
        return -1;
    }

    initializeStudent((*arrayList)->students[nextEmpty]);

    (*arrayList)->students[nextEmpty]->AEM = AEM;
    (*arrayList)->students[nextEmpty]->courses = courses;
    strcpy((*arrayList)->students[nextEmpty]->name, name);

    (*arrayList)->size++;

    (*arrayList)->nextEmpty++;

    if ((*arrayList)->students[(*arrayList)->nextEmpty] != NULL)
    {
        while ((*arrayList)->students[(*arrayList)->nextEmpty] != NULL)
        {
            (*arrayList)->nextEmpty++;
        }
    }

    // for (int i = 0; i < (*arrayList)->capacity; i++)
    // {
    //     if ((*arrayList)->students[i] == NULL)
    //     {
    //         (*arrayList)->nextEmpty = i;
    //     }
    // }

    return 1;
}

void initializeStudent(student *student)
{
    student->AEM = 0;
    student->courses = 0;
    for (int i = 0; i < 64; i++)
    {
        student->name[i] = '\0';
    }
}
void initializeArray(ArrayList **arrayList, char *argv[])
{
    (*arrayList) = (ArrayList *)malloc(sizeof(ArrayList));

    (*arrayList)->size = 0;
    (*arrayList)->isSorted = 0;
    (*arrayList)->nextEmpty = 0;

    (*arrayList)->threshHold = atoi(argv[2]);
    (*arrayList)->capacity = atoi(argv[1]);

    (*arrayList)->students = NULL;

    if ((*arrayList)->capacity > 0)
    {
        (*arrayList)->students = (student **)malloc((*arrayList)->capacity * sizeof(student *));

        for (int i = 0; i < (*arrayList)->capacity; i++)
        {
            (*arrayList)->students[i] = NULL;
        }
    }
}

void printUppercase(char name[])
{
    for (int i = 0; i < strlen(name); i++)
    {
        printf("%c", toupper(name[i]));
    }
}
