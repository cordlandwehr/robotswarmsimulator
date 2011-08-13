/**
 * \class	Identifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of robots.
 */

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

#include <cstddef>
#include <boost/shared_ptr.hpp>

// forward declaration for cross namepsace friendship (awww...)
namespace LuaWrapper{class WorldInformationWrapper;}

class Identifier {
public:
	virtual ~Identifier();

	virtual boost::shared_ptr<Identifier> clone() const = 0;


protected:
	explicit Identifier(std::size_t id);
	/**
	 * Return ID of identifier.
	 * \return ID of identifier.
	 */
	std::size_t id() const;

private:
	std::size_t id_;

	// TODO(craupach) this class has many many friends
	friend class View;
	friend class WorldInformation;
	friend class LuaWrapper::WorldInformationWrapper;
	friend class Parser;
	friend class write_obstacle_1;
	friend class SimulationRenderer;
	friend class StatsControl;
	friend class MessageRequestHandler;
};

#endif /* IDENTIFIER_H_ */
