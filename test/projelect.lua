function projelect(in_GenEvent, out_Projections)

  print("projeclecting event: ", in_GenEvent.event_number)

  assert(in_GenEvent:vertices() and in_GenEvent:vertices():size() > 0)
  vert = in_GenEvent:vertices()[1]

  assert(vert)
  assert(vert:attribute_as_int("HardScatterMode"))

  HSM = vert:attribute_as_int("HardScatterMode"):value()

  if not HSM == 1 then
    print("[CUT]: hard scatter mode ", HSM)
    return false 
  end

  assert(vert:particles_in() and vert:particles_in():size() > 0)

  for _,part in ipairs(vert:particles_in()) do
    if part.status == 4 and part.pid == 14 then 
      ISnu = part 
      break 
    end
  end

  if not ISnu then
    print("[CUT]: no IS nu")
    return false 
  end

  print("add proj: ", ISnu.momentum.e)
  out_Projections:add(ISnu.momentum.e)

  nnuc = 0
  nother = 0

  FSmu = nil

  for _,part in ipairs(vert:particles_out()) do
    if part.status == 1 then
      if part.pid == 13 and not FSmu then 
        FSmu = part 
      elseif part.pid == 2212 or part.pid == 2112 then
        nnuc = nnuc + 1
      else
        nother = nother + 1
      end
    end
  end

  if not FSmu then
    print("[CUT]: no FS muon")
    return false
  end

  print("add proj: ", FSmu.momentum.e)
  out_Projections:add(FSmu.momentum.e)

  if nother > 0 then
    print("[CUT]: other particles")
    return false
  end

  print("add proj: ", nnuc)
  out_Projections:add(nnuc)

  return true

end
