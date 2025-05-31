#include <stdio.h>
#include <stdbool.h>
#include <string.h>




//Etape 1: Structure de donnée //Spécifier les différentes structures de données (salle, classe, réservation)
                              //Spécifier le format des fichiers de sauvegarde
                             //Définir les structures de données en C, ainsi que les listes correspondantes (tableaux).
#define MAX_RESERVATIONS 100
#define NOMBRE_SALLES 100
#define NOMBRE_CLASSES 100

#define FICHIER_SAUVEGARDE "/Users/bayeabdoulaye/Documents/reservations_salles.txt"
#define FICHIER_CLASSES "classes.txt"

typedef enum {
    RESERVEE,
    ANNULEE,
    VALIDEE,
    EN_COURS,
    TERMINEE
} EtatReservation;
typedef struct {
    int id;
    int salle;
    int classe;
    char jour[10];
    int heure_debut;
    int duree;
    char motif[100];
    EtatReservation etat;
} Reservation;

typedef struct {
    char nom[20];
    int code;
    char position[50];
    char disponibilite_machine[30];
    int capacite;
} Salle;

typedef struct {
    int classe_id;
    char classe_nom[50];
    char niveau[20];
    int effectif;
} Classe;

Salle salles[NOMBRE_SALLES];
Classe classes[NOMBRE_CLASSES];
Reservation reservations[MAX_RESERVATIONS];
int nb_reservations = 0;
int nb_salles = 0;
int nb_classes = 0;
//Fin de l'étape 1











//Etape 2: Gestion des salles//Spécifier les commandes liées à la gestion des salles
                            //Implémenter les actions de création, affichage, modification et suppression d’ une salle
                           // Tester les commandes selon des scénarios


int lireFichierSalles(Salle tableau[]) {
    FILE *fichier = fopen("salles.txt", "r");
    if (fichier == NULL) {
        return 0;
    }

    int i = 0;
    while (i < NOMBRE_SALLES) {
        int resultat = fscanf(fichier, "Nom: %[^\n]\n", tableau[i].nom);
        if (resultat != 1) break;

        fscanf(fichier, "Code unique: %d\n", &tableau[i].code);
        fscanf(fichier, "Position: %[^\n]\n", tableau[i].position);
        fscanf(fichier, "Disponibilite des machines: %[^\n]\n", tableau[i].disponibilite_machine);
        fscanf(fichier, "Capacite: %d\n", &tableau[i].capacite);
        fscanf(fichier, "\n");
        i++;
    }

    fclose(fichier);
    return i;
}


void afficherSalles() {
    FILE *fichier = fopen("salles.txt", "r");
    char contenu[200];
    int vide = 1;

    if (fichier == NULL) {
        printf("Erreur de lecture du fichier.\nVous n'avez sans doute pas encore ajoute une salle\n");
        return;
    }

    while (fgets(contenu, sizeof(contenu), fichier) != NULL) {
        fputs(contenu, stdout);
        vide = 2;
    }

    if (vide == 1) {
        printf("Vous n'avez pas encore ajoute de salle\n");
    }

    fclose(fichier);
}

void modifierSalles(Salle tableau[], int n) {
    int reponse = 1;

    do {
        int code_r, p = -1;
        printf("Saisir le code unique de la salle a modifier: \n");
        scanf("%d", &code_r);

        for (int i = 0; i < n; i++) {
            if (code_r == tableau[i].code) {
                p = i;
                break;
            }
        }

        if (p == -1) {
            printf("Aucune salle trouvee avec ce code.\n");
            continue;
        }

        getchar();
        printf("Saisir le nouveau nom de la salle:\n");
        scanf("%99[^\n]", tableau[p].nom);
        getchar();

        printf("Saisir la nouvelle position de la salle:\n");
        scanf("%99[^\n]", tableau[p].position);
        getchar();

        printf("Saisir la nouvelle disponibilite des machines:\n");
        scanf("%99[^\n]", tableau[p].disponibilite_machine);
        getchar();

        printf("Saisir la nouvelle capacite de la salle: ");
        scanf("%d", &tableau[p].capacite);
        getchar();

        int h;
        do {
            h = 0;
            printf("Saisir le nouveau code unique: \n");
            scanf("%d", &tableau[p].code);

            for (int i = 0; i < n; i++) {
                if (i != p && tableau[p].code == tableau[i].code) {
                    printf("Ce code existe deja. Veuillez en saisir un autre.\n");
                    h = 1;
                    break;
                }
            }
        } while (h == 1);

        FILE *fichier = fopen("salles.txt", "w");
        if (fichier == NULL) {
            printf("Impossible d'ouvrir le fichier\n");
            return;
        }

        for (int i = 0; i < n; i++) {
            fprintf(fichier, "Nom: %s\nCode unique: %d\nPosition: %s\nDisponibilite des machines: %s\nCapacite: %d\n\n",
                    tableau[i].nom, tableau[i].code, tableau[i].position,
                    tableau[i].disponibilite_machine, tableau[i].capacite);
        }

        fclose(fichier);
        printf("Salle modifiee avec succes !!\n");

        printf("Souhaitez-vous modifier une autre salle ? Tapez 1 pour oui : ");
        scanf("%d", &reponse);
    } while (reponse == 1);
}


