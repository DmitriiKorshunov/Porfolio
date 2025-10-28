from sqlalchemy import *
from sqlalchemy.orm import *
from sqlalchemy.ext.declarative import *
from parsing_docs import DocsTable
import uuid, logging, json, time

def create_doc_item(json_new_doc):
    session_create_doc_item = sessionmaker(bind=create_engine("sqlite:///databases/main.db"))
    session_create_doc_item.configure(bind=create_engine("sqlite:///databases/main.db"))
    session_create_doc_item = session_create_doc_item()
    create_doc_item.add(DocsTable(uuid_doc=str(uuid.uuid4()),
                                           name=str(json_new_doc['name']),
                                           path=str(json_new_doc["pdftk"]),
                                           user_address=str(json_new_doc["user_address"]),
                                           special_notes=str(json_new_doc["special_notes"]),
                                           sender=str(json_new_doc["sender"]),
                                           resolution=json.dumps(json_new_doc["resolution"]),
                                           receive_flag=str(json_new_doc["receive_flag"]),
                                           read_flag=str(json_new_doc["read_flag"]),
                                           owner=str(json_new_doc["owner"]),
                                           output_date=str(json_new_doc["output_date"]),
                                           number_output=str(json_new_doc["number_output"]),
                                           number_input=str(json_new_doc["number_input"]),
                                           log=json.dumps(json_new_doc["log"]),
                                           in_date=str(json_new_doc["in_date"]),
                                           date_upload=str(json_new_doc["date_upload"]),
                                           address_flag=str(json_new_doc["address_flag"]),
                                           archive_flag=json_new_doc["archive_flag"]
                                           ))
