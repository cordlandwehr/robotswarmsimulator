/**
 * \brief	TODO insert description here
 */

#ifndef MARKER_INFORMATION_H_
#define MARKER_INFORMATION_H_

#include <boost/smart_ptr.hpp>

class MarkerInformation {
public:
	MarkerInformation();
	virtual ~MarkerInformation();

	/**
	 * Clones this object and returns a shared ptr to the cloned object.
	 * typeid(*this) == typeid(*clone)
	 * @return shared ptr to the cloned object
	 */
	virtual boost::shared_ptr<MarkerInformation> clone() const;
};

#endif /* MARKER_INFORMATION_H_ */
