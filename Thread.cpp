#include "Multisend.h"

uint32_t Multisend::ThreadEntry(void)
{
	while (!this->IsTerminated())
	{
		if (p_MMF->Follow.target_process_id == GetCurrentProcessId())
		{
			uint16_t zone = m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberZone(0);
			uint16_t myindex = m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberTargetIndex(0);
			uint32_t renderflags = m_AshitaCore->GetMemoryManager()->GetEntity()->GetRenderFlags0(myindex);
			bool isrendered = (((renderflags & 0x200) != 0) && ((renderflags & 0x4000) == 0));
			float my_pos_x = m_AshitaCore->GetMemoryManager()->GetEntity()->GetLocalPositionX(myindex);
			float my_pos_z = m_AshitaCore->GetMemoryManager()->GetEntity()->GetLocalPositionY(myindex);
			if ((zone == 0) || (myindex == 0) || (!isrendered))
			{
				if (p_MMF->Follow.zone != 0)
				{
					p_MMF->Follow.lastzone = p_MMF->Follow.zone;
				}
				p_MMF->Follow.zone = 0;
			}
			else
			{
				if (p_MMF->Follow.zone == 0)
				{
					p_MMF->Follow.idle_count = 0;
				}
				else if ((my_pos_x != p_MMF->Follow.position_x) || (my_pos_z != p_MMF->Follow.position_z))
				{
					p_MMF->Follow.idle_count = 0;
				}
				else p_MMF->Follow.idle_count++;

				p_MMF->Follow.zone = zone;
				p_MMF->Follow.position_x = my_pos_x;
				p_MMF->Follow.position_z = my_pos_z;
			}
		}

		while (ReadCommand()) {}

		::Sleep(1);
	}
	return 0;
}

