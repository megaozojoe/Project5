/*COP 3502c Programming Assignment 5
by Matthew del Real
Ma696941
*/

//max string length is 50 so one extra for null pointer
#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 51
#include "leak_detector_c.h"

struct heapStruct {
    char** heaparray;
    int capacity;
    int size;
};


void readData(); // this function will read the data 
struct heapStruct *initHeap(int size);// intialization of the heap function
int insert(struct heapStruct *h, char word[], int mm); //insert the variable in the heap
void percolateUp(struct heapStruct *h, int index);// will percolate the word up
void percolateDown(struct heapStruct *h, int index);// will percolate the word down
int compareTo(char a[], char b[]); //just using and return strcmp
void swap(struct heapStruct *h, int index1, int index2); //swap will swap the words of heaps
void printHeap(struct heapStruct *h, int size);
int minimum(char a[], int indexa, char b[], int indexb);//finds the min value
char * removeMin(struct heapStruct *h, struct heapStruct * oHeap);// removes the top value
void peek(struct heapStruct *h); //lets us peak at the top value of the heap
char * removeMax(struct heapStruct *h, struct heapStruct * oHeap); //removes the max value
void heapify(struct heapStruct *h);// can be used to heap a array
void setMed(struct heapStruct * minHeap, struct heapStruct * maxHeap, int size, char med[MAXLEN], char temp[MAXLEN]); //this should be used to set the median
void percolateUpMax(struct heapStruct *h, int index);//This creates the maxHeap
void freeHeap(struct heapStruct *h, int size); //frees all the mallocs


int main(void) {
  atexit(report_mem_leak); //Memory leak detector

  readData();
  return 0;
}


void readData(){

  //gets data from file
  FILE * infile = fopen("in.txt", "r");

  //these are some temp variables
  char temp[MAXLEN];
  int size;
  char med[MAXLEN];
  
  //scanning for the size
  fscanf(infile, "%d", &size);

  
  struct heapStruct * minHeap = initHeap(size);// these are the two heaps that are going to be used to get values
  struct heapStruct * maxHeap = initHeap(size);

  int tempint;
  
  for(int i = 0; i < size; i++){
    fscanf(infile, "%s", temp); //scans in the word
    //printf("%s \n", temp);
    setMed(minHeap,maxHeap,size,med, temp);// should process the median
    
  }
  
  //printHeap(maxHeap,size);
  //printf("===================================\n");
  //printHeap(minHeap,size);

  freeHeap(minHeap,size);//frees all the data
  freeHeap(maxHeap,size);
  fclose(infile);
}

void setMed(struct heapStruct * minHeap, struct heapStruct * maxHeap, int size, char med[MAXLEN], char temp[MAXLEN]){
  int num = 0;//temporary number
  char tempStr[MAXLEN];

  //the first value is going to be the median right off the bat
  if(compareTo(med, "\0") == 0){
    strcpy(med, temp);
    printf("%s\n", med);
    num = insert(minHeap, temp, -1); //inserts the word
  }
  else{//here the size is uneven so the max is being switched
    if((maxHeap->size + minHeap->size)%2 == 0){//if they are even then insert
      insert(minHeap, temp, -1);//then the median is copied 
      strcpy(med, temp);//they are next printed in
      if(compareTo(minHeap->heaparray[1], maxHeap->heaparray[1]) < 0){
        printf("%s %s\n", maxHeap->heaparray[1], minHeap->heaparray[1]);
      }
      else{
          printf("%s %s\n", minHeap->heaparray[1], maxHeap->heaparray[1]);
      }
    }
    else if(maxHeap->size > minHeap->size){
      insert(maxHeap, temp, 1);
      removeMax(maxHeap, minHeap);
      //insert(maxHeap, temp, 1);
      strcpy(med,minHeap->heaparray[1]);
      printf("%s\n", med);
    }
    else if(maxHeap->size < minHeap->size){//same here the size is uneven so being switched
      insert(minHeap, temp, -1);
      removeMin(minHeap, maxHeap);
      //insert(minHeap, temp, -1);
      strcpy(med,maxHeap->heaparray[1]);
      printf("%s\n", med);
    }
    /*
    if((maxHeap->size + minHeap->size)%2 == 0){ //here there are an equal amount of values on both sides and so it prints the both in alphabetical order
      insert(minHeap, temp, -1);
      strcpy(med, temp);
      if(compareTo(minHeap->heaparray[1], maxHeap->heaparray[1]) > 0){
        printf("%s %s\n", minHeap->heaparray[1], maxHeap->heaparray[1]);
      }
      else{
          printf("%s %s\n", minHeap->heaparray[1], maxHeap->heaparray[1]);
      }
    
    }
    */
    
    //if the min heap is smaller
    /*
    if(compareTo(med,temp) < 0){
      num = insert(minHeap, temp, -1);
    }
    //if the min heap is larger
    else if(compareTo(med,temp) > 0){
      num = insert(maxHeap,temp, 1);
    }
    else{ //if they are equal in size
      
    }
    */
 
  }  
}



