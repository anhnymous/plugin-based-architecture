/***************************************************************************************
* COMPONENT: feel-plugins
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: PluginXImplImpl.cc
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
#include "PluginXImpl.hh"

PluginProvider* create(void) {
  return new PluginXImpl;
}

void destroy(PluginProvider* px) {
  if (px) delete px;
}

std::vector<giid> PluginXImpl::get_giids() const noexcept {
  return std::vector<giid>{"giid/001", "giid/002"};
}

string PluginXImpl::read(const string& giid) const noexcept {
  if (giid == "giid/001") {
    return std::to_string(m_value);
  } else {
    std::cerr << "== ERROR: PluginXImpl::read(): unrecognized interface ID\n";
    return "";
  }
}

bool PluginXImpl::write(const string& giid, const string& value) {
  if (giid == "giid/002") {
    m_value = std::stoi(value);
    return true;
  } else {
    std::cerr << "== ERROR: PluginXImpl::write(): unrecognized interface ID\n";
    return false;
  }
}
