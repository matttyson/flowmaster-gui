#include "tachometer.hpp"

#include <QPainter>
#include <QColor>

#include <qwt_dial_needle.h>
#include <qwt_round_scale_draw.h>

MtTachoMeter::MtTachoMeter(QWidget *parent) :
	QwtDial(parent),m_label("RPM")
{
	setWrapping(false);
	setReadOnly(true);

	setOrigin(135.0);

	setScaleArc( 0.0, 270.0 );
	scaleDraw()->setSpacing( 5 );

	QwtDialSimpleNeedle *needle = new QwtDialSimpleNeedle(
		QwtDialSimpleNeedle::Arrow, true, Qt::red,
		QColor( Qt::gray ).light( 130 ) );
	setNeedle( needle );

	QwtRoundScaleDraw *rsd = new QwtRoundScaleDraw();

	rsd->enableComponent( QwtAbstractScaleDraw::Backbone, true );
	rsd->enableComponent( QwtAbstractScaleDraw::Labels, true );
	rsd->enableComponent( QwtAbstractScaleDraw::Ticks, true );
	rsd->setTickLength(QwtScaleDiv::MinorTick, 4);
	rsd->setTickLength(QwtScaleDiv::MediumTick, 8);
	rsd->setTickLength(QwtScaleDiv::MajorTick, 16);

	setScaleDraw(rsd);
}


void MtTachoMeter::drawNeedle	(	QPainter * 	painter,
	const QPointF & 	center,
	double 	radius,
	double 	direction,
	QPalette::ColorGroup colorGroup)	const
{
	QwtDial::drawNeedle(painter, center, radius, direction, colorGroup);

	QRectF rect( 0.0, 0.0, 2.0 * radius, 2.0 * radius - 10.0 );
	rect.moveCenter( center );

	const QColor color = palette().color( QPalette::Text );
	painter->setPen( color );

	const int value = (int) this->value();
	const QString format = QString("%1 %2").arg(value).arg(m_label);

	const int flags = Qt::AlignBottom | Qt::AlignHCenter;
	painter->drawText( rect, flags, format );

	if(m_name.length() > 0) {
		rect.moveTop(30.0f);
		painter->drawText(rect, flags, m_name);
	}
}

void MtTachoMeter::setString(const QString &str)
{
	m_name = str;
}
