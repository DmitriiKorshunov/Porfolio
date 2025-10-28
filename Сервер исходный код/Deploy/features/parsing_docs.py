from sqlalchemy import *
from sqlalchemy.orm import *
from sqlalchemy.ext.declarative import declarative_base
import uuid, logging, json, time

Base = declarative_base()


class DocsTable(Base):
    __tablename__ = 'docs'
    uuid_doc = Column(String(255), nullable=False, primary_key=True)
    name = Column(String(255), nullable=False)
    path = Column(String(255), nullable=False)
    user_address = Column()
    special_notes = Column()
    sender = Column(String(255), nullable=False)
    resolution = Column(String(255))
    receive_flag = Column()
    read_flag = Column()
    owner = Column(String())
    output_date = Column()
    number_output = Column()
    number_input = Column()
    log = Column()
    in_date = Column(String(255))
    date_upload = Column()
    address_flag = Column()
    archive_flag = Column()

    def __init__(self,
                 uuid_doc,
                 name,
                 path,
                 user_address,
                 special_notes,
                 sender,
                 resolution,
                 receive_flag,
                 read_flag,
                 owner,
                 output_date,
                 number_output,
                 number_input,
                 log,
                 in_date,
                 date_upload,
                 address_flag,
                 archive_flag
                 ):
        self.uuid_doc = uuid_doc
        self.name = name
        self.path = path
        self.user_address = user_address
        self.special_notes = special_notes
        self.sender = sender
        self.resolution = resolution
        self.receive_flag = receive_flag
        self.read_flag = read_flag
        self.owner = owner
        self.output_date = output_date
        self.number_output = number_output
        self.number_input = number_input
        self.log = log
        self.in_date = in_date
        self.date_upload = date_upload
        self.address_flag = address_flag
        self.archive_flag = archive_flag

    def __repr__(self):
        return f"({self.uuid_doc})" \
               f"({self.name})" \
               f"({self.path})" \
               f"({self.user_address})" \
               f"({self.special_notes})" \
               f"({self.sender})" \
               f"({self.resolution})" \
               f"({self.receive_flag})" \
               f"({self.read_flag})" \
               f"({self.owner})" \
               f"({self.output_date})" \
               f"({self.number_output})" \
               f"({self.number_input})" \
               f"({self.log})" \
               f"({self.date_upload})" \
               f"({self.in_date})" \
               f"({self.address_flag})" \
               f"({self.archive_flag})"


def uuid_docs_json(uuids):
    """
        Function for get information in JSON about ONLY one doc from 'uuid'

        :param uuids: unique doc id
        :return:
            Information about user in JSON format.
            Ex:
            {
                "archive_flag": bool
                "address_flag": list(bool, ...),
                "editing_time": str,
                "input_date": str,
                "last_editing_time": str,
                "log": list(str, ...),
                "name": str",
                "number_input": str,
                "number_output": str,
                "output_date": str,
                "owner": str,
                "pdftk": str,
                "read_flag": list(bool, ...),
                "receive_flag": list(bool, ...),
                "resolution": list(str, ...),
                "resolution_sender": list(str, ...),
                "sender": str,
                "special_notes": list(str, ...),
                "user_address": list(str, ...),
                "uuid_doc ": uuid
            }
        """
    try:
        id_process = ''.join((str(time.time())).split('.'))
        logging.info('id_process: ' + id_process + ' -> Start process doc-uuid ' + str(uuids))
        session_uuid_docs_json = sessionmaker(bind=create_engine("sqlite:///databases/main.db"))
        session_uuid_docs_json.configure(bind=create_engine("sqlite:///databases/main.db"))
        session_uuid_docs_json = session_uuid_docs_json()
        result_session = session_uuid_docs_json.query(DocsTable).filter(DocsTable.uuid_doc == str(uuids)).all()
        if len(result_session) == 0:
            logging.warning('id_process: ' + id_process + ' Result process: Not found')
            session_uuid_docs_json.close()
            return {'process': 'Not found'}
        if len(result_session) > 1:
            logging.warning('id_process: ' + id_process + ' Result process:  error in database')
            session_uuid_docs_json.close()
            return {'process': 'error'}
        else:
            for doc in result_session:
                doc_info = {
                    'uuid_doc': str(doc.uuid_doc),
                    'name': str(doc.name),
                    'pdftk': str(doc.path),
                    'log': str_to_array_elements(doc.log, "', '"),
                    'user_address': str_to_array_elements(doc.user_address, "', '"),
                    'resolution': json.loads(doc.resolution),
                    'special_notes': str_to_array_elements(doc.special_notes, "', '"),
                    'address_flag': str_to_array_bool(doc.address_flag),
                    'read_flag': str_to_array_bool(doc.read_flag),
                    'receive_flag': str_to_array_bool(doc.receive_flag),
                    'number_input': str(doc.number_input),
                    'in_date': str(doc.in_date),
                    'sender': str(doc.sender),
                    'number_output': str(doc.number_output),
                    'output_date': str(doc.output_date),
                    'editing_time': str(doc.editing_time),
                    'date_upload': str(doc.date_upload),
                    'owner': str(doc.owner),
                    'archive_flag': bool(doc.archive_flag)
                }
                session_uuid_docs_json.close()
                logging.info('id_process: ' + id_process + 'Process return information about doc')
                return doc_info
    except:
        logging.warning('Parser doesnt found doc. Expect.')
        return {'process': 'parser doesnt found doc'}


