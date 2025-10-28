#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.h"

#include <QAbstractItemModel>

class UserItem;

class UserModel : public QAbstractItemModel
{
     Q_OBJECT

public:
    explicit UserModel(QObject *parent = nullptr);
    ~UserModel();

    void setUserList(QList<User*>* temp);

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex indexByUUID(QString uuid);
    User* userByIndex(QModelIndex index);

    QList<UserItem*>* getUserList();

private:
    UserItem *rootItem;
    QList<UserItem*> m_users;
};

class UserItem
{
public:
    explicit UserItem(User* us, UserItem *parentItem = 0);
    explicit UserItem(QString cat, UserItem *parentItem = 0);
    ~UserItem();

    void appendChild(UserItem *child);

    UserItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    UserItem *parentItem();
    User *getUser();

private:
    enum TYPE_ITEM
    {
        USER,
        CATEGORY
    };

    TYPE_ITEM type;
    QList<UserItem*> m_childItems;
    User *m_user = Q_NULLPTR;
    QString category;
    UserItem *m_parentItem = Q_NULLPTR;
};

#endif // USERMODEL_H
