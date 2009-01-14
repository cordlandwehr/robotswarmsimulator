#include "marker_information.h"

MarkerInformation::MarkerInformation() {
	// TODO Auto-generated constructor stub

}

MarkerInformation::~MarkerInformation() {
	// TODO Auto-generated destructor stub
}

boost::shared_ptr<MarkerInformation> MarkerInformation::clone() const {
	return boost::shared_ptr<MarkerInformation>(new MarkerInformation(*this));
}
