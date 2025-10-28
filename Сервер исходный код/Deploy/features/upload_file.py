from Deploy.features.parsing_docs import DocsTable
from Deploy.features.update_projects_list import ProjectsTable
from Deploy.features.parsing_users import list_all_users_json
from Deploy.features.make_pdf import create_new_pdf
from sqlalchemy.orm import *
from sqlalchemy import *
import json, uuid, logging


def create_new_file_pdf(files, id_process, ip):
    # try:
        info_file = json.loads(files[0].file.read())['docs']
        if len(files) == 2:
            file_obj = files[1].file.read()
            logging.info('id_process: ' + id_process + ' -> Start process create_new_file_pdf ' +
                         '\nuser: ' + str('0') + '\nIP: ' + ip)
            create_new_file_pdf_session = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
            create_new_file_pdf_session.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
            create_new_file_pdf_session = create_new_file_pdf_session()
            logging.info('id_process: ' + id_process + ' -> Creating file... ')
            if len(info_file['uuid_doc']) == 0:
                uuid_file = uuid.uuid4()
                path_file = './Deploy/clients_files/' + str(uuid_file) + '.' + str(files[1].filename.split('.')[-1])
                try:
                    open(path_file, 'wb').write(file_obj)
                except:
                    return {'process': 'Error with writing new file'}
                json_to_pdf = {'in_date': str(info_file['in_date']),
                               'in_number': str(info_file['number_input']),
                               'sender': str(info_file['sender']),
                               'local_date': str(info_file['output_date']),
                               'local_number': str(info_file['number_output']),
                               'resolution': info_file['resolution'],
                               'special_notes': info_file['special_notes'],
                               'nii_head_uuid': str("pass"),
                               'signatures_path': str("pass"),
                               'execution_mark': False,
                               'execution_mark_number': '',
                               'execution_mark_date': '',
                               'uuid_doc': str(uuid_file)
                               }
                # creating_pdf = create_new_pdf(json_pdf=json_to_pdf, id_process=id_process, uuid_doc=uuid_file)
                creating_pdf = True
                if creating_pdf:
                    logging.info('id_process: ' + id_process + ' -> File added. \nname: ' + str(uuid_file))
                else:
                    logging.info('id_process: ' + id_process + ' -> PDF error. \nname: ' + str(uuid_file))
                    return {'error': 'Error. not creating PDF ' + str(uuid_file)}
                create_new_file_pdf_session.add(DocsTable(uuid_doc=str(uuid_file),
                                                          name=str(info_file['name']),
                                                          path=f'./gen_pdf_files/{uuid_file}.{files[1].filename.split(".")[-1]}',
                                                          user_address=str(info_file["user_address"]),
                                                          special_notes=str(info_file["special_notes"]),
                                                          sender=str(info_file["sender"]),
                                                          resolution=json.dumps(info_file["resolution"]),
                                                          receive_flag=str(info_file["receive_flag"]),
                                                          read_flag=str(info_file["read_flag"]),
                                                          owner=str(info_file["owner"]),
                                                          output_date=str(info_file["output_date"]),
                                                          number_output=str(info_file["number_output"]),
                                                          number_input=str(info_file["number_input"]),
                                                          log=json.dumps(info_file["log"]),
                                                          in_date=str(info_file["in_date"]),
                                                          date_upload=str(info_file["date_upload"]),
                                                          address_flag=str(info_file["address_flag"]),
                                                          archive_flag=info_file["archive_flag"]
                                                          ))
                create_new_file_pdf_session.commit()
                create_new_file_pdf_session.close()
                name = ''
                for user_name in list_all_users_json()['users']:
                    if user_name['uuid_user'] == str(info_file['owner']):
                        name = user_name['name']
                        pass

                return {'process': 'Create new doc ' + str(uuid_file)}
            else:
                try:
                    uuid_file = info_file['uuid_doc']
                    path_file = './clients_files/' + str(uuid_file) + '.' + str(files[1].filename.split('.')[-1])
                    try:
                        open(path_file, 'wb').write(file_obj)
                    except:
                        return {'process': 'Error with writing new file'}
                    logging.info('id_process: ' + id_process + ' -> Start process create_new_file_pdf ' +
                                 '\nuser: ' + str('0') + '\nIP: ' + ip)
                    create_new_file_pdf_session = sessionmaker(bind=create_engine("sqlite:///databases/main.db"))
                    create_new_file_pdf_session.configure(bind=create_engine("sqlite:///databases/main.db"))
                    create_new_file_pdf_session = create_new_file_pdf_session()
                    logging.info('id_process: ' + id_process + ' -> Creating file... ')
                    path_file = './clients_files/' + str(uuid_file) + '.' + str(files[1].filename.split('.')[-1])
                    try:
                        open(path_file, 'wb').write(file_obj)
                    except:
                        return {'process': 'Error with writing new file'}
                    json_to_pdf = {'in_date': str(info_file['in_date']),
                                   'in_number': str(info_file['number_input']),
                                   'sender': str(info_file['sender']),
                                   'local_date': str(info_file['output_date']),
                                   'local_number': str(info_file['number_output']),
                                   'resolution': info_file['resolution'],
                                   'special_notes': info_file['special_notes'],
                                   'nii_head_uuid': str(
                                       (json.load(open('./config_files/permissions.json', 'r'))['director'])),
                                   'signatures_path': str(
                                       (json.load(open('./config_files/settings.json.json', 'r'))['signatures_path'])),
                                   'execution_mark': False,
                                   'execution_mark_number': '',
                                   'execution_mark_date': '',
                                   'uuid_doc': info_file['uuid_doc']
                                   }
                    creating_pdf = create_new_pdf(json_pdf=json_to_pdf, id_process=id_process, uuid_doc=uuid_file)
                    if creating_pdf:
                        logging.info('id_process: ' + id_process + ' -> File added. \nname: ' + str(uuid_file))
                    else:
                        logging.info('id_process: ' + id_process + ' -> PDF error. \nname: ' + str(uuid_file))
                        return {'error': 'Error. not creating PDF ' + str(uuid_file)}
                    create_new_file_pdf_session.query(DocsTable).filter(DocsTable.uuid_doc == info_file['uuid_doc']). \
                        update({
                        'name': str(info_file['name']),
                        'path': f'./gen_pdf_files/{uuid_file}.{files[1].filename.split(".")[-1]}',
                        'user_address': str(info_file["user_address"]),
                        'special_notes': str(info_file["special_notes"]),
                        'sender': str(info_file["sender"]),
                        'resolution': json.dumps(info_file["resolution"]),
                        'receive_flag': str(info_file["receive_flag"]),
                        'read_flag': str(info_file["read_flag"]),
                        'owner': str(info_file["owner"]),
                        'output_date': str(info_file["output_date"]),
                        'number_output': str(info_file["number_output"]),
                        'number_input': str(info_file["number_input"]),
                        'log': json.dumps(info_file["log"]),
                        'in_date': str(info_file["in_date"]),
                        'date_upload': str(info_file["date_upload"]),
                        'address_flag': str(info_file["address_flag"]),
                        'archive_flag': info_file["archive_flag"]
                    })
                    create_new_file_pdf_session.commit()
                    create_new_file_pdf_session.close()
                    return {'process': 'Create new doc and update ' + str(uuid_file) }
                except:
                    return {'process': 'error with code 2552'}

        else:
            json_to_pdf = {'in_date': str(info_file['in_date']),
                           'in_number': str(info_file['number_input']),
                           'sender': str(info_file['sender']),
                           'local_date': str(info_file['output_date']),
                           'local_number': str(info_file['number_output']),
                           'resolution': info_file['resolution'],
                           'special_notes': info_file['special_notes'],
                           'nii_head_uuid': str(
                               (json.load(open('./config_files/permissions.json', 'r'))['director'])),
                           'signatures_path': str(
                               (json.load(open('./config_files/settings.json', 'r'))['signatures_path'])),
                           'execution_mark': False,
                           'execution_mark_number': '',
                           'execution_mark_date': '',
                           'uuid_doc': info_file['uuid_doc']
                           }
            creating_pdf = create_new_pdf(json_pdf=json_to_pdf, id_process=id_process, uuid_doc=info_file["uuid_doc"])
            if creating_pdf:
                logging.info('id_process: ' + id_process + ' -> File added. \nname: ' + str(info_file["uuid_doc"]))
            else:
                logging.info('id_process: ' + id_process + ' -> PDF error. \nname: ' + str(info_file["uuid_doc"]))
                return {'error': 'Error. not creating PDF ' + str(info_file["uuid_doc"])}
            create_new_file_pdf_session = sessionmaker(bind=create_engine("sqlite:///databases/main.db"))
            create_new_file_pdf_session.configure(bind=create_engine("sqlite:///databases/main.db"))
            create_new_file_pdf_session = create_new_file_pdf_session()
            create_new_file_pdf_session.query(DocsTable).filter(DocsTable.uuid_doc == info_file['uuid_doc']). \
                update({
                'name': str(info_file['name']),
                'path': f'./gen_pdf_files/{info_file["uuid_doc"]}.pdf',
                'user_address': str(info_file["user_address"]),
                'special_notes': str(info_file["special_notes"]),
                'sender': str(info_file["sender"]),
                'resolution': json.dumps(info_file["resolution"]),
                'receive_flag': str(info_file["receive_flag"]),
                'read_flag': str(info_file["read_flag"]),
                'owner': str(info_file["owner"]),
                'output_date': str(info_file["output_date"]),
                'number_output': str(info_file["number_output"]),
                'number_input': str(info_file["number_input"]),
                'log': json.dumps(info_file["log"]),
                'in_date': str(info_file["in_date"]),
                'date_upload': str(info_file["date_upload"]),
                'address_flag': str(info_file["address_flag"]),
                'archive_flag': info_file["archive_flag"]
            })
            create_new_file_pdf_session.commit()
            create_new_file_pdf_session.close()
            for user_name in list_all_users_json()['users']:
                if user_name['uuid_user'] == str(info_file['owner']):
                    name = user_name['name']
                    pass
            for user in info_file['user_address']:
                send_notifications(uuid_user=user,
                                   message_text=f"Отредактирован новый документ: "
                                                f"{info_file['name']}. Отправитель: {name}")
            return {'process': 'Create new doc and update ' + str(info_file["uuid_doc"])}

    # except:
    #     logging.info('id_process: ' + id_process + ' -> expect. \nname: ' + str(info_file["uuid_doc"]))
    #     return {'error': 'Except. not creating PDF ' + str(info_file["uuid_doc"])}



