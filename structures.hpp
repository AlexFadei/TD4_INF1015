#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <cassert>
#include "gsl/span"
#include <memory>
#include <iostream>
#include <cppitertools/enumerate.hpp>
#include <cppitertools/range.hpp>
using gsl::span;
using namespace::std;
using namespace::iter;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront définis après.

struct Acteur
{
	string nom; int anneeNaissance; char sexe;
	//ListeFilms joueDans;
	Acteur()
	{
		nom = {};
		anneeNaissance = 0;
		sexe = 'U';
	}
	friend ostream& operator<<(ostream& o, Acteur& acteur) {
		return o << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
	}

	friend ostream& operator<<(ostream& o, Acteur* acteur) {
		return o << "  " << acteur->nom << ", " << acteur->anneeNaissance << " " << acteur->sexe << endl;
	}
};

class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur>  trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	Film& operator[](int index); //reference a l objet voulu puisque l'idee c'est de pouvoir y acceder et non de la pointer

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};
//struct ListeActeurs {
//	int capacite, nElements;
//	unique_ptr<shared_ptr<Acteur>[]> elements = nullptr; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
//	ListeActeurs() {
//		elements = nullptr;
//		nElements = capacite = 0;
//	}
//	ListeActeurs(int grandeur) {
//		elements = make_unique<shared_ptr<Acteur>[]>(grandeur);
//		capacite = grandeur;
//		nElements = grandeur;
//	}
//	ListeActeurs(const ListeActeurs& autre) = default;
//
//	ListeActeurs& operator=(const ListeActeurs& autre) {
//		capacite = autre.capacite;
//		nElements = autre.nElements;
//		elements = make_unique<shared_ptr<Acteur>[]>(autre.capacite);
//		for (auto&& [i, elem] : enumerate(span(autre.elements.get(), autre.nElements)))
//			elements[i] = elem;
//		
//		return *this;
//	}
//
//	Acteur& operator[](const int index){
//		return *this->elements[index];
//	}
//
//};

template <typename T> class Liste {
private:
	int capacite_;
	int nElements_;
	unique_ptr<shared_ptr<T>[]> elements_ = nullptr;
	void changeDimension(int newCapacite) {
		unique_ptr<shared_ptr<T>[]> nouvelleListe = make_unique<shared_ptr<T>[]>(newCapacite);
		if (elements_ != nullptr) {
			nElements_ = min(nElements_, newCapacite);
			for (int i : range(nElements_)) {
				nouvelleListe[i] = elements_[i];
			}
		}
		elements_ = move(nouvelleListe); //transfere le groupe pointer par unique_ptr a un autre
		capacite_ = newCapacite;
	}
public:
	Liste() = default;
	Liste(int capacite) : capacite_(capacite) {
		nElements_ = capacite;
		elements_ = make_unique<shared_ptr<T>[]>(capacite_);
	};
	Liste(const Liste& autre) {
		*this = autre;
	};
	void addElem(T& elem) {
		if (capacite_ == nElements_){
			changeDimension(max(1, capacite_ * 2));
		}
		if (elements_[nElements_-1] != nullptr) {
			elements_[nElements_++] = make_shared<T>(elem);
		}
		else {
			elements_[nElements_-1] = make_shared<T>(elem);
		}
		
			

	}
	Liste& operator=(const Liste& autre) {
		capacite_ = autre.capacite_;
		nElements_ = autre.nElements_;
		elements_ = make_unique<shared_ptr<T>[]>(autre.capacite_);
		for (auto&& [i, elem] : enumerate(span(autre.elements_.get(), autre.nElements_)))
			elements_[i] = elem;

		return *this;
	}
	int size() const { return nElements_; }
	shared_ptr<T>* get() const { return elements_.get(); } //ici elements.get() doit etre preciser car la liste contient des shared_ptrs
	shared_ptr<T>& operator[]( int index) const{
		return elements_[index];
	}
	
	

};
using ListeActeurs = Liste<Acteur>;

struct Film
{
	string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;

	Film(const string& titre, const string& realisateur, int anneeSortie, int recette, int nActeurs) :titre(titre), realisateur(realisateur),
																						anneeSortie(anneeSortie),recette(recette),
																						acteurs(nActeurs) 
	{
	}
	Film(const Film& film) {
		*this = film;
	}
	Film& operator=(const Film& autre) {
		titre = autre.titre;
		realisateur = autre.realisateur;
		anneeSortie = autre.anneeSortie;
		recette = autre.recette;
		acteurs = autre.acteurs; 
		return *this;
	}

	friend ostream& operator<<(ostream& o,Film& film)
	{
		o << "Titre: " << film.titre << endl;
		o << "  Réalisateur: " << film.realisateur << "  Année :" << film.anneeSortie << endl;
		o << "  Recette: " << film.recette << "M$" << endl;
		o << "Acteurs:" << endl;
		for (int i = 0; i < film.acteurs.size(); i++)
			o << " " << film.acteurs[i] << endl;
		return o;
	}

};