def list_all_docs_json():
    """

    Function for get information in JSON all users

    :return:
       Information about all docs in JSON format.
       Ex:
       { docs:
                {
                    "archive_flag": bool
                    "address_flag": list(bool, ...),
                    "editing_time": str,
                    "input_date": str,
                    "last_editing_time": str,
                    "log": list(str, ...),
                    "name": str",
                    "number_input": str,
                    "number_output": str,
                    "output_date": str,
                    "owner": str,
                    "pdftk": str,
                    "read_flag": list(bool, ...),
                    "receive_flag": list(bool, ...),
                    "resolution": list(str, ...),
                    "resolution_sender": list(str, ...),
                    "sender": str,
                    "special_notes": list(str, ...),
                    "user_address": list(str, ...),
                    "uuid_doc ": uuid
                }
        }
    """
    try:
        id_process = ''.join((str(time.time())).split('.'))
        logging.info('id_process: ' + id_process + ' -> Start process list all docs')
        docs = list()
        session_list_all_docs_json = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
        session_list_all_docs_json.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
        session_list_all_docs_json = session_list_all_docs_json()
        for doc in (session_list_all_docs_json.query(DocsTable).all()):
            doc_info = {
                'uuid_doc': str(doc.uuid_doc),
                'name': str(doc.name),
                'pdftk': str(doc.path),
                'log': json.loads(doc.log),
                'user_address': str_to_array_elements(doc.user_address, "', '"),
                'resolution': json.loads(doc.resolution),
                'special_notes': str_to_array_elements(doc.special_notes, "', '"),
                'address_flag': str_to_array_elements(doc.address_flag, "', '"),
                'read_flag': str_to_array_elements(doc.read_flag, "', '"),
                'receive_flag': str_to_array_elements(doc.receive_flag, "', '"),
                'number_input': str(doc.number_input),
                'in_date': str(doc.in_date),
                'sender': str(doc.sender),
                'number_output': str(doc.number_output),
                'output_date': str(doc.output_date),
                'date_upload': str(doc.date_upload),
                'owner': str(doc.owner),
                'archive_flag': bool(doc.archive_flag)
            }
            docs.append(doc_info)
        logging.info('id_process: ' + id_process + ' -> Process return JSON')
        session_list_all_docs_json.close()
        return {'docs': docs}
    except:
        logging.warning('Json-process doc was kill. Except.')
        return {'process': 'json-process was kill'}


