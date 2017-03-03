/*****************************************************/
/**                                                 **/
/**         2. Projekt - Iterační výpočty           **/
/**                                                 **/
/**                 Martin Omacht                   **/
/**                                                 **/
/**                  16.11.2016                     **/
/**                                                 **/
/*****************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdbool.h>

#define PRINT_ERROR(format, args...) fprintf(stderr, "Error: "); fprintf (stderr, format, ## args)

/**
 * Spočítá přirozený logaritmus pro zadané číslo pomocí taylorova polynomu
 * @param x - ln(x)
 * @param n - počet iterací (čím vyšší, tím přesnější)
 * @return výsledek (může být i INF nebo NAN)
 */
double taylor_log(double x, unsigned int n);

/**
 * Spočítá přirozený logaritmus pro zadané číslo pomocí zřetězených zlomků
 * @param x - ln(x)
 * @param n - počet iterací (čím vyšší, tím přesnější)
 * @return výsledek (může být i INF nebo NAN)
 */
double cfrac_log(double x, unsigned int n);

/**
 * Spočítá x^y pomocí taylorova polynomu s použitím funkce taylor_log
 * @param x
 * @param y
 * @param n - počet iterací (čím vyšší, tím přesnější)
 * @return vásledek (může být i INF nebo NAN)
 */
double taylor_pow(double x, double y, unsigned int n);

/**
 * Spočítá x^y pomocí taylorova polynomu s použitím funkce cfrac_log
 * @param x
 * @param y
 * @param n - počet iterací (čím vyšší, tím přesnější)
 * @return vásledek (může být i INF nebo NAN)
 */
double taylorcf_pow(double x, double y, unsigned int n);

/**
 * Vypíše nápovědu
 */
void printHelp();

/**
 * Pomocná funkce pro kontrolu argumentů funkcí *_pow
 * @param x - základ
 * @param y - exponent
 * @param returnVal - pokud funkce nalezne speciální případ argumentů, naplní tuto proměnou výsledkem, který by měla funkce pow navrátit
 * @return - Vrátí true pokud, funkce narazí na speciální případ argumentů (INF, NAN,...), jinak false
 */
bool checkPowArguments(double x, double y, double *returnVal);

/**
 * Pomocná funkce pro kontrolu argumentů funkcí *_log
 * @param x - číslo pro které se má spočítat logaritmus
 * @param returnVal - pokud funkce nalezne speciální případ argumentů, naplní tuto proměnou výsledkem, který by měla funkce log navrátit
 * @return - Vrátí true pokud, funkce narazí na speciální případ argumentů (INF, NAN,...), jinak false
 */
bool checkLogArguments(double x, double *returnVal);

