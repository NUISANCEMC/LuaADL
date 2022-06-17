function filter_and_project(in_GenEvent, out_projections)

  local ISnu = false
  local FSmu = false

  for _,i in ipairs(in_GenEvent:particles()) do
    if i.pid == 14 and i.status == 4 then
      ISnu = i
    end

    if i.status == 1 then
      if (i.pid == 13) then
        if FSmu then return false end
        FSmu = i
      elseif not (i.pid == 2212 or i.pid == 2112) then
        return false
      end
    end
  end

  if not ISnu then return false end
  if not FSmu then return false end

  out_projections:add(FSmu.momentum:length())
  out_projections:add(FSmu.momentum:theta())
  
  return true
end