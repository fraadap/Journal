# Journal

Journal è un progetto scritto in C che permette (partendo da un file di testo e dei parametri) di incolonnare un testo in stile giornale.

## Installation

Per compilare il programma basta posizionarsi con il terminale nella cartella in cui è salvato e digitare il comando:
```make``` 
Verrà generato un eseguibile all'interno della stessa cartella.

Per cancellare l'eseguibile è possibile utilizzare il comando:
```bash
make clean
``` 

## Usage

Per eseguire il programma basta digitare da terminale:

```bash
./journal
``` 
Seguito dai seguenti parametri:

- Nome del file (specificando il path relativo o assoluto): stringa
- il numero (>0) di righe per colonna (lunghezza di una colonna): int
- il numero (>0) di caratteri per colonna; (larghezza della singola colonna): int   
- il numero (>0) di colonne per pagina;    (larghezza della pagina): int      
- il numero (>0) di spazi delimitatori tra le colonne (distanza tra le colonne): int

Un eventuale **errore** in fase di inserimento sarà segnalato a schermo specificando quale parametro inserito non è stato approvato.

Possibili problemi che si potrebbero presentare nell'inserimento dei parametri:
- impossibile aprire il file con il nome fornito in input
- al posto del nome del file è stato digitato un carattere numerico
- al posto di un intero è stato digitato un carattere o una stringa
- l'intero corrispondente a un parametro numerico risulta <= 0
- All'interno del file è stata trovata una parola di lunghezza maggiore rispetto al parametro della larghezza della colonna 

## Examples

Per esempio il seguente codice esegue il programma fornendo:
- Nome del file -> 'file.txt';
- Numero di righe per colonna -> 10;
- Numero di caratteri per colonna -> 25;
- Numero di colonne per pagina -> 5
- Numero di spazi delimitatori tra le colonne -> 8

```bash
./journal file.txt 10 25 5 8
``` 

## Composition

Il progetto è formato da 5 file: 

- journal.c 
    E' il file in cui è situtato il main del programma, legge il file di input e compone le singole righe che andranno poi a comporre le singole righe delle colonne nel file di output.

- struct.c 
    E' il file in cui vengono gestite tutte le operazioni relative la gestione della struttura dati 
    (inserimento di righe, creazione colonne, liberazione della memoria, conto di righe e colonne).
    Una funzione di questo file si occupa anche della scrittura su file di una pagina quando ne è terminata la composizione. 

- manage_text.c
    All'interno di questo file ci sono le funzioni per la manipolazione delle stringhe 
    (allineamento, conto dei caratteri effettivi, controllo se un carattere è UTF-8 o meno e anche la copia di una sottostringa).

- struct.h
    File header in cui si definiscono le struct dei nodi delle liste puntate delle colonne e delle righe. 
    Sono presenti le signatures delle funzioni che vengono implementate nel file struct.c

- manage_text.h
    Sono presenti le signatures delle funzioni che vengono implementate nel file manage_text.c

## Versions

#### monoprocesso
#### multiprocesso

Queste due versioni si trovano rispettivamente nella cartella singleProcess e multiProcess.

Il funzionamento, la struttura e l'algoritmo cardine è identico, si differenziano per l'uso di un processi e 3 processi concorrenti.

Il multiprocesso è stato implementato nel file journal.c e nel file struct.c. tramite 3 processi: il processo padre e quello figlio.

Il padre legge dal file input un byte e lo scrive in una pipe;

il figlio legge un byte alla volta tramite la pipe, gestisce la riga e la passa richiama la funzione addRow che permette di aggiungere una riga alla struttura dati. 

Se il numero massimo di colonne della pagina e di righe dell'ultima colonna sono stati raggiunti allora il processo figlio crea un processo figlio che si occupa di scrivere la pagina mentre suo padre continua a formattare righe.

## Libraries used

Tutte le librerie utilizzate sono standard C.

**stdio.h**: Libreria per le operazioni di input/output standard. Utilizzata per printf().

**stdlib.h**: Libreria che fornisce funzioni per l'allocazione di memoria dinamica, la gestione degli errori e altre funzioni di utilità. Utilizzata per malloc() e exit().

**string.h**: Libreria che contiene funzioni per la manipolazione di stringhe. Utilizzata per strcpy() e strlen().

**unistd.h**: Libreria per l'accesso alle funzioni del sistema operativo Unix, come la gestione dei processi e delle pipe. Utilizzata per fork() e pipe().

**sys/wait.h**: Libreria per la gestione dei processi, utilizzata per la funzione wait() per attendere la terminazione di un processo figlio.

**fcntl.h**: Libreria per la gestione dei descrittori di file, inclusa la funzione. Utilizzata per open() per aprire e creare file.


_@author: Francesco D'Aprile_