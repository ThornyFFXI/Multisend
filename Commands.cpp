#include "Multisend.h"

bool Multisend::HandleCommand(int32_t mode, const char* command, bool injected)
{
	std::string arg;
	const char* com = command + GetArg(command, &arg);
	if ((_strnicmp(command, "/ms", 3))
		&& (_stricmp(arg.c_str(), "/multisend")))
	{
		return false;
	}

	if (strlen(com) < 2) return true;
	if ((_stricmp(arg.c_str(), "/ms") == 0)
		|| (_stricmp(arg.c_str(), "/multisend")) == 0)
	{
		com++;
		com += GetArg(com, &arg);
	}

	if ((_stricmp(arg.c_str(), "send") == 0)
		|| (_stricmp(arg.c_str(), "/mss") == 0))
	{
		if (strlen(com) < 2) return true;
		SendCommand(multisend_type::all, 0, com + 1);
		return true;
	}

	else if ((_stricmp(arg.c_str(), "sendto") == 0)
		|| (_stricmp(arg.c_str(), "/mst") == 0))
	{
		if (strlen(com) < 2)
		{
			m_AshitaCore->GetChatManager()->Write(0, false, "Multisend: Invalid command.");
			return true;
		}
		com++;
		com += GetArg(com, &arg);

		arg = SubValues(arg);

		Claim(&(p_MMF->Name.ProcessID), 0);
		for (int x = 0; x < 100; x++)
		{
			if (p_MMF->Name.Names[x].Active)
			{
				if (_stricmp((const char*)&(p_MMF->Name.Names[x].Name), arg.c_str()) == 0)
				{
					SendCommand(multisend_type::single, p_MMF->Name.Names[x].Process, com + 1);
					InterlockedExchange(&(p_MMF->Name.ProcessID), 0);
					return true;
				}
			}
		}

		InterlockedExchange(&(p_MMF->Name.ProcessID), 0);
		m_AshitaCore->GetChatManager()->Write(0, false, "Multisend: Character not found.");
		return true;
	}

	else if ((_stricmp(arg.c_str(), "sendothers") == 0)
		|| (_stricmp(arg.c_str(), "/mso") == 0))
	{
		if (strlen(com) < 2) return true;
		SendCommand(multisend_type::others, ::GetCurrentProcessId(), com + 1);
		return true;
	}

	else if ((_stricmp(arg.c_str(), "sendparty") == 0)
		|| (_stricmp(arg.c_str(), "/msp") == 0))
	{
		if (strlen(com) < 2) return true;
		SendCommand(multisend_type::party, m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberServerId(0), com + 1);
		return true;
	}

	else if ((_stricmp(arg.c_str(), "sendally") == 0)
		|| (_stricmp(arg.c_str(), "/msa") == 0))
	{
		if (strlen(com) < 2) return true;
		SendCommand(multisend_type::alliance, m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberServerId(0), com + 1);
		return true;
	}

	else if ((_stricmp(arg.c_str(), "sendgroup") == 0)
		|| (_stricmp(arg.c_str(), "/msg") == 0))
	{
		if (strlen(com) < 3)
		{
			m_AshitaCore->GetChatManager()->Write(0, false, "Multisend: Invalid command.");
			return true;
		}
		com++;
		com += GetArg(com, &arg);

		for (std::map<uint16_t, std::string>::iterator it = s_group.Map.begin(); it != s_group.Map.end(); it++)
		{
			if (_stricmp((*it).second.c_str(), arg.c_str()) == 0)
			{
				SendCommand(multisend_type::group, (*it).first, com + 1);
				return true;
			}
		}

		m_AshitaCore->GetChatManager()->Write(0, false, "Multisend: Group not found.");
		return true;
	}

	else if (_stricmp(arg.c_str(), "followme") == 0)
	{
		if (strlen(com) < 2) return true;
		com++;
		com += GetArg(com, &arg);

		bool toggle = !(p_MMF->Follow.target_process_id == GetCurrentProcessId());
		if (_stricmp(arg.c_str(), "on") == 0) toggle = true;
		else if (_stricmp(arg.c_str(), "off") == 0) toggle = false;

		if (toggle)
		{
			InterlockedExchange(&(p_MMF->Follow.target_process_id), GetCurrentProcessId());
			uint16_t zone = m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberZone(0);
			uint16_t myindex = m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberTargetIndex(0);
			if ((zone != 0) && (myindex != 0))
			{
				p_MMF->Follow.zone = zone;
				p_MMF->Follow.position_x = m_AshitaCore->GetMemoryManager()->GetEntity()->GetLocalPositionX(myindex);
				p_MMF->Follow.position_z = m_AshitaCore->GetMemoryManager()->GetEntity()->GetLocalPositionY(myindex);
				p_MMF->Follow.idle_count = 0;
			}
			else
			{
				p_MMF->Follow.zone = 0;
			}
		}
		else
		{
			p_MMF->Follow.zone = 0;
			InterlockedExchange(&(p_MMF->Follow.target_process_id), 0);
		}

		m_AshitaCore->GetChatManager()->Writef(0, false, "Multisend: Followme is currently %s.", (p_MMF->Follow.target_process_id == GetCurrentProcessId()) ? "enabled" : "disabled");
		return true;
	}

	else if (_stricmp(arg.c_str(), "followmax") == 0)
	{
		if (strlen(com) < 2) return true;
		com++;
		com += GetArg(com, &arg);

		float dist = atof(arg.c_str());
		if (dist < 1.0f) dist = 1.0f;
		else if (dist > 5000.0f) dist = 5000.0f;
		c_maxdist = dist;

		m_AshitaCore->GetChatManager()->Writef(0, false, "Multisend: Maximum follow distance is currently %f.", c_maxdist);
		return true;
	}

	else if (_stricmp(arg.c_str(), "follow") == 0)
	{
		if (strlen(com) < 2) return true;
		com++;
		com += GetArg(com, &arg);

		if (_stricmp(arg.c_str(), "on") == 0)
		{
			c_follow = true;
		}
		else if (_stricmp(arg.c_str(), "off") == 0)
		{
			c_follow = false;
		}
		else
		{
			c_follow = !c_follow;
		}

		m_AshitaCore->GetChatManager()->Writef(0, false, "Multisend: Follow is currently %s.", c_follow ? "enabled" : "disabled");
		return true;
	}

	else if (_stricmp(arg.c_str(), "attemptzone") == 0)
	{
		if (strlen(com) < 2) return true;
		com++;
		com += GetArg(com, &arg);

		if (_stricmp(arg.c_str(), "on") == 0)
		{
			c_attemptzone = true;
		}
		else if (_stricmp(arg.c_str(), "off") == 0)
		{
			c_attemptzone = false;
		}
		else
		{
			c_attemptzone = !c_attemptzone;
		}

		m_AshitaCore->GetChatManager()->Writef(0, false, "Multisend: Attempted zoning currently %s.", c_attemptzone ? "enabled" : "disabled");
		return true;
	}

	else if (_stricmp(arg.c_str(), "ignoreself") == 0)
	{
		if (strlen(com) < 2) return true;
		com++;
		com += GetArg(com, &arg);

		if (_stricmp(arg.c_str(), "on") == 0)
		{
			c_ignoreself = true;
		}
		else if (_stricmp(arg.c_str(), "off") == 0)
		{
			c_ignoreself = false;
		}
		else
		{
			c_ignoreself = !c_ignoreself;
		}

		m_AshitaCore->GetChatManager()->Writef(0, false, "Multisend: Ignore self is currently %s.", c_ignoreself ? "enabled" : "disabled");
		return true;
	}

	else if (_stricmp(arg.c_str(), "debug") == 0)
	{
		if (strlen(com) < 2) return true;
		com++;
		com += GetArg(com, &arg);

		if (_stricmp(arg.c_str(), "on") == 0)
		{
			c_debug = true;
		}
		else if (_stricmp(arg.c_str(), "off") == 0)
		{
			c_debug = false;
		}
		else
		{
			c_debug = !c_debug;
		}

		m_AshitaCore->GetChatManager()->Writef(0, false, "Multisend: Debug prints currently %s.", c_debug ? "enabled" : "disabled");
		return true;
	}

	else if (_stricmp(arg.c_str(), "safemode") == 0)
	{
		if (strlen(com) < 2) return true;
		com++;
		com += GetArg(com, &arg);

		if (_stricmp(arg.c_str(), "on") == 0)
		{
			c_safemode = true;
		}
		else if (_stricmp(arg.c_str(), "off") == 0)
		{
			c_safemode = false;
		}
		else
		{
			c_safemode = !c_safemode;
		}

		m_AshitaCore->GetChatManager()->Writef(0, false, "Multisend: Safe mode currently %s.", c_safemode ? "enabled" : "disabled");
		return true;
	}

	else if (_stricmp(arg.c_str(), "reload") == 0)
	{
		LoadGroups();
		return true;
	}

	else if (_stricmp(arg.c_str(), "help") == 0)
	{
		m_AshitaCore->GetChatManager()->Write(0, false, "Multisend Command Listing");
		m_AshitaCore->GetChatManager()->Write(0, false, "/ms send [command] - Sends [command] to all characters with multisend loaded.");
		m_AshitaCore->GetChatManager()->Write(0, false, "/ms sendto [char name] [command] - Sends [command] to all characters with multisend loaded named [char name].");
		m_AshitaCore->GetChatManager()->Write(0, false, "/ms sendgroup [group name] [command] - Sends [command] to all characters with multisend loaded that are defined within the group [group name].");
		m_AshitaCore->GetChatManager()->Write(0, false, "/ms follow on/off - When enabled, the current character will obey followme.");
		m_AshitaCore->GetChatManager()->Write(0, false, "/ms followme on/off - When enabled, all characters with follow enabled will follow this character.  Only one character can have this active at a time, if you activate it on a second the first will deactivate automatically.");
		m_AshitaCore->GetChatManager()->Write(0, false, "/ms reload - Reloads group file without reloading Multisend.");
		m_AshitaCore->GetChatManager()->Write(0, false, "/ms ignoreself on/off - When enabled, send and sendgroup commands sent by this character will not execute on this character.");
		m_AshitaCore->GetChatManager()->Write(0, false, "/ms attemptzone on/off - When enabled, multisend will continue running in the direction you were running when follow leader zones.");
		m_AshitaCore->GetChatManager()->Write(0, false, "/ms debug on/off - When enabled, debug prints will be visible.");
		m_AshitaCore->GetChatManager()->Write(0, false, "/ms safemode on/off - When enabled, '/echo' will be inserted for commands missing the leading '/' to prevent chatlog spam.");
		return true;
	}

	return (_strnicmp(command, "/ms ", 4) == 0);
}

uint32_t Multisend::GetArg(const char* text, std::string* buffer)
{
	std::string working(text);

	if (working[0] == '"')
	{
		size_t second = working.substr(1).find_first_of('"');
		if (second != std::string::npos)
		{
			*buffer = working.substr(1, second);
			return second + 1;
		}
	}

	size_t space = working.find_first_of(' ');
	if (space != std::string::npos)
	{
		*buffer = working.substr(0, space);
		return space;
	}

	*buffer = std::string(text);
	return strlen(text);
}