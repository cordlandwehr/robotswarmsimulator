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
   */
  static void add_edge(std::size_t source, std::size_t target);
  
  /** Adds an edge with given MarkerInformation.
   * 
   * \param source	ID of the source robot.
   * \param target	ID of the target robot.
   * \param marker	Wrapped MarkerInformation object.
   */
  static void add_edge(std::size_t source, std::size_t target, MarkerInformationWrapper marker);
  
  /**
   * Creates a MarkerRequest to be processed directly after the script.
   * 
   * The generated MarkerRequest is cached in a set of Request until the Lua
   * script finishes. Thereafter all cached requests are executed directly.
   * 
   * \param id		ID of the robot whose MarkerInformation object is
   * 			to be replaced.
   * \param marker	Wrapped MarkerInformation object.
   */
  static void add_marker_request(std::size_t id, MarkerInformationWrapper marker);
  
  /**
   * Returns outgoing edges od a given robot.
   * 
   * \param id		ID of the robot.
   * \returns		Vector of integer edges IDs.
   */
  static const std::vector<std::size_t> get_adjacent_edges(std::size_t id);
  
  /**
   * Returns robot IDs for a given edge.
   * 
   * \param id		ID of the edge.
   * \returns		Pair of robot IDs (in the order [ID1, ID2]). 
   */
  static const std::pair<std::size_t, std::size_t> get_edge_anchors(std::size_t id);
  
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
  static const std::vector<std::size_t> get_edges();
  
  /**
   * Returns MarkerInformation object for a given marker.
   * 
   * \param id		ID of the marker.
   * \returns		Wrapped MarkerInformation object.
   */
  static const MarkerInformationWrapper get_marker_information(std::size_t id);
  
  /**
   * Returns set of markes.
   * 
   * \returns		Vector of integer marker IDs.
   */
  static const std::vector<std::size_t> get_markers();
  
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
   * Returns the timestamp stored in the WorldInformation object.
   * 
   * \returns 		Time as iteger (round number). 
   */
  static int get_time();
  
  /**
   * Removes a given edge.
   * 
   * The removal is executed immediately. No Request is generated.
   * 
   * \param id		The id of the edge to be removed.
   */
  static void remove_edge(std::size_t id);
  
  /**
   * (Re)set WorldInformation object to be used by the wrapper.
   * 
   * \param world_information	The new WorldInformation object.
   */
  static void set_world_information(const boost::shared_ptr<WorldInformation> &world_information);

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
   * \var 		Map of references to queried MarkerIdentifier objects
   * 			(to be cleared each time a new WorldInformation object
   * 			is set).
   */
  static std::map< std::size_t, boost::shared_ptr<MarkerIdentifier> > marker_identifiers_;
  
  /**
   * \var 		Map of references to queried RobotIdentifier objects
   * 			(to be cleared each time a new WorldInformation object
   * 			is set).
   */
  static std::map< std::size_t, boost::shared_ptr<RobotIdentifier> > robot_identifiers_;
 
};
  
}

#endif // WORLD_INFORMATION_WRAPPER_H
