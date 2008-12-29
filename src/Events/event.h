/*
 * event.h
 *
 *  Created on: Dec 29, 2008
 *      Author: craupach
 */

#ifndef EVENT_H_
#define EVENT_H_

class Event {
public:
	Event(int time) : time_(time) {};
	virtual ~Event() {};

	int get_time() const {return time_;};
private:
	/**
	 * The time the event happens
	 */
	int time_;
};

#endif /* EVENT_H_ */
