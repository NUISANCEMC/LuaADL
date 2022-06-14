verts = event:vertices()
assert(verts)

vert = verts[1]
assert(vert)

for i,att_name in ipairs(vert:attribute_names()) do
  io.write(string.format("\t%d: %s = %s\n", 
    i, att_name, vert:attribute_as_string(att_name)))
end