local tabFiles = {   
[1] = "test2",   
[6] = "test3",   
[4] = "test1"  
}   
for k, v in ipairs(tabFiles) do  
    print(k, v)   
end 


Set = {}
-- create a new set with the values of the given list
local mt = {}    -- metatable for sets

function Set.union (a, b)
    if getmetatable(a) ~= mt or getmetatable(b) ~= mt then
         print("attempt to ’add’ a set with a non-set value", 2) 
    else
      local res = Set.new{}
      for k in pairs(a) do res[k] = true end
      for k in pairs(b) do res[k] = true end
      return res
    end
end


function Set.intersection (a, b)
    local res = Set.new{}
        for k in pairs(a) do
         res[k] = b[k]
        end
    return res
end

function Set.tostring (set)
    local l = {}     -- list to put all elements from the set
    for e in pairs(set) do
        l[#l + 1] = e
    end
    return "{" .. table.concat(l, ", ") .. "}"
end

function Set.print (s)
    print(Set.tostring(s))
end



mt.__add = Set.union

function Set.new (l)   -- 2nd version
   local set = {}
   setmetatable(set, mt)
   for _, v in ipairs(l) do set[v] = true end
   return set
end

mt.__le = function (a, b)    -- set containment
       for k in pairs(a) do
         if not b[k] then return false end
       end
       return true
end

mt.__lt = function (a, b)
       return a <= b and not (b <= a)
end

mt.__eq = function (a, b)
       return a <= b and b <= a
end

mt.__tostring = Set.tostring

 s1 = Set.new{10, 20, 30, 50}
 s2 = Set.new{30, 1}
 print(getmetatable(s1))          --> table: 00672B60
 print(getmetatable(s2))          --> table: 00672B60



s3 = s1 + s2
Set.print(s3)  --> {1, 10, 20, 30, 50}

mt.__mul = Set.intersection
Set.print((s1 + s2)*s1)     --> {10, 20, 30, 50}

s = Set.new{1,2,3}
-- s=s+8
-- Set.print(s)

s1 = Set.new{2, 4}
s2 = Set.new{4, 10, 2}
print(s1 <= s2)       --> true
print(s1 < s2)        --> true
print(s1 >= s1)       --> true
print(s1 > s1)        --> false
print(s1 == s2 * s1)  --> true

s1 = Set.new{10, 4, 5}
print(s1)    --> {4, 5, 10}

mt.__metatable = "not your business"
s1 = Set.new{}
print(getmetatable(s1))     --> not your business
-- setmetatable(s1, {}) --> stdin:1: cannot change protected metatable

-- The __index metamethod
Window = {}       -- create a namespace
-- create the prototype with default values
Window.prototype = {x=0, y=0, width=100, height=100}
Window.mt = {}    -- create a metatable
-- declare the constructor function
function Window.new (o)
  setmetatable(o, Window.mt)
  return o 
end

-- Window.mt.__index = function (table, key)
--        return Window.prototype[key]
-- end

Window.mt.__index = Window.prototype

w = Window.new{x=10, y=20}
print(w.width)    --> 100

function setDefault (t, d)
       local mt = {__index = function () return d end}
       setmetatable(t, mt)
end

local mt = {__index = function (t) return t.___ end}
function setDefault (t, d)
  t.___ = d
  setmetatable(t, mt)
end

tab = {x=10, y=20}
print(tab.x, tab.z)--> 10   nil
setDefault(tab, 0)
print(tab.x, tab.z)--> 10   0


function cannotModifyHp(object)
    local proxy = {}
    local mt = {
        __index = object,
    __newindex = function(t,k,v)
        if k ~= "hp" then
        object[k] = v
        end
    end
    }
    setmetatable(proxy,mt)
    return proxy
end
 
object = {hp = 10,age = 11}
function object.sethp(self,newhp)
    self.hp = newhp
end
 
o = cannotModifyHp(object)
 
o.hp = 100
print(o.hp)
 
o:sethp(111)
print(o.hp)
 
object:sethp(100)
print(o.hp)

-- print('--------print global variables ------------')
-- for n in pairs(_G) do print(n) end
-- print('--------end ------------')

s = "hello world from Lua"
for w in string.gmatch(s, "%a+") do
 print(w)
end

t = {}
s = "from=world, to=Lua"
for k, v in string.gmatch(s, "(%w+)=(%w+)") do
 t[k] = v
end

for k, v in pairs(t) do 
  print(k, v)
end

function getfield (f)
       local v = _G    -- start with the table of globals
       for w in string.gmatch(f, "[%w_]+") do
v = v[w] end
return v end

function setfield (f, v)
       local t = _G            -- start with the table of globals
       for w, d in string.gmatch(f, "([%w_]+)(.?)") do
end
  if d == "." then-- not last field?
    t[w] = t[w] or {}-- create table if absent
    t = t[w]-- get the table
else-- last field
t[w] = v-- do the assignment
end end






