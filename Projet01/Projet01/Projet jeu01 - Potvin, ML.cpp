#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <conio.h>
using namespace std;

//Code �crit par Marie-Lee Potvin
//Le code pour le syst�me de combat, les tableaux 2D et le syst�me de naviagation dans les salles avec pointeur est bas� sur ce que nous avons vu en classe/le code de F�lix Soumpholphakdy.
//Il a par la suite �t� modifi� selon les besoins du jeu.

bool onRecommence = false;
string combien = "Combien de points voulez-vous attribuer � votre ";
string erreurAttributionPoints = "\nErreur! Le nombre de points que vous voulez attribuer est invalide. Voulez-vous recommencer au d�but pour l'attribution des points? Veuillez r�pondre par ''oui'' ou par ''non''.\n";
string reponse = "non";
string erreurCreationPerso = "\nErreur. Vous devez cr�er ou charger la fiche d'un personnage.\n";
string erreurInvalide = "Entr�e invalide.\n";
string erreurRestorationHP = "\nErreur. PV est d�j� � 100%.\n";
string erreurArgentManquant = "\nErreur. Vous n'avez pas assez de pi�ces d'or pour un tel achat.\n";
string quitterMine = "\nAyant perdu votre combat, vous aller quitter la mine.\n";
string pvZero = "\nVos PV sont � z�ro. Pour les r�cup�rer, vous devez vous rendre � la taverne ou � l'auberge.\n";

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
	if (nomArme == "-�p�eSimple-")
	{
		equipArme = 100;
	}
	if (nomArme == "-HacheSimple-")
	{
		equipArme = 100;
	}
	if (nomArme == "-�p�eDesRois-")
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
	"Potion Supr�me"
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

	cout << "\nNous allons cr�er votre personnage!\n";

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
				cout << "\nVotre vitalit�, force et vitesse ont 2 points d�s le commencement et vous devez r�partir 5 points entre ces trois caract�ristiques. Si vous n'attribuez pas tous les points disponibles, ceux-ci seront convertis en pi�ces d'or.\n";

				pbonus = 5;
				totalVital = 2;
				totalForce = 2;
				totalVitesse = 2;
				pAttribue = 0;
				bool onRecommence = false;

				while (true)
				{
					cout << "\n" << combien << "vitalit� (2)? Il vous reste (" << pbonus << ") points:\n";
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

				cout << "\n" << "Les points restants (" << pbonus << ") vont �tre convertis en pi�ces d'or.\n";


				//boucle pour les races
				while (true)
				{
					cout << "\nChoisisez une race dans celles suivantes en inscrivant le chiffre appropri�: \n";

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

		cout << "\nVous avez fini la cr�ation de votre personnage. �tes-vous satisfait? Veuillez r�pondre par ''oui'' ou ''non''.\n";
		string recommencer;
		cin >> recommencer;

		if (recommencer == "non" || recommencer == "Non" || recommencer == "NON")
		{
			cout << "\nPuisque vous avez r�pondu par ''non'', vous allez recommencer la cr�ation de votre personnage.\n";
			continue;
		}

		else
		{
			cout << "\nF�licitations, vous avez cr�� un personnage! N'oubliez pas de sauvegarder votre fiche.\n";
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

	cout << "\nLa cr�ation de votre personnage par d�faut est termin�e! N'oubliez pas d'enregistrer la fiche.\n";
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
		cout << "\nNom: " << nomPerso << "\nVitalit�: " << totalVital << "\nForce: " << totalForce << "\nVitesse: " << totalVitesse << "\nPi�ces d'or: " << pbonus;
		cout << "\nPoints de vie (PV): " << pvPersoActuel << "/" << pvPersoMax << "\nRace: " << racePerso << "\n�quipement: " << nomArme << nomBouclier;
		cout << "\nItems: " << itemKit << itemAvarice << itemEnfer << itemProtecteur << "\nPotion: " << potion << "\n";

		cout << "\nPensez � sauvegarder votre fiche r�guli�rement pour ne pas perdre vos progr�s.\n";
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
		cout << "\nLa fiche du personnage " << nomPerso << " a �t� enregistr�e avec succ�s.\n";

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
		cout << "\nLe chargement de votre personnage est compl�t�.\n";

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
	//chiffres � c�t� en commentaire pour aider lors du code dans zone de combat et mine1/2

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
	Perso demon("D�mon sup�rieur", 30000, 30000, 6000, 90, 2000, 300, 3000, 0, 0, " ", " ", " ", " ", " ", " "); //6
	monster.push_back(demon);
	Perso celeste("�tre c�leste", 90000, 90000, 18000, 270, 6000, 500, 9000, 0, 0, " ", " ", " ", " ", " ", " "); //7
	monster.push_back(celeste);
	Perso dragon("Dragon ancien", 300000, 300000, 54000, 700, 18000, 700, 21000, 0, 0, " ", " ", " ", " ", " ", " "); //8
	monster.push_back(dragon);
	Perso dieu("Dieu d�chu", 1000000, 1000000, 170000, 2100, 55000, 2100, 63000, 0, 0, " ", " ", " ", " ", " ", " "); //9
	monster.push_back(dieu);

	//Quete #2, mine
	Perso rat("Rat g�ant", 1500, 1500, 300, 20, 300, 40, 200, 0, 0, " ", " ", " ", " ", " ", " "); //10
	monster.push_back(rat);
	Perso chauvesouris("Chauve-souris g�ante", 1500, 1500, 300, 20, 300, 40, 200, 0, 0, " ", " ", " ", " ", " ", " "); //11
	monster.push_back(chauvesouris);
	Perso hobgoblin("Hobgoblin", 4000, 4000, 800, 40, 600, 100, 800, 0, 0, " ", " ", " ", " ", " ", " "); //12
	monster.push_back(hobgoblin);
	Perso orc("Orc", 4000, 4000, 800, 40, 600, 100, 800, 0, 0, " ", " ", " ", " ", " ", " "); //13
	monster.push_back(orc);
	Perso cerbere("Cerb�re", 30000, 30000, 6000, 90, 2000, 300, 3000, 0, 0, " ", " ", " ", " ", " ", " "); //14
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
			cout << premier->nom << " inflige 0 points de d�gats � " << deuxieme->nom << "\n";
		}
		if (premier->degats + premier->arme > deuxieme->defense)
		{
			cout << premier->nom << " inflige " << premier->degats + premier->arme - deuxieme->defense << " points de d�gats � " << deuxieme->nom << "\n";
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
			cout << "\nVICTOIRE! Vous avez remport� votre combat et avez trouv� " << adversaire->pOr << " pi�ces d'or.\n";
			joueur.pOr += adversaire->pOr;
			combatRemporte = 1;
		}
		if (bagueAvarice == 2)
		{
			cout << "\nVICTOIRE! Vous avez remport� votre combat et avez trouv� 2x" << adversaire->pOr << " pi�ces d'or gr�ce � la bague d'Avarice.\n";
			joueur.pOr += adversaire->pOr * 2;
			combatRemporte = 1;
		}
	}
	else
	{
		cout << "\nVous avez perdu votre combat. Il semblerait que les dieux vous ont abandonn�...\n";
	}

	pvPersoActuel = joueur.pvActuel;
	pvPersoMax = joueur.pvMax;
	totalForce = joueur.degats;
	totalVitesse = joueur.vitesse;
	pbonus = joueur.pOr;

	cout << "\nVoulez-vous boire une potion pour r�cup�rer tous vos PV?\n";
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
				cout << "\nArme: " << joueur.nameArme << "(" << joueur.arme << ")" << "\nD�fense: " << joueur.nameBouclier << "(" << joueur.defense << ")";
				cout << "\nPi�ces d'or: " << joueur.pOr << "\nItems:" << joueur.capeKit << joueur.bagueAvarice << joueur.braceletEnfer << joueur.protecteur << "\nPotion: " << potion << "\n";
				break;
			case 2:
				cout << "\nQuel ennemi vous int�resse? �crivez le num�ro correspondant:\n";

				for (size_t i = 0; i < 10; i++)
				{
					cout << i << ": " << monster[i].nom << "\n";
				}
				cin >> indexMonstre;

				if (indexMonstre >= 0)
				{
					if (joueur.pvActuel == 0)
					{
						cout << "\nVos PV sont � 0, vous ne pouvez pas combattre.\n";
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

	string achatEffectue = "\nAchat effectu� avec succ�s. Votre personnage est maintenant �quip� avec votre achat.\n";
	string erreurEquip = "\nErreur. Vous ne pouvez pas acheter un �quipement plus faible ou �gal que celui que vous avez pr�sentement en votre possession.\n";
	string pasConfianceQuetes = "\nMitsuko n'a pas assez confiance en vos capacit�s pour vous donner une autre qu�te. Revenez la voir quand vous serez plus exp�riment�.\n";
	string pasConfianceLegende = "\nMitsuko vous �value du regard et ne semble pas impressionn�e par ce qu'elle voit. Revenez la voir quand vous aurez fait vos preuves.\n";
	string legendeEvaluation = "\nL'aubergiste vous juge du regard. Apr�s un moment de silence, elle finit par continuer son r�cit.";
	string PasDeQuete = "Pour votre niveau, il n'y a pas d'autre qu�te. Revenez quand vous aurez plus d'exp�rience.\n";

	if (nomPerso == "")
	{
		cout << erreurCreationPerso;
	}

	else
	{
		while (true)
		{
			cout << "\nVotre chemin vous m�ne jusqu'� un village paisible qui repose au pied d'une montagne. Que voulez-vous faire?\n";
			cout << "1 - Faire un achat � la boutique ''Madness''\n" << "2 - Aller vous restaurer � la taverne du village, ''Saphir''\n";
			cout << "3 - Vous rendre � l'auberge ''Chez Mitsuko''\n" << "4 - Quitter le village\n";
			int choixVillage;
			cin >> choixVillage;

			switch (choixVillage)
			{
			case 1: //Boutique
				while (choixVillage != 5)
				{
					cout << "\nBienvenue au magasin ''Madness''! Par quoi �tes-vous int�ress�?\n" << "1 - Acheter une arme\n";
					cout << "2 - Acqu�rir un bouclier\n" << "3 - Acheter une potion\n" << "4 - Autre\n" << "5 - Quitter la boutique\n";
					cin >> choixVillage;

					switch (choixVillage)
					{
					case 1: //Section armes
						while (choixVillage != 7)
						{
							cout << "\nNous avons plusieurs armes dans notre inventaire (augmente les d�gats inflig�s lors d'un combat). Que voulez-vous acheter:\n";
							cout << "1 - Dague (+20): 30 pi�ces d'or\n" << "2 - Arc (+20): 30 pi�ces d'or\n" << "3 - �p�e simple (+100): 200 pi�ces d'or\n";
							cout << "4 - Hache simple (+100): 200 pi�ces d'or\n" << "5 - �p�e des rois (+300): 600 pi�ces d'or\n";
							cout << "6 - Lance des dieux (+600): 1200 pi�ces d'or\n" << "7 - Ne rien acheter dans la section des armes\n";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
									equipArme = 100;
									nomArme = "-�p�eSimple-";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
									equipArme = 300;
									nomArme = "-�p�eDesRois-";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
							cout << "\nNous avons plusieurs boucliers qu'il est possible d'acheter (r�duit les d�gats re�us lors d'un combat):\n";
							cout << "1 - Petit bouclier (-10): 30 pi�ce d'or\n" << "2 - Bouclier moyen (-30): 90 pi�ces d'or\n" << "3 - Grand bouclier (-100): 300 pi�ces d'or\n";
							cout << "4 - Bouclier des rois (-200): 600 pi�ces d'or\n" << "5 - Bouclier des dieux (-600): 1200 pi�ces d'or\n" << "6 - Quitter la section des boucliers\n";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
									cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
									equipBouclier = 600;
									nomBouclier = "-BouclierDesDieux-";
								}
								break;
							}
						}
						break;
					case 3:
						cout << "\nCombien voulez-vous en acheter en sachant qu'une potion co�te 100 pi�ces d'or? Si vous changez d'avis et ne voulez pas acheter de potion, inscrivez le chiffre suivant: 0\n";
						cin >> nombrePotion;
						coutPotion = nombrePotion * 100;

						if (pbonus < coutPotion)
						{
							erreurArgentManquant;
						}
						if (nombrePotion <= 0)
						{
							cout << "\nAucun achat d'effectu�.\n";
						}
						else
						{
							pbonus -= coutPotion;
							potion += nombrePotion;
							cout << "\nAchat effectu�! Vous avez maintenant: " << potion << " potions\n";
							cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
						}
						break;
					case 4: //Section autre
						if (pvPersoMax >= 1500)
						{
							cout << "\nLe propri�taire du magasin, un Dwarf, s'approche et vous demande vous �tes � la recherche de quoi:\n";
							string recherche;
							cin >> recherche;
							if (recherche == "tofu" || recherche == "Tofu")
							{
								if (capeKitsune == 0)
								{
									cout << "\nIl vous regarde surpris.\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
									cout << "**Ce n'est pas tous les jours que quelqu'un arrive � s'attirer les faveurs de Mitsuko.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "\nVous recevez la cape Kitsunetsuki! Pour plus d'informations sur l'accessoire re�u, allez voir Mitsuko � l'auberge.\n";
									capeKitsune = 333;
									itemKit = "-CapeKinetsuki-";
									pvPersoMax += capeKitsune;
								}
								else
								{
									cout << "\nVous avez d�j� re�u la cape Kitsunetsuki.\n";
								}
							}
							else
							{
								cout << "\nIl vous informe qu'il n'a pas l'item recherch�.\n";
							}
						}
						else
						{
							cout << "\nLe propri�taire du magasin, un Dwarf, vous informe qu'il n'a pr�sentement rien d'autre.\n";
						}
					}
				}
				break;
			case 2: //Taverne
				while (choixVillage != 7)
				{
					cout << "\nBienvenue � la taverne ''Saphir''! Que voulez-vous manger (restaure imm�diatement une partie de vos points de vie) ou faire?\n";
					cout << "1 - Caf� (+50 PV): 3 pi�ces d'or\n" << "2 - Snack (+150 PV): 10 pi�ces d'or\n" << "3 - Repas simple (+300 PV): 20 pi�ces d'or\n";
					cout << "4 - Repas gourmet (+600 PV): 35 pi�ces d'or\n" << "5 - Repas digne d'un roi (+2/3 des PV max): 55 pi�ces d'or\n";
					cout << "6 - Parler avec le propri�taire\n" << "7 - Quitter la taverne\n";
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
								cout << "\nRecevant votre caf�, son ar�me vous parvient, r�veillant tous vos sens. D�s la premi�re gorg�e, vous vous sentez revivre.\n";
								cout << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
								cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
								cout << "\nVotre serveur arrive avec votre assiette, un assortiment de l�gumes avec trempette et quelques croustilles.\n" << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
								cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
								cout << "\nVotre serveur d�pose devant vous un bol qui contient un simple rago�t de boeuf avec pommes de terre et carottes. Un repas r�confortant.\n" << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
								cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
								cout << "\nVous n'attendez pas longtemps avant que votre steak vous soit servi. L'odeur vous met l'eau � la bouche!\n" << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
								cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
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
								cout << "\nVous recevez plusieurs assiettes contenant viandes et fruits de mer, le tout accompagn� de petits l�gumes. Abondance de nourriture!\n" << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
								cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
							}
						}
						break;
					case 6: //parler avec le proprio. Qu�te pour obtenir bague avarice
						if (pvPersoMax < 700)
						{
							cout << "\nLe propri�taire, un Tiefling, arrive et vous juge du regard.\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
							cout << "**Je n'ai rien � dire � quelqu'un d'aussi faible que toi.**\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						}

						if (pvPersoMax > 700 && bagueAvarice == 1)
						{
							cout << "\nLe propri�taire, un Tiefling, arrive et vous demande ce que vous lui voulez. Vous lui demandez alors quel est le secret de son succ�s.\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
							cout << "**Mon secret? Haha! Tu ne manques pas de cran de me poser une telle question. Ma femme a toujours �t� � mes c�t�s � me supporter et c'est elle qui a cr�� tous les recettes pour le menu que nous proposons � Saphir. C'est pourquoi le secret de mon succ�s est ma femme: Nerka**\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						}

						if (bagueAvarice == 2)
						{
							cout << "\nLe propri�taire vous a d�j� dit le secret de son succ�s. Il n'a rien de plus � vous dire.\n";
						}
					}
				}
				break;
			case 3: //Auberge
				while (choixVillage != 4)
				{
					cout << "\nBienvenue � l'auberge ''Chez Mitsuko''! Que voulez-vous faire?\n";
					cout << "1 - Prendre une chambre et faire une sieste (+50% de vos PV max): 40 pi�ces d'or\n";
					cout << "2 - Prendre une chambre et y passer la nuit (vous r�cup�rer 100% de vos PV): 80 pi�ces d'or\n";
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
							cout << "\nApr�s une petite sieste, vous avez r�cup�r� la moiti� de vos PV max\n";
							cout << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
							pbonus -= 40;
							cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
						}
						break;
					case 2: //Louer une chambre, r�cup�re tous ses PV
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
							cout << "\nApr�s une bonne nuit de sommeil, vous avez r�cup�r� tous vos PV\n";
							cout << "Hp de " << nomPerso << ": " << pvPersoActuel << "/" << pvPersoMax << "\n";
							pbonus -= 80;
							cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";
						}
						break;
					case 3: //Parler avec Mitsuko
						while (choixVillage != 5)
						{
							cout << "\nAu comptoir d'accueil se trouve une Kitsune.\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
							cout << "**Bonjour, voyageur! Mon nom est Mitsuko et je suis la propri�taire de cette auberge. Comment puis-je t'aider?**\n";
							SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
							cout << "1 - L�gende sur la cr�ation du village\n" << "2 - Recevoir une qu�te\n" << "3 - Tenter de s�duire l'aubergiste\n";
							cout << "4 - Informations sur les accessoires\n" << "5 - Quitter la compagnie de Mitsuko\n";
							cin >> choixVillage;

							switch (choixVillage)
							{
							case 1: //L�gende, ce qui donne �ventuellement acc�s � un nouveau lieu au sommet de la montagne
								if (pvPersoMax >= 100)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "\n**Selon la l�gende, le dieu Tepnem a �t� banni des cieux apr�s avoir commis un crime grave et r�duit � parcourir la terre pour l'�ternit�. Comme si ce n��tait pas assez, son ch�timent �tait que chaque pas qu'il faisait lui donnait la sensation de se faire poignarder. Ne connaissant que mis�res et souffrances, il tenta plusieurs fois de mettre fin � ses jours, sans succ�s. Pour ne plus ";
									cout << "ressentir une telle douleur, il cessa de se d�placer. Se posant sur le haut d'une montagne, tout ce qu'il pouvait faire �tait d'attendre avec impatience la fin du monde. Tepnem a vu des civilisations na�trent et mourirent. Il a vu l'Homme d�truire la nature autour de lui pour son profit personnel et la nature qui a toujours fini par r�clamer son territoire avec le temps.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								if (pvPersoMax >= 500)
								{
									cout << legendeEvaluation;
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "\n**Cela faisait d�j� plusieurs ann�es, voire des centaines, que Tepnem reposait au sommet du mont ''Vithral''. On pouvait difficilement faire la diff�rence entre son corps et la v�g�tation qui le recouvrait. S��tant plac� contre un arbre au plus haut point de la montagne, il avait auparavant une vue imprenable sur la r�gion, mais avec le temps qui passa, l�arbre se tordit pour accommoder son corps, ";
									cout << "comme s�il voulait le prot�ger du monde externe et ne faire qu�un avec la divinit�. Cet arbre aurait d� finir par fl�trir avec le passage du temps, mais il n�en fit rien. Au contraire, il continua de grandir et de cacher de plus en plus Tepnem en son sein, se nourrissant indirectement de son �nergie. Ce dernier aurait tr�s bien pu se d�placer, mais pour quoi faire? Qu�un arbre l�entoure de plus en plus, ";
									cout << "qu�il ne puisse pratiquement plus voir devant lui � cause des diverses racines qui le cachaient du monde ext�rieur, ne le perturbait aucunement. Sa vie n�avait aucune valeur � ses propres yeux, mais pour cet arbre, il avait une raison d��tre, m�me si ce n��tait que pour �tre une source de nourriture. Aussi �trange cela puisse-t-il paraitre, cela lui suffisait pour rester o� il se trouvait.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								if (pvPersoMax >= 1000)
								{
									cout << legendeEvaluation;
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "\n**Cependant, cela changea lorsqu�une elf se nommant Orianna se rendit au mont ''Vithral'' � la recherche d�herbes m�dicinales. Elle savait qu�un arbre plus grand que tous les autres et qui semblaient exister depuis la nuit des temps se trouvait quelque part au sommet de la montagne, mais elle n�aurait jamais pu imaginer que la raison �tait qu�un �tre sup�rieur s�y renfermait. Elle r�colta avec insouciance ce dont ";
									cout << "elle avait besoin tout en s��merveillant devant la grande vari�t� de faune et de flore se trouvant autour d�elle. Orionna n�aurait jamais pu se douter que son existence avait piqu� l�int�r�t de Tepnem qui n�avait pas vu de si proche une forme de vie dou�e de parole depuis bien longtemps. Il l�observa, le moindre de ses gestes le fascinait. Il se surpris � vouloir r�v�ler sa pr�sence et � lui parler. Ce ne fut qu�au ";
									cout << "moment o� l�elf alla pour quitter les lieux qu�il se d�cida � agir. Se trouvant pr�s du pr�cipice quand le dieu fit sa pr�sence connue, ce fut en se tournant rapidement vers la provenance de sa voix, qu�elle perdit l��quilibre. Elle aurait chut� vers une mort certaine si Tepnem ne s��tait pas lev� et n�avait pas us� de ses pouvoirs pour la retenir dans le vide, lui sauvant ainsi la vie.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								if (pvPersoMax >= 1500)
								{
									cout << legendeEvaluation;
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "\n**Pendant plusieurs ann�es, Orionna rendit visite � Tepnem au sommet du mont. Apprenant � se connaitre, ils finirent par tomber amoureux l�un de l�autre. L�elf tenta de le convaincre de descendre de la montagne et de venir voir le village qui se trouvait � son pied, ne comprenant pas pourquoi il s�obstinait � rester et ne comprenant pas compl�tement � quel point chaque pas �tait une vraie torture pour lui. Elle finit par l�avoir � l�usure et lui fit d�couvrir ";
									cout << "le lieu o� elle vivait, esp�rant qu�un jour il pourra lui aussi appeler ce petit village son chez soi. Les ann�es pass�rent relativement de mani�re paisible, mais avec le temps, la divinit� retrouva son amertume envers sa vie et finit m�me par en vouloir � son �tre aim� de le faire autant souffrir en le faisant marcher tous bords tous c�t�s. Il aurait pu retourner au sommet de ''Vithral'' comme auparavant, mais il apprit qu�Orionna �tait enceinte et ne pouvait s�imaginer l�abandonner elle et sa prog�niture.**\n";

									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "\nVous interrompez l�aubergiste, ne comprenant pas un �l�ment de l�histoire.\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "**Il aurait pu lui expliquer qu�� chaque fois qu�il se d�pla�ait il avait l�impression qu�on le poignardait? Oui, tu as tout � fait raison. Et c�est ce qu�il fit �ventuellement. S�il l�avait fait plus t�t, il se serait �pargn� bien des tourments. Peu de temps apr�s lui avoir r�v�l� le supplice qu�il vivait, l�elf l�emmena jusqu�� la forge du village o� une surprise l�y attendait. Le forgeron, fier de sa cr�ation, avait insist� pour �tre pr�sent lors du d�voilement. Devant les yeux de Tepnem se trouvait une chaise-roulante. ";
									cout << "N�ayant eu aucun contact avec les autres depuis des centaines d�ann�es, il ne s��tait pas rendu � quel point l�Homme avait �volu� et n�avait cess� de cr�er de nouvelles choses pour am�liorer son quotidien. Il fallu que l�artisan et Orionna lui explique ce qu��tait une chaise-roulante. Comprenant qu�il allait pouvoir se d�placer facilement et sans douleurs, des larmes coul�rent librement sur son visage. Pour la premi�re fois, depuis tr�s longtemps, le dieu reprit espoir en la vie.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								if (pvPersoMax >= 2500)
								{
									cout << legendeEvaluation;
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "\n**C��tait un jour comme les autres, la divinit� s�occupait de son enfant de maintenant 5 ans, pendant que sa femme �tait sur le mont ''Vithral'' pour refaire le plein d�herbes m�dicinales. Le soleil se coucha et toujours aucune trace d�Orianna. Ce fait en soi n��tait pas si pr�occupant en sachant qu�elle y restait parfois deux/trois jours sur la montagne. Cependant, apr�s la cinqui�me journ�e, il demanda � deux amis de l�elf de se mettre � sa recherche. ";
									cout << "Quand ils revinrent au village, ce fut en transportant le corps ensanglant� de sa bien-aim�e, elle avait �t� poignard�e. Comme si le ch�timent qu�il vivait depuis son bannissement avait �t� une pr�monition du sort que sa femme allait connaitre. Sous le coup de la peine et de la col�re, Tepnem se leva de sa chaise-roulante et se rendit sur la montagne, dans une vaine tentative de trouver la personne qui avait os� s�en prendre � Orianna et la tuer. Il ne se ";
									cout << "retourna m�me pas, oubliant compl�tement sur le coup qu�il avait un enfant � s�occuper. Retournant au village apr�s quelques jours, il ressentait toujours ce besoin visc�ral de faire payer quelqu�un. Ne pouvant trouver le coupable, il se mit � accuser les villageois pour la mort de son �tre aim�. Si ce n��tait pour son enfant qui se met � travers de son chemin, il aurait probablement tu� tous les habitants du village. Et encore, cela n�aurait pas �t� suffisant ";
									cout << "pour att�nuer cette rage qui le consumait. En revanche, arr�tant son p�re dans son �lan meurtrier lui co�ta la vie. Toute la rage que Tepnem ressentait se transforma de nouveau en d�sespoir. Il prit dans ses bras le corps de sa femme et celui de son enfant et retourna au sommet du mont ''Vithral''. Selon la l�gende, si tu t�y rends, quelque chose apparait quand tu prononces le nom de son enfant: Orblood.**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								else
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << pasConfianceLegende;
								}
								break;
							case 2: //Qu�tes pour recevoir diff�rents items
								if (pvPersoMax < 1000)
								{
									cout << "\nMitsuko ne fait pas assez confiance en vos capacit�s pour vous donner une qu�te. Revenez la voir quand vous serez plus exp�riment�.\n";
								}
								if (pvPersoMax >= 700 && bagueAvarice == 1)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "\nQu�te #1:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Convaincre le propri�taire de la taverne de partager le secret de son succ�s.\n";
									cout << "R�compense: Bague de l'avarice\n";

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
									cout << "\nQu�te #2:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "� la recherche d'une personne pour vaincre le Cerb�re qui se cache dans la mine se trouvant dans la for�t ''Rasaelros''.\n";
									cout << "R�compense: Le bracelet de l'Enfer\n";

									if (changeMapMine == 2)
									{
										cout << "\nVous avez obtenu le bracelet de l'Enfer!\n";
										braceletEnfer = 2;
										itemEnfer = "-BraceletEnfer-";
									}
									
								}
								if (braceletEnfer == 2)
								{
									cout << "\nVous avez compl�t� toutes les qu�tes disponibles.\n";
								}
								break;
							case 3: //Flirter avec Mitsuko, ce qui d�bloque �ventuellement un item
								if (pvPersoMax >= 1500)
								{
									cout << "\nMitsuko vous lance un regard amus�.\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
									cout << "**Tu n'abandonnes pas facilement, c'est une caract�ristique que j'appr�cie chez les autres. Tu me plais! Je vais donc te dire un petit secret. ";
									cout << "� la boutique ''Madness'', vas dans la section ''Autre'' et dit au propri�taire que tu es � la recheche de: tofu. Ah, �a me fait penser que je vais bient�t devoir en acheter...**\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
								}
								else
								{
									cout << "\nMitsuko n'est pas impressionn�e par vos tentatives de s�duction.\n";
								}
								break;
							case 4: //Renseignements sur items re�u par l'entremise de Mitsuko

								cout << "\nL'aubergiste sort un catalogue.\n";

								if (capeKitsune == 333)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "\nCape Kitsunetsuki:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Kitsunetsuki signifie ''�tre poss�d� par un renard''. Cette cape blanche aux bordure rouge augmente de 333 vos points de vie maximum.\n";
								}

								if (bagueAvarice == 2)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "\nBague de l'avarice:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Le d�tenteur de cet accessoire va trouver 2x plus de pi�ces d'or.\n";
								}

								if (braceletEnfer == 2)
								{
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "\nBracelet de l'Enfer:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Son ancien propri�taire et cr�ateur �tait un d�mon sup�rieur. Si vous avez une arme, celle-ci inflige 2x plus de d�gats qu'auparavant.\n";
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
					cout << "\nCombien voulez-vous en acheter en sachant qu'une potion co�te 70 pi�ces d'or? Si vous changez d'avis et ne voulez pas acheter de potion, inscrivez le chiffre suivant: 0\n";
					cin >> nombrePotion;
					coutPotion = nombrePotion * 70;

					if (nombrePotion <= 0)
					{
						cout << "\nAucun achat d'effectu�.\n";
					}
					if (nombrePotion > 0 && pbonus > coutPotion)
					{
						pbonus -= coutPotion;
						potion += nombrePotion;
						cout << "\nAchat effectu�! Vous avez maintenant: " << potion << " potions\n";
						cout << "Il vous reste: " << pbonus << " pi�ces d'or\n";

					}
					else
					{
						erreurArgentManquant;
					}
					break;
				case 2:
					cout << "\nPeu importe le sujet que vous abordez, il n'est pas int�ress� � vous parler. Tout ce qui lui importe est de faire une vente.\n";
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
			cout << "Vous vaincu Cerb�re et avez atteint la sortie de la mine! Allez voir Mitsuko pour recevoir la r�compense � li�e � la qu�te.\n";
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
	string dejaVaincu = "\nVous avez d�j� vaincu le monstre qui se cachait ici.\n";
	string orZero = "\nExaminant de nouveau, vous ne trouvez rien d'autre.\n";
	string retourMontagne = "\nVous PV sont � 0, vous allez retourner au pied de la montagne.\n";
	string impasse = "\nVous ne remarquez rien qui sort de l'ordinaire � part le fait que les trois couloirs devant vous semblent aller dans la m�me direction.";

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
	Temple rouge("Couloir Rouge", "\nD'un rouge intense qui rappelle la couleur du sang, le couloir est �troit.");
	Temple jaune("Couloir Jaune", "\nD'un jaune p�le, vous remarquez d'�tranges dessins sur toutes les parois du couloir.");
	Temple mauve("Couloir Mauve", "\nLa couleur vous donne mal � la t�te, mais sinon il n'y a rien de remarquable le long de votre route.");
	Temple jade("Couloir Jade", "\nVous trouvez un vieux sac et une arme dont la lame est rouill�e au sol. Les deux items n'ont aucune valeur, alors vous d�cidez de ne pas les prendre avec vous.");
	Temple marron("Couloir Marron", "\nLe chemin est sinueux et semble sans fin.");
	Temple noir("Couloir Noir", "\nDes mots dans une langue ancienne sont inscrit sur les murs. Vous essayez de les d�chiffrer sans succ�s.");
	Temple dore("Salle Dor�e", "\nAu millieu de la pi�ce se trouve un homme que vous �valuez dans la trentaine. � votre arriv�e, il vous regarde et sourit, comme s'il vous attendait.");
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

			cout << "\nArrivant au pied du mont ''Vithral'', vous avez un choix � faire. Vers o� voulez-vous aller?:\n";
			cout << "1 - For�t ''Rasaelros''\n" << "2 - Haut de la montagne\n" << "3 - Quitter la montagne\n";
			cin >> choixMontagne;

			switch (choixMontagne)
			{
			case 1: //foret
				while (choixMontagne != 3)
				{
					if (bagueAvarice != 2 || pvPersoMax < 3000)
					{
						cout << "\nAlors que vous parcourez la for�t, vous appercevez une mine. Plus vous vous en approchez, plus votre instinct vous dit de fuir.\n";
						choixMontagne = 3;
					}

					if (joueur.pvActuel == 0)
					{
						cout << pvZero;
						choixMontagne = 3;
					}

					if (bagueAvarice == 2 && pvPersoMax >= 3000 && joueur.pvActuel > 0)
					{
						cout << "\nVous vous dirigez vers la mine mentionn�e dans la qu�te, mais plus vous vous en approchez, plus vous devenez tendu, comme si vous pouviez ressentir qu'un monstre s'y terre. Que voulez-vous faire?:\n";
						cout << "1 - Examiner les lieux\n" << "2 - Poursuivre votre qu�te\n" << "3 - Revenir sur vos pas\n";
						cin >> choixMontagne;

						switch (choixMontagne)
						{
						case 1:
							cout << "\nPrudent, vous jetez un coup d'oeil � l'int�rieur, mais il fait trop sombre pour que vous puissiez voir quoique ce soit.\n";
							break;
						case 2:

							if (joueur.pvActuel == 0)
							{
								cout << "\nVos PV sont � 0, vous ne pouvez pas combattre.\n";
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
					cout << "\nApr�s une longue mont�e, vous arrivez enfin au plus haut point du mont. Vous avez une vue imprenable de la r�gion et vous pouvez voir clairement le village ''Orionna''. Plus le temps passe, plus vous ressentez qu'une �nergie �mane des lieux. Cela vous intrigue.\n";
				}
				if (pvPersoMax >= 1000 && joueur.pvActuel > 0)
				{
					cout << "\nVous explorer le haut de la montagne, mais rien ne semble sortir de l'ordinaire. Toutefois, vous ressentez toujours cette myst�rieuse �nergie. Vous revenez plusieurs jours d'affil�s sur place, sans jamais rien trouver de plus qui expliquerait la sensation que vous avez.\n";
				}

				if (joueur.pvActuel == 0)
				{
					cout << pvZero;
				}

				if (pvPersoMax >= 1500 && joueur.pvActuel > 0)
				{
					cout << "\nAlors que vous vous reposez au pied du plus grand arbre qui se trouve au sommet, vous demandant si vous devriez abandonner, l'arbre se met � briller d'une lueur dor�e. En m�me temps que des mots apparaissent sur l'�corce, une douce voix retentit:\n";
					SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
					cout << "**Mis�res et souffrances �taient le quotidien de Tepnem jusqu'� ce qu'il rencontre Orionna. Il retomba dans ce cercle de douleurs d�s qu'elle quitta cette terre et il a failli tout d�truire autour de lui. Heureusement ou malheureusement, il tua la seule personne pr�cieuse qui lui restait. Ce lieu � pour but de rappeler � ceux qui y parviennent qu'il ne faut jamais perdre espoir et de prot�ger vos �tres chers... Qui �tait cette personne que Tepnem a tu�e?**\n";
					SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					string orblood;
					cin >> orblood;

					if (orblood == "Orblood" || orblood == "orblood")
					{
						cout << "\nAttention, votre d�fense est r�duite � 0 lorsque le temple vous est visible. En quittant le sommet de la montage, vous retrouverez votre d�fense.\n";
						
						while (choixMontagne != 3 && joueur.pvActuel > 0)
						{
							cout << "\nDevant vous se dresse le temple ''Orblood'', un temple en l'honneur du dieu Tepnem et de son enfant. Que voulez-vous faire?:\n";
							cout << "1 - Examiner autour du monument\n" << "2 - P�n�trer dans le temple\n" << "3 - Retourner au pied de la montagne\n";
							cin >> choixMontagne;

							switch (choixMontagne)
							{
							case 1: //exterieur temple
								while (choixMontagne != 4 && joueur.pvActuel > 0)
								{
									cout << "\nLe temple se fond dans la v�g�tation, comme s'ils ne faisaient qu'un. �trangement, le passage du temps ne semble pas avoir affect� le monument, comme si sa construction avait fini la journ�e d'avant, ce qui est impossible. Quel action voulez-vous faire?\n";
									cout << "1 - Examiner les buissons aux alentours\n" << "2 - Agiter un arbre se trouvant proche de vous\n" << "3 - �tudier la fa�ade du temple\n" << "4 - Revenir sur vos pas\n";
									cin >> choixMontagne;

									switch (choixMontagne)
									{
									case 1:
										cout << "\nD�cidant d'investiguer les buissons autour du temple, vous d�couvrer ce qui semble �tre un chiot. Vous en approchant, vous vous arr�tez net en entendant un grognement en arri�re de vous. Vous retournant lentement, vous fa�tes face � un Direwolf. Ce dernier se jette sur vous pour prot�ger son petit!\n";
										cout << "\nDirewolf inflige " << degatBuissons << " points de d�gats � " << joueur.nom << " avant de s'enfuir avec le louveteau.\n";
										
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
										cout << "\nD�cidant de secouer l'arbre proche de vous, un serpent vous tombe dessus. Avant que vous puissiez r�agir, il plante ses crocs en vous!\n";
										cout << "\nSerpent inflige " << degatArbre << " points de d�gats � " << joueur.nom << " avant de disparaitre aussi rapidement qu'il vous a attaqu�.\n";

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
											cout << "\nExaminant la fa�ade du monument, vous remarquer quelque chose qui brille dans l'une des vo�tes au-dessus de l'entr�e. Usant d'ing�niosit� et de votre adresse, vous arrivez � obtenir ce qui se cachait l�-haut.\n";
											if (bagueAvarice == 1)
											{
												cout << "\nVous avez trouv�: " << orExt << " pi�ces d'or\n";
											}
											else
											{
												cout << "\nVous avez trouv�: 2x" << orExt << " pi�ces d'or\n";
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
									cout << "\nVous trouvant � l'int�rieur du temple, vous explorer les lieux et d�couvrez un passage secret. Devant vous apparait un escalier descendant vers des bas fonds inconnus, ainsi qu'une plaque avec le message suivant grav� dessus:\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
									cout << "''Celui qui perd son chemin dans ce labyrinthe ne fera que souffrir jusqu'� sa mort � moins de recevoir la gr�ce de Tepnem.''\n";
									SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
									cout << "Que d�cidez-vous de faire?:\n";
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
												cout << "\nFouillant chaque recoin, vous finissez par trouver: " << orTemple << " pi�ces d'or\n";
											}
											else
											{
												cout << "\nFouillant chaque recoin, vous finissez par trouver: 2x" << orTemple << " pi�ces d'or\n";
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
											if (couloirActuel->nom != "Salle Dor�e")
											{
												cout << "\nVous �tes dans le " << couloirActuel->nom << ". Celui-ci se divise en trois chemins. Choisissez une action:\n" << "1 - Inspecter le couloir actuel\n";
												cout << "2 - Aller vers le couloir de gauche\n" << "3 - Aller vers le couloir du milieu\n" << "4 - Aller vers le couloir de droite\n";
												cin >> choixTemple;

												switch (choixTemple)
												{
												case 1: //Description
													cout << couloirActuel->descrip << "\n";

													//section pieges
													if (couloirActuel->nom == "Couloir Jaune" || couloirActuel->nom == "Couloir Marine" || couloirActuel->nom == "Couloir Gris")
													{
														cout << "\nPoussant votre investigation, vous d�clenchez un pi�ge par m�garde! Il inflige " << degatImpasse1 << " points de d�gats � " << nomPerso << "\n";

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
														cout << "\nPoussant votre investigation, vous d�clenchez un pi�ge par m�garde! Il inflige " << degatImpasse2 << " points de d�gats � " << nomPerso << "\n";

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
														cout << "\nPoussant votre investigation, vous d�clenchez un pi�ge par m�garde! Il inflige " << degatImpasse3 << " points de d�gats � " << nomPerso << "\n";

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
														cout << "\nPoussant votre investigation, vous d�clenchez un pi�ge par m�garde! Il inflige " << degatImpasse4 << " points de d�gats � " << nomPerso << "\n";

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
																cout << "\nPoussant votre investigation, vous trouvez: " << orLab1 << " pi�ces d'or\n";
															}
															else
															{
																cout << "\nPoussant votre investigation, vous trouvez: 2x" << orLab1 << " pi�ces d'or\n";
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
																cout << "\nPoussant votre investigation, vous trouvez: " << orLab2 << " pi�ces d'or\n";
															}
															else
															{
																cout << "\nPoussant votre investigation, vous trouvez: 2x" << orLab2 << " pi�ces d'or\n";
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
												cout << "\nApr�s tous ces couloirs, vous arrivez finalement dans la " << couloirActuel->nom << ". Choisissez une action:\n";
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
													cout << "**Je me nomme Tepnem, tu as peut-�tre d�j� entendu parler de moi.**\n";
													SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
													cout << "\nIl vous observe un instant avant de reprendre, son regard s'assombrant pendant un bref instant.\n";
													SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
													cout << "**Ta d�termination n'est pas � prendre � la l�g�re. J'ai connu quelqu'un d'aussi t�tu que toi dans le pass�. Si je peux me permettre un conseil: n'oublie jamais de prot�ger ceux qui te sont chers, peu importe les moyens et les cons�quences. Ceci devrait t'aider.**\n";
													SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

													if (itemProtecteur == "-")
													{
														cout << "\nVous recevez de la part de Tepnem un item: Protecteur!\n";
														protecteur = 2;
														itemProtecteur = "-Protecteur-";
													}
													else
													{
														cout << "\nVous avez d�j� re�u l'item: Protecteur\n";
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
						cout << "\n**Quand tu sauras ce que tu recherches, reviens me donner ta r�ponse.**\n";
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

		cout << "\n�crivez le chiffre qui correspond � votre choix :\n" << "1 - Cr�er un nouveau personnage\n";
		cout << "2 - Cr�er un personnage par d�faut\n" << "3 - Afficher les caract�ristiques du personnage\n";
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
			cout << "\nVous allez quitter le programme. Que les dieux aient piti� de votre �me...\n";
			return 0;
		}
	}
}