def create_new_file_pdf_projects(files, id_process, ip):
    info_file = json.loads(files[0].file.read())
    file_obj = files[1].file.read()
    logging.info('id_process: ' + id_process + ' -> Start process create_new_file_pdf ' +
                 '\nuser: ' + str('0') + '\nIP: ' + ip)
    create_new_file_pdf_projects_session = sessionmaker(bind=create_engine("sqlite:///databases/control_doc.db"))
    create_new_file_pdf_projects_session.configure(bind=create_engine("sqlite:///databases/control_doc.db"))
    create_new_file_pdf_projects_session = create_new_file_pdf_projects_session()
    logging.info('id_process: ' + id_process + ' -> Creating file... ')
    path_file = f'./control_docs/{info_file["uuid"]}.{files[1].filename.split(".")[-1]}'
    try:
            open(path_file, 'wb').write(file_obj)
    except:
            logging.info('id_process: ' + id_process + ' -> Error with writing file ' +
                         '\nuser: ' + str('0') + '\nIP: ' + ip)
            return {'process': 'Error with writing new file'}
    create_new_file_pdf_projects_session.add(ProjectsTable(
                                                uuid=info_file['uuid'],
                                                localDate=info_file['localDate'],
                                                extDate=info_file['extDate'],
                                                deadLine=info_file['deadLine'],
                                                path=path_file,
                                                localNumber=info_file['localNumber'],
                                                kindOfDoc=info_file['kindOfDoc'],
                                                extNumber=info_file['extNumber'],
                                                about=info_file['about'],
                                                extDest=info_file['extDest'],
                                                worker=info_file['worker'],
                                                outNumber=info_file['outNumber'],
                                                flagControl=info_file['flagControl'],
                                                VAMTOinpNumber=info_file['VAMTOinpNumber'],
                                                note=info_file['note']))
    create_new_file_pdf_projects_session.commit()
    create_new_file_pdf_projects_session.close()
    logging.info('id_process: ' + id_process + ' -> Start process create_new_file_pdf done!' +
                 '\nuser: ' + str('0') + '\nIP: ' + ip)
    return {'process': f'process with {info_file["uuid"]} doc is done'}