io.write("Running ", _VERSION, "\n")

print("")
result = get_visible_robots()
for k, v in ipairs(result) do
        print(k .. ": " .. v)
end 
