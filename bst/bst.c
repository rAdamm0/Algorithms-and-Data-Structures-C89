/****************************************************************************
 *
 * bst.c -- Alberi binari di ricerca
 *
 * Copyright (C) 2021--2025 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

/***
% LabASD - Alberi binari di ricerca
% [Moreno Marzolla](https://www.unibo.it/sitoweb/moreno.marzolla)
% Ultimo aggiornamento: 2025-04-01

![Fonte: <https://www.pinterest.com/pin/binary-tree-funny--547961479663761031/>](binary_tree.png "Un albero binario")

Il file [bst.h](bst.h) contiene l'interfaccia della struttura dati
_Albero Binario di Ricerca_ (BST, _binary search tree_). Completare
l'implementazione realizzando le funzioni indicate con `[TODO]`. È
possibile definire ulteriori funzioni di supporto, se lo si ritiene
necessario.

Un BST è definito dalla struttura:

```C
typedef struct {
    BSTNode *root;
    int size;
} BST;
```

dove `root` punta alla radice dell'albero oppure a `NULL` se l'albero
è vuoto, e `size` è il numero di nodi. Il campo `size` consente di
conoscere il numero di nodi in tempo $O(1)$, senza bisogno di
contarli; occorre però mantenere aggiornato tale valore durante le
operazioni di inserimento e cancellazione.

Un nodo è definito dalla struttura `BSTNode`:

```C
typedef int BSTKey;

typedef struct BSTNode {
    BSTKey key;
    struct BSTNode *parent, *left, *right;
} BSTNode;
```

dove `BSTKey` è il tipo dell'informazione associata ai nodi (di
default è il tipo `int`), mentre `parent`, `left` e `right` sono
puntatori al padre, al figlio sinistro e al figlio destro; il nodo
radice avrà `parent == NULL`. Un esempio è mostrato nella Figura 1.

![Figura 1: Esempio di albero binario di ricerca](bst.svg)

In questa implementazione dei BST occorre mantenere la seguente
proprietà di ordine:

> **Proprietà di un BST**. Sia $x$ un nodo di un BST. Se $y$ è un nodo
> del sottoalbero sinistro di $x$, allora si avrà $y.\mathit{key} <
> x.\mathit{key}$; se $z$ è un nodo del sottoalbero destro di $x$,
> allora si avrà $z.\mathit{key} > x.\mathit{key}$.

Si noti che, in base alla definizione, non sono ammesse chiavi duplicate.

Le operazioni di inserimento e ricerca non dovrebbero presentare
difficoltà; personalmente ritengo più semplice implementarle in forma
ricorsiva, ma è possibile realizzarle in forma iterativa. Le
implementazioni ricorsive richiedono la definizione di ulteriori
funzioni di supporto.

L'operazione più complessa è la cancellazione. Sono possibili diverse
implementazioni; in quella descritta nel libro, se il nodo da
cancellare ha due figli, si rimpiazza la chiave in esso contenuta con
la chiave _minima_ del sottoalbero _destro_.

La funzione `bst_pretty_print()` (da lasciare per ultima perché è la
meno importante) deve stampare il contenuto di un BST in modo
"leggibile" ruotando l'output di 90 gradi.

Ad esempio, l'albero

```
         12
        /  \
       5    55
      / \
    -3   9
```

dovrebbe essere stampato come:

```
   55
12
      9
   5
      -3
```

Il programma [bst-main.c](bst-main.c) legge una sequenza di comandi da
un file il cui nome va specificato come unico parametro sulla riga di
comando. I comandi sono descritti nella Tabella 1, dove _k_ è una
chiave intera.

: Tabella 1: Comandi nel file di input

Comando       Significato
-----------   ----------------------------------------------
`+` _k_       Inserisce la chiave _k_, se non presente
`-` _k_       Cancella il nodo contenente _k_, se presente
`?` _k_       Verifica se la chiave _k_ è presente
`s`           Stampa il numero di nodi nell'albero
`h`           Stampa l'altezza dell'albero
`p`           Stampa il contenuto dell'albero
-----------   ----------------------------------------------

Un esempio di file di input è [bst.in](bst.in); l'output
corrispondente è [bst.out](bst.out).

Per compilare:

        gcc -std=c90 -Wall -Wpedantic bst.c bst-main.c -o bst-main

Per eseguire in ambiente Linux/MacOSX:

        ./bst-main bst.in

Per eseguire in ambiente Windows:

        .\bst-main bst.in

## File

- [bst.c](bst.c)
- [bst.h](bst.h)
- [bst-main.c](bst-main.c)
- [bst.in](bst.in) ([output atteso](bst.out))
- [bst1.in](bst1.in)
- [bst2.in](bst2.in)

***/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"