//heapify is used in the construction of heaps
void heapify(struct heapStruct *h) {

    int i;

    // We form a heap by just running percolateDown on the first half of the
    // elements, in reverse order.
    for (i=h->size/2; i>0; i--)
        percolateDown(h, i);

}


// will compare two strings and then return the value which is small or first
int compareTo(char a[], char b[]){
  //string compare should return the proper comparison
  return strcmp(a, b);
}



struct heapStruct* initHeap(int size) {

    struct heapStruct* h;

    // Allocate space for the heap and set the size for an empty heap.
    h = (struct heapStruct*)(malloc(sizeof(struct heapStruct)));
    //capacity of the array is the size
    h->capacity = size;
    //this declares the array of strings
    h->heaparray = malloc(sizeof(char*)*(size+1));
    //this will create space for all the strings
    for(int i = 1; i < size +1; i++){
      h->heaparray[i] = malloc(sizeof(char) * MAXLEN); //this should create space for all of the words
    }
    //there are no values in the heap so far so 0
    h->size = 0;
    //returns the pointer to the heap
    return h;
}

//inserts the variable into the heap
//should use heap based off the call
//then make call to percolate up or down depending on the heap
int insert(struct heapStruct *h, char word[], int mm) {

    // Our array is full, we need to allocate some new space!
    if (h->size == h->capacity) {

        // Allocate new space for an array.
        h->heaparray = (char**) realloc(h->heaparray, sizeof(char*)*(2*h->capacity+1));

        // when the realloc happens. This will malloc all new arrays for words
        // i is the capacity 
        for(int i = h->capacity+1; i < (h->capacity * 2) + 1; i++){ 
          h->heaparray[i] = malloc(sizeof(char) * MAXLEN);
        }

        // Realloc failed so we quit.
        if (h->heaparray == NULL) return 0;

        // Double the capacity.
        h->capacity *= 2;
    }

    // Adjust all the necessary components of h, and then move the inserted
    // item into its appropriate location.
    h->size++;
    h->heaparray[h->size] = strcpy(h->heaparray[h->size], word); //this should copy the word from the var to the heap
    if(mm == -1){
      percolateUp(h, h->size);
    }
    else if(mm == 1){
      percolateUpMax(h, h->size);
    }
}

void percolateUp(struct heapStruct *h, int index) {

    // Can only percolate up if the node isn't the root.
    if (index > 1) {
      
      
        // See if our current node is smaller in value than its parent.
        if (compareTo(h->heaparray[index/2], h->heaparray[index]) > 0) { // shoud be if the word is smaller

            // Move our node up one level.
            swap(h, index, index/2);

            // See if it needs to be done again.
            percolateUp(h, index/2);
        }
    }
}
//this will form the maxHeap
void percolateUpMax(struct heapStruct *h, int index) {

    // Can only percolate up if the node isn't the root.
    if (index > 1) {
      
      
        // See if our current node is smaller in value than its parent.
        if (compareTo(h->heaparray[index/2], h->heaparray[index]) < 0) { // shoud be if the word is smaller

            // Move our node up one level.
            swap(h, index, index/2);

            // See if it needs to be done again.
            percolateUpMax(h, index/2);
        }
    }
}

