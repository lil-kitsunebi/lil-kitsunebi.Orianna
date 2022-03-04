#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <conio.h>
using namespace std;

//Code écrit par Marie-Lee Potvin
//Le code pour le système de combat, les tableaux 2D et le système de naviagation dans les salles avec pointeur est basé sur ce que nous avons vu en classe/le code de Félix Soumpholphakdy.
//Il a par la suite été modifié selon les besoins du jeu.

bool onRecommence = false;
string combien = "Combien de points voulez-vous attribuer à votre ";
string erreurAttributionPoints = "\nErreur! Le nombre de points que vous voulez attribuer est invalide. Voulez-vous recommencer au début pour l'attribution des points? Veuillez répondre par ''oui'' ou par ''non''.\n";
string reponse = "non";
string erreurCreationPerso = "\nErreur. Vous devez créer ou charger la fiche d'un personnage.\n";
string erreurInvalide = "Entrée invalide.\n";
string erreurRestorationHP = "\nErreur. PV est déjà à 100%.\n";
string erreurArgentManquant = "\nErreur. Vous n'avez pas assez de pièces d'or pour un tel achat.\n";
string quitterMine = "\nAyant perdu votre combat, vous aller quitter la mine.\n";
string pvZero = "\nVos PV sont à zéro. Pour les récupérer, vous devez vous rendre à la taverne ou à l'auberge.\n";

int equipArme = 0;
string nomArme = "-";
int equipBouclier = 0;
string nomBouclier = "-";
int capeKitsune = 0;
string itemKit = "-";
int bagueAvarice = 1;
string itemAvarice = "-";
int braceletEnfer = 1;
string itemEnfer = "-";
int protecteur = 1;
string itemProtecteur = "-";

string nomPerso;
int pbonus = 5;
int totalVital = 2;
int totalForce = 2;
int totalVitesse = 2;
int initiative = totalVitesse * 10;
int pAttribue = 0;
int pvPersoMax = (totalVital * 10) + capeKitsune;
int pvPersoActuel = pvPersoMax;
int potion = 2;

vector<string> races;
int numeroRace;
string racePerso;

int indexM = -1;
int changeMapMine = 0;
int nombrePotion;
int coutPotion;
int combatRemporte = 0;
int orExt = 15;
int orTemple = 10;
int orLab1 = 20;
int orLab2 = 35;

void ListeRaces()
{
	races.push_back("1 - Centaure");
	races.push_back("2 - Dragonborn");
	races.push_back("3 - Dwarf");
	races.push_back("4 - Elf");
	races.push_back("5 - Gnome");
	races.push_back("6 - Harpie");
	races.push_back("7 - Humain");
	races.push_back("8 - Kitsune");
	races.push_back("9 - Lizardfolk");
	races.push_back("10 - Merfolk");
	races.push_back("11 - Minotaure");
	races.push_back("12 - Orc");
}

void equipEtItems()
{
	//items
	if (itemKit == "-CapeKinetsuki-")
	{
		capeKitsune = 333;
	}
	if (itemAvarice == "-BagueAvarice-")
	{
		bagueAvarice = 2;
	}
	if (itemEnfer == "-BraceletEnfer-")
	{
		braceletEnfer = 2;
	}
	if (itemProtecteur == "-Protecteur-")
	{
		protecteur = 2;
	}

	//armes
	if (nomArme == "-Dague-")
	{
		equipArme = 20;
	}
	if (nomArme == "-Arc-")
	{
		equipArme = 20;
	}
	if (nomArme == "-ÉpéeSimple-")
	{
		equipArme = 100;
	}
	if (nomArme == "-HacheSimple-")
	{
		equipArme = 100;
	}
	if (nomArme == "-ÉpéeDesRois-")
	{
		equipArme = 300;
	}
	if (nomArme == "-LanceDesDieux-")
	{
		equipArme = 600;
	}

	//boucliers
	if (nomBouclier == "-PetitBouclier-")
	{
		equipBouclier = 10;
	}
	if (nomBouclier == "-BouclierMoyen-")
	{
		equipBouclier = 30;
	}
	if (nomBouclier == "-GrandBouclier-")
	{
		equipBouclier = 100;
	}

	if (nomBouclier == "-BouclierDesRois-")
	{
		equipBouclier = 200;
	}
	if (nomBouclier == "-BouclierDesDieux-")
	{
		equipBouclier = 600;
	}
}

string inventairePotion[1] =
{
	"Potion Suprême"
};

struct Perso
{
	string nom = "?";
	int pvActuel = 0;
	int pvMax = 0;
	int degats = 0;
	int defense = 0;
	int exp = 0;
	int pOr = 0;
	int initiative = 0;
	int vitesse = 0;
	int arme = 0;
	string nameArme = "?";
	string nameBouclier = "?";
	string capeKit = "?";
	string bagueAvarice = "?";
	string braceletEnfer = "?";
	string protecteur = "?";

	Perso(string _nom, int _pvActuel, int _pvMax, int _degats, int _defense, int _exp, int _pOr, int _initiative, int _vitesse, int _arme, string _nameArme, string _nameBouclier, string _capeKit, string _bagueAvarice, string _braceletEnfer, string _protecteur)
	{
		nom = _nom;
		pvActuel = _pvActuel;
		pvMax = _pvMax;
		degats = _degats;
		defense = _defense;
		exp = _exp;
		pOr = _pOr;
		initiative = _initiative;
		vitesse = _vitesse;
		arme = _arme;
		nameArme = _nameArme;
		nameBouclier = _nameBouclier;
		capeKit = _capeKit;
		bagueAvarice = _bagueAvarice;
		braceletEnfer = _braceletEnfer;
		protecteur = _protecteur;
	}

	bool stillAlive()
	{
		return pvActuel > 0;
	}
};
vector<Perso> monster;
Perso joueur(nomPerso, pvPersoActuel, pvPersoMax, totalForce, equipBouclier* protecteur, 0, pbonus, initiative, totalVitesse, equipArme* braceletEnfer, nomArme, nomBouclier, itemKit, itemAvarice, itemEnfer, itemProtecteur);

void creerPerso()
{
	equipArme = 0;
	nomArme = "-";
	equipBouclier = 0;
	nomBouclier = "-";
	capeKitsune = 0;
	itemKit = "-";
	bagueAvarice = 1;
	itemAvarice = "-";
	braceletEnfer = 1;
	itemEnfer = "-";
	protecteur = 1;
	itemProtecteur = "-";
	potion = 2;

	cout << "\nNous allons créer votre personnage!\n";

	while (true)
	{
		cout << "\nEntrez votre nom:\n";
		cin >> nomPerso;

		bool onrecommence = false;

		while (true)
		{
			//boucle on recommence ou non a lattribution des points si erreur
			while (true)
			{
				cout << "\nVotre vitalité, force et vitesse ont 2 points dès le commencement et vous devez répartir 5 points entre ces trois caractéristiques. Si vous n'attribuez pas tous les points disponibles, ceux-ci seront convertis en pièces d'or.\n";

				pbonus = 5;
				totalVital = 2;
				totalForce = 2;
				totalVitesse = 2;
				pAttribue = 0;
				bool onRecommence = false;

				while (true)
				{
					cout << "\n" << combien << "vitalité (2)? Il vous reste (" << pbonus << ") points:\n";
					cin >> pAttribue;

					if (pAttribue < 0 || pAttribue > pbonus)
					{
						cout << erreurAttributionPoints;
						cin >> reponse;

						if (reponse == "non" || reponse == "Non")
						{
							continue;
						}
						onRecommence = true;
						break;
					}
					break;
				}

				if (onRecommence)
				{
					continue;
				}

				totalVital += pAttribue;
				pbonus -= pAttribue;
				pvPersoMax = totalVital * 10;
				pvPersoActuel = pvPersoMax;

				while (true)
				{
					cout << "\n" << combien << "force (2)? Il vous reste (" << pbonus << ") points:\n";
					cin >> pAttribue;

					if (pAttribue < 0 || pAttribue > pbonus)
					{
						cout << erreurAttributionPoints;
						cin >> reponse;

						if (reponse == "non" || reponse == "Non")
						{
							continue;
						}
						onRecommence = true;
						break;

					}
					break;
				}
				if (onRecommence)
				{
					continue;
				}

				totalForce += pAttribue;
				pbonus -= pAttribue;

				while (true)
				{
					cout << "\n" << combien << "vitesse (2)? Il vous reste (" << pbonus << ") points:\n";
					cin >> pAttribue;

					if (pAttribue < 0 || pAttribue > pbonus)
					{
						cout << erreurAttributionPoints;
						cin >> reponse;

						if (reponse == "non" || reponse == "Non")
						{
							continue;
						}
						onRecommence = true;
						break;
					}
					break;
				}
				if (onRecommence)
				{
					continue;
				}

				totalVitesse += pAttribue;
				pbonus -= pAttribue;

				cout << "\n" << "Les points restants (" << pbonus << ") vont être convertis en pièces d'or.\n";


				//boucle pour les races
				while (true)
				{
					cout << "\nChoisisez une race dans celles suivantes en inscrivant le chiffre approprié: \n";

					for (int i = 0; i < races.size(); i++)
					{
						cout << races[i] << "\n";
					}

					numeroRace;
					cin >> numeroRace;

					//Selon le chiffre inscrit, on met la bonne valeur au string racePerso
					if (numeroRace == 1)
					{
						racePerso = "Centaure";
					}
					if (numeroRace == 2)
					{
						racePerso = "Dragonborn";
					}
					if (numeroRace == 3)
					{
						racePerso = "Dwarf";
					}
					if (numeroRace == 4)
					{
						racePerso = "Elf";
					}
					if (numeroRace == 5)
					{
						racePerso = "Goblin";
					}
					if (numeroRace == 6)
					{
						racePerso = "Harpie";
					}
					if (numeroRace == 7)
					{
						racePerso = "Humain";
					}
					if (numeroRace == 8)
					{
						racePerso = "Kitsune";
					}
					if (numeroRace == 9)
					{
						racePerso = "Lizardfolk";
					}
					if (numeroRace == 10)
					{
						racePerso = "Merfolk";
					}
					if (numeroRace == 11)
					{
						racePerso = "Minotaure";
					}
					if (numeroRace == 12)
					{
						racePerso = "Orc";
					}
					break;
				}
				if (onRecommence)
				{
					continue;
				}
				break;
			}
			break;
		}

		cout << "\nVous avez fini la création de votre personnage. Êtes-vous satisfait? Veuillez répondre par ''oui'' ou ''non''.\n";
		string recommencer;
		cin >> recommencer;

		if (recommencer == "non" || recommencer == "Non" || recommencer == "NON")
		{
			cout << "\nPuisque vous avez répondu par ''non'', vous allez recommencer la création de votre personnage.\n";
			continue;
		}

		else
		{
			cout << "\nFélicitations, vous avez créé un personnage! N'oubliez pas de sauvegarder votre fiche.\n";
			joueur = Perso(nomPerso, pvPersoActuel, pvPersoMax, totalForce, equipBouclier* protecteur, 0, pbonus, initiative, totalVitesse, equipArme* braceletEnfer, nomArme, nomBouclier, itemKit, itemAvarice, itemEnfer, itemProtecteur);
			break;
		}
	}
}

