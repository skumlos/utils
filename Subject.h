#ifndef SUBJECT_H
#define SUBJECT_H
#include <list>

class Observer;

class Subject {
public:
	Subject() {};
	virtual ~Subject() {};
	void attach(Observer* obs);
	void detach(Observer* obs);
protected:
	void notify(void* obj = 0);
private:
	std::list<Observer*> observers;
};

#endif /* SUBJECT_H */