#include "query.h"

namespace eu
{
namespace tgcm
{
namespace minidsp
{

std::uint8_t Query::computeChecksum_(const Query::Message &data, std::uint8_t length)
{
	std::uint32_t checksum = 0;
	for(auto i = 0u; i < length; ++i)
	{
		checksum += data[i];
	}
	return checksum & 0xFFu;
}

// change volume sample
// == 64 ==> 03 42 5a 9f 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// <== 64 == 03 42 5a ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 00
Query::Message Query::changeVolumeQuery(std::int16_t newVolume)
{
	if(newVolume < -255 || newVolume > 0)
		return {0};
	Query::Message data{0};
	data[0] = 3u; // command + 1 byte data + checksum, length = 3
	data[1] = static_cast<std::uint8_t>(Commands::SetVolume);
	data[2] = -newVolume; // 0xFF in query means min volume, 0 means maximum volume, so just reverse
	data[3] = computeChecksum_(data, 3);
	return data;
}


// mute query sample :
// == 64 ==> 03 17 01 1b 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// <== 64 == 01 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 00
// NOTE : answer does not contain the mute status
Query::Message Query::muteQuery(bool mute)
{
	Query::Message data{0};
	data[0] = 3u; // command + 1 byte data + checksum, length = 3
	data[1] = static_cast<std::uint8_t>(Commands::Mute);
	data[2] = mute ? 1u : 0u;
	data[3] = computeChecksum_(data, 3);
	return data;
}

// sample :
// no idea why the reply is not always 64 bytes (even see it being 192, weird…)
// == 64 ==> 03 49 00 4c 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//<== 128 == 03 50 00 da 72 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 1b
//           01 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 00
Query::Message Query::switchHdmiInputQuery(HDMIInput input)
{
	Query::Message data{0};
	data[0] = 3u; // command + 1 byte data + checksum, length = 3
	data[1] = static_cast<std::uint8_t>(Commands::SelectHDMI);
	data[2] = static_cast<std::uint8_t>(input);
	data[3] = computeChecksum_(data, 3);
	return data;
}


// example :
// == 64 ==> 04 25 02 01 2c 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// <== 64 == 02 ab ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 00
// the android app always follows it by, which is probably needed to take the preset in consideration
// == 64 ==> 05 05 ff e0 01 ea 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// <== 64 == 05 05 ff e0 ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 00
Query::Message Query::switchEQPresetQuery(EQPreset preset)
{
	Query::Message data{0};
	data[0] = 4u; // command + 2 byte data + checksum, length = 4
	data[1] = static_cast<std::uint8_t>(Commands::SelectPreset);
	data[2] = static_cast<std::uint8_t>(preset);
	data[3] = 1u; // always 1, don't know what it is
	data[4] = computeChecksum_(data, 4);
	return data;
}

// com query sample :
// == 64 ==> 05 05 ff a3 30 dc 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// <== 64 == 34 05 ff a3 03 20 03 c8 00 01 00 64 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 ff ff f8 ff ff ff 01 00 b8 d1 01 bf 35
//           86 00 00 00 ff ff ff ff ff ff ff ff ff ff ff 00
Query::Message Query::initCommQuery()
{
	Query::Message data{0};
	data[0] = 5u; // command + 3 byte data + checksum, length = 5
	data[1] = static_cast<std::uint8_t>(Commands::Query1);
	// the 3 following bytes are what the android app send.
	data[2] = 0xFFu;
	data[3] = 0xA3u;
	data[4] = 0x30u;
	data[5] = computeChecksum_(data, 5);
	return data;
}

// device id query sample
// == 64 ==> 02 31 33 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// <== 64 == 05 31 01 1e 08 ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 00
Query::Message Query::deviceIdentificationQuery()
{
	Query::Message data{0};
	data[0] = 2u; // command + checksum, no data, length = 2
	data[1] = static_cast<std::uint8_t>(Commands::Query2);
	data[2] = computeChecksum_(data, 2);
	return data;
}


// device whatever query dump
// == 64 ==> 05 05 ff a1 01 ab 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// <== 64 == 05 05 ff a1 60 ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff 00
Query::Message Query::deviceWhateverQuery()
{
	Query::Message data{0};
	data[0] = 5u; // command + 3 byte data + checksum, length = 5
	data[1] = static_cast<std::uint8_t>(Commands::Query1);
	// the 3 following bytes are what the android app send.
	data[2] = 0xFFu;
	data[3] = 0xA1u;
	data[4] = 0x01u;
	data[5] = computeChecksum_(data, 5);
	return data;
}

// device information query dump
// == 64 ==> 05 05 ff d0 30 09 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//           00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// <== 64 == 34 05 ff d0 00 00 00 00 02 ae b8 c3 00 ff 70 00
//           ff ff ff ff ff ff ff ff ff 00 03 ff 80 44 ff ff
//           ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff
//           00 05 04 3d ff ff ff ff ff ff ff ff ff ff ff 00

// byte 0xe is volume (0x70 in sample)
// byte 0xc is preset (0x00 in sample)
// byte 0xf is mute (0x00 in sample)
// byte 0x1c is hdmi input in 4 lower bits (high is always 0x80, low is 0x0 for hdmi input 1, 0x1 for hdmi input 2
Query::Message Query::deviceInformationQuery()
{
	Query::Message data{0};
	data[0] = 5u; // command + 3 byte data + checksum, length = 5
	data[1] = static_cast<std::uint8_t>(Commands::Query1);
	data[2] = 0xFFu;
	data[3] = 0xD0u;
	data[4] = 0x30u;
	data[5] = computeChecksum_(data, 5);
	return data;
}

Commands Query::queryCommand(Message const& msg)
{
	return static_cast<Commands>(msg[1]);
}

bool Query::isValid(Message const& message)
{
	return message[0] != 0;
}

}
}
}