void persoDefaut()
{
	equipArme = 0;
	nomArme = "-";
	equipBouclier = 0;
	nomBouclier = "-";
	equipArme = 0;
	equipBouclier = 0;
	capeKitsune = 0;
	itemKit = "-";
	bagueAvarice = 1;
	itemAvarice = "-";
	braceletEnfer = 1;
	itemEnfer = "-";
	protecteur = 1;
	itemProtecteur = "-";
	potion = 2;

	nomPerso = "ChickenNugget";
	totalVital = 4;
	totalForce = 4;
	totalVitesse = 2;
	pbonus = 1;
	pvPersoMax = totalVital * 10;
	pvPersoActuel = pvPersoMax;
	racePerso = "Harpie";

	cout << "\nLa création de votre personnage par défaut est terminée! N'oubliez pas d'enregistrer la fiche.\n";
	joueur = Perso(nomPerso, pvPersoActuel, pvPersoMax, totalForce, equipBouclier * protecteur, 0, pbonus, initiative, totalVitesse, equipArme * braceletEnfer, nomArme, nomBouclier, itemKit, itemAvarice, itemEnfer, itemProtecteur);

}

void caracteristiquesPerso()
{
	if (nomPerso == "")
	{
		cout << erreurCreationPerso;
	}

	else
	{
		cout << "\nNom: " << nomPerso << "\nVitalité: " << totalVital << "\nForce: " << totalForce << "\nVitesse: " << totalVitesse << "\nPièces d'or: " << pbonus;
		cout << "\nPoints de vie (PV): " << pvPersoActuel << "/" << pvPersoMax << "\nRace: " << racePerso << "\nÉquipement: " << nomArme << nomBouclier;
		cout << "\nItems: " << itemKit << itemAvarice << itemEnfer << itemProtecteur << "\nPotion: " << potion << "\n";

		cout << "\nPensez à sauvegarder votre fiche régulièrement pour ne pas perdre vos progrès.\n";
	}
}

void savePerso()
{
	ofstream file{ "saveperso.txt" };

	if (file)
	{
		file << nomPerso << "\n" << totalVital << "\n" << totalForce << "\n" << totalVitesse << "\n" << pbonus << "\n" << pvPersoMax << "\n";
		file << pvPersoActuel << "\n" << racePerso << "\n" << nomArme << "\n" << nomBouclier << "\n" << itemKit << "\n" << itemAvarice;
		file << "\n" << itemEnfer << "\n" << itemProtecteur << "\n" << potion << "\n";
		cout << "\nLa fiche du personnage " << nomPerso << " a été enregistrée avec succès.\n";

		//Autres elements a enregistrer
		file << changeMapMine << "\n" << orExt << "\n" << orTemple << "\n" << orLab1 << "\n" << orLab2 << "\n";
	}
	else
	{
		cout << "\nError opening file.\n";
	}
}

void chargeSave()
{
	ifstream file{ "saveperso.txt" };

	if (file)
	{
		string creationperso;
		file >> nomPerso >> totalVital >> totalForce >> totalVitesse >> pbonus >> pvPersoMax >> pvPersoActuel >> racePerso;
		file >> nomArme >> nomBouclier >> itemKit >> itemAvarice >> itemEnfer >> itemProtecteur >> potion;
		cout << "\nLe chargement de votre personnage est complété.\n";

		//Autres elements a charger
		file >> changeMapMine >> orExt >> orTemple >> orLab1 >> orLab2;

		//Pour vector Perso joueur
		file >> joueur.nom >> joueur.pvActuel >> joueur.pvMax >> joueur.degats >> joueur.defense >> joueur.exp >> joueur.pOr >> joueur.initiative >> joueur.vitesse >> joueur.arme;
		file >> joueur.nameArme >> joueur.nameBouclier >> joueur.capeKit >> joueur.bagueAvarice >> joueur.braceletEnfer >> joueur.protecteur;
	}
	else
	{
		cout << "\nError opening file.\n";
	}
}

void tousLesMonster()
{
	//chiffres à côté en commentaire pour aider lors du code dans zone de combat et mine1/2

	//Monstres pour la section combat
	Perso slime("Slime", 10, 10, 2, 0, 30, 2, 0, 0, 0, " ", " ", " ", " ", " ", " "); //0
	monster.push_back(slime);
	Perso goblin("Goblin", 100, 100, 20, 5, 60, 6, 0, 0, 0, " ", " ", " ", " ", " ", " "); //1
	monster.push_back(goblin);
	Perso kelpie("Kelpie", 500, 500, 100, 10, 150, 15, 0, 0, 0, " ", " ", " ", " ", " ", " "); //2
	monster.push_back(kelpie);
	Perso goule("Goule", 1500, 1500, 300, 20, 300, 40, 200, 0, 0, " ", " ", " ", " ", " ", " "); //3
	monster.push_back(goule);
	Perso loupGarou("Loup-Garou", 4000, 4000, 800, 40, 600, 100, 800, 0, 0, " ", " ", " ", " ", " ", " "); //4
	monster.push_back(loupGarou);
	Perso basilic("Basilic", 10000, 10000, 2000, 90, 1200, 200, 1500, 0, 0, " ", " ", " ", " ", " ", " "); //5
	monster.push_back(basilic);
	Perso demon("Démon supérieur", 30000, 30000, 6000, 90, 2000, 300, 3000, 0, 0, " ", " ", " ", " ", " ", " "); //6
	monster.push_back(demon);
	Perso celeste("Être céleste", 90000, 90000, 18000, 270, 6000, 500, 9000, 0, 0, " ", " ", " ", " ", " ", " "); //7
	monster.push_back(celeste);
	Perso dragon("Dragon ancien", 300000, 300000, 54000, 700, 18000, 700, 21000, 0, 0, " ", " ", " ", " ", " ", " "); //8
	monster.push_back(dragon);
	Perso dieu("Dieu déchu", 1000000, 1000000, 170000, 2100, 55000, 2100, 63000, 0, 0, " ", " ", " ", " ", " ", " "); //9
	monster.push_back(dieu);

	//Quete #2, mine
	Perso rat("Rat géant", 1500, 1500, 300, 20, 300, 40, 200, 0, 0, " ", " ", " ", " ", " ", " "); //10
	monster.push_back(rat);
	Perso chauvesouris("Chauve-souris géante", 1500, 1500, 300, 20, 300, 40, 200, 0, 0, " ", " ", " ", " ", " ", " "); //11
	monster.push_back(chauvesouris);
	Perso hobgoblin("Hobgoblin", 4000, 4000, 800, 40, 600, 100, 800, 0, 0, " ", " ", " ", " ", " ", " "); //12
	monster.push_back(hobgoblin);
	Perso orc("Orc", 4000, 4000, 800, 40, 600, 100, 800, 0, 0, " ", " ", " ", " ", " ", " "); //13
	monster.push_back(orc);
	Perso cerbere("Cerbère", 30000, 30000, 6000, 90, 2000, 300, 3000, 0, 0, " ", " ", " ", " ", " ", " "); //14
	monster.push_back(cerbere);
}

void systemeCombat(int indexM)
{
	combatRemporte = 0;

	Perso* adversaire = &monster[indexM];
	cout << "\nOn commence le combat avec " << adversaire->nom << "\n\n";

	adversaire->pvActuel = adversaire->pvMax;

	Perso* premier;
	Perso* deuxieme;

	if (adversaire->initiative > joueur.initiative)
	{
		premier = &monster[indexM];
		deuxieme = &joueur;
	}
	else
	{
		premier = &joueur;
		deuxieme = &monster[indexM];
	}

	while (premier->pvActuel > 0 && deuxieme->pvActuel > 0)
	{
		if (premier->degats + premier->arme < deuxieme->defense)
		{
			cout << premier->nom << " inflige 0 points de dégats à " << deuxieme->nom << "\n";
		}
		if (premier->degats + premier->arme > deuxieme->defense)
		{
			cout << premier->nom << " inflige " << premier->degats + premier->arme - deuxieme->defense << " points de dégats à " << deuxieme->nom << "\n";
			deuxieme->pvActuel -= premier->degats + premier->arme - deuxieme->defense;
		}

		if (deuxieme->pvActuel < 0)
		{
			deuxieme->pvActuel = 0;
		}
		cout << deuxieme->nom << ": " << deuxieme->pvActuel << "/" << deuxieme->pvMax << "\n";

		Perso* rotation = premier;
		premier = deuxieme;
		deuxieme = rotation;

	}

	if (joueur.pvActuel > 0)
	{
		joueur.pvMax += adversaire->exp / 2;
		joueur.degats += adversaire->exp / 6;
		joueur.vitesse += adversaire->exp / 8;

		if (bagueAvarice == 1)
		{
			cout << "\nVICTOIRE! Vous avez remporté votre combat et avez trouvé " << adversaire->pOr << " pièces d'or.\n";
			joueur.pOr += adversaire->pOr;
			combatRemporte = 1;
		}
		if (bagueAvarice == 2)
		{
			cout << "\nVICTOIRE! Vous avez remporté votre combat et avez trouvé 2x" << adversaire->pOr << " pièces d'or grâce à la bague d'Avarice.\n";
			joueur.pOr += adversaire->pOr * 2;
			combatRemporte = 1;
		}
	}
	else
	{
		cout << "\nVous avez perdu votre combat. Il semblerait que les dieux vous ont abandonné...\n";
	}

	pvPersoActuel = joueur.pvActuel;
	pvPersoMax = joueur.pvMax;
	totalForce = joueur.degats;
	totalVitesse = joueur.vitesse;
	pbonus = joueur.pOr;

	cout << "\nVoulez-vous boire une potion pour récupérer tous vos PV?\n";
	cout << "1 - Oui\n" << "2 - Non\n";
	int boirePotion;
	cin >> boirePotion;

	if (boirePotion == 1)
	{
		if (potion == 0)
		{
			cout << "\nErreur. Vous n'avez aucune potion.\n";
		}
		else
		{
			cout << "\nVous utilisez l'item: " << inventairePotion[0] << "\n";
			pvPersoActuel = pvPersoMax;
			potion -= 1;
		}
	}

	joueur= Perso(nomPerso, pvPersoActuel, pvPersoMax, totalForce, equipBouclier * protecteur, 0, pbonus, initiative, totalVitesse, equipArme * braceletEnfer, nomArme, nomBouclier, itemKit, itemAvarice, itemEnfer, itemProtecteur);
}

