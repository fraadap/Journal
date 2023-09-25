#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "struct.h"
#include "manage_text.h"

#define OUTPUT_FILE_NAME "output.txt"

int main(int argc, char *argv[])
{
    // controllo dei parametri passati in input
    if (argc != 6)
    { // istruzioni per l'uso mostrate con lunghezza di input = 0 o comunque minore del necessario
        printf("Istruzioni per l'uso:\n\nInserire 5 parametri in input rispettivamente:\n"
               "\t1) stringa: nome del file di input;              (con path relativo o assoluto)\n"
               "\t2) int: il numero (>0) di righe per colonna;     (lunghezza di una colonna)\n"
               "\t3) int: il numero (>0) di caratteri per colonna; (larghezza della singola colonna)\n"
               "\t4) int: il numero (>0) di colonne per pagina;    (larghezza della pagina)\n"
               "\t5) int: il numero (>0) di spazi delimitatori tra le colonne;\n\n"
               "per maggiori informazioni consultare il file README.txt\n");
        exit(EXIT_FAILURE);
    }

    const char *fileName = argv[1];
    int params[4];
    short int error = 0;
    for (int i = 0; i < 4; i++)
    {
        if (atoi(argv[i + 2]) < 1)
        { // controllo se il parametro sia un numero <1 o un carattere
            printf("errore nell'inserimento del parametro numerico %d\n", i + 1);
            error = 1;
        }
        params[i] = atoi(argv[i + 2]);
    }

    if (error == 1)
    {
        exit(EXIT_FAILURE);
    }

    FILE *f;
    f = fopen(OUTPUT_FILE_NAME, "w");   //crea il file di output se non esiste e lo svuota prima se già esiste
    fclose(f);

    // rpc=righe per colonne, cpc = caratteri per colonna,
    // cpp= colonne per pagina, dcc=distanza colonne

    int rpc = params[0]; // i valori vengono salvati in delle variabili solo per leggibilità del codice
    int cpc = params[1];
    int cpp = params[2];
    int dcc = params[3];

    int pipefd[2]; // dichiarazione pipe
    pid_t pid;     // dichiarazione variabile contenente process id

    int file = open(fileName, O_RDONLY); // apertura del file in sola lettura
    if (file == -1)                      // ricezione eccezione errore apertura del file e restituzione dell'errore
    {
        perror("Errore nell'apertura del file");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1) // ricezione eccezione errore crezione pipe e restituzione errore
    {
        perror("Errore nella creazione della pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();  // creo il processo figlio
    if (pid == -1) // controllo eventuali errori avvenuti facendo la fork e restituisco l'errore
    {
        perror("Errore nella fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) // Codice del processo figlio
    {
        struct ColumnNode *firstColumn = newColumn(NULL); // creazione della prima colonna

        int endParagraph = 0; // flag per rilevare la fine del paragrafo
        int last = 0;         // flag per rilevare che si è letto l'ultimo carattere del file
        int nBytesSaved = 0;  // numero di byte nel buffer d'appoggio
        int space = -1;       // indice dell'ultimo spazio trovato nel buffer di appoggio

        // allocazione memoria per il buffer di appoggio che rappresenta la riga, con lunghezza
        // con lunghezza numero dei caratteri della riga+1 (per \0) *4 (poiché i caratteri sono UTF-8)
        char *bufferRow = malloc((cpc + 1) * 4 * sizeof(char));

        int blankLine = 0; // flag per rilevare che più \n consecutivi generino più righe vuote
        char c;            // carattere fornito tramite la pipe dal processo padre di lettura del file

        do
        {
            read(pipefd[0], &c, 1); // lettura bloccante dalla pipe

            if (nBytesSaved == 0 && c == ' ') // se il primo carattere della riga è uno spazio lo salto
            {
                continue;
            }

            if (c == '\0') // se il carattere è \0 imposto il flag
            {

                last = 1;
            }

            if (c == ' ' || c == '\n') // se il carattere è spazio o \n salvo l'indice per reset
            {
                space = nBytesSaved;
            }

            if (c == '\n' || last == 1) // se il carattere è uno \n o è l'ultimo carattere dovrò allineare la riga solo a sinistra
            {
                endParagraph = 1;
            }
            else
            {
                endParagraph = 0;
            }

            bufferRow[nBytesSaved] = '\0'; // aggiungo la fine della stringa

            if ((strlen_UTF8(bufferRow) == cpc && isUTF8(c) == 0) || c == '\n' || last == 1) // se i lunghezza dei caratteri effettivi della riga è cpc o è uno \n o è l'ultimo carattere
            {

                if (c == '\n' && nBytesSaved == 0) // caso in cui ci sono stati 2 o pi \n di seguito
                {

                    if (blankLine == 0) // controllo se non ha già scritto la riga vuota del secondo \n
                    {
                        bufferRow[0] = '\0';
                        blankLine = 1;
                    }
                    else
                    {
                        // non devo stampare un'ulteriore riga vuota
                        continue;
                    }
                }
                else
                {
                    blankLine = 0;
                }

                if (space == -1 && c != ' ' && endParagraph != 1) // se non ho trovato uno spazio nella frase, il nuovo carattere non è uno spazio e non è la fine di paragrafo
                {
                    printf("parola che inizia con '%s' e' troppo lunga \n", bufferRow); // restituisco l'iniziale della parola che non entra perche della lunghezza maggiore di cpc

                    return 1;
                }

                if (c != ' ' && c != '\n' && last == 0) // controllo se l'ultimo carattere è spazio per resettare
                {
                    char *temp = malloc((cpc + 1) * 4 * sizeof(char)); // allocazione di una stringa temporanea
                    copySubstring(bufferRow, temp, space + 1);         // copio in temp dall'ultimo spazio trovato fino alla fine
                    bufferRow[space] = '\0';                           // taglio bufferRow fino all'ultimo spazio

                    bufferRow = alignString(bufferRow, cpc, endParagraph);                               // allineo la riga
                    firstColumn = addRow(bufferRow, rpc, firstColumn, cpp, last, dcc, OUTPUT_FILE_NAME); // aggiungo la riga alla struttura dati, in caso di totalità della pagina questa funzione resituirà un'altro puntatore a colonna
                    free(bufferRow);                                                                     // libero bufferRow per poi salvarci temp e l'ultimo carattere letto
                    bufferRow = temp;
                    nBytesSaved = strlen(temp);
                    bufferRow[nBytesSaved] = c;
                    nBytesSaved++;
                }
                else
                {
                    bufferRow[space] = '\0';                               // elimino eventuale spazio o \n aggiunto alla fine a bufferRow
                    bufferRow = alignString(bufferRow, cpc, endParagraph); // allineo la riga
                    firstColumn = addRow(bufferRow, rpc, firstColumn, cpp, last, dcc, OUTPUT_FILE_NAME);
                    nBytesSaved = 0;
                    free(bufferRow);
                    if (last == 0)
                    {
                        char *bufferRow = malloc((cpc + 1) * 4 * sizeof(char)); // viene allocato nuovamente bufferRow vuoto
                    }
                }

                space = -1;
            }
            else // se i caratteri effettivi della buffer riga sono minori dei caratteri da scrivere
            {
                bufferRow[nBytesSaved] = c; // salvo il carattere nella riga
                nBytesSaved++;

                if (last == 1) // se è l'ultimo forzo ad aggiungere la riga alla struttura dati
                {
                    bufferRow = alignString(bufferRow, cpc, endParagraph);
                    firstColumn = addRow(bufferRow, rpc, firstColumn, cpp, last, dcc, OUTPUT_FILE_NAME);
                }
            }

        } while (c != '\0');
        wait(NULL);       // aspetta un eventuale processo figlio
        close(pipefd[0]); // Chiude il lato di lettura della pipe nel processo figlio
        close(file);      // Chiude il file nel processo figlio
        exit(EXIT_SUCCESS);
    }
    else // Siamo nel processo padre di lettura
    {
        char character;

        while (read(file, &character, 1) > 0)
        { // legge dal file ogni byte e lo scrive sulla pipe
            write(pipefd[1], &character, 1);
        }

        // Inserimento del carattere '\0' nella pipe per segnalare la fine del file
        char endOfFile = '\0';
        write(pipefd[1], &endOfFile, 1);

        close(pipefd[1]); // Chiude il lato di scrittura della pipe nel processo padre
        close(file);      // Chiude il file nel processo padre
        wait(NULL);       // Aspetta la terminazione del processo figlio
        exit(EXIT_SUCCESS);
    }
    return 0;
}
