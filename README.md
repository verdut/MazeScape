# MazeScape


Nume:Raileanu Costin
Grupa:251 an II


Tema 2 - Arduino
~MazeScape~


Jocul, idee si descriere

Jocul se numeste MazeScape. Este vorba despre un labirint, un player si un obiect. Player-ul poate impinge obiectul prin labirint fara sa blocheze obiectul intr-un colt. Obiectivul este ca obiectul sa ajunga la iesirea din labirint. 
Se imparte pe nivele si la fiecare final de nivel apare un SmileyFace care arata trecerea cu succes a nivelului si scorul initial.

II. Cum acopera toate punctele necesare?
    
a. Exista meniu de inceput prin care se poate alege nivelul dorit de user printr-un input de la user, sau verificarea highscore-ului, sau meniul de optiuni ( schimbarea intensitatii) sau resetarea nivelelor deblocate si a highscore-ului(care sunt salvate pe EEPROM)
b.La castigarea unui nivel se afiseaza scorul obtinut, un smiley face si se revine la meniul principal.(cel de la punctul a).
  De asemenea se poate iesi dintr-un nivel curent in cazul in care s-a ajuns intr-un punct mort.
c. 1) Este impartit pe nivele, unul mai greu decat celalalt, dar vor fi generate manual deoarece anumite generari automate sau random pot sa nu aiba solutie.
    2)Are scor calculat in functie de numarul de miscari care s-au executat pana la terminarea nivelului. Exista si highscore.
 d. Nu exista limita de timp pentru nivele indiferent de dificultate deci jocul poate fi jucat cu experienta mediocra.
 e. Exista end-game(dar nu exista vreun ecran de final) cand toate nivele sunt terminate cu succes, insa asta nu impiedica user-ul sa poata da replay la nivele. 
 

III. Evolutia in timp

         Vor exista in jur de 10 nivele in total. Scorul se va calcula pe baza numarului de miscari executate de player pana la ducerea obiectului la iesirea din labirint. Pentru fiecare nivel exista un numar  minim de miscari in care acesta poate fi rezolvat. Scorul va fi calculat in functie de numarul minim de miscari si numarul de miscari executate de player pana la finalul jocului.

IV. Metode de control
       
       Player-ul se misca cu ajutorul joystick-ului.(stg,dr,sus,jos)
       Switch-ul de pe joystick va avea mai multe intrebuintari in functie de modulul de program din care face parte. Adica in meniuri va avea functie de reset, in timpul unui nivel va avea functie de restart nivel.
       
   
V. Alte elemente conexe

   Voi folosi display-ul LCD pentru a fi mai usor sa afisez anumite texte si detalii ca sa fie mai usor de inteles jocul si pentru afisarea scorului si a meniului.

