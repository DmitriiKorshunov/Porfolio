
from Deploy.features.update_exist_doc import *
from Deploy.features.parsing_user_once import *
from Deploy.features.parsing_users import *
import uuid, datetime, logging, json
from sqlalchemy.orm import *
from sqlalchemy import *

Base = declarative_base()


def update_docs_list(docs, id_process, ip, type_return):
    # try:
    logging.info(f'id_process: {id_process}  -> Start process update_docs_list with "{type_return}" return type. '
                 f'\nuser: {docs.uuid_user} \nIP: {ip}')
    num_items_added = 0
    num_items_update = 0
    num_items_full_update = 0
    error_docs = list()
    uuids_doc = list()
    error_creating = list()
    time_start = datetime.datetime.now()
    session_update_docs_list = sessionmaker(bind=create_engine("sqlite:///Deploy/databases/main.db"))
    session_update_docs_list.configure(bind=create_engine("sqlite:///Deploy/databases/main.db"))
    session_update_docs_list = session_update_docs_list()
    for json_doc_info in docs.docs:
        name = None
        for user_name in list_all_users_json()['users']:
            if user_name['uuid_user'] == str(json_doc_info['owner']):
                name = user_name['name']
                pass
        if len(session_update_docs_list.query(DocsTable).filter(
                DocsTable.uuid_doc == str(json_doc_info['uuid_doc'])).all()) == 0:
            if json_doc_info['uuid_doc'] == "":
                uuid_doc_new = uuid.uuid4()
                uuids_doc.append(str(uuid_doc_new))
                session_update_docs_list.add(DocsTable(uuid_doc=str(uuid_doc_new),
                                                       name=str(json_doc_info['name']),
                                                       path=str(f"./gen_pdf_files/{uuid_doc_new}.pdf"),
                                                       user_address=str(json_doc_info["user_address"]),
                                                       special_notes=str(json_doc_info["special_notes"]),
                                                       sender=str(json_doc_info["sender"]),
                                                       resolution=json.dumps(json_doc_info["resolution"]),
                                                       receive_flag=str(json_doc_info["receive_flag"]),
                                                       read_flag=str(json_doc_info["read_flag"]),
                                                       owner=str(json_doc_info["owner"]),
                                                       output_date=str(json_doc_info["output_date"]),
                                                       number_output=str(json_doc_info["number_output"]),
                                                       number_input=str(json_doc_info["number_input"]),
                                                       log=json.dumps(json_doc_info["log"]),
                                                       in_date=str(json_doc_info["in_date"]),
                                                       date_upload=str(json_doc_info["date_upload"]),
                                                       address_flag=str(json_doc_info["address_flag"]),
                                                       archive_flag=json_doc_info["archive_flag"]
                                                       ))
                json_to_pdf = {'in_date': str(json_doc_info["in_date"]),
                               'in_number': str(json_doc_info["number_input"]),
                               'sender': str(json_doc_info["sender"]),
                               'local_date': str(json_doc_info["output_date"]),
                               'local_number': str(json_doc_info["number_output"]),
                               'resolution': json_doc_info["resolution"],
                               'special_notes': str_to_array_elements(json_doc_info["special_notes"], "', '"),
                               'nii_head_uuid': str(
                                   (json.load(open('./config_files/permissions.json', 'r'))['director'])),
                               'signatures_path': str(
                                   (json.load(open('./config_files/settings.json', 'r'))['signatures_path'])),
                               'execution_mark': False,
                               'execution_mark_number': '',
                               'execution_mark_date': '',
                               'uuid_doc': str(uuid_doc_new),
                               }
                # creating_pdf = create_new_pdf(json_pdf=json_to_pdf,
                #                               uuid_doc=uuid_doc_new,
                #                               id_process=id_process)
                # if creating_pdf:
                #     num_items_added += 1
                #     session_update_docs_list.commit()
                #
                # else:
                #     error_creating.append(json_doc_info['name'])
                # for user in json_doc_info["user_address"]:
                #     send_notifications(uuid_user=user,
                #                        message_text=f"Вам направлен документ: "
                #                                     f"{json_doc_info['name']}. Отправитель: {name}")
            else:
                uuids_doc.append(str(json_doc_info['uuid_doc']))
                session_update_docs_list.add(DocsTable(uuid_doc=str(json_doc_info['uuid_doc']),
                                                       name=str(json_doc_info['name']),
                                                       path=str(f"./gen_pdf_files/{json_doc_info['uuid_doc']}.pdf"),
                                                       user_address=str(json_doc_info["user_address"]),
                                                       special_notes=str(json_doc_info["special_notes"]),
                                                       sender=str(json_doc_info["sender"]),
                                                       resolution=json.dumps(json_doc_info["resolution"]),
                                                       receive_flag=str(json_doc_info["receive_flag"]),
                                                       read_flag=str(json_doc_info["read_flag"]),
                                                       owner=str(json_doc_info["owner"]),
                                                       output_date=str(json_doc_info["output_date"]),
                                                       number_output=str(json_doc_info["number_output"]),
                                                       number_input=str(json_doc_info["number_input"]),
                                                       log=json.dumps(json_doc_info["log"]),
                                                       in_date=str(json_doc_info["in_date"]),
                                                       date_upload=str(json_doc_info["date_upload"]),
                                                       address_flag=str(json_doc_info["address_flag"]),
                                                       archive_flag=json_doc_info["archive_flag"]
                                                       ))
                json_to_pdf = {'in_date': str(json_doc_info["in_date"]),
                               'in_number': str(json_doc_info["number_input"]),
                               'sender': str(json_doc_info["sender"]),
                               'local_date': str(json_doc_info["output_date"]),
                               'local_number': str(json_doc_info["number_output"]),
                               'resolution': json_doc_info["resolution"],
                               'special_notes': str_to_array_elements(json_doc_info["special_notes"], "', '"),
                               'nii_head_uuid': str(
                                   (json.load(open('./config_files/permissions.json', 'r'))['director'])),
                               'signatures_path': str(
                                   (json.load(open('./config_files/settings.json', 'r'))['signatures_path'])),
                               'execution_mark': False,
                               'execution_mark_number': '',
                               'execution_mark_date': '',
                               'uuid_doc': str(json_doc_info['uuid_doc']),
                               }
                # creating_pdf = create_new_pdf(json_pdf=json_to_pdf,
                #                               uuid_doc=json_doc_info['uuid_doc'],
                #                               id_process=id_process)
                # if creating_pdf:
                #     num_items_added += 1
                #     session_update_docs_list.commit()
                # else:
                #     error_creating.append(json_doc_info['uuid_doc'])
                # for user in json_doc_info["user_address"]:
                #     send_notifications(uuid_user=user,
                #                        message_text=f"Вам направлен документ: "
                #                                     f"{json_doc_info['name']}. Отправитель: {name}")
        else:
            uuids_doc.append(str(json_doc_info['uuid_doc']))
            resolutions, \
            special_notes, \
            update_status, \
            new_user_addresses = update_exist_doc(doc=json_doc_info,
                                                uuid_user=docs.uuid_user,
                                                session_sql=session_update_docs_list)
            if resolutions and special_notes:
                json_to_pdf = {'in_date': str(json_doc_info["in_date"]),
                               'in_number': str(json_doc_info["number_input"]),
                               'sender': str(json_doc_info["sender"]),
                               'local_date': str(json_doc_info["output_date"]),
                               'local_number': str(json_doc_info["number_output"]),
                               'resolution': resolutions,
                               'special_notes': str_to_array_elements(special_notes, "', '"),
                               'nii_head_uuid': str(("pass")),
                               'signatures_path': str(("pass")),
                               'execution_mark': False,
                               'execution_mark_number': '',
                               'execution_mark_date': '',
                               'uuid_doc': str(json_doc_info['uuid_doc']),
                               }
                # creating_pdf = create_new_pdf(json_pdf=json_to_pdf,
                #                               uuid_doc=json_doc_info['uuid_doc'],
                #                               id_process=id_process)
                # if creating_pdf:
                #     num_items_update += 1
                #     session_update_docs_list.commit()
                # else:
                #     error_creating.append(json_doc_info['uuid_doc'])


            elif update_status:
                num_items_update += 1
                session_update_docs_list.commit()
            # if result_update['type_update'] == 'full':
            #     num_items_full_update += 1
            # elif result_update['type_update'] == 'smart':
            #     num_items_update += 1
            # else:
            #     error_docs.append(result_update['type_update'])

            # for user in new_user_addresses:
            #     send_notifications(uuid_user=user,
            #                        message_text=f"Вам направлен документ: "
            #                                     f"{json_doc_info['name']}. Отправитель: {name}")


    error_status = 'None'
    if not len(error_docs) == 0:
        error_status = f'uuid_doc: {error_docs}'
    if len(error_creating) == 0:
        error_creating = 'None'
    else:
        error_creating = f'uuid_doc: {error_creating}'

    logging.info(f'id_process: {id_process} -> Result process:'
                 f'\nadded: {num_items_added} \nsmart_update: {num_items_update}'
                 f'\nfull_update: {num_items_full_update}'
                 f'\nerror: {error_status}'
                 f'\ntime: {datetime.datetime.now() - time_start}'
                 f' error_pdf: {error_creating}')
    if type_return == 'process':
        session_update_docs_list.close()
        return {
            'process': {'added': num_items_added,
                        'smart_update': num_items_update,
                        'full_update': num_items_full_update,
                        'error': error_status,
                        'time': str(datetime.datetime.now() - time_start),
                        'error_pdf': error_creating,
                        'count': docs.docs_count}}
    elif type_return == 'uuid_doc':
        session_update_docs_list.close()
        return {'uuid_doc': uuid_doc_new}
    else:
        session_update_docs_list.close()
        logging.warning('Result process ' + str(id_process) + ':  error. Not match return_type.')
        return {'process': 'error. type for return not available'}

#
# except:
#     logging.warning('Result process ' + str(id_process) + ':  error. Expect.')
#     return {'process': 'error'}
