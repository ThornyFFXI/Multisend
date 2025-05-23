#include "Multisend.h"

bool Multisend::ReadCommand()
{
	if (p_MMF->Command.Command[s_position].active)
	{
		if (CheckMatch(p_MMF->Command.Command[s_position]))
		{
			char text[256] = {0};
			memcpy(text, &(p_MMF->Command.Command[s_position].command), 256);
			if (c_debug)
			{
				m_AshitaCore->GetChatManager()->Writef(0, false, "Sending command: %s", text);
			}
			if (m_AshitaCore->GetPluginManager()->Get("Shorthand"))
			{
				m_AshitaCore->GetChatManager()->QueueCommand(-1, text);
			}
			else
			{
				m_AshitaCore->GetChatManager()->QueueCommand(0, text);
			}
		}
		s_position++;
		if (s_position == 100) s_position = 0;
		return true;
	}
	return false;
}

void Multisend::SendCommand(multisend_type type, uint32_t param, const char* Command)
{
    char commandText[248] = {0};
	memcpy(commandText, Command, strlen(Command));
	if (c_safemode)
		SanitizeCommand(commandText);
	SubValues(commandText);
	Claim(&(p_MMF->Command.ProcessID), 0);

	int NextPosition = p_MMF->Command.Position + 1;
	if (NextPosition == 100) NextPosition = 0;

	p_MMF->Command.Command[NextPosition].active = false;

	memset(&(p_MMF->Command.Command[p_MMF->Command.Position].command), 0, 248);
	memcpy(&(p_MMF->Command.Command[p_MMF->Command.Position].command), commandText, min(strlen(commandText), 247));
	if (c_debug)
	{
		m_AshitaCore->GetChatManager()->Writef(0, false, "Publishing position %d : %s", p_MMF->Command.Position, commandText);
	}
	p_MMF->Command.Command[p_MMF->Command.Position].sender_process_id = GetCurrentProcessId();
	p_MMF->Command.Command[p_MMF->Command.Position].type = type;
	p_MMF->Command.Command[p_MMF->Command.Position].param = param;
	p_MMF->Command.Command[p_MMF->Command.Position].active = true;
	p_MMF->Command.Position = NextPosition;
	InterlockedExchange(&(p_MMF->Command.ProcessID), 0);
}

void Multisend::UpdateName(std::string Name)
{
	Claim(&(p_MMF->Name.ProcessID), 0xFFFF0000);
	bool Written = false;
	for (int x = 0; x < 100; x++)
	{
		if (!p_MMF->Name.Names[x].Active)
		{
			if (!Written)
			{
				memcpy(&(p_MMF->Name.Names[x].Name), Name.c_str(), strlen(Name.c_str()) + 1);
				p_MMF->Name.Names[x].Process = GetCurrentProcessId();
				p_MMF->Name.Names[x].Active = 1;
				Written = true;
			}
			continue;
		}

		else if (p_MMF->Name.Names[x].Process == GetCurrentProcessId())
		{
			memset(&(p_MMF->Name.Names[x]), 0, sizeof(MMF_Name_Single));
			if (!Written)
			{
				memcpy(&(p_MMF->Name.Names[x].Name), Name.c_str(), strlen(Name.c_str()) + 1);
				p_MMF->Name.Names[x].Process = GetCurrentProcessId();
				p_MMF->Name.Names[x].Active = 1;
				Written = true;
			}
		}

		else if (strcmp((const char*)&(p_MMF->Name.Names[x].Name), Name.c_str()) == 0)
		{
			memset(&(p_MMF->Name.Names[x]), 0, sizeof(MMF_Name_Single));
			if (!Written)
			{
				memcpy(&(p_MMF->Name.Names[x].Name), Name.c_str(), strlen(Name.c_str()) + 1);
				p_MMF->Name.Names[x].Process = GetCurrentProcessId();
				p_MMF->Name.Names[x].Active = 1;
				Written = true;
			}
		}
	}
	s_name = Name;
	MatchGroups();
	InterlockedExchange(&(p_MMF->Name.ProcessID), 0);
}

void Multisend::Claim(uint32_t* Target, uint32_t Mod)
{
	uint32_t val = GetCurrentProcessId() + Mod;
	int FailCount = 0;
	InterlockedCompareExchange(Target, val, 0);

	while (*Target != val)
	{
		FailCount++;
		if (FailCount == 50)
		{
			InterlockedExchange(Target, val);
			return;
		}
		::Sleep(1);
		InterlockedCompareExchange(Target, val, 0);
	}
}