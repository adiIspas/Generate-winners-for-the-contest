/**
* © 2015 Toate drepturile rezervate
* Autor: ADRIAN ISPAS, FACULTATE DE MATEMATICA SI INFORMATICA UNIVERSITATEA BUCURESTI
* Pentru: Carti365.Com, Daniel Pavel
*/

#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <cstdlib>

using namespace std;

// Pentru afisare informatii in fisier extern
ofstream out("rezultate.txt", ios_base::app);

const int MAX_NR = 100;		// Aceasta constanta reprezinta numarul total de taguri ce se anticipeaza a fi create
const int MAX_TAG = 100;	// Aceasta constanta reprezinta numarul de taguri anticipat pe concurent

static int numere[MAX_NR];		// Va contine numerele random generate pana la un moment pentru toti participantii
static int nrTotalTag = 0;	// Va contine numarul total de taguri create de participanti
static int nrConcurenti = 0;		// Numarul total de concurenti determinat dupa citirea datelor de intrare

struct InformatiiConcurent{
	string nume;
	int nrTag = 0;
};

class Concurent{
	
	int numereAlocate[MAX_TAG];	 // Va contine numerele unice random alocate fiecarui participant in parte
	string nume;		// Contine numele concurentului
	int nrTag = 0;		   // Contine numarul de taguri create de acesta

public:
	Concurent(){}		// Constructor implicit (nu face nimic, e doar pentru a evita anumite erori si pentru faptul ca cel oferit de compilator se pierde la definirea unuia explicit)
	Concurent(string nume, int nrTag);	// Constructor explicit definit in afara clasei
	string GetNume();	// Metoda de returnare a numelui
	int GetNrTaguri();	// Metoda de returnare a numarului de taguri
	void ShowNumereScreen();	// Metoda de afisare a numerelor generate pe ecran
	void ShowNumereFile();	// Metoda de afisare a numerelor generate in fisier
	bool FindeNumberRandom(int numar); // Metoda primeste ca parametru un numar random si il cauta printre numerel ascociate concurentului
};

// Definitia constructorului explicit
Concurent::Concurent(string nume, int nrTag){
	this->nume = nume;
	this->nrTag = nrTag;

	// Se initializaza generatorul de numere Random
	time_t t;
	srand((unsigned)time(&t));

	// Se construieste vectorul "numereAlocate" cu numere unice alocate fiecarui participant
	int nrRandom = 0;
	for (int i = 0; i < nrTag; i++){
		nrRandom = rand() % nrTotalTag + 1;

		// Se genereaza un alt numar atata timp cat ele deja exista printre numerele generate pana la momentul actual
		while (numere[nrRandom]){
			nrRandom = rand() % nrTotalTag + 1;
		}

		// Se salveaza informatiile, pentru participant numarul generat, iar pentru vectorul ce contine numerele generate se marcheaza si ultimul numar
		numereAlocate[i] = nrRandom;
		numere[nrRandom] = 1;
	}
}

string Concurent::GetNume(){
	return nume;
}

int Concurent::GetNrTaguri(){
	return nrTag;
}

void Concurent::ShowNumereScreen(){
	for (int i = 0; i < nrTag; i++)
		cout << numereAlocate[i] << " ";
}

void Concurent::ShowNumereFile(){
	
	for (int i = 0; i < nrTag; i++)
		out << numereAlocate[i] << " ";
}

bool Concurent::FindeNumberRandom(int numar){

	for (int i = 0; i < nrTag; i++){
		if (numereAlocate[i] == numar)
			return true;
	}

	return false;
}

// Functia va zeroriza vectorul ce contine informatiile despre numerele generate
void NullNumereGenerate(){
	for (int i = 0; i < MAX_NR; i++)
		numere[i] = 0;
}

// Functia va citi informatiile din fisierul de intrare
void CitireInformatii(InformatiiConcurent *info){
	ifstream in("date_concurenti.txt");

	string nume;
	int nrTag;

	// Atata timp cat vor exista concurenti in fisierul de intrare acesta ii va citi
	while (in >> nume >> nrTag){

		// Se salveaza datele despre concurent
		info[nrConcurenti].nume = nume;
		info[nrConcurenti].nrTag = nrTag;

		// Incrementam numarul total de taguri si de concurenti
		nrTotalTag += nrTag;
		nrConcurenti++;
	}
}


// Functia va afisa pe ecran toti concurentii inscrisi cu numerele generate aferente
void ShowScreenInfo(Concurent **concurenti){
	
	for (int i = 0; i < nrConcurenti; i++){
		cout << "Concurentul " << i + 1 << ": " << concurenti[i]->GetNume() << endl;
		cout << "Numerele alocate lui: ";
		concurenti[i]->ShowNumereScreen();
		cout << endl << endl;
	}
}

// Functia va afisa in fisier toti concurentii inscrisi cu numerele generate aferente
void ShowFileInfo(Concurent **concurenti){
	
	for (int i = 0; i < nrConcurenti; i++){
		out << "Concurentul " << i + 1 << ": " << concurenti[i]->GetNume() << endl;
		out << "Numerele alocate lui: ";
		concurenti[i]->ShowNumereFile();
		out << endl << endl;
	}

}

