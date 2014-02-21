/*
 *  RingBuffer.c
 */

#import <stdio.h>
#import <stdlib.h>
#import "RingBuffer.h"

RingBuffer *init(int size) {
	int i;
	
	// malloc the buffer
	RingBuffer *b = malloc(sizeof(*b));
	if (!b) {
		printf("error: memory allocation failure\n");
		exit(1);
	}

	// initialize data as 0
	int arr[size];
	for (i = 0; i < size; i++) 
		arr[i] = 0;

	b->data  = arr;
	b->last = 0;
	b->size  = 0;
	return b;
}

void enqueue(RingBuffer *b, int v) {
	// increment
	b->last++;

	// store data
	b->data[b->last] = v;

	// wrap around
	if (b->last == b->size) 
		b->last = 0;
}

int weighted_avg(RingBuffer *b) {
	int sum    = 0;
	int weight = b->size;
	int i;

	for (i = b->size + b->last; i > b->last; i--, weight--)
		sum += b->data[i % b->size] * weight;

	// Why is this 210? copied from buffer_fix.ino
	return (int) (sum / 210);
}

void print_buffer(RingBuffer *b) {
	int i;
	printf("size: %d\n last: %d\n", b->size, b->last);
	for (i = b->size + b->last; i > b->last; i--) {
		printf("%d", b->data[i % b->size]);
	}
	printf("\n");
}

void free_buffer(RingBuffer *b) {
	free(b->data);
	free(b);
}




























