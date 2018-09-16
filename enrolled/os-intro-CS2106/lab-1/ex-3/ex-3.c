/*************************************
 * Lab 1 Exercise 3
 * Name: Jerrell Ezralemuel
 * Matric No: A0181002B
 * Lab Group: 08
 *************************************/

#include <stdio.h>

typedef int (*arithFuncPtr)(int, int);

int lumos(int x, int y) {
    return y * (y + 1) / 2 - x * (x - 1) / 2;
}

int alohomora(int x, int y) {
    return y ? alohomora(y, x % y) : x;
}

int sonorus(int x, int y) {
    if (y < 2) return y == 0 ? 1 : x;
    int val = sonorus(x, y / 2);
    return val * val * sonorus(x, y % 2);
}

int expelliarmus(int x, int y) {
    while (x % y == 0) x /= y; 
    return x;
}

int main() {
    arithFuncPtr fun[] = {NULL, lumos, alohomora, expelliarmus, sonorus};
    const char *name[] = {"", "lumos", "alohomora", "expelliarmus", "sonorus"};
    int a, b, optype;
    while (scanf("%d %d %d", &optype, &a, &b) != EOF) 
        printf("%s %d\n", name[optype], fun[optype](a, b));
    return 0;
}
