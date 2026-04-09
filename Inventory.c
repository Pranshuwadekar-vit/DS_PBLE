#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITEMS 50
#define NUM_CATEGORIES 3

// Structure for item
typedef struct {
    int itemId;
    char itemName[50];
    int quantity;
} Item;

// Stack structure
typedef struct {
    Item items[MAX_ITEMS];
    int top;
} Stack;

// Initialize stack
void initStack(Stack *s) {
    s->top = -1;
}

// Check full
int isFull(Stack *s) {
    return s->top == MAX_ITEMS - 1;
}

// Check empty
int isEmpty(Stack *s) {
    return s->top == -1;
}

// Add or update item
void push(Stack *s, Item newItem) {
    int i;

    // Check if item exists
    for (i = 0; i <= s->top; i++) {
        if (s->items[i].itemId == newItem.itemId) {
            s->items[i].quantity += newItem.quantity;
            printf("\n[Updated] '%s' quantity = %d\n",
                   s->items[i].itemName,
                   s->items[i].quantity);
            return;
        }
    }

    // New item
    if (isFull(s)) {
        printf("\n[Error] Stock limit reached!\n");
    } else {
        s->top++;
        s->items[s->top] = newItem;
        printf("\n[Success] Item '%s' added with %d units.\n",
               newItem.itemName,
               newItem.quantity);
    }
}

// Delete units with 60% warning
void deleteUnits(Stack *s, int id, int units) {
    int i, foundIndex = -1;

    if (isEmpty(s)) {
        printf("\n[Error] No stock available!\n");
        return;
    }

    // Search item
    for (i = 0; i <= s->top; i++) {
        if (s->items[i].itemId == id) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("\n[Error] Item not found!\n");
        return;
    }

    int currentQty = s->items[foundIndex].quantity;

    if (units > currentQty) {
        printf("\n[Error] Not enough stock! Available = %d\n", currentQty);
        return;
    }

    // ? Warning BEFORE deletion
    if (units > (0.6 * currentQty)) {
        printf("\n[Warning] You are deleting more than 60%% of available stock!\n");
    }

    // Perform deletion
    s->items[foundIndex].quantity -= units;

    printf("\n[Success] %d units removed from '%s'. Remaining = %d\n",
           units,
           s->items[foundIndex].itemName,
           s->items[foundIndex].quantity);

    // Remove item if quantity becomes 0
    if (s->items[foundIndex].quantity == 0) {
        for (i = foundIndex; i < s->top; i++) {
            s->items[i] = s->items[i + 1];
        }
        s->top--;
        printf("[Info] Item completely removed from stock.\n");
    }
}

// Display stock
void displayStock(Stack *s, const char* categoryName) {
    int i;

    if (isEmpty(s)) {
        printf("\n--- %s Stock ---\n", categoryName);
        printf("Stock is empty.\n");
    } else {
        printf("\n--- %s Stock ---\n", categoryName);
        for (i = 0; i <= s->top; i++) {
            printf("ID: %d | Name: %s | Units: %d\n",
                   s->items[i].itemId,
                   s->items[i].itemName,
                   s->items[i].quantity);
        }
        printf("-------------------------------\n");
    }
}

// Select category
int selectCategory() {
    int choice;

    printf("\nSelect Category:\n");
    printf("1. Electronics\n");
    printf("2. Clothing\n");
    printf("3. Groceries\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= 3)
        return choice - 1;
    else {
        printf("Invalid choice!\n");
        return -1;
    }
}

int main() {
    Stack inventory[NUM_CATEGORIES];
    const char* categoryNames[NUM_CATEGORIES] =
        {"Electronics", "Clothing", "Groceries"};

    int i;

    // Initialize stacks
    for (i = 0; i < NUM_CATEGORIES; i++) {
        initStack(&inventory[i]);
    }

    int menuChoice;
    Item tempItem;
    int catIndex;

    while (1) {
        printf("\n=========================================\n");
        printf("   INVENTORY SYSTEM (60%% DELETE ALERT)   \n");
        printf("=========================================\n");
        printf("1. Add Item / Increase Units\n");
        printf("2. Delete Units from Item\n");
        printf("3. Display Stock\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &menuChoice);

        switch (menuChoice) {

            case 1:
                catIndex = selectCategory();
                if (catIndex != -1) {
                    printf("Enter Item ID: ");
                    scanf("%d", &tempItem.itemId);
                    printf("Enter Item Name: ");
                    scanf("%s", tempItem.itemName);
                    printf("Enter Units to Add: ");
                    scanf("%d", &tempItem.quantity);

                    push(&inventory[catIndex], tempItem);
                }
                break;

            case 2:
                catIndex = selectCategory();
                if (catIndex != -1) {
                    int id, units;
                    printf("Enter Item ID: ");
                    scanf("%d", &id);
                    printf("Enter Units to Delete: ");
                    scanf("%d", &units);

                    deleteUnits(&inventory[catIndex], id, units);
                }
                break;

            case 3:
                catIndex = selectCategory();
                if (catIndex != -1) {
                    displayStock(&inventory[catIndex], categoryNames[catIndex]);
                }
                break;

            case 4:
                printf("\nExiting... Goodbye!\n");
                exit(0);

            default:
                printf("\nInvalid choice!\n");
        }
    }

    return 0;
}
