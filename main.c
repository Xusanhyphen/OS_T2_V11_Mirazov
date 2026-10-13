/* OS_T2_V11: Ikki saralangan dinamik massivni bitta saralangan massivga birlashtirish (merge)
 * Kompilyatsiya: gcc -Wall -Wextra -g main.c -o main
 * Tekshirish:    valgrind --leak-check=full ./main
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 1000000

int oqi_int(const char *taklif, int *natija) {
    char buf[100], *oxir;
    printf("%s", taklif);
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    if (buf[0] == '\0') return 0;
    long v = strtol(buf, &oxir, 10);
    if (*oxir != '\0' || v < -2000000000L || v > 2000000000L) return 0;
    *natija = (int)v;
    return 1;
}

/* Massivni kiritadi. Xotira ajratadi; muvaffaqiyatda *n ni pointer orqali o'zgartiradi. */
int *massiv_oqi(const char *nom, int *n) {
    if (!oqi_int("Element soni: ", n) || *n <= 0 || *n > MAX_N) {
        printf("Xato: %s o'lchami 1..%d oralig'ida butun son bo'lishi kerak!\n", nom, MAX_N);
        return NULL;
    }
    int *p = malloc((size_t)*n * sizeof(int));
    if (p == NULL) { printf("Xotira ajratib bo'lmadi!\n"); return NULL; }
    printf("%s elementlarini o'sish tartibida kiriting:\n", nom);
    for (int i = 0; i < *n; i++) {
        if (!oqi_int("  > ", p + i)) {
            printf("Xato: butun son kiriting!\n");
            free(p);
            return NULL;
        }
        if (i > 0 && *(p + i) < *(p + i - 1)) {
            printf("Xato: massiv saralanmagan (o'sish tartibida emas)!\n");
            free(p);
            return NULL;
        }
    }
    return p;
}

/* Merge: natija massiv *out ga pointer orqali yoziladi. 0 - xato, 1 - muvaffaqiyat. */
int birlashtir(const int *a, int n, const int *b, int m, int **out) {
    int *c = malloc((size_t)(n + m) * sizeof(int));
    if (c == NULL) return 0;
    int i = 0, j = 0, k = 0;
    while (i < n && j < m) {
        if (*(a + i) <= *(b + j)) *(c + k++) = *(a + i++);
        else                      *(c + k++) = *(b + j++);
    }
    while (i < n) *(c + k++) = *(a + i++);
    while (j < m) *(c + k++) = *(b + j++);
    *out = c;
    return 1;
}

void chiqar(const char *nom, const int *p, int n) {
    printf("%s: ", nom);
    for (int i = 0; i < n; i++) printf("%d ", *(p + i));
    printf("\n");
}

int main(void) {
    int n = 0, m = 0;
    int *a = NULL, *b = NULL, *c = NULL;

    printf("--- 1-massiv ---\n");
    a = massiv_oqi("1-massiv", &n);
    if (a == NULL) return 1;
    printf("--- 2-massiv ---\n");
    b = massiv_oqi("2-massiv", &m);
    if (b == NULL) { free(a); return 1; }

    if (!birlashtir(a, n, b, m, &c)) {
        printf("Xotira ajratib bo'lmadi!\n");
        free(a); free(b);
        return 1;
    }
    chiqar("Natija", c, n + m);

    free(a); a = NULL;
    free(b); b = NULL;
    free(c); c = NULL;
    return 0;
}
