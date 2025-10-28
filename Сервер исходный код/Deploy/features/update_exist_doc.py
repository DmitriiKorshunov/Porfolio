from Deploy.features.parsing_docs import *

Base = declarative_base()


def update_exist_doc(doc, uuid_user, session_sql):
    # try:
    #     if uuid_user in (json.load(open('./config_files/permissions.json', 'r'))['admin']):
    #         return {'type_update': 'full'}
    #     elif uuid_user in (json.load(open('./config_files/permissions.json', 'r'))['developer']):
    #         session_sql.query(DocsTable).filter(DocsTable.uuid_doc == doc['uuid_doc']). \
    #             update({"archive_flag": doc['archive_flag'],
    #                     "address_flag": str(doc['address_flag']),
    #                     "date_upload": str(doc['date_upload']),
    #                     "in_date": doc['in_date'],
    #                     "log": json.dumps(doc['log']),
    #                     "name": doc['name'],
    #                     "number_input": doc['number_input'],
    #                     "number_output": doc['number_output'],
    #                     "output_date": doc['output_date'],
    #                     "owner": doc['owner'],
    #                     "read_flag": str(doc['read_flag']),
    #                     "receive_flag": str(doc['receive_flag']),
    #                     "resolution": json.dumps(doc['resolution']),
    #                     "sender": doc['sender'],
    #                     "special_notes": str(doc['special_notes']),
    #                     "user_address": str(doc['user_address'])
    #                     })
    #         return {'type_update': 'full'}
    #     else:
    doc_update_status = False
    doc_update_resolution = False
    doc_update_special_notes = False
    for item in (session_sql.query(DocsTable).filter(DocsTable.uuid_doc == str(doc['uuid_doc'])).all()):
        resolutions_bd = json.loads(item.resolution)
        resolutions_post = doc['resolution']
        read_flags = str_to_array_elements(item.read_flag, "', '")
        read_flag = False
        for new_author in list(set(resolutions_post.keys()) - set(resolutions_bd)):
            doc_update_status = True
            doc_update_resolution = True
            resolutions_bd[new_author] = resolutions_post[new_author]
            if not new_author in read_flags:
                read_flag = True
                read_flags.append(new_author)
        if read_flag:
            session_sql.query(DocsTable).filter(DocsTable.uuid_doc == str(doc['uuid_doc'])). \
                update({'resolution': json.dumps(resolutions_bd),
                        'read_flag': str(read_flags)})
        else:
            session_sql.query(DocsTable).filter(DocsTable.uuid_doc == str(doc['uuid_doc'])). \
                update({'resolution': json.dumps(resolutions_bd)})

        log_bd = json.loads(item.log)
        log_post = doc['log']
        for new_time in list(set(log_post.keys()) - set((log_bd).keys())):
            doc_update_status = True
            log_bd[str(new_time)] = json.dumps(log_post[new_time])
        session_sql.query(DocsTable).filter(DocsTable.uuid_doc == str(doc['uuid_doc'])). \
            update({'log': json.dumps(log_bd)})
        user_address_bd = str_to_array_elements(item.user_address, "', '")
        user_address_post = doc['user_address']
        user_address_notif = list(set(user_address_post) - set(user_address_bd))
        session_sql.query(DocsTable).filter(DocsTable.uuid_doc == str(doc['uuid_doc'])). \
            update({'user_address': str(list(set(user_address_post) | set(user_address_bd)))})

        address_flag_bd = item.address_flag
        address_flag_post = doc['address_flag']
        if not (len(list((set(address_flag_post) - set(address_flag_bd)))) == 0):
            doc_update_status = True
        flags_handler(flags_post=address_flag_post, flags_bd=address_flag_bd, uuid_doc=str(doc['uuid_doc']),
                      session_sql=session_sql, name='address_flag')

        receive_flag_bd = item.receive_flag
        receive_flag_post = doc['receive_flag']
        if not (len(list((set(receive_flag_post) - set(receive_flag_bd)))) == 0):
            doc_update_status = True
        flags_handler(flags_post=receive_flag_post, flags_bd=receive_flag_bd, uuid_doc=str(doc['uuid_doc']),
                      session_sql=session_sql, name='receive_flag')

        read_flag_bd = item.read_flag
        read_flag_post = doc['read_flag']
        if not (len(list((set(read_flag_post) - set(read_flag_bd)))) == 0):
            doc_update_status = True
        flags_handler(flags_post=read_flag_post, flags_bd=read_flag_bd, uuid_doc=str(doc['uuid_doc']),
                      session_sql=session_sql, name='read_flag')

        special_notes_bd = item.special_notes
        special_notes_post = doc['special_notes']
        if not (len(list((set(special_notes_post) - set(special_notes_bd)))) == 0):
            doc_update_status = True
            doc_update_special_notes = True
        special_notes = str(flags_handler(flags_post=special_notes_post, flags_bd=special_notes_bd, uuid_doc=str(doc['uuid_doc']),
                      session_sql=session_sql, name='special_notes'))

        session_sql.query(DocsTable).filter(DocsTable.uuid_doc == str(doc['uuid_doc'])). \
            update({'archive_flag': doc['archive_flag']})

        session_sql.commit()

        """
        Old mark
        
                if doc_update_resolution:
            return resolutions_bd, special_notes_bd, doc_update_status, user_address_notif
        elif doc_update_special_notes:
            return resolutions_bd, special_notes_bd, doc_update_status, user_address_notif
        elif doc_update_status:
            return resolutions_bd, special_notes_bd, doc_update_status, user_address_notif
        else:
            return False, False, False, user_address_notif
            
        """
        if doc_update_resolution:
            return resolutions_bd, special_notes, doc_update_status, user_address_notif
        elif doc_update_special_notes:
            return resolutions_bd, special_notes, doc_update_status, user_address_notif
        elif doc_update_status:
            return resolutions_bd, special_notes, doc_update_status, user_address_notif
        else:
            return False, False, False, user_address_notif


# except:
#     return {'type_update': doc['uuid_doc']}


def flags_handler(flags_post, flags_bd, session_sql, name, uuid_doc):
    if flags_bd == "[]":
        session_sql.query(DocsTable).filter(DocsTable.uuid_doc == str(uuid_doc)). \
            update({str(name): str(flags_post)})
        return flags_post
    else:
        session_sql.query(DocsTable).filter(DocsTable.uuid_doc == str(uuid_doc)). \
            update({str(name): str(list(set(str_to_array_elements(flags_bd, "', '"))
                                        | set(flags_post)))})
        return str(list(set(str_to_array_elements(flags_bd, "', '"))
                        | set(flags_post)))
