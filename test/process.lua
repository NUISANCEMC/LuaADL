--Check that we have LuaADL
assert(LuaADL)

function isCC0Pi(in_GenEvent)
  if not LuaADL.sel.part.beam(in_GenEvent, 14) then return false end

  local nnother = LuaADL.sel.parts.other_out(in_GenEvent, {13, 2212, 2112}):size()
  if not nnother == 0 then return false end

  if not LuaADL.sel.part.out.highest_momentum(in_GenEvent, 13) then 
    return false 
  end

  return true
end

function isTrueCCQE(in_GenEvent)
  if not in_GenEvent:vertices() or 
    in_GenEvent:vertices():size() == 0 or
    not in_GenEvent:vertices()[1] then
    return false
  end

  local attr = in_GenEvent:vertices()[1]:attribute_as_int("HardScatterMode")

  return attr and (attr:value() == 1)
end

function filter(in_GenEvent)
  return isTrueCCQE(in_GenEvent) and isCC0Pi(in_GenEvent)
end

function project(in_GenEvent, out_projections)

  local ISnu = LuaADL.sel.part.beam(in_GenEvent, 14)
  print("Enu: ", ISnu.momentum.e)
  out_projections:add(ISnu.momentum.e)

  local FSmu = LuaADL.sel.part.out.highest_momentum(in_GenEvent, 13)
  print("Pmu: ", FSmu.momentum:length())
  out_projections:add(FSmu.momentum:length())

  local q0 = LuaADL.proj.parts.q0(ISnu,FSmu)
  print("q0: ", q0)
  out_projections:add(q0)

  local q3 = LuaADL.proj.parts.q3(ISnu,FSmu)
  print("q3: ", q3)
  out_projections:add(q3)

  local Q2 = LuaADL.proj.parts.Q2(ISnu,FSmu)
  print("Q2: ", Q2)
  out_projections:add(Q2)

  local cpt = LuaADL.proj.parts.pt(LuaADL.sel.parts.out(in_GenEvent, {13, 2212}))
  print("charged pt: ", cpt)
  out_projections:add(cpt)

end