#include "macaddressfinder.h"

#include <QTextStream>

namespace eu
{
namespace tgcm
{
namespace arp
{

MacAddressFinder::MacAddressFinder(QObject* parent) : QObject{parent}
{
}

QString MacAddressFinder::readMacAddress(QString ipAddress)
{
	QTextStream str("/proc/net/arp", QIODevice::ReadOnly);
	while (str.status() == QTextStream::Ok)
	{
		QString line = str.readLine(255);
		if (line.startsWith(ipAddress)) // found the line
		{
			return extractMacAddress_(line);
		}
	}
	return "";
}

QString MacAddressFinder::extractMacAddress_(QString arpLine)
{
	QString mac;
	int state = 0;
	for (int i = 0; i < arpLine.length(); ++i)
	{
		switch (state)
		{
			case 0:
				if (arpLine[i] == ' ')
					state = 1;
				break;
			case 1: // inside first space
				if (arpLine[i] != ' ')
					state = 2;
				break;
			case 2: // HW type
				if (arpLine[i] == ' ')
					state = 3;
				break;
			case 3: // spaces
				if (arpLine[i] != ' ')
					state = 4;
				break;
			case 4: // Flags
				if (arpLine[i] == ' ')
					state = 5;
				break;
			case 5:
				if (arpLine[i] != ' ')
				{
					mac.push_back(arpLine[i]);
					state = 6;
				}
				break;
			case 6:
				if (arpLine[i] == ' ')
					return mac;
				mac.push_back(arpLine[i]);
				break;
		}
	}
	return "";
}

}
}
}