void rechercherSalle() {
    int code_r, p = -1;
    printf("Saisir le code unique de la salle que vous voulez rechercher: \n");
    scanf("%d", &code_r);

    for (int i = 0; i < nb_salles; i++) {
        if (code_r == salles[i].code) {
            p = i;
            break;
        }
    }

    if (p == -1) {
        printf("Cette salle n'existe pas!!\n");
        return;
    }

    printf("Nom: %s\nPosition: %s\nDisponibilite des machines: %s\nCapacite: %d\nCode: %d\n",
           salles[p].nom, salles[p].position, salles[p].disponibilite_machine,
           salles[p].capacite, salles[p].code);
}

void supprimerSalle() {
    int code_r, p = -1;
    printf("Saisir le code unique de la salle que vous voulez supprimer: \n");
    scanf("%d", &code_r);

    for (int i = 0; i < nb_salles; i++) {
        if (code_r == salles[i].code) {
            p = i;
            break;
        }
    }

    if (p == -1) {
        printf("Aucune salle trouvee avec ce code.\n");
        return;
    }

    for (int i = p; i < nb_salles - 1; i++) {
        salles[i] = salles[i + 1];
    }
    nb_salles--;

    FILE *fichier = fopen("salles.txt", "w");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier\n");
        return;
    }

    for (int i = 0; i < nb_salles; i++) {
        fprintf(fichier, "Nom: %s\nCode unique: %d\nPosition: %s\nDisponibilite des machines: %s\nCapacite: %d\n\n",
                salles[i].nom, salles[i].code, salles[i].position,
                salles[i].disponibilite_machine, salles[i].capacite);
    }

    fclose(fichier);
    printf("La salle a ete supprimee avec succes !!\n");
}

void ajouterSalles(Salle tableau[], int *n) {
    int reponse = 1;

    do {
        getchar(); // Consommer le '\n'
        printf("Saisir le nom de la salle:\n");
        scanf("%99[^\n]", tableau[*n].nom);
        getchar();

        printf("Saisir la position de la salle:\n");
        scanf("%99[^\n]", tableau[*n].position);
        getchar();

        printf("Saisir la disponibilite des machines:\n");
        scanf("%99[^\n]", tableau[*n].disponibilite_machine);
        getchar();

        printf("Saisir la capacite de la salle: \n");
        scanf("%d", &tableau[*n].capacite);
        getchar();

        int h;
        do {
            h = 0;
            printf("Saisir le code unique: \n");
            scanf("%d", &tableau[*n].code);

            for (int i = 0; i < *n; i++) {
                if (tableau[*n].code == tableau[i].code) {
                    printf("Ce code existe deja. Veuillez en saisir un autre.\n");
                    h = 1;
                    break;
                }
            }
        } while (h == 1);

        (*n)++;

        FILE *fichier = fopen("salles.txt", "w");
        if (fichier == NULL) {
            printf("Impossible d'ouvrir le fichier\n");
            return;
        }

        for (int i = 0; i < *n; i++) {
            fprintf(fichier, "Nom: %s\nCode unique: %d\nPosition: %s\nDisponibilite des machines: %s\nCapacite: %d\n\n",
                    tableau[i].nom, tableau[i].code, tableau[i].position,
                    tableau[i].disponibilite_machine, tableau[i].capacite);
        }

        fclose(fichier);
        printf("Salle ajoutee avec succes !\n");

        printf("Voulez-vous ajouter une nouvelle salle ? Tapez 1 pour oui : ");
        scanf("%d", &reponse);
    } while (reponse == 1);
}


