#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

typedef struct node {
    char type;
    double value;
    struct node *next;
} node;

typedef struct list {
    node *head;
    node *tail;
} list;

list *create_list() {
    list *l = (list *)malloc(sizeof(list));
    l->head = NULL;
    l->tail = NULL;
    return l;
}

void append(list *l, char type, double value) {
    node *n = (node *)malloc(sizeof(node));
    n->type = type;
    n->value = value;
    n->next = NULL;
    if (l->head == NULL) {
        l->head = n;
        l->tail = n;
    } else {
        l->tail->next = n;
        l->tail = n;
    }
}

double pop(list *l) {
    if (l->head == NULL) {
        return 0;
    } else {
        double value = l->head->value;
        char type = l->head->type;
        node *temp = l->head;
        l->head = l->head->next;
        free(temp);
        if (type == 'N') {
            return value;
        } else {
            return -value;
        }
    }
}

void print_list(list *l) {
    node *cur = l->head;
    while (cur != NULL) {
        if (cur->type == 'N') {
            printf("%f ", cur->value);
        } else {
            printf("%c ", (char)cur->value);
        }
        cur = cur->next;
    }
    printf("\n");
}

void free_list(list *l) {
    node *cur = l->head;
    while (cur != NULL) {
        node *temp = cur;
        cur = cur->next;
        free(temp);
    }
    free(l);
}

node *current;
int error;

double expr();
double term();
double factor();

double expr() {
    double left = term();
    while (current != NULL && !error) {
        char op = (char)current->value;
        if (op == '+' || op == '-') {
            current = current->next;
            double right = term();
            if (op == '+') {
                left += right;
            } else {
                left -= right;
            }
        } else {
            break;
        }
    }
    return left;
}

double term() {
    double result = factor();
    if (error) {
        return 0;
    }
    while (current != NULL && current->type == 'O') {
        char op = (char)current->value;
        if (op == '*' || op == '/' || op == '%') {
            current = current->next;
            double right = factor();
            if (error) {
                return 0;
            }
            if (op == '*') {
                result *= right;
            } else if (op == '/') {
                result /= right;
            } else if (op == '%') {
                result = fmod(result, right);
            }
        } else {
            break;
        }
    }
    return result;
}

double factor() {
    double result; 
    if (current == NULL) {
        error = 1;
        return 0;
    }
    char type = current->type;
    if (type == 'N') {
        result = current->value;
        current = current->next;
    } else if (type == 'O') {
        char op = (char)current->value; 
        if (op == '(') {
            current = current->next;
            result = expr();
            if (current == NULL || current->type != 'O' || current->value != ')') { 
                error = 1; 
                return 0; 
            } else {
                current = current->next; 
            }
        } else if (op == '%') {
            current = current->next;
            double right = factor();
            result = fmod(result, right);
        } else { 
            error = 1; 
            return 0; 
        }
    } else { 
        error = 1; 
        return 0; 
    }
    return result; 
}

list *read_line() {
    
    list *l = create_list(); 

    char c;

    while ((c=getchar()) != '\n' && c != EOF) {

        if (isspace(c)) continue;

        if (isdigit(c) || (c == '-' && l->tail == NULL) || (c == '-' && l->tail->type == 'O' && l->tail->value != ')')) {

            double num=0;
            int sign = 1;

            if (c == '-') {
                sign = -1;
                c = getchar();
            }

            while(isdigit(c)) {
                num=num*10+(c-'0');
                c=getchar();
            }

            if(c=='.') {
                c=getchar();
                double frac=1;
                while(isdigit(c)) {
                    frac/=10;
                    num+=frac*(c-'0');
                    c=getchar();
                }

            }

            ungetc(c,stdin);

            append(l,'N',sign*num);

        } else {

            append(l,'O',(double)c);

        }

    }

    return l;

}



int main() {

    list *l=read_line();

    current=l->head;

    error=0;

    int result=(int)expr();

    if (error || current != NULL) {
        printf("错误：无效的表达式\n");
    } else {
        printf("%d\n", result);
    }
    free_list(l);
    return 0;
}