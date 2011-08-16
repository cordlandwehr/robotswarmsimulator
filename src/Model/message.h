//
//  message.h
//  Robot Swarm Simulator
//
//  Created by Jonas Knoll on 21.07.11.
//

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "world_object.h"
#include "robot_data.h"
#include "identifier.h"

class RobotData;

class Message : public WorldObject {
public:
	Message(boost::shared_ptr<Identifier> id,
	        boost::shared_ptr<Vector3d> position,
	        boost::shared_ptr<RobotData> sender,
	        boost::shared_ptr<RobotData> receiver) : WorldObject(id, position), sender_(sender), receiver_(receiver)
	{
		;
	}

	/**
	 * Adds data to marker_information of this message (wrapper)
	 */
	void add_data(const std::string& var_name, const boost::any& insert_data) {
		marker_information().add_data(var_name, insert_data);
	}

	/**
	 * Removes data from marker_information of this message (wrapper)
	 */
	void remove_data(const std::string& var_name){
		marker_information().remove_data(var_name);
	}

	/**
	 * Returns data from marker_information of this message (wrapper)
	 */
	const boost::any get_data(const std::string& var_name){
		return marker_information().get_data(var_name);
	}

	boost::shared_ptr<RobotData> sender() const {
		return sender_;
	}

	boost::shared_ptr<RobotData> receiver() const {
		return receiver_;
	}

private:
	boost::shared_ptr<RobotData> sender_;
	boost::shared_ptr<RobotData> receiver_;
};

#endif /* MESSAGE_H_ */