// Functia care extrage un anumite numar de castigatori
void ExtrageCastigatori(Concurent **concurenti){
	
	int nrCastigatori = 0;
	int nrCastigator = 0;
	int gasit = 0;

	cout << "\t\tCati castigatori doriti sa extrageti?\n";
	cout << "\t\t\tNumar castigatori: ";
	cin >> nrCastigatori;

	time_t t;
	srand((unsigned)time(&t));

	cout << "\n\tCei " << nrCastigatori << " castigatori sunt:\n\n";

	// Se vor cauta X castigatori printre concurenti
	for (int i = 0; i < nrCastigatori; i++){

		nrCastigator = rand() % nrTotalTag + 1;
		
		// Atata timp cat nu se gaseste un castigator pentru un numar, se genereaza alt numar
		gasit = 0;
		while (!gasit){

			for (int j = 0; j < nrConcurenti; j++){
				if (concurenti[j]->FindeNumberRandom(nrCastigator)){
					
					// Afisam un gastigator gasit
					gasit = 1;
					cout << "\t" << i + 1 << ". " << concurenti[j]->GetNume() << " cu numarul castigator " << nrCastigator << endl;
					
					// Odata gasit un castigator acesta se sterge pentru a evita situatiile in care acesta poate aparea de 2 ori
					// Acesta este si motivul pentru care odata extrasi castigatorii nu se mai pot efectua alte optiuni asupra listei, deoarece aceasta este eronata.
					delete concurenti[j];

					break;
				}
			}
			
			// Cautam in continuare un numar castigator ...
			nrCastigator = rand() % nrTotalTag + 1;
		}
	}

}

int main(){
	
	// Fisierele de intrare si iesire pentru datele participantilor si pentru rezultatele finale
	//ofstream out("rezultate.txt");

	// Pregatim vectorul pentru generarea numerelor
	NullNumereGenerate();

	// Creem vectorul cu structura de informatii pentru a stoca toti participantii si ai pasa mai departe catre constructor
	InformatiiConcurent *info = new InformatiiConcurent[100];
	
	// Citim informatiile despre concurenti
	CitireInformatii(info);

	// Creem un vector de concurenti, momentan neinitializat
	Concurent *concurenti[100];

	// Pentru fiecare concurent salvat in structura info apelam constructorul explicit pentru a crea un obiect de tip Concurent cu informatiile aferente
	for (int i = 0; i < nrConcurenti; i++){
		concurenti[i] = new Concurent(info[i].nume, info[i].nrTag);
	}
	
	// INFO Soft
	cout << "\n Soft creat de Adrian Ispas pentru Carti365.com | Toate drepturile rezervate\n\n";

	// Optiuni ce pot fi efectuate
	cout << "\n\t\t\t Alegeti una dintre optiunile: \n";
	cout << endl << "\t1. Afiseaza pe ecran concurentii inscrisi si numerele alocate acestora";
	cout << endl << "\t2. Afiseaza in fisier concurentii inscrisi si numerele alocate acestora";
	cout << endl << "\t3. Extrage in mod random un numar de castigatori";
	cout << endl << "\t0. Inchide programul\n";

	int optiune;
	cout << "\n\tOptiune dvs: ";
	cin >> optiune;

	while (optiune){

		switch (optiune){
			case 1: {
						system("cls");
						cout << "\n Soft creat de Adrian Ispas pentru Carti365.com | Toate drepturile rezervate\n\n";
						ShowScreenInfo(concurenti); 

						cout << "\n\t\t\t Alegeti una dintre optiunile: \n";
						//cout << endl << "\t1. Afiseaza pe ecran concurentii inscrisi si numerele alocate acestora";
						cout << endl << "\t2. Afiseaza in fisier concurentii inscrisi si numerele alocate acestora";
						cout << endl << "\t3. Extrage in mod random un numar de castigatori";
						cout << endl << "\t0. Inchide programul\n";
						cout << "\n\tOptiune dvs: ";
						
					} break;
			case 2: {	
						system("cls");
						out << "\n Soft creat de Adrian Ispas pentru Carti365.com " << (char)0xA9 << " Toate drepturile rezervate.\n\n"; 
						cout << "\n Soft creat de Adrian Ispas pentru Carti365.com | Toate drepturile rezervate\n\n";
						ShowFileInfo(concurenti);

						cout << "\t\tINFORMATIILE AU FOST SCRISE CU SUCCES IN FISIER!\n\n";

						out.close();

						cout << "\n\t\t\t Alegeti una dintre optiunile: \n";
						cout << endl << "\t1. Afiseaza pe ecran concurentii inscrisi si numerele alocate acestora";
						//cout << endl << "\t2. Afiseaza in fisier concurentii inscrisi si numerele alocate acestora";
						cout << endl << "\t3. Extrage in mod random un numar de castigatori";
						cout << endl << "\t0. Inchide programul\n";
						cout << "\n\tOptiune dvs: ";
					} break;
			case 3:{
					   system("cls");
					   cout << "\n Soft creat de Adrian Ispas pentru Carti365.com | Toate drepturile rezervate\n\n";

					   ExtrageCastigatori(concurenti);

					   cout << "\t\n Apasati 0 pentru a iesi! ";

				   } break;
			default: {
						 system("cls");
						 cout << "\n Soft creat de Adrian Ispas pentru Carti365.com | Toate drepturile rezervate\n\n";
						 cout << "\t\t\t NU ATI ALES O OPTIUNE VALIDA!\n";
						 cout << "\n\t\t\t Alegeti una dintre optiunile: \n";
						 cout << endl << "\t1. Afiseaza pe ecran concurentii inscrisi si numerele alocate acestora";
						 cout << endl << "\t2. Afiseaza in fisier concurentii inscrisi si numerele alocate acestora";
						 cout << endl << "\t3. Extrage in mod random un numar de castigatori";
						 cout << endl << "\t0. Inchide programul\n";
						 cout << "\n\tOptiune dvs: ";
					} break;
		}
		cin >> optiune;
	}

	return 0;
}