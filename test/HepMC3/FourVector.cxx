#include "HepMC3/LuaInterface.hxx"

#include <cassert>
#include <iostream>

int main(int argc, char const *argv[]) {

  sol::state lua;
  lua.open_libraries();
  HepMC3::LuaInterface LI;
  LI.RegisterLuaFunctions(lua);

  lua["fvtest"] = HepMC3::FourVector(1.234,5.678,9.101,1.121);
  lua["fvxy"] = HepMC3::FourVector(3,4,0,0);
  lua["fvxe"] = HepMC3::FourVector(4,0,0,5);
  lua["fvxz"] = HepMC3::FourVector(0,1,1,0);
  lua["fvzero"] = HepMC3::FourVector();

  lua.safe_script(R"(
    io.write(string.format("fvtest.x = %s\n", fvtest.x))
    assert(fvtest.x == 1.234)
    io.write(string.format("fvtest.y = %s\n", fvtest.y))
    assert(fvtest.y == 5.678)
    io.write(string.format("fvtest.z = %s\n", fvtest.z))
    assert(fvtest.z == 9.101)
    io.write(string.format("fvtest.t = %s\n", fvtest.t))
    assert(fvtest.t == 1.121)

    io.write(string.format("fvtest.px = %s\n", fvtest.px))
    assert(fvtest.px == 1.234)
    io.write(string.format("fvtest.py = %s\n", fvtest.py))
    assert(fvtest.py == 5.678)
    io.write(string.format("fvtest.pz = %s\n", fvtest.pz))
    assert(fvtest.pz == 9.101)
    io.write(string.format("fvtest.e = %s\n", fvtest.e))
    assert(fvtest.e == 1.121)

    io.write(string.format("fvxy:length2() = %s\n", fvxy:length2()))
    assert(fvxy:length2() == 25)
    io.write(string.format("fvxy:length() = %s\n", fvxy:length()))
    assert(fvxy:length() == 5)

    io.write(string.format("fvxe:m2() = %s\n", fvxe:m2()))
    assert(fvxe:m2() == 9)
    io.write(string.format("fvxe:m() = %s\n", fvxe:m()))
    assert(fvxe:m() == 3)

    io.write(string.format("fvxz:theta() = %s\n", fvxz:theta()))
    assert(fvxz:theta() == math.atan(1))
    io.write(string.format("fvxz:phi() = %s\n", fvxz:phi()))
    assert(fvxz:phi() == math.acos(0))
    
    io.write(string.format("fvzero:is_zero() = %s\n", fvzero:is_zero()))
    assert(fvzero:is_zero())

    print(fvtest:to_str())
    )");
}