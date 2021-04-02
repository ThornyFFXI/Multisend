#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdint.h>

enum multisend_type
{
	all, //Param irrelevant
	others, //Param = sender's process ID
	alliance, //Param = sender's server id
	party, //Param = sender's server id
	group, //Param = group id
	single //Param = process id of target
};

struct MMF_Name_Single
{
	uint32_t Process;
	uint8_t Name[64];
	uint8_t Active;
};

struct MMF_Name
{
	uint32_t				ProcessID;
	MMF_Name_Single			Names[100];
};

struct MMF_ICommand_Single
{
	uint8_t			active;
	multisend_type	type;
	uint32_t		param;
	uint32_t		sender_process_id;
	uint8_t			command[248];
};

struct MMF_ICommand
{
	uint32_t				ProcessID;
	uint32_t				Position;
	MMF_ICommand_Single		Command[100];
};

struct MMF_IFollow
{
	uint32_t				target_process_id;
	uint16_t				lastzone;
	uint16_t				zone;
	float					position_x;
	float					position_z;
	uint32_t				idle_count;
};

struct MMF_Global
{
	MMF_Name				Name;
	MMF_ICommand			Command;
	MMF_IFollow				Follow;
};
#endif