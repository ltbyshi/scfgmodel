#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__


typedef struct Sequence{
	int length;
	int* data;
};

Sequence* CreateSequence(int length);
void FreeSequence(Sequence* seq);

#endif
