#pragma warning(disable : 4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Définition des constantes avec enum
enum {
    MAX_ETUDIANTS = 100,
    MAX_NOM = 30,
    MAX_ABSENCES = 100,
    DEMI_JOURNEE = 3,
    NOMBRE_MAX_J_SEMESTRE = 40,
    TAILLE_MAX_COMMANDE = 31,
    MAX_JUSTIF = 50,
    VERDICT = 3,
};

// Définition de la structure Etudiant
typedef struct {
    char nom[MAX_NOM];
    int groupe;
    int id_etu;
} Etudiant;

// Définition de la structure Absence
typedef struct {
    int id_etu;  // Identifiant de l'étudiant
    int jour;    // Jour d'absence (1-40)
    char demi_journee[DEMI_JOURNEE]; // "am" ou "pm"
    int id_abs;
} Absence;


// Définition de la structure Justificatif
typedef struct {
    int id_abs;
    int jour;
    char motif[MAX_JUSTIF];
    bool valide;
    char verdict[VERDICT];
}Justificatif;



# define OK = 0,
# define KO = 1,


//-----------------------------------------------------------------------------------------------------C1------------------------------------------------------------------------------------------------------------//

// Vérification de l'existence d'un étudiant
int verification(Etudiant etudiants[MAX_ETUDIANTS], int nbEtudiants, char nom[], int groupe) {
    for (int i = 0; i < nbEtudiants; ++i) {
        if (strcmp(etudiants[i].nom, nom) == 0 && etudiants[i].groupe == groupe) {
            printf("Nom incorrect\n");
            return 0; // L'étudiant existe déjà
        }
    }
    return 1; // Étudiant non trouvé
}

// Fonction d'inscription d'un étudiant
void inscription(Etudiant etudiants[], int* nbEtudiants) {
    char nom[MAX_NOM];
    int groupe;

    scanf("%s %d", nom, &groupe);

    if (verification(etudiants, *nbEtudiants, nom, groupe)) {
        strcpy(etudiants[*nbEtudiants].nom, nom);
        etudiants[*nbEtudiants].groupe = groupe;
        etudiants[*nbEtudiants].id_etu = *nbEtudiants + 1;  // Ajout d'un identifiant unique
        (*nbEtudiants)++;
        printf("Inscription realisee (%d)\n", *nbEtudiants);
    }
}

//-----------------------------------------------------------------------------------------------------C2------------------------------------------------------------------------------------------------------------//


// Vérification d'une absence
int verification_abs(Absence absences[], int nbAbsences, int id_etu, int jour, char* demi_journee, int nbEtudiants) {
    if (id_etu < 1 || id_etu > nbEtudiants) {
        printf("Identifiant incorrect\n");
        return 0; // Identifiant incorrect
    }
    if (jour < 1 || jour > 40) {
        printf("Date incorrecte\n");
        return 0; // Date incorrecte
    }
    if (strcmp(demi_journee, "am") != 0 && strcmp(demi_journee, "pm") != 0) {
        printf("Demi-journee incorrecte\n");
        return 0; // Demi-journée incorrecte
    }

    for (int i = 0; i < nbAbsences; ++i) {
        if (absences[i].id_etu == id_etu && absences[i].jour == jour && strcmp(absences[i].demi_journee, demi_journee) == 0) {
            printf("Absence deja connue\n");
            return 0; // Absence déjà enregistrée
        }
    }

    return 1;
}


// Enregistrement d'une absence
void absence_enregistrement(Absence absences[], int* nbAbsences, int nbEtudiants) {
    int id_etu, jour;
    char demi_journee[DEMI_JOURNEE];

    scanf("%d %d %s", &id_etu, &jour, demi_journee);

    if (verification_abs(absences, *nbAbsences, id_etu, jour, demi_journee, nbEtudiants)) {
        absences[*nbAbsences].id_abs = *nbAbsences + 1;
        absences[*nbAbsences].id_etu = id_etu;
        absences[*nbAbsences].jour = jour;
        strcpy(absences[*nbAbsences].demi_journee, demi_journee);
        (*nbAbsences)++;
        printf("Absence enregistree [%d]\n", *nbAbsences);
    }
}


//-----------------------------------------------------------------------------------------------------C3------------------------------------------------------------------------------------------------------------//


// Trier les étudiants par groupe
void trierEtudiantsParGroupe(Etudiant etudiants[], int nbEtudiants) {
    for (int i = 0; i < nbEtudiants - 1; ++i) {
        for (int j = 0; j < nbEtudiants - i - 1; ++j) {
            if (etudiants[j].groupe > etudiants[j + 1].groupe) {
                Etudiant temp = etudiants[j];
                etudiants[j] = etudiants[j + 1];
                etudiants[j + 1] = temp;
            }
        }
    }
}