void zoneCombat()
{
	tousLesMonster();
	joueur = Perso(nomPerso, pvPersoActuel, pvPersoMax, totalForce, equipBouclier * protecteur, 0, pbonus, initiative, totalVitesse, equipArme * braceletEnfer, nomArme, nomBouclier, itemKit, itemAvarice, itemEnfer, itemProtecteur);

	if (nomPerso == "")
	{
		cout << erreurCreationPerso;
	}
	if (joueur.pvActuel == 0)
	{
		cout << pvZero;
	}

	else
	{
		while (true)
		{
			cout << "\nChoisissez une option:\n";
			cout << "1- Afficher les stats du joueur\n";
			cout << "2- Commencer un combat\n";
			cout << "3- Quitter\n";

			int choix = 0;
			cin >> choix;

			int indexMonstre = -1;
			switch (choix)
			{
			case 1:
				cout << "\nNom: " << joueur.nom << "\nPoints de vie (PV): " << joueur.pvActuel << "/" << joueur.pvMax << "\nForce: " << joueur.degats << "\nVitesse: " << joueur.vitesse;
				cout << "\nArme: " << joueur.nameArme << "(" << joueur.arme << ")" << "\nDéfense: " << joueur.nameBouclier << "(" << joueur.defense << ")";
				cout << "\nPièces d'or: " << joueur.pOr << "\nItems:" << joueur.capeKit << joueur.bagueAvarice << joueur.braceletEnfer << joueur.protecteur << "\nPotion: " << potion << "\n";
				break;
			case 2:
				cout << "\nQuel ennemi vous intéresse? Écrivez le numéro correspondant:\n";

				for (size_t i = 0; i < 10; i++)
				{
					cout << i << ": " << monster[i].nom << "\n";
				}
				cin >> indexMonstre;

				if (indexMonstre >= 0)
				{
					if (joueur.pvActuel == 0)
					{
						cout << "\nVos PV sont à 0, vous ne pouvez pas combattre.\n";
					}

					else
					{
						if (indexMonstre == 0)
						{
							indexM = 0;
						}
						if (indexMonstre == 1)
						{
							indexM = 1;
						}
						if (indexMonstre == 2)
						{
							indexM = 2;
						}
						if (indexMonstre == 3)
						{
							indexM = 3;
						}
						if (indexMonstre == 4)
						{
							indexM = 4;
						}
						if (indexMonstre == 5)
						{
							indexM = 5;
						}
						if (indexMonstre == 6)
						{
							indexM = 6;
						}
						if (indexMonstre == 7)
						{
							indexM = 7;
						}
						if (indexMonstre == 8)
						{
							indexM = 8;
						}
						if (indexMonstre == 9)
						{
							indexM = 9;
						}
						
						systemeCombat(indexM);
					}
				}

				break;
			case 3:
				return;
			}

			
		}
	}
}

