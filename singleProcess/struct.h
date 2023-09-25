// commentare
struct ColumnNode // struct di un nodo di una lista puntata di colonne, punta alla sua prima riga e alla colonna successiva
{
    struct RowNode *row;
    struct ColumnNode *next;
};

struct RowNode // struct di un nodo di una lista puntata di righe, punta al primo carattere del suo testo e alla riga successiva
{
    char *testo;
    struct RowNode *next;
};

struct ColumnNode *newColumn(struct ColumnNode *lastColumn); //crea una nuova colonna, prende in input la colonna a cui puntarla altrimenti null per una prima colonna di una pagina


void newRow(struct ColumnNode *column, char *testo, int cpc);

int countRows(struct ColumnNode *column); //conta le righe di una colonna

int countColums(struct ColumnNode *column); //conta le colonne di una pagina passandogli la prima colonna

void free_albero_colonne(struct ColumnNode *firstColumn); //fa il free di tutte le colonne all'interno di una pagina, liberando lo spazio occupato dalle righe e dalle colonne

void writePage(struct ColumnNode *firstColumn, char *nomeFile, int dcc); //scrive su file la pagina

//input: testo della riga, righe per colonna, prima colonna della pagina, colonna per pagina, flag se il file è finito, distanza tra le colonne, nome del file di output
//questa funzione aggiunge il testo della riga alla struttura dati, se la colonna dove viene inserita la riga è piena viene creata una nuova colonna e collegata alla precedente,
//se il numero di colonne create è proprio cpp (il file è finito) allora chiama la funzione di stampa e liberazione della memoria utilizzando un altro processo. Mentre il processo padre prosegue.
struct ColumnNode *addRow(char *bufferRow, int rpc, struct ColumnNode *firstColumn, int cpp, int end, int dcc, char *outputFile);