// Trier les étudiants par nom
void trierEtudiantsParNom(Etudiant etudiants[], int nbEtudiants) {
    for (int i = 0; i < nbEtudiants - 1; ++i) {
        for (int j = 0; j < nbEtudiants - i - 1; ++j) {
            if (strcmp(etudiants[j].nom, etudiants[j + 1].nom) > 0) {
                Etudiant temp = etudiants[j];
                etudiants[j] = etudiants[j + 1];
                etudiants[j + 1] = temp;
            }
        }
    }
}

// Afficher la liste des étudiants et leurs absences
// Afficher la liste des étudiants triée par groupe puis par nom
void afficherEtudiants(int jour, Etudiant etudiants[], Absence absences[], int nbEtudiants, int nbAbsences) {
    if (jour < 1) {
        printf("Date incorrecte\n");
        return;
    }

    if (nbEtudiants == 0) {
        printf("Aucun inscrit\n");
        return;
    }

    // Trier les étudiants par groupe
    trierEtudiantsParGroupe(etudiants, nbEtudiants);

    // Trier les étudiants par nom au sein de chaque groupe
    int debutGroupe = 0;
    for (int i = 1; i <= nbEtudiants; ++i) {
        // Si on a atteint la fin du groupe ou la fin de la liste d'étudiants
        if (i == nbEtudiants || etudiants[i].groupe != etudiants[debutGroupe].groupe) {
            trierEtudiantsParNom(&etudiants[debutGroupe], i - debutGroupe);  // Tri par nom des étudiants du groupe
            debutGroupe = i;  // Passer au groupe suivant
        }
    }

    // Afficher les étudiants et leurs absences
    for (int i = 0; i < nbEtudiants; ++i) {
        int totalAbsencesAceJour = 0;

        for (int j = 0; j < nbAbsences; ++j) {
            if (absences[j].id_etu == etudiants[i].id_etu && absences[j].jour <= jour) {
                totalAbsencesAceJour++;
            }
        }

        // Afficher les informations de l'étudiant
        printf("(%d) %s %d %d\n", etudiants[i].id_etu, etudiants[i].nom, etudiants[i].groupe, totalAbsencesAceJour);
    }
}


//-----------------------------------------------------------------------------------------------------C4------------------------------------------------------------------------------------------------------------//

void depot_justificatif(Absence absences[], int nbAbsences, Justificatif justificatifs[], int* nbJustificatifs) {
    int id_abs, jour_justif;
    char motif[MAX_JUSTIF];

    // Saisie des données
    scanf("%d %d ", &id_abs, &jour_justif);
    fgets(motif, MAX_JUSTIF, stdin);  // Permet de saisir la chaîne avec des espaces
    motif[strlen(motif) - 1] = '\0';

    int absenceTrouvee = 0;

    // Parcourir les absences pour trouver l'absence correspondante par ID d'absence
    for (int i = 0; i < nbAbsences; ++i) {
        if (absences[i].id_abs == id_abs) {  // Utilisation de id_absence ici
            absenceTrouvee = 1;

            // Vérifier si le jour du justificatif est avant le jour de l'absence
            if (jour_justif < absences[i].jour) {
                printf("Date Incorrecte\n");
                return;
            }

            // Vérifier si le justificatif existe déjà
            for (int j = 0; j < *nbJustificatifs; ++j) {
                if (justificatifs[j].id_abs == id_abs) {
                    printf("Justificatif deja connu\n");
                    return;
                }
            }

            // Si tout est correct, on enregistre le justificatif
            justificatifs[*nbJustificatifs].id_abs = id_abs;
            justificatifs[*nbJustificatifs].jour = jour_justif;
            strcpy(justificatifs[*nbJustificatifs].motif, motif);
            (*nbJustificatifs)++;
            printf("Justificatif enregistre\n");
            return;
        }
    }

    // Si aucune absence n'a été trouvée
    if (!absenceTrouvee) {
        printf("Identifiant incorrect\n");
    }
}

//-----------------------------------------------------------------------------------------------------C5------------------------------------------------------------------------------------------------------------//
void permutationAbsences(Absence* a, Absence* b) {
    Absence temp = *a;
    *a = *b;
    *b = temp;
}

// Fonction de tri des validations
void tri_validations(Absence absences[], const int nombre_absences) {
    for (int i = 0; i < nombre_absences - 1; ++i) {
        for (int j = 0; j < nombre_absences - i - 1; ++j) {
            // Trier par id_etudiant (ordre croissant)
            if (absences[j].id_etu > absences[j + 1].id_etu) {
                permutationAbsences(&absences[j], &absences[j + 1]);
            }
            // Si les id_etudiant sont égaux, trier par jour (ordre croissant)
            else if (absences[j].id_etu == absences[j + 1].id_etu &&
                absences[j].jour > absences[j + 1].jour) {
                permutationAbsences(&absences[j], &absences[j + 1]);
            }
            // Si les id_etudiant et jour sont égaux, trier par demi_journee (ordre alphabétique croissant)
            else if (absences[j].id_etu == absences[j + 1].id_etu &&
                absences[j].jour == absences[j + 1].jour &&
                strcmp(absences[j].demi_journee, absences[j + 1].demi_journee) > 0) {
                permutationAbsences(&absences[j], &absences[j + 1]);
            }
        }
    }
}

