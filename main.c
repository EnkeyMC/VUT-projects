/*****************************************************/
/**                                                 **/
/**         1. Projekt - práce s textem             **/
/**                                                 **/
/**                 Martin Omacht                   **/
/**                                                 **/
/**                  30.10.2016                     **/
/**                                                 **/
/*****************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define HEX_PER_LINE 16
// buffer has to be multiple of HEX_PER_LINE
#define BUFFER_SIZE 1 * HEX_PER_LINE


/**
 * Vypíše nápovědu programu
 */
void printHelp();
/**
 * Vrátí délku řetězce
 * @param string
 * @return délka řetězce
 */
int strLength(char * string);

/**
 * Převede řetězec na kladné číslo
 * @param string - řetězec ze, který bude převáděn na řetězec
 * @return výsledné číslo, -1 při chybě
 */
int parsePosInt(char *string);

/**
 * Vypočítá mocninu základu
 * @param base
 * @param exponent
 * @return mocnina
 */
int power(int base, int exponent);

/**
 * Funkce převede standardní vstup na hexidecimální reprezentaci znaků
 * @param skip - počet znaků, které se přeskočí (jestliže je menší než 1 nepřeskočí se žádné)
 * @param numOfChars - maximální počet znaků, které se zpracují (-1 znamená nekonečno, jinak musí být větší než 0)
 * @return návratový kód (0 = úspěch)
 */
int hexdump(int skip, int numOfChars);

/**
 * Převede vstupní byty na jejich hexadecimální reprezentaci
 * @return návratový kód (0 = úspěch)
 */
int stdinToHex();

/**
 * V posloupnosti znaků nalezne řetězce a vypíše je oddělené ukončením řádku.
 * Řetězec je nejdelší posloupnost tisknutelných a prázdných znaků
 * (tj. mezera nebo tabulátor, viz isblank), jejíž délka je větší nebo rovna N znaků.
 * @param N - minimální délka řetězce (0 < N < 200)
 * @return návratový kód
 */
int parseStrings(int n);

/**
 * Převede hexadecimální číslo na decimální
 * @param hexChar - hexadecimální číslo
 * @return převedené číslo
 */
int hexToDec(int hex);

/**
 * Převede hexadecimální vstup do binárního formátu. Na vstupu ignoruje bílé znaky.
 * @return návratový kód (0 = úspěch)
 */
int reverse();

/**
 * Vstupní bod programu
 * @param argc - počet argumentů z příkazové řádky
 * @param argv - argumenty z příkazové řádky
 * @return return code
 */
int main(int argc, char * argv[]){
    long int skip, numOfChars;
    skip = numOfChars = -1;
    bool argumentError = false;
    int exitCode = EXIT_SUCCESS;

    // zpracování argumentů
    if (argc > 1){
        if (argv[1][0] == '-') {
            if (argv[1][1] == 'x' && argc == 2){ // Argument -x
                exitCode = stdinToHex();
            }else if (argv[1][1] == 'r' && argc == 2){ // Argument -r
                exitCode = reverse();
            }else if (argv[1][1] == 'S' && argc == 3 && parsePosInt(argv[2]) != -1){ // Argument -S N
                exitCode = parseStrings(parsePosInt(argv[2]));
            }else if (argv[1][1] == 's' && argc > 2 && parsePosInt(argv[2]) != -1){
                skip = parsePosInt((argv[2]));
                if (argc > 3) {
                    if (argc == 5 && argv[3][0] == '-' && argv[3][1] == 'n' && parsePosInt(argv[4]) != -1) {
                        numOfChars = parsePosInt(argv[4]);
                    } else {
                        argumentError = true;
                    }
                }
            }else if (argv[1][1] == 'n' && argc > 2 && parsePosInt(argv[2]) != -1){
                numOfChars = parsePosInt(argv[2]);
                if (argc > 3) {
                    if (argc == 5 && argv[3][0] == '-' && argv[3][1] == 's' && parsePosInt(argv[4]) != -1) {
                        skip = parsePosInt(argv[4]);
                    } else {
                        argumentError = true;
                    }
                }
            }else{
                argumentError = true;
            }
        }else{
            argumentError = true;
        }
    }

    if (argumentError){
        fprintf(stderr, "Invalid arguments.\n");
        printHelp();
        return EXIT_SUCCESS;
    }

    // když nebyly zadány argumenty, nebo byly zadány argumenty -s nebo -n, spustí se výpis bez povinných argumentů
    if (argc == 1 || skip > -1 || numOfChars > -1)
        exitCode = hexdump(skip, numOfChars);

    return exitCode;
}

