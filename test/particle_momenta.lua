assert(event)
assert(momenta)

for i,j in ipairs(event:particles()) do
  momenta:insert(i, j.momentum:length())
  print("momenta in lua: ", j.momentum:length())
end