/* Le funzioni bst_check() e bst_check_rec() servono per controllare
   se la struttura del BST `T` è corretta, cioè se i nodi sono
   correttamente collegati tra di loro. In particolare, verifica che:

   1. I valori delle chiavi di ciascun nodo e dei due figli, se
      presenti, soddisfino la regola per i BST.

   2. I puntatori ai nodi padri siano corretti.

   Il controllo va effettuato chiamando `bst_check(T)`, che innesca la
   ricorsione invocando `bst_check_rec()`. `bst_check(T)` esplora
   tutti i nodi dell'albero, quindi ha costo proporzionale al numero
   di nodi. Di conseguenza, il suo uso ha un impatto notevole sulle
   altre operazioni; può tuttavia essere utile in fase di sviluppo del
   codice, e viene pertanto resa disponibile.
*/
static void bst_check_rec( const BST* T, const BSTNode *n )
{
    if (n != NULL) {
        if (n->left) {
            assert(n->left->key <= n->key);
            assert(n->left->parent == n);
            bst_check_rec(T, n->left);
        }
        if (n->right) {
            assert(n->right->key > n->key);
            assert(n->right->parent == n);
            bst_check_rec(T, n->right);
        }
    }
}

static void bst_check( const BST* T )
{
    if (T->root)
        assert(T->root->parent == NULL);
    bst_check_rec(T, T->root);
}

BST *bst_create( void )
{
    BST *T = (BST*)malloc(sizeof(*T));
    assert(T != NULL);

    T->root = NULL;
    T->size = 0;
    bst_check(T);
    return T;
}

/* Funzione ausiliaria che libera ricorsivamente la memoria usata dal
   nodo `n` e da tutti i nodi che discendono da esso */
static void bst_clear_rec(BSTNode *n)
{
    if (n != NULL) {
        bst_clear_rec(n->left);
        bst_clear_rec(n->right);
        free(n);
    }
}

void bst_clear(BST *T)
{
    assert(T != NULL);

    bst_clear_rec(T->root);
    T->root = NULL;
    T->size = 0;
    bst_check(T);
}

void bst_destroy(BST *T)
{
    bst_clear(T);
    free(T);
}

BSTNode* bst_search(const BST* T, BSTKey k)
{
    assert(T != NULL);

    /* [TODO] questa funzione può essere realizzata, a scelta, in modo
       iterativo o ricorsivo. */
   /* BSTNode* temp = T->root;
    while (temp->key != k) {
        if (temp->key < k) {
            temp = temp->right;
        }
        else {
            temp = temp->left;
        }
    }
    */

    if (T->root->key == k) {
        return T->root;
    }
    BST* temp;
    temp = malloc(sizeof(BST));
    if (temp == NULL) {
        return NULL;
    }
    if (T->root->key <= k) {
        temp->root = T->root->right;
    }
    else {
        temp->root = T->root->left;
    }
    bst_search(temp, k);
    free(temp);
}

BSTNode* parent_search(BSTNode* root, BSTKey k) {
    if (root != NULL) {
        if (root->key == k) {
            return root;
        }
        root->key < k ? parent_search(root->right, k) : parent_search(root->left, k);
    }
    return NULL;
}

int bst_insert(BST *T, BSTKey k)
{
    /* [TODO] */
    
    /* Se si implementa lo pseudocodice mostrato nel libro di testo,
       si presti attenzione ai punti seguenti:

       - La specifica di questa implementazione richiede che se la chiave
         `k` è già presente nell'albero, l'inserimento non deve avvenire.

       - Se la chiave `k` non è presente nell'albero, deve essere
         creato un nuovo nodo (il libro lo chiama `z`) da inserire
         nella posizione appropriate, e deve essere incrementato
         il valore di `T->size`.

       - Questa funzione deve restituire 0 se la chiave
         era già presente, 1 altrimenti.
    */
    if (bst_search(T, k) == NULL) {
        BSTNode* z;
        z = (BSTNode*)malloc(sizeof(z));
        assert(z != NULL);
        z->key = k;
        BSTNode* parent = parent_search(T->root, k);
        z->parent = parent;
        if (parent->key < z->key)
        {
            parent->right = z;
        }
        else {
            parent->left = z;
        }
    }
    return 1; /* sostituire con il valore di ritorno corretto */
}

/* Funzione ausiliaria (non definita nell'interfaccia pubblica) che
   restituisce il puntatore al nodo contenente il valore minimo del
   sottoalbero radicato in `n`. Il minimo si trova nel nodo "più a
   sinistra" del sottoalbero radicato in `n`.

   Questa funzione potrebbe risultare utile per la cancellazione; al
   momento però non viene usata, e alcuni compilatori potrebbero
   segnalare un "warning". Per risolvere il warning si può commentare
   la funzione. */
