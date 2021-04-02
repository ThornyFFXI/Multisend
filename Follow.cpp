#include "Multisend.h"

bool Multisend::Direct3DInitialize(IDirect3DDevice8* lpDevice)
{
	this->m_Direct3DDevice = lpDevice;
	return true;
}

/**
* @brief Direct3D prerender call to allow this plugin to prepare for rendering.
*
* @note This will only be called if you returned true in Direct3DInitialize!
*/
void Multisend::Direct3DPresent(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	if (c_follow)
	{
		if (p_MMF->Follow.target_process_id == ::GetCurrentProcessId()) return;

		if (p_MMF->Follow.target_process_id == 0)
		{
			if (s_last_run_state)
			{
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetIsAutoRunning(0);
				s_last_run_state = false;
			}
		}
		else if (p_MMF->Follow.zone == m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberZone(0))
		{
			uint16_t myindex = m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberTargetIndex(0);
			float my_pos_x = m_AshitaCore->GetMemoryManager()->GetEntity()->GetLocalPositionX(myindex);
			float my_pos_z = m_AshitaCore->GetMemoryManager()->GetEntity()->GetLocalPositionY(myindex);
			s_vector_x = p_MMF->Follow.position_x - my_pos_x;
			s_vector_z = p_MMF->Follow.position_z - my_pos_z;
			double distance = sqrt(pow(s_vector_x, 2) + pow(s_vector_z, 2));

			if ((distance > 0.4f) && (distance < c_maxdist))
			{
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetFollowTargetServerId(0);
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetFollowTargetIndex(0);
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetFollowDeltaX(s_vector_x);
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetFollowDeltaY(s_vector_z);
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetFollowDeltaZ(0);
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetIsAutoRunning(1);
				s_last_run_state = true;
			}
			else if (s_last_run_state)
			{
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetIsAutoRunning(0);
				s_last_run_state = false;
			}
		}
		else if (p_MMF->Follow.zone == 0)
		{
			if ((c_attemptzone)
				&& (p_MMF->Follow.lastzone == m_AshitaCore->GetMemoryManager()->GetParty()->GetMemberZone(0))
				&& (p_MMF->Follow.idle_count < 5000))
			{
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetFollowTargetServerId(0);
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetFollowTargetIndex(0);
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetFollowDeltaX(s_vector_x);
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetFollowDeltaY(s_vector_z);
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetFollowDeltaZ(0);
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetIsAutoRunning(1);
				s_last_run_state = true;
			}
			else if (s_last_run_state)
			{
				m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetIsAutoRunning(0);
				s_last_run_state = false;
			}
		}
		else if (s_last_run_state)
		{
			m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetIsAutoRunning(0);
			s_last_run_state = false;
		}
	}
	else if (s_last_run_state)
	{
		m_AshitaCore->GetMemoryManager()->GetAutoFollow()->SetIsAutoRunning(0);
		s_last_run_state = false;
	}
}