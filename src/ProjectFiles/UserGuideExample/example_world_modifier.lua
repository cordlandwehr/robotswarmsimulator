-- global settings
del_prob = 0.0125

function main()
  -- get all edges
  local messages = WorldInformation.get_messages()
  
  -- iterate through all messages and delete messages with given probability
  for i, message in ipairs(messages) do
    if math.random() < del_prob then
      log("info", "[example_world_modifier.lua] Removing random message.")
      WorldInformation.remove_message(message)
    end
  end
end
