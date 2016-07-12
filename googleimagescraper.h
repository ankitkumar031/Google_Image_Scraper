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

private slots:

private:
	QStringList htmlResolve(const QString html);

private:
	QWebEngineView view;

	bool _safeModeOn;
};

#endif // GOOGLEIMAGESCRAPER_H
