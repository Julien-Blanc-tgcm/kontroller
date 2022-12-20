#include "sbiconimageprovider.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QGuiApplication>
#include <QPainter>
#include <QStandardPaths>
#include <sailfishapp.h>
#include <silicatheme.h>

SBIconImageProvider::SBIconImageProvider() : QQuickImageProvider{ImageType::Pixmap}
{
	auto ratio = static_cast<double>(Silica::Theme::instance()->pixelRatio());
	QString name = QGuiApplication::instance()->applicationName();
	auto path = SailfishApp::pathTo("assets");

	QVector<QPair<double, QString> > candidates;
	QDirIterator iterator{path.toLocalFile(), QDirIterator::NoIteratorFlags};
	while (iterator.hasNext())
	{
		QFileInfo f = iterator.next();
		if (f.isDir())
		{
			if (f.fileName().startsWith("z"))
			{
				QString ratioStr = f.fileName().mid(1);
				bool ok = false;
				double r = ratioStr.toDouble(&ok);
				if (!ok)
					continue;
				candidates.push_back(QPair<double, QString>(std::abs(r - ratio), f.filePath()));
			}
		}
	}
	std::sort(candidates.begin(),
	          candidates.end(),
	          [](QPair<double, QString> const& p1, QPair<double, QString> const& p2) { return p1.first < p2.first; });
	paths_.reserve(candidates.size());
	for (auto candidate : candidates)
	{
		paths_.push_back(candidate.second);
	}
	qDebug() << "Found paths " << paths_;
}

SBIconImageProvider::~SBIconImageProvider() noexcept
{
}

QPixmap SBIconImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
	auto components = id.split("?");
	if (components.size() == 0)
	{
		qDebug() << "Empty id given, can't get an image";
		return QPixmap{};
	}
	for (auto const& p : paths_)
	{
		auto path = p + "/" + components[0] + ".png";
		QFileInfo info{path};
		if (info.exists())
		{
			QPixmap ret;
			ret.load(path);
			if (size)
			{
				size->setWidth(ret.width());
				size->setHeight(ret.height());
			}
			if (components.length() > 1)
			{
				if (QColor::isValidColor(components[1]))
				{
					QPainter painter(&ret);
					painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
					painter.fillRect(ret.rect(), components[1]);
					painter.end();
				}
			}
			if (requestedSize.isValid())
			{
				return ret.scaled(requestedSize.width(), requestedSize.height(), Qt::IgnoreAspectRatio);
			}
			return ret;
		}
	}
	qDebug() << id << *size << requestedSize;
	return QPixmap{};
}

