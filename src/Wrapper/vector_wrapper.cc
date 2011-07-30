//
//  vector_wrapper.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 30.07.11.
//

#include <boost/bind.hpp>

#include "../ComputationalGeometry/misc_algorithms.h"
#include "../ComputationalGeometry/ch_algorithms.h"
#include "../ComputationalGeometry/point_algorithms.h"
#include "../ComputationalGeometry/points_sepplane.h"

#include "vector_wrapper.h"

namespace LuaWrapper {
    /**
     * transformas a vector wrapper into a vector
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
    
    /**
     * Computes distance between two Vectors by p-norm
     * @param v is the first vector
     * @param w is the second vector
     * @param p is value of norm
     * @return distance
     */
    const double compute_distance(const LuaWrapper::Vector3dWrapper& v, const LuaWrapper::Vector3dWrapper& w, int p) {
        return vector3d_distance(transform(v), transform(w), p);
    }
    
    /**
     * Computes center of given points.
     * @param point_list is vector of points
     * @return the center of gravity
     */
    const LuaWrapper::Vector3dWrapper compute_COG(std::vector<LuaWrapper::Vector3dWrapper> point_list) {
        return transform(PointAlgorithms::compute_COG(transform(point_list)));
    }
    
    /**
     * Computes center of the miniball around the given points.
     * @param point_list is vector of points
     * @return the center of the miniball around the given points
     */
    const LuaWrapper::Vector3dWrapper compute_CMinBall(std::vector<LuaWrapper::Vector3dWrapper> point_list) {
        return transform(PointAlgorithms::compute_CMinBall(transform(point_list)));
    }
    
    /**
     * Sort vectors by euclidean norm, distance to zero
     * @return sorted point_list
     */
    const std::vector<LuaWrapper::Vector3dWrapper> sort_points_by_distance(std::vector<LuaWrapper::Vector3dWrapper> point_list) {
        std::vector<Vector3d> vec = transform(point_list);
        MiscAlgorithms::sort_points_by_distance(vec, 2);
        return transform(vec);
    }
    
    /**
     * Wrapper around separate_point_from_points().
     */
    const LuaWrapper::Vector3dWrapper separate_point_from_points(const LuaWrapper::Vector3dWrapper& v, const std::vector<LuaWrapper::Vector3dWrapper>& w, double epsilon) {
        return transform(::separate_point_from_points(transform(v), transform(w), epsilon));
    }
    
    const LuaWrapper::Vector3dWrapper compute_cog_of_ch_of_points(const std::vector<LuaWrapper::Vector3dWrapper>& points) {
        return transform(CHAlgorithms::compute_cog_of_ch_of_points(transform(points)));
    }
    
    const bool point_contained_in_convex_hull_of_points(const LuaWrapper::Vector3dWrapper& point, const std::vector<LuaWrapper::Vector3dWrapper> points) {
        return CHAlgorithms::point_contained_in_convex_hull_of_points(transform(point), transform(points));
    }
    
    const LuaWrapper::Vector3dWrapper random_point_in_ch(const std::vector<LuaWrapper::Vector3dWrapper> points, int seed) {
        return transform(CHAlgorithms::random_point_in_ch(transform(points), seed));
    }
    
    
}