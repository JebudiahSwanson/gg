#include <stdio.h>
#include <stdlib.h>

struct RecordType
{
    int id;
    char name;
    int order;
};

struct HashType
{
    struct RecordType *records;
    int count;
};

int hash(int x, int hashSz)
{
    return x % hashSz;
}

int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

void printRecords(struct RecordType pData[], int dataSz)
{
    printf("\nRecords:\n");
    for (int i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    printf("\nRecords in Hash:\n");
    for (int i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].records != NULL)
        {
            printf("Index %d -> ", i);
            for (int j = 0; j < pHashArray[i].count; ++j)
            {
                printf("%d %c %d", pHashArray[i].records[j].id, pHashArray[i].records[j].name, pHashArray[i].records[j].order);
                if (j < pHashArray[i].count - 1)
                    printf(" -> ");
            }
            printf("\n");
        }
    }
}

void insertRecord(struct HashType *hashTable, int hashSz, struct RecordType record)
{
    int index = hash(record.id, hashSz);
    if (hashTable[index].records == NULL)
    {
        hashTable[index].records = (struct RecordType *)malloc(sizeof(struct RecordType));
        hashTable[index].records[0] = record;
        hashTable[index].count = 1;
    }
    else
    {
        int newCount = hashTable[index].count + 1;
        hashTable[index].records = (struct RecordType *)realloc(hashTable[index].records, newCount * sizeof(struct RecordType));
        hashTable[index].records[newCount - 1] = record;
        hashTable[index].count = newCount;
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    int hashSz = 10;
    struct HashType *hashTable = (struct HashType *)malloc(hashSz * sizeof(struct HashType));

    for (int i = 0; i < hashSz; ++i)
    {
        hashTable[i].records = NULL;
        hashTable[i].count = 0;
    }

    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, hashSz, pRecords[i]);
    }

    displayRecordsInHash(hashTable, hashSz);

    for (int i = 0; i < hashSz; ++i)
    {
        free(hashTable[i].records);
    }
    free(hashTable);
    free(pRecords);

    return 0;
}