static BSTNode *bst_minimum(BSTNode *n)
{
    assert(n != NULL);

    while (n->left != NULL) {
        n = n->left;
    }
    return n;
}

static int children_Num(BSTNode* n) {
    if (n->right && n->left) {
        return 2;
    }
    else if (n->right || n->left) {
        return 1;
    } 
    return 0;
}

void bst_delete(BST *T, BSTNode *n)
{
    assert(T != NULL);
    assert(n != NULL);
    BSTNode* rMin;
    switch (children_Num(n))
    {
    case 2:
        rMin = bst_minimum(n->right);
        rMin->parent = n->parent;
        rMin->left = n->left;
        rMin->right = n->right != rMin ? n->right : NULL;
        if (n->parent->right == n) {
            n->parent->right = rMin;
        }
        n->parent->left = rMin;
        break;
    case 1:
        if (n->parent->right == n) {
            n->parent->right = n->left ? n->left : n->right;
        }
        n->parent->left = n->left ? n->left : n->right;
        break;
    case 0: 
        if (n->parent->right == n) {
            n->parent->right = NULL;
        }
        n->parent->left = NULL;
        break;
    }
    free(n);
    bst_check(T); /* al termine conviene controllare che la struttura dell'albero sia corretta */
}

/* Funzione ricorsiva che calcola l'altezza dell'albero radicato in
   `n`:

   - Se `n == NULL`, il risultato deve essere -1;

   - Altrimenti, l'altezza dell'albero radicato in `n` è uguale al
     massimo tra l'altezza di `n->left` e `n->right`, più uno.

   Si noti che, dalla regola precedente, deriva che l'altezza di un
   albero composto da un singolo nodo è zero, il che è il risultato
   corretto (l'altezza di un albero è il numero _massimo_ di archi
   attraversati per andare dalla radice alla foglia più profonda).
*/
static int bst_height_rec(const BSTNode *n)
{
    /* [TODO] */
    return -1; /* valore di ritorno fittizio per evitare warning del compilatore */
}

int bst_height(const BST *T)
{
    assert(T != NULL);
    return bst_height_rec(T->root);
}

/* Stampa il contenuto del sottoalbero radicato nel nodo `n`. Il
   formato della stampa è:

   (n->key left right)

   dove `n->key` è il valore presente in `n`, mentre `left` e `right`
   sono il contenuto del sottoalbero sinistro e destro di `n`.  Se `n`
   è vuoto (cioè n == NULL), stampa `()`
*/
static void bst_print_rec( const BSTNode *n )
{
    printf("(");
    if (n != NULL) {
        printf("%d ", n->key);
        bst_print_rec(n->left);
        printf(" ");
        bst_print_rec(n->right);
    }
    printf(")");
}

/* Nota: la funzione di stampa assume che `BSTKey` sia il tipo
   `int` */
void bst_print( const BST *T )
{
    assert(T != NULL);

    bst_print_rec(T->root);
    printf("\n");
}

/* Conviene realizzare la funzione `bst_pretty_print()` in modo che
   richiami la funzione ricorsiva `bst_pretty_print_rec()` che
   effettuerà la stampa vera e propria.

   `bst_pretty_print_rec(n, depth)` stampa il sottoalbero radicato nel
   nodo `n`, che si trova a profondità `depth` dell'albero. Il valore
   `depth` serve per decidere quanti spazi vanno stampati a partire
   dal margine sinistro per fare "rientrare" correttamente il
   sottoalbero. Suggerisco di stampare 3*`depth` spazi prima della
   chiave di `n`. */
static void bst_pretty_print_rec( const BSTNode *n, int depth )
{
    if (n != NULL) {
        /* [TODO] Si suggerisce di procedere come segue:

           1. Stampa ricorsivamente il sottoalbero destro invocando
              bst_pretty_print_rec(n->right, depth+1);

           2. Stampa 3*n spazi, seguiti dal valore della chiave n->key;

           3. Stampa ricorsivamente il sottoalbero sinistro invocando
           bst_pretty_print_rec(n->left, depth+1);
        */
    }
}

void bst_pretty_print( const BST *T )
{
    assert(T != NULL);
    bst_pretty_print_rec(T->root, 0);
}

int bst_size(const BST *T)
{
    assert(T != NULL);

    return T->size;
}

int bst_is_empty(const BST *T)
{
    assert(T != NULL);

    return (T->root == NULL);
}
