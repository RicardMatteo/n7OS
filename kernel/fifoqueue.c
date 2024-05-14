#include "n7OS/fifoqueue.h"

pid_t items[MAX_SIZE];
int front = 0;
int rear = -1;
int count = 0;

// Fonction pour initialiser la file d'attente
void initQueue() {
    front = 0;
    rear = -1;
    count = 0;

}

// Fonction pour vérifier si la file d'attente est vide
int isEmpty() {
    return count == 0;
}

// Fonction pour vérifier si la file d'attente est pleine
int isFull() {
    return count == MAX_SIZE;
}

// Fonction pour ajouter un élément à l'arrière de la file d'attente
void enqueue(int value) {
    if (isFull()) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }
    rear = (rear + 1) % MAX_SIZE; // Déplacement circulaire
    items[rear] = value;
    count++;
}

// Fonction pour supprimer un élément du début de la file d'attente
int dequeue() {
    int removedItem;
    if (isEmpty()) {
        printf("Queue is empty. Cannot dequeue.\n");
        return -1;
    }
    removedItem = items[front];
    front = (front + 1) % MAX_SIZE; // Déplacement circulaire
    count--;
    return removedItem;
}

// Fonction pour afficher les éléments de la file d'attente
void display() {
    if (isEmpty()) {
        printf("Queue is empty.\n");
        return;
    }
    printf("Queue elements: ");
    int i = front;
    int j;
    for (j = 0; j < count; j++) {
        printf("%d ", items[i]);
        i = (i + 1) % MAX_SIZE; // Déplacement circulaire
    }
    printf("\n");
}
