# Browser History Management Using Stack (C)

A console-based C program that simulates a web browser's back/forward navigation using two stacks implemented as linked lists.

## How it works

| Action | Effect |
|--------|--------|
| Visit | Push current page onto the **back stack**, clear the **forward stack**, set new page as current |
| Back | Push current onto **forward stack**, pop **back stack** into current |
| Forward | Push current onto **back stack**, pop **forward stack** into current |

All stack operations (push, pop, is-empty, clear) run in O(1) except clear, which is O(n).

## Features

- Visit a new URL
- Back and Forward navigation
- View current page
- View full history (back stack, current, forward stack)
- Clear history
- Dynamic memory (linked-list stack), freed on exit

## Build and run

```bash
make run
```

Or manually:

```bash
gcc -Wall -Wextra -std=c99 -o browser_history browser_history.c
./browser_history
```

## Sample session

```
Enter choice: 1
Enter URL: google.com
Visited: google.com
Enter choice: 1
Enter URL: github.com
Visited: github.com
Enter choice: 2
Back to: google.com
Enter choice: 3
Forward to: github.com
```

## Concepts used

- Stack (LIFO) data structure
- Linked lists
- Dynamic memory allocation (`malloc` / `free`)
- Structs and pointers
