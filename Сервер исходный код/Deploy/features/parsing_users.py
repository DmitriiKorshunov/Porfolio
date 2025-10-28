
from sqlalchemy import *
from sqlalchemy.orm import *
from sqlalchemy.ext.declarative import declarative_base
import uuid, logging, time

Base = declarative_base()


class UsersTable(Base):
    __tablename__ = 'users'
    uuid_user = Column(String(255), nullable=False, primary_key=True, unique=True)
    name = Column(String(255), nullable=False)
    password = Column(String(255), nullable=False)
    end_resolution = Column(String(255), nullable=False)
    category = Column(String(255), nullable=True)
    post_position = Column(String(255), nullable=False)
    sender = Column(String(255), nullable=False)

    def __init__(self,
                 uuid_user,
                 name,
                 password,
                 sender,
                 end_resolution,
                 category,
                 post_position
                 ):
        self.uuid_user = uuid_user
        self.name = name
        self.password = password
        self.sender = sender
        self.end_resolution = end_resolution
        self.category = category
        self.post_position = post_position

    def __repr__(self):
        return f"({self.uuid_user}," \
               f" {self.name}," \
               f" {self.password}," \
               f" {self.sender}," \
               f" {self.end_resolution}," \
               f" {self.sign_path})" \
               f" {self.photo})"


def uuid_user_json(uuids):
    """
    Function for get information in JSON about ONLY one user from 'uuid'

    :param uuids: unique user id
    :return:
        Information about user in JSON format.
        Ex:
        {
                    'uuid_user': uuid,
                    'name': str,
                    'password': str,
                    'beg_resolution': str,
                    'end_resolution': str,
                    'category': str,
                    'sign_path': str,
                    'post_position': str
                }
    """
    try:
        id_process = ''.join((str(time.time())).split('.'))
        logging.info('id_process: ' + id_process + ' -> Start process get user: ' + str(uuids))
        session_make_uuid_user_json = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
        session_make_uuid_user_json.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
        session_make_uuid_user_json = session_make_uuid_user_json()
        result_session = session_make_uuid_user_json.query(UsersTable).filter(UsersTable.uuid_user == str(uuids)).all()
        if len(result_session) == 0:
            logging.warning('id_process: ' + id_process + 'Result process: Not found')
            session_make_uuid_user_json.close()
            return {'process': 'Not found'}
        if len(result_session) > 1:
            logging.warning('id_process: ' + id_process + 'Result process: Error in database')
            session_make_uuid_user_json.close()
            return {'process': 'error'}
        else:
            for user in result_session:
                user_info = {
                    'uuid_user': str(user.uuid_user),
                    'name': str(user.name),
                    'password': str(user.password),
                    'end_resolution': str(user.end_resolution),
                    'category': str(user.category),
                    'post_position': str(user.post_position),
                    'sender': str(user.sender)
                }
                logging.info('id_process: ' + id_process + ' -> Process return JSON')
                session_make_uuid_user_json.close()
                return user_info
    except:
        logging.warning('Parser doesnt found user. Expect.')
        return {'process': 'parser doesnt found user'}


def list_all_users_json():
    """

    Function for get information in JSON all users

    :return:
        Information about all users in JSON format.
        Ex:
        { users:
                {
                    'uuid_user': ...,
                    'name': ...,
                    'password': ...,
                    'beg_resolution': ...,
                    'end_resolution': ...,
                    'category': ...,
                    'sign_path': ...,
                    'post_position': ...
                },
                ...
        }

    """

    # try:
    id_process = ''.join((str(time.time())).split('.'))
    logging.info('id_process: ' + id_process + ' -> Start process list all users')
    users = list()
    session_list_all_users_json = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
    session_list_all_users_json.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
    session_list_all_users_json = session_list_all_users_json()
    for user in (session_list_all_users_json.query(UsersTable).all()):
        user_info = {
            'uuid_user': str(user.uuid_user),
            'name': str(user.name),
            'password': str(user.password),
            'sender': str(user.sender),
            'end_resolution': str(user.end_resolution),
            'category': str(user.category),
            'post_position': str(user.post_position)
        }
        users.append(user_info)
    logging.info('id_process: ' + id_process + ' -> Process return JSON')
    session_list_all_users_json.close()
    print(users)
    return {'users': users}

    # except:
    #     logging.warning('Json-process user was kill. Except.')
    #     return {'process': 'json-process was kill'}


