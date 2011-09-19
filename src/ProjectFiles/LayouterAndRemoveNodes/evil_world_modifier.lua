-- global settings
del_prob = 0.0125

function main()
  -- get all edges
  local robots = WorldInformation.get_robots()
  
  -- iterate through all messages and delete messages with given probability
  for i, robot in ipairs(robots) do
    if math.random() < del_prob then
      log("info", "[evil_world_modifier.lua] Removing random node.")
      WorldInformation.remove_robot(robot)
    else
      log("info", "[evil_world_modifier.lua] Not removing random node ".. i ..".")
    end
  end
end
