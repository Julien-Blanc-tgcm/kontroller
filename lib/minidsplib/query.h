#ifndef EU_TGCM_MINIDSP_QUERY_H
#define EU_TGCM_MINIDSP_QUERY_H

#include "constants.h"

#include <array>

namespace eu
{
namespace tgcm
{
namespace minidsp
{

class Query
{
	static std::uint8_t computeChecksum_(std::array<std::uint8_t, 64> const& data, std::uint8_t length);
public:
	Query() = delete;
	~Query() = delete;

	using Message = std::array<std::uint8_t, 64>;

	/**
	 * @brief changeVolumeQuery returns a new set volume query which will set the volume to the given one
	 * @param newVolume The new volume. Must be in the range -255 (-127.5dB) 0 (0dB, max volume)
	 * @return the array for the query. An empty array (all data set to 0) if volume was not valid.
	 */
	static Message changeVolumeQuery(std::int16_t newVolume);

	/**
	 * @brief muteQuery returns a query that can be used to mute or unmute the device
	 * @param mute whether to mute (true) or unmute (true)
	 * @return the array for the query. Will always be valid.
	 */
	static Message muteQuery(bool mute);

	static Message switchHdmiInputQuery(HDMIInput input);

	static Message switchEQPresetQuery(EQPreset preset);

	/**
	 * @brief initCommQuery creates the query used at initialization of communication.
	 * It asks information about the device, which is returned
	 * @return
	 */
	static Message initCommQuery();

	/**
	 * @brief deviceIdentificationQuery creates the second query made by the android app, which is
	 * likely a device identification query
	 * @return
	 */
	static Message deviceIdentificationQuery();

	/**
	 * @brief deviceWhateverQuery creates the third query used by android app. To investigate
	 * what it replies
	 * @return
	 */
	static Message deviceWhateverQuery();

	/**
	 * @brief deviceInformationQuery creates a device information query. Third query made by the android app
	 * @return
	 */
	static Message deviceInformationQuery();

	/**
	 * @brief returns the command this message is. Allows inspecting a command before it is being sent
	 * @return 
	 */
	static Commands queryCommand(Message const& message);

	static bool isValid(Message const& message);
};

}
}
}

#endif // EU_TGCM_MINIDSP_QUERY_H
