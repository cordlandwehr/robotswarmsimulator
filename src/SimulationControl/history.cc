/*
 * history.cc
 *
 *  Created on: Jan 7, 2009
 *      Author: craupach
 *
 *
 */

#include "history.h"
#include "../Model/world_information.h"
#include "time_point.h"
#include <iostream>

History::History(std::size_t size) : history_(size), consumer_position_(0), empty_count_(size), fill_count_(0){

}

void History::insert(boost::shared_ptr<TimePoint> time_point) {
	empty_count_.wait();
	//TOTA (dwonisch): Mutex is needed, since consumer_position_ is altered in both methods.
	//                 Maybe use a plain array instead of circular_buffer to avoid this.
	boost::mutex::scoped_lock lock(mutex_);
	time_point->lock();
	history_.push_front(time_point);
	++consumer_position_;
	lock.unlock();

	fill_count_.post();
}

boost::shared_ptr<TimePoint> History::get_oldest_unused(bool block) {
	if(block) {
		fill_count_.wait();
	}
	else if(!fill_count_.try_wait()) {
		return boost::shared_ptr<TimePoint>();
	}
	boost::mutex::scoped_lock lock(mutex_);
	--consumer_position_;
	boost::shared_ptr<TimePoint> result(new TimePoint(*history_[consumer_position_]));
	lock.unlock();

	empty_count_.post();
	return result;
}

const TimePoint& History::get_newest() const {
	return *history_.front();
}
