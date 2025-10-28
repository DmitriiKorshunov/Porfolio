from Deploy.features.parsing_users import *

Base = declarative_base()


class WsPairTable(Base):
    __tablename__ = 'sessions'
    uuid_ws = Column(String(), primary_key=True)
    uuid_user = Column(String())
    name = Column(String())
    ip = Column(String())

    def __init__(self,
                 uuid_ws,
                 uuid_user,
                 name,
                 ip
                 ):
        self.uuid_ws = uuid_ws
        self.uuid_user = uuid_user
        self.name = name
        self.ip = ip

    def __repr__(self):
        return f"({self.uuid_ws})" \
               f"({self.uuid_user})" \
               f"({self.name})" \
               f"({self.ip})"


def add_new_connection(uuid_ws, uuid_user, password, ip):
    if uuid_user_json(uuid_user)['password'] == password:
        add_new_connection_session = sessionmaker(bind=create_engine("sqlite:///databases/chats.db"))
        add_new_connection_session.configure(bind=create_engine("sqlite:///databases/chats.db"))
        add_new_connection_session = add_new_connection_session()
        pairs = add_new_connection_session.query(WsPairTable).all()
        name = 'None'
        for exist_user in pairs:
            if exist_user.uuid_user == uuid_user:
                add_new_connection_session.query(WsPairTable).filter(WsPairTable.uuid_user == uuid_user).delete()
                add_new_connection_session.commit()
                return {'result': 'exist',
                        'uuid_ws': exist_user.uuid_ws}
        for user in list_all_users_json()['users']:
            if user['uuid_user'] == str(uuid_user):
                name = user['name']
        for user in pairs:
            if user.uuid_user == str(uuid_user):
                add_new_connection_session.query(WsPairTable).update(
                    {'uuid_user': str(uuid_user),
                     'uuid_ws': str(uuid_ws),
                     'name': str(name),
                     'ip': ip})
                add_new_connection_session.commit()
        new_pair = WsPairTable(
            uuid_user=str(uuid_user),
            uuid_ws=str(uuid_ws),
            name=str(name),
            ip=str(ip)
        )
        add_new_connection_session.add(new_pair)
        add_new_connection_session.commit()
        add_new_connection_session.close()
        return {'result': 'new',
                'uuid_ws': ''}
    else:
        return {'result': 'incorrectly',
                'uuid_ws': ''}


def delete_connection(uuid_session):
    delete_connection_session = sessionmaker(bind=create_engine("sqlite:///databases/chats.db"))
    delete_connection_session.configure(bind=create_engine("sqlite:///databases/chats.db"))
    delete_connection_session = delete_connection_session()
    delete_connection_session.query(WsPairTable).filter(WsPairTable.uuid_ws == str(uuid_session)).delete()
    delete_connection_session.commit()
    delete_connection_session.close()


def delete_all_connection():
    delete_connection_session = sessionmaker(bind=create_engine("sqlite:///databases/chats.db"))
    delete_connection_session.configure(bind=create_engine("sqlite:///databases/chats.db"))
    delete_connection_session = delete_connection_session()
    delete_connection_session.query(WsPairTable).delete()
    delete_connection_session.commit()
    delete_connection_session.close()


def get_ip(uuid_session):
    get_ip_session = sessionmaker(bind=create_engine("sqlite:///databases/chats.db"))
    get_ip_session.configure(bind=create_engine("sqlite:///databases/chats.db"))
    get_ip_session = get_ip_session()
    for ip_ses in get_ip_session.query(WsPairTable).filter(WsPairTable.uuid_ws == str(uuid_session)).all():
        return ip_ses.ip
    get_ip_session.close()
