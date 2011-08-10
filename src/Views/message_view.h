/*
 * message_view.h
 *
 *  Created on: 10.08.2011
 *      Author: Jonas
 */

#ifndef MESSAGE_VIEW_H_
#define MESSAGE_VIEW_H_

#include "view.h"

class MessageView : public virtual View {
public:
	MessageView();
	virtual ~MessageView();

protected:
	virtual boost::shared_ptr<Message> get_message(RobotData& robot);
	virtual std::size_t get_number_of_messages(const RobotData& robot) const;
};

#endif /* MESSAGE_VIEW_H_ */
