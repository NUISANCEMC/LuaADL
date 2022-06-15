--Check that we have LuaADL
assert(LuaADL)

function filter_and_project(in_GenEvent, out_Projections)

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

  ISnu = LuaADL.sel.part.beam(in_GenEvent, 14)

  if not ISnu then
    print("[CUT]: no IS nu")
    return false 
  end

  print("add proj: ", ISnu.momentum.e)
  out_Projections:add(ISnu.momentum.e)

  nnuc = LuaADL.sel.parts.out(in_GenEvent, {2212, 2112}):size()
  nother = LuaADL.sel.parts.other_out(in_GenEvent, {13, 2212, 2112}):size()
  FSmu = LuaADL.sel.part.out.highest_momentum(in_GenEvent, 13)

  if not FSmu then
    print("[CUT]: no FS muon")
    return false
  end

  print("add proj: ", FSmu.momentum.e)
  out_Projections:add(FSmu.momentum.e)

  if nother > 0 then
    print("[CUT]: other particles: ", nother)

    for _,j in ipairs(LuaADL.sel.parts.other_out(in_GenEvent, {13, 2212, 2112})) do
      print(j:to_str())
    end

    return false
  end

  print("add proj: ", nnuc)
  out_Projections:add(nnuc)

  return true

end