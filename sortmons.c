#include <stdlib.h>
#include <stdio.h>

typedef struct Monitor Monitor;

struct Monitor {
    int id;
    int mx, my, mw, mh;
    Monitor* next;
};

static Monitor *mons;

char
is_mon_a_before_b(Monitor *a, Monitor *b)
{
    int iy = a->my, ih = a->mh, oy = b->my, oh = b->mh;
    int iby = iy + ih, oby = oy + oh;
    int min_by = iby < oby ? iby : oby;
    int max_y = iy > oy ? iy : oy;
    float overlap = min_by - max_y;
    float min_h = ih < oh ? ih : oh;
    char is_same_row = overlap / min_h > 0.5;
    return is_same_row ? a->mx < b->mx : a->my < b->my;
}

void
sortmons(void)
{
    Monitor *curr = mons, *next, **cursor; 
    mons = NULL;
    while (curr) {
        next = curr->next;
        curr->next = NULL;
        cursor = &mons;
        while (*cursor) {
            if (is_mon_a_before_b(curr, *cursor)) {
                curr->next = *cursor;
                *cursor = curr;
                break;
            }
            cursor = &(*cursor)->next;
        }
        if (*cursor == NULL) {
            *cursor = curr;
        }
        curr = next;
    }
}

int main() {
    Monitor *m1 = calloc(1, sizeof(Monitor));
    m1->id = 1;
    m1->mx = 1920;
    m1->my = 0;
    m1->mw = 1920;
    m1->mh = 1080;

    Monitor *m2 = calloc(1, sizeof(Monitor));
    m2->id = 2;
    m2->mx = 0;
    m2->my = 80;
    m2->mw = 1920;
    m2->mh = 1000;

    Monitor *m3 = calloc(1, sizeof(Monitor));
    m3->id = 3;
    m3->mx = 0;
    m3->my = 600;
    m3->mw = 1920;
    m3->mh = 1080;

    mons = m1;
    m1->next = m2;
    m2->next = m3;

    printf("is m1 before m2: %d\n", is_mon_a_before_b(m1, m2));
    printf("is m2 before m1: %d\n", is_mon_a_before_b(m2, m1));
    printf("is m1 before m3: %d\n", is_mon_a_before_b(m1, m3));
    printf("is m3 before m2: %d\n", is_mon_a_before_b(m3, m2));

    sortmons();
    Monitor *temp = mons;
    while (temp) {
        printf("%d ", temp->id);
        temp = temp->next;
    }
    printf("\n");
}


