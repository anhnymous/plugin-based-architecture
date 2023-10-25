/***************************************************************************************
* COMPONENT: feel-plugins
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: PluginYImplImpl.cc
* License: GPLv3
*
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************/

#include "PluginYImpl.hh"

PluginProvider* create(void) {
  return new PluginYImpl;
}

void destroy(PluginProvider* py) {
  if (py) delete py;
}

std::vector<giid> PluginYImpl::get_giids() const noexcept {
  return std::vector<giid>{"giid/001", "giid/002"};
}

string PluginYImpl::read(const string& giid) const noexcept {
  if (giid == "giid/003") {
    return std::to_string(m_value);
  } else {
    std::cerr << "== ERROR: PluginYImpl::read(): unrecognized interface ID\n";
    return "";
  }
}

bool PluginYImpl::write(const string& giid, const string& value) {
  if (giid == "giid/004") {
    m_value = std::stof(value);
    return true;
  } else {
    std::cerr << "== ERROR: PluginYImpl::write(): unrecognized interface ID\n";
    return false;
  }
}