void menu_salles() {
    int choix;
    do {
        printf("\nGESTION DES SALLES \n");
        printf("1. Afficher les salles\n");
        printf("2. Modifier une salle\n");
        printf("3. Ajouter une salle\n");
        printf("4. Rechercher une salle\n");
        printf("5. Supprimer une salle\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix: ");
        scanf("%d", &choix);

        switch(choix) {
            case 1: afficherSalles(); break;
            case 2: modifierSalles(salles, nb_salles); break;
            case 3: ajouterSalles(salles, &nb_salles); break;
            case 4: rechercherSalle(); break;
            case 5: supprimerSalle(); break;
            case 0: printf("Retour au menu principal\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

//Fin de l'etape 2


//Etape 3: Gestion des classes //Spécifier les commandes liées à la gestion des classes
                              //Implémenter les actions de création, affichage, modification et suppression d’une classe
                             //Tester les commandes selon des scénarios


int chargerClasses(Classe *classes, int *nb_classes) {
    FILE *fp = fopen(FICHIER_CLASSES, "r");
    if (fp == NULL) {
        printf("Aucune sauvegarde de classes trouvée\n");
        return 0;
    }

    int n;
    fscanf(fp, "%d\n", &n);

    if (n > NOMBRE_CLASSES) {
        printf("Erreur: Trop de classes dans le fichier\n");
        fclose(fp);
        return 0;
    }

    char buffer[100];

    for (int i = 0; i < n; i++) {
        fgets(buffer, sizeof(buffer), fp);
        sscanf(buffer, "Code: %d", &classes[i].classe_id);

        fgets(buffer, sizeof(buffer), fp);
        sscanf(buffer, "Nom: %[^\n]", classes[i].classe_nom);

        fgets(buffer, sizeof(buffer), fp);
        sscanf(buffer, "Niveau: %[^\n]", classes[i].niveau);

        fgets(buffer, sizeof(buffer), fp);
        sscanf(buffer, "Effectif: %d", &classes[i].effectif);
    }


    fclose(fp);
    *nb_classes = n;
    return n;
}


void sauvegarderClasses(Classe *classes, int nb_classes) {
    FILE *fp = fopen(FICHIER_CLASSES, "w");
    if (fp == NULL) {
        printf("Erreur d'ouverture du fichier classes!\n");
        return;
    }

    fprintf(fp, "%d\n", nb_classes);

    for (int i = 0; i < nb_classes; i++) {
        fprintf(fp, "Code: %d\n", classes[i].classe_id);
        fprintf(fp, "Nom: %s\n", classes[i].classe_nom);
        fprintf(fp, "Niveau: %s\n", classes[i].niveau);
        fprintf(fp, "Effectif: %d\n", classes[i].effectif);
    }

    fclose(fp);
}


void ajouterClasse(Classe *classes, int *nb_classes) {
    if (*nb_classes >= NOMBRE_CLASSES) {
        printf("Erreur: Nombre maximum de classes atteint\n");
        return;
    }

    Classe nouvelle;

    printf("\nAjout d'une nouvelle classe\n");
    do {
        printf("Code (entier positif unique): ");
        scanf("%d", &nouvelle.classe_id);
        getchar();

        bool existe = false;
        for (int i = 0; i < *nb_classes; i++) {
            if (classes[i].classe_id == nouvelle.classe_id) {
                printf("Ce code existe déjà!\n");
                existe = true;
                break;
            }
        }
        if (!existe) break;
    } while (true);

    printf("Nom de la classe: ");
    fgets(nouvelle.classe_nom, sizeof(nouvelle.classe_nom), stdin);
    nouvelle.classe_nom[strcspn(nouvelle.classe_nom, "\n")] = '\0';

    printf("Niveau: ");
    fgets(nouvelle.niveau, sizeof(nouvelle.niveau), stdin);
    nouvelle.niveau[strcspn(nouvelle.niveau, "\n")] = '\0';

    do {
        printf("Effectif: ");
        scanf("%d", &nouvelle.effectif);
    } while (nouvelle.effectif <= 0);

    classes[*nb_classes] = nouvelle;
    (*nb_classes)++;
    sauvegarderClasses(classes, *nb_classes);
    printf("Classe ajoutée avec succès!\n");
}

void afficherClasses(Classe *classes, int nb_classes) {
    if (nb_classes == 0) {
        printf("Aucune classe enregistrée\n");
        return;
    }

    printf("\nListe des classes (%d):\n", nb_classes);
    for (int i = 0; i < nb_classes; i++) {
        printf("code: %d Nom: %s (Niveau: %s) - Effectif: %d\n",
               classes[i].classe_id,
               classes[i].classe_nom,
               classes[i].niveau,
               classes[i].effectif);
    }
}

void rechercherClasse(Classe *classes, int nb_classes) {
    int code;
    printf("Code de la classe à rechercher: ");
    scanf("%d", &code);

    for (int i = 0; i < nb_classes; i++) {
        if (classes[i].classe_id == code) {
            printf("\nClasse trouvée:\n");
            printf("Code: %d\n", classes[i].classe_id);
            printf("Nom: %s\n", classes[i].classe_nom);
            printf("Niveau: %s\n", classes[i].niveau);
            printf("Effectif: %d\n", classes[i].effectif);
            return;
        }
    }
    printf("Classe non trouvée\n");
}

void modifierClasse(Classe *classes, int nb_classes) {
    int code;
    printf("Code de la classe à modifier: ");
    scanf("%d", &code);
    getchar();

    for (int i = 0; i < nb_classes; i++) {
        if (classes[i].classe_id == code) {
            printf("Nouveau nom (actuel: %s): ", classes[i].classe_nom);
            fgets(classes[i].classe_nom, sizeof(classes[i].classe_nom), stdin);
            classes[i].classe_nom[strcspn(classes[i].classe_nom, "\n")] = '\0';

            printf("Nouveau niveau (actuel: %s): ", classes[i].niveau);
            fgets(classes[i].niveau, sizeof(classes[i].niveau), stdin);
            classes[i].niveau[strcspn(classes[i].niveau, "\n")] = '\0';

            do {
                printf("Nouvel effectif (actuel: %d): ", classes[i].effectif);
                scanf("%d", &classes[i].effectif);
            } while (classes[i].effectif <= 0);

            sauvegarderClasses(classes, nb_classes);
            printf("Classe modifiée avec succès\n");
            return;
        }
    }
    printf("Classe non trouvée\n");
}

void supprimerClasse(Classe *classes, int *nb_classes) {
    int code;
    printf("Code de la classe à supprimer: ");
    scanf("%d", &code);

    for (int i = 0; i < *nb_classes; i++) {
        if (classes[i].classe_id == code) {
            for (int j = i; j < *nb_classes - 1; j++) {
                classes[j] = classes[j + 1];
            }
            (*nb_classes)--;
            sauvegarderClasses(classes, *nb_classes);
            printf("Classe supprimée avec succès\n");
            return;
        }
    }
    printf("Classe non trouvée\n");
}

void menu_classes() {
    int choix;
    do {
        printf("\nGESTION DES CLASSES\n");
        printf("1. Ajouter une classe\n");
        printf("2. Afficher toutes les classes\n");
        printf("3. Rechercher une classe\n");
        printf("4. Modifier une classe\n");
        printf("5. Supprimer une classe\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix: ");
        scanf("%d", &choix);

        switch(choix) {
            case 1: ajouterClasse(classes, &nb_classes); break;
            case 2: afficherClasses(classes, nb_classes); break;
            case 3: rechercherClasse(classes, nb_classes); break;
            case 4: modifierClasse(classes, nb_classes); break;
            case 5: supprimerClasse(classes, &nb_classes); break;
            case 0: printf("Retour au menu principal\n"); break;
            default: printf("Choix invalide\n");
        }
    } while (choix != 0);
}

//Fin etape 3




//Etape 4: Gestion des reservations //Spécifier les commandes liées à la gestion des réservations
                                    //Implémenter les actions de création, affichage,
                                    //modification et suppression d’une réservation (avec contrôle des données salle et classe)


/*ces trois fonctions permettrait de comparer les capacites entre une salle
 et une classe au moment d'ajouter une reservation et de verifier
 si une salle a une capacite inferieure à celle d'une classe
 A considerer plutard: le fait de pouvoir ajouter des chaises si la difference
                        des capacites n'est pas enormes.
 */


/*pour avoir la capacite de la salle dont le code est saisi, si le code de la salle saisi existe
                                dans les salles(if (salles[i].code == code_salle)
                            retourne sa capacite*/
int capacite_salle(int code_salle) {
    for (int i = 0; i < nb_salles; i++) {
         if (salles[i].code == code_salle) {
            return salles[i].capacite;
        }
    }
    return -1;
}

//Meme objectif que la fonction si dessus
int effectif_classe(int id_classe) {
    for (int i = 0; i < nb_classes; i++) {
        if (classes[i].classe_id == id_classe) {
            return classes[i].effectif;
        }
    }
    return -1;
}


/*Fonction qui prend comme parametres les valeurs retournées par les deux fonctions au dessus
 ensuite compare les deux valeurs pour savoir si la salle peut accueillir la classe(return true)
 par defaut return false
 */

bool verifier_capacite(int code_salle, int id_classe) {
    int capacite = capacite_salle(code_salle);
    int effectif = effectif_classe(id_classe);

    if (capacite == -1 || effectif == -1) {
        printf("Erreur : salle ou classe introuvable\n");
        return false;
    }
    if (capacite >= effectif) {
        return true;
    }
    printf("Erreur : capacité insuffisante (%d < %d)\n", capacite, effectif);
    return false;
}
////Fin des fonctions sur les capacités/comparaison.




//Fonction pour verifier si le jour est correctement saisi
bool jour_valide(const char* jour) {
    const char* jours_valides[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};
    for (int i = 0; i < 7; i++) {
        if (strcmp(jour, jours_valides[i]) == 0)
            return true;
    }
    return false;
}

//Fonction pour verifier si l'heure saisi est entre 8h et 20h
bool heure_valide(int heure) {
    if (heure < 8 || heure > 20) {
        printf("Erreur: l'heure doit être entre 8h et 20h\n");
        return false;
    }
    return true;
}

/*cette fonction sera appele au niveau de la saisi des informations de reservations
  donc apres saisi des informations on verifiera en comparant par rapport
  aux reservations existante si une salle est reservée à la meme heure que la reservation
  actuelle sauf les etats terminée et annulée
*/
bool salle_disponible(int salle, const char* jour, int heure_debut, int duree) {
    int heure_fin = heure_debut + (duree / 60);

    for (int i = 0; i < nb_reservations; i++) {
        if (reservations[i].etat == ANNULEE || reservations[i].etat == TERMINEE) continue;

        if (reservations[i].salle == salle && strcmp(reservations[i].jour, jour) == 0) {
            int fin = reservations[i].heure_debut + (reservations[i].duree / 60);

            if (heure_debut < fin && heure_fin > reservations[i].heure_debut) {
                printf("Conflit avec la réservation existante : %dh-%dh\n",
                       reservations[i].heure_debut, fin);
                return false;
            }
        }
    }
    return true;
}

void enregistrer_res() {
    FILE *f = fopen(FICHIER_SAUVEGARDE, "w");
    if (!f) {
        printf("Erreur: impossible de créer %s\n", FICHIER_SAUVEGARDE);
        return;
    }

    for (int i = 0; i < nb_reservations; i++) {
        fprintf(f, "%d %d %d %s %d %d \"%s\" %d\n",
                reservations[i].id,
                reservations[i].salle,
                reservations[i].classe,
                reservations[i].jour,
                reservations[i].heure_debut,
                reservations[i].duree,
                reservations[i].motif,
                reservations[i].etat);
    }
    fclose(f);
    printf("Enregistrer dans dans %s\n", FICHIER_SAUVEGARDE);
}

void lirefichierRes() {
    FILE *f = fopen(FICHIER_SAUVEGARDE, "r");
    if (!f) {
        printf("Aucune donnée existante (fichier: %s)\n", FICHIER_SAUVEGARDE);
        return;
    }

    nb_reservations = 0;
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        int result;
        result = sscanf(ligne, "%d %d %d %9s %d %d \"%99[^\"]\" %d",    //avec sscanf chaque ligne est lu completement et isolé
                          &reservations[nb_reservations].id,
                          &reservations[nb_reservations].salle,
                          &reservations[nb_reservations].classe,
                          reservations[nb_reservations].jour,
                          &reservations[nb_reservations].heure_debut,
                          &reservations[nb_reservations].duree,
                          reservations[nb_reservations].motif,          //avec fscanf si le champ motif contient
                          &reservations[nb_reservations].etat);        //contient plusieurs mot et n'est pas entre guillemets seul le 1er mot
                                                                            //serait lu

        if (result == 8) {   //verifie si toute les champs ont ete correctement lus
            nb_reservations++;
            if (nb_reservations >= MAX_RESERVATIONS) break;
        }
    }
    fclose(f);
    printf("%d réservations chargées\n", nb_reservations);
}

void ajouter_reservation() {
    if (nb_reservations >= MAX_RESERVATIONS) {
        printf("Erreur: Nombre maximum de réservations atteint.\n");
        return;
    }

    Reservation r;
    printf("\nAjout d'une nouvelle réservation \n");
    while(getchar() != '\n');

    //saisi du code
    bool id_valide;
    do {
        id_valide = true;
        printf("ID de la réservation (entier unique) : ");
        scanf("%d", &r.id);

        for (int i = 0; i < nb_reservations; i++) {
            if (reservations[i].id == r.id) {
                printf("Erreur : cet ID existe déjà. Veuillez en choisir un autre.\n");
                id_valide = false;
                break;
            }
        }

        if (id_valide && r.id <= 0) {
            printf("Erreur : l'ID doit être un nombre positif.\n");
            id_valide = false;
        }
    } while (!id_valide);

    //saisi du code de la salle
    bool salle_existe;
    do {
        salle_existe = false;
        printf("Code de la salle: ");
        scanf("%d", &r.salle);
        while(getchar() != '\n'); // pour enlever le retour a la ligne \n

        for (int i = 0; i < nb_salles; i++) {
            if (salles[i].code == r.salle) {
                salle_existe = true;
                break;
            }
        }

        if (!salle_existe) {
            printf("Erreur : cette salle n'existe pas. Codes disponibles :\n");
            for (int i = 0; i < nb_salles; i++) {
                printf("- %d: %s\n", salles[i].code, salles[i].nom);
            }
        }
    } while (!salle_existe);


    //saisi du code de la classe
    bool classeExiste;
    do {
        classeExiste = false;
        printf("Code de la classe: ");
        scanf("%d", &r.classe);
        while(getchar() != '\n');


        for (int i = 0; i < nb_classes; i++) {
            if (classes[i].classe_id == r.classe) {
                classeExiste = true;
                printf("Classe validée: %s (Effectif: %d)\n",
                      classes[i].classe_nom, classes[i].effectif);
                break;
            }
        }


        if (!classeExiste) {
            printf("\nERREUR: Classe inexistante. Options valides:\n");
            for (int i = 0; i < nb_classes; i++) {
                printf("- %d: %s (Niveau: %s, Cap: %d)\n",
                      classes[i].classe_id,
                      classes[i].classe_nom,
                      classes[i].niveau,
                      classes[i].effectif);
            }
        }
    } while (!classeExiste);

    //saisi du jour
    do {
        printf("Jour (ex: Lundi): ");
        scanf("%9s", r.jour);
    } while (!jour_valide(r.jour));  //Fonction definit à la 611e ligne

    //saisi de l'heure
    do {
        printf("Heure de début (8-20): ");
        scanf("%d", &r.heure_debut);
    } while (!heure_valide(r.heure_debut));  //Fonction definit à la 620e ligne

    //saisi de la durée
    printf("Durée en minutes: ");
    scanf("%d", &r.duree);

    printf("Motif: ");
    scanf(" %99[^\n]", r.motif);

    //verification par rapport aux reservations existante
    if (!verifier_capacite(r.salle, r.classe)) {   //Fonction definit à la 576e ligne
        printf("Erreur : la salle ne peut pas accueillir cette classe.\n");
        return;
    }

    if (!salle_disponible(r.salle, r.jour, r.heure_debut, r.duree)) {  //Fonction definit a la 583e ligne
        printf("Erreur: La salle n'est pas disponible\n");
        return;
    }

    r.etat = RESERVEE;
    reservations[nb_reservations++] = r;
    enregistrer_res();
    printf("Réservation ajoutée avec succès (ID: %d)\n", r.id);
}

void afficher_reservations() {
    printf("\nListe des réservations (%d) \n", nb_reservations);

    for (int i = 0; i < nb_reservations; i++) {
        printf("ID: %d | Salle: %d | Classe: %d\n",
               reservations[i].id,
               reservations[i].salle,
               reservations[i].classe);
        printf("Jour: %s | De %dh à %dh | Motif: %s\n",
               reservations[i].jour,
               reservations[i].heure_debut,
               reservations[i].heure_debut + (reservations[i].duree / 60),
               reservations[i].motif);
        printf("État: %d\n\n", reservations[i].etat);
    }
}

void modifier_etatres() {
    int id;
    int nouvel_etat;

    printf("\nModifier l'état d'une réservation \n");
    printf("ID de la réservation: ");
    scanf("%d", &id);

    bool trouve = false;
    for (int i = 0; i < nb_reservations; i++) {
        if (reservations[i].id == id) {
            trouve = true;
            printf("Nouvel état (0=Réservée, 1=Annulée, 2=Validée, 3=En cours, 4=Terminée): ");
            scanf("%d", &nouvel_etat);

            if (nouvel_etat >= 0 && nouvel_etat <= 4) {
                reservations[i].etat = nouvel_etat;
                enregistrer_res();
                printf("État mis à jour avec succès.\n");
            } else {
                printf("Erreur: État invalide.\n");
            }
            break;
        }
    }

    if (!trouve) {
        printf("Erreur: Aucune réservation trouvée avec cet ID.\n");
    }
}

/* Fonction pour rechercher les réservations d'une classe
  Ne prends pas en compte les reservations annulées ou terminées
 */

void rechercherRes_classe() {
    int id_classe;
    printf("Entrez l'ID de la classe : ");
    scanf("%d", &id_classe);

    printf("\nRéservations pour la classe %d :\n", id_classe);
    bool trouve = false;

    for (int i = 0; i < nb_reservations; i++) {
        if (reservations[i].classe == id_classe &&
            (reservations[i].etat == RESERVEE ||
             reservations[i].etat == VALIDEE ||   //reservations = Reservation[100]
             reservations[i].etat == EN_COURS)) {

            printf("ID: %d | Salle: %d\n", reservations[i].id, reservations[i].salle);
            printf("Jour: %s | De %dh à %dh\n", reservations[i].jour,
                   reservations[i].heure_debut,
                   reservations[i].heure_debut + (reservations[i].duree / 60));
            printf("Motif: %s | Etat: ", reservations[i].motif);

            switch(reservations[i].etat) {
                case RESERVEE: printf("Réservée\n"); break;
                case VALIDEE: printf("Validée\n"); break;
                case EN_COURS: printf("En cours\n"); break;
                default: break;
            }
            printf("\n");
            trouve = true;
        }
    }

    if (!trouve) {
        printf("Aucune réservation trouvée pour cette classe.\n");
    }
}

/* Fonction pour rechercher les réservations d'une salle
  cette fonctions ne prends pas en compte terminees et annulees
 */

void rechercherRes_salle() {
    int code_salle;
    printf("Entrez le code de la salle : ");
    scanf("%d", &code_salle);

    printf("\nRéservations pour la salle %d :\n", code_salle);
    bool trouve = false;

    for (int i = 0; i < nb_reservations; i++) {
        if (reservations[i].salle == code_salle &&
            (reservations[i].etat == RESERVEE ||
             reservations[i].etat == VALIDEE ||
             reservations[i].etat == EN_COURS)) {

            printf("ID: %d | Classe: %d\n", reservations[i].id, reservations[i].classe);
            printf("Jour: %s | De %dh à %dh\n", reservations[i].jour,
                   reservations[i].heure_debut,
                   reservations[i].heure_debut + (reservations[i].duree / 60));
            printf("Motif: %s | Etat: ", reservations[i].motif);

            switch(reservations[i].etat) {
                case RESERVEE: printf("Réservée\n"); break;
                case VALIDEE: printf("Validée\n"); break;
                case EN_COURS: printf("En cours\n"); break;
                default: break;
            }
            printf("\n");
            trouve = true;
        }
    }

    if (!trouve) {
        printf("Aucune réservation trouvée pour cette salle.\n");
    }
}

// Fonction pour supprimer une réservation
void supprimerReservation() {
    int id;
    printf("Entrez l'ID de la réservation à supprimer : ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < nb_reservations; i++) {
        if (reservations[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Aucune réservation trouvée avec cet ID.\n");
        return;
    }

    ;
    for (int i = index; i < nb_reservations - 1; i++) {
        reservations[i] = reservations[i + 1];
    }

    nb_reservations--;
    enregistrer_res();
    printf("Réservation supprimée avec succès.\n");
}


void afficherplansalle() {
    int code_salle;
    printf("Entrez le code de la salle pour voir son planning: ");
    scanf("%d", &code_salle);


    bool salle_existe = false;
    char nom_salle[20] = "Inconnue";
    for (int i = 0; i < nb_salles; i++) {
        if (salles[i].code == code_salle) {
            salle_existe = true;
            strcpy(nom_salle, salles[i].nom);
            break;
        }
    }

    if (!salle_existe) {
        printf("Erreur: Aucune salle trouvée avec ce code.\n");
        return;
    }


    char nom_fichier[100];
    sprintf(nom_fichier, "planning_salle_%d.txt", code_salle);
    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir ce fichier. %s\n", nom_fichier);
        return;
    }


    const char* jours_semaine[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};


    printf("\nPlanning - %s (Code: %d)\n", nom_salle, code_salle);
    fprintf(fichier, "Planning - %s (Code: %d)\n", nom_salle, code_salle);


    printf("Heure   |");
    fprintf(fichier, "Heure   |");
    for (int j = 0; j < 7; j++) {
        printf(" %-10s|", jours_semaine[j]);
        fprintf(fichier, " %-10s|", jours_semaine[j]);
    }
    printf("\n-------------------------------------------------------------------------------------\n");
    fprintf(fichier, "\n-------------------------------------------------------------------------------------\n");

    int heure;
    for (heure = 8; heure <= 20; heure += 2) {
        printf("%dh-%2dh |", heure, heure+2);
        fprintf(fichier, "%dh-%dh |", heure, heure+2);

        for (int j = 0; j < 7; j++) {
            bool trouve = false;

            for (int i = 0; i < nb_reservations; i++) {
                if (reservations[i].salle == code_salle &&
                    strcmp(reservations[i].jour, jours_semaine[j]) == 0 &&
                    reservations[i].etat != ANNULEE) {

                    int heure_fin = reservations[i].heure_debut + (reservations[i].duree / 60);

                    if (reservations[i].heure_debut < heure+2 && heure_fin > heure) {
                        printf(" %-10s|", reservations[i].motif);
                        fprintf(fichier, " %-10s|", classes[i].classe_nom); // A remplacer par nom des classe(Haroun)
                        trouve = true;
                        break;
                    }
                }
            }

            if (!trouve) {
                printf(" %-10s|", "Libre");
                fprintf(fichier, " %-10s|", "Libre");
            }
        }
        printf("\n");
        fprintf(fichier, "\n");
    }



    fclose(fichier);
    printf("Planning aussi enregistré dans le fichier : %s\n", nom_fichier);
}




// Fonction pour compter les réservations d'une salle
int compter_reservations_salle(int code_salle) {
    int compteur = 0;
    for (int i = 0; i < nb_reservations; i++) {
        if (reservations[i].salle == code_salle && reservations[i].etat != ANNULEE) {
            compteur++;
        }
    }
    return compteur;
}

// Fonction pour afficher les salles les plus utilises
void afficher_salles_plus_utilisees() {
    printf("\nVoici la liste des salles les plus utilises\n");



    int utilisations[NOMBRE_SALLES] = {0};

    int i;
    for (i = 0; i < nb_salles; i++) {
        utilisations[i] = compter_reservations_salle(salles[i].code);
    }

    // Trier les salles par nombre d'utilisations
    for (i = 0; i < nb_salles - 1; i++) {
        for (int j = 0; j < nb_salles - i - 1; j++) {
            if (utilisations[j] < utilisations[j+1]) {

                int temp_util = utilisations[j];
                utilisations[j] = utilisations[j+1];
                utilisations[j+1] = temp_util;


                Salle temp_salle = salles[j];
                salles[j] = salles[j+1];
                salles[j+1] = temp_salle;
            }
        }
    }

    // Afficher les salles par ordre décroissant d'utilisation
    for (i = 0; i < nb_salles; i++) {
        printf("%d. %s (Code: %d) - %d réservations\n",
               i+1,
               salles[i].nom,
               salles[i].code,
               utilisations[i]);
    }


    FILE *fichier = fopen("salles_utilisees.txt", "w");
    if (fichier) {
        fprintf(fichier, "SALLES LES PLUS UTILISEES\n");


        for (i = 0; i < nb_salles; i++) {
            fprintf(fichier, "%d. %s (Code: %d) - %d réservations\n",
                    i+1,
                    salles[i].nom,
                    salles[i].code,
                    utilisations[i]);
        }

        fclose(fichier);
        printf("\nListe enregistrée dans 'salles_utilisees.txt'\n");
    }


}



void edt() {
    if (nb_classes == 0) {
        printf("Aucune classe enregistrée.\n");
        return;
    }

    printf("\nListe des classes disponibles:\n");
    for (int i = 0; i < nb_classes; i++) {
        printf("%d. %s (%s)\n", classes[i].classe_id, classes[i].classe_nom, classes[i].niveau);
    }

    int id_classe;
    printf("\nEntrez l'ID de la classe: ");
    scanf("%d", &id_classe);

    bool classe_existe = false;
    char nom_classe[50] = "Inconnue";
    for (int i = 0; i < nb_classes; i++) {
        if (classes[i].classe_id == id_classe) {
            classe_existe = true;
            strcpy(nom_classe, classes[i].classe_nom);
            break;
        }
    }

    if (!classe_existe) {
        printf("Erreur: Aucune classe trouvée avec cet ID.\n");
        return;
    }

    char nom_fichier[100];
    sprintf(nom_fichier, "emploi_du_temps_%s.txt", nom_classe);

    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        printf("Impossible de créer le fichier %s\n", nom_fichier);
        return;
    }

    const char* jours_semaine[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};

    printf("\nEmploi du temps - Classe %s (ID: %d)\n", nom_classe, id_classe);
    fprintf(fichier, "Emploi du temps - Classe %s (ID: %d)\n", nom_classe, id_classe);

    printf("Heure   |");
    fprintf(fichier, "Heure   |");
    for (int j = 0; j < 7; j++) {
        printf(" %-18s|", jours_semaine[j]);
        fprintf(fichier, " %-18s|", jours_semaine[j]);
    }
    printf("\n-------------------------------------------------------------------------------------\n");
    fprintf(fichier, "\n-------------------------------------------------------------------------------------\n");

    for (int heure = 8; heure <= 20; heure += 2) {
        printf("%dh-%2dh |", heure, heure+2);
        fprintf(fichier, "%dh-%dh |", heure, heure+2);

        for (int j = 0; j < 7; j++) {
            bool trouve = false;

            for (int i = 0; i < nb_reservations; i++) {
                if (reservations[i].classe == id_classe &&
                    strcmp(reservations[i].jour, jours_semaine[j]) == 0 &&
                    reservations[i].etat != ANNULEE) {

                    int heure_fin = reservations[i].heure_debut + (reservations[i].duree / 60);

                    if (reservations[i].heure_debut < heure+2 && heure_fin > heure) {
                        char nom_salle[20] = "Inconnue";
                        for (int k = 0; k < nb_salles; k++) {
                            if (salles[k].code == reservations[i].salle) {
                                strcpy(nom_salle, salles[k].nom);
                                break;
                            }
                        }

                        char cours[6];
                        strncpy(cours, reservations[i].motif, 5);
                        cours[5] = '\0';


                        char cellule[50];
                        sprintf(cellule, "%-5s/%-5s", cours, nom_salle);

                        printf(" %-18s|", cellule);
                        fprintf(fichier, " %-18s|", cellule);
                        trouve = true;
                        break;
                    }
                }
            }

            if (!trouve) {
                printf(" %-18s|", "Libre");
                fprintf(fichier, " %-18s|", "Libre");
            }
        }
        printf("\n");
        fprintf(fichier, "\n");
    }

    fclose(fichier);
    printf("\nEmploi du temps enregistré dans %s\n", nom_fichier);
}

//Voici la fonction sur le menu des reservations
void menu_reservations() {
    int choix;
    do {
        printf("\nGESTION DES RESERVATIONS \n");
        printf("1. Ajouter une réservation\n");
        printf("2. Afficher toutes les réservations\n");
        printf("3. Modifier l'état d'une réservation\n");
        printf("4. Rechercher les réservations d'une classe\n");
        printf("5. Rechercher les réservations d'une salle\n");
        printf("6. Supprimer une réservation\n");
        printf("7. Afficher planning d'une salle\n");
        printf("8. Emploi du temps\n");
        printf("9. Voir salles les plus utilisées\n");
        printf("0. Retour au menu principal\n");
        printf("Votre choix: ");
        scanf("%d", &choix);

        switch(choix) {
            case 1: ajouter_reservation(); break;
            case 2: afficher_reservations(); break;
            case 3: modifier_etatres(); break;
            case 4: rechercherRes_classe(); break;
            case 5: rechercherRes_salle(); break;
            case 6: supprimerReservation(); break;
            case 7: afficherplansalle(); break;
            case 8: edt(); break;
            case 9: afficher_salles_plus_utilisees(); break;
            case 0: printf("Retour au menu principal\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

//Fin etape 4










//        MENU PRINCIPAL
//Voici le menu principal avec les reserv salles classes, ...
void menu_principal() {
    int choix;
    do {
        printf("Bienvenue! Vous êtes dans le menu principal\n");
        printf("[1]Réservations  [2]Salles  [3]Classes  [0]Quitter\n");
        printf("> ");
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                menu_reservations();
                break;
            case 2:
                menu_salles();
                break;
            case 3:
                menu_classes();
                break;
            case 0:
                printf("Au revoir!\n");
                break;
            default:
                printf("Choix invalide.\n");

        }
    } while (choix != 0);
}




//Fonction principale du programme int main()
int main() {
    nb_salles = lireFichierSalles(salles);
    if (nb_salles == 0) {
        printf("Aucune salle trouvee, initialisation des salles par defaut...\n");
    }

    lirefichierRes();

    nb_classes = chargerClasses(classes, &nb_classes);
    if (nb_classes == 0) {
        printf("Aucune classe trouvee, initialisation vide...\n");
    }

    menu_principal();

    enregistrer_res();

    sauvegarderClasses(classes, nb_classes);

    return 0;
}