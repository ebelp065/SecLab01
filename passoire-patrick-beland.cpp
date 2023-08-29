// Patrick Béland
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <fstream>
#include <iostream>

// Utile pour que le compilateur puisse trouver cout et stoi sans écrire std::.
using namespace std;

int main(int argc, char* argv[])
{
	// Je vais utiliser des enums pour clarifier la fonction des arguments.
	enum arguments
	{
		CHEMIN_PROGRAMME,
		NOMBRE_PANS,
		MONTANT_A_PAYE,
		NOM_FICHIER,
		// CONTENU_FICHIER, <- pas utile finalement
		COMMANDE
	};
	// Il faudrait vérifier si argv[?] n'est pas nul pour ne pas accéder à l'addresse 0 et avoir un page fault.
	// Il faut vérifier combien il y a d'arguments. Il va en avoir autant que la valeur de argc, il faut arrêter l'exécution du programme s'il manque des arguments.
	// Je vais changer les index utilisés pour que ce soit plus logique que de commencer avec l'index 1 de argv.
	// Je vais limiter les arguments à 5 et vérifier que argc est bien 5 pour ne pas manquer d'argument et devoir fermer plus tard alors qu'on sais que le format est mauvais.
	if (argc != 5)
	{
		cout << "ERREUR: Il faut 4 arguments. Vérifiez le guide d'utilisateur pour déterminer le sens de ceux-ci";
		exit(-1);
	}

	int pansDeCloture{ stoi(argv[NOMBRE_PANS]) };
	float montantAPayer{ stof(argv[MONTANT_A_PAYE]) };

	// Vérifier que le diviseur n'est pas 0, pour éviter de générer une exception.
	if (pansDeCloture == 0)
	{
		std::cout << "ERREUR: Aucun pans.";
		exit(-2);
	}
	float coutParPan{ montantAPayer / pansDeCloture };

	// Poteaux nécessaires pour faire une clôture en ligne droite
	int poteaux{ pansDeCloture };
	montantAPayer += poteaux * 5;

	// Ce serait mieux d'avoir un tampon plus long pour contenir la taille maximale d'un nom de fichier, probablement 255. Sinon, on va pas pouvoir utiliser ce nom.
	// Ici je présuppose que l'utilisateur va entrer un nom valide, et qu'il ne va probablement pas dépasser la limite, mais il pourrait vouloir faire ça.
	// S'il dépasse la limite, il ne va rien se passer à moins d'atteindre la fin de la page de mémoire ce qui va générer un page fault.
	// On peut initialiser le tampon à 0 pour ne pas avoir à mettre un caractère nul à la fin manuellement pour identifier la fin du nom de fichier.
	char nomFichier[256] = {0};
	strcpy(nomFichier, argv[NOM_FICHIER]);
	// Il faut éviter d'ajouter un .exe si le nom donné l'inclut déjà. Sinon, le nom sera incorrect et le fichier ne pourra pas s'ouvrir.
	// La création du fichier n'est pas utile, mais je vais laisser le code pareil.
	// Une personne malveillante pourrait exécuter le programme en boucle avec un nom différent pour créer du stress sur le système de fichier.
	string nouveauTamponString(nomFichier);
	if(nouveauTamponString.find(".exe") == string::npos)
	{
		strcat(nomFichier, ".exe");
	}
	ofstream fichier{ nomFichier };
	// Je vais fermer le fichier pour éviter que le système d'exploitation le bloque indéfiniment.
	fichier.close();
	// On peut utiliser cet argument de contenu pour ajouter du code dangereux à l'exécutable.
	// On ne peut pas régler le problème sans enlever la ligne, elle n'est pas utile de toute façon. Je vais seulement la mettre en commentaire.
	// fichier << argv[CONTENU_FICHIER];

	// Je vais enlever le code qui imprime le contenu aussi puisqu'on ne l'utilise plus.
	// const char* msg{ argv[CONTENU_FICHIER] };
	// J'ai adapté l'argument de formatage ici, j'utilise le format qui semble le plus logique selon le nom des arguments.
	// Ça aurait été possible d'utiliser l'argument de contenu du programme pour écrire les deux bons formats, mais c'est délicat de le faire manuellement dans la console.
	// Utiliser un format fixe permet d'éviter les situations où la librarie de C utiliserait des valeurs non-définies sur la pile d'exécution.
	printf("Nombre de pans: %d\nCout par pan: %f\n", pansDeCloture, coutParPan);

	// C'est risqué de permettre d'exécuter une commande arbitraire parce que cela pourrait endommager les fichiers de la machine.
	// Je vais utiliser la commande Pause, car l'utilisateur veut probablement voir le résultat du programme au lieu de fermer la console rapidement.
	const char* commande = "pause";
	system(commande);
}