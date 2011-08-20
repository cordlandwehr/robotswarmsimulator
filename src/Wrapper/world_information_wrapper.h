//
//  world_information_wrapper.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 06.08.11.
//


#ifndef WORLD_INFORMATION_WRAPPER_H
#define WORLD_INFORMATION_WRAPPER_H

#include <vector>
#include <map>
#include <set>
#include <utility>
#include <boost/shared_ptr.hpp>

#include "../Model/world_information.h"
#include "../Requests/request.h"
#include "../Wrapper/marker_information_wrapper.h"

#include "../Model/edge_identifier.h"
#include "../Model/message_identifier.h"
#include "../Model/robot_identifier.h"

namespace LuaWrapper {

/**
 * A wrapper for WorldInformation objects.
 * 
 * This class provides static methods to work with WorldInformation objects.
 * These methods can be bound to Lua function calls. Using the static methods
 * requires an instance of a WorldInformation object. Most methods simply call
 * the member methods of this object and transform return values when
 * necessary.
 * 
 * \author 	Michael Knopf
 *
 * \warning	This class is NOT thread-safe.
 * \warning	The current implementation lacks securty checks (i.e. checking
 * 		for the validity of IDs etc.).
 */
class WorldInformationWrapper
{

public:
  WorldInformationWrapper ();
  virtual ~ WorldInformationWrapper ();
  
  /**
   * Adds an edge with no WorldInformation.
   * 
   * \param source	ID of the source robot.
   * \param target	ID of the target robot.
   * \returns		ID of the new edge.
   */
  static std::size_t add_edge(std::size_t source, std::size_t target, const std::string &type);
  
  /** Adds an edge with given MarkerInformation.
   * 
   * \param source	ID of the source robot.
   * \param target	ID of the target robot.
   * \param marker	Wrapped MarkerInformation object.
   * \returns		ID of the new edge.
   */
  static std::size_t add_edge(std::size_t source, std::size_t target, MarkerInformationWrapper marker, const std::string &type);

  /**
   * Creates an message with the given WorldInformation.
   * 
   * \param sender	ID of the sender.
   * \param receiver	ID of the receiver.
   * \param marker	Wrapper MarkerInformation object.
   * \returns		ID of the new message.
   */
  static std::size_t add_message(std::size_t sender, std::size_t receiver, MarkerInformationWrapper marker);
    
  /**
   * Returns MarkerInformation object for a given edge.
   * 
   * \param id		ID of the edge.
   * \returns		Wrapped MarkerInformation object.
   */
  static const MarkerInformationWrapper get_edge_information(std::size_t id);
  
  /**
   * Returns set of edges.
   * 
   * \returns		Vector of integer edge IDs.
   */
  static const std::vector<std::size_t> get_edges(const std::string &filter);
  
  /**
   * Returns outgoing edges od a given robot.
   * 
   * \param id		ID of the robot.
   * \returns		Vector of integer edges IDs.
   */
  static const std::vector<std::size_t> get_edges(std::size_t id, const std::string &filter);
    
  /**
   * Returns head of a given edge.
   * 
   * \param id		ID of the edge.
   * \returns		ID of the head (robot):
   */
  static std::size_t get_head(std::size_t id);
  
  /**
   * Returns the MarkerInformation for a given message.
   * 
   * \param id		ID of the message.
   * \returns		Wrapped MarkerInformation object.
   */
  static const MarkerInformationWrapper get_message_information(std::size_t id);
  
  /**
   * Returns set of messages.
   * 
   * \returns		ID of the message.
   */
  static const std::vector<std::size_t> get_messages();
  
  /**
   * Returns set of messages of a given robot.
   *
   * \param id		ID of the robot.
   * \returns		Vector of integer IDS.
   */
  static const std::vector<std::size_t> get_messages(std::size_t);
  
  /**
   * Return receiver ID of a given message.
   * 
   * \param id		ID of the message.
   * \returns		ID of the receiver (robot).
   */
  static std::size_t get_receiver(std::size_t id);
    
  /**
   * Returns the cached request set.
   * 
   * \returns		The set of cached Request object.
   */
  static std::set< boost::shared_ptr<Request> > get_request_set();
  
