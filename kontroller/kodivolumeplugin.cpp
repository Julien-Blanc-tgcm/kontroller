#include "kodivolumeplugin.h"

#include "client.h"

#include <qjsonrpcservicereply.h>

namespace eu
{
namespace tgcm
{
namespace kontroller
{
KodiVolumePlugin::KodiVolumePlugin(Client *parent) :
    VolumePlugin(parent),
    client_{parent}
{

}

QString KodiVolumePlugin::static_name()
{
	return QString::fromUtf8("Kodi");
}

QString KodiVolumePlugin::realName_() const
{
	return static_name();
}

int KodiVolumePlugin::maxVolume_() const
{
	return 100;
}

int KodiVolumePlugin::minVolume_() const
{
	return 0;
}

int KodiVolumePlugin::currentVolume_() const
{
	return currentVolumeStored_;
}

void KodiVolumePlugin::updateVolume_(int newVolume)
{
	QJsonArray parameters;
	parameters.append(newVolume);
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Application.SetVolume", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &KodiVolumePlugin::volumeReply_);
// todo : implement
}

int KodiVolumePlugin::volumeStep_() const
{
	return 2;
}

void KodiVolumePlugin::refreshVolume_()
{
	QJsonObject parameters;
	QJsonArray properties;
	properties.push_back(QString::fromUtf8("volume"));
	parameters["properties"] = properties;
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Application.GetProperties", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &KodiVolumePlugin::volumeReply_);
}

void KodiVolumePlugin::increaseVolume_()
{
	QJsonArray parameters;
	parameters.append(QString("increment"));
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Application.SetVolume", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &KodiVolumePlugin::volumeReply_);
}

void KodiVolumePlugin::decreaseVolume_()
{
	QJsonArray parameters;
	parameters.append(QString("decrement"));
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("Application.SetVolume", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &KodiVolumePlugin::volumeReply_);
}

bool KodiVolumePlugin::valueValid_() const
{
	return currentVolumeStored_ != -1;
}

QString KodiVolumePlugin::displayValue_() const
{
	if (valueValid_())
	{
		return QString::number(currentVolumeStored_) + " %";
	}
	return "";
}

QString KodiVolumePlugin::formatVolume_(int value) const
{
	return QString::number(value) + "%";
}

void KodiVolumePlugin::volumeReply_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if(reply != nullptr)
	{
		auto obj = reply->response().toObject();
		if(obj.contains("result"))
		{
			auto result = obj["result"];
			if(result.isObject() && result.toObject().contains("volume"))
			{
				auto volume = result.toObject()["volume"];
				if(volume.isDouble())
				{
					currentVolumeStored_ = volume.toInt();
					emit currentVolumeChanged(currentVolumeStored_);
					emit valueValidChanged(true);
				}
			}
			else if(result.isDouble())
			{
				currentVolumeStored_ = result.toInt();
				emit currentVolumeChanged(currentVolumeStored_);
				emit valueValidChanged(true);
			}
		}
	}
}

}
}
}

