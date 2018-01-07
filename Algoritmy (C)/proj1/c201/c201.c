
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2016
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
** 
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu před prvním použitím,
**      DisposeList ... zrušení všech prvků seznamu,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->Act = NULL;
    L->First = NULL;
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
    /* Mazat prvky musime jen pokud tam nejake jsou */
	if (L->First != NULL) {
        /* Iterujeme prvky a postupne je uvolnujeme */
        tElemPtr processed = L->First;
        do {
            tElemPtr next = processed->ptr;
            free(processed);
            processed = next;
        } while(processed != NULL);
    }
    L->Act = NULL;
    L->First = NULL;
}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
    /* Vytvorime novy ukazatel na strukturu tElem s dostatkem mista */
    tElemPtr new = malloc(sizeof(struct tElem));
    if (new == NULL) {
        Error();
        return;
    }
    /* Seznam je prazdny, tudiz vlozeny prvek nema nasledovnika */
    if (L->First == NULL) {
        new->ptr = NULL;
    /* Seznam nebyl prazdny, tudiz vlozime novy prvek a ten bude ukazovat na nyni druhy prvek */
    } else {
        new->ptr = L->First;
    }
    new->data = val; // Priradime informaci o hodnote novemu prvku
    L->First = new; // Urcime nas nove vytvoreny prvek jako prvni v seznamu
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
	L->Act = L->First; // Nastavit aktivni na prvni
}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
    /* Prazdny seznam = chyba */
	if (L->First == NULL) {
        Error();
        return;
    } else {
        *val = L->First->data; // Pres ukazatel ulozime datovou hodnotu */
    }
}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
	if (L->First != NULL) {
        tElemPtr next = L->First->ptr; // Ziskame druhy prvek, klidne to muze byt NULL
        /* Pokud byl prvni prvek aktivni, tak se aktivita ztrati */
        if (L->Act == L->First) {
            L->Act = NULL;
        }
        /* Uvolnime prvni prvek a nasledujici bude prvnim (muze byt NULL = prazdny seznam) */
        free(L->First);
        L->First = next;
    }
}	

void PostDelete (tList *L) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
	if (L->Act != NULL && L->Act->ptr != NULL) { // Jednoduche zpracovani definovanych podminek
        tElemPtr next = L->Act->ptr; // Ziskame prvek za aktivnim
        L->Act->ptr = next->ptr; // Propojime aktivni s tim, co bylo za nasledujicim, ktery mazeme
        free(next); // Muzeme bez obav uvolnit
    }
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
	if (L->Act != NULL) { // Ignorujeme neaktivni seznam
        tElemPtr new = malloc(sizeof(struct tElem)); // Misto pro novy prvek
        /* Malloc se nezdaril */
        if (new == NULL) {
            Error();
            return;
            /* Vse je pripraveno */
        } else {
            new->data = val; // Priradime datovou hodnotu
            tElemPtr next = L->Act->ptr; // Ulozime si prvek za aktivnim (muze byt NULL)
            L->Act->ptr = new; // Aktivni nyni ukazuje na nove vytvoreny
            new->ptr = next; // A nove vytvoreny ukazuje na to, co bylo za aktivnim (klidne NULL, jako ze je posledni)
        }
    }
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
    /* Prazdny seznam = chyba */
	if (L->Act == NULL) {
        Error();
        return;
    } else {
        *val = L->Act->data; // Pres ukazatel ulozime datovou hodnotu
    }
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
	if (L->Act != NULL) { // Ignorujeme neaktivni seznam
        L->Act->data = val; // Prepiseme datovou hodnotu aktivniho prvku
    }
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
	if (L->Act != NULL) { // Ignorujeme neaktivni seznam
        L->Act = L->Act->ptr; // Posuneme aktivitu dale (klidne na NULL)
    }
}

int Active (tList *L) {		
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
	return (L->Act != NULL);
}

/* Konec c201.c */