void printHelp(){
    printf("USAGE:\n");
    printf("\t./proj1 [-s M] [-n N]\n");
    printf("\t./proj1 -x\n");
    printf("\t./proj1 -S N\n");
    printf("\t./proj1 -r\n");
    printf("\n");
    printf("ARGUMENTS:\n");
    printf("\t-s M\t - Number of characters to be ignored from input. M is a number larger than 0. \n\t\t   If M is larger than input length, nothing is printed out.\n");
    printf("\t-n N\t - Defines maximal number of characters to be processed.\n");
    printf("\t-x\t - All input data is transferred to hexadecimal on one line.\n");
    printf("\t-S N\t - Prints only string with printable or blank characters.\n\t\t - N defines the minimal length of string. 0 < N < 200\n");
    printf("\t-r\t - Converts hexadecimal input into characters.\n");
    printf("\n");
    printf("Without arguments this converts characters to their hexadecimal representation based on ASCII table.\n");
    printf("Output format:\n\n");
    printf("AAAAAAAA  xx xx xx xx xx xx xx xx  xx xx xx xx xx xx xx xx  |bbbbbbbbbbbbbbbb|\n");
    printf("\n");
    printf("AAAAAAAA - Position of first byte in the row.\n");
    printf("xx - Hexadecimal value of the byte.\n");
    printf("b - Printable representation of the byte. (Non-printable characters are represented by '.').\n");

    return;
}

int strLength(char * string){
    int length = 0;
    while (string[length] != '\0')
        length++;
    return length;
}

int parsePosInt(char *string){
    int result = 0;
    int len = strLength(string);

    for (int i = 0; i < len; i++){
        if (string[i] >= '0' && string[i] <= '9'){
            result += power(10, len - i - 1) * (string[i] - '0');
        }else{
            return -1;
        }
    }
    return result;
}

int power(int base, int exponent){
    int result = 1;
    while (exponent > 0){
        result *= base;
        exponent--;
    }
    return result;
}


int hexdump(int skip, int numOfChars) {
    unsigned int address = 0;
    int buffer[BUFFER_SIZE];
    int i = 0;
    int startIndex = 0;
    int nCounter = -2;
    bool end = false;

    if (skip > 0){
        // přeskočí znaky podle parametru skip
        while (skip > 0 && getchar() != EOF){
            skip--;
            address++;
        }
    }

    // kontrola parametru numOfChars
    if (numOfChars != -1){
        if (numOfChars <= 0){
            fprintf(stderr, "Error: Value of -n has to be larger than 0 (got %d)\n", numOfChars);
            return EXIT_FAILURE;
        }else{
            nCounter = 0;
        }
    }

    // cyklus pro zpracování vstupu (končí při EOF)
    while (!end){
        i = 0;
        // načtení vstupu podle velikosti bufferu nebo do EOF
        while (i < BUFFER_SIZE && (buffer[i] = getchar()) != EOF)
            i++;

        i = 0;
        // zpracování bufferu
        while (i < BUFFER_SIZE && !end && nCounter < numOfChars) {
            startIndex = i;
            // zabránění výpisu pokud je hned první znak v řádku EOF
            if (buffer[i] == EOF) {
                end = true;
                break;
            }

            printf("%08x  ", address);
            // cyklus vypisující jednotlivé byty v hexadecimální soustavě
            for (; i < HEX_PER_LINE + startIndex; i++) {
                // mezera navíc po 8 bytech
                if (i - startIndex == 8)
                    putchar(' ');

                // kontrola zda už nenastal konec výpisu (zbytek se vyplní mezerama)
                if (buffer[i] != EOF && !end && nCounter < numOfChars) {
                    // výpis pouze bytů (znaky o více bytech jsou zastoupeny tečkami)
                    if (buffer[i] <= 0xff)
                        printf("%02x ", buffer[i]);
                    else
                        printf(".. ");
                } else {
                    end = true;
                    printf("   ");
                }

                // jestliže je definován maximální počet znaků, inkrementuje nCounter
                if (numOfChars != -1)
                    nCounter++;

                address++;
            }
            // resetování proměnné end pro další cyklus
            end = false;

            // vrácení proměnné nCounter zpět na začátek řádku pro další cyklus
            if (numOfChars != -1)
                nCounter -= HEX_PER_LINE;

            printf(" |");
            // vypis tisknutelné podoby bytů
            for (int j = startIndex; j < HEX_PER_LINE + startIndex; j++){
                // tiskni znaky pokud není konec, jinak mezery
                if (buffer[j] != EOF && !end && nCounter < numOfChars) {
                    if (isprint(buffer[j])) {
                        putchar(buffer[j]);
                    }else {
                        putchar('.');
                    }
                }else{
                    end = true;
                    putchar(' ');
                }
                // jestliže je definován maximální počet znaků, inkrementuje nCounter
                if (numOfChars != -1)
                    nCounter++;
            }
            // odřádkování na konci řádku
            printf("|\n");
        }
        // zabránění zacyklení pokud numOfChars je násobek délky řádku
        if (nCounter >= numOfChars)
            end = true;
    }

    return EXIT_SUCCESS;
}

