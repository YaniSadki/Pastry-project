#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* the declaration of the structures to be used later.*/
typedef struct Element_str {
    char text[50];
    struct Element_str* next;
}Element_str;

typedef struct Taste_Stack {
    struct Element_str* flavors;
}Taste_Stack;

typedef struct Cake {
    struct Element_str* order;
    struct Taste_Stack* s_tastes;
}Cake;

typedef struct Element_cake {
    struct Cake* cake;
    struct Element_cake* next;
}Element_cake;

typedef struct Order_Queue {
    struct Element_str* orders;
}Order_Queue;

typedef struct Tasting_Queue {
    struct Element_cake* t_cake;
}Tasting_Queue;


/* Prototypes of the main functions */
Element_str* initialize_tastes();                       /* here we initialize a list with the different tastes available. */
void pass_order(char order[50], Order_Queue* q_orders); /* function to add an order at the end of the order queue */
Element_str* process_order(Order_Queue* q_orders);      /* function to retrieve a command from the order queue. However, if the queue order is empty the function will return NULL. */
Cake* create_cake(Element_str* order);                  /* function allowing us to create a cake containing only an order. */
void build_cake(Cake* cake, Element_str* l_tastes);     /* function to build the cake, using the list of tastes.*/
void deliver(Cake* cake, Tasting_Queue* q_tasting);     /* function to deliver cakes to the tasting queue. */
void tasting(Tasting_Queue* q_tasting, int nb_parts);   /* function removing a certain number of tastes of the first cake in the tasting queue */

/* Prototypes of other functions */
Element_str* create_element(char val[50]);              /* a function allowing to create a list with only one element, which we will use later to initialize a list of tastes. */
void add_element(Element_str** list, char val[50]);     /* function to add a string to a list. */
char* retire_element(Order_Queue* q_orders);            /* function to take out the first element of the order queue, so we can use the order to create the cake. */
int size_queue(Order_Queue* f_orders);                  /* function using a counter to return the number of element of the order queue */
void dequeue_tasting(Tasting_Queue* q_tasting);         /* function  to remove the flavor on top of the first cake in the tasting queue. */

void display_element(Element_str* list);                /* function to display an entire list.*/
void display_queue (Order_Queue* q_orders);             /* function to display the order queue. */
void display_cake(Cake* cake);                          /* function to display a cake (as a pile of flavors). */
void display_queue_tasting (Tasting_Queue* q_tasting);  /* function to display the tasting queue. */

void free_element(Element_str* list);                   /*a recursive function to free a list, to avoid memory leaks. */
void free_queue_orders(Order_Queue* q_orders);          /* function to free the order queue to avoid memory leaks */
void free_cake(Cake* cake);                             /* function to free a cake. It avoids memory leaks. */
void free_queue_tasting(Tasting_Queue* q_tasting);      /* function to avoid memory leaks if the customer does not finish every cake he ordered (food waste is bad tho !). */


Element_str* create_element(char val[50]) {
    Element_str* new_el=malloc(sizeof(Element_str));    /* we allocate space. */
    strcpy(new_el->text, val);                          /* we copy val in the new list */
    new_el->next=NULL;                                  /* we set the next element to NULL */
    return new_el;
}

void free_element(Element_str* list) {
    if (list != NULL) {                                 /* until the list is NULL, we use the function recursively on the next element, then we free every element starting from the end */
        free_element(list->next);
        free(list);
    }
}

void add_element(Element_str** list, char val[50]) {
    Element_str* temp= create_element(val);             /* we create a temporary list with the help of the create_element function */
    temp->next=*list;                                   /* we add val at the beginning of our list */
    *list=temp;
}

Element_str* initialize_tastes() {
    Element_str* l_tastes= create_element("Fudge"); /* we create a list with just 1 element, then we add every element using the add_element function */
    add_element(&l_tastes, "Banana");
    add_element(&l_tastes, "Apple");
    add_element(&l_tastes, "Orange");
    add_element(&l_tastes, "Strawberry");
    add_element(&l_tastes, "Vanilla");
    add_element(&l_tastes, "Chocolate");
    return l_tastes;
}

void display_element(Element_str* list){
    Element_str* temp=list;                             /* we create a temporary Element_str to navigate through list */
    if (temp!=NULL) {                                   /* we use temp to display every element of the list */
        while (temp->next!=NULL) {
            printf(" %s,", temp->text);
            temp=temp->next;
        }
        printf(" %s",temp->text);
    }
    printf(".\n");
}

int size_queue(Order_Queue* f_orders) {
    int cpt=0;                                          /* we set the counter to 0 */
    Element_str* temp=f_orders->orders;
    while (temp != NULL) {                              /* until the temp hasn't reached the last element, we keep on incrementing the counter */
        temp=temp->next;
        cpt++;
    }
    return cpt;                                         /* at the end, the counter is the number of element in the whole list */
}

