#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"

#include "LuaADL/Projectors.hxx"

namespace LuaADL {

double q0_2parts(HepMC3::ConstGenParticlePtr p1,
                 HepMC3::ConstGenParticlePtr p2) {
  if (!p1 || !p2) {
    return 0xdeadbeef;
  }

  return (p2->momentum() - p1->momentum()).e();
}

double q0(std::vector<HepMC3::ConstGenParticlePtr> parts) {
  if (parts.size() < 0) {
    return 0xdeadbeef;
  }

  return q0_2parts(parts[0], parts[1]);
}

double q3_2parts(HepMC3::ConstGenParticlePtr p1,
                 HepMC3::ConstGenParticlePtr p2) {
  if (!p1 || !p2) {
    return 0xdeadbeef;
  }

  return (p2->momentum() - p1->momentum()).length();
}

double q3(std::vector<HepMC3::ConstGenParticlePtr> parts) {
  if (parts.size() < 0) {
    return 0xdeadbeef;
  }

  return q3_2parts(parts[0], parts[1]);
}

double Q2_2parts(HepMC3::ConstGenParticlePtr p1,
                 HepMC3::ConstGenParticlePtr p2) {
  if (!p1 || !p2) {
    return 0xdeadbeef;
  }

  return -(p2->momentum() - p1->momentum()).m2();
}

double Q2(std::vector<HepMC3::ConstGenParticlePtr> parts) {
  if (parts.size() < 0) {
    return 0xdeadbeef;
  }

  return Q2_2parts(parts[0], parts[1]);
}

double costheta_2parts(HepMC3::ConstGenParticlePtr p1,
                       HepMC3::ConstGenParticlePtr p2) {
  if (!p1 || !p2) {
    return 0xdeadbeef;
  }

  auto const &p1mom = p1->momentum();
  auto const &p2mom = p2->momentum();

  double dotp =
      p1mom.x() * p2mom.x() + p1mom.y() * p2mom.y() + p1mom.z() * p2mom.z();

  return dotp / (p1mom.length() * p2mom.length());
}

double costheta(std::vector<HepMC3::ConstGenParticlePtr> parts) {
  if (parts.size() < 0) {
    return 0xdeadbeef;
  }
  return costheta_2parts(parts[0], parts[1]);
}

double theta_2parts(HepMC3::ConstGenParticlePtr p1,
                    HepMC3::ConstGenParticlePtr p2) {
  if (!p1 || !p2) {
    return 0xdeadbeef;
  }

  return std::acos(costheta_2parts(p1, p2));
}

double theta(std::vector<HepMC3::ConstGenParticlePtr> parts) {
  if (parts.size() < 0) {
    return 0xdeadbeef;
  }

  return theta_2parts(parts[0], parts[1]);
}

double W(std::vector<HepMC3::ConstGenParticlePtr> parts) {
  HepMC3::FourVector fv;

  for (auto &p : parts) {
    if (!p) {
      return 0xdeadbeef;
    }
    fv += p->momentum();
  }

  return fv.m();
}

double pt(std::vector<HepMC3::ConstGenParticlePtr> parts) {
  HepMC3::FourVector fv;

  for (auto &p : parts) {
    if (!p) {
      return 0xdeadbeef;
    }
    fv += p->momentum();
  }

  return fv.perp();
}

HepMC3::FourVector pmiss(std::vector<HepMC3::ConstGenParticlePtr> parts_in,
                         std::vector<HepMC3::ConstGenParticlePtr> parts_out) {
  HepMC3::FourVector fv_in, fv_out;

  for (auto &p : parts_in) {
    if (!p) {
      return HepMC3::FourVector{0xdeadbeef, 0, 0, 0};
    }
    fv_in += p->momentum();
  }

  for (auto &p : parts_out) {
    if (!p) {
      return HepMC3::FourVector{0xdeadbeef, 0, 0, 0};
    }
    fv_out += p->momentum();
  }

  return fv_out - fv_in;
}

void Projectors::RegisterLuaFunctions(sol::state &lua) {
  if (!lua["LuaADL"].valid()) {
    lua["LuaADL"] = lua.create_table();
  }

  lua["LuaADL"]["proj"] = lua.create_table();
  lua["LuaADL"]["proj"]["parts"] = lua.create_table();

  lua["LuaADL"]["proj"]["parts"]["q0"] = sol::overload(&q0_2parts, &q0);
  lua["LuaADL"]["proj"]["parts"]["q3"] = sol::overload(&q3_2parts, &q3);
  lua["LuaADL"]["proj"]["parts"]["Q2"] = sol::overload(&Q2_2parts, &Q2);
  lua["LuaADL"]["proj"]["parts"]["costheta"] =
      sol::overload(&costheta_2parts, &costheta);
  lua["LuaADL"]["proj"]["parts"]["theta"] =
      sol::overload(&theta_2parts, &theta);

  lua["LuaADL"]["proj"]["parts"]["W"] = &W;
  lua["LuaADL"]["proj"]["parts"]["pt"] = &pt;
  lua["LuaADL"]["proj"]["parts"]["pmiss"] = &pmiss;
}

} // namespace LuaADL