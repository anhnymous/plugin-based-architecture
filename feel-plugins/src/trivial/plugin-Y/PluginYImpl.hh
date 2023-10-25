/***************************************************************************************
* COMPONENT: feel-plugins
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: PluginYImplImpl.hh
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

#ifndef PLUGINYIMPL_H_
#define PLUGINYIMPL_H_

#include "PluginY.hh"

class PluginYImpl: public PluginY
{
public:
  PluginYImpl() = default;
  ~PluginYImpl() = default;

  PluginYImpl(PluginYImpl&&) = delete;
  PluginYImpl(const PluginYImpl&) = delete;
  PluginYImpl& operator=(PluginYImpl&&) = delete;
  PluginYImpl& operator=(const PluginYImpl&) = delete;

  std::vector<giid> get_giids() const noexcept override;

  /**
   * giid/003
   */
  string read(const string& giid) const noexcept override;
 
  /**
   * giid/004
   */
  bool write(const string& giid, const string& value) override;

private:
  float m_value{0.0};  // Y handles for Integers
};

#endif /* PLUGINYIMPL_H_ */