int stdinToHex() {
    int c;
    while ((c = getchar()) != EOF)
        printf("%02x", c);
    putchar('\n');
    return EXIT_SUCCESS;
}

int parseStrings(int n) {
    // kontrola rozsahu n
    if (n <= 0 || n >= 200){
        fprintf(stderr, "Error: Argument N out of range (0 < N < 200)\n");
        return EXIT_FAILURE;
    }

    int buffer[n]; // pole pro ukládání vstupu

    int i, currentStrLen = 0;
    bool end = false;
    bool reset;
    bool found;

    // cyklus pro zpracování vstupu
    while (!end) {
        i = 0;
        reset = false;
        found = true;

        while (i < n && !end && !reset) {
            // přečtení vstupu
            buffer[i] = getchar();
            if (buffer[i] == EOF){ // ošetření konce vstupu
                if (currentStrLen >= n)
                    found = true;
                else
                    found = false;
                end = true;
            } else if (!isprint(buffer[i]) && !isblank(buffer[i])) { // detekce oddělovacích znaků
                if (currentStrLen >= n)
                    found = true;
                else {
                    found = false;
                }
                currentStrLen = 0;
                reset = true;
            }else{
                currentStrLen++;
            }

            if (!end && !reset)
                i++;
        }

        if (found){ // nalezen řetězec nebo konec bufferu
            for (int j = 0; j < i; j++){
                putchar(buffer[j]);
            }
            if (reset || end) // jestli je řetězec vytisknutý celý, odřádkuje
                putchar('\n');
        }
    }
    return EXIT_SUCCESS;
}

int hexToDec(int hex)
{
    if (hex >= '0' && hex <= '9') {
        return hex - '0';
    } else if (hex >= 'a' && hex <= 'f') {
        return hex - 'a' + 10;
    } else if (hex >= 'A' && hex <= 'F') {
        return hex - 'A' + 10;
    } else {
        return -1;
    }
}

int reverse() {
    int buffer[2];
    int dec;
    int i = 0;
    while ((buffer[i] = getchar()) != EOF){
        if (!isspace(buffer[i])) // bílé znaky se ignorují (neinkrementuje se pozice a znak se přepíše dalším)
            i++;

        if (i == 2){ // jestliže je načtený celý byte
            dec = 0;
            for (int j = 0; j < i; j++) {
                if (hexToDec(buffer[j]) != -1)
                    dec += power(16, i - j - 1) * hexToDec(buffer[j]);
                else {
                    fprintf(stderr, "Error: character %02x ('%c') is not hexadecimal\n", buffer[j], buffer[j]);
                    return EXIT_FAILURE;
                }
            }

            putchar(dec);
            i = 0;
        }
    }

    if (i == 1 && hexToDec(buffer[0]) != -1) // vypsání posledního lichého bytu
        putchar(hexToDec(buffer[0]));

    return EXIT_SUCCESS;
}
