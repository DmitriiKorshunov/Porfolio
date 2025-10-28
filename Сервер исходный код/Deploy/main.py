import uvicorn, multiprocessing
from fastapi import FastAPI, Request, UploadFile, File
from pydantic import BaseModel
from Deploy.features.update_projects_list import *
from Deploy.features.parsing_user_once import *
from Deploy.features.parsing_docs_once import *
from Deploy.features.application_upgrade import *
from Deploy.features.download_file import *
from Deploy.features.get_actuality_ver import *
from Deploy.features.gen_id_ws import *
from Deploy.features.make_pdf import *
from Deploy.features.upload_file import *
from sqlalchemy.ext.declarative import declarative_base
from typing import List

app: FastAPI = FastAPI()

logging.basicConfig(filename=os.path.join(f'Deploy/logs/main/{time.time()}_logfile.log'),
                    filemode="w",
                    format="%(levelname)s %(asctime)s - %(message)s",
                    level=logging.INFO)
logging.path = '../logs'
logger = logging.getLogger()


class POSTNewUser(BaseModel):
    name: str
    password: bytes
    sender: str
    end_resolution: str
    category: str
    post_position: str

class POSTAuth(BaseModel):
        name: str
        password: bytes



class POSTNewDoc(BaseModel):
    uuid_doc: str
    name: str
    path: str
    user_address: list
    special_notes: list
    sender: str
    resolution_sender: list
    resolution: list
    receive_flag: list
    read_flag: list
    owner: str
    output_date: str
    number_output: str
    number_input: str
    log: list
    last_editing_time: str
    in_date: str
    editing_time: str
    address_flag: list
    archive_flag: bool


class POSTUpdateProjects(BaseModel):
    docs: list


class POSTUserOnce(BaseModel):
    users: list


class POSTDocsOnce(BaseModel):
    docs: list
    uuid_user: str
    docs_count: str


"""
GET

urls:
1) /check/ - url for check to available server
2) /api/v1/get/user/all - url for get list of all users
3) /api/v1/get/doc/all - url for get list of all docs
4) /api/v1/get/user/{uuidgetuser} - url for get information about user from uuid
5) /api/v1/get/doc/{uuidgetdoc} - url for get information about doc from uuid
6) /api/v1/delete/user/{uuiddeleteuser} - url for delete information about doc from uuid
7) /api/v1/get/admin/projects - url for get information about all projects (admin option) 
8) /api/v1/get/admin/file - url for get file (admin option in development)
"""


@app.get("/check/")
def test_connect():
    id_process = ''.join((str(time.time())).split('.'))
    logging.info('id_process: ' + id_process + ' Test -> Server is available')


@app.get("/api/v1/get/user/all")
def get_user_all():
    return list_all_users_json()


@app.get("/api/v1/get/doc/all")
def get_user_all():
    return list_all_docs_json()


@app.get("/api/v1/get/user/{uuidgetuser}")
def get_user(uuidgetuser):
    return uuid_user_json(str(uuidgetuser))


@app.get("/api/v1/get/doc/{uuidgetdoc}")
def get_user(uuidgetdoc):
    return uuid_docs_json(str(uuidgetdoc))


# @app.get("/api/v1/delete/user/{uuiddeleteuser}")
# def delete_new_user(uuiddeleteuser):
#     return delete_exist_user(uuiddeleteuser)


# дописать удаление
@app.get("/api/v1/delete/doc/{uuidgetdoc}")
def delete_doc(uuidgetdoc):
    return delete_exist_doc(uuidgetdoc)


@app.get("/api/v1/get/admin/projects")
def get_admin_projects():
    return list_all_projects_json()


@app.get("/ip")
def get_file(res: Request):
    return {'ip:': res.client.host}


@app.get("/api/v1/download/doc/{uuid_doc}")
def get_file(uuid_doc, req: Request):
    id_process = ''.join((str(time.time())).split('.'))
    return uuid_doc_download(uuid_doc, id_process, req.client.host)


@app.get("/api/v1/get/upgrade/versions")
def get_versions():
    return get_act_ver()


@app.get("/api/v1/get/upgrade/{application}")
def get_upgrade_version(application):
    return application_upgrade(application)


@app.get("/sednii")
def get_applications_zip():
    return get_applications()


"""
POST

urls: 
1) /api/v1/post/user - url for post once user
2) /api/v1/post/admin/update/projects - url for get information about
   project (admin option in development)
3) /api/v1/post/admin/update/docs - url for get information about
   doc (admin option in development)
4) /api/v1/post/admin/update/users - url for get information about
   user (admin option in development)
"""


@app.post("/api/v1/post/users")
def post_new_user(user: POSTNewUser, req: Request):
    id_process = ''.join((str(time.time())).split('.'))
    return update_users_list(user, id_process, req.client.host)


@app.post("/api/v1/post/docs")
def post_new_doc(doc: POSTDocsOnce, req: Request):
    id_process = ''.join((str(time.time())).split('.'))
    if not doc.docs_count == 0:
        print("/api/v1/post/docs done")
        return update_docs_list(doc, id_process, req.client.host, "process")
    else:
        print("'process': 'no files to update '")
        return {'process': 'no files to update '}


@app.post("/api/v1/post/admin/update/projects")
def post_new_doc(projects: POSTUpdateProjects):
    return update_projects_list(projects)


@app.post("/api/v1/post/doc")
def add_new_doc(doc: POSTDocsOnce, req: Request):
    id_process = ''.join((str(time.time())).split('.'))
    return {update_docs_list(doc, id_process, req.client.host, 'uuid_doc')}


@app.post("/api/v1/upload/doc")
async def create_upload_files(
        files: List[UploadFile] = File(..., description="Multiple files as UploadFile")):
    id_process = ''.join((str(time.time())).split('.'))
    return create_new_file_pdf(files=files, id_process=id_process, ip='0.0.0.0')



@app.post("/api/v1/auth")
async  def auth_user(user: POSTAuth):
    print(user)
    for u in list_all_users_json()['users']:
        if u["name"]==user.name:
            if u["password"]==str(user.password):
                print("checkpass")
                return  {"uuid": u["uuid_user"]}
    return 0




if __name__ == "__main__":
    multiprocessing.freeze_support()
    uvicorn.run("main:app", port=200, host="0.0.0.0")
