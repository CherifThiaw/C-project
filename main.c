#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_RESERVATIONS 100
#define MAX_CODE_LENGTH 10

// #define NOMBRE_SALLES 10
// #define NOMBRE_CLASSES 10

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
    char salle[100];
    char classe[10];
    char jour[10];
    int heure_debut;
    int duree;
    char motif[100];
    EtatReservation etat;
} Reservation;

// typedef struct {
//     char salle_id[10];
//     char batiment[50];
//     int capacite;
//     bool avec_machines;
// } Salle;

// typedef struct {
//     char classe_id[10];
//     char classe_nom[50];
//     char niveau[20];
//     int effectif;
// } Classe;

// Salle salles[NOMBRE_SALLES_FIXE];
// Classe classes[NOMBRE_CLASSES_FIXE];

Reservation reservations[100];
int nb_reservations = 0;

// void charger_salles(const char* fichier) {
//     FILE* f = fopen(fichier, "r");
//     if (!f) {
//         printf("Erreur: impossible d'ouvrir %s\n", fichier);
//         return;
//     }

//     for (int i = 0; i < NOMBRE_SALLES_FIXE; i++) {
//         fscanf(f, "%s %s %d %d",
//                salles[i].salle_id,
//                salles[i].batiment,
//                &salles[i].capacite,
//                (int*)&salles[i].avec_machines);
//     }
//     fclose(f);
// }

// void charger_classes(const char* fichier) {
//     FILE* f = fopen(fichier, "r");
//     if (!f) {
//         printf("Erreur: impossible d'ouvrir %s\n", fichier);
//         return;
//     }

//     for (int i = 0; i < NOMBRE_CLASSES_FIXE; i++) {
//         fscanf(f, "%s %s %s %d",
//                classes[i].classe_id,
//                classes[i].classe_nom,
//                classes[i].niveau,
//                &classes[i].effectif);
//     }
//     fclose(f);
// }

// int trouver_capacite_salle(const char* id_salle) {
//     for (int i = 0; i < NOMBRE_SALLES_FIXE; i++) {
//         if (strcmp(salles[i].salle_id, id_salle) == 0) {
//             return salles[i].capacite;
//         }
//     }
//     return -1;
// }

// int trouver_effectif_classe(const char* id_classe) {
//     for (int i = 0; i < NOMBRE_CLASSES_FIXE; i++) {
//         if (strcmp(classes[i].classe_id, id_classe) == 0) {
//             return classes[i].effectif;
//         }
//     }
//     return -1;
// }

// bool verifier_capacite(const char* id_salle, const char* id_classe) {
//     int capacite = trouver_capacite_salle(id_salle);
//     int effectif = trouver_effectif_classe(id_classe);

//     if (capacite == -1 || effectif == -1) {
//         printf("Erreur : salle ou classe introuvable\n");
//         return false;
//     }
//     if (capacite >= effectif) {
//         return true;
//     }
//     printf("Erreur : capacité insuffisante (%d < %d)\n", capacite, effectif);
//     return false;
// }

bool salle_disponible(const char* salle, const char* jour, int heure_debut, int duree) {
    int heure_fin = heure_debut + (duree / 60);

    for (int i = 0; i < nb_reservations; i++) {
        if (reservations[i].etat == ANNULEE || reservations[i].etat == TERMINEE) continue;

        if (strcmp(reservations[i].salle, salle) == 0 &&
            strcmp(reservations[i].jour, jour) == 0) {
            int fin = reservations[i].heure_debut + (reservations[i].duree / 60);

            if (heure_debut < fin && heure_fin > reservations[i].heure_debut) {
                printf("Probleme avec votre réservation : %dh-%dh\n",
                       reservations[i].heure_debut, fin);
                return false;
            }
        }
    }
    return true;
}

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

void ajouter_reservation() {
    if (nb_reservations >= MAX_RESERVATIONS) {
        printf("Erreur: Nombre maximum de réservations atteint.\n");
        return;
    }

    Reservation r;
    printf("\nAjout d'une nouvelle réservation \n");

    int id_saisi;
    bool id_existe;
    int i;

    do {
        id_existe = false;
        printf("ID de la réservation (entier unique) : ");
        scanf("%d", &id_saisi);

        for (i = 0; i < nb_reservations; i++) {
            if (reservations[i].id == id_saisi) {
                printf("Erreur : cet ID existe déjà. Veuillez en choisir un autre.\n");
                id_existe = true;
                break;
            }
        }
    } while (id_existe);

    r.id = id_saisi;

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

    // if (!verifier_capacite_suffisante(r.salle, r.classe)) {
    //     printf("Erreur : la salle ne peut pas accueillir cette classe.\n");
    //     return;
    // }

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
    // charger_salles("salles.txt");
    // charger_classes("classes.txt");
    menu_reservations();
    sauvegarder();
    return 0;
}
