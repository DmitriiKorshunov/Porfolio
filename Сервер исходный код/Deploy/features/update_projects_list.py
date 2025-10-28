from sqlalchemy import *
from sqlalchemy.orm import *
from sqlalchemy.ext.declarative import declarative_base
import uuid, datetime

Base = declarative_base()


class ProjectsTable(Base):
    __tablename__ = 'control'
    uuid = Column(String(255), primary_key=True)
    localDate = Column(String(255))
    extDate = Column(String(255))
    deadLine = Column(String(255))
    path = Column(String(255))
    localNumber = Column(String(255))
    kindOfDoc = Column(String(255))
    extNumber = Column(String(255))
    about = Column(String(255))
    extDest = Column(String(255))
    worker = Column(String(255))
    outNumber = Column(String(255))
    flagControl = Column(String(255))
    VAMTOinpNumber = Column(String(255))
    note = Column(String(255))

    def __init__(self,
                 uuid,
                 localDate,
                 extDate,
                 deadLine,
                 path,
                 localNumber,
                 kindOfDoc,
                 extNumber,
                 about,
                 extDest,
                 worker,
                 outNumber,
                 flagControl,
                 VAMTOinpNumber,
                 note
                 ):
        self.uuid = uuid
        self.localDate = localDate
        self.extDate = extDate
        self.deadLine = deadLine
        self.path = path
        self.localNumber = localNumber
        self.kindOfDoc = kindOfDoc
        self.extNumber = extNumber
        self.about = about
        self.extDest = extDest
        self.worker = worker
        self.outNumber = outNumber
        self.flagControl = flagControl
        self.VAMTOinpNumber = VAMTOinpNumber
        self.note = note

    def __repr__(self):
        return f"({self.uuid}," \
               f" {self.localDate}," \
               f" {self.extDate}," \
               f" {self.deadLine}," \
               f" {self.path}," \
               f" {self.localNumber})," \
               f" {self.kindOfDoc})," \
               f" {self.extNumber}," \
               f" {self.about}," \
               f" {self.extDest})," \
               f" {self.flagControl})," \
               f" {self.worker})," \
               f" {self.outNumber})," \
               f" {self.VAMTOinpNumber})," \
               f" {self.note}),"


def update_projects_list(docs):
    num_items_added = 0
    num_items_update = 0
    time_start = datetime.datetime.now()
    session_add_edit_new_user = sessionmaker(bind=create_engine("sqlite:///databases/control_doc.db"))
    session_add_edit_new_user.configure(bind=create_engine("sqlite:///databases/control_doc.db"))
    session_add_edit_new_user = session_add_edit_new_user()
    cheking_exist = session_add_edit_new_user.query(ProjectsTable).all()
    uuids_exist = list()
    for uuid_project in cheking_exist:
        uuids_exist.append(uuid_project.uuid)
    for json_doc_info in docs.docs:
        if not json_doc_info['uuid'] in uuids_exist:
            session_add_edit_new_user.add(ProjectsTable(uuid=str(uuid.uuid4()),
                                                        localDate=str(json_doc_info['localDate']),
                                                        extDate=str(json_doc_info["extDate"]),
                                                        deadLine=str(json_doc_info["deadLine"]),
                                                        path=str(json_doc_info["path"]),
                                                        localNumber=str(json_doc_info["localNumber"]),
                                                        kindOfDoc=str(json_doc_info["kindOfDoc"]),
                                                        extDest=str(json_doc_info["extDest"]),
                                                        extNumber=str(json_doc_info["extNumber"]),
                                                        about=str(json_doc_info["about"]),
                                                        outNumber=str(json_doc_info["outNumber"]),
                                                        VAMTOinpNumber=str(json_doc_info["VAMTOinpNumber"]),
                                                        flagControl=str(json_doc_info["flagControl"]),
                                                        note=str(json_doc_info["note"]),
                                                        worker=str(json_doc_info["worker"])))
            num_items_added += 1
        else:
            session_add_edit_new_user.query(ProjectsTable).filter(ProjectsTable.uuid == json_doc_info['uuid']). \
                update({'localDate': str(json_doc_info['localDate']),
                        'extDate': str(json_doc_info["extDate"]),
                        'deadLine': str(json_doc_info["deadLine"]),
                        'outDate': str(json_doc_info["outDate"]),
                        'localNumber': str(json_doc_info["localNumber"]),
                        'kindOfDoc': str(json_doc_info["kindOfDoc"]),
                        'extDest': str(json_doc_info["extDest"]),
                        'extNumber': str(json_doc_info["extNumber"]),
                        'about': str(json_doc_info["about"]),
                        'outNumber': str(json_doc_info["outNumber"]),
                        'VAMTOinpNumber': str(json_doc_info["VAMTOinpNumber"]),
                        'flagControl': str(json_doc_info["flagControl"]),
                        'note': str(json_doc_info["note"]),
                        'worker': str(json_doc_info["worker"])})

            # session_add_edit_new_user.commit()
            num_items_update += 1
    session_add_edit_new_user.commit()
    session_add_edit_new_user.close()
    stop_time = datetime.datetime.now()
    time = (stop_time - time_start)
    return {'process': 'added: ' + str(num_items_added) + 'update: ' + str(num_items_update) + ' time: ' + str(time)}


def list_all_projects_json():
    session_list_all_docs_json = sessionmaker(bind=create_engine("sqlite:///databases/control_doc.db"))
    session_list_all_docs_json.configure(bind=create_engine("sqlite:///databases/control_doc.db"))
    session_list_all_docs_json = session_list_all_docs_json()
    docs = list()
    for doc in session_list_all_docs_json.query(ProjectsTable).all():
        doc_info = {
            'uuid': str(doc.uuid),
            'localDate': str(doc.localDate),
            'extDate': str(doc.extDate),
            'deadLine': str(doc.deadLine),
            'path': str(doc.path),
            'localNumber': str(doc.localNumber),
            'kindOfDoc': str(doc.kindOfDoc),
            'extNumber': str(doc.extNumber),
            'about': str(doc.about),
            'extDest': str(doc.extDest),
            'flagControl': str(doc.flagControl),
            'worker': str(doc.worker),
            'outNumber': str(doc.outNumber),
            'VAMTOinpNumber': str(doc.VAMTOinpNumber),
            'note': str(doc.note)
        }
        docs.append(doc_info)
    session_list_all_docs_json.close
    return {'docs': docs}


