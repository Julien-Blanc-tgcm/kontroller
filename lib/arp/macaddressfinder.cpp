#include "macaddressfinder.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>

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
	auto file = new QFile("/proc/net/arp", this);
	if (!file->open(QIODevice::ReadOnly |QIODevice::Text))
		return "";
	QTextStream str(file);
	while (str.status() == QTextStream::Ok)
	{
		QString line;
		auto res = str.readLineInto(&line, 255);
		if (!res)
			break;
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