void village()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	string achatEffectue = "\nAchat effectué avec succès. Votre personnage est maintenant équipé avec votre achat.\n";
	string erreurEquip = "\nErreur. Vous ne pouvez pas acheter un équipement plus faible ou égal que celui que vous avez présentement en votre possession.\n";
	string pasConfianceQuetes = "\nMitsuko n'a pas assez confiance en vos capacités pour vous donner une autre quête. Revenez la voir quand vous serez plus expérimenté.\n";
	string pasConfianceLegende = "\nMitsuko vous évalue du regard et ne semble pas impressionnée par ce qu'elle voit. Revenez la voir quand vous aurez fait vos preuves.\n";
	string legendeEvaluation = "\nL'aubergiste vous juge du regard. Après un moment de silence, elle finit par continuer son récit.";
	string PasDeQuete = "Pour votre niveau, il n'y a pas d'autre quête. Revenez quand vous aurez plus d'expérience.\n";

	if (nomPerso == "")
	{
		cout << erreurCreationPerso;
	}

	else
	{
		while (true)
		{
			cout << "\nVotre chemin vous mène jusqu'à un village paisible qui repose au pied d'une montagne. Que voulez-vous faire?\n";
			cout << "1 - Faire un achat à la boutique ''Madness''\n" << "2 - Aller vous restaurer à la taverne du village, ''Saphir''\n";
			cout << "3 - Vous rendre à l'auberge ''Chez Mitsuko''\n" << "4 - Quitter le village\n";
			int choixVillage;
			cin >> choixVillage;

			switch (choixVillage)
			{
			case 1: //Boutique
				while (choixVillage != 5)
				{
					cout << "\nBienvenue au magasin ''Madness''! Par quoi êtes-vous intéressé?\n" << "1 - Acheter une arme\n";
					cout << "2 - Acquérir un bouclier\n" << "3 - Acheter une potion\n" << "4 - Autre\n" << "5 - Quitter la boutique\n";
					cin >> choixVillage;

					switch (choixVillage)
					{
					case 1: //Section armes
						while (choixVillage != 7)
						{
							cout << "\nNous avons plusieurs armes dans notre inventaire (augmente les dégats infligés lors d'un combat). Que voulez-vous acheter:\n";
							cout << "1 - Dague (+20): 30 pièces d'or\n" << "2 - Arc (+20): 30 pièces d'or\n" << "3 - Épée simple (+100): 200 pièces d'or\n";
							cout << "4 - Hache simple (+100): 200 pièces d'or\n" << "5 - Épée des rois (+300): 600 pièces d'or\n";
							cout << "6 - Lance des dieux (+600): 1200 pièces d'or\n" << "7 - Ne rien acheter dans la section des armes\n";
							cin >> choixVillage;

							switch (choixVillage)
							{
							case 1:
								if (pbonus < 30)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 30 && equipArme >= 20)
								{
									cout << erreurEquip;
								}

								if (pbonus >= 30 && equipArme < 20)
								{
									cout << achatEffectue;
									pbonus -= 30;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipArme = 20;
									nomArme = "-Dague-";
								}
								break;
							case 2:
								if (pbonus < 30)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 30 && equipArme >= 20)
								{
									cout << erreurEquip;
								}
								if (pbonus >= 30 && equipArme < 20)
								{
									cout << achatEffectue;
									pbonus -= 30;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipArme = 20;
									nomArme = "-Arc-";
								}
								break;
							case 3:
								if (pbonus < 200)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 200 && equipArme >= 100)
								{
									cout << erreurEquip;
								}
								if (pbonus >= 200 && equipArme < 100)
								{
									cout << achatEffectue;
									pbonus -= 200;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipArme = 100;
									nomArme = "-ÉpéeSimple-";
								}
								break;
							case 4:
								if (pbonus < 200)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 200 && equipArme >= 100)
								{
									cout << erreurEquip;
								}
								if (pbonus >= 200 && equipArme < 100)
								{
									cout << achatEffectue;
									pbonus -= 200;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipArme = 100;
									nomArme = "-HacheSimple-";
								}
								break;
							case 5:
								if (pbonus < 600)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 600 && equipArme >= 300)
								{
									cout << erreurEquip;
								}
								if (pbonus >= 600 && equipArme < 300)
								{
									cout << achatEffectue;
									pbonus -= 600;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipArme = 300;
									nomArme = "-ÉpéeDesRois-";
								}
								break;
							case 6:
								if (pbonus < 1200)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 1200 && equipArme >= 600)
								{
									cout << erreurEquip;
								}
								if (pbonus >= 1200 && equipArme < 600)
								{
									cout << achatEffectue;
									pbonus -= 1200;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipArme = 600;
									nomArme = "-LanceDesDieux-";
								}
								break;
							}
						}
						break;
					case 2: //Section boucliers
						while (choixVillage != 6)
						{
							cout << "\nNous avons plusieurs boucliers qu'il est possible d'acheter (réduit les dégats reçus lors d'un combat):\n";
							cout << "1 - Petit bouclier (-10): 30 pièce d'or\n" << "2 - Bouclier moyen (-30): 90 pièces d'or\n" << "3 - Grand bouclier (-100): 300 pièces d'or\n";
							cout << "4 - Bouclier des rois (-200): 600 pièces d'or\n" << "5 - Bouclier des dieux (-600): 1200 pièces d'or\n" << "6 - Quitter la section des boucliers\n";
							cin >> choixVillage;

							switch (choixVillage)
							{
							case 1:
								if (pbonus < 30)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 30 && equipBouclier >= 10)
								{
									cout << erreurEquip;
								}

								if (pbonus >= 30 && equipBouclier < 10)
								{
									cout << achatEffectue;
									pbonus -= 30;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipBouclier = 10;
									nomBouclier = "-PetitBouclier-";
								}
								break;
							case 2:
								if (pbonus < 90)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 90 && equipBouclier >= 30)
								{
									cout << erreurEquip;
								}

								if (pbonus >= 90 && equipBouclier < 30)
								{
									cout << achatEffectue;
									pbonus -= 90;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipBouclier = 30;
									nomBouclier = "-BouclierMoyen-";
								}
								break;
							case 3:
								if (pbonus < 300)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 300 && equipBouclier >= 100)
								{
									cout << erreurEquip;
								}

								if (pbonus >= 300 && equipBouclier < 100)
								{
									cout << achatEffectue;
									pbonus -= 300;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipBouclier = 100;
									nomBouclier = "-GrandBouclier-";
								}
								break;
							case 4:
								if (pbonus < 600)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 600 && equipBouclier >= 200)
								{
									cout << erreurEquip;
								}

								if (pbonus >= 600 && equipBouclier < 200)
								{
									cout << achatEffectue;
									pbonus -= 600;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipBouclier = 200;
									nomBouclier = "-BouclierDesRois-";
								}
								break;
							case 5:
								if (pbonus < 1200)
								{
									cout << erreurArgentManquant;
								}
								if (pbonus >= 1200 && equipBouclier >= 600)
								{
									cout << erreurEquip;
								}

								if (pbonus >= 1200 && equipBouclier < 600)
								{
									cout << achatEffectue;
									pbonus -= 1200;
									cout << "Il vous reste: " << pbonus << " pièces d'or\n";
									equipBouclier = 600;
									nomBouclier = "-BouclierDesDieux-";
								}
								break;
							}
						}
						break;
					case 3:
						cout << "\nCombien voulez-vous en acheter en sachant qu'une potion coûte 100 pièces d'or? Si vous changez d'avis et ne voulez pas acheter de potion, inscrivez le chiffre suivant: 0\n";
						cin >> nombrePotion;
						coutPotion = nombrePotion * 100;

						if (pbonus < coutPotion)
						{
							erreurArgentManquant;
						}
						if (nombrePotion <= 0)
						{
							cout << "\nAucun achat d'effectué.\n";
						}
						else
						{
							pbonus -= coutPotion;
							potion += nombrePotion;
							cout << "\nAchat effectué! Vous avez maintenant: " << potion << " potions\n";
							cout << "Il vous reste: " << pbonus << " pièces d'or\n";
						}
						break;
					case 4: //Section autre
						if (pvPersoMax >= 1500)
						{
							cout << "\nLe propriétaire du magasin, un Dwarf, s'approche et vous demande vous êtes à la recherche de quoi:\n";
							string recherche;
							cin >> recherche;
							if (recherche == "tofu" || recherche == "Tofu")
							{
								if (capeKitsune == 0)
								{
									cout << "\nIl vous regarde surpris.\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
									cout << "**Ce n'est pas tous les jours que quelqu'un arrive à s'attirer les faveurs de Mitsuko.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "\nVous recevez la cape Kitsunetsuki! Pour plus d'informations sur l'accessoire reçu, allez voir Mitsuko à l'auberge.\n";
									capeKitsune = 333;
									itemKit = "-CapeKinetsuki-";
									pvPersoMax += capeKitsune;
								}
								else
								{
									cout << "\nVous avez déjà reçu la cape Kitsunetsuki.\n";
								}
							}
							else
							{
								cout << "\nIl vous informe qu'il n'a pas l'item recherché.\n";
							}
						}
						else
						{
							cout << "\nLe propriétaire du magasin, un Dwarf, vous informe qu'il n'a présentement rien d'autre.\n";
						}
					}
				}
				break;
			case 2: //Taverne
				while (choixVillage != 7)
				{
					cout << "\nBienvenue à la taverne ''Saphir''! Que voulez-vous manger (restaure immédiatement une partie de vos points de vie) ou faire?\n";
					cout << "1 - Café (+50 PV): 3 pièces d'or\n" << "2 - Snack (+150 PV): 10 pièces d'or\n" << "3 - Repas simple (+300 PV): 20 pièces d'or\n";
					cout << "4 - Repas gourmet (+600 PV): 35 pièces d'or\n" << "5 - Repas digne d'un roi (+2/3 des PV max): 55 pièces d'or\n";
					cout << "6 - Parler avec le propriétaire\n" << "7 - Quitter la taverne\n";
					cin >> choixVillage;

					switch (choixVillage)
					{
					case 1:
						if (pbonus < 3)
						{
							cout << erreurArgentManquant;
						}
						else
						{
							if (pvPersoActuel == pvPersoMax)
							{
								cout << erreurRestorationHP;
							}
							else
							{
								pbonus -= 3;
								pvPersoActuel += 50;
								if (pvPersoActuel > pvPersoMax)
								{
									pvPersoActuel = pvPersoMax;
								}
								cout << "\nRecevant votre café, son arôme vous parvient, réveillant tous vos sens. Dès la première gorgée, vous vous sentez revivre.\n";
								cout << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
								cout << "Il vous reste: " << pbonus << " pièces d'or\n";
							}
						}
						break;
					case 2:
						if (pbonus < 10)
						{
							cout << erreurArgentManquant;
						}
						else
						{
							if (pvPersoActuel == pvPersoMax)
							{
								cout << erreurRestorationHP;
							}
							else
							{
								pbonus -= 10;
								pvPersoActuel += 150;
								if (pvPersoActuel > pvPersoMax)
								{
									pvPersoActuel = pvPersoMax;
								}
								cout << "\nVotre serveur arrive avec votre assiette, un assortiment de légumes avec trempette et quelques croustilles.\n" << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
								cout << "Il vous reste: " << pbonus << " pièces d'or\n";
							}
						}
						break;
					case 3:
						if (pbonus < 20)
						{
							cout << erreurArgentManquant;
						}
						else
						{
							if (pvPersoActuel == pvPersoMax)
							{
								cout << erreurRestorationHP;
							}
							else
							{
								pbonus -= 20;
								pvPersoActuel += 300;
								if (pvPersoActuel > pvPersoMax)
								{
									pvPersoActuel = pvPersoMax;
								}
								cout << "\nVotre serveur dépose devant vous un bol qui contient un simple ragoût de boeuf avec pommes de terre et carottes. Un repas réconfortant.\n" << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
								cout << "Il vous reste: " << pbonus << " pièces d'or\n";
							}
						}
						break;
					case 4:
						if (pbonus < 35)
						{
							cout << erreurArgentManquant;
						}
						else
						{
							if (pvPersoActuel == pvPersoMax)
							{
								cout << erreurRestorationHP;
							}
							else
							{
								pbonus -= 35;
								pvPersoActuel += 600;
								if (pvPersoActuel > pvPersoMax)
								{
									pvPersoActuel = pvPersoMax;
								}
								cout << "\nVous n'attendez pas longtemps avant que votre steak vous soit servi. L'odeur vous met l'eau à la bouche!\n" << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
								cout << "Il vous reste: " << pbonus << " pièces d'or\n";
							}
						}
						break;
					case 5:
						if (pbonus < 55)
						{
							cout << erreurArgentManquant;
						}
						else
						{
							if (pvPersoActuel == pvPersoMax)
							{
								cout << erreurRestorationHP;
							}
							else
							{
								pbonus -= 55;
								pvPersoActuel += (pvPersoMax / 3) * 2;
								if (pvPersoActuel > pvPersoMax)
								{
									pvPersoActuel = pvPersoMax;
								}
								cout << "\nVous recevez plusieurs assiettes contenant viandes et fruits de mer, le tout accompagné de petits légumes. Abondance de nourriture!\n" << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
								cout << "Il vous reste: " << pbonus << " pièces d'or\n";
							}
						}
						break;
					case 6: //parler avec le proprio. Quête pour obtenir bague avarice
						if (pvPersoMax < 700)
						{
							cout << "\nLe propriétaire, un Tiefling, arrive et vous juge du regard.\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
							cout << "**Je n'ai rien à dire à quelqu'un d'aussi faible que toi.**\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						}

						if (pvPersoMax > 700 && bagueAvarice == 1)
						{
							cout << "\nLe propriétaire, un Tiefling, arrive et vous demande ce que vous lui voulez. Vous lui demandez alors quel est le secret de son succès.\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
							cout << "**Mon secret? Haha! Tu ne manques pas de cran de me poser une telle question. Ma femme a toujours été à mes côtés à me supporter et c'est elle qui a créé tous les recettes pour le menu que nous proposons à Saphir. C'est pourquoi le secret de mon succès est ma femme: Nerka**\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						}

						if (bagueAvarice == 2)
						{
							cout << "\nLe propriétaire vous a déjà dit le secret de son succès. Il n'a rien de plus à vous dire.\n";
						}
					}
				}
				break;
			case 3: //Auberge
				while (choixVillage != 4)
				{
					cout << "\nBienvenue à l'auberge ''Chez Mitsuko''! Que voulez-vous faire?\n";
					cout << "1 - Prendre une chambre et faire une sieste (+50% de vos PV max): 40 pièces d'or\n";
					cout << "2 - Prendre une chambre et y passer la nuit (vous récupérer 100% de vos PV): 80 pièces d'or\n";
					cout << "3 - Parler avec l'aubergiste\n" << "4 - Quitter l'auberge\n";
					cin >> choixVillage;

					switch (choixVillage)
					{
					case 1: //Sieste, pv actuel + 50% pv max
						if (pvPersoActuel == pvPersoMax)
						{
							cout << erreurRestorationHP;
						}
						if (pbonus < 40)
						{
							cout << erreurArgentManquant;
						}

						if (pvPersoActuel != pvPersoMax && pbonus >= 40)
						{
							pvPersoActuel += pvPersoMax / 2;
							if (pvPersoActuel > pvPersoMax)
							{
								pvPersoActuel = pvPersoMax;
							}
							cout << "\nAprès une petite sieste, vous avez récupéré la moitié de vos PV max\n";
							cout << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
							pbonus -= 40;
							cout << "Il vous reste: " << pbonus << " pièces d'or\n";
						}
						break;
					case 2: //Louer une chambre, récupère tous ses PV
						if (pvPersoActuel == pvPersoMax)
						{
							cout << erreurRestorationHP;
						}
						if (pbonus < 80)
						{
							cout << erreurArgentManquant;
						}

						if (pvPersoActuel != pvPersoMax && pbonus >= 80)
						{
							pvPersoActuel = pvPersoMax;
							cout << "\nAprès une bonne nuit de sommeil, vous avez récupéré tous vos PV\n";
							cout << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
							pbonus -= 80;
							cout << "Il vous reste: " << pbonus << " pièces d'or\n";
						}
						break;
					case 3: //Parler avec Mitsuko
						while (choixVillage != 5)
						{
							cout << "\nAu comptoir d'accueil se trouve une Kitsune.\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
							cout << "**Bonjour, voyageur! Mon nom est Mitsuko et je suis la propriétaire de cette auberge. Comment puis-je t'aider?**\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							cout << "1 - Légende sur la création du village\n" << "2 - Recevoir une quête\n" << "3 - Tenter de séduire l'aubergiste\n";
							cout << "4 - Informations sur les accessoires\n" << "5 - Quitter la compagnie de Mitsuko\n";
							cin >> choixVillage;

							switch (choixVillage)
							{
							case 1: //Légende, ce qui donne éventuellement accès à un nouveau lieu au sommet de la montagne
								if (pvPersoMax >= 100)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "\n**Selon la légende, le dieu Tepnem a été banni des cieux après avoir commis un crime grave et réduit à parcourir la terre pour l'éternité. Comme si ce n’était pas assez, son châtiment était que chaque pas qu'il faisait lui donnait la sensation de se faire poignarder. Ne connaissant que misères et souffrances, il tenta plusieurs fois de mettre fin à ses jours, sans succès. Pour ne plus ";
									cout << "ressentir une telle douleur, il cessa de se déplacer. Se posant sur le haut d'une montagne, tout ce qu'il pouvait faire était d'attendre avec impatience la fin du monde. Tepnem a vu des civilisations naîtrent et mourirent. Il a vu l'Homme détruire la nature autour de lui pour son profit personnel et la nature qui a toujours fini par réclamer son territoire avec le temps.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								if (pvPersoMax >= 500)
								{
									cout << legendeEvaluation;
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "\n**Cela faisait déjà plusieurs années, voire des centaines, que Tepnem reposait au sommet du mont ''Vithral''. On pouvait difficilement faire la différence entre son corps et la végétation qui le recouvrait. S’étant placé contre un arbre au plus haut point de la montagne, il avait auparavant une vue imprenable sur la région, mais avec le temps qui passa, l’arbre se tordit pour accommoder son corps, ";
									cout << "comme s’il voulait le protéger du monde externe et ne faire qu’un avec la divinité. Cet arbre aurait dû finir par flétrir avec le passage du temps, mais il n’en fit rien. Au contraire, il continua de grandir et de cacher de plus en plus Tepnem en son sein, se nourrissant indirectement de son énergie. Ce dernier aurait très bien pu se déplacer, mais pour quoi faire? Qu’un arbre l’entoure de plus en plus, ";
									cout << "qu’il ne puisse pratiquement plus voir devant lui à cause des diverses racines qui le cachaient du monde extérieur, ne le perturbait aucunement. Sa vie n’avait aucune valeur à ses propres yeux, mais pour cet arbre, il avait une raison d’être, même si ce n’était que pour être une source de nourriture. Aussi étrange cela puisse-t-il paraitre, cela lui suffisait pour rester où il se trouvait.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								if (pvPersoMax >= 1000)
								{
									cout << legendeEvaluation;
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "\n**Cependant, cela changea lorsqu’une elf se nommant Orianna se rendit au mont ''Vithral'' à la recherche d’herbes médicinales. Elle savait qu’un arbre plus grand que tous les autres et qui semblaient exister depuis la nuit des temps se trouvait quelque part au sommet de la montagne, mais elle n’aurait jamais pu imaginer que la raison était qu’un être supérieur s’y renfermait. Elle récolta avec insouciance ce dont ";
									cout << "elle avait besoin tout en s’émerveillant devant la grande variété de faune et de flore se trouvant autour d’elle. Orionna n’aurait jamais pu se douter que son existence avait piqué l’intérêt de Tepnem qui n’avait pas vu de si proche une forme de vie douée de parole depuis bien longtemps. Il l’observa, le moindre de ses gestes le fascinait. Il se surpris à vouloir révéler sa présence et à lui parler. Ce ne fut qu’au ";
									cout << "moment où l’elf alla pour quitter les lieux qu’il se décida à agir. Se trouvant près du précipice quand le dieu fit sa présence connue, ce fut en se tournant rapidement vers la provenance de sa voix, qu’elle perdit l’équilibre. Elle aurait chuté vers une mort certaine si Tepnem ne s’était pas levé et n’avait pas usé de ses pouvoirs pour la retenir dans le vide, lui sauvant ainsi la vie.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								if (pvPersoMax >= 1500)
								{
									cout << legendeEvaluation;
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "\n**Pendant plusieurs années, Orionna rendit visite à Tepnem au sommet du mont. Apprenant à se connaitre, ils finirent par tomber amoureux l’un de l’autre. L’elf tenta de le convaincre de descendre de la montagne et de venir voir le village qui se trouvait à son pied, ne comprenant pas pourquoi il s’obstinait à rester et ne comprenant pas complètement à quel point chaque pas était une vraie torture pour lui. Elle finit par l’avoir à l’usure et lui fit découvrir ";
									cout << "le lieu où elle vivait, espérant qu’un jour il pourra lui aussi appeler ce petit village son chez soi. Les années passèrent relativement de manière paisible, mais avec le temps, la divinité retrouva son amertume envers sa vie et finit même par en vouloir à son être aimé de le faire autant souffrir en le faisant marcher tous bords tous côtés. Il aurait pu retourner au sommet de ''Vithral'' comme auparavant, mais il apprit qu’Orionna était enceinte et ne pouvait s’imaginer l’abandonner elle et sa progéniture.**\n";

									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "\nVous interrompez l’aubergiste, ne comprenant pas un élément de l’histoire.\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "**Il aurait pu lui expliquer qu’à chaque fois qu’il se déplaçait il avait l’impression qu’on le poignardait? Oui, tu as tout à fait raison. Et c’est ce qu’il fit éventuellement. S’il l’avait fait plus tôt, il se serait épargné bien des tourments. Peu de temps après lui avoir révélé le supplice qu’il vivait, l’elf l’emmena jusqu’à la forge du village où une surprise l’y attendait. Le forgeron, fier de sa création, avait insisté pour être présent lors du dévoilement. Devant les yeux de Tepnem se trouvait une chaise-roulante. ";
									cout << "N’ayant eu aucun contact avec les autres depuis des centaines d’années, il ne s’était pas rendu à quel point l’Homme avait évolué et n’avait cessé de créer de nouvelles choses pour améliorer son quotidien. Il fallu que l’artisan et Orionna lui explique ce qu’était une chaise-roulante. Comprenant qu’il allait pouvoir se déplacer facilement et sans douleurs, des larmes coulèrent librement sur son visage. Pour la première fois, depuis très longtemps, le dieu reprit espoir en la vie.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								if (pvPersoMax >= 2500)
								{
									cout << legendeEvaluation;
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "\n**C’était un jour comme les autres, la divinité s’occupait de son enfant de maintenant 5 ans, pendant que sa femme était sur le mont ''Vithral'' pour refaire le plein d’herbes médicinales. Le soleil se coucha et toujours aucune trace d’Orianna. Ce fait en soi n’était pas si préoccupant en sachant qu’elle y restait parfois deux/trois jours sur la montagne. Cependant, après la cinquième journée, il demanda à deux amis de l’elf de se mettre à sa recherche. ";
									cout << "Quand ils revinrent au village, ce fut en transportant le corps ensanglanté de sa bien-aimée, elle avait été poignardée. Comme si le châtiment qu’il vivait depuis son bannissement avait été une prémonition du sort que sa femme allait connaitre. Sous le coup de la peine et de la colère, Tepnem se leva de sa chaise-roulante et se rendit sur la montagne, dans une vaine tentative de trouver la personne qui avait osé s’en prendre à Orianna et la tuer. Il ne se ";
									cout << "retourna même pas, oubliant complètement sur le coup qu’il avait un enfant à s’occuper. Retournant au village après quelques jours, il ressentait toujours ce besoin viscéral de faire payer quelqu’un. Ne pouvant trouver le coupable, il se mit à accuser les villageois pour la mort de son être aimé. Si ce n’était pour son enfant qui se met à travers de son chemin, il aurait probablement tué tous les habitants du village. Et encore, cela n’aurait pas été suffisant ";
									cout << "pour atténuer cette rage qui le consumait. En revanche, arrêtant son père dans son élan meurtrier lui coûta la vie. Toute la rage que Tepnem ressentait se transforma de nouveau en désespoir. Il prit dans ses bras le corps de sa femme et celui de son enfant et retourna au sommet du mont ''Vithral''. Selon la légende, si tu t’y rends, quelque chose apparait quand tu prononces le nom de son enfant: Orblood.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								else
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << pasConfianceLegende;
								}
								break;
							case 2: //Quêtes pour recevoir différents items
								if (pvPersoMax < 1000)
								{
									cout << "\nMitsuko ne fait pas assez confiance en vos capacités pour vous donner une quête. Revenez la voir quand vous serez plus expérimenté.\n";
								}
								if (pvPersoMax >= 700 && bagueAvarice == 1)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "\nQuête #1:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Convaincre le propriétaire de la taverne de partager le secret de son succès.\n";
									cout << "Récompense: Bague de l'avarice\n";

									cout << "\nQuel est le secret de Golluk?:\n";
									string secretGolluk;
									cin >> secretGolluk;

									if (secretGolluk == "Nerka" || secretGolluk == "nerka")
									{
										cout << "\nVous avez obtenu la bague de l'avarice!\n";
										bagueAvarice = 2;
										itemAvarice = "-BagueAvarice-";
									}
									
								}
								if (pvPersoMax < 3000 && bagueAvarice == 2)
								{
									cout << PasDeQuete;
								}

								if (pvPersoMax >= 3000 && bagueAvarice == 2 && braceletEnfer != 2)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "\nQuête #2:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "À la recherche d'une personne pour vaincre le Cerbère qui se cache dans la mine se trouvant dans la forêt ''Rasaelros''.\n";
									cout << "Récompense: Le bracelet de l'Enfer\n";

									if (changeMapMine == 2)
									{
										cout << "\nVous avez obtenu le bracelet de l'Enfer!\n";
										braceletEnfer = 2;
										itemEnfer = "-BraceletEnfer-";
									}
									
								}
								if (braceletEnfer == 2)
								{
									cout << "\nVous avez complété toutes les quêtes disponibles.\n";
								}
								break;
							case 3: //Flirter avec Mitsuko, ce qui débloque éventuellement un item
								if (pvPersoMax >= 1500)
								{
									cout << "\nMitsuko vous lance un regard amusé.\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "**Tu n'abandonnes pas facilement, c'est une caractéristique que j'apprécie chez les autres. Tu me plais! Je vais donc te dire un petit secret. ";
									cout << "À la boutique ''Madness'', vas dans la section ''Autre'' et dit au propriétaire que tu es à la recheche de: tofu. Ah, ça me fait penser que je vais bientôt devoir en acheter...**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								else
								{
									cout << "\nMitsuko n'est pas impressionnée par vos tentatives de séduction.\n";
								}
								break;
							case 4: //Renseignements sur items reçu par l'entremise de Mitsuko

								cout << "\nL'aubergiste sort un catalogue.\n";

								if (capeKitsune == 333)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "\nCape Kitsunetsuki:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Kitsunetsuki signifie ''être possédé par un renard''. Cette cape blanche aux bordure rouge augmente de 333 vos points de vie maximum.\n";
								}

								if (bagueAvarice == 2)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "\nBague de l'avarice:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Le détenteur de cet accessoire va trouver 2x plus de pièces d'or.\n";
								}

								if (braceletEnfer == 2)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "\nBracelet de l'Enfer:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Son ancien propriétaire et créateur était un démon supérieur. Si vous avez une arme, celle-ci inflige 2x plus de dégats qu'auparavant.\n";
								}

								if (protecteur == 2)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "\nProtecteur:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Le dieu Tepnem vous a fait ce cadeau. Si vous avez un bouclier, celui-ci devient 2x plus performant.\n";
								}

								if (capeKitsune == 0 && bagueAvarice == 1 && braceletEnfer == 1 && protecteur == 1)
								{
									cout << "Cependant, vous n'avez aucun accessoire en votre possession.\n";
								}
								break;
							}
						}
						break;
					}
				}
				break;
			case 4:
				cout << "\nVous allez quitter le village.\n";
				return;
			}
		}
	}
}

