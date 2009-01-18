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


History::History(std::size_t size) : history_(size), consumer_position_(0), empty_count_(size), fill_count_(0){

}

void History::insert(boost::shared_ptr<WorldInformation> world_information) {
	empty_count_.wait();
	//TODO (dwonisch): Mutex is needed, since consumer_position_ is altered in both methods.
	//                 Maybe use a plain array instead of circular_buffer to avoid this.
	boost::mutex::scoped_lock lock(mutex_);
	history_.push_front(world_information);
	++consumer_position_;
	lock.unlock();

	fill_count_.post();
}

boost::shared_ptr<WorldInformation> History::get_oldest_unused() {
	fill_count_.wait();

	boost::mutex::scoped_lock lock(mutex_);
	--consumer_position_;
	boost::shared_ptr<WorldInformation> result(new WorldInformation(*history_[consumer_position_]));
	lock.unlock();

	empty_count_.post();
	return result;
}

const WorldInformation& History::get_newest() const {
	return *history_.front();
}