def add_edit_new_doc(json_doc):
    """

        Function for add new information in JSON about doc or edit exist information


        :param json_doc: JSON-information about doc
            ex.
            {
                "archive_flag": bool
                "address_flag": list(bool, ...),
                "editing_time": str,
                "input_date": str,
                "last_editing_time": str,
                "log": list(str, ...),
                "name": str",
                "number_input": str,
                "number_output": str,
                "output_date": str,
                "owner": str,
                "pdftk": str,
                "read_flag": list(bool, ...),
                "receive_flag": list(bool, ...),
                "resolution": list(str, ...),
                "resolution_sender": list(str, ...),
                "sender": str,
                "special_notes": list(str, ...),
                "user_address": list(str, ...),
                "uuid_doc ": uuid
            }

        :return: operation status in JSON format
            add doc: {'process': 'added new doc'}
            update doc: {'process': 'doc was update'}
            error: {'process': 'error'}
        """
    try:
        id_process = ''.join((str(time.time())).split('.'))
        logging.info('id_process: ' + id_process + ' -> Start process add_edit_new_doc')
        session_add_edit_new_doc = sessionmaker(bind=create_engine("sqlite:///databases/main.db"))
        session_add_edit_new_doc.configure(bind=create_engine("sqlite:///databases/main.db"))
        session_add_edit_new_doc = session_add_edit_new_doc()
        if len(session_add_edit_new_doc.query(DocsTable).filter(
                DocsTable.uuid_doc == str(json_doc.uuid_doc)).all()) == 0:
            logging.info('id_process: ' + id_process + ' -> Detected event for create_new_doc')
            session_add_edit_new_doc.close()
            return create_new_doc(json_doc, id_process)
        else:
            logging.info('id_process: ' + id_process + ' -> Detected event for editing_exist_doc')
            session_add_edit_new_doc.close()
            return editing_exist_doc(json_doc, id_process)

    except:
        logging.warning('Result process: error. Expect.')
        return {'process': 'error'}


def str_to_array_bool(json_str):
    """
     Function for create array bool-types from str.
     (use for first release parsing modules)

    :param json_str: array in str-format
    :return: array bool-types
    """
    flags_list = list()
    json_str = json_str.translate({ord(i): None for i in '['})
    json_str = json_str.translate({ord(i): None for i in ']'})
    json_str = json_str.translate({ord(i): None for i in "'"})
    json_str = json_str.split(', ')
    for flag in json_str:
        if flag == 'False':
            flags_list.append(bool(0))
        else:
            flags_list.append(bool(1))
    return flags_list


def str_to_array_elements(json_list, separator):
    """
    Function for create array from str.
     (use for first release parsing modules)
    :param json_list: array in str-format
    :param separator: used separator
    :return: array items
    """
    items_list = list()
    json_list = json_list.translate({ord(i): None for i in '['})
    json_list = json_list.translate({ord(i): None for i in ']'})
    json_list = json_list.split(separator)

    if json_list.__len__ == 0:
        return
    for item in json_list:
        item = item.translate({ord(i): None for i in "'"})
        items_list.append(item)
    return items_list


def str_to_array_json(json_list):
    """
    Function for create array from str.
     (use for first release parsing modules)
    :param json_list: array in str-format
    :return: array items
    """
    items_dict = dict()
    items_list = list()
    json_list = json_list.translate({ord(i): None for i in '{'})
    json_list = json_list.translate({ord(i): None for i in '}'})
    if json_list.__len__() == 0:
        return ""
    json_list = json_list.split("', '")
    for item in json_list:
        item = item.split("': '")
        item[0] = item[0].translate({ord(i): None for i in "'"})
        item[1] = item[1].translate({ord(i): None for i in "'"})
        # item[1] = item[1].translate({ord(i): "\" for i in "\\n"})
        items_dict[item[0]] = item[1]
    return items_dict


def editing_exist_doc(json_editing_doc, id_process):
    # try:
    logging.info('id_process: ' + id_process + ' -> Start process editing_exist_doc')
    session_editing_exist_doc = sessionmaker(bind=create_engine("sqlite:///databases/main.db"))
    session_editing_exist_doc.configure(bind=create_engine("sqlite:///databases/main.db"))
    session_editing_exist_doc = session_editing_exist_doc()
    logging.info('Update doc')
    session_editing_exist_doc.query(DocsTable).filter(DocsTable.uuid_doc == json_editing_doc.uuid_doc). \
        update({"archive_flag": json_editing_doc.archive_flag,
                "address_flag": str(json_editing_doc.address_flag),
                "date_upload": str(time.time()),
                "in_date": str(json_editing_doc.in_date),
                "log": str(json_editing_doc.log),
                "name": str(json_editing_doc.name),
                "number_input": str(json_editing_doc.number_input),
                "number_output": str(json_editing_doc.number_output),
                "output_date": str(json_editing_doc.output_date),
                "owner": str(json_editing_doc.owner),
                "pdftk": str(json_editing_doc.path),
                "read_flag": str(json_editing_doc.read_flag),
                "receive_flag": str(json_editing_doc.receive_flag),
                "resolution": json_editing_doc.resolution,
                "sender": str(json_editing_doc.sender),
                "special_notes": str(json_editing_doc.special_notes),
                "user_address": str(json_editing_doc.user_address)})
    session_editing_exist_doc.commit()
    session_editing_exist_doc.close()
    logging.info('id_process: ' + id_process + ' -> Result process: doc was full update')
    return {'process': 'doc was full update'}

    # except:
    #     logging.warning('Result process: error. Expect.')
    #     return {'process': 'error'}


