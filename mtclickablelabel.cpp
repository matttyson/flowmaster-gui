#include "mtclickablelabel.hpp"

#include <QMouseEvent>

MtClickableLabel::MtClickableLabel(const QString &text, QWidget *parent) :
	QLabel(parent)
{
	if(text.length()){
		setText(text);
	}
}



void MtClickableLabel::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() & Qt::LeftButton) {
		event->accept();
		emit clicked();
	}
}
