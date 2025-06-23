# Završni rad: Nesigurna aplikacija s namjernim ranjivostima

## 1. Naziv projekta

**Nesigurna aplikacija za upravljanje korisnicima i novcem**

## 2. Opis projekta

Ovaj projekt predstavlja jednostavnu aplikaciju za upravljanje korisničkim računima i financijama, s osnovnim funkcionalnostima kao što su registracija korisnika, prijava i uplata sredstava. Aplikacija je izrađena u programskom jeziku C i koristi SQLite bazu podataka za pohranu podataka.

Specifičnost ove aplikacije je da sadrži više namjerno implementiranih sigurnosnih ranjivosti. Cilj je demonstrirati koliko lako ranjivosti mogu biti uvedene u programski kod ako se ne poštuju sigurnosne prakse tijekom razvoja.

## 3. Cilj projekta

Cilj projekta je ilustrirati i objasniti uobičajene sigurnosne propuste koji se pojavljuju prilikom razvoja aplikacija. Aplikacija služi kao edukativno sredstvo za analizu sigurnosnih grešaka i vježbu detekcije i iskoristavanje ranjivosti.

## 4. Pregled funkcionalnosti

- Registracija korisnika
- Prijava korisnika
- Uplata sredstava
- Pregled trenutnog stanja računa

## 5. Implementirane ranjivosti

Sljedeće ranjivosti su namjerno uključene u aplikaciju radi edukativnih i demonstracijskih svrha:

1. **Buffer overflow** – Prilikom unosa korisničkog imena moguće je prepisati memorijski prostor ako se unese predugačak string. Tako se može prepisati povratnu adresu funkcije jer string raste prema njoj, pa se program pri povratku iz nje ruši. Moderni sustavi se od toga štite provjerom veličine ulaza i multiprocessingom pri obradi zahtjeva da se ugasi samo jedan proces.
2. **SQL injection** – Prilikom prijave moguće je koristiti posebne znakove (`'--`) kako bi se zaobišla autentikacija ili izvršile razne SQL naredbe. Prilikom prijave moguće je za username upisati validno ime i dodati znakove "'--" kako se ne bi provjeravalo je li unesena ispravna lozinka.
3. **Format string** – Ako se korisnik registrira s korisničkim imenom koje sadrži `%x` specifiere, moguće je pristupiti sadržaju memorije putem printf funkcije koja će ispisati vrijednosti sa memorijskih lokacija umjesto imena.
4. **Dereferenciranje oslobođene memorije** – U slučaju da korisnik pokuša uplatiti 0, memorija se najprije oslobodi, a zatim se dereferencira, što može uzrokovati nepredvidivo ponašanje.
5. **Memory leak** – Ako korisnik uplati validan iznos, funkcija se izvrši bez oslobađanja memorije, što dovodi do curenja memorije, osobito štetnog pri većem broju procesa i korisnika.
6. **No input validation** – Ako korisnik pokuša uplatiti iznos koji nije broj, dolazi do neocekivanog ponašanja programa. Program ne inicijalizira broj za uplatu, ali kada ga pokuša pročitati, čita memorije sa heapa na kojima je prije bilo nešto, pa su sada te vrijednposti nasumične, dok se ne pronađe neka vrijednost koja zadovoljava uvjet veličine, pa se tako sa računa oduzme nepredvidiva vrijednost.

## 6. Tehnologije

- C programski jezik
- SQLite baza podataka
- Windows okruženje
- GCC kompajler

## 7. Struktura projekta

Aplikacija/
├── build.bat # Skripta za kompajliranje i pokretanje aplikacije na Windows sustavu
├── main.c # Glavni izvorni kod aplikacije
├── README.md # Ova datoteka s opisom projekta
├── sqlite3.c # Izvorni kod SQLite3 biblioteke
├── sqlite3.h # Header datoteka SQLite3 biblioteke
└── test.db # SQLite baza podataka za pohranu korisničkih podataka u kojoj postoji zapis korisnika Tomislav, kojem je sifra 11111

## 8. Upute za instalaciju i pokretanje

### Preduvjeti

- Instaliran GCC kompajler
- Instaliran SQLite (`sqlite3.c` i `sqlite3.h` u direktoriju projekta)

### Kompilacija ručno:

gcc main.c sqlite3.c -o main.exe
./main.exe

### Alternativno: Korištenje build.bat

.\build.bat
## Pokreće kompilaciju i program main.exe nastao kompilacijom

