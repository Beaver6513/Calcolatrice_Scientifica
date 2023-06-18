Il progetto contiene un MAKEFILE che permette di compilare e lanciare il programma, le task sono le seguenti:
    all: compila il programma
    clean: cancella tutti i file .d* e .o per permettere una ricompilazione pulita
    run: compila e lancia il programma
    debug: lancia il programma gdb e avvia il debug dell'eseguibile
    val: lancia il programma utilizzando valgrind

il compilatore utilizzato è gcc, il debugger utilizzato è gdb-gef

* I file .d sono file utilizzati da un utile feature di Makefile che permette di rendere la ricompilazione più intelligente

Sono inoltre presenti due script in python che permettono la stampa della lista utilizzata. 
Una volta caricati tramite il comando source su gef, o inserendo il comando source (con path assoluta al file dello script) nel file .gdbinit , i comandi sono i seguenti:

plist <head of list>
plistr <tail of list>

I due comandi stampano la lista partendo da sinistra e da destra, ciò permette di debuggare casi in cui i next sono tutti collegati ma non lo sono i previous, ad esempio.
Lo script se non riconosce il tipo inserito nell'argomento del comando, stamperà quello che ha ricevuto.