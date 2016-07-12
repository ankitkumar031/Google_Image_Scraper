#include <QApplication>
#include <QCommandLineParser>
#include "googleimagescraper.h"

bool setProxy(const QString &proxyStr){
	QNetworkProxy::ProxyType type;
	QString hostname;
	qint32 port;

	QStringList list1 = proxyStr.split(' ');
	if(list1.size() != 2){
		return false;
	}

	QString typeStr = list1.at(0);
	if(typeStr.compare("HTTP", Qt::CaseInsensitive) == 0
			|| typeStr.compare("HTTPS", Qt::CaseInsensitive) == 0){
		type = QNetworkProxy::HttpProxy;
	}
	else if(typeStr.compare("SOCKS5", Qt::CaseInsensitive) == 0){
		type = QNetworkProxy::Socks5Proxy;
	}
	else{
		return false;
	}

	QStringList list2 = list1.at(1).split(':');
	if(list2.size() != 2){
		return false;
	}

	hostname = list2.at(0);
	port = list2.at(1).toInt();

	QNetworkProxy proxy;
	proxy.setType(type);
	proxy.setHostName(hostname);
	proxy.setPort(port);
	QNetworkProxy::setApplicationProxy(proxy);

	fprintf(stderr, "Using proxy:\t%s %s:%d\n",
			typeStr.toLocal8Bit().constData(),
			hostname.toLocal8Bit().constData(),
			port);

	return true;
}

void showUsage(){
	fprintf(stderr, R"usage(
Usage: ./google_image_scraper [options] -k <keywords>
Options:
	-h, --help                  Displays this help.
	-v, --version               Displays version information.
	-p, --proxy <type address>  Set application proxy, example: "socks5 127.0.0.1:1080"
	-n, --number <num of imgs>	Output at most number of images
	-f, --face_only				Search human face of keywords
	-S, --safe_mode				Turn on google search safe mode
	-k, --keywords <keywords>   The keywords of images to scrape, example: "Yao Ming"
)usage");

	exit(0);
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("Google Image Scraper");
	QApplication::setApplicationVersion("v0.2");

	QCommandLineParser parser;
	parser.addVersionOption();

	QCommandLineOption op_help(QStringList() << "h" << "help");
	parser.addOption(op_help);
	QCommandLineOption op_proxy(QStringList() << "p" << "proxy", " ", " ");
	parser.addOption(op_proxy);
	QCommandLineOption op_number(QStringList() << "n" << "number", " ", " ");
	parser.addOption(op_number);
	QCommandLineOption op_faceOnly(QStringList() << "f" << "face_only");
	parser.addOption(op_faceOnly);
	QCommandLineOption op_safeMode(QStringList() << "S" << "safe_mode");
	parser.addOption(op_safeMode);
	QCommandLineOption op_keywords(QStringList() << "k" << "keywords", " ", " ");
	parser.addOption(op_keywords);

	GoogleImageScraper scraper;

	parser.process(app);

	if(parser.isSet(op_help)){
		showUsage();
	}

	if(parser.isSet(op_proxy)){
		QString proxyStr = parser.value(op_proxy);
		if(!setProxy(proxyStr)){
			showUsage();
		}
	}

	if(parser.isSet(op_number)){
		qint32 number = parser.value(op_number).toInt();
		if(number <= 0){
			showUsage();
		}
		scraper.setNumber(number);
	}

	scraper.setFaceOnly(parser.isSet(op_faceOnly));
	scraper.setSafeModeOn(parser.isSet(op_safeMode));

	QString keywords = parser.value(op_keywords);
	if(keywords.isEmpty()){
		showUsage();
	}

	QStringList imgurlList = scraper.scrapeImageOfKeyWord(keywords);

	for(const QString &imgurl : imgurlList){
		fprintf(stdout, "%s\n", imgurl.toLocal8Bit().constData());
	}
	fflush(stdout);

	qDebug("\nTotal %d images been scraped.\n", imgurlList.length());

	return 0;
}
