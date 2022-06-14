#pragma once

#include "HepMC3/GenRunInfo.h"
#include "HepMC3/Print.h"

#include "HepMC3/Lua/Attribute.hxx"

#include "sol/sol.hpp"

namespace HepMC3 {

struct GenRunInfoTypes {
  sol::usertype<GenRunInfo> GenRunInfo_type;
  sol::usertype<GenRunInfo::ToolInfo> ToolInfo_type;
};

inline GenRunInfoTypes RegisterGenRunInfo(sol::state &lua) {
  GenRunInfoTypes types;

  types.ToolInfo_type = lua.new_usertype<GenRunInfo::ToolInfo>(
      "ToolInfo", sol::constructors<GenRunInfo::ToolInfo()>(), "name",
      &GenRunInfo::ToolInfo::name, "version", &GenRunInfo::ToolInfo::version,
      "description", &GenRunInfo::ToolInfo::description);

  types.GenRunInfo_type = lua.new_usertype<GenRunInfo>(
      "GenRunInfo", sol::constructors<GenRunInfo()>());

  //-------------- Convenience methods---------------------
  types.GenRunInfo_type["tools"] =
      sol::resolve<const std::vector<GenRunInfo::ToolInfo> &() const>(
          &GenRunInfo::tools);

  //-------------- Attribute helpers---------------------
  DecorateWithAttributeCastHelpers(types.GenRunInfo_type);

  //-------------- Extra Lua methods---------------------
  types.GenRunInfo_type["to_str"] = [](GenRunInfo const &self) -> std::string {
    std::stringstream ss;
    Print::listing(ss, self, true);
    return ss.str();
  };

  return types;
}

} // namespace HepMC3