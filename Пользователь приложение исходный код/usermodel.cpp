#include "usermodel.h"

UserModel::UserModel(QObject *parent):
    QAbstractItemModel(parent)
{
    rootItem = new UserItem("");
}

UserModel::~UserModel()
{

}

void UserModel::setUserList(QList<User *>*temp)
{
    //    delete rootItem;
    //    rootItem = new UserItem("");
    m_users.clear();
    QMap<QString,UserItem*> parents;

    for(auto user:*temp)
    {
        if(parents.contains(user->getCategory()))
        {
            auto parent = parents.value((user->getCategory()));
            auto child = new UserItem(user,parent);
            m_users.append(child);
            parent->appendChild(child);

        }
        else
        {
            auto parent = new UserItem(user->getCategory(),rootItem);
            rootItem->appendChild(parent);
            parents.insert(user->getCategory(),parent);
            auto child = new UserItem(user,parent);
            m_users.append(child);
            parent->appendChild(child);
        }
    }
}

int UserModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<UserItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QModelIndex UserModel::indexByUUID(QString uuid)
{
    for(auto user:m_users)
    {
        if(user->getUser()->getUuid() == uuid)
            return createIndex(user->row(), 0, user);
    }
    return QModelIndex();
}

User *UserModel::userByIndex(QModelIndex index)
{
    UserItem *item = static_cast<UserItem*>(index.internalPointer());
    return item->getUser();
}

QList<UserItem *> *UserModel::getUserList()
{
    return &m_users;
}

QVariant UserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    UserItem *item = static_cast<UserItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags UserModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant UserModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex UserModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    UserItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<UserItem*>(parent.internalPointer());

    UserItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex UserModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    UserItem *childItem = static_cast<UserItem*>(index.internalPointer());
    UserItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int UserModel::rowCount(const QModelIndex &parent) const
{
    UserItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<UserItem*>(parent.internalPointer());

    return parentItem->childCount();
}





UserItem::UserItem(User *us, UserItem *parentItem)
{
    type = USER;
    m_user = us;
    m_parentItem = parentItem;
}

UserItem::UserItem(QString cat, UserItem *parentItem)
{
    type = CATEGORY;
    category = cat;
    m_parentItem = parentItem;
}

UserItem::~UserItem()
{
    qDeleteAll(m_childItems);
}

void UserItem::appendChild(UserItem *item)
{
    m_childItems.append(item);
}

UserItem *UserItem::child(int row)
{
    return m_childItems.value(row);
}

int UserItem::childCount() const
{
    return m_childItems.count();
}

int UserItem::columnCount() const
{
    if(type == USER)
        return 2;
    return 1;
}

QVariant UserItem::data(int column) const
{
    if(type == CATEGORY)
        return category;

    if(column == 0)
        return m_user->getName();
    else
        return m_user->getPostPosition();
}

UserItem *UserItem::parentItem()
{
    return m_parentItem;
}

User *UserItem::getUser()
{
    return m_user;
}

int UserItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<UserItem*>(this));

    return 0;
}
