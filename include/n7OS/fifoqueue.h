#ifndef _FIFOQUEUE_H
#define _FIFOQUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <n7OS/process.h>

#define MAX_SIZE 100


// Fonction pour initialiser la file d'attente
void initQueue();

// Fonction pour vérifier si la file d'attente est vide
int isEmpty();

// Fonction pour vérifier si la file d'attente est pleine
int isFull();

// Fonction pour ajouter un élément à l'arrière de la file d'attente
void enqueue(int value);

// Fonction pour supprimer un élément du début de la file d'attente
int dequeue();

// Fonction pour afficher les éléments de la file d'attente
void display();


#endif