struct Temple
{
	string nom;
	string descrip;
	Temple* gauche;
	Temple* milieu;
	Temple* droite;

	Temple(string _nom, string _descrip)
	{
		nom = _nom;
		descrip = _descrip;
	}
};

const int ligneY = 12;
const int colonneX = 12;

char mine1[ligneY][colonneX] =
{
	{'.','#','.','.','.','.','.','#','.','.','S','.'},
	{'.','#','.','#','#','#','.','#','.','#','.','.'},
	{'.','#','.','#','.','.','+','#','.','#','.','#'},
	{'.','#','.','#','.','.','.','#','.','#','.','#'},
	{'.','#','.','#','.','#','#','#','.','#','.','#'},
	{'.','#','.','#','.','#','.','.','.','#','.','#'},
	{'.','#','.','#','.','#','.','.','.','#','.','#'},
	{'.','#','.','#','.','#','.','#','#','#','.','#'},
	{'.','#','.','#','.','#','.','#','#','.','.','#'},
	{'.','#','.','#','.','#','.','.','#','.','#','#'},
	{'.','#','.','#','.','#','#','.','#','.','.','.'},
	{'R','.','.','#','.','.','.','.','#','.','@','.'},
};
char mine2[ligneY][colonneX] =
{
	{'.','.','.','.','#','.','.','.','.','.','#','H'},
	{'#','#','#','.','#','.','#','.','#','.','#','.'},
	{'.','.','.','.','#','.','#','.','#','.','#','.'},
	{'.','#','#','#','#','.','#','.','#','.','.','.'},
	{'.','#','.','.','.','O','#','.','#','.','#','#'},
	{'.','#','.','#','#','#','#','.','#','.','#','.'},
	{'.','.','.','#','.','.','.','.','#','.','#','.'},
	{'#','#','#','#','.','#','#','#','#','.','#','.'},
	{'.','.','.','.','.','#','.','.','.','.','#','.'},
	{'.','#','#','#','#','#','.','#','#','.','.','.'},
	{'.','#','.','.','.','#','.','#','#','#','#','#'},
	{'.','.','.','#','.','.','.','C','.','.','.','X'},
};

