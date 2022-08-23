#pragma once

#include "HepMC3/Attribute.h"
#include "HepMC3/Print.h"

#include "sol/sol.hpp"

namespace HepMC3 {

struct AttributeTypes {
  sol::usertype<Attribute> Attribute_type;
  sol::usertype<IntAttribute> IntAttribute_type;
  sol::usertype<DoubleAttribute> DoubleAttribute_type;
};

inline AttributeTypes RegisterAttributes(sol::state &lua) {
  AttributeTypes types;
  types.Attribute_type = lua.new_usertype<Attribute>("Attribute");
  types.Attribute_type.set_function("to_string",
                                    [](Attribute const &att) -> std::string {
                                      std::string str;
                                      att.to_string(str);
                                      return str;
                                    });

  types.IntAttribute_type = lua.new_usertype<IntAttribute>(
      "IntAttribute", sol::base_classes, sol::bases<Attribute>());
  types.IntAttribute_type.set_function(
      "to_string", [](IntAttribute const &att) -> std::string {
        std::string str;
        att.to_string(str);
        return str;
      });
  types.IntAttribute_type.set_function("value", &IntAttribute::value);

  types.DoubleAttribute_type = lua.new_usertype<DoubleAttribute>(
      "DoubleAttribute", sol::base_classes, sol::bases<Attribute>());
  types.DoubleAttribute_type.set_function(
      "to_string", [](DoubleAttribute const &att) -> std::string {
        std::string str;
        att.to_string(str);
        return str;
      });
  types.DoubleAttribute_type.set_function("value", &DoubleAttribute::value);

  return types;
}

template <typename T>
void DecorateWithAttributeCastHelpers(sol::usertype<T> &ut) {
  ut["attribute_names"] =
      sol::resolve<std::vector<std::string>() const>(&T::attribute_names);

  ut["attribute_as_string"] = &T::attribute_as_string;

  ut["attribute_as_int"] = &T::template attribute<IntAttribute>;
  ut["attribute_as_double"] = &T::template attribute<DoubleAttribute>;
}

} // namespace HepMC3