def delete_exist_doc(uuid_delete_doc):
    """

    Function for delete information ONLY one user from 'uuid'

    :param uuid_delete_doc: unique doc ud, which will delete from database
    :return: operation status in JSON format
        successful delete: {'process': 'delete'}
        error: {'process': 'error'} or {'process': 'error. user doesnt delete'}
        other error: {'process': 'error. user doesnt exist'} or {'process': 'error. some items with uuid'}
    """
    try:
        id_process = ''.join((str(time.time())).split('.'))
        logging.info('id_process: ' + id_process + ' -> Start delete process')
        session_delete_exist_doc = sessionmaker(bind=create_engine("sqlite:///databases/main.db"))
        session_delete_exist_doc.configure(bind=create_engine("sqlite:///databases/main.db"))
        session_delete_exist_doc = session_delete_exist_doc()
        if 0 == len(session_delete_exist_doc.query(DocsTable).filter(DocsTable.uuid_doc == uuid_delete_doc).all()):
            logging.warning('id_process: ' + id_process + ' -> Error process: doc doesnt exist')
            session_delete_exist_doc.close()
            return {'process': 'error. user doesnt exist'}
        if len(session_delete_exist_doc.query(DocsTable).filter(DocsTable.uuid_doc == uuid_delete_doc).all()) > 1:
            logging.warning('id_process: ' + id_process + ' -> Error process: some items with uuid')
            session_delete_exist_doc.close()
            return {'process': 'error. some items with uuid'}
        else:
            session_delete_exist_doc.query(DocsTable).filter(DocsTable.uuid_doc == uuid_delete_doc).delete()
            session_delete_exist_doc.commit()
            if not session_delete_exist_doc.query(DocsTable).filter(DocsTable.uuid_doc == uuid_delete_doc).all():
                logging.info('id_process: ' + id_process + ' -> Result process: delete doc ' + uuid_delete_doc)
                session_delete_exist_doc.close()
                return {'process': 'delete ' + uuid_delete_doc}
            else:
                logging.warning('Process: error. doc doesnt delete')
                session_delete_exist_doc.close()
                return {'process': 'error. doc doesnt delete'}
    except:
        logging.warning('Process doc was kill. Except.')
        return {'process': 'error'}


def create_new_doc(json_create_doc, id_process):
    try:
        logging.info('id_process: ' + id_process + ' -> Start process create_new_doc')
        session_create_new_doc = sessionmaker(bind=create_engine("sqlite:///databases/main.db"))
        session_create_new_doc.configure(bind=create_engine("sqlite:///databases/main.db"))
        session_create_new_doc = session_create_new_doc()
        logging.info('Add new doc')
        new_item = DocsTable(str(uuid.uuid4()),
                             str(json_create_doc.name),
                             str(json_create_doc.path),
                             str(json_create_doc.user_address),
                             str(json_create_doc.special_notes),
                             str(json_create_doc.sender),
                             str(json_create_doc.resolution),
                             str(json_create_doc.receive_flag),
                             str(json_create_doc.read_flag),
                             str(json_create_doc.owner),
                             str(json_create_doc.output_date),
                             str(json_create_doc.number_output),
                             str(json_create_doc.number_input),
                             str(json_create_doc.log),
                             str(time.time()),
                             str(json_create_doc.in_date),
                             str(time.time()),
                             str(json_create_doc.address_flag),
                             archive_flag=json_create_doc.archive_flag
                             )
        session_create_new_doc.add(new_item)
        session_create_new_doc.commit()
        session_create_new_doc.close()
        logging.info('id_process: ' + id_process + ' -> Result process: added new doc')
        return {'process': 'added new doc'}
    except:
        logging.warning('Result process: error. Expect.')
        return {'process': 'error'}
