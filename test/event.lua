assert(event)

print(event:to_str())

verts = event:vertices()
assert(verts)

vert = verts[1]