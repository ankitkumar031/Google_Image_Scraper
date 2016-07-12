#include "googleimagescraper.h"

#include <QStringList>

static bool g_finished;
static bool g_compared;
static QString g_html;

void htmlCompare(const QString &html)
{
	g_compared = true;
	if(g_html != html){
		g_html = html;
		return;
	}
	g_finished = true;
}

GoogleImageScraper::GoogleImageScraper(QObject *parent) : QObject(parent)
{

}

QStringList GoogleImageScraper::scrapeImageOfKeyWord(const QString &keyWords)
{
	QStringList listOfKeyWord = keyWords.split(' ');
	QString queryStr = listOfKeyWord.join('+');

	QString baseUrl = "https://www.google.com/search?tbm=isch&q=";

	QUrl webUrl = QUrl(baseUrl + queryStr);

	qDebug() << "Query: " << webUrl;

	view.show();
	view.resize(4000, 3000);
	view.hide();
	view.load(webUrl);
	view.setZoomFactor(0.25);

	QTest::qWait(15000);

	g_finished = false;
	while(!g_finished){
		g_compared = false;
		view.page()->toHtml(htmlCompare);
		while(!g_compared){
			QTest::qWait(200);
		}
	}

	return htmlResolve(g_html);
}

bool GoogleImageScraper::safeModeOn() const
{
	return _safeModeOn;
}

void GoogleImageScraper::setSafeModeOn(bool safeModeOn)
{
	_safeModeOn = safeModeOn;
}

QStringList GoogleImageScraper::htmlResolve(const QString html)
{
	QString label = "imgurl";
	qint32 indexOfImgurl = 0;
	qint32 startIndex = 0;

	QStringList hrefList;

	while((indexOfImgurl = html.indexOf(label, startIndex)) != -1){
		qint32 indexOfHref = indexOfImgurl + label.length() + 1;
		qint32 indexOfAnd = html.indexOf('&', indexOfImgurl);
		qint32 lengthOfHref = indexOfAnd - indexOfHref;
		QString imageHref = QByteArray::fromPercentEncoding(html.mid(indexOfHref, lengthOfHref).toLocal8Bit());

		hrefList.append(imageHref);

		startIndex = indexOfAnd + 1;
	}

	return hrefList;
}
