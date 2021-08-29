#ifndef __ASHITA_Multisend_H_INCLUDED__
#define __ASHITA_Multisend_H_INCLUDED__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "C:\Ashita 4\plugins\sdk\Ashita.h"
#include "Structs.h"
#include "..\common\Utilities.h"
#include "..\common\thirdparty\rapidxml.hpp"
#include <time.h>
#include <list>
#include <map>

struct GroupData
{
    bool Loaded;
    char* rawdata;
    rapidxml::xml_document<> doc;
    std::map<uint16_t, std::string>		Map;
};

class Multisend : IPlugin, Ashita::Threading::Thread
{
private:
    IAshitaCore* m_AshitaCore;
    ILogManager* m_LogManager;
    IDirect3DDevice8* m_Direct3DDevice;
    uint32_t m_PluginId;

    //References
    HANDLE				hMapFile;
    MMF_Global*         p_MMF;

    //State tracking
    GroupData			s_group;
    std::string			s_name;
    uint32_t			s_position;
    bool				s_last_run_state;
    //Configuration
    std::list<uint16_t>	c_groups;
    float				c_maxdist;
    volatile bool		c_follow;
    volatile bool		c_attemptzone;
    volatile bool		c_ignoreself;
    float				s_vector_x;
    float				s_vector_z;

    volatile bool		c_debug;
    volatile bool		c_safemode;

public:
    const char* GetName(void) const override
    {
        return "Multisend";
    }
    const char* GetAuthor(void) const override
    {
        return "Thorny";
    }
    const char* GetDescription(void) const override
    {
        return "Insert description here.";
    }
    const char* GetLink(void) const override
    {
        return "Insert link here.";
    }
    double GetVersion(void) const override
    {
        return 1.17f;
    }
    int32_t GetPriority(void) const override
    {
        return 0;
    }
    uint32_t GetFlags(void) const override
    {
        return (uint32_t)Ashita::PluginFlags::All;
    }
	
    bool Initialize(IAshitaCore* core, ILogManager* logger, const uint32_t id) override;
    void Release(void) override;
    bool Direct3DInitialize(IDirect3DDevice8* device) override;	
    void Direct3DPresent(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) override;
    bool HandleIncomingPacket(uint16_t id, uint32_t size, const uint8_t* data, uint8_t* modified, uint32_t sizeChunk, const uint8_t* dataChunk, bool injected, bool blocked) override;
    bool HandleCommand(int32_t mode, const char* command, bool injected) override;
    uint32_t ThreadEntry(void) override;

    bool CheckMatch(MMF_ICommand_Single command);
    bool ReadCommand();
    void SendCommand(multisend_type type, uint32_t param, const char* Command);
    void SanitizeCommand(char* Input);
    std::string SubValues(std::string Input);
    void SubValues(char* Input);
    void UpdateName(std::string Name);
    void Claim(uint32_t* Target, uint32_t Mod);

    void LoadGroups();
    void ReadXML();
    void MatchGroups();

    uint32_t GetArg(const char* text, std::string* buffer);
};
#endif