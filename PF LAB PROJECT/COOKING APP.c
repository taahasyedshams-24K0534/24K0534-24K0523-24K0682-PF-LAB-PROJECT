#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Ingredients {
    char iname[100];
    char quantity[50];
};

struct Recipes {
    char name[100];
    int icount;
    struct Ingredients ingredient[100];
    char instructions[1000];
    char notes[500];
};

struct Notes {
    char title[100];
    char stuff[500];
    char image[100];
};

void addrecipe() {
    struct Recipes recipe;
    FILE *file = fopen("SavedRecipes.txt", "a");

    if (file == NULL) {
        printf("Error opening file !!!!! \n");
        return;
    }

    printf("***** ADD NEW RECIPE *****\n");
    printf("1. Enter Recipe Name: ");
    fgets(recipe.name, sizeof(recipe.name), stdin);
    recipe.name[strcspn(recipe.name, "\n")] = '\0';

    printf("2. Number of Ingredients: ");
    scanf("%d", &recipe.icount);
    getchar();

    for (int i = 0; i < recipe.icount; i++) {
        printf("  -Enter Ingredient %d: ", i + 1);
        fgets(recipe.ingredient[i].iname, sizeof(recipe.ingredient[i].iname), stdin);
        recipe.ingredient[i].iname[strcspn(recipe.ingredient[i].iname, "\n")] = '\0';

        printf("  -Enter Quantity of Ingredient %d: ", i + 1);
        fgets(recipe.ingredient[i].quantity, sizeof(recipe.ingredient[i].quantity), stdin);
        recipe.ingredient[i].quantity[strcspn(recipe.ingredient[i].quantity, "\n")] = '\0';
    }

    printf("3. Enter Recipe Instructions: ");
    fgets(recipe.instructions, sizeof(recipe.instructions), stdin);
    recipe.instructions[strcspn(recipe.instructions, "\n")] = '\0';

    printf("4. Enter Notes for Recipe: ");
    fgets(recipe.notes, sizeof(recipe.notes), stdin);
    recipe.notes[strcspn(recipe.notes, "\n")] = '\0';

    fprintf(file, "Name: %s\n", recipe.name);
    fprintf(file, "Ingredients and Quantity:- \n");
    for (int i = 0; i < recipe.icount; i++) {
        fprintf(file, "%s:%s\n", recipe.ingredient[i].iname, recipe.ingredient[i].quantity);
    }
    fprintf(file, "Instructions: %s\nNotes: %s\n \n", recipe.instructions, recipe.notes);

    fclose(file);
    printf("***** RECIPE ADDED *****\n");
}

void displayrecipe() {
    FILE *file = fopen("SavedRecipes.txt", "r");
    if (file == NULL) {
        printf("error opening file !!!!! \n");
        return;
    }
    char line[1000];
    int printLine = 0;

    printf("***** RECIPES FOR YOU *****\n");

    while (fgets(line, 1000, file)) {
        printf("%s", line);
        if (strstr(line, "Notes:") != NULL) {
            printLine = 1;
        }
        if (printLine) {
            printf("\n ------------------------------------ \n");
            printLine = 0;
        }
    }
    fclose(file);
}

void searchsavedrecipe() {
    char searchName[100];
    char line[1000];
    int found = 0;

    printf("\n***** SEARCH FOR A RECIPE *****\n");
    printf("Enter the recipe name to search: ");
    getchar();
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    FILE *file = fopen("SavedRecipes.txt", "r");
    if (file == NULL) {
        printf("error opening file !!!!!\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Name:") && strstr(line, searchName)) {
            found = 1;
            printf("\n***** RECIPE FOUND *****\n");
            printf("%s", line);
            while (fgets(line, sizeof(line), file)) {
                if (strcmp(line, " \n") == 0) {
                    break;
                }
                printf("%s", line);
            }
            break;
        }
    }

    if (!found) {
        printf("\nRecipe with name '%s' not found.\n", searchName);
    }

    fclose(file);
}

void addnote() {
    struct Notes note1;
    FILE *file = fopen("Notes.txt", "a");
    if (file == NULL) {
        printf("error opening file !!!!!");
        return;
    }
    printf("\n***** Add New Note *****\n");
    printf("1. Enter Note Title: ");
    scanf(" %[^\n]", note1.title);
    printf("2. Enter Your Notes: ");
    scanf(" %[^\n]", note1.stuff);
    printf("3. Enter Image Path (optional, press N to skip): ");
    scanf(" %[^\n]", note1.image);

    fprintf(file, "%s \n%s \n%s \n", note1.title, note1.stuff, note1.image);

    fclose(file);
    printf("*****NOTE ADDED SUCCESSFULLY*****\n");
}

void displaynote() {
    FILE *file = fopen("Notes.txt", "r");
    if (file == NULL) {
        printf("error opening file !!!!!\n");
        return;
    }
    char line[500];
    int printLine = 0;

    printf("\n***** NOTES *****\n");

    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
        if (strstr(line, "Notes:") != NULL) {
            printLine = 1;
        }
        if (printLine) {
            printf("\n--------------------\n");
            printLine = 0;
        }
    }

    fclose(file);
}

void find_recipes_with_ingredients() {
    char line[256];
    char *user_ingredients[10];
    int num_ingredients = 0;

    printf("Enter your ingredients (type 'done' to finish):\n");
    while (num_ingredients < 10) {
        char ingredient[50];
        printf("Ingredient %d: ", num_ingredients + 1);
        scanf("%s", ingredient);
        if (strcmp(ingredient, "done") == 0) {
            break;
        }
        user_ingredients[num_ingredients] = strdup(ingredient);
        num_ingredients++;
    }

    FILE *file = fopen("recipes.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        for (int i = 0; i < num_ingredients; i++) {
            free(user_ingredients[i]);
        }
        return;
    }

    printf("\nRecipes you can make:\n");
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        int all_ingredients_found = 1;
        for (int i = 0; i < num_ingredients; i++) {
            if (strstr(line, user_ingredients[i]) == NULL) {
                all_ingredients_found = 0;
                break;
            }
        }
        if (all_ingredients_found) {
            printf("- %s", line);
            found = 1;
        }
    }

    if (!found) {
        printf("No recipes match your ingredients.\n");
    }

    fclose(file);

    for (int i = 0; i < num_ingredients; i++) {
        free(user_ingredients[i]);
    }
}

int main() {
    int choice;
    do {
        printf("\n***** Cooking App Menu *****\n");
        printf("1. Add Recipe\n");
        printf("2. Display Recipes\n");
        printf("3. Add Note\n");
        printf("4. Display Notes\n");
        printf("5. Search Saved Recipe\n");
        printf("6. Recipe Suggestions\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                addrecipe();
                break;
            case 2:
                displayrecipe();
                break;
            case 3:
                addnote();
                break;
            case 4:
                displaynote();
                break;
            case 5:
                searchsavedrecipe();
                break;
            case 6:
                find_recipes_with_ingredients();
                break;
            case 7:
                printf("\n***** Exiting the app *****\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}
