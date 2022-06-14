assert(fourvector)

print(fourvector:to_str())

io.write(string.format("x: %s\n", fourvector.x))
io.write(string.format("y: %s\n", fourvector.y))
io.write(string.format("z: %s\n", fourvector.z))
io.write(string.format("t: %s\n", fourvector.t))

io.write(string.format("px: %s\n", fourvector.px))
io.write(string.format("py: %s\n", fourvector.py))
io.write(string.format("pz: %s\n", fourvector.pz))
io.write(string.format("e: %s\n", fourvector.e))

io.write(string.format("length2: %s\n", fourvector:length2()))
io.write(string.format("length: %s\n", fourvector:length()))
io.write(string.format("m2: %s\n", fourvector:m2()))
io.write(string.format("m: %s\n", fourvector:m()))
io.write(string.format("phi: %s\n", fourvector:phi()))
io.write(string.format("theta: %s\n", fourvector:theta()))
io.write(string.format("is_zero: %s\n", fourvector:is_zero()))







