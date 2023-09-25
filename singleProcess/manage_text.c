#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "manage_text.h"

int strlen_UTF8(char *text) // restituisce il numero di caratteri effettivi
{
    int count = 0;
    const unsigned char *p = (const unsigned char *)text;
    while (*p != '\0')
    { 
        if ((*p & 0xC0) != 0x80) // inizio della serie di byte (fino a 4) o carattere che occupa 1 byte
        {

            count++;
        }
        p++;
    }
    return count;
}

int isUTF8(char c){ //restituisce 1 se il byte è di un carattere utf e non è il primo byte del carattere
    unsigned char p = (const unsigned char )c;
    if ((p & 0xC0) == 0x80){
        return 1;
    }
    return 0;
}

char *alignString(char *str, int totalLength, int endParagraph) // totalLength è il numero di caratteri per colonna, endParagraph è il flag per la fine del paragrafo
{
    // allinea il testo di una riga, solo a sinistra se è una sola parola o la fine di un paragrafo aggiungendo spazi a riempimento
    // oppure allinea a destra e sinistra aggiungendo gli opportuni spazi

    if (strlen_UTF8(str) == totalLength)
    {
        return str; // str
    }
    int p =endParagraph;
    int words = 0;
    int length_UTF8 = strlen_UTF8(str);
    int length = strlen(str);
    int index_spazi[totalLength - 1];                       // array in cui memorizzo l'indice della stringa in cui trovo uno spazio
    memset(index_spazi, -1, sizeof(int) * totalLength - 1); // lo inizializzo a -1

    for (int i = 0; i < length; i++) // riempio index_spazi
    {

        if (str[i] == ' ' && str[i + 1] != ' ')
        {
            index_spazi[words] = i;
            words++;
        }
    }

    char *alignedStr = malloc((totalLength + 1) * 4 * sizeof(char)); // alloco la stringa risultato

    if (words == 0 || p == 1) // allineo solo a sinistra
    {
        int nSpacesToInsert = totalLength - length_UTF8; // calcolo quanti spazi devo inserire
        int i;
        for (int j = 0; j < length; j++) // copio l'intera stringa
        {
            alignedStr[j] = str[j];
        }

        for (i = length; i < length + nSpacesToInsert; i++) // aggiungo gli spazi necessari alla fine
        {

            alignedStr[i] = ' ';
        }
        alignedStr[i] = '\0';
        return alignedStr;
    }

    int nSpacesToInsert = totalLength - length_UTF8;
    int spacesPerWord = nSpacesToInsert / (words); // Numero di spazi da inserire tra ogni parola
    int extraSpaces = nSpacesToInsert % (words);   // Spazi extra da inserire a partire dalla prima parola

    int j = 0;         // contatore che tiene conto della stringa allineata
    int wordCount = 0; // Contatore delle parole
    for (int i = 0; i < length; i++)
    {
        alignedStr[j] = str[i]; // Copia i caratteri della parola nella nuova posizione
        j++;
        int consider = 0;
        if (str[i] == ' ') // quando trovo uno spazio nella stringa
        {
            for (int k = 0; k < words; k++)
            {
                if (index_spazi[k] == -1)
                {
                    break;
                }

                if (i == index_spazi[k]) // controllo che l'indice i sia proprio il k-essimo spazio
                {
                    consider = 1;
                    break;
                }
            }
        }
        if (consider == 1)
        {

            wordCount++;                  // incremento il numero word
            int extra = 0;                // lo spazio extra
            if (wordCount <= extraSpaces) // se il contatore è minore degli spazi dispari da assegnare
            {
                extra = 1; // assegnerà anche uno dispari extra
            }
            int k;
            for (k = 0; k < spacesPerWord + extra; k++) // per ogni spazio normale + 1 eventuale extra, lo aggiunge
            {
                alignedStr[k + j] = ' ';
            }
            j += k;
        }
    }
    alignedStr[j] = '\0';
    return alignedStr;
}

void copySubstring(const char *source, char *destination, int startIndex) // copia la prima stringa da un certo indice in poi nella seconda
{
    int i = startIndex;
    int j = 0;

    // Copia il sottostringa nella stringa di destinazione
    while (source[i] != '\0')
    {
        destination[j] = source[i];
        i++;
        j++;
    }

    // Aggiungi il terminatore di stringa alla fine della stringa di destinazione
    destination[j] = '\0';
}