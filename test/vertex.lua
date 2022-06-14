assert(vert)

for i, part in ipairs(vert:particles_in()) do
  
  assert(part)

  io.write(string.format("%d: %s\n", i, part:to_str()))
end