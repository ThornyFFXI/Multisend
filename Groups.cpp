#include "Multisend.h"
#include <fstream>
#include <iostream>
using namespace rapidxml;

bool Multisend::CheckMatch(MMF_ICommand_Single command)
{
	if ((c_ignoreself) && (::GetCurrentProcessId() == command.sender_process_id)) return false;

	if (command.type == multisend_type::all)
	{
		return true;
	}
	else if (command.type == multisend_type::others)
	{
		return (::GetCurrentProcessId() != command.param);
	}
	else if (command.type == multisend_type::group)
	{
		if (std::find(c_groups.begin(), c_groups.end(), command.param) != c_groups.end())
		{
			return true;
		}
		return false;
	}
	else if (command.type == multisend_type::party)
	{
		for (int x = 0; x < 6; x++)
		{
			if ((m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberIsActive(x))
				&& (m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberServerId(x) == command.param))
			{
				return true;
			}
		}
		return false;
	}
	else if (command.type == multisend_type::alliance)
	{
		for (int x = 0; x < 18; x++)
		{
			if ((m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberIsActive(x))
				&& (m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberServerId(x) == command.param))
			{
				return true;
			}
		}
		return false;
	}
	else if (command.type == multisend_type::single)
	{
		return (::GetCurrentProcessId() == command.param);
	}
	else
	{
		return false;
	}
}

void Multisend::LoadGroups()
{
	s_group.Map.clear();
	c_groups.clear();
	std::string Path = m_AshitaCore->GetInstallPath();
	Path += "config\\Multisend.xml";

	std::ifstream Reader(Path, std::ios::in | std::ios::binary | std::ios::ate);
	if (Reader.is_open())
	{
		Reader.seekg(0, std::ios::end);
		uint32_t Size = (uint32_t)Reader.tellg();

		s_group.rawdata = new char[Size + 1];
		Reader.seekg(0, std::ios::beg);
		Reader.read(s_group.rawdata, Size);
		Reader.close();
		s_group.rawdata[Size] = '\0';
		try
		{
			s_group.doc.parse<0>(s_group.rawdata);
			m_AshitaCore->GetChatManager()->Write(0, false, "Multisend: Groups loaded.");
			s_group.Loaded = true;
		}
		catch (...)
		{
			m_AshitaCore->GetChatManager()->Write(0, false, "Multisend: Groups failed to parse.");
			s_group.doc.clear();
			s_group.Loaded = false;
			delete s_group.rawdata;
		}
	}
	else
	{
		if (s_group.Loaded)
		{
			s_group.doc.clear();
			delete s_group.rawdata;
			s_group.Loaded = false;
		}
		m_AshitaCore->GetChatManager()->Write(0, false, "Multisend: No groups file found.");
	}

	if (s_group.Loaded)
	{
		ReadXML();
		MatchGroups();
	}
}

void Multisend::ReadXML()
{
	s_group.Map.clear();

	xml_node<>* Node = s_group.doc.first_node();
	if (_stricmp(Node->name(), "multisend") == 0) Node = Node->first_node();
	while (Node)
	{
		if (_stricmp(Node->name(), "group") == 0)
		{
			xml_attribute<>* Attr = Node->first_attribute("index");
			if (Attr)
			{
				xml_attribute<>* Attr2 = Node->first_attribute("name");
				if (Attr2)
				{
					s_group.Map.insert(std::make_pair((uint16_t)atoi(Attr->value()), std::string(Attr2->value())));
				}
			}
		}
		Node = Node->next_sibling();
	}
}

void Multisend::MatchGroups()
{
	c_groups.clear();
	if (s_name.length() < 3) return;
	if (!s_group.Loaded) return;

	xml_node<>* Node = s_group.doc.first_node();
	if (_stricmp(Node->name(), "multisend") == 0) Node = Node->first_node();
	while (Node)
	{
		if (_stricmp(Node->name(), "group") == 0)
		{
			xml_attribute<>* Attr = Node->first_attribute("index");
			if (Attr)
			{
				xml_attribute<>* Attr2 = Node->first_attribute("name");
				if (Attr2)
				{
					uint16_t Index = (uint16_t)atoi(Attr->value());
					for (xml_node<>* SubNode = Node->first_node("char"); SubNode != NULL; SubNode = SubNode->next_sibling("char"))
					{
						if (_stricmp(s_name.c_str(), SubNode->value()) == 0)
						{
							c_groups.push_back(Index);
							break;
						}
					}
				}
			}
		}
		Node = Node->next_sibling();
	}
}