/**
 * \class	Identifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of robots.
 */

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

#include <cstddef>
#include <boost/shared_ptr.hpp>

class Identifier {
public:
	virtual ~Identifier();

	virtual boost::shared_ptr<Identifier> clone() const = 0;
	//TODO (dwonisch): Why is this class needed?
	class Comp{
		public:
			bool operator() (const boost::shared_ptr<Identifier> & id1, const boost::shared_ptr<Identifier> &id2){
				if(id1->id() < id2->id() ){
					return true;
				}

				return false;
			}

		};

protected:
	explicit Identifier(std::size_t id);
	/**
	 * Return ID of identifier.
	 * \return ID of identifier.
	 */
	std::size_t id() const;

private:
	std::size_t id_;

	friend class View;
	friend class WorldInformation;
	friend class Parser;
	friend class write_obstacle_1;
};

#endif /* IDENTIFIER_H_ */
