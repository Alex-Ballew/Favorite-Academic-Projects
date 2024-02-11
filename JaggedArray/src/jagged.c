#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "jagged.h"


// Initialize a jagged array object with the given number of bins.
// The array is initially unpacked.
void jagged_init(jagged_t* jagged, int bins) {
    jagged->bins = (entry_t**) malloc(sizeof(entry_t)*bins);
    jagged->number_of_bins = bins;
    jagged->size = 0;
    jagged->packed_values = NULL;
    jagged->offsets = NULL;
    for (int i = 0; i < bins; i++) {
        jagged->bins[i] = NULL;
    }
}

void jagged_free(jagged_t* jagged) {
    if (jagged->bins != NULL) {
        for (int i = 0; i < jagged_bins(jagged); i++) {
            entry_t* node1 = jagged->bins[i];
            while (node1 != NULL) {
                // Must free node1
                entry_t* next = node1->next;
                free(node1);
                node1 = next;
            }
            jagged->bins[i] = NULL;
        }
        jagged->bins = NULL;
    }
}

// Return the number of elements in the jagged array
int jagged_size(jagged_t* jagged) {
    return jagged->size;
}

// Return the number of bins
int jagged_bins(jagged_t* jagged) {
    return jagged->number_of_bins;
}

// Return the number of slots in the given bin
int jagged_slots(jagged_t* jagged, int bin) {
    entry_t* node1 = jagged->bins[bin];
    int tracker = 0;
    if (node1 == NULL) {  // error handle
        return 0;
    } else {
        tracker++;
        while (node1->next != NULL) {  // loop through slots in linked list
            node1 = node1->next;
            tracker++;
        }
        return tracker;
    }
}

// Return the element stored at the given bin and slot number.
// Success is set to 0 if the element was found, or -1 otherwise.
// If success is -1, 0 is returned.
int jagged_element(jagged_t* jagged, int bin, int slot, int* success) {
    if (jagged->bins != NULL) {
        *success = 0;
        entry_t* node1 = jagged->bins[bin];
        if (slot == 0) {  // front case where node is in front of linked list
            return node1->value;
        } else {  // all other cases
            for (int i = 0; i < slot; i++) {
                if (node1 == NULL) {  // check at any given point for error
                    *success = -1;
                    return -1;
                }
                node1 = node1->next;
            }
            if (node1 == NULL) {
                *success = -1;
                return -1;
            } else {
                return node1->value;
            }
        }
    } else {  // return for when in packed form
        return jagged->packed_values[jagged->offsets[bin - 1] + slot];
    }
}

// Add an element to the bin. Return 0 is the element was
// added, or -1 if the representation is packed
int jagged_add(jagged_t* jagged, int bin, int element) {
    if (jagged->bins == NULL) {
         return -1;
    } else {
        entry_t* node = jagged->bins[bin];
        if (node == NULL) {  // create node and add to front of list if null
            entry_t* newnode = (entry_t*) malloc(sizeof(entry_t));
            newnode->value = element;
            newnode->next = NULL;
            jagged->bins[bin] = newnode;
            jagged->size++;
        } else {  // find back of list and add there
            while (node->next != NULL) {
                node = node->next;
            }
            entry_t* newnode = (entry_t*) malloc(sizeof(entry_t));
            newnode->next = NULL;
            newnode->value = element;
            node->next = newnode;
            jagged->size++;
        }
        return 0;
    }
}
// Remove the element from the given bin and slot. Return 0 on success,
// or -1 if the representation was packed or element not found.
int jagged_remove(jagged_t* jagged, int bin, int slot) {
    if (jagged_slots(jagged, bin) == 1) {  // last element in list case
        entry_t* node1 = jagged->bins[bin];
        free(node1);
        node1 = NULL;
        jagged->size--;
    } else if (slot == 0) {  // first element in list case
        entry_t* node1 = jagged->bins[bin];
        node1 = node1->next;
        jagged->bins[bin] = node1;
        jagged->size--;
        return 0;
    } else if (slot + 1 == jagged_slots(jagged, bin)) {
        entry_t* node1 = jagged->bins[bin];
        for (int i = 0; i < slot - 1; i++) {
            node1 = node1->next;
        }
        free(node1->next);
        node1->next = NULL;
        jagged->size--;
        return 0;
    } else {  // all middle cases
        entry_t* node1 = jagged->bins[bin];
        for (int i = 0; i < slot - 1; i++) {
            node1 = node1->next;
        }
        entry_t* node2 = node1->next;
        node1->next = node2->next;
    }
    return -1;
}

// Unpack the jagged array. Return 0 if successful or -1 if the array is
// already unpacked.
int jagged_unpack(jagged_t* jagged) {
    int loc = 0;
    if (jagged->bins != NULL) {
        return -1;
    } else {  // create room and set to null
        jagged->bins = (entry_t**) malloc(sizeof(entry_t)*jagged_bins(jagged));
        for (int i = 0; i < jagged_bins(jagged); i++) {
            jagged->bins[i] = NULL;
        }  // loop through colleced data
        for (int i = 0; i < jagged_bins(jagged); i++) {
            if (jagged->offsets[i + 1] - jagged->offsets[i] != 0) {
                for (int j = 0; j < jagged->offsets[i + 1] -
                    jagged->offsets[i]; j++) {
                    jagged_add(jagged, i, jagged->packed_values[loc]);
                    loc++;
                }
            }
        }
        free(jagged->packed_values);
        free(jagged->offsets);
        jagged->packed_values = NULL;
        jagged->offsets = NULL;
    }
    return -1;
}

// Pack the jagged array. Return 0 if successful or -1 if the array is already
// packed.
int jagged_pack(jagged_t* jagged) {
    int tracker = 0;
    int arrloc = 0;
    if (jagged->packed_values != NULL) {
        return -1;
    } else {  // create room for values and offsets
        jagged->packed_values = (int*) malloc(sizeof(int)*jagged_bins(jagged));
        jagged->offsets = (int*) malloc(sizeof(int)*jagged_bins(jagged));
        for (int i = 0; i < jagged_bins(jagged); i++) {  // put values in array
            entry_t* node1 = jagged->bins[i];
            for (int j = 0; j < jagged_slots(jagged, i); j++) {
                jagged->packed_values[arrloc] = node1->value;
                node1 = node1->next;
                arrloc++;
            }
        }
        jagged->offsets[0] = 0;  // set offsets
        for (int i = 1; i < jagged_bins(jagged); i++) {
            jagged->offsets[1] = 0;
            if (jagged_slots(jagged, i) == 0) {
                jagged->offsets[i + 1] = tracker;
            } else {
                tracker += jagged_slots(jagged, i);
                jagged->offsets[i + 1] = tracker;
            }
        }
        jagged_free(jagged);
    }
    return 0;
}

// Print a jagged array out. Useful for debugging
void jagged_print(jagged_t* jagged) {
}
