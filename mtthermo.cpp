#include "mtthermo.hpp"

#include <qwt_thermo.h>
#include <qwt_color_map.h>

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
#include <QDebug>
#include <QString>


/*
	This was stolen from sysinfo.cpp from the qwt examples
*/

MtThermo::MtThermo(Qt::Orientation orientation, const QString &text, QWidget *parent, double value) :
	QWidget(parent)
{
	m_label = new QLabel( text, this );
	//m_label->setFont( QFont( "Helvetica", 10 ) );

	m_value = new QLabel("0.0",this);
	//m_value->setFont(QFont("Helvectia",10));
	m_value->setAlignment(Qt::AlignHCenter);

	m_thermo = new QwtThermo( this );
	m_thermo->setScale( 0.0, 80.0 );
	m_thermo->setValue( value );
	m_thermo->setPipeWidth( 6 );
	m_thermo->setScaleMaxMajor( 6 );
	m_thermo->setScaleMaxMinor( 5 );
	m_thermo->setFillBrush( Qt::darkMagenta );

	setColorMap(0.0, 0.40, 0.50);

	QVBoxLayout *layout = new QVBoxLayout( this );
	layout->setMargin( 0 );
	layout->setSpacing( 0 );

	if ( orientation == Qt::Horizontal )
	{
		// setScalePosition?
		m_thermo->setOrientation( orientation );
		layout->addWidget( m_value );
		layout->addWidget( m_thermo );
		layout->addWidget( m_label );
	}
	else
	{
		// setScalePosition QwtThermo::LeftScale ?
		m_label->setAlignment( Qt::AlignCenter );
		m_thermo->setOrientation( orientation );
		layout->addWidget( m_label, 0 );
		layout->addWidget( m_thermo, 10, Qt::AlignHCenter );
		layout->addWidget( m_value );
	}

	setLayout(layout);
}

void MtThermo::setValue(double value)
{
	m_thermo->setValue(value);
	QChar chr[] = {0x00b0}; // Degree symbol. U+00B0
	QString str = QString::fromRawData(chr, sizeof(chr) / sizeof(QChar));
	const QString format = QString("%1 %2").arg(QString::number(value,'f',1)).arg(str);
	m_value->setText(format);
}

double MtThermo::value()const
{
	return m_thermo->value();
}

void MtThermo::setColorMap(double blue, double yellow, double red)
{
	QwtLinearColorMap *colorMap =
		new QwtLinearColorMap( Qt::blue, Qt::red );

	colorMap->addColorStop( blue, Qt::blue);
	colorMap->addColorStop( yellow, Qt::yellow );
	colorMap->addColorStop( red, Qt::red );
	colorMap->setMode( QwtLinearColorMap::FixedColors );
	m_thermo->setColorMap( colorMap );

}

void MtThermo::setScale(double min, double max)
{
	m_thermo->setScale(min, max);
}
