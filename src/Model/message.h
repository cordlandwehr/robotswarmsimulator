//
//  message.h
//  Robot Swarm Simulator
//
//  Created by Jonas on 21.07.11.
//

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "world_object.h"
#include "robot_data.h"
#include "identifier.h"
#include "message_identifier.h"

class MessageIdentifier;

class Message : public WorldObject {
public:
	Message(boost::shared_ptr<RobotIdentifier> sender,
	        boost::shared_ptr<RobotIdentifier> receiver)
	: WorldObject(boost::shared_ptr<Identifier>(new MessageIdentifier()), boost::shared_ptr<Vector3d>(new Vector3d())), sender_(sender), receiver_(receiver)
	{
		;
	}

	Message(const Message& rhs) : WorldObject(rhs), sender_(rhs.sender_), receiver_(rhs.receiver_) {};

	virtual ~Message();

	/**
	 * Adds data to marker_information of this message (wrapper)
	 */
	void add_data(const std::string& var_name, const boost::any& insert_data);

	/**
	 * Removes data from marker_information of this message (wrapper)
	 */
	void remove_data(const std::string& var_name);

	/**
	 * Returns data from marker_information of this message (wrapper)
	 */
	const boost::any get_data(const std::string& var_name);

	boost::shared_ptr<RobotIdentifier> sender() const;

	boost::shared_ptr<RobotIdentifier> receiver() const;

	virtual boost::shared_ptr<WorldObject> clone() const;

private:
	boost::shared_ptr<RobotIdentifier> sender_;
	boost::shared_ptr<RobotIdentifier> receiver_;
};

#endif /* MESSAGE_H_ */