# def add_edit_new_user(json_user):
#     """
#
#     Function for add new information in JSON about user or edit exist information
#
#
#     :param json_user: JSON-information about user
#         ex.
#         {
#                     'uuid_user': ...,
#                     'name': ...,
#                     'password': ...,
#                     'beg_resolution': ...,
#                     'end_resolution': ...,
#                     'category': ...,
#                     'sign_path': ...,
#                     'post_position': ...
#                 }
#
#     :return: operation status in JSON format
#         add user: {'process': 'added new user'}
#         update user: {'process': 'user was update'}
#         error: {'process': 'error'}
#     """
#     try:
#         id_process = ''.join((str(time.time())).split('.'))
#         session_add_edit_new_user = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
#         session_add_edit_new_user.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
#         session_add_edit_new_user = session_add_edit_new_user()
#         if len(session_add_edit_new_user.query(UsersTable).filter(
#                 UsersTable.uuid_user == str(json_user.uuid_user)).all()) == 0:
#             logging.info('id_process: ' + id_process + ' -> Start process add new user')
#             new_item = UsersTable(str(uuid.uuid4()),
#                                   str(json_user.name),
#                                   str(json_user.password),
#                                   str(json_user.sender),
#                                   str(json_user.end_resolution),
#                                   str(json_user.category),
#                                   str(json_user.sign_path),
#                                   str(json_user.photo),
#                                   str(json_user.post_position))
#             session_add_edit_new_user.add(new_item)
#             session_add_edit_new_user.commit()
#             session_add_edit_new_user.close()
#             logging.info('id_process: ' + id_process + ' -> Result process: added new user')
#             return {'process': 'added new user'}
#         else:
#             logging.info('id_process: ' + id_process + ' -> Start process update user')
#             session_add_edit_new_user.query(UsersTable).filter(UsersTable.uuid_user == json_user.uuid_user). \
#                 update({'name': str(json_user.name),
#                         'password': str(json_user.password),
#                         'sign_path': str(json_user.sign_path),
#                         'photo': str(json_user.photo),
#                         'count': str(json_user.count),
#                         'end_resolution': str(json_user.end_resolution),
#                         'category': str(json_user.category),
#                         'post_position': str(json_user.post_position)})
#             session_add_edit_new_user.commit()
#             session_add_edit_new_user.close()
#             logging.info('id_process: ' + id_process + ' -> Result process: user was update')
#             return {'process': 'user was update'}
#     except:
#         logging.warning('Process doc was kill. Except.')
#         return {'process': 'error'}


# def delete_exist_user(uuid_delete_user):
#     """
#
#     Function for delete information ONLY one user from 'uuid'
#
#     :param uuid_delete_user: unique user ud, which will delete from database
#     :return: operation status in JSON format
#         successful delete: {'process': 'delete'}
#         error: {'process': 'error'} or {'process': 'error. user doesnt delete'}
#         other error: {'process': 'error. user doesnt exist'} or {'process': 'error. some items with uuid'}
#     """
#     try:
#         id_process = ''.join((str(time.time())).split('.'))
#         logging.info('id_process: ' + id_process + ' -> Start delete process')
#         session_delete_exist_user = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
#         session_delete_exist_user.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
#         session_delete_exist_user = session_delete_exist_user()
#         if 0 == len(session_delete_exist_user.query(UsersTable).filter(UsersTable.uuid_user == uuid_delete_user).all()):
#             logging.warning('id_process: ' + id_process + ' -> Error process: user doesnt exist')
#             session_delete_exist_user.close()
#             return {'process': 'error. user doesnt exist'}
#         if len(session_delete_exist_user.query(UsersTable).filter(UsersTable.uuid_user == uuid_delete_user).all()) > 1:
#             logging.warning('id_process: ' + id_process + ' -> Error process: some items with uuid')
#             session_delete_exist_user.close()
#             return {'process': 'error. some items with uuid'}
#         else:
#             session_delete_exist_user.query(UsersTable).filter(UsersTable.uuid_user == uuid_delete_user).delete()
#             session_delete_exist_user.commit()
#             session_delete_exist_user.close()
#             if not session_delete_exist_user.query(UsersTable).filter(UsersTable.uuid_user == uuid_delete_user).all():
#                 logging.info('id_process: ' + id_process + ' -> Result process: delete user ' + uuid_delete_user)
#                 return {'process': 'delete'}
#             else:
#                 logging.warning('Process: error. user doesnt delete')
#                 return {'process': 'error. user doesnt delete'}
#     except:
#         logging.warning('Process doc was kill. Except.')
#         return {'process': 'error'}


# def sort_all_list_users():
#     session_list_all_users_json = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
#     session_list_all_users_json.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
#     session_list_all_users_json = session_list_all_users_json()
#     result = list()
#     for user in (session_list_all_users_json.query(UsersTable).all()):
#         result.append((
#             user.count,
#             user.uuid_user,
#             user.category,
#             user.name,
#             user.password,
#             user.sign_path,
#             user.post_position,
#             user.photo,
#             user.end_resolution))
#
#     session_list_all_users_json.close()
#     users = list()
#     for user in sorted(result):
#         user_info = {
#             'uuid_user': str(user[1]),
#             'name': str(user[3]),
#             'password': str(user[4]),
#             'count': str(user[0]),
#             'end_resolution': str(user[8]),
#             'category': str(user[2]),
#             'sign_path': str(user[5]),
#             'photo': str(user[7]),
#             'post_position': str(user[6])
#         }
#         users.append(user_info)
#     return {'users': users}
