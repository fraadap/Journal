#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include "struct.h"
#include "manage_text.h"

struct ColumnNode *newColumn(struct ColumnNode *lastColumn) // crea una nuova colonna, prende in input la colonna a cui puntarla altrimenti null per una prima colonna di una pagina
{
    struct ColumnNode *column = (struct ColumnNode *)malloc(sizeof(struct ColumnNode));
    column->row = NULL;
    column->next = NULL;

    if (lastColumn == NULL)
    {
        return column;
    }
    else
    {
        lastColumn->next = column;
        return lastColumn->next;
    }
}

void newRow(struct ColumnNode *column, char *testo, int cpc)
{
    struct RowNode *newRow = (struct RowNode *)malloc(sizeof(struct RowNode));
    struct RowNode *ultimaRiga = column->row;

    newRow->testo = (char *)malloc((cpc + 1) * sizeof(char)); // +1 per il terminatore di stringa '\0'
    // copia del testo in riga->testo

    strncpy(newRow->testo, testo, cpc);
    newRow->testo[cpc] = '\0'; // assicurarsi che il testo termini con '\0'
    newRow->next = NULL;

    if (ultimaRiga == NULL) // se non ci sono righe nella colonna la aggiungo
    {
        column->row = newRow;
    }
    else // sennò la aggiungo dopo l'ultima riga
    {
        while (ultimaRiga->next != NULL)
        {
            ultimaRiga = ultimaRiga->next;
        }
        ultimaRiga->next = newRow;
    }
}

int countRows(struct ColumnNode *column) // conta le righe di una colonna
{
    int i = 0;
    struct RowNode *row = column->row;
    while (row != NULL)
    {
        i++;
        row = row->next;
    }
    return i;
}

int countColums(struct ColumnNode *column) // conta le colonne di una pagina passandogli la prima colonna
{
    int i = 0;
    struct ColumnNode *col = column;
    while (col != NULL)
    {
        i++;
        col = col->next;
    }
    return i;
}

void free_albero_colonne(struct ColumnNode *firstColumn) // fa il free di tutte le colonne all'interno di una pagina, liberando lo spazio occupato dalle righe e dalle colonne
{
    struct ColumnNode *colonnaCorrente = firstColumn;

    while (colonnaCorrente != NULL)
    {
        struct RowNode *rigaCorrente = colonnaCorrente->row;

        while (rigaCorrente != NULL)
        {
            struct RowNode *rigaSuccessiva = rigaCorrente->next;
            free(rigaCorrente->testo);
            free(rigaCorrente);
            rigaCorrente = rigaSuccessiva;
        }

        struct ColumnNode *colonnaSuccessiva = colonnaCorrente->next;
        free(colonnaCorrente);
        colonnaCorrente = colonnaSuccessiva;
    }
}

void writePage(struct ColumnNode *firstColumn, char *nomeFile, int dcc) // scrive su file la pagina
{

    int nRighe = countRows(firstColumn);
    struct ColumnNode *column;
    struct RowNode *row;
    int end = 0;

    FILE *file = fopen(nomeFile, "a"); // Apre il file in modalità "append" perché stampa una pagina per chiamata di funzione

    if (file == NULL)
    {
        printf("Impossibile aprire il file di output%s\n", nomeFile);
        return;
    }

    for (int i = 0; i < nRighe; i++)
    {
        column = firstColumn;  // si parte dalla prima colonna della pagina
        while (column != NULL) // per ogni colonna si stampa la riga i-esima scorrendo le colonne e le righe delle colonne
        {
            row = column->row;
            end = 0;
            for (int j = 0; j < i; j++)
            {
                if (row->next != NULL)
                {
                    row = row->next;
                }
                else
                {
                    end = 1;
                    break;
                }
            }
            // l'ultima colonna dell'ultima pagina potrebbe avere meno righe, finite le righe non stampa più
            if (end != 1)
            {
                fprintf(file, "%s", row->testo);
            }

            if (column->next != NULL) // aggiungo gli spazi distanziatori tra le colonne solo se non è l'ultima colonna
            {
                for (int j = 0; j < dcc; j++)
                {
                    fprintf(file, "%s", " ");
                }
            }

            column = column->next;
        }
        fprintf(file, "%s", "\n"); // delimitatore di pagina
    }
    fprintf(file, "%s", "\n%%%\n");
    fclose(file);
}

// input: testo della riga, righe per colonna, prima colonna della pagina, colonna per pagina, flag se il file è finito, distanza tra le colonne, nome del file di output
// questa funzione aggiunge il testo della riga alla struttura dati, se la colonna dove viene inserita la riga è piena viene creata una nuova colonna e collegata alla precedente,
// se il numero di colonne create è proprio cpp (il file è finito) allora chiama la funzione di stampa e liberazione della memoria.
struct ColumnNode *addRow(char *bufferRow, int rpc, struct ColumnNode *firstColumn, int cpp, int end, int dcc, char *outputFile)
{
    struct ColumnNode *column = firstColumn;

    while (column->next != NULL) // si prende l'ultima colonna
    {
        column = column->next;
    }

    if (countRows(column) == rpc) // se la colonna è piena
    {

        column = newColumn(column); // si crea una nuova colonna
    }
    int caratteri = strlen(bufferRow);
    newRow(column, bufferRow, caratteri); // si crea la riga con il testo passato in input e si mette dentro l'ultima colonna

    if ((countColums(firstColumn) == cpp && countRows(column) == rpc) || end == 1) // se tutte le colonne sono piene o il file è finito
    {

        writePage(firstColumn, outputFile, dcc); // scrivo la pagina su file
        free_albero_colonne(firstColumn);        // libero tutta la memoria

        if (end == 0) // se il file non è finito
        {
            firstColumn = newColumn(NULL); // creo una colonna che sarà la nuova prima colonna della nuova pagina e la restituisco
        }
    }
    return firstColumn; // se non scrivo su file la pagina primaColonnna rimarrà invariata
}