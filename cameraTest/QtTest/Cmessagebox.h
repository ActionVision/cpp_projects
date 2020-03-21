#pragma once
#include <QMessageBox>  
#include <QEventLoop>  

class MessageBoxMT : public QObject
{
	Q_OBJECT

private:
	const QString m_title;
	const QString m_message;

public:
	MessageBoxMT(const QString &title, const QString &message);

	static void show(const QString &title, const QString &message);

private:
	void readyShow(void);

	private slots:
	void onShow(void);
};