void pass_order(char order[50], Order_Queue* q_orders) {
    if (size_queue(q_orders)<10) {                      /* there can't be more than 10 orders in the order queue */
        Element_str* ord= create_element(order);
        if (q_orders->orders==NULL) {                   /* if the queue is NULL, we just add the new order */
            q_orders->orders=ord;
        }
        else {                                          /* if it isn't, we navigate through it until we reach the last element, then we add the new order */
            Element_str* temp=q_orders->orders;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = ord;
        }
    }
}

char* retire_element(Order_Queue* q_orders) {
    if (q_orders->orders == NULL) {
        return NULL;
    }
    else {
        char* res=malloc(strlen(q_orders->orders->text)*sizeof(char));  /* we allocate space for the res value */
        strcpy(res, q_orders->orders->text);                            /* we copy the content of the order we want to remove in res */
        Element_str* old=q_orders->orders;                              /* we move the pointer to the next order, and we free the one we removed */
        q_orders->orders=q_orders->orders->next;
        free(old);
        return res;
    }
}

Element_str* process_order(Order_Queue* q_orders) {
    if (q_orders->orders != NULL) {
        Element_str* res=create_element(retire_element(q_orders));      /* we want to return an Element_str using the order that has been removed from the order queue */
        return res;
    }
    return NULL;
}

void display_queue (Order_Queue* q_orders) {
    Element_str* temp=q_orders->orders;                   /* works the same as display_element */
    while (temp != NULL) {
        printf("%s => ",temp->text);
        temp=temp->next;
    }
    printf("X\n");
}

void display_cake(Cake* cake) {
    Element_str* temp=cake->s_tastes->flavors;              /* works the same as display_element, except we display the different tastes in a column, like a real cake */
    while (temp!=NULL) {
        printf("^ %s\n", temp->text);
        temp=temp->next;
    }
}

void display_queue_tasting (Tasting_Queue* q_tasting) {
    Element_cake* temp=q_tasting->t_cake;                   /* works the same as display_element */
    int i=1;
    while (temp != NULL) {
        printf("Cake %d :\n",i);
        display_cake(temp->cake);
        temp=temp->next;
        printf("----------------------------\n");
        i++;
    }
}

void free_queue_orders(Order_Queue* q_orders) {
    while (size_queue(q_orders)!=0) {                        /* until the order queue is empty, we free the elements in it */
        free(retire_element(q_orders));
    }
    free(q_orders);                                          /* then we free the queue itself */
}

Cake* create_cake(Element_str* order) {
    Cake* cake= malloc(sizeof(Cake));                        /* we allocate space for the cake */
    cake->order=order;                                       /* the order is received as an input */
    cake->s_tastes=malloc(sizeof(Taste_Stack));
    cake->s_tastes->flavors=NULL;                            /* the pile of flavors is set to NULL */
    return cake;
}

void build_cake(Cake* cake, Element_str* l_tastes) {
    int i;
    for (i=0;i<strlen(cake->order->text);i++) {
        char x=cake->order->text[i];
        Element_str* temp=l_tastes;
        while (x != temp->text[0]) {                         /* we want to match every letter of the order with the first letter of the flavor in l_tastes */
            temp=temp->next;                                 /* we have to navigate through l_tastes to find the right flavor */
        }
        add_element(&(cake->s_tastes->flavors), temp->text); /* we add the right flavor at the top of our cake */
    }
}

void free_cake(Cake* cake) {
    free_element(cake->order);                               /* we free the orders */
    free_element(cake->s_tastes->flavors),                   /* we free the flavors */
    free(cake->s_tastes);                                    /* we free the stack */
    free(cake);                                              /* finally, we free the cake itself */
}

void deliver(Cake* cake, Tasting_Queue* q_tasting) {
    free_element(cake->order);                               /* we have to free the order to avoid memory leaks, as they are now useless */
    Element_cake* e_cake=malloc(sizeof(Element_cake));
    e_cake->cake=cake;                                       /* the first cake of our list of cakes the one in the input */
    e_cake->next=NULL;                                       /* the next one is set to NULL */
    if (q_tasting->t_cake==NULL) {
        q_tasting->t_cake=e_cake;                            /* if the tasting queue is empty, we just add it in */
    }
    else {
        Element_cake* temp=q_tasting->t_cake;                /* otherwise, we add it at the end using a temporary list to go through the tasting queue */
        while (temp->next != NULL) {
            temp=temp->next;
        }
        temp->next=e_cake;
    }
}

void dequeue_tasting(Tasting_Queue* q_tasting) {
    if (q_tasting->t_cake != NULL) {
        if (q_tasting->t_cake->cake->s_tastes->flavors->next != NULL) {                                 /* if there are flavors in the first cake in the queue, we just remove one of them */
            Element_str* old=q_tasting->t_cake->cake->s_tastes->flavors;
            q_tasting->t_cake->cake->s_tastes->flavors=q_tasting->t_cake->cake->s_tastes->flavors->next;
            free(old);                                                                                  /* and we free it of course... */
        }
        else {                                                                                          /* otherwise, we move to the next cake */
            Cake* old=q_tasting->t_cake->cake;
            q_tasting->t_cake=q_tasting->t_cake->next;
            free_cake(old);                                                                             /* and we free the first one */
        }
    }
}

