#include "Subject.h"
#include "Observer.h"

void Subject::attach(Observer* obs)
{
	observers.push_back(obs);
}

void Subject::detach(Observer* obs)
{
	observers.remove(obs);
}

void Subject::notify(void* obj)
{
	std::list<Observer*>::iterator it;
	for(it = observers.begin(); it != observers.end(); it++) {
		(*it)->update(this,obj);
	}
}