  /**
   * Returns MarkerInformation object for a given robot.
   * 
   * \param id		ID of the robot.
   * \returns		Wrapped MarkerInformation object.
   */  
  static const MarkerInformationWrapper get_robot_information(std::size_t id);
  
  /**
   * Returns set of robots.
   * 
   * \returns		Vector of integer robot IDs.
   */
  static const std::vector<std::size_t> get_robots();
  
  /**
   * Return sender ID of a given message.
   * 
   * \param id		ID of the message.
   * \returns		ID of the sender (robot).
   */
  static std::size_t get_sender(std::size_t id);
  
  /**
   * Returns tail of a given edge.
   * 
   * \param id		ID of the edge.
   * \returns		ID of the tail (robot):
   */
  static std::size_t get_tail(std::size_t id);
  
  /**
   * Returns the timestamp stored in the WorldInformation object.
   * 
   * \returns 		Time as iteger (round number). 
   */
  static int get_time();
  
  /**
   * Checks whether an edge is directed.
   * 
   * \param id 		Id of the edge to be checked.
   * \returns		Boolean (true = edge is directed).
   */
  static bool is_directed(std::size_t);
  
   /**
   * Checks whether an edge is undirected.
   * 
   * \param id 		Id of the edge to be checked.
   * \returns		Boolean (true = edge is undirected).
   */
  static bool is_undirected(std::size_t);
  
  /**
   * Removes a given edge.
   * 
   * The removal is executed immediately. No Request is generated.
   * 
   * \param id		The id of the edge to be removed.
   */
  static void remove_edge(std::size_t id);
  
  /**
   * Removes a given message.
   * 
   * The removal is executed immediately. No Request is generated.
   * 
   * \param id		The id of the message to be removed.
   */
  static void remove_message(std::size_t id);  
  
  /**
   * (Re)set WorldInformation object to be used by the wrapper.
   * 
   * \param world_information	The new WorldInformation object.
   */
  static void set_world_information(const boost::shared_ptr<WorldInformation> &world_information);
  
   /**
   * Sets MarkerInformation object for a given edge.
   * 
   * \param id		The id of the edge.
   * \param marker	The wrapper MarkerInformation object.
   */
  static void set_edge_information(std::size_t id, MarkerInformationWrapper marker);
  
  
  /**
   * Sets MarkerInformation object for a given robot.
   * 
   * \param id		The id of the robot.
   * \param marker	The wrapper MarkerInformation object.
   */
  static void set_robot_information(std::size_t id, MarkerInformationWrapper marker);

private:
  /**
   * \var		Pointer to the current WorldInformation object.
   */
  static boost::shared_ptr<WorldInformation> world_information_;
  
  /**
   * \var		Set of cached requests (to be cleared each time a
   * 			new WorldInformation object is set).
   */
  static std::set< boost::shared_ptr<Request> > request_set_;
  
 
  /**
   * \var 		Map of references to queried EdgeIdentifier objects
   * 			(to be cleared each time a new WorldInformation object
   * 			is set).
   */
  static std::map< std::size_t, boost::shared_ptr<EdgeIdentifier> > edge_identifiers_;
  
  /**
   * \var 		Map of references to queried MessageIdentifier objects
   * 			(to be cleared each time a new WorldInformation object
   * 			is set).
   */
  static std::map< std::size_t, boost::shared_ptr<MessageIdentifier> > message_identifiers_;
    
  /**
   * \var 		Map of references to queried RobotIdentifier objects
   * 			(to be cleared each time a new WorldInformation object
   * 			is set).
   */
  static std::map< std::size_t, boost::shared_ptr<RobotIdentifier> > robot_identifiers_;
 
  /**
   * Checks reference map.
   * 
   * Checks for a given pair of a reference map and a key (ID), whether the 
   * key exists. Throws an exception if the key does not exist.
   * 
   * \param map		Map to be checked.
   * \param key		Key (ID) to be looked up.
   */
  template<typename T>
  static void check_mapping(const std::map<std::size_t, boost::shared_ptr<T> >& map, std::size_t key);
  
  /**
   * Updates references maps.
   * 
   * Creates mappings from integer representations to Identifier objects.
   */
  static void update_mappings();
};
  
}

#endif // WORLD_INFORMATION_WRAPPER_H
