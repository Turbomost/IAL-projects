    
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar(Stack* stack, char* postfixExpression, unsigned* postfixExpressionLength) {
    
    // Create a variable for character at the top of the stack
    char top;
    Stack_Top(stack, &top);

    // Repeat until we found our left parenthesis OR until the stack is empty
    while (!Stack_IsEmpty(stack) && top != '(') {
        
        // Write the top element to the output and then iterate length by one
        postfixExpression[(*postfixExpressionLength)++] = top;
        
        // Load new top element
        Stack_Pop(stack);
        Stack_Top(stack, &top);
    }

    // Pop the left parenthesis
    Stack_Pop(stack);
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {

    // Create a variable for character at the top of the stack
    char top;
    if (!Stack_IsEmpty(stack))
        Stack_Top(stack, &top);
    
    // Push an element to the stack if:
    // > The stack is empty (no need to check for operation priorities)
    // > The top element is right parenthesis
    // > else: check operation priority:
    // > > If our char is '*' or '/' and the top element has lower prority ('+' or '-')
    if (Stack_IsEmpty(stack) || top == '(' || ((c == '*' || c == '/') && (top == '+' || top == '-')))
        Stack_Push(stack, c);
    
    // Else:
    // > We are loading '+' or '-' element and there is already one at the top with the same priority
    // > We are loading '*' or '-' element -||-
    else {
        // Pop the top element to the output (iterate lenght afterwards) and run this function again
        postfixExpression[(*postfixExpressionLength)++] = top;
        Stack_Pop(stack);
        doOperation(stack, c, postfixExpression, postfixExpressionLength);
    }
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků wStack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char* infix2postfix(const char* infixExpression) {

    // Initialize a variable for character (c), output length (length) and input position (i)
    char c;
    unsigned int length = 0;
    int i = 0;
    
    // Allocate needed space for output and Stack and check pointers
    char* output = (char*)malloc(MAX_LEN * sizeof(char));
    if (!output)
        return NULL;

    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack)
        return NULL;
   
    // Initialize new stack
    Stack_Init(stack);

    // Check every char and do the following:
    while ((c = infixExpression[i++]) != '\0') {
        
        // > Left parenthesis: simply push to stack and move on
        if (c == '(')
            Stack_Push(stack, c);
        
        // > Right parenthesis: use our function to pop everything until left parenthesis
        else if (c == ')')
            untilLeftPar(stack, output, &length);
        
        // > Alphanumeric symbol: Write it directly to ouput and increment length
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            output[length++] = c;
        
        // > Operator: Check priorities using doOperation function
        else if (c == '*' || c == '/' || c == '+' || c == '-')
            doOperation(stack, c, output, &length);
        
        // > Equals sign: Pop everything out of the stack while incrementing length
        else if (c == '='){ 
           while (!Stack_IsEmpty(stack)) {
               Stack_Top(stack, &output[length++]);
               Stack_Pop(stack);
           }
           // Add the equals sign to the end (length++)
           output[length++] = '=';
        }
    }
    
    // And for the final touch add null character to the end of output (length++)
    output[length++] = '\0';

    // Clear used memory for stack and return output
    free(stack);
    return output;
}
/* Konec c204.c */