// Runs percolate down on the heap pointed to by h on the node stored in index.
void percolateDown(struct heapStruct *h, int index) {

    int min;

    // Only try to percolate down internal nodes.
    if ((2*index+1) <= h->size) {

        // Find the minimum value of the two children of this node.
        min = minimum(h->heaparray[2*index], 2*index, h->heaparray[2*index+1], 2*index+1);

      // If this value is less than the current value, then we need to move
      // our current value down the heap.
        if (compareTo(h->heaparray[index], h->heaparray[min]) > 0) {
            swap(h, index, min);

            // This part is recursive and allows us to continue percolating
            // down the element in question.
            percolateDown(h, min);
        }
    }

    // Case where our current element has exactly one child, a left child.
    else if (h->size == 2*index) {

        // Here we only compare the current item to its only child.
        // Clearly, no recursive call is needed since the child of this node
        // is a leaf.
        if (compareTo(h->heaparray[index], h->heaparray[2*index]) > 0)
            swap(h, index, 2*index);
    }
}

// Returns indexa if a > b, and returns indexb otherwise.
int minimum(char a[], int indexa, char b[], int indexb) {

    // Return the value associated with a.
    if (compareTo(a,b) < 0)
        return indexa;

    // Return the value associated with b.
    else
        return indexb;
}
int maximum(char a[], int indexa, char b[], int indexb) {

    // Return the value associated with a.
    if (compareTo(a,b) > 0)
        return indexa;

    // Return the value associated with b.
    else
        return indexb;
}



char * removeMin(struct heapStruct *h, struct heapStruct * oHeap) {

    char retval[MAXLEN];

    // We can only remove an element, if one exists in the heap!
    if (h->size > 0) {

        // This is where the minimum is stored.
        strcpy(retval, h->heaparray[1]);

        // Copy the last value into this top slot.
        strcpy(h->heaparray[1], h->heaparray[h->size]);

        // Our heap will have one fewer items.
        h->size--;

        // Need to let this value move down to its rightful spot in the heap.
        percolateDown(h, 1);
      
        insert(oHeap, retval, 1);//takes the value and adds it to other heap
        // Now we can return our value.
        return retval;

    }

    // No value to return, indicate failure with a -1.
    else
        return NULL;
}

char * removeMax(struct heapStruct *h, struct heapStruct * oHeap) {

    char retval[MAXLEN];

    // We can only remove an element, if one exists in the heap!
    if (h->size > 0) {

        // This is where the maximum is stored.
        strcpy(retval, h->heaparray[1]);

        // Copy the last value into this top slot.
        strcpy(h->heaparray[1], h->heaparray[h->size]);

        // Our heap will have one fewer items.
        h->size--;

        // Need to let this value move down to its rightful spot in the heap.
        percolateDown(h, 1);
      
        insert(oHeap, retval, -1);//this should take value and add it to the other heap
        // Now we can return our value.
        return retval;

    }

    // No value to return, indicate failure with a -1.
    else
        return NULL;
}


// Swaps the values stored in the heap pointed to by h in index1 and index2.
void swap(struct heapStruct *h, int index1, int index2) {
    char temp[MAXLEN];
    strcpy(temp, h->heaparray[index1]); // should set the temp to the word of index 1
    strcpy(h->heaparray[index1], h->heaparray[index2]); //should copy index 1 from index 2
    strcpy(h->heaparray[index2], temp); // this should be index 2 copying from temp
}

//this is a printing of the min heap
void printHeap(struct heapStruct *h, int size){
  for(int i = 1; i < size + 1; i++){
  printf("%s, index: %d\n", h->heaparray[i], i);
  }
}

void peek(struct heapStruct *h){
  printf("%s\n", h->heaparray[1]);
}

//frees the data
void freeHeap(struct heapStruct *h, int size){
  for(int i = 0; i < size +1; i++){
    free(h->heaparray[i]);//frees the word mallocs
  }
  free(h->heaparray);
  free(h);//frees the intial malloc
}