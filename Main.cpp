#include "Multisend.h"

__declspec(dllexport) IPlugin* __stdcall expCreatePlugin(const char* args)
{
    UNREFERENCED_PARAMETER(args);

    return (IPlugin*)(new Multisend());
}

__declspec(dllexport) double __stdcall expGetInterfaceVersion(void)
{
    return ASHITA_INTERFACE_VERSION;
}

bool Multisend::Initialize(IAshitaCore* core, ILogManager* logger, const uint32_t id)
{
    m_AshitaCore = core;
    m_LogManager = logger;
    m_PluginId = id;
    LoadGroups();

	char buffer[256];
	strcpy_s(buffer, 256, "FFXI_Multisend_Compatibility");
	/*
	Create name for memory mapped file using plugin version, to guarantee mismatched multisends don't collide if user loads two.
	sprintf_s(buffer, 256, "FFXI_Multisend_V4_%f", this->GetPluginInfo().PluginVersion); */

	//create a handle to the MMF, size matches the struct we're using for it
	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,                       // maximum object size (high-order DWORD)
		sizeof(MMF_Global),		 // maximum object size (low-order DWORD)
		buffer);

	bool made = (GetLastError() == 0);

	if (hMapFile == NULL)
	{
		throw std::exception("Could not open or create MMF.");
	}


	p_MMF = (MMF_Global*)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		sizeof(MMF_Global));
	if (p_MMF == NULL)
	{
		CloseHandle(hMapFile);
		throw std::exception("Could not map MMF.");
	}

	if (made)
	{
		memset(p_MMF, 0, sizeof(MMF_Global));
	}

	s_last_run_state = false;
	s_position = p_MMF->Command.Position;
	s_name = "";
	if (m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberIsActive(0))
	{
		uint16_t myindex = m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberTargetIndex(0);
		UpdateName(std::string(m_AshitaCore->GetMemoryManager()->GetEntity()->GetName(myindex)));
	}

	//Default settings.
	c_follow = true;
	c_ignoreself = false;
	c_attemptzone = false;
	c_debug = false;
	c_safemode = false;
	c_maxdist = 5000.0f;

	this->Start();

    return true;
}

void Multisend::Release(void)
{
	this->Stop();
	if (p_MMF)
	{
		UnmapViewOfFile((LPCVOID)p_MMF);
	}
	if (hMapFile)
	{
		CloseHandle(hMapFile);
	}
}