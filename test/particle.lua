assert(part)

print(part:to_str())

print()

io.write(string.format("pid: %s\n", part.pid))
io.write(string.format("abs_pid: %s\n", part:abs_pid()))
io.write(string.format("status: %s\n", part.status))
io.write(string.format("momentum: %s\n", part.momentum:to_str()))

io.write(string.format("generated_mass: %f\n", part.generated_mass))

io.write(string.format("in_event: %s\n", part:in_event()))
io.write(string.format("id: %s\n", part:id()))

io.write(string.format("production_vertex: %s\n", part:production_vertex()))
io.write(string.format("end_vertex: %s\n", part:end_vertex()))

io.write(string.format("parents:size(): %s\n", part:parents():size()))
io.write(string.format("children:size(): %s\n", part:children():size()))

io.write(string.format("is_generated_mass_set: %s\n", part:is_generated_mass_set()))

io.write(string.format("parent_event: %s\n", part:parent_event()))

return 0;