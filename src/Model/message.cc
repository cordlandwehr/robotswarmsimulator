//
//  message.cc
//  Robot Swarm Simulator
//
//  Created by Jonas on 27.07.11.
//

#include "message.h"

Message::~Message(){
}

void Message::add_data(const std::string& var_name, const boost::any& insert_data) {
	marker_information().add_data(var_name, insert_data);
}

void Message::remove_data(const std::string& var_name){
	marker_information().remove_data(var_name);
}

const boost::any Message::get_data(const std::string& var_name){
	return marker_information().get_data(var_name);
}

boost::shared_ptr<RobotData> Message::sender() const {
	return sender_;
}

boost::shared_ptr<RobotData> Message::receiver() const {
	return receiver_;
}
