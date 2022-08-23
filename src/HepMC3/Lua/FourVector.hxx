#pragma once

#include "HepMC3/FourVector.h"
#include "HepMC3/Print.h"

#include "sol/sol.hpp"

namespace HepMC3 {

inline sol::usertype<FourVector> RegisterFourVector(sol::state &lua) {
  sol::usertype<FourVector> FourVector_type = lua.new_usertype<FourVector>(
      "FourVector",
      sol::constructors<FourVector(),
                        FourVector(double, double, double, double)>());

  //-------------- Metamethod Overrides ---------------------
  FourVector_type[sol::meta_function::addition] = &FourVector::operator+;
  FourVector_type[sol::meta_function::subtraction] = &FourVector::operator-;

  //-------------- Property Accessors ---------------------
  FourVector_type["x"] = sol::property(&FourVector::x, &FourVector::set_x);
  FourVector_type["y"] = sol::property(&FourVector::y, &FourVector::set_y);
  FourVector_type["z"] = sol::property(&FourVector::z, &FourVector::set_z);
  FourVector_type["t"] = sol::property(&FourVector::t, &FourVector::set_t);
  FourVector_type["px"] = sol::property(&FourVector::px, &FourVector::set_px);
  FourVector_type["py"] = sol::property(&FourVector::py, &FourVector::set_py);
  FourVector_type["pz"] = sol::property(&FourVector::pz, &FourVector::set_pz);
  FourVector_type["e"] = sol::property(&FourVector::e, &FourVector::set_e);

  //-------------- Convenience methods---------------------
  FourVector_type["length2"] = &FourVector::length2;
  FourVector_type["length"] = &FourVector::length;
  FourVector_type["m2"] = &FourVector::m2;
  FourVector_type["m"] = &FourVector::m;
  FourVector_type["phi"] = &FourVector::phi;
  FourVector_type["theta"] = &FourVector::theta;
  FourVector_type["is_zero"] = &FourVector::is_zero;

  //-------------- Extra Lua methods---------------------
  FourVector_type["to_str"] = [](FourVector const &self) {
    std::stringstream ss("");
    Print::line(ss, self);
    return ss.str();
  };

  return FourVector_type;
}

} // namespace HepMC3