bool deplaceVers1(int x, int y)
{
	return x >= 0
		&& x < colonneX
		&& y >= 0
		&& y < ligneY
		&& mine1[y][x] != '#';
}
bool deplaceVers2(int x, int y)
{
	return x >= 0
		&& x < colonneX
		&& y >= 0
		&& y < ligneY
		&& mine2[y][x] != '#';
}

void systemeMine1()
{
	int emplacementX = 0;
	int emplacementY = 0;

	while (true)
	{
		tousLesMonster();
		system("cls");
		char tuile = mine1[emplacementY][emplacementX];
		bool rotationMap = false;
		int choixPotion = -1;

		switch (tuile)
		{
		case 'R':
			systemeCombat(10);
			if (combatRemporte == 0)
			{
				rotationMap = true;
				cout << quitterMine;
			}
			break;
		case 'S':
			systemeCombat(11);
			if (combatRemporte == 0)
			{
				rotationMap = true;
				cout << quitterMine;
			}
			break;
		case '+':
			while (choixPotion != 3)
			{
				cout << "\nUn marchand de potion se trouve devant vous. Que voulez-vous faire?:\n";
				cout << "1 - Acheter une potion\n" << "2 - Engager la conversation avec le marchand\n" << "3 - Poursuivre votre route\n";
				cin >> choixPotion;

				switch (choixPotion)
				{
				case 1:
					cout << "\nCombien voulez-vous en acheter en sachant qu'une potion coûte 70 pièces d'or? Si vous changez d'avis et ne voulez pas acheter de potion, inscrivez le chiffre suivant: 0\n";
					cin >> nombrePotion;
					coutPotion = nombrePotion * 70;

					if (nombrePotion <= 0)
					{
						cout << "\nAucun achat d'effectué.\n";
					}
					if (nombrePotion > 0 && pbonus > coutPotion)
					{
						pbonus -= coutPotion;
						potion += nombrePotion;
						cout << "\nAchat effectué! Vous avez maintenant: " << potion << " potions\n";
						cout << "Il vous reste: " << pbonus << " pièces d'or\n";

					}
					else
					{
						erreurArgentManquant;
					}
					break;
				case 2:
					cout << "\nPeu importe le sujet que vous abordez, il n'est pas intéressé à vous parler. Tout ce qui lui importe est de faire une vente.\n";
					break;
				}
			}
			break;
		case '@':
			changeMapMine = 1;
			rotationMap = true;
			break;
		}

		if (rotationMap == true)
		{
			return;
		}

		system("cls");
		for (int y = 0; y < ligneY; y++)
		{
			for (int x = 0; x < colonneX; x++)
			{
				if (emplacementX == x && emplacementY == y)
				{
					cout << "P";
				}
				else
				{
					cout << mine1[y][x];
				}
				cout << " ";
			}
			cout << "\n";
		}

		cout << "Deplacez vous avec WASD:\n";
		char choix;
		choix = tolower(_getch());

		switch (choix)
		{
		case 'w':
			if (deplaceVers1(emplacementX, emplacementY - 1))
			{
				emplacementY--;
			}
			break;
		case 'a':
			if (deplaceVers1(emplacementX - 1, emplacementY))
			{
				emplacementX--;
			}
			break;
		case 's':
			if (deplaceVers1(emplacementX, emplacementY + 1))
			{
				emplacementY++;
			}
			break;
		case 'd':
			if (deplaceVers1(emplacementX + 1, emplacementY))
			{
				emplacementX++;
			}
			break;
		}
	}
}

void systemeMine2()
{
	int emplX = 0;
	int emplY = 0;

	while (true)
	{
		tousLesMonster();
		system("cls");
		char tuile2 = mine2[emplY][emplX];
		bool rotationMap = false;

		switch (tuile2)
		{
		case 'H':
			systemeCombat(12);
			if (combatRemporte == 0)
			{
				rotationMap = true;
				cout << quitterMine;
			}
			break;
		case 'O':
			systemeCombat(13);
			if (combatRemporte == 0)
			{
				rotationMap = true;
				cout << quitterMine;
			}
			break;
		case 'C':
			systemeCombat(14);
			if (combatRemporte == 0)
			{
				rotationMap = true;
				cout << quitterMine;
			}
			break;
		case 'X':
			cout << "Vous vaincu Cerbère et avez atteint la sortie de la mine! Allez voir Mitsuko pour recevoir la récompense à liée à la quête.\n";
			changeMapMine = 2;
			rotationMap = true;
			break;
		}

		if (rotationMap)
		{
			return;
		}

		system("cls");
		for (int y = 0; y < ligneY; y++)
		{
			for (int x = 0; x < colonneX; x++)
			{
				if (emplX == x && emplY == y)
				{
					cout << "P";
				}
				else
				{
					cout << mine2[y][x];

				}
				cout << " ";
			}
			cout << "\n";
		}

		cout << "Deplacez vous avec WASD:\n";
		char choix;
		choix = tolower(_getch());

		switch (choix)
		{
		case 'w':
			if (deplaceVers2(emplX, emplY - 1))
			{
				emplY--;
			}
			break;
		case 'a':
			if (deplaceVers2(emplX - 1, emplY))
			{
				emplX--;
			}
			break;
		case 's':
			if (deplaceVers2(emplX, emplY + 1))
			{
				emplY++;
			}
			break;
		case 'd':
			if (deplaceVers2(emplX + 1, emplY))
			{
				emplX++;
			}
			break;
		}
	}
}

