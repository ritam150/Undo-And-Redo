#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct Node {
    char *text;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
} Stack;

void initializeStack(Stack *stack) {
    stack->top = NULL;
}

void push(Stack *stack, const char *text) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->text = strdup(text);
    newNode->next = stack->top;
    stack->top = newNode;
}

char *pop(Stack *stack) {
    if (stack->top == NULL) {
        return NULL;
    }

    Node *temp = stack->top;
    char *text = strdup(temp->text);
    stack->top = temp->next;
    free(temp->text);
    free(temp);

    return text;
}

void undo(Stack *undoStack, Stack *redoStack, char *currentText) {
    char *undoText = pop(undoStack);
    if (undoText != NULL) {
        push(redoStack, currentText);
        strcpy(currentText, undoText);
        free(undoText);
    }
}

void redo(Stack *undoStack, Stack *redoStack, char *currentText) {
    char *redoText = pop(redoStack);
    if (redoText != NULL) {
        push(undoStack, currentText);
        strcpy(currentText, redoText);
        free(redoText);
    }
}

void displayText(const char *text) {
    printf("Current Text: %s\n", text);
}

int main() {
    Stack undoStack, redoStack;
    initializeStack(&undoStack);
    initializeStack(&redoStack);

    char currentText[MAX_SIZE];
    char userInput[MAX_SIZE];

    printf("Enter initial text: ");
    fgets(currentText, MAX_SIZE, stdin);
    currentText[strcspn(currentText, "\n")] = '\0';

    while (1) {
        displayText(currentText);

        printf("Enter text to append (or 'undo', 'redo', or 'exit'): ");
        fgets(userInput, MAX_SIZE, stdin);
        userInput[strcspn(userInput, "\n")] = '\0';

        if (strcmp(userInput, "undo") == 0) {
            undo(&undoStack, &redoStack, currentText);
        } else if (strcmp(userInput, "redo") == 0) {
            redo(&undoStack, &redoStack, currentText);
        } else if (strcmp(userInput, "exit") == 0) {
            break;
        } else {
            push(&undoStack, currentText);
            strcat(currentText, userInput);
        }
    }

    // Clean up: free memory used by the stacks
    while (pop(&undoStack) != NULL)
        ;
    while (pop(&redoStack) != NULL)
        ;

    return 0;
}
