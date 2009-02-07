io.write("Running ", _VERSION, "\n")

print("Robots")
result = get_visible_robots()
for k, v in ipairs(result) do
        print(k .. ": " .. v)
        pos = get_position(v);
        print("x = " .. pos.x .. ", y = " .. pos.y .. ", z = " .. pos.z);
end 

print("Obstacles")
result = get_visible_obstacles()
for k, v in ipairs(result) do
        print(k .. ": " .. v)
        pos = get_position(v);
        print("x = " .. pos.x .. ", y = " .. pos.y .. ", z = " .. pos.z);
end 

print("Marker")
result = get_visible_markers()
for k, v in ipairs(result) do
        print(k .. ": " .. v)
end 
