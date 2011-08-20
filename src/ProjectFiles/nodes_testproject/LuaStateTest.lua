-- some global variables to test integrity of the Lua state
a = 0 -- Fib_(n-2)
b = 1 -- Fib_(n-1)
n = 1

function main()
  -- say who I am and print Fib_(n)
  log("debug", "Running LuaStateTest.lua ...")
  log("debug", "[LuaStateTest.lua] Fib(" .. n .. ") = " .. b)
  -- update fibonacci nubers
  a, b, n = b, a+b, n+1
end
