//
//  coordinate_system_wrapper.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 01.08.11.
//

#ifndef COORDINATE_SYSTEM_WRAPPER_H
#define COORDINATE_SYSTEM_WRAPPER_H

#include "../Wrapper/vector_wrapper.h"

namespace LuaWrapper {
    struct CoordinateSystemWrapper {
        CoordinateSystemWrapper() {;}
        CoordinateSystemWrapper(LuaWrapper::Vector3dWrapper x_axis, LuaWrapper::Vector3dWrapper y_axis, LuaWrapper::Vector3dWrapper z_axis) : x_axis(x_axis), y_axis(y_axis), z_axis(z_axis) {;}
        LuaWrapper::Vector3dWrapper x_axis;
        LuaWrapper::Vector3dWrapper y_axis;
        LuaWrapper::Vector3dWrapper z_axis;
    };
}

#endif // COORDINATE_SYSTEM_WRAPPER_H