void tasting(Tasting_Queue* q_tasting, int nb_parts) {
    while (nb_parts > 0) {                                      /* we just remove nb_parts of the cakes in the tasting queue, using the dequeue_tasting function */
        dequeue_tasting(q_tasting);
        nb_parts--;
        if (q_tasting->t_cake == NULL) {
            nb_parts=0;
        }
    }
}

void free_queue_tasting(Tasting_Queue* q_tasting) {
    while (q_tasting->t_cake != NULL) {
        Cake* temp=q_tasting->t_cake->cake;                     /* we free every cake in the tasting queue using a temporary pointer */
        q_tasting->t_cake=q_tasting->t_cake->next;
        free_cake(temp);
    }
    free(q_tasting);                                            /* and of course, we free the tasting queue itself ! */
}




int main() {
    Element_str *l_tastes = initialize_tastes();                /* we initialize the list of flavors */
    Order_Queue* q_orders=malloc(sizeof(Order_Queue));
    q_orders->orders=NULL;                                      /* the order queue is empty at the beginning */
    Tasting_Queue* q_tasting= malloc(sizeof(Tasting_Queue));
    q_tasting->t_cake=NULL;                                     /* the tasting queue is empty at the beginning */


    printf("Welcome in our pastry shop !\n");
    printf("Pastry chefs Benjamin CORCOS, Cleophas FOURNIER, Alyson REZAIRE, Yani SADKI will serve you !\n\n");

    printf("How many cakes do you want ? (We can't take more than 10 orders...)\n");
    int size;
    scanf("%d",&size);                                              /* the customer writes the number of cakes he wants */

    if (size>0) {
        printf("Type the first letter of the flavors you want in your cake, here are the flavors available :\n");
        display_element(l_tastes);                                  /* the list of tastes available is displayed */

        printf("\nMake your own cakes ! (for example CVS : Chocolate, Vanilla with Strawberry on top !)\n");
        for (int i=0;i<size;i++) {                                  /* for every cake, he has to write the first letter of the tastes he wants */
            int retry=1;                                            /* the retry variable is used to say if the order is correct */
            while (retry != 0) {
                char order[50];
                int error=0;
                printf("Cake %d :\n",i+1);
                scanf("%s",order);
                for (int k=0;k<strlen(order);k++) {
                    if (order[k]!='C' && order[k]!='V' && order[k]!='S' && order[k]!='O' && order[k]!='A' && order[k]!='B' && order[k]!='F') {
                        error=1;                                    /* if he writes a bad symbol, the error is set to 1 */
                    }
                }
                if (error==1) {
                    printf ("Please write the first letter of existing flavors...\n");  /* when there is an error, the customer has to rewrite a good order */
                    retry=1;
                }
                else {                                                                  /* when there is no error, the order is passed to the order queue */
                    pass_order(order,q_orders);
                    retry=0;
                }
            }
        }

        printf("\nHere is the queue of your orders, we are currently cooking them !\n");
        display_queue(q_orders);                                    /* we display the order queue */

        while(q_orders->orders != NULL) {                           /* for every order in the order queue... */
            Element_str* a=process_order(q_orders);
            Cake* b=create_cake(a);                                 /* ... we create a cake containing the first order in the queue... */
            build_cake(b,l_tastes);                                 /* ... and we build the cake using this order and the list of tastes. */
            deliver(b,q_tasting);                                   /* we pass the cake to the tasting queue */
        }

        printf("\nHere are your cakes, sir :\n");
        display_queue_tasting(q_tasting);                           /* we display this tasting queue containing all the cakes */

        char answer='E';
        while (answer == 'E') {                                     /* the answer variable determines if the customer wants to keep eating or not */
            answer='D';
            int p;
            printf("How many parts will you eat ?\n");
            scanf("%d",&p);                                         /* we ask him how many parts of the cakes he wants to eat */
            tasting(q_tasting,p);
            if (q_tasting->t_cake !=NULL){                          /* if there are still cakes in the tasting queue ... */
                printf("\n Here are the cakes remaining : \n");
                display_queue_tasting(q_tasting);                   /*... we display the queue ... */
                printf("Are you done ? Write D if you are and E if you still want to eat !\n");
                do {
                    scanf("%c",&answer);                            /* ... and we ask him if he wants to keep eating or not */
                }while(answer != 'D' && answer != 'E');             /* if he writes 'E' he will keep eating, and if he writes 'D', he will stop */
            }

        }
    }
    printf("\nThank you for your visit ! Have a nice day !\n");


    free_queue_tasting(q_tasting);                                  /* and of course, we have to free the memory we allocated to our 3 variables... */
    free_queue_orders(q_orders);
    free_element(l_tastes);

    return 0;
}
