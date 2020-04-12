#include "reply.h"


namespace eu
{
namespace tgcm
{
namespace minidsp
{
// Note : see file query.cpp for examples of request / replies
Reply::Reply(unsigned char *data, std::uint32_t dataSize) noexcept
{
	for(auto i = 0u; i < dataSize && i < data_.size(); ++i)
	{
		data_[i] = *(data + i);
	}
}

Reply::Type Reply::type() const
{
	if(data_[0] == 0x34u)
	{
		if(data_[1] == 0x05u && data_[2] == 0xFFu && data_[3] == 0xD0u)
			return Type::DeviceInformationReply;
		// currently does not handle other replies
		return Type::Unknown;
	}
	if(data_[0] == 0x01u) // mute reply, but without mute status. May just be a standard ok reply in fact
		return Type::MuteReply;
	if(data_[0] == 0x02u)
	{
		if(data_[1] == 0xABu)
			return Type::SwitchPresetReply;
		return Type::Unknown;
	}
	if(data_[0] == 0x03u)
	{
		if(data_[1] == 0x50u)
		{
			return Type::SwitchHdmiInputReply;
		}
		if(data_[1] == 0x42u)
			return Type::VolumeReply;
	}
	return Type::Unknown;
}

HDMIInput Reply::hdmiInput() const
{
	if(type() == Type::DeviceInformationReply)
	{
		return static_cast<HDMIInput>(data_[0x1c] & 0xFu);
	}
	return HDMIInput::HDMI1;
}

std::int16_t Reply::volume() const
{
	if(type() == Type::VolumeReply)
	{
		return -static_cast<std::int16_t>(data_[2]);
	}
	if(type() == Type::DeviceInformationReply)
	{
		return -static_cast<std::int16_t>(data_[0xe]);
	}
	return 0;
}

EQPreset Reply::preset() const
{
	if(type() == Type::DeviceInformationReply)
	{
		return static_cast<EQPreset>(data_[0xc]);
	}
	return EQPreset::Preset1;
}

bool Reply::muted() const
{
	if(type() == Type::DeviceInformationReply)
	{
		return data_[0xf] == 0x1u;
	}
	return false;
}

}
}
}
