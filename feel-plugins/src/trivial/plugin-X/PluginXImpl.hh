/***************************************************************************************
* COMPONENT: feel-plugins
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: PluginXImplImpl.hh
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

#ifndef PLUGINXIMPL_H_
#define PLUGINXIMPL_H_

#include "PluginX.hh"

class PluginXImpl: public PluginX
{
public:
  PluginXImpl() = default;
  ~PluginXImpl() = default;

  PluginXImpl(PluginXImpl&&) = delete;
  PluginXImpl(const PluginXImpl&) = delete;
  PluginXImpl& operator=(PluginXImpl&&) = delete;
  PluginXImpl& operator=(const PluginXImpl&) = delete;

  std::vector<giid> get_giids() const noexcept override;

  /**
   * giid/001
   */
  string read(const string& giid) const noexcept override;
 
  /**
   * giid/002
   */
  bool write(const string& giid, const string& value) override;

private:
  int m_value{0};  // X handles for Integers
};

#endif /* PLUGINXIMPL_H_ */
