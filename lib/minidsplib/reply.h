#ifndef EU_TGCM_MINIDSP_REPLY_H
#define EU_TGCM_MINIDSP_REPLY_H

#include "constants.h"

#include <array>

namespace eu
{
namespace tgcm
{
namespace minidsp
{

// this should use a variant. But variant are not available with jolla outdated gcc.
// so currently, use a classic union. Client need to check the type(), and then, use
// the accessors accordingly (data will be garbage otherwise)
class Reply
{
	std::array<std::uint8_t, 64> data_ {{0}};
public:
	enum class Type
	{
		Unknown,
		VolumeReply,
		MuteReply,
		SwitchHdmiInputReply,
		SwitchPresetReply,
		DeviceInformationReply
	};
	Reply(unsigned char* data, std::uint32_t dataSize) noexcept;
	Reply(Reply const&) noexcept = default;
	Reply(Reply&&) noexcept = default;
	Reply& operator=(Reply const&) noexcept = default;
	Reply& operator=(Reply&&) noexcept = default;
	~Reply() noexcept = default;

	Type type() const;

	HDMIInput hdmiInput() const;

	std::int16_t volume() const;

	EQPreset preset() const;

	bool muted() const;
};

}
}
}
#endif // EU_TGCM_MINIDSP_REPLY_H
