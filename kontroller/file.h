#ifndef EU_TGCM_KONTROLLER_FILE_H
#define EU_TGCM_KONTROLLER_FILE_H

#include <QObject>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class File
{
	Q_GADGET
  private:
	int id_;
	QString file_;
	QString filetype_;
	QString label_;
	QString type_;
	QString thumbnail_;
	QString icon_;
	QString secondaryLabel_;
	bool played_ = false;

  public:
	File() = default;

	~File() = default;

	File(File const&) = default;

	File(File&&) = default;

	File& operator=(File const&) = default;

	File& operator=(File&&) = default;

  public:
	Q_PROPERTY(int id READ id WRITE setId)
	Q_PROPERTY(QString file READ file WRITE setFile)
	Q_PROPERTY(QString filetype READ filetype WRITE setFiletype)
	Q_PROPERTY(QString label READ label WRITE setLabel)
	Q_PROPERTY(QString type READ type WRITE setType)
	Q_PROPERTY(QString thumbnail READ thumbnail WRITE setThumbnail)
	Q_PROPERTY(QString icon READ icon WRITE setIcon)
	Q_PROPERTY(QString secondaryLabel READ secondaryLabel WRITE setSecondaryLabel)
	Q_PROPERTY(bool played READ played WRITE setPlayed)

	int id() const;
	QString file() const;
	QString filetype() const;
	QString label() const;
	QString type() const;
	QString thumbnail() const;
	QString icon() const;
	QString secondaryLabel() const;
	/**
	 * @brief played whether the item has already been played or not
	 * @return
	 */
	bool played() const;

	void setId(int id);
	void setFile(QString file);
	void setFiletype(QString filetype);
	void setLabel(QString label);
	void setType(QString type);
	void setThumbnail(QString thumbnail);
	void setIcon(QString icon);
	void setPlayed(bool played);
	void setSecondaryLabel(QString secondaryLabel);

};

}
}
}

QDataStream& operator<<(QDataStream& stream, eu::tgcm::kontroller::File const& file);

QDataStream& operator>>(QDataStream& stream, eu::tgcm::kontroller::File& file);

Q_DECLARE_METATYPE(eu::tgcm::kontroller::File)

#endif // EU_TGCM_KONTROLLER_FILE_H
