=======================================================================
======================= Struktura slozek BP ===========================
-----------------------------------------------------------------------
=======================================================================
-----------------------------------------------------------------------
- FITracer2015_bin (slozka se spustitelnym programem)
  - data (slozka s 3D modely)
  - kernels (zdrojove kody pouzitych kernelu)
  - shaders (zdrojove kody pouzitych shaderu)
  - images (obrazky pouzite v animaci)
  - fonts (font zobrazeneho pisma)
  - Fitracer2015.exe (spustitelny program) 

Navod na spusteni:
1) Ve slozce FITracer2015_bin spuste Fitracer2015.exe
2) Vyberte v prikazove radce OpenCL zarizeni, ktere bude pouzite pro vypocet
3) Pokud zarizeni podporuje vsechny potrebne vlastnosti spusti se okno aplikace. V opacnem pripade aplikace oznami chybu a ukonci se.
4) Pro spusteni vyberu je nutne vybrat jednu z moznosti v kolonkach
   "Choose rendering mode" a "Choose accelerated data structure" a kliknout 
   na tlacitko se symbolem "||".

Poznamky: U volby CPU renderer se zobrazuje v okne aplikace i v prikazove radce aktualni stav vypoctu. Vypocet lze kdykoliv pozastavit (cas vypoctu ale uz nebude odpovidat realne hodnote). Po spusteni s volbou GPU renderer
neni zadnym zpusobem indikovan stv vypoctu a je nutne pockat na jeho
dokonceni. U teto volby se pozastaveni vypoctu aplikuje az po dokonceni 
stavajiciho vypoctu. Vysledne snimky si lze v kterekoliv fazi vypoctu 
ulozit na disk skrz tlacitko "Save". 
-----------------------------------------------------------------------
=======================================================================
-----------------------------------------------------------------------
- FITracer2015_solution (slozka se solution pro VS2013)
  - data (slozka s 3D modely)
  - kernels (zdrojove kody pouzitych kernelu)
  - shaders (zdrojove kody pouzitych shaderu)
  - include (hlavickove soubory pouzite v projektu)
  - lib (knihovny ve verzi pro x86 i x64)
  - src (zdrojove kody pouzite v projektu)
  - images (obrazky pouzite v animaci)
  - fonts (font zobrazeneho pisma)
  - Fitracer2015.sln (solution soubor projektu)

Poznamky: Pro uspesne prelozeni projektu je nutne definovat v nastaveni
projektu vsechny cesty ke slozkam include a lib. Pro spusteni je potreba
vlastnit grafickou kartu s API OpenGL 3.0+ a procesor nebo grafickou kartu
s podporou API OpenCL 1.1+ (netestovano na OpenCL 1.0).
-----------------------------------------------------------------------
=======================================================================
-----------------------------------------------------------------------