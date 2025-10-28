from sqlalchemy import *
from sqlalchemy.orm import *
from Deploy.features.parsing_docs import DocsTable, str_to_array_elements
from Deploy.features.update_projects_list import ProjectsTable
# from features.make_pdf import create_new_pdf
from fastapi.responses import FileResponse
import logging, json


import sys, os
def resource_path(relative_path):
        if hasattr(sys, '_MEIPASS'):
            return os.path.join(sys._MEIPASS, relative_path)
        return os.path.join(os.path.abspath(""), relative_path)

def uuid_doc_download(uuid_doc, id_process, ip):
    try:
        logging.info('id_process: ' + id_process + ' -> Start process uuid_doc_download from ip: ' + ip)
        session_uuid_doc_download = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
        session_uuid_doc_download.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
        session_uuid_doc_download = session_uuid_doc_download()
        for doc in (session_uuid_doc_download.query(DocsTable).filter(
                DocsTable.uuid_doc == str(uuid_doc)).all()):
            filepath = doc.path
            filename = str(doc.path).split('/')[-1]
            try:
                open(str(filepath))
                logging.info('id_process: ' + id_process + ' -> Process uuid_doc_download done')
                return FileResponse(filename=str(filename), path=str(filepath))
            except:
                logging.info('id_process: ' + id_process + ' -> Try gen resolution in uuid_doc_download module')
                json_to_pdf = {'in_date': str(doc.in_date),
                               'in_number': str(doc.number_input),
                               'sender': str(doc.sender),
                               'local_date': str(doc.output_date),
                               'local_number': str(doc.number_output),
                               'resolution': doc.resolution,
                               'special_notes': str_to_array_elements(doc.special_notes, "', '"),
                               'nii_head_uuid': str(
                                   (json.load(open('.Deploy/config_files/permissions.json', 'r'))['director'])),
                               'signatures_path': str(
                                   (json.load(open('.Deploy/config_files/settings.json.json', 'r'))['signatures_path'])),
                               'execution_mark': False,
                               'execution_mark_number': '',
                               'execution_mark_date': '',
                               'uuid_doc': doc.uuid_doc
                               }
                # creating_pdf = create_new_pdf(json_pdf=json_to_pdf,
                #                               uuid_doc=doc.uuid_doc,
                #                               id_process=id_process)
                session_uuid_doc_download.close()
                return FileResponse(filename=str(filename), path=str(f'./clients_files/{filename}'))
    except:
        logging.info('id_process: ' + id_process + ' -> Return doc without resolution')
        return FileResponse(filename=str(filename), path=str(f'./Deploy/clients_files/{filename}'))


def uuid_doc_download_project(uuid_doc, id_process, ip):
    logging.info('id_process: ' + id_process + ' -> Start process uuid_doc_download_project from ip: ' + ip)
    uuid_doc_download_project_session = sessionmaker(bind=create_engine("sqlite:///databases/control_doc.db"))
    uuid_doc_download_project_session.configure(bind=create_engine("sqlite:///databases/control_doc.db"))
    uuid_doc_download_project_session = uuid_doc_download_project_session()
    for doc in (uuid_doc_download_project_session.query(ProjectsTable).filter(
            ProjectsTable.uuid == str(uuid_doc)).all()):
        filepath = doc.path
        filename = str(doc.path).split('/')[-1]
        try:
            open(str(filepath))
            logging.info('id_process: ' + id_process + ' -> Process uuid_doc_download done')
            return FileResponse(filename=str(filename), path=str(filepath))
        except:
            logging.info('id_process: ' + id_process + ' -> Process uuid_doc_download ERROR')
            return {'process': 'Error. Can`t found the file'}
