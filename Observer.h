/**
 * This is the observer class that observers should inherit from to
 * be notified from subjects.
 *
 * 2012, Martin Hejnfelt (martin@hejnfelt.com)
 */

#ifndef OBSERVER_H
#define OBSERVER_H
#include "Subject.h"

class Observer {
	public:
		Observer() {};
		~Observer() {};
		virtual void update(Subject* sub, void* obj) = 0;
};

#endif /* OBSERVER_H */
