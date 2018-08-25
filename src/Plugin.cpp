#include "Plugin.hpp"

#include "Interface.hpp"
#include "Platform.hpp"
#include "SDK.hpp"

Plugin::Plugin()
    : ptr(nullptr)
    , index(0)
{
}
bool Plugin::Find()
{
    auto s_ServerPlugin = Interface::Create(MODULE("engine"), INTERFACEVERSION_ISERVERPLUGINHELPERS, false);
    if (s_ServerPlugin) {
        auto m_Size = *reinterpret_cast<int*>((uintptr_t)s_ServerPlugin->ThisPtr() + CServerPlugin_m_Size);
        if (m_Size > 0) {
            auto m_Plugins = *reinterpret_cast<uintptr_t*>((uintptr_t)s_ServerPlugin->ThisPtr() + CServerPlugin_m_Plugins);
            for (int i = 0; i < m_Size; i++) {
                auto ptr = *reinterpret_cast<CPlugin**>(m_Plugins + sizeof(uintptr_t) * i);
                if (!std::strcmp(ptr->m_szName, SGP_SIGNATURE)) {
                    this->ptr = ptr;
                    this->index = i;
                    return true;
                }
            }
        }
    }
    return false;
}
