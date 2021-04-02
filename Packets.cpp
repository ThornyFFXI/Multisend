#include "Multisend.h"

bool Multisend::HandleIncomingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked)
{
	if (id == 0x00A)
	{
		if (strcmp(s_name.c_str(), (const char*)data + 0x84))
		{
			UpdateName(std::string((const char*)data + 0x84));
		}
	}
	if ((id == 0x32) || (id == 0x34))
	{
		if (p_MMF->Follow.target_process_id == ::GetCurrentProcessId())
		{
			if (p_MMF->Follow.idle_count < 5000)
			{
				p_MMF->Follow.idle_count = 5000;
			}
		}
	}

	return false;
}