void validations(Absence absences[], Justificatif justificatifs[], Etudiant etudiants[], int nbAbsences, int nbEtudiants, int nbJustificatifs) {
    int validationTrouvee = 0; // Drapeau pour détecter si des validations ont été trouvées

    for (int i = 0; i < nbAbsences; i++) {
        for (int j = 0; j < nbJustificatifs; j++) {
            if (absences[i].id_abs == justificatifs[j].id_abs && absences[i].jour - justificatifs[j].jour >= -3) {
                for (int k = 0; k < nbEtudiants; k++) {
                    if (etudiants[k].id_etu == absences[i].id_etu) {
                        // Si une validation est trouvée, on affiche et on met à jour le drapeau
                        printf("[%d] (%d) %s %d %d/%s (%s)\n",
                            absences[i].id_abs,
                            absences[i].id_etu,
                            etudiants[k].nom,
                            etudiants[k].groupe,
                            absences[i].jour,
                            absences[i].demi_journee,
                            justificatifs[j].motif);

                        validationTrouvee = 1; // Validation trouvée
                    }
                }
            }
        }
    }

    // Si aucune validation n'a été trouvée, on affiche le message correspondant
    if (!validationTrouvee) {
        printf("Aucune validation en attente\n");
    }
}





//-----------------------------------------------------------------------------------------------------C6------------------------------------------------------------------------------------------------------------//

void validation_justif(Absence absences[], int nbAbsences, Justificatif justificatifs[], int nbJustificatifs) {
    int id_abs;
    char verdict[VERDICT];

    // Lire l'entrée
    scanf("%d %s", &id_abs, verdict);

    // Vérifier si le verdict est "ok" ou "ko"
    if (strcmp(verdict, "ok") != 0 && strcmp(verdict, "ko") != 0) {
        printf("Code incorrect\n");
        return;
    }

    // Parcourir les absences pour trouver l'absence correspondante
    for (int i = 0; i < nbAbsences; ++i) {
        if (absences[i].id_abs == id_abs) {
            // Vérification si la validation existe déjà
            for (int j = 0; j < nbJustificatifs; j++) {
                if (justificatifs[j].id_abs == id_abs) {
                    if (absences[i].jour - justificatifs[j].jour <= -3) {
                        printf("Identifiant incorrect\n");
                        return;
                    }
                    if (justificatifs[j].valide) {
                        printf("Validation deja connue\n");
                        return;
                    }

                    justificatifs[j].valide = true;
                    strcpy(justificatifs[j].verdict, verdict);
                    printf("Validation enregistree\n");
                    return;
                }
            }
        }
        else if (absences[i].id_abs != id_abs && i == nbAbsences - 1) {
            printf("Identifiant incorrect\n");
        }
    }
}

// Fonction principale
int main() {
    Etudiant etudiants[MAX_ETUDIANTS];
    Absence absences[MAX_ABSENCES];
    Justificatif justificatifs[MAX_JUSTIF];
    int nbEtudiants = 0;
    int nbAbsences = 0;
    int nbJustificatifs = 0;
    char commande[TAILLE_MAX_COMMANDE];
    int jour;

    do {
        scanf("%s", commande);

        if (strcmp(commande, "exit") == 0) {
            break;
        }

        if (strcmp(commande, "inscription") == 0) {
            inscription(etudiants, &nbEtudiants);
        }

        if (strcmp(commande, "absence") == 0) {
            absence_enregistrement(absences, &nbAbsences, nbEtudiants);
        }

        if (strcmp(commande, "etudiants") == 0) {
            scanf("%d", &jour);
            afficherEtudiants(jour, etudiants, absences, nbEtudiants, nbAbsences);
        }

        if (strcmp(commande, "justificatif") == 0) {
            depot_justificatif(absences, nbAbsences, justificatifs, &nbJustificatifs);
        }

        if (strcmp(commande, "validations") == 0) {
            validations(absences, justificatifs, etudiants, nbAbsences, nbEtudiants, nbJustificatifs);
        }

        if (strcmp(commande, "validation") == 0) {
            validation_justif(absences, nbAbsences, justificatifs, nbJustificatifs);

        }

    } while (strcmp(commande, "exit") != 0);

    return 0;
}