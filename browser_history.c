/*
 * Browser History Management using Stacks
 *
 * Two stacks model browser navigation:
 *   - back stack    : pages you can go back to
 *   - forward stack : pages you can go forward to
 *   - current       : the page being viewed
 *
 * Visit new page : push current -> back, clear forward, set current = new
 * Back           : push current -> forward, pop back -> current
 * Forward        : push current -> back, pop forward -> current
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_URL 256

typedef struct Node {
    char url[MAX_URL];
    struct Node *next;
} Node;

typedef struct {
    Node *top;
    int size;
} Stack;

typedef struct {
    Stack back;
    Stack forward;
    char current[MAX_URL];
    int has_current;
} Browser;

/* ---------- Stack operations ---------- */

void stack_init(Stack *s) {
    s->top = NULL;
    s->size = 0;
}

int stack_is_empty(const Stack *s) {
    return s->top == NULL;
}

int stack_push(Stack *s, const char *url) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "Error: memory allocation failed.\n");
        return 0;
    }
    strncpy(n->url, url, MAX_URL - 1);
    n->url[MAX_URL - 1] = '\0';
    n->next = s->top;
    s->top = n;
    s->size++;
    return 1;
}

int stack_pop(Stack *s, char *out) {
    if (stack_is_empty(s)) return 0;
    Node *n = s->top;
    strcpy(out, n->url);
    s->top = n->next;
    free(n);
    s->size--;
    return 1;
}

void stack_clear(Stack *s) {
    char tmp[MAX_URL];
    while (stack_pop(s, tmp)) { }
}

/* Print stack from top (most recent) to bottom */
void stack_print(const Stack *s) {
    const Node *n = s->top;
    if (!n) {
        printf("   (empty)\n");
        return;
    }
    int i = 1;
    while (n) {
        printf("   %d. %s\n", i++, n->url);
        n = n->next;
    }
}

/* ---------- Browser operations ---------- */

void browser_init(Browser *b) {
    stack_init(&b->back);
    stack_init(&b->forward);
    b->current[0] = '\0';
    b->has_current = 0;
}

void browser_visit(Browser *b, const char *url) {
    if (b->has_current) {
        if (!stack_push(&b->back, b->current)) return;
    }
    stack_clear(&b->forward);   /* new visit invalidates forward history */
    strncpy(b->current, url, MAX_URL - 1);
    b->current[MAX_URL - 1] = '\0';
    b->has_current = 1;
    printf("Visited: %s\n", b->current);
}

void browser_back(Browser *b) {
    if (stack_is_empty(&b->back)) {
        printf("No previous page. Cannot go back.\n");
        return;
    }
    stack_push(&b->forward, b->current);
    stack_pop(&b->back, b->current);
    printf("Back to: %s\n", b->current);
}

void browser_forward(Browser *b) {
    if (stack_is_empty(&b->forward)) {
        printf("No next page. Cannot go forward.\n");
        return;
    }
    stack_push(&b->back, b->current);
    stack_pop(&b->forward, b->current);
    printf("Forward to: %s\n", b->current);
}

void browser_show_current(const Browser *b) {
    if (!b->has_current)
        printf("No page open.\n");
    else
        printf("Current page: %s\n", b->current);
}

void browser_show_history(const Browser *b) {
    printf("\n--- Browser History ---\n");
    printf("Back stack (%d), most recent first:\n", b->back.size);
    stack_print(&b->back);
    printf("Current page:\n   %s\n", b->has_current ? b->current : "(none)");
    printf("Forward stack (%d), next page first:\n", b->forward.size);
    stack_print(&b->forward);
    printf("-----------------------\n");
}

void browser_clear(Browser *b) {
    stack_clear(&b->back);
    stack_clear(&b->forward);
    b->current[0] = '\0';
    b->has_current = 0;
    printf("History cleared.\n");
}

/* ---------- Input helpers ---------- */

void read_line(char *buf, size_t size) {
    if (fgets(buf, (int)size, stdin)) {
        buf[strcspn(buf, "\r\n")] = '\0';
    } else {
        buf[0] = '\0';
    }
}

void print_menu(void) {
    printf("\n===== Browser History Manager =====\n");
    printf("1. Visit a new page\n");
    printf("2. Back\n");
    printf("3. Forward\n");
    printf("4. Show current page\n");
    printf("5. Show full history\n");
    printf("6. Clear history\n");
    printf("0. Exit\n");
    printf("Enter choice: ");
}

int main(void) {
    Browser browser;
    char input[MAX_URL];
    int choice;

    browser_init(&browser);

    while (1) {
        print_menu();
        read_line(input, sizeof(input));
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter URL: ");
                read_line(input, sizeof(input));
                if (input[0] == '\0')
                    printf("URL cannot be empty.\n");
                else
                    browser_visit(&browser, input);
                break;
            case 2: browser_back(&browser); break;
            case 3: browser_forward(&browser); break;
            case 4: browser_show_current(&browser); break;
            case 5: browser_show_history(&browser); break;
            case 6: browser_clear(&browser); break;
            case 0:
                browser_clear(&browser);
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
