#include "HepMC3/LuaInterface.hxx"

#include <cassert>
#include <iostream>

int main(int argc, char const *argv[]) {

  HepMC3::LuaInterface LI;

  LI.lua["attr_int"] = std::make_shared<HepMC3::IntAttribute>(1);
  LI.lua["attr_double"] = std::make_shared<HepMC3::DoubleAttribute>(1.234);
  LI.lua["attr_str"] = std::shared_ptr<HepMC3::Attribute>(
      new HepMC3::StringAttribute("1.23456"));

  LI.lua.safe_script(R"(
    assert(attr_int)
    io.write(string.format("attr_int:value() = %s\n\n",attr_int:value()))
    assert(attr_int:value() == 1)

    assert(attr_double)
    io.write(string.format("attr_double:value() = %s\n\n",attr_double:value()))
    assert(attr_double:value() == 1.234)

    assert(attr_str)
    io.write(string.format("attr_str:to_string() = %s\n\n",attr_str:to_string()))
    assert(attr_str:to_string() == "1.23456")
    )");
}