#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_RESERVATIONS 100
#define MAX_CODE_LENGTH 10

#define FICHIER_SAUVEGARDE "/Users/bayeabdoulaye/Documents/reservations_salles.txt"

// États possibles d'une réservation
typedef enum {
    RESERVEE,
    ANNULEE,
    VALIDEE,
    EN_COURS,
    TERMINEE
} EtatReservation;


typedef struct {
    int id;
    char salle[100];
    char classe[10];
    char jour[10];
    int heure_debut;
    int duree; // en minutes
    char motif[100];
    EtatReservation etat;
} Reservation;

// Tableau pour stocker les réservations
Reservation reservations[100];
int nb_reservations = 0;

// Procédures de base

void sauvegarder() {
    FILE *f = fopen(FICHIER_SAUVEGARDE, "w");
    if (!f) {
        printf("Erreur: impossible de créer %s\n", FICHIER_SAUVEGARDE);
        return;
    }

    for (int i = 0; i < nb_reservations; i++) {

        fprintf(f, "%d %s %s %s %d %d \"%s\" %d\n",
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
    printf("Sauvegardé dans %s\n", FICHIER_SAUVEGARDE);
}

void charger() {
    FILE *f = fopen(FICHIER_SAUVEGARDE, "r");
    if (!f) {
        printf("Aucune donnée existante (fichier: %s)\n", FICHIER_SAUVEGARDE);
        return;
    }

    nb_reservations = 0;
    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f) != NULL){

        int result = sscanf(ligne, "%d %9s %9s %9s %d %d \"%99[^\"]\" %d",
                          &reservations[nb_reservations].id,
                          reservations[nb_reservations].salle,
                          reservations[nb_reservations].classe,
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

bool salle_disponible(const char* salle, const char* jour, int heure_debut, int duree) {
    int heure_fin = heure_debut + (duree / 60);

    for (int i = 0; i < nb_reservations; i++) {
        // On ne vérifie que les réservations confirmées/en cours
        if (reservations[i].etat == ANNULEE || reservations[i].etat == TERMINEE) {
            continue;
        }

        // Même salle et même jour
        if (strcmp(reservations[i].salle, salle) == 0 &&
            strcmp(reservations[i].jour, jour) == 0) {
            int fin;
            fin = reservations[i].heure_debut + (reservations[i].duree / 60);

            // Vérification du chevauchement
            if (heure_debut < fin && heure_fin > reservations[i].heure_debut) {
                printf("Probleme avec votre réservation : %dh-%dh\n",
                      reservations[i].heure_debut, fin);
                return false;
            }
            }
    }
    return true;
}











void ajouter_reservation() {
    if (nb_reservations >= MAX_RESERVATIONS) {
        printf("Erreur: Nombre maximum de réservations atteint.\n");
        return;
    }

    Reservation r;

    printf("\nAjout d'une nouvelle réservation \n");

    r.id = nb_reservations + 1;

    printf("Code de la salle: ");
    scanf("%9s", r.salle);

    printf("Code de la classe: ");
    scanf("%9s", r.classe);

    printf("Jour: ");
    scanf("%9s", r.jour);

    printf("Heure de début (8-20): ");
    scanf("%d", &r.heure_debut);

    printf("Durée en minutes: ");
    scanf("%d", &r.duree);

    printf("Motif: ");
    scanf(" %99[^\n]", r.motif);

    if (!salle_disponible(r.salle, r.jour, r.heure_debut, r.duree)) {
        printf("Erreur: La salle n'est pas disponible\n");
        return;
    }

    r.etat = RESERVEE;
    reservations[nb_reservations++] = r;
    sauvegarder();
    printf("Réservation ajoutée avec succès (ID: %d)\n", r.id);
}

void afficher_reservations() {
    printf("\nListe des réservations (%d) \n", nb_reservations);

    for (int i = 0; i < nb_reservations; i++) {
        printf("ID: %d | Salle: %s | Classe: %s\n",
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

void modifier_etat_reservation() {
    int id;
    int nouvel_etat;

    printf("\n--- Modifier l'état d'une réservation ---\n");
    printf("ID de la réservation: ");
    scanf("%d", &id);

    // Recherche de la réservation
    bool trouve = false;
    for (int i = 0; i < nb_reservations; i++) {
        if (reservations[i].id == id) {
            trouve = true;

            printf("Nouvel état (0=Réservée, 1=Annulée, 2=Validée, 3=En cours, 4=Terminée): ");
            scanf("%d", &nouvel_etat);

            if (nouvel_etat >= 0 && nouvel_etat <= 4) {
                reservations[i].etat = nouvel_etat;
                sauvegarder();
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

// Sauvegarde ultra-simple




// Menu principal
void menu_reservations() {
    int choix;

    do {
        printf("\nGESTION DES RESERVATIONS \n");
        printf("1. Ajouter une réservation\n");
        printf("2. Afficher toutes les réservations\n");
        printf("3. Modifier l'état d'une réservation\n");
        printf("0. Quitter\n");
        printf("Votre choix: ");
        scanf("%d", &choix);

        switch(choix) {
            case 1: ajouter_reservation(); break;
            case 2: afficher_reservations(); break;
            case 3: modifier_etat_reservation(); break;
            case 0: printf("Fin\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 0);
}

int main() {
    charger();
    menu_reservations();
    sauvegarder();
    return 0;
}