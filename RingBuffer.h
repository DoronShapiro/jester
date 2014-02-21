/*
 *  RingBuffer.h
 */

typedef struct buffer_tag {
	int *data;
	int last;
	int size;
} RingBuffer;

// create RingBuffer of size
RingBuffer *init(int size);

// enqueue v onto buffer
void enqueue(RingBuffer *b, int v);

// finds weighted average of buffer
int weighted_avg(RingBuffer *b);

// prints RingBuffer
void print_buffer(RingBuffer *b);

// free memory
void free_buffer(RingBuffer *b);