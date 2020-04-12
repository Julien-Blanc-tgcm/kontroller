#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>

namespace eu
{
namespace tgcm
{
namespace minidsp
{
/**
 * @brief The Commands enum lists the known commands with their value
 */
enum class Commands : std::uint8_t
{
	Query1 = 0x05, /**< Query some information. Arguments to check, used at connection. Probably used to discover device type, etc. */
	Mute = 0x17, /**< Accept one argument, 1 for mute, 0 for unmute */
	SelectPreset = 0x25, /**< Accept two arguments. First one is preset number (0-3), second one is always 1 */
	Query2 = 0x31, /**< Query some information. No arguments. Currently returns 31 01 1e 08. Not sure what it is */
	SetVolume = 0x42, /**< Accept one argument, which is 255 + volume * 2 (volume in range [-127.5db - 0]), so 0xFF is max and 0x00 is min */
	SelectHDMI = 0x49, /**< Accept one argument, 0 for HDMI1, 1 for HDMI2 */
};

enum class HDMIInput : std::uint8_t
{
	HDMI1 = 0u,
	HDMI2 = 1u
};

enum class EQPreset : std::uint8_t
{
	Preset1 = 0u,
	Preset2 = 1u,
	Preset3 = 2u,
	Preset4 = 3u
};

constexpr const int maxVolume = 0;
constexpr const int minVolume = -255;

}
}
}

#endif // CONSTANTS_H
