#ifndef GOOGLEIMAGESCRAPER_H
#define GOOGLEIMAGESCRAPER_H

#include <QObject>
#include <QtWebEngineWidgets>
#include <QtTest>

class GoogleImageScraper : public QObject
{
	Q_OBJECT

public:
	explicit GoogleImageScraper(QObject *parent = 0);

	QStringList scrapeImageOfKeyWord(const QString &keyWords);

	bool safeModeOn() const;
	void setSafeModeOn(bool safeModeOn);

	bool faceOnly() const;
	void setFaceOnly(bool faceOnly);

	qint32 number() const;
	void setNumber(const qint32 &number);

private slots:

private:
	QStringList htmlResolve(const QString html);

private:
	QWebEngineView view;

	qint32 _number = -1;
	bool _safeModeOn = false;
	bool _faceOnly = false;
};

#endif // GOOGLEIMAGESCRAPER_H
