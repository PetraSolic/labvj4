#include <stdio.h>
#include <stdlib.h>

typedef struct Term {
    int coefficient;
    int exponent;
    struct Term* next;
} Term;

typedef struct Polynomial {
    Term* head;
} Polynomial;

// Funkcija za stvaranje novog člana polinoma
Term* createTerm(int coefficient, int exponent) {
    Term* newTerm = (Term*)malloc(sizeof(Term));
    newTerm->coefficient = coefficient;
    newTerm->exponent = exponent;
    newTerm->next = NULL;
    return newTerm;
}

// Dodavanje člana u polinom (sortirano po eksponentima)
void addTerm(Polynomial* poly, int coefficient, int exponent) {
    Term* newTerm = createTerm(coefficient, exponent);
    if (!poly->head || poly->head->exponent < exponent) {
        newTerm->next = poly->head;
        poly->head = newTerm;
    }
    else {
        Term* current = poly->head;
        while (current->next && current->next->exponent >= exponent) {
            if (current->next->exponent == exponent) {
                current->next->coefficient += coefficient;
                free(newTerm);
                return;
            }
            current = current->next;
        }
        newTerm->next = current->next;
        current->next = newTerm;
    }
}

// Učitavanje polinoma iz datoteke
void loadPolynomial(Polynomial* poly, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    int coefficient, exponent;
    while (fscanf(file, "%d %d", &coefficient, &exponent) != EOF) {
        addTerm(poly, coefficient, exponent);
    }
    fclose(file);
}

// Ispis polinoma
void printPolynomial(const Polynomial* poly) {
    Term* current = poly->head;
    while (current) {
        printf("%dx^%d ", current->coefficient, current->exponent);
        if (current->next && current->next->coefficient > 0) printf("+ ");
        current = current->next;
    }
    printf("\n");
}

// Zbrajanje polinoma
Polynomial addPolynomials(const Polynomial* poly1, const Polynomial* poly2) {
    Polynomial result = { NULL };
    Term* t1 = poly1->head;
    Term* t2 = poly2->head;

    while (t1) {
        addTerm(&result, t1->coefficient, t1->exponent);
        t1 = t1->next;
    }
    while (t2) {
        addTerm(&result, t2->coefficient, t2->exponent);
        t2 = t2->next;
    }
    return result;
}

// Množenje polinoma
Polynomial multiplyPolynomials(const Polynomial* poly1, const Polynomial* poly2) {
    Polynomial result = { NULL };
    for (Term* t1 = poly1->head; t1; t1 = t1->next) {
        for (Term* t2 = poly2->head; t2; t2 = t2->next) {
            addTerm(&result, t1->coefficient * t2->coefficient, t1->exponent + t2->exponent);
        }
    }
    return result;
}

// Oslobađanje memorije polinoma
void freePolynomial(Polynomial* poly) {
    Term* current = poly->head;
    while (current) {
        Term* temp = current;
        current = current->next;
        free(temp);
    }
    poly->head = NULL;
}

int main() {
    Polynomial poly1 = { NULL };
    Polynomial poly2 = { NULL };

    loadPolynomial(&poly1, "polynomial1.txt");
    loadPolynomial(&poly2, "polynomial2.txt");

    printf("Polinom 1: ");
    printPolynomial(&poly1);
    printf("Polinom 2: ");
    printPolynomial(&poly2);

    Polynomial sum = addPolynomials(&poly1, &poly2);
    printf("Zbroj polinoma: ");
    printPolynomial(&sum);

    Polynomial product = multiplyPolynomials(&poly1, &poly2);
    printf("Umnožak polinoma: ");
    printPolynomial(&product);

    freePolynomial(&poly1);
    freePolynomial(&poly2);
    freePolynomial(&sum);
    freePolynomial(&product);

    return 0;
}
