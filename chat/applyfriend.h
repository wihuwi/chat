#ifndef APPLYFRIEND_H
#define APPLYFRIEND_H

#include <QDialog>
#include "clickedlabel.h"
#include "userdata.h"
#include "friendlabel.h"
//reset Remove ResetNormalState
namespace Ui {
class ApplyFriend;
}

class ApplyFriend : public QDialog
{
    Q_OBJECT

public:
    explicit ApplyFriend(QWidget *parent = nullptr);
    ~ApplyFriend();
    void SetSearchInfo(std::shared_ptr<SearchInfo> si);
    void SetPlaceHolder(QString back_name, QString name);

protected:
    bool eventFilter(QObject* obj, QEvent* ev);

private:
    void InitTipLbs();
    void addLabels(QString text);
    void AddTipLbs(ClickedLabel* lb, QPoint cur_point, QPoint& next_point, int text_width, int text_height);
    void resetLabels();
    Ui::ApplyFriend *ui;
    //默认标签
    QVector<QString> _tip_data;
    //
    QMap<QString, ClickedLabel*> _add_labels;
    QVector<QString> _add_label_keys;
    QPoint _tip_cur_point;
    //
    QMap<QString, FriendLabel*> _friend_labels;
    QVector<QString> _friend_label_keys;
    QPoint _label_point;

    std::shared_ptr<SearchInfo> _si;

private slots:
    void ShowMoreLabels();
    void SlotLabelEditFinished();
    void SlotLabelEnter();
    void SlotLabelTextChange(const QString& text);
    void SlotAddFirendLabelByClickTip(QString text);
    void SlotChangeFriendLabelByTip(QString lbText, ClickLbState state);
    void SlotRemoveFriendLabel(QString name);
    void SlotApplySure();
    void SlotApplyCancel();
};

#endif // APPLYFRIEND_H
