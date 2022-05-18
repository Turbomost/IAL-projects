
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
    printf("*ERROR* The program has performed an illegal operation.\n");
    error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init(DLList* list) {

    // Initialize new list and set all parameters tu NULL pointer
    list->firstElement = NULL;
    list->activeElement = NULL;
    list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose(DLList* list) {

    // Variable for scrolling through the list
    DLLElementPtr pointer;

    // Save adress of first element to 'pointer' and scroll to the end
    while ((pointer = list->firstElement) != NULL) {

        // Delete first element and retarged adress of the first element to the following one
        list->firstElement = list->firstElement->nextElement;

        // Free the used space of the first element
        free(pointer);
    }

    // Set all variables to NULL like at the initialization process
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->activeElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst(DLList* list, int data) {

    // Create new element ans assing its memory
    DLLElementPtr pointer = malloc(sizeof(struct DLLElement));

    // Check allocation
    if (pointer == NULL)
        DLL_Error();

    else {
        // Set parameters and link it to the list
        pointer->data = data;
        pointer->nextElement = list->firstElement;
        pointer->previousElement = NULL;

        // If the list is empty we have to make this element LAST aswell
        if (list->firstElement == NULL)
            list->lastElement = pointer;

        // Otherwise link now the 2nd element to the new one
        else
            list->firstElement->previousElement = pointer;

        // And finally make our new element officially first
        list->firstElement = pointer;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast(DLList* list, int data) {

    // Literally the same as the previous function (next <-> previous, first <-> last)
    // Create new element ans assing its memory
    DLLElementPtr pointer = malloc(sizeof(struct DLLElement));

    // Check allocation
    if (pointer == NULL)
        DLL_Error();

    else {
        // Set parameters and link it to the list
        pointer->data = data;
        pointer->previousElement = list->lastElement;
        pointer->nextElement = NULL;

        // If the list is epmty we have to make this element FIRST aswell
        if (list->firstElement == NULL)
            list->firstElement = pointer;

        // Otherwise link now the 2nd to last element to the new one
        else
            list->lastElement->nextElement = pointer;

        // And make our new element last
        list->lastElement = pointer;
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First(DLList* list) {

    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last(DLList* list) {

    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst(DLList* list, int* dataPtr) {

    // Empty check
    if (list->firstElement == NULL)
        DLL_Error();
    else
        *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast(DLList* list, int* dataPtr) {

    // Empty check
    if (list->lastElement == NULL)
        DLL_Error();
    else
        *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst(DLList* list) {

    // First, we have to check if the list is not empty
    if (list->firstElement != NULL) {

        // If we are trying to delete the active element, the activity will disappear
        if (list->activeElement == list->firstElement)
            list->activeElement = NULL;

        // Remeber the adress, so we can clear the used memory later
        DLLElementPtr pointer = list->firstElement;

        // Check if our old first element was also the last one in the list
        if (list->firstElement->nextElement != NULL) {

            // If not, set the 2nd element as the first and delete its previousElement
            list->firstElement = list->firstElement->nextElement;
            list->firstElement->previousElement = NULL;
        }

        else {
            //If it was the only element, clear the list
            list->lastElement = NULL;
            list->firstElement = NULL;
        }

        // Free memory
        free(pointer);
    }

}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast(DLList* list) {

    // see 'DLL_DeleteFirst':
    // > next   <--> previous
    // > first  <--> last

    if (list->lastElement != NULL) {
        if (list->activeElement == list->lastElement)
            list->activeElement = NULL;

        DLLElementPtr pointer = list->lastElement;

        if (list->lastElement->previousElement != NULL) {
            list->lastElement = list->lastElement->previousElement;
            list->lastElement->nextElement = NULL;
        }
        else {
            list->lastElement = NULL;
            list->firstElement = NULL;
        }

        free(pointer);

    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter(DLList* list) {

    // The function is useless if the active element is the last one or if it does not exist
    if (list->activeElement != NULL && list->activeElement != list->lastElement) {

        // Remember adress for garbage cleaning
        DLLElementPtr pointer = list->activeElement->nextElement;

        // We need to check if our soon-to-be-deleted element is the last one
        if (list->activeElement->nextElement->nextElement != NULL) {

            // If it isn't, relink the next and previous pointers
            list->activeElement->nextElement = list->activeElement->nextElement->nextElement;
            list->activeElement->nextElement->previousElement = list->activeElement;
        }

        else {

            // If it was the last element, active element is now the last and don't have a follower anymore
            list->activeElement->nextElement = NULL;
            list->lastElement = list->activeElement;
        }

        // Delete
        free(pointer);
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore(DLList* list) {

    // see 'DLL_DeleteAfter':
    // > next   <--> previous
    // > first  <--> last

    if (list->activeElement != NULL && list->activeElement != list->firstElement) {

        DLLElementPtr pointer = list->activeElement->previousElement;

        if (list->activeElement->previousElement->previousElement != NULL) {
            list->activeElement->previousElement = list->activeElement->previousElement->previousElement;
            list->activeElement->previousElement->nextElement = list->activeElement;
        }

        else {
            list->activeElement->previousElement = NULL;
            list->firstElement = list->activeElement;
        }

        free(pointer);
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter(DLList* list, int data) {

    // Empty check
    if (list->activeElement != NULL) {
        
        // Allocate and check memory
        DLLElementPtr pointer = malloc(sizeof(struct DLLElement));
        if (pointer == NULL)
            DLL_Error();
        
        else {
            
            // Set parameters of element and link it to active element
            pointer->data = data;
            pointer->previousElement = list->activeElement;
            list->activeElement->nextElement = pointer;

            // Link it to the next element (if exists)
            if (list->lastElement == list->activeElement) {

                // In case new element is last
                list->lastElement = pointer;
                pointer->nextElement = NULL;
            }

            // If it isn't last link it to the following item
            else {
                pointer->nextElement = list->activeElement->nextElement;
                pointer->nextElement->previousElement = pointer;
            }
        }
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore(DLList* list, int data) {

    // see 'DLL_InsertAfter':
    // > next   <--> previous
    // > first  <--> last

    if (list->activeElement != NULL) {
        DLLElementPtr pointer = malloc(sizeof(struct DLLElement));

        if (pointer == NULL)
            DLL_Error();

        else {
            pointer->data = data;
            pointer->nextElement = list->activeElement;
            list->activeElement->previousElement = pointer;

            if (list->firstElement == list->activeElement) {
                list->firstElement = pointer;
                pointer->previousElement = NULL;
            }
            else {
                pointer->previousElement = list->activeElement->previousElement;
                pointer->previousElement->nextElement = pointer;
            }
        }

    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue(DLList* list, int* dataPtr) {

    // If list has an acitve element - store its value in *dataPtr
    if (list->activeElement == NULL)
        DLL_Error();
    else
        *dataPtr = list->activeElement->data;

}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue(DLList* list, int data) {

    // If the list has an active element - push 'data' into it
    if (list->activeElement != NULL)
        list->activeElement->data = data;

}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next(DLList* list) {

    // If list has active element
    if (list->activeElement != NULL) {

        // Check wheater there is any following item (to hand over active status)
        if (list->activeElement == list->lastElement)
            list->activeElement = NULL;
        else
            list->activeElement = list->activeElement->nextElement;
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous(DLList* list) {

    // see 'DLL_Next':
    // > next   <--> previous
    // > first  <--> last

    if (list->activeElement != NULL) {
        if (list->activeElement == list->firstElement)
            list->activeElement = NULL;
        else
            list->activeElement = list->activeElement->previousElement;
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive(DLList* list) {

    // returns 0 if there isn't active element  
    return (list->activeElement != NULL);
}

/* Konec c206.c */
