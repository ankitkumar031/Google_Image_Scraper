#include "googleimagescraper.h"

#include <QStringList>
#include <QDateTime>

static qint32 g_finished;
static bool g_compared;
static QString g_html;

void htmlCompare(const QString &html)
{
	g_compared = true;
	if(g_html == html){
		g_finished++;
	}
	else{
		g_html = html;
		g_finished = 0;
	}
}

GoogleImageScraper::GoogleImageScraper(QObject *parent) : QObject(parent)
{
//	view.setWindowFlags(Qt::FramelessWindowHint);
}

QStringList GoogleImageScraper::scrapeImageOfKeyWord(const QString &keywords)
{
	QString baseUrl = "https://www.google.com/search?tbm=isch";

	QStringList listOfKeyword = keywords.split(' ');
	QString keywordsStr = QString("&q=") + listOfKeyword.join('+');

	fprintf(stderr, "Keywords:  \t%s\n", keywords.toLocal8Bit().constData());

	if(_number == -1){
		fprintf(stderr, "Img number:\tNo limit\n");
	}
	else{
		fprintf(stderr, "Img number:\t%d\n", _number);
	}

	if(_faceOnly){
		baseUrl += "&tbs=itp:face";
		fprintf(stderr, "Face only:\tYes\n");
	}
	else{
		fprintf(stderr, "Face only:\tNo\n");
	}

	if(_safeModeOn){
		baseUrl += "&safe=on";
		fprintf(stderr, "Safe mode:\tYes\n");
	}
	else{
		fprintf(stderr, "Safe mode:\tNo\n");
	}

	QUrl queryUrl = QUrl(baseUrl + keywordsStr);

	fprintf(stderr, "Scrape URL:\t%s\n\n", queryUrl.toString().toLocal8Bit().constData());

	view.show();
//	view.setGeometry(4000, 4000, 4000, 1000);
	view.setGeometry(2000, 0, 4000, 1000);
	QTest::qWait(100);
	view.hide();
	view.load(queryUrl);
	view.setZoomFactor(0.25);

	g_finished = 0;
	while(g_finished <= 100){
		g_compared = false;
		view.page()->toHtml(htmlCompare);
		while(!g_compared){
			QTest::qWait(100);
		}
	}

	QStringList result = htmlResolve(g_html);

	if(_number > 0  &&  _number < result.size()){
		return result.mid(0, _number);
	}

	return result;
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

qint32 GoogleImageScraper::number() const
{
	return _number;
}

void GoogleImageScraper::setNumber(const qint32 &number)
{
	_number = number;
}

bool GoogleImageScraper::faceOnly() const
{
	return _faceOnly;
}

void GoogleImageScraper::setFaceOnly(bool faceOnly)
{
	_faceOnly = faceOnly;
}
