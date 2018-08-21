#include "Command.hpp"

#include "Modules/Tier1.hpp"

Command::Command()
    : ptr(nullptr)
    , shouldRegister(nullptr)
    , isRegistered(false)
    , isReference(false)
{
}
Command::~Command()
{
    if (!isReference) {
        delete ptr;
        ptr = nullptr;
    }
}
Command::Command(const char* name)
{
    this->ptr = reinterpret_cast<ConCommand*>(tier1->FindCommandBase(tier1->g_pCVar->ThisPtr(), name));
    this->isReference = true;
}
Command::Command(const char* pName, _CommandCallback callback, const char* pHelpString, int flags,
    _CommandCompletionCallback completionFunc)
{
    this->ptr = new ConCommand();
    this->ptr->m_pszName = pName;
    this->ptr->m_pszHelpString = pHelpString;
    this->ptr->m_nFlags = flags;
    this->ptr->m_fnCommandCallback = callback;
    this->ptr->m_fnCompletionCallback = completionFunc;
    this->ptr->m_bHasCompletionCallback = completionFunc != nullptr;
    this->ptr->m_bUsingNewCommandCallback = true;

    Command::list.push_back(this);
}
ConCommand* Command::ThisPtr()
{
    return this->ptr;
}
void Command::UniqueFor(_ShouldRegisterCallback callback)
{
    this->shouldRegister = callback;
}
void Command::Register()
{
    if (!this->isRegistered) {
        this->ptr->ConCommandBase_VTable = tier1->ConCommand_VTable;
        tier1->RegisterConCommand(tier1->g_pCVar->ThisPtr(), this->ptr);
        tier1->m_pConCommandList = this->ptr;
    }
    this->isRegistered = true;
}
void Command::Unregister()
{
    if (this->isRegistered) {
        tier1->UnregisterConCommand(tier1->g_pCVar->ThisPtr(), this->ptr);
    }
    this->isRegistered = false;
}
bool Command::operator!()
{
    return this->ptr == nullptr;
}
int Command::RegisterAll()
{
    auto result = 0;
    for (const auto& command : Command::list) {
        if (command->shouldRegister && !command->shouldRegister()) {
            continue;
        }
        command->Register();
        ++result;
    }
    return result;
}
void Command::UnregisterAll()
{
    for (const auto& command : Command::list) {
        command->Unregister();
    }
}
Command* Command::Find(const char* name)
{
    for (const auto& command : Command::list) {
        if (!std::strcmp(command->ThisPtr()->m_pszName, name)) {
            return command;
        }
    }
    return nullptr;
}

std::vector<Command*> Command::list;
