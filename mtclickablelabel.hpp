#ifndef MTCLICKABLELABEL_HPP
#define MTCLICKABLELABEL_HPP

#include <QLabel>

class MtClickableLabel : public QLabel
{
	Q_OBJECT
public:
	explicit MtClickableLabel(const QString &text = "", QWidget *parent = 0);

signals:
	void clicked();

public slots:

protected:
	void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MTCLICKABLELABEL_HPP
