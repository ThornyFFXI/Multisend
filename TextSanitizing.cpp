#include "Multisend.h"

std::string Multisend::SubValues(std::string Input)
{
	std::string Working = Input;

	size_t find = Working.find("[me]");
	if (find != std::string::npos)
	{
		Working.replace(find, 4, std::string(m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberName(0)));
	}

	find = Working.find("[p0]");
	if (find != std::string::npos)
	{
		Working.replace(find, 4, std::string(m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberName(0)));
	}

	find = Working.find("[p1]");
	if (find != std::string::npos)
	{
		Working.replace(find, 4, std::string(m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberName(1)));
	}

	find = Working.find("[p2]");
	if (find != std::string::npos)
	{
		Working.replace(find, 4, std::string(m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberName(2)));
	}

	find = Working.find("[p3]");
	if (find != std::string::npos)
	{
		Working.replace(find, 4, std::string(m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberName(3)));
	}

	find = Working.find("[p4]");
	if (find != std::string::npos)
	{
		Working.replace(find, 4, std::string(m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberName(4)));
	}

	find = Working.find("[p5]");
	if (find != std::string::npos)
	{
		Working.replace(find, 4, std::string(m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberName(5)));
	}

	find = Working.find("[t]");
	if (find != std::string::npos)
	{
		unsigned int TargetIndex = m_AshitaCore->GetMemoryManager()->GetTarget()->GetTargetIndex(m_AshitaCore->GetMemoryManager()->GetTarget()->GetIsSubTargetActive());
		Working.replace(find, 3, std::to_string(m_AshitaCore->GetMemoryManager()->GetEntity()->GetServerId(TargetIndex)));
	}
	return Working;
}

void Multisend::SubValues(char* Input)
{
	std::string Working = SubValues(std::string(Input));
	const char* base = Working.c_str();
	memset(Input, 0, 248);
	memcpy(Input, base, strlen(base));
}

void Multisend::SanitizeCommand(char* Input)
{
	if (Input[0] != '/')
	{
        if (c_debug)
        {
            m_AshitaCore->GetChatManager()->Write(0, false, "Detected command without leading '/'. Prepending '/echo'...");
        }
        char buffer[248] = {0};
        sprintf_s(buffer, 248, "/echo %s", Input);
        memcpy(Input, buffer, 248);
	}
	else
	{
		if (c_debug)
			m_AshitaCore->GetChatManager()->Write(0, false, "No Sanitization Needed.");
	}

}