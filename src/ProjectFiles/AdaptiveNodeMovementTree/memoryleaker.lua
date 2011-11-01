i = 0

function main()
    local marker = MarkerInformation()
    local pos = Vector3d(0, 0, 0)
    WorldInformation.add_robot(i, pos, "SimpleRobot", marker)
    WorldInformation.add_robot(i+1, pos, "SimpleRobot", marker)
    WorldInformation.add_edge(i, i+1, "undirected")
    WorldInformation.remove_robot(i)
    WorldInformation.remove_robot(i+1)
    i = i+2

  end
