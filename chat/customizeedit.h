#ifndef CUSTOMIZEEDIT_H
#define CUSTOMIZEEDIT_H
#include <QLineEdit>

class CustomizeEdit: public QLineEdit
{
    Q_OBJECT
public:
    CustomizeEdit(QWidget* parent);
    ~CustomizeEdit();
    void SetMaxLen(int maxlen);

protected:
    virtual void focusOutEvent(QFocusEvent*) override;
private:
    int _max_len;

signals:
    void sig_focus_out();

public slots:
    void limitTextLength(const QString& text);
};

#endif // CUSTOMIZEEDIT_H
