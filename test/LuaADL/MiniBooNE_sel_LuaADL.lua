function filter_and_project(in_GenEvent, out_projections)

  local ISnu = LuaADL.sel.part.beam(in_GenEvent, 14)
  if not ISnu then return false end

  local FSmus = LuaADL.sel.parts.out(in_GenEvent, 13)
  if not (FSmus:size() == 1) then return false end

  local nnother = LuaADL.sel.parts.other_out(in_GenEvent, {13, 2212, 2112}):size()
  if nnother > 0 then return false end

  out_projections:add(FSmus[1].momentum:length())
  out_projections:add(FSmus[1].momentum:theta())

  return true
  
end