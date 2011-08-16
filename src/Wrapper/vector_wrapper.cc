//
//  vector_wrapper.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 30.07.11.
//

#include <boost/bind.hpp>



#include "vector_wrapper.h"

namespace LuaWrapper {
    /**
     * transforms a vector wrapper into a vector
     */
    const Vector3d transform(const LuaWrapper::Vector3dWrapper& vec) {
        Vector3d result;
        result.insert_element(kXCoord, vec.x);
        result.insert_element(kYCoord, vec.y);
        result.insert_element(kZCoord, vec.z);
        return result;
    }
    
    /**
     * transforms a vector of vector wrappers into a vector of vectors
     */
    const std::vector<Vector3d> transform(const std::vector<LuaWrapper::Vector3dWrapper>& vec) {
        std::vector<Vector3d> result;
        result.resize(vec.size());
        std::transform(vec.begin(), vec.end(), result.begin(), boost::bind(static_cast<const Vector3d(*)(const LuaWrapper::Vector3dWrapper&)>(&transform),_1));
        return result;
    }
    
    
    const LuaWrapper::Vector3dWrapper transform(const Vector3d& vec) {
        return LuaWrapper::Vector3dWrapper(vec(0), vec(1), vec(2));
    }
    
    const std::vector<LuaWrapper::Vector3dWrapper> transform(const std::vector<Vector3d>& vec) {
        std::vector<LuaWrapper::Vector3dWrapper> result;
        result.resize(vec.size());
        std::transform(vec.begin(), vec.end(), result.begin(), boost::bind(static_cast<const LuaWrapper::Vector3dWrapper(*)(const Vector3d&)>(&transform),_1));
        return result;
    }
    
    
    /**
     * Computes distance between two Vectors
     * @param v is the first vector
     * @param w is the second vector
     * @return distance
     */
    const double compute_distance(const LuaWrapper::Vector3dWrapper& v, const LuaWrapper::Vector3dWrapper& w) {
        return vector3d_distance(transform(v), transform(w));
    }
    
 
   
    
}