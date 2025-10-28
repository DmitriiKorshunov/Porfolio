from Deploy.features.parsing_users import UsersTable
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import *
from sqlalchemy import *
# from Deploy.features.notifications_methods import *
import uuid, datetime, logging, hashlib

Base = declarative_base()


def update_users_list(users, id_process, ip):
    # try:
    logging.info('id_process: ' + id_process + ' -> Start process update_users_list from ip: ' + ip)
    num_items_added = 0
    num_items_update = 0
    time_start = datetime.datetime.now()
    session_add_edit_new_user = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
    session_add_edit_new_user.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
    session_add_edit_new_user = session_add_edit_new_user()
    result_session = session_add_edit_new_user.query(UsersTable).filter(UsersTable.name == users.name).all()
    session_add_edit_new_user.close()
    if len(result_session) > 0:
        logging.warning('id_process: ' + id_process + 'Result process: Not found')
        return 0
    session_add_edit_new_user = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
    session_add_edit_new_user.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
    session_add_edit_new_user = session_add_edit_new_user()
    session_add_edit_new_user.add(UsersTable(uuid_user=str(uuid.uuid4()),
                                             name=str(users.name),
                                             password=str(users.password),
                                             sender=str(users.sender),
                                             end_resolution=str(users.end_resolution),
                                             category=str(users.category),
                                             post_position=str(users.post_position)))
    session_add_edit_new_user.commit()
    num_items_added += 1
    session_add_edit_new_user.close()
    return {'process': 'added new user'}

# except:
#     logging.warning('Result process: error. Expect.')
#     return {'process': 'error'}
