#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#define MAX_RESERVATIONS 100
#define MAX_CODE_LENGTH 10
#define NOMBRE_SALLES 100
#define NOMBRE_CLASSES 100

#define FICHIER_SAUVEGARDE "/Users/bayeabdoulaye/Documents/reservations_salles.txt"

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











// Cette partie contient les fonctions concernant les salles,
//Implémenté par Bamba
void initialiserSalles(Salle tableau[], int *n) {
    strcpy(tableau[0].nom, "Salle-RC1");
    tableau[0].code = 1;
    strcpy(tableau[0].position, "Rez-de-chaussee");
    strcpy(tableau[0].disponibilite_machine, "Pas-de-machine");
    tableau[0].capacite = 70;

    strcpy(tableau[1].nom, "Salle-RC2");
    tableau[1].code = 2;
    strcpy(tableau[1].position, "Rez-de-chaussee");
    strcpy(tableau[1].disponibilite_machine, "Pas-de-machine");
    tableau[1].capacite = 58;

    strcpy(tableau[2].nom, "Salle-RC3");
    tableau[2].code = 3;
    strcpy(tableau[2].position, "Rez-de-chaussee");
    strcpy(tableau[2].disponibilite_machine, "Pas-de-machine");
    tableau[2].capacite = 58;

    strcpy(tableau[3].nom, "Salle-RC4");
    tableau[3].code = 4;
    strcpy(tableau[3].position, "Rez-de-chaussee");
    strcpy(tableau[3].disponibilite_machine, "Pas-de-machine");
    tableau[3].capacite = 90;

    strcpy(tableau[4].nom, "Salle-TP1");
    tableau[4].code = 5;
    strcpy(tableau[4].position, "Premier-etage");
    strcpy(tableau[4].disponibilite_machine, "Pas-de-machines");
    tableau[4].capacite = 30;

    strcpy(tableau[5].nom, "Salle-TP2");
    tableau[5].code = 6;
    strcpy(tableau[5].position, "Premier-etage");
    strcpy(tableau[5].disponibilite_machine, "15-machines");
    tableau[5].capacite = 15;

    strcpy(tableau[6].nom, "Salle-tp-B-ext");
    tableau[6].code = 7;
    strcpy(tableau[6].position, "Premier-etage-batiment-extension");
    strcpy(tableau[6].disponibilite_machine, "24-machines");
    tableau[6].capacite = 24;

    strcpy(tableau[7].nom, "Amphi-3");
    tableau[7].code = 8;
    strcpy(tableau[7].position, "Batiment-principal");
    strcpy(tableau[7].disponibilite_machine, "Pas-de-machines");
    tableau[7].capacite = 108;

    strcpy(tableau[8].nom, "Salle-TP3");
    tableau[8].code = 9;
    strcpy(tableau[8].position, "Premier-etage");
    strcpy(tableau[8].disponibilite_machine, "Pas-de-machines");
    tableau[8].capacite = 30;

    strcpy(tableau[9].nom, "Salle-doctorants");
    tableau[9].code = 10;
    strcpy(tableau[9].position, "Premier-etage");
    strcpy(tableau[9].disponibilite_machine, "Pas-de-machines");
    tableau[9].capacite = 30;

    FILE *fichier = fopen("salles.txt", "w");
    if (fichier == NULL) {
        printf("Impossible d'ouvrir le fichier\n");
        return;
    }

    for (int i = 0; i < 10; i++) {
        fprintf(fichier, "Nom: %s\nCode unique: %d\nPosition: %s\nDisponibilite des machines: %s\nCapacite: %d\n\n",
                tableau[i].nom, tableau[i].code, tableau[i].position,
                tableau[i].disponibilite_machine, tableau[i].capacite);
    }

    fclose(fichier);
    *n = 10;
}

