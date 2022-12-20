#include "libraryservice.h"

#include <QDebug>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

LibraryService::LibraryService(QObject* parent) :
    QObject{parent},
    videoLibraryScanProgress_{0},
    videoLibraryCleanProgress_{0},
    audioLibraryScanProgress_{0},
    audioLibraryCleanProgress_{0}
{
}

Client* LibraryService::client() const
{
	return client_;
}

int LibraryService::videoLibraryScanProgress() const
{
	return videoLibraryScanProgress_;
}

int LibraryService::videoLibraryCleanProgress() const
{
	return videoLibraryCleanProgress_;
}

int LibraryService::audioLibraryScanProgress() const
{
	return audioLibraryScanProgress_;
}

int LibraryService::audioLibraryCleanProgress() const
{
	return audioLibraryCleanProgress_;
}

void LibraryService::setClient(eu::tgcm::kontroller::Client* client)
{
	if (client_ == client)
		return;

	client_ = client;
	connect(client_, &Client::videoLibraryScanStarted, this, &LibraryService::handleVideoLibraryScanStarted_);
	connect(client_, &Client::videoLibraryScanFinished, this, &LibraryService::handleVideoLibraryScanFinished_);
	connect(client_, &Client::audioLibraryScanStarted, this, &LibraryService::handleAudioLibraryScanStarted_);
	connect(client_, &Client::audioLibraryScanFinished, this, &LibraryService::handleAudioLibraryScanFinished_);
	connect(client_, &Client::videoLibraryCleanStarted, this, &LibraryService::handleVideoLibraryCleanStarted_);
	connect(client_, &Client::videoLibraryCleanFinished, this, &LibraryService::handleVideoLibraryCleanFinished_);
	connect(client_, &Client::audioLibraryCleanStarted, this, &LibraryService::handleAudioLibraryCleanStarted_);
	connect(client_, &Client::audioLibraryCleanFinished, this, &LibraryService::handleAudioLibraryCleanFinished_);
	emit clientChanged(client_);
}

void LibraryService::setVideoLibraryScanProgress(int videoLibraryScanProgress)
{
	if (videoLibraryScanProgress_ == videoLibraryScanProgress)
		return;

	videoLibraryScanProgress_ = videoLibraryScanProgress;
	emit videoLibraryScanProgressChanged(videoLibraryScanProgress_);
}

void LibraryService::setVideoLibraryCleanProgress(int videoLibraryCleanProgress)
{
	if (videoLibraryCleanProgress_ == videoLibraryCleanProgress)
		return;

	videoLibraryCleanProgress_ = videoLibraryCleanProgress;
	emit videoLibraryCleanProgressChanged(videoLibraryCleanProgress_);
}

void LibraryService::setAudioLibraryScanProgress(int audioLibraryScanProgress)
{
	if (audioLibraryScanProgress_ == audioLibraryScanProgress)
		return;

	audioLibraryScanProgress_ = audioLibraryScanProgress;
	emit audioLibraryScanProgressChanged(audioLibraryScanProgress_);
}

void LibraryService::setAudioLibraryCleanProgress(int audioLibraryCleanProgress)
{
	if (audioLibraryCleanProgress_ == audioLibraryCleanProgress)
		return;

	audioLibraryCleanProgress_ = audioLibraryCleanProgress;
	emit audioLibraryCleanProgressChanged(audioLibraryCleanProgress_);
}

void LibraryService::refreshVideoLibrary()
{
	QJsonObject parameters;
	parameters["directory"] = "";
	parameters["showdialogs"] = false;
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("VideoLibrary.Scan", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &LibraryService::handleRefreshVideoLibraryReply_);
}

void LibraryService::refreshAudioLibrary()
{
	QJsonObject parameters;
	parameters["directory"] = "";
	parameters["showdialogs"] = false;
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("AudioLibrary.Scan", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &LibraryService::handleRefreshVideoLibraryReply_);
}

void LibraryService::cleanVideoLibrary()
{
	QJsonObject parameters;
	parameters["showdialogs"] = false;
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("VideoLibrary.Clean", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &LibraryService::handleRefreshVideoLibraryReply_);
}

void LibraryService::cleanAudioLibrary()
{
	QJsonObject parameters;
	parameters["showdialogs"] = false;
	QJsonRpcMessage message = QJsonRpcMessage::createRequest("AudioLibrary.Clean", parameters);
	auto reply = client_->send(message);
	connect(reply, &QJsonRpcServiceReply::finished, this, &LibraryService::handleRefreshVideoLibraryReply_);
}

void LibraryService::handleRefreshVideoLibraryReply_()
{
	auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
	if (reply != nullptr)
	{
		qDebug() << reply->response();
	}
	// do nothing
}

void LibraryService::handleVideoLibraryScanStarted_()
{
	setVideoLibraryScanProgress(1);
}

void LibraryService::handleVideoLibraryScanFinished_()
{
	setVideoLibraryScanProgress(100);
}

void LibraryService::handleAudioLibraryScanStarted_()
{
	setAudioLibraryScanProgress(1);
}

void LibraryService::handleAudioLibraryScanFinished_()
{
	setAudioLibraryScanProgress(100);
}

void LibraryService::handleVideoLibraryCleanStarted_()
{
	setVideoLibraryCleanProgress(1);
}

void LibraryService::handleVideoLibraryCleanFinished_()
{
	setVideoLibraryCleanProgress(100);
}

void LibraryService::handleAudioLibraryCleanStarted_()
{
	setAudioLibraryCleanProgress(1);
}

void LibraryService::handleAudioLibraryCleanFinished_()
{
	setAudioLibraryCleanProgress(100);
}

}
}
}
