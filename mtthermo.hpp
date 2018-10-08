#ifndef MTTHERMO_HPP
#define MTTHERMO_HPP

#include <QWidget>

class QwtThermo;
class QLabel;

/**
 * @brief The MtThermo class
 *
 * TODO: Fix this so the color stops and colors themselves
 * can be set by the caller
 */


class MtThermo : public QWidget
{
	Q_OBJECT
public:
	explicit MtThermo( Qt::Orientation orientation,
					   const QString &text, QWidget *parent, double value = 0.0 );

	double value() const;

	void setScale(double min, double max);
	void setColorMap(double blue, double yellow, double red);

signals:

public slots:

	void setValue(double value);

private:
	QwtThermo *m_thermo;
	QLabel *m_label;
	QLabel *m_value;

};

#endif // MTTHERMO_HPP