int lireFichierSalles(Salle tableau[]) {
    FILE *fichier = fopen("salles.txt", "r");
    if (fichier == NULL) {
        return 0;
    }

    int i = 0;
    while (!feof(fichier) && i < NOMBRE_SALLES) {
        fscanf(fichier, "Nom: %[^\n]\n", tableau[i].nom);
        fscanf(fichier, "Code unique: %d\n", &tableau[i].code);
        fscanf(fichier, "Position: %[^\n]\n", tableau[i].position);
        fscanf(fichier, "Disponibilite des machines: %[^\n]\n", tableau[i].disponibilite_machine);
        fscanf(fichier, "Capacite: %d\n", &tableau[i].capacite);
        fscanf(fichier, "\n"); // Ligne vide entre les blocs
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
//Fin des fonctions sur les salles
//Fin







/*ces trois fonctions permettrait de comparer les capacites entre une salle
 et une classe au moment d'ajouter une reservation et de verifier
 si une salle a une capacite inferieure à celle d'une classe
 A considerer plutard: le fait de pouvoir ajouter des chaises si la difference
                        des capacites n'est pas enormes.
 */

int capacite_salle(int code_salle) {
    for (int i = 0; i < nb_salles; i++) {
         if (salles[i].code == code_salle) {
            return salles[i].capacite;
        }
    }
    return -1;
}

int effectif_classe(int id_classe) {
    for (int i = 0; i < nb_classes; i++) {
        if (classes[i].classe_id == id_classe) {
            return classes[i].effectif;
        }
    }
    return -1;
}


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


/*La fonction qui suit permet de verifier lors d'un ajout de reservation
  si à une heure donnée par l'utilisateur cette salle est deja reservee sauf les etat annulé ou terminé
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
///Fin etape verification des reservations et horaires

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
        int result = sscanf(ligne, "%d %d %d %9s %d %d \"%99[^\"]\" %d",
                          &reservations[nb_reservations].id,
                          &reservations[nb_reservations].salle,
                          &reservations[nb_reservations].classe,
                          reservations[nb_reservations].jour,
                          &reservations[nb_reservations].heure_debut,
                          &reservations[nb_reservations].duree,
                          reservations[nb_reservations].motif,
                          &reservations[nb_reservations].etat);

        if (result == 8) {
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

    int id_saisi;
    bool id_existe;

    do {
        id_existe = false;
        printf("ID de la réservation (entier unique) : ");
        scanf("%d", &id_saisi);

        for (int i = 0; i < nb_reservations; i++) {
            if (reservations[i].id == id_saisi) {
                printf("Erreur : cet ID existe déjà. Veuillez en choisir un autre.\n");
                id_existe = true;
                break;
            }
        }
    } while (id_existe);

    r.id = id_saisi;

    printf("Code de la salle: ");
    scanf("%d", &r.salle);

    printf("Code de la classe: ");
    scanf("%d", &r.classe);

    int codeClasse;
    bool classeExiste = false;
    do {
        printf("Code de la classe: ");
        scanf("%d", &codeClasse);

        for (int i = 0; i < nb_classes; i++) {
            if (classes[i].classe_id == codeClasse) {
                classeExiste = true;
                break;
            }
        }

        if (!classeExiste) {
            printf("Erreur : cette classe n'existe pas. Veuillez entrer un code valide.\n");
        }
    } while (!classeExiste);

    r.classe = codeClasse;

    printf("Jour: ");
    scanf("%9s", r.jour);

    printf("Heure de début (8-20): ");
    scanf("%d", &r.heure_debut);

    printf("Durée en minutes: ");
    scanf("%d", &r.duree);

    printf("Motif: ");
    scanf(" %99[^\n]", r.motif);

    //Attends code Haroun
    /*if (!verifier_capacite(r.salle, r.classe)) {
        printf("Erreur : la salle ne peut pas accueillir cette classe.\n");
        return;
    }*/

    if (!salle_disponible(r.salle, r.jour, r.heure_debut, r.duree)) {
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
             reservations[i].etat == VALIDEE ||
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
            case 0: printf("Retour au menu principal\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 0);
}



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
                // // //Haroun
                printf("Gestion des classes - Fonctionnalité à venir\n");
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
        initialiserSalles(salles, &nb_salles);
    }

    lirefichierRes();

    menu_principal();

    enregistrer_res();


    return 0;
}