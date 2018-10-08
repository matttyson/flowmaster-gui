#ifndef TACHOMETER_HPP
#define TACHOMETER_HPP

#include <QString>
#include <qwt_dial.h>

class MtTachoMeter : public QwtDial
{
public:
	explicit MtTachoMeter(QWidget *parent = 0);

	void setString(const QString &str);
protected:

	void drawNeedle	(	QPainter * 	painter,
		const QPointF &center,
		double 	radius,
		double 	direction,
		QPalette::ColorGroup colorGroup )const;
signals:

public slots:

private:
	QString m_label;
	QString m_name;

};

#endif // TACHOMETER_HPP
