function print_edges()
  -- print all edges
  edges = View.get_visible_edges()
  me = View.get_own_id()
  log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Edge overview:")
  for i = 1, #edges do
    local id = edges[i]
    if View.is_directed(id) then
      log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Directed edge from " .. View.get_tail(id) .. " to " .. View.get_head(id) .. ".")
    else
      log("debug", "[move_and_forget.lua][Robot #" .. me .. "] Undirected edge between " .. View.get_tail(id) .. " and " .. View.get_head(id) .. ".")
    end    
  end
end

function calculateHopDistances()
  ignorelist = { 6 }
  distance = Statistics.calculate_hop_distance(1, 3, ignorelist)
  log("info", "hop Distance: " .. distance)
end



function main()
  --calculateHopDistances()

end