int main(int argc, char* argv[]) {
    // Zpracování argumentů
    if (argc > 1){
        if (strcmp("--log", argv[1]) == 0) { // kontrola parametru --log
            if (argc == 4) {
                char* pEnd; // ukazatel pro ošetřování chybových stavů funkcí strtod a strtol
                double x = strtod(argv[2], &pEnd);

                if (*pEnd != 0){ // jestliže  pEnd ukazuje na nějaký znak, znamená to, že za číslem byly ještě další znaky
                    PRINT_ERROR("%s is not a valid number.\n", argv[2]);
                    printHelp();
                    return EXIT_FAILURE;
                }

                long n = strtol(argv[3], &pEnd, 10);

                if (*pEnd != 0 || n < 0 || (n == INT_MAX && errno == ERANGE)){ // opět kontrola ukazatele a přetečení proměnné
                    PRINT_ERROR("%s is not a valid number.\n", argv[3]);
                    printHelp();
                    return EXIT_FAILURE;
                }

                if (n == 0) {
                    PRINT_ERROR("N has to be larger than 0.\n");
                    return EXIT_FAILURE;
                }

                // Vyýpis
                printf("       log(%g) = %.12g\n", x, log(x));
                printf(" cfrac_log(%g) = %.12g\n", x, cfrac_log(x, n));
                printf("taylor_log(%g) = %.12g\n", x, taylor_log(x, n));
            }else{
                PRINT_ERROR("Unexpected number of arguments (expected 3, got %d).\n", argc-1);
                printHelp();
                return EXIT_FAILURE;
            }
        }else if (strcmp("--pow", argv[1]) == 0) { // kontrola parametru --pow
            if (argc == 5) {
                char* pEnd; // ukazatel pro ošetřování chybových stavů funkcí strtod a strtol
                double x = strtod(argv[2], &pEnd);

                if (*pEnd != 0){ // jestliže  pEnd ukazuje na nějaký znak, znamená to, že za číslem byly ještě další znaky
                    PRINT_ERROR("%s is not a valid number.\n", argv[2]);
                    printHelp();
                    return EXIT_FAILURE;
                }

                double y = strtod(argv[3], &pEnd);

                if (*pEnd != 0){ // jestliže  pEnd ukazuje na nějaký znak, znamená to, že za číslem byly ještě další znaky
                    PRINT_ERROR("%s is not a valid number.\n", argv[3]);
                    printHelp();
                    return EXIT_FAILURE;
                }

                long n = strtol(argv[4], &pEnd, 10);

                if (*pEnd != 0 || n < 0 || (n == INT_MAX && errno == ERANGE)){ // opět kontrola ukazatele a přetečení proměnné
                    PRINT_ERROR("%s is not a valid number.\n", argv[4]);
                    printHelp();
                    return EXIT_FAILURE;
                }

                if (n == 0) {
                    PRINT_ERROR("N has to be larger than 0.\n");
                    return EXIT_FAILURE;
                }

                printf("         pow(%g,%g) = %.12g\n", x, y, pow(x, y));
                printf("  taylor_pow(%g,%g) = %.12g\n", x, y, taylor_pow(x, y, n));
                printf("taylorcf_pow(%g,%g) = %.12g\n", x, y, taylorcf_pow(x, y, n));
            }else{
                PRINT_ERROR("Unexpected number of arguments (expected 4, got %d).\n", argc-1);
                printHelp();
                return EXIT_FAILURE;
            }
        }else{
            PRINT_ERROR("Invalid argument %s\n", argv[1]);
            printHelp();
            return EXIT_FAILURE;
        }
    }else{
        printHelp();
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

double taylor_log(double x, unsigned int n){
    double returnVal = 0.0;
    if (checkLogArguments(x, &returnVal))
        return returnVal;

    double result = 0.0;
    double x_exp = 1.0; // pro průběžné počítání mocniny v čitateli

    if (x > 1) { // pro hodnoty větší než 1 je lepší použít druhý vzorec
        double numerator_fraction = (x - 1) / x; // předem si spočítáme čitatel
        for (unsigned int i = 1; i <= n; ++i) {
            x_exp *= numerator_fraction; // průběžné umocňování čitatele
            result += x_exp / i;
        }
    }else {
        x = 1 - x; // úprava x podle vzorce

        for (unsigned int i = 1; i <= n; ++i) {
            x_exp *= x; // průběžné umocňování čitatele
            result -= x_exp / i;
        }
    }

    return result;
}

double cfrac_log(double x, unsigned int n){
    double returnVal = 0;
    if (checkLogArguments(x, &returnVal))
        return returnVal;

    double z = - (1.0 - x) / (x + 1.0); // vypočítání 'z' podle vzorce
    double z2 = z*z; // z na druhou (ať se nemusí počítat každou iteraci znova)
    double result = 0;
    n--; // poslední iterace se provede manuálně, v cyklu se musí provést o 1 méně iterací

    for (int i = n * 2 + 1; n >= 1; n--, i -= 2){
        result = (n * n * z2) / (i - result);
    }

    result = (2 * z) / (1 - result); // poslední iterace

    return result;
}

double taylor_pow(double x, double y, unsigned int n) {
    double returnVal = 0;
    if (checkPowArguments(x, y, &returnVal))
        return returnVal;

    double result = 1.0;
    double last = 1.0; // předchozí polynom
    double lnx = taylor_log(x, n);

    for (unsigned int i = 1; i <= n; i++) {
        last *= y * lnx / i;
        result += last;
    }

    return result;
}

double taylorcf_pow(double x, double y, unsigned int n) {
    double returnVal = 0;
    if (checkPowArguments(x, y, &returnVal))
        return returnVal;

    double result = 1.0;
    double last = 1.0; // předchozí polynom
    double ln = cfrac_log(x, n);

    for (unsigned int i = 1; i <= n; i++) {
        last *= y * ln / i;
        result += last;
    }

    return result;
}

void printHelp(){
    printf("USAGE:\n");
    printf("\t./proj2 --log X N\n");
    printf("\t./proj2 --pow X Y N\n");
    printf("\n");
    printf("ARGUMENTS:\n");
    printf("\t--log X N\t - Calculates natural logarithm for number X with N iterations.\n");
    printf("\t--pow X Y N\t - Raises X to a power of Y with N iterations.\n");
}

bool checkPowArguments(double x, double y, double *returnVal) {
    if (x <= 0) // vzorec pro výpočet pow platí pouze pro x > 0
        *returnVal = NAN;
    if (x == 1.0)
        *returnVal = 1.0;
    else if (y == 0.0)
        *returnVal = 1.0;
    else if (isinf(x) && y > 0.0)
        *returnVal = INFINITY;
    else if (isinf(x) && y < 0.0)
        *returnVal = 0;
    else if (fabs(x) < 1.0 && isinf(y) && y < 0.0)
        *returnVal = INFINITY;
    else if (fabs(x) < 1.0 && isinf(y) && y > 0.0)
        *returnVal = 0.0;
    else if (fabs(x) > 1.0 && isinf(y) && y < 0.0)
        *returnVal = 0.0;
    else
        return false;
    return true;
}

bool checkLogArguments(double x, double *returnVal) {
    if (x == 0.0)
        *returnVal = -INFINITY;
    else if (x < 0.0)
        *returnVal = NAN;
    else if (x == 1.0)
        *returnVal = 0;
    else if (isinf(x))
        *returnVal = INFINITY;
    else
        return false;
    return true;
}

