#include <QApplication>
#include <QCommandLineParser>
#include "googleimagescraper.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("Google Image Scraper");
	QApplication::setApplicationVersion("v0.1");

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption op_proxy(QStringList() << "p" << "proxy",
								"set application proxy, example: \"socks5 127.0.0.1:1080\"",
								"type address");
	parser.addOption(op_proxy);
	parser.process(app);

	QString proxyStr = parser.value(op_proxy);
	if(!proxyStr.isEmpty()  &&  proxyStr != "none"){
		qDebug() << proxyStr;
	}

	QNetworkProxy proxy;
	proxy.setType(QNetworkProxy::Socks5Proxy);
	proxy.setHostName("127.0.0.1");
	proxy.setPort(1080);
	QNetworkProxy::setApplicationProxy(proxy);

	GoogleImageScraper scraper;
	QStringList imgurlList = scraper.scrapeImageOfKeyWord("Tim Duncan");

	for(const QString &imgurl : imgurlList){
		fprintf(stderr, "%s\n", imgurl.toLocal8Bit().constData());
	}

	qDebug("\nTotal %d images been scraped.\n", imgurlList.length());

	return 0;
}
