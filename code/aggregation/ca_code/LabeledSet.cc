#include "LabeledSet.h"

LabeledSet::LabeledSet(int d){
  size = 1;
  node = d;
  label = d;
  parent = this;
}

LabeledSet::~LabeledSet(){
}

LabeledSet *LabeledSet::findSet(){
	if (parent != this){
		parent = parent->findSet();
	}
	return parent;
}

LabeledSet *LabeledSet::link(LabeledSet *otherSet){
	if (this->rank > otherSet->rank){
		otherSet->parent = this;
		return this;
	} else {
		this->parent = otherSet;
		if (this->rank == otherSet->rank){
			otherSet->rank = otherSet->rank + 1;
		}
		return otherSet;
	}
}


/**
 * Performs union of the sets that contain this and otherSet
 * If they belong to the same set it returns NULL;
 */

LabeledSet *LabeledSet::unionSets(LabeledSet *otherSet){
	LabeledSet *thisRep = findSet();
	LabeledSet *otherRep = otherSet->findSet();
	return thisRep->link(otherRep);
}

void LabeledSet::merge(LabeledSet *otherSet){
  //cout << size << " " <<otherSet->size<<endl;
  int newSize =  findSet()->size + otherSet->findSet()->size;
  LabeledSet *newRep = unionSets(otherSet);
  newRep->size = newSize;
  newRep->label = label;
  //  cout << newRep->node<<" "<<newRep->size<<" "<<newRep->label<<  endl;
}
