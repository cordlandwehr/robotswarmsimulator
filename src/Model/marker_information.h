/**
 * \brief	TODO insert description here
 */

#ifndef MARKER_INFORMATION_H_
#define MARKER_INFORMATION_H_

#include <boost/smart_ptr.hpp>

// TODO(peter) comment class
// TODO(peter) redesign class to met the following design (eMail discussion Christoph/Martina/Peter)
//             - it is NOT designed as a base class, so do not use any virtual methods
//             - carried data will be placed inside a map using a string ('variable name') as key and boost::any as
//               value for the key (boost::any holding the custom data)
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
