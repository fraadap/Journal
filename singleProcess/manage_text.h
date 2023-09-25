#include <stdio.h>
// commentare

int strlen_UTF8(char *text); // restituisce il numero di caratteri effettivi

int isUTF8(char c); //restituisce 1 se il byte è di un carattere utf e non è il primo byte del carattere

char *alignString(char *str, int totalLength, int endParagraph); // totalLength è il numero di caratteri per colonna, fineParagrafo è il flag per la fine del paragrafo

    // allinea il testo di una riga, solo a sinistra se è una sola parola o la fine di un paragrafo aggiungendo spazi a riempimento
    // oppure allinea a destra e sinistra aggiungendo gli opportuni spazi


void copySubstring(const char *source, char *destination, int startIndex); // copia la prima stringa da un certo indice in poi nella seconda