void montagne()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	joueur = Perso(nomPerso, pvPersoActuel, pvPersoMax, totalForce, equipBouclier * protecteur, 0, pbonus, initiative, totalVitesse, equipArme * braceletEnfer, nomArme, nomBouclier, itemKit, itemAvarice, itemEnfer, itemProtecteur);

	changeMapMine;
	string dejaVaincu = "\nVous avez déjà vaincu le monstre qui se cachait ici.\n";
	string orZero = "\nExaminant de nouveau, vous ne trouvez rien d'autre.\n";
	string retourMontagne = "\nVous PV sont à 0, vous allez retourner au pied de la montagne.\n";
	string impasse = "\nVous ne remarquez rien qui sort de l'ordinaire à part le fait que les trois couloirs devant vous semblent aller dans la même direction.";

	//Degats et or temple
	int degatBuissons = 300;
	int degatArbre = 100;
	orExt;
	orTemple;

	//Degats et or labyrinthe
	int degatImpasse1 = 200;
	int degatImpasse2 = 300;
	int degatImpasse3 = 400;
	int degatImpasse4 = 500;
	orLab1;
	orLab2;

	//Creation et initialisation labyrinthe temple
	Temple rouge("Couloir Rouge", "\nD'un rouge intense qui rappelle la couleur du sang, le couloir est étroit.");
	Temple jaune("Couloir Jaune", "\nD'un jaune pâle, vous remarquez d'étranges dessins sur toutes les parois du couloir.");
	Temple mauve("Couloir Mauve", "\nLa couleur vous donne mal à la tête, mais sinon il n'y a rien de remarquable le long de votre route.");
	Temple jade("Couloir Jade", "\nVous trouvez un vieux sac et une arme dont la lame est rouillée au sol. Les deux items n'ont aucune valeur, alors vous décidez de ne pas les prendre avec vous.");
	Temple marron("Couloir Marron", "\nLe chemin est sinueux et semble sans fin.");
	Temple noir("Couloir Noir", "\nDes mots dans une langue ancienne sont inscrit sur les murs. Vous essayez de les déchiffrer sans succès.");
	Temple dore("Salle Dorée", "\nAu millieu de la pièce se trouve un homme que vous évaluez dans la trentaine. À votre arrivée, il vous regarde et sourit, comme s'il vous attendait.");
	Temple impasseUn("Couloir Turquoise", impasse);
	Temple impasseDeux("Couloir Rose", impasse);
	Temple impasseTrois("Couloir Marine", impasse);
	Temple impasseQuatre("Couloir Argent", impasse);
	Temple impasseCinq("Couloir Vert", impasse);
	Temple impasseSix("Couloir Charcoal", impasse);
	Temple impasseSept("Couloir Bronze", impasse);
	Temple impasseHuit("Couloir Violet", impasse);
	Temple impasseNeuf("Couloir Bourgogne", impasse);
	Temple impasseDix("Couloir Bleu", impasse);
	Temple impasseOnze("Couloir Gris", impasse);
	Temple impasseDouze("Couloir Orange", impasse);

	//Connection des couloirs dans le labyrinthe
	rouge.gauche = &impasseUn;
	rouge.milieu = &impasseDeux;
	rouge.droite = &jaune;

	impasseUn.gauche = &rouge;
	impasseUn.milieu = &rouge;
	impasseUn.droite = &rouge;

	impasseDeux.gauche = &rouge;
	impasseDeux.milieu = &rouge;
	impasseDeux.droite = &rouge;

	jaune.gauche = &impasseTrois;
	jaune.milieu = &impasseQuatre;
	jaune.droite = &mauve;

	impasseTrois.gauche = &jaune;
	impasseTrois.milieu = &jaune;
	impasseTrois.droite = &jaune;

	impasseQuatre.gauche = &jaune;
	impasseQuatre.milieu = &jaune;
	impasseQuatre.droite = &jaune;

	mauve.gauche = &impasseCinq;
	mauve.milieu = &jade;
	mauve.droite = &impasseSix;

	impasseCinq.gauche = &mauve;
	impasseCinq.milieu = &mauve;
	impasseCinq.droite = &mauve;

	impasseSix.gauche = &mauve;
	impasseSix.milieu = &mauve;
	impasseSix.droite = &mauve;

	jade.gauche = &impasseSept;
	jade.milieu = &impasseHuit;
	jade.droite = &marron;

	impasseSept.gauche = &jade;
	impasseSept.milieu = &jade;
	impasseSept.droite = &jade;

	impasseHuit.gauche = &jade;
	impasseHuit.milieu = &jade;
	impasseHuit.droite = &jade;

	marron.gauche = &noir;
	marron.milieu = &impasseNeuf;
	marron.droite = &impasseDix;

	impasseNeuf.gauche = &marron;
	impasseNeuf.milieu = &marron;
	impasseNeuf.droite = &marron;

	impasseDix.gauche = &marron;
	impasseDix.milieu = &marron;
	impasseDix.droite = &marron;

	noir.gauche = &impasseOnze;
	noir.milieu = &dore;
	noir.droite = &impasseDouze;

	impasseOnze.gauche = &noir;
	impasseOnze.milieu = &noir;
	impasseOnze.droite = &noir;

	impasseDouze.gauche = &noir;
	impasseDouze.milieu = &noir;
	impasseDouze.droite = &noir;

	Temple* couloirActuel = &rouge;
	protecteur = 1;

	if (nomPerso == "")
	{
		cout << erreurCreationPerso;
	}
	if (joueur.pvActuel == 0)
	{
		cout << pvZero;
	}
	
	else
	{
		while (true)
		{
			int choixMontagne;

			cout << "\nArrivant au pied du mont ''Vithral'', vous avez un choix à faire. Vers où voulez-vous aller?:\n";
			cout << "1 - Forêt ''Rasaelros''\n" << "2 - Haut de la montagne\n" << "3 - Quitter la montagne\n";
			cin >> choixMontagne;

			switch (choixMontagne)
			{
			case 1: //foret
				while (choixMontagne != 3)
				{
					if (bagueAvarice != 2 || pvPersoMax < 3000)
					{
						cout << "\nAlors que vous parcourez la forêt, vous appercevez une mine. Plus vous vous en approchez, plus votre instinct vous dit de fuir.\n";
						choixMontagne = 3;
					}

					if (joueur.pvActuel == 0)
					{
						cout << pvZero;
						choixMontagne = 3;
					}

					if (bagueAvarice == 2 && pvPersoMax >= 3000 && joueur.pvActuel > 0)
					{
						cout << "\nVous vous dirigez vers la mine mentionnée dans la quête, mais plus vous vous en approchez, plus vous devenez tendu, comme si vous pouviez ressentir qu'un monstre s'y terre. Que voulez-vous faire?:\n";
						cout << "1 - Examiner les lieux\n" << "2 - Poursuivre votre quête\n" << "3 - Revenir sur vos pas\n";
						cin >> choixMontagne;

						switch (choixMontagne)
						{
						case 1:
							cout << "\nPrudent, vous jetez un coup d'oeil à l'intérieur, mais il fait trop sombre pour que vous puissiez voir quoique ce soit.\n";
							break;
						case 2:

							if (joueur.pvActuel == 0)
							{
								cout << "\nVos PV sont à 0, vous ne pouvez pas combattre.\n";
								choixMontagne = 3;
							}
							if (changeMapMine == 2)
							{
								cout << dejaVaincu;
								choixMontagne = 3;
							}

							if (changeMapMine != 2)
							{
								systemeMine1();
							}
							if (changeMapMine == 1)
							{
								systemeMine2();
							}
							break;
						}
					}
				}
				break;
			case 2: //sommet montagne
				if (pvPersoMax < 1000 && joueur.pvActuel > 0)
				{
					cout << "\nAprès une longue montée, vous arrivez enfin au plus haut point du mont. Vous avez une vue imprenable de la région et vous pouvez voir clairement le village ''Orionna''. Plus le temps passe, plus vous ressentez qu'une énergie émane des lieux. Cela vous intrigue.\n";
				}
				if (pvPersoMax >= 1000 && joueur.pvActuel > 0)
				{
					cout << "\nVous explorer le haut de la montagne, mais rien ne semble sortir de l'ordinaire. Toutefois, vous ressentez toujours cette mystérieuse énergie. Vous revenez plusieurs jours d'affilés sur place, sans jamais rien trouver de plus qui expliquerait la sensation que vous avez.\n";
				}

				if (joueur.pvActuel == 0)
				{
					cout << pvZero;
				}

				if (pvPersoMax >= 1500 && joueur.pvActuel > 0)
				{
					cout << "\nAlors que vous vous reposez au pied du plus grand arbre qui se trouve au sommet, vous demandant si vous devriez abandonner, l'arbre se met à briller d'une lueur dorée. En même temps que des mots apparaissent sur l'écorce, une douce voix retentit:\n";
					SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
					cout << "**Misères et souffrances étaient le quotidien de Tepnem jusqu'à ce qu'il rencontre Orionna. Il retomba dans ce cercle de douleurs dès qu'elle quitta cette terre et il a failli tout détruire autour de lui. Heureusement ou malheureusement, il tua la seule personne précieuse qui lui restait. Ce lieu à pour but de rappeler à ceux qui y parviennent qu'il ne faut jamais perdre espoir et de protéger vos êtres chers... Qui était cette personne que Tepnem a tuée?**\n";
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					string orblood;
					cin >> orblood;

					if (orblood == "Orblood" || orblood == "orblood")
					{
						cout << "\nAttention, votre défense est réduite à 0 lorsque le temple vous est visible. En quittant le sommet de la montage, vous retrouverez votre défense.\n";
						
						while (choixMontagne != 3 && joueur.pvActuel > 0)
						{
							cout << "\nDevant vous se dresse le temple ''Orblood'', un temple en l'honneur du dieu Tepnem et de son enfant. Que voulez-vous faire?:\n";
							cout << "1 - Examiner autour du monument\n" << "2 - Pénétrer dans le temple\n" << "3 - Retourner au pied de la montagne\n";
							cin >> choixMontagne;

							switch (choixMontagne)
							{
							case 1: //exterieur temple
								while (choixMontagne != 4 && joueur.pvActuel > 0)
								{
									cout << "\nLe temple se fond dans la végétation, comme s'ils ne faisaient qu'un. Étrangement, le passage du temps ne semble pas avoir affecté le monument, comme si sa construction avait fini la journée d'avant, ce qui est impossible. Quel action voulez-vous faire?\n";
									cout << "1 - Examiner les buissons aux alentours\n" << "2 - Agiter un arbre se trouvant proche de vous\n" << "3 - Étudier la façade du temple\n" << "4 - Revenir sur vos pas\n";
									cin >> choixMontagne;

									switch (choixMontagne)
									{
									case 1:
										cout << "\nDécidant d'investiguer les buissons autour du temple, vous découvrer ce qui semble être un chiot. Vous en approchant, vous vous arrêtez net en entendant un grognement en arrière de vous. Vous retournant lentement, vous faîtes face à un Direwolf. Ce dernier se jette sur vous pour protéger son petit!\n";
										cout << "\nDirewolf inflige " << degatBuissons << " points de dégats à " << joueur.nom << " avant de s'enfuir avec le louveteau.\n";
										
										joueur.pvActuel -= degatBuissons;

										if (joueur.pvActuel < 0)
										{
											joueur.pvActuel = 0;
										}
										cout << nomPerso << ": " << joueur.pvActuel << "/" << joueur.pvMax << "\n";

										pvPersoActuel = joueur.pvActuel;
										pvPersoMax = joueur.pvMax;
										if (joueur.pvActuel == 0)
										{
											cout << retourMontagne;
											choixMontagne = 4;
										}
										break;
									case 2:
										cout << "\nDécidant de secouer l'arbre proche de vous, un serpent vous tombe dessus. Avant que vous puissiez réagir, il plante ses crocs en vous!\n";
										cout << "\nSerpent inflige " << degatArbre << " points de dégats à " << joueur.nom << " avant de disparaitre aussi rapidement qu'il vous a attaqué.\n";

										joueur.pvActuel -= degatArbre;
										if (joueur.pvActuel < 0)
										{
											joueur.pvActuel = 0;
										}
										cout << nomPerso << ": " << joueur.pvActuel << "/" << joueur.pvMax << "\n";

										pvPersoActuel = joueur.pvActuel;
										pvPersoMax = joueur.pvMax;
										if (joueur.pvActuel == 0)
										{
											cout << retourMontagne;
											choixMontagne = 4;
										}
										break;
									case 3:
										if (orExt == 0)
										{
											cout << orZero;
										}
										else
										{
											cout << "\nExaminant la façade du monument, vous remarquer quelque chose qui brille dans l'une des voûtes au-dessus de l'entrée. Usant d'ingéniosité et de votre adresse, vous arrivez à obtenir ce qui se cachait là-haut.\n";
											if (bagueAvarice == 1)
											{
												cout << "\nVous avez trouvé: " << orExt << " pièces d'or\n";
											}
											else
											{
												cout << "\nVous avez trouvé: 2x" << orExt << " pièces d'or\n";
											}
											pbonus += orExt * bagueAvarice;
											orExt = 0;
										}
										break;
									}
								}
								break;
							case 2: //interieur temple
								while (choixMontagne != 4 && joueur.pvActuel > 0)
								{
									cout << "\nVous trouvant à l'intérieur du temple, vous explorer les lieux et découvrez un passage secret. Devant vous apparait un escalier descendant vers des bas fonds inconnus, ainsi qu'une plaque avec le message suivant gravé dessus:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "''Celui qui perd son chemin dans ce labyrinthe ne fera que souffrir jusqu'à sa mort à moins de recevoir la grâce de Tepnem.''\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Que décidez-vous de faire?:\n";
									cout << "1 - Continuer d'explorer le temple\n" << "2 - Examiner le passage secret\n" << "3 - Entrer dans le labyrinthe\n" << "4 - Sortir du monument\n";
									cin >> choixMontagne;

									switch (choixMontagne)
									{
									case 1:
										if (orTemple == 0)
										{
											cout << orZero;
										}
										else
										{
											if (bagueAvarice == 1)
											{
												cout << "\nFouillant chaque recoin, vous finissez par trouver: " << orTemple << " pièces d'or\n";
											}
											else
											{
												cout << "\nFouillant chaque recoin, vous finissez par trouver: 2x" << orTemple << " pièces d'or\n";
											}
											pbonus += orTemple * bagueAvarice;
											orTemple = 0;
										}
										break;
									case 2:
										cout << "\nVous ne voyez rien de plus qu'auparavant.\n";
										break;
									case 3:
										int choixTemple = -1;
										while (choixTemple != 0)
										{
											if (couloirActuel->nom != "Salle Dorée")
											{
												cout << "\nVous êtes dans le " << couloirActuel->nom << ". Celui-ci se divise en trois chemins. Choisissez une action:\n" << "1 - Inspecter le couloir actuel\n";
												cout << "2 - Aller vers le couloir de gauche\n" << "3 - Aller vers le couloir du milieu\n" << "4 - Aller vers le couloir de droite\n";
												cin >> choixTemple;

												switch (choixTemple)
												{
												case 1: //Description
													cout << couloirActuel->descrip << "\n";

													//section pieges
													if (couloirActuel->nom == "Couloir Jaune" || couloirActuel->nom == "Couloir Marine" || couloirActuel->nom == "Couloir Gris")
													{
														cout << "\nPoussant votre investigation, vous déclenchez un piège par mégarde! Il inflige " << degatImpasse1 << " points de dégats à " << nomPerso << "\n";

														joueur.pvActuel -= degatImpasse1;
														if (joueur.pvActuel < 0)
														{
															joueur.pvActuel = 0;
														}
														cout << nomPerso << ": " << joueur.pvActuel << "/" << joueur.pvMax << "\n";

														pvPersoActuel = joueur.pvActuel;
														pvPersoMax = joueur.pvMax;
														if (joueur.pvActuel == 0)
														{
															cout << retourMontagne;
															choixTemple = 0;
														}
													}
													if (couloirActuel->nom == "Couloir Charcoal" || couloirActuel->nom == "Couloir Bronze" || couloirActuel->nom == "Couloir Noir")
													{
														cout << "\nPoussant votre investigation, vous déclenchez un piège par mégarde! Il inflige " << degatImpasse2 << " points de dégats à " << nomPerso << "\n";

														joueur.pvActuel -= degatImpasse2;
														if (joueur.pvActuel < 0)
														{
															joueur.pvActuel = 0;
														}
														cout << nomPerso << ": " << joueur.pvActuel << "/" << joueur.pvMax << "\n";

														pvPersoActuel = joueur.pvActuel;
														pvPersoMax = joueur.pvMax;
														if (joueur.pvActuel == 0)
														{
															cout << retourMontagne;
															choixTemple = 0;
														}
													}
													if (couloirActuel->nom == "Couloir Argent" || couloirActuel->nom == "Couloir Jade" || couloirActuel->nom == "Couloir Violet")
													{
														cout << "\nPoussant votre investigation, vous déclenchez un piège par mégarde! Il inflige " << degatImpasse3 << " points de dégats à " << nomPerso << "\n";

														joueur.pvActuel -= degatImpasse3;
														if (joueur.pvActuel < 0)
														{
															joueur.pvActuel = 0;
														}
														cout << nomPerso << ": " << joueur.pvActuel << "/" << joueur.pvMax << "\n";

														pvPersoActuel = joueur.pvActuel;
														pvPersoMax = joueur.pvMax;
														if (joueur.pvActuel == 0)
														{
															cout << retourMontagne;
															choixTemple = 0;
														}
													}
													if (couloirActuel->nom == "Couloir Bourgogne" || couloirActuel->nom == "Couloir Orange")
													{
														cout << "\nPoussant votre investigation, vous déclenchez un piège par mégarde! Il inflige " << degatImpasse4 << " points de dégats à " << nomPerso << "\n";

														joueur.pvActuel -= degatImpasse4;
														if (joueur.pvActuel < 0)
														{
															joueur.pvActuel = 0;
														}
														cout << nomPerso << ": " << joueur.pvActuel << "/" << joueur.pvMax << "\n";

														pvPersoActuel = joueur.pvActuel;
														pvPersoMax = joueur.pvMax;
														if (joueur.pvActuel == 0)
														{
															cout << retourMontagne;
															choixTemple = 0;
														}
													}

													//section pieces d'or
													if (couloirActuel->nom == "Couloir Turquoise" || couloirActuel->nom == "Couloir Vert" || couloirActuel->nom == "Couloir Marron")
													{
														if (orLab1 == 0)
														{
															cout << orZero;
														}
														else
														{
															if (bagueAvarice == 1)
															{
																cout << "\nPoussant votre investigation, vous trouvez: " << orLab1 << " pièces d'or\n";
															}
															else
															{
																cout << "\nPoussant votre investigation, vous trouvez: 2x" << orLab1 << " pièces d'or\n";
															}
															pbonus += orLab1 * bagueAvarice;
															orLab1 = 0;
														}
													}
													if (couloirActuel->nom == "Couloir Rose" || couloirActuel->nom == "Couloir Mauve" || couloirActuel->nom == "Couloir Bleu")
													{
														if (orLab2 == 0)
														{
															cout << orZero;
														}
														else
														{
															if (bagueAvarice == 1)
															{
																cout << "\nPoussant votre investigation, vous trouvez: " << orLab2 << " pièces d'or\n";
															}
															else
															{
																cout << "\nPoussant votre investigation, vous trouvez: 2x" << orLab2 << " pièces d'or\n";
															}
															pbonus += orLab2 * bagueAvarice;
															orLab2 = 0;
														}
													}
													break;
												case 2: //Aller a gauche
													couloirActuel = couloirActuel->gauche;
													break;
												case 3: //Aller milieu
													couloirActuel = couloirActuel->milieu;
													break;
												case 4: //Aller a droite
													couloirActuel = couloirActuel->droite;
													break;
												}
											}

											else
											{
												cout << "\nAprès tous ces couloirs, vous arrivez finalement dans la " << couloirActuel->nom << ". Choisissez une action:\n";
												cout << "1 - Inspecter la salle\n" << "2 - Parler avec l'inconnu\n" << "0 - Quitter le labyrinthe\n";
												cin >> choixTemple;

												switch (choixTemple)
												{
												case 1:
													cout << couloirActuel->descrip << "\n";
													break;
												case 2:

													cout << "\n\nL'inconnu vous accueil avec un sourire.\n";
													SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
													cout << "**Je me nomme Tepnem, tu as peut-être déjà entendu parler de moi.**\n";
													SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
													cout << "\nIl vous observe un instant avant de reprendre, son regard s'assombrant pendant un bref instant.\n";
													SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
													cout << "**Ta détermination n'est pas à prendre à la légère. J'ai connu quelqu'un d'aussi têtu que toi dans le passé. Si je peux me permettre un conseil: n'oublie jamais de protéger ceux qui te sont chers, peu importe les moyens et les conséquences. Ceci devrait t'aider.**\n";
													SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

													if (itemProtecteur == "-")
													{
														cout << "\nVous recevez de la part de Tepnem un item: Protecteur!\n";
														protecteur = 2;
														itemProtecteur = "-Protecteur-";
													}
													else
													{
														cout << "\nVous avez déjà reçu l'item: Protecteur\n";
													}
													break;
												}
											}
										}
										break;
									}
								}
								break;
							}
						}
					}

					else
					{
						SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
						cout << "\n**Quand tu sauras ce que tu recherches, reviens me donner ta réponse.**\n";
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					}

				}
				break;
			case 3:
				cout << "\nVous allez quitter la montagne.\n";
				return;
			}
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Fr_CA");
	ListeRaces();

	while (true)
	{
		equipEtItems();

		cout << "\nÉcrivez le chiffre qui correspond à votre choix :\n" << "1 - Créer un nouveau personnage\n";
		cout << "2 - Créer un personnage par défaut\n" << "3 - Afficher les caractéristiques du personnage\n";
		cout << "4 - Sauvegarder la fiche du personnage\n" << "5 - Charger la fiche du personnage\n";
		cout << "6 - Entrer la zone de combat\n" << "7 - Visiter le village ''Orionna''\n" << "8 - Marcher vers la montagne ''Vithral''\n";
		cout << "9 - Quitter le programme\n";
		int chiffreChoisi;
		cin >> chiffreChoisi;

		switch (chiffreChoisi)
		{
		case 1:
			creerPerso();
			break;
		case 2:
			persoDefaut();
			break;
		case 3:
			caracteristiquesPerso();
			break;
		case 4:
			savePerso();
			break;
		case 5:
			chargeSave();
			break;
		case 6:
			zoneCombat();
			break;
		case 7:
			village();
			break;
		case 8:
			montagne();
			break;
		case 9:
			cout << "\nVous allez quitter le programme. Que les dieux aient pitié de votre âme...\n";
			return 0;